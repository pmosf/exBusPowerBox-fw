#include <string.h>
#include "board_conf.h"
#include "data_types.h"
#include "exBus.h"
#include "crc.h"
#include "usbcfg.h"
#include "servos.h"
#include "exDevice.h"
#include "jetiBox.h"

static bool exbus_initDone = false;

void exbus_init(exbus_t *ex)
{
	chDbgCheck(exbus_initDone != true);
	chDbgCheck(ex != NULL);

	memset(ex, 0, sizeof(*ex));

	jetiExInit();

	exbus_initDone = true;
}

/*
    0x3E 0x01 LEN Packet_ID 0x31 SUB_LEN Data_array CRC16      // Channel Data with telemetry request (2nd byte 0x01)
    0x3E 0x03 LEN Packet_ID 0x31 SUB_LEN Data_array CRC16      // Channel Data forbids answering (2nd byte 0x03)
    0x3D 0x01 0x08 Packet_ID 0x3A 0x00 CRC16                   // Telemetry Request EX telemetry (5th byte 0x3A)
    0x3D 0x01 0x09 Packet_ID 0x3B 0x01 0xF0 CRC16              // Jetibox request (5th byte 0x3B)
*/
bool exbus_decode(exbus_t *ex)
{
	chDbgCheck(exbus_initDone != false);

	switch (ex->state) {
	// header
	case 0:
		if (ex->rx_data == 0x3E || ex->rx_data == 0x3D) {
			ex->pkt.header[0] = ex->rx_data;
			ex->state++;
		}
		break;

	// header
	case 1:
		if (ex->rx_data == 0x01 || ex->rx_data == 0x03) {
			ex->pkt.header[1] = ex->rx_data;
			ex->state++;
		} else {
			ex->pkt.header[0] = 0;
			ex->pkt.header[1] = 0;
			ex->state = 0;
		}
		break;

	// packet length
	case 2:
		ex->pkt.pktLen = ex->rx_data;
		ex->state++;
		break;

	// packet ID
	case 3:
		ex->pkt.packetId = ex->rx_data;
		ex->state++;
		break;

	// data ID
	case 4:
		ex->pkt.dataId = ex->rx_data;
		ex->state++;
		break;

	// data length
	case 5:
		ex->pkt.dataLength = ex->rx_data;
		ex->state++;
		break;

	// data
	default:
		ex->pkt.data[ex->state - 6] = ex->rx_data;
		ex->state++;
		if (ex->state == ex->pkt.pktLen) {
			return true;
		}
		break;
	}

	return false;
}

bool exbus_check_crc(exbus_t *ex)
{
	chDbgCheck(exbus_initDone != false);

	uint16_t crc = (ex->pkt.data[ex->pkt.dataLength + 1] << 8)
	               | ex->pkt.data[ex->pkt.dataLength];

	if (crc != get_crc16((uint8_t *) &ex->pkt,
	                     ex->pkt.pktLen - 2))
		return !JETI_EXBUS_PKT_CRC_OK;
	else
		return JETI_EXBUS_PKT_CRC_OK;
}

void exbus_reset(exbus_t *ex)
{
	chDbgCheck(exbus_initDone != false);

	int8_t *p = (int8_t *) &ex->pkt;

	for (int i = 0; i < 4; ++i) {
		*p++ = 0;
	}

	ex->state = 0;
}

void exbusGetJetibox(exbus_packet_t *exbusPkt)
{
	exbusPkt->header[0] = 0x3B;
	exbusPkt->header[1] = 0x01;
	exbusPkt->pktLen = JETI_EXBUS_HEAD_CRC_LEN + 32;
	exbusPkt->dataId = JETI_EXBUS_ID_JETIBOX;
	exbusPkt->dataLength = 32;

	jetiboxProcessKey(exbusPkt->data[0], exbusPkt->data);

	uint16_t crc = get_crc16((const uint8_t *)exbusPkt,
		                         exbusPkt->pktLen - 2);

	exbusPkt->data[exbusPkt->dataLength] = crc;
	exbusPkt->data[exbusPkt->dataLength + 1] = crc >> 8;
}

void exbusGetNextTxtPkt(exbus_packet_t *exbusPkt)
{
	ex_pkt_t *exPkt = (ex_pkt_t *) exbusPkt->data;

	exbusPkt->header[0] = 0x3B;
	exbusPkt->header[1] = 0x01;
	exbusPkt->dataId = JETI_EXBUS_ID_TELEMETRY;
	exbusPkt->dataLength = jetiExGetNextTxtFrame(exPkt);
	exbusPkt->pktLen = exbusPkt->dataLength +
	                   JETI_EXBUS_HEAD_CRC_LEN;

	uint16_t crc = get_crc16((const uint8_t *)exbusPkt,
	                         exbusPkt->pktLen - 2);

	exbusPkt->data[exbusPkt->dataLength] = crc;
	exbusPkt->data[exbusPkt->dataLength + 1] = crc >> 8;
}

void exbusGetNextDataPkt(exbus_packet_t *exbusPkt)
{
	ex_pkt_t *exPkt = (ex_pkt_t *) exbusPkt->data;

	exbusPkt->header[0] = 0x3B;
	exbusPkt->header[1] = 0x01;
	exbusPkt->dataId = JETI_EXBUS_ID_TELEMETRY;
	exbusPkt->dataLength = jetiExGetNextDataPkt(exPkt);
	exbusPkt->pktLen = exbusPkt->dataLength +
	                   JETI_EXBUS_HEAD_CRC_LEN;

	uint16_t crc = get_crc16((const uint8_t *)exbusPkt,
	                         exbusPkt->pktLen - 2);

	exbusPkt->data[exbusPkt->dataLength] = crc;
	exbusPkt->data[exbusPkt->dataLength + 1] = crc >> 8;
}
