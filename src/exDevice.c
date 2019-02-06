#include <stdbool.h>
#include <string.h>
#include <ch.h>
#include "exDevice.h"
#include "jetiBox.h"
#include "crc.h"
#include "acquisition.h"

static bool init_done = false;

enum {
	EXSENSOR_TYPE_6b = 0, // int6_t  Data type 6b (-31 ¸31)
	EXSENSOR_TYPE_14b = 1, // int14_t Data type 14b (-8191 ¸8191)
	EXSENSOR_TYPE_22b = 4, // int22_t Data type 22b (-2097151 ¸2097151)
	EXSENSOR_TYPE_DT = 5, // int22_t Special data type – time and date
	EXSENSOR_TYPE_30b = 8, // int30_t Data type 30b (-536870911 ¸536870911)
	EXSENSOR_TYPE_GPS = 9, // int30_t Special data type – GPS coordinates:  lo/hi minute - lo/hi degree.
} EXSENSOR_DATATYPE;

static const uint8_t exDataTypeLen[] = {
	[EXSENSOR_TYPE_6b] = 1,
	[EXSENSOR_TYPE_14b] = 2,
	[EXSENSOR_TYPE_22b] = 3,
	[EXSENSOR_TYPE_DT] = 3,
	[EXSENSOR_TYPE_30b] = 4,
	[EXSENSOR_TYPE_GPS] = 4
};

///////////////////////////////////////////////////
// board dependent declarations
enum {
	ID_DEVICE,
	ID_VBAT1 = 1,
	ID_VBAT2,
	ID_IBAT1,
	ID_IBAT2,
	ID_CBAT1,
	ID_CBAT2,
	ID_TLOCAL,
	ID_TEXT1,
	ID_TEXT2,
	ID_RPM1,
	ID_RPM2
};

static exdevice_sensor_list_t device[] = {
	{ ID_DEVICE, "exBox", "", 0, 0 },
	{ ID_VBAT1, "V Bat1", "V", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(2) },
	{ ID_VBAT2, "V Bat2", "V", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(2) },
	{ ID_IBAT1, "I Bat1", "A", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(2) },
	{ ID_IBAT2, "I Bat2", "A", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(2) },
	{ ID_CBAT1, "C Bat1", "mAh", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(0) },
	{ ID_CBAT2, "C Bat2", "mAh", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(0) },
	{ ID_TLOCAL, "T Local", "°C", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(0) },
	{ ID_TEXT1, "T Ext1", "°C", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(0) },
	{ ID_TEXT2, "T Ext2", "°C", EXSENSOR_TYPE_14b, JETI_DECIMAL_MASK(0) },
	{ ID_RPM1, "Rpm1", "rpm", EXSENSOR_TYPE_22b, JETI_DECIMAL_MASK(0) },
	{ ID_RPM2, "Rpm2", "rpm", EXSENSOR_TYPE_22b, JETI_DECIMAL_MASK(0) }
};

static const uint8_t device_sn[] =  {0x00, 0xA4, 0x03, 0x00};

static int32_t getSensorValue(int sensor)
{
	switch (sensor) {
	case ID_VBAT1:
	case ID_VBAT2:
		return acqGetVoltage(sensor - ID_VBAT1);
	case ID_IBAT1:
	case ID_IBAT2:
		return acqGetCurrent(sensor - ID_IBAT1);
	case ID_CBAT1:
	case ID_CBAT2:
		return acqGetCapacity(sensor - ID_CBAT1);
	case ID_TLOCAL:
		return acqGetLocalTemp();
	case ID_TEXT1:
	case ID_TEXT2:
		return acqGetExtTemp(sensor - ID_TEXT1);
	case ID_RPM1:
	case ID_RPM2:
		return acqGetRpm(sensor - ID_RPM1);
	default:
		return -1;
	}
}

///////////////////////////////////////////////////

void jetiExInit(void)
{
	chDbgCheck(init_done != true);
	jetiboxInit();

	init_done = true;
}

uint8_t jetiExGetNextDataPkt(ex_pkt_t *pkt)
{
	static uint8_t sensor_idx = 1;
	uint8_t sensor_data_length;
	int32_t sensorValue;
	uint8_t pktLength;

	chDbgCheck(init_done != false);
	chDbgCheck(pkt != NULL);

#ifdef JETI_EXBUS
	pkt->header = JETI_SENSOR_EX_ID;
#else
	pkt->header[0] = JETI_SENSOR_HEADER;
	pkt->header[1] = JETI_SENSOR_EX_ID;
#endif

	for (int i = 0; i < 4; i++)
		pkt->sn[i] = device_sn[i];

	pkt->rsvd = 0;

	uint8_t *p = &pkt->id;

	while (true) {
		// data type and sensor ID
		if (sensor_idx > 15) {
			*p++ = device[sensor_idx].dataType;
			*p++ = sensor_idx;
		} else {
			*p++ = (sensor_idx << 4) | (device[sensor_idx].dataType);
		}
		// data
		sensorValue = getSensorValue(sensor_idx);
		sensor_data_length = exDataTypeLen[device[sensor_idx].dataType];
		while (sensor_data_length > 1) {
			*p++ = sensorValue;
			sensorValue >>= 8;
			sensor_data_length--;
		}
		if (device[sensor_idx].dataType != EXSENSOR_TYPE_GPS) {
			*p++ = (sensorValue & 0x9F) | device[sensor_idx].decimals;
		} else {
			*p++ = sensorValue;
		}

		sensor_idx++;
		if (sensor_idx > JETI_EX_SENSOR_COUNT) {
			sensor_idx = 1;
			break;
		}

		// +2: data type and sensor ID if sensor index > 15
		if ((p - &pkt->id) + exDataTypeLen[device[sensor_idx].dataType] + 2 >=
		    JETI_EX_DATA_LEN_MAX) {
			break;
		}
	}

	pktLength = JETI_SENSOR_DATA_LEN + p - &pkt->id;
	pkt->type_len = JETI_SENSOR_PKT_DATA_TYPE | pktLength;
	*p = get_crc8((uint8_t *) &
	              (pkt->type_len),
	              pktLength);

#ifdef JETI_EXBUS
	// CRC + 1 byte for header, 0x7E separator is omitted
	return pktLength + 2;
#else
	// CRC + 2 bytes for header
	return pktLength + 3;
#endif
}

uint8_t jetiExGetNextTxtFrame(ex_pkt_t *pkt)
{
	static uint8_t device_idx = 0;
	uint8_t labelLength;
	uint8_t unitLength;

	chDbgCheck(init_done != false);
	chDbgCheck(pkt != NULL);

	if (device_idx > JETI_EX_SENSOR_COUNT) {
		device_idx = 0;
	}

	labelLength = strlen(device[device_idx].label);
	unitLength = strlen(device[device_idx].unit);

#ifdef JETI_EXBUS
	pkt->header = JETI_SENSOR_EX_ID;
#else
	pkt->header[0] = JETI_SENSOR_HEADER;
	pkt->header[1] = JETI_SENSOR_EX_ID;
#endif

	pkt->type_len =
	    JETI_SENSOR_PKT_TXT_TYPE |
	    (JETI_SENSOR_TXT_LEN + labelLength +
	     unitLength);

	for (int i = 0; i < 4; i++)
		pkt->sn[i] = device_sn[i];

	pkt->rsvd = 0;

	pkt->id = device[device_idx].id;

	pkt->length = (labelLength << 3) + unitLength;

	chDbgCheck((labelLength + unitLength) < JETI_EX_DATA_LEN_MAX);

	memcpy(pkt->data, device[device_idx].label, labelLength);
	memcpy(&pkt->data[labelLength], device[device_idx].unit,
	       unitLength);

	pkt->data[labelLength + unitLength] = get_crc8((uint8_t *) &
	                                               (pkt->type_len),
	                                               pkt->type_len & 0x3F);

	device_idx++;

#ifdef JETI_EXBUS
	// CRC + 1 byte for header, 0x7E separator is omitted
	return pkt->type_len + 2;
#else
	// CRC + 2 bytes for header
	return pkt->type_len + 3;
#endif
}
