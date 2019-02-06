#include <stdbool.h>
#include <hal.h>

#include "board_conf.h"
#include "MS561101BA03.h"


static I2CDriver *ms5611_i2cDriver = I2C_DRIVER;
static __attribute__((aligned(32))) uint8_t ms5611_i2cTxBuffer[MS5611_TX_BUFFER_SIZE];
static __attribute__((aligned(32))) uint8_t ms5611_i2cRxBuffer[MS5611_RX_BUFFER_SIZE];
static bool ms5611_initDone = false;
static msg_t ms5611_i2cStatus;
static i2cflags_t ms5611_i2cErrors;
static bool ms5611_isPresent = false;
static uint8_t i2cAddress = IIC_MS5611_ADDR;
extern I2CConfig i2cConfig;

static void ms5611ProcessI2cError(void);

bool ms5611Init(void)
{
	chDbgCheck(ms5611_initDone != true);
	ms5611_initDone = true;

	i2cAcquireBus(ms5611_i2cDriver);

	for (;i2cAddress < IIC_MS5611_ADDR + 4; i2cAddress++){
		ms5611_i2cTxBuffer[0] = MS5611_DEVID_ADDR;
		cacheBufferFlush(ms5611_i2cTxBuffer, MS5611_TX_BUFFER_SIZE * sizeof(*ms5611_i2cTxBuffer));
		ms5611_i2cStatus = i2cMasterTransmitTimeout(ms5611_i2cDriver, i2cAddress,
													 ms5611_i2cTxBuffer, 1, ms5611_i2cRxBuffer, 1, TIME_MS2I(10));
		cacheBufferInvalidate(ms5611_i2cRxBuffer, MS5611_RX_BUFFER_SIZE * sizeof(*ms5611_i2cRxBuffer));

		if (ms5611_i2cStatus == MSG_OK) {
			ms5611_isPresent = true;
			break;
		}
	}

	if (!ms5611_isPresent){
		ms5611ProcessI2cError();
		return false;
	}

	if (*ms5611_i2cRxBuffer != MS5611_DEVID) {
		ms5611_isPresent = false;
		ms5611ProcessI2cError();
		return false;
	}

	i2cReleaseBus(ms5611_i2cDriver);

	return true;
}

float ms5611GetExtTemperature(uint8_t sensor_index)
{
	chDbgCheck(ms5611_initDone != false);

	i2cAcquireBus(ms5611_i2cDriver);

	ms5611_i2cTxBuffer[0] = MS5611_RTEMP1_ADDR + sensor_index;
	cacheBufferFlush(ms5611_i2cTxBuffer, MS5611_TX_BUFFER_SIZE * sizeof(*ms5611_i2cTxBuffer));
	ms5611_i2cStatus = i2cMasterTransmitTimeout(ms5611_i2cDriver, i2cAddress,
	                                             ms5611_i2cTxBuffer, 1, ms5611_i2cRxBuffer, 1, TIME_MS2I(10));

	if (ms5611_i2cStatus != MSG_OK) {
		ms5611ProcessI2cError();
		return -255;
	}

	ms5611_i2cTxBuffer[0] = MS5611_RTEMP1_LB_ADDR + sensor_index;
	cacheBufferFlush(ms5611_i2cTxBuffer, MS5611_TX_BUFFER_SIZE * sizeof(*ms5611_i2cTxBuffer));
	ms5611_i2cStatus = i2cMasterTransmitTimeout(ms5611_i2cDriver, i2cAddress,
	                                             ms5611_i2cTxBuffer, 1, &ms5611_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (ms5611_i2cStatus != MSG_OK) {
		ms5611ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(ms5611_i2cRxBuffer, MS5611_RX_BUFFER_SIZE * sizeof(*ms5611_i2cRxBuffer));

	uint8_t temp = ms5611_i2cRxBuffer[0];
	uint8_t ext_temp = ms5611_i2cRxBuffer[1];

	i2cReleaseBus(ms5611_i2cDriver);

	// checking diode fault flag
	if (ext_temp & 1)
		return 256;

	int8_t msb = ms5611_i2cRxBuffer[0];

	return ((float)msb + (float)(ms5611_i2cRxBuffer[1] >> 4) * 0.0625);
}

float ms5611GetLocalTemperature(void)
{
	chDbgCheck(ms5611_initDone != false);

	if (!ms5611_isPresent)
		return -254;

	i2cAcquireBus(ms5611_i2cDriver);

	*ms5611_i2cTxBuffer = MS5611_LTEMP_ADDR;
	cacheBufferFlush(ms5611_i2cTxBuffer, MS5611_TX_BUFFER_SIZE * sizeof(*ms5611_i2cTxBuffer));
	ms5611_i2cStatus = i2cMasterTransmitTimeout(ms5611_i2cDriver, i2cAddress,
	                                             ms5611_i2cTxBuffer, 1, &ms5611_i2cRxBuffer[0], 1, TIME_MS2I(10));

	if (ms5611_i2cStatus != MSG_OK) {
		ms5611ProcessI2cError();
		return -255;
	}

	*ms5611_i2cTxBuffer = MS5611_LTEMP_LB_ADDR;
	cacheBufferFlush(ms5611_i2cTxBuffer, MS5611_TX_BUFFER_SIZE * sizeof(*ms5611_i2cTxBuffer));
	ms5611_i2cStatus = i2cMasterTransmitTimeout(ms5611_i2cDriver, i2cAddress,
	                                             ms5611_i2cTxBuffer, 1, &ms5611_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (ms5611_i2cStatus != MSG_OK) {
		ms5611ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(ms5611_i2cRxBuffer, MS5611_RX_BUFFER_SIZE * sizeof(*ms5611_i2cRxBuffer));

	i2cReleaseBus(ms5611_i2cDriver);

	int8_t msb = ms5611_i2cRxBuffer[0];

	return ((float)msb + (float)(ms5611_i2cRxBuffer[1] >> 4) * 0.0625);
}

static void ms5611ProcessI2cError(void)
{
	ms5611_i2cErrors = i2cGetErrors(ms5611_i2cDriver);
	if (ms5611_i2cStatus == MSG_TIMEOUT)
		i2cStart(ms5611_i2cDriver, &i2cConfig);
	i2cReleaseBus(ms5611_i2cDriver);
}
