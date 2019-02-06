#include <stdbool.h>
#include <hal.h>

#include "board_conf.h"
#include "tmp422.h"


static I2CDriver *tmp422_i2cDriver = I2C_DRIVER;
static __attribute__((aligned(32))) uint8_t tmp422_i2cTxBuffer[TMP422_TX_BUFFER_SIZE];
static __attribute__((aligned(32))) uint8_t tmp422_i2cRxBuffer[TMP422_RX_BUFFER_SIZE];
static bool tmp422_initDone = false;
static msg_t tmp422_i2cStatus;
static i2cflags_t tmp422_i2cErrors;
static bool tmp422_isPresent = false;
static uint8_t i2cAddress = IIC_TMP422_ADDR;
extern I2CConfig i2cConfig;

static void tmp422ProcessI2cError(void);

bool tmp422Init(void)
{
	chDbgCheck(tmp422_initDone != true);
	tmp422_initDone = true;

	i2cAcquireBus(tmp422_i2cDriver);

	for (;i2cAddress < IIC_TMP422_ADDR + 4; i2cAddress++){
		tmp422_i2cTxBuffer[0] = TMP422_DEVID_ADDR;
		cacheBufferFlush(tmp422_i2cTxBuffer, TMP422_TX_BUFFER_SIZE * sizeof(*tmp422_i2cTxBuffer));
		tmp422_i2cStatus = i2cMasterTransmitTimeout(tmp422_i2cDriver, i2cAddress,
													 tmp422_i2cTxBuffer, 1, tmp422_i2cRxBuffer, 1, TIME_MS2I(10));
		cacheBufferInvalidate(tmp422_i2cRxBuffer, TMP422_RX_BUFFER_SIZE * sizeof(*tmp422_i2cRxBuffer));

		if (tmp422_i2cStatus == MSG_OK) {
			tmp422_isPresent = true;
			break;
		}
	}

	if (!tmp422_isPresent){
		tmp422ProcessI2cError();
		return false;
	}

	if (*tmp422_i2cRxBuffer != TMP422_DEVID) {
		tmp422_isPresent = false;
		tmp422ProcessI2cError();
		return false;
	}

	i2cReleaseBus(tmp422_i2cDriver);

	return true;
}

float tmp422GetExtTemperature(uint8_t sensor_index)
{
	chDbgCheck(tmp422_initDone != false);

	i2cAcquireBus(tmp422_i2cDriver);

	tmp422_i2cTxBuffer[0] = TMP422_RTEMP1_ADDR + sensor_index;
	cacheBufferFlush(tmp422_i2cTxBuffer, TMP422_TX_BUFFER_SIZE * sizeof(*tmp422_i2cTxBuffer));
	tmp422_i2cStatus = i2cMasterTransmitTimeout(tmp422_i2cDriver, i2cAddress,
	                                             tmp422_i2cTxBuffer, 1, tmp422_i2cRxBuffer, 1, TIME_MS2I(10));

	if (tmp422_i2cStatus != MSG_OK) {
		tmp422ProcessI2cError();
		return -255;
	}

	tmp422_i2cTxBuffer[0] = TMP422_RTEMP1_LB_ADDR + sensor_index;
	cacheBufferFlush(tmp422_i2cTxBuffer, TMP422_TX_BUFFER_SIZE * sizeof(*tmp422_i2cTxBuffer));
	tmp422_i2cStatus = i2cMasterTransmitTimeout(tmp422_i2cDriver, i2cAddress,
	                                             tmp422_i2cTxBuffer, 1, &tmp422_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (tmp422_i2cStatus != MSG_OK) {
		tmp422ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(tmp422_i2cRxBuffer, TMP422_RX_BUFFER_SIZE * sizeof(*tmp422_i2cRxBuffer));

	uint8_t temp = tmp422_i2cRxBuffer[0];
	uint8_t ext_temp = tmp422_i2cRxBuffer[1];

	i2cReleaseBus(tmp422_i2cDriver);

	// checking diode fault flag
	if (ext_temp & 1)
		return 256;

	int8_t msb = tmp422_i2cRxBuffer[0];

	return ((float)msb + (float)(tmp422_i2cRxBuffer[1] >> 4) * 0.0625);
}

float tmp422GetLocalTemperature(void)
{
	chDbgCheck(tmp422_initDone != false);

	if (!tmp422_isPresent)
		return -254;

	i2cAcquireBus(tmp422_i2cDriver);

	*tmp422_i2cTxBuffer = TMP422_LTEMP_ADDR;
	cacheBufferFlush(tmp422_i2cTxBuffer, TMP422_TX_BUFFER_SIZE * sizeof(*tmp422_i2cTxBuffer));
	tmp422_i2cStatus = i2cMasterTransmitTimeout(tmp422_i2cDriver, i2cAddress,
	                                             tmp422_i2cTxBuffer, 1, &tmp422_i2cRxBuffer[0], 1, TIME_MS2I(10));

	if (tmp422_i2cStatus != MSG_OK) {
		tmp422ProcessI2cError();
		return -255;
	}

	*tmp422_i2cTxBuffer = TMP422_LTEMP_LB_ADDR;
	cacheBufferFlush(tmp422_i2cTxBuffer, TMP422_TX_BUFFER_SIZE * sizeof(*tmp422_i2cTxBuffer));
	tmp422_i2cStatus = i2cMasterTransmitTimeout(tmp422_i2cDriver, i2cAddress,
	                                             tmp422_i2cTxBuffer, 1, &tmp422_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (tmp422_i2cStatus != MSG_OK) {
		tmp422ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(tmp422_i2cRxBuffer, TMP422_RX_BUFFER_SIZE * sizeof(*tmp422_i2cRxBuffer));

	i2cReleaseBus(tmp422_i2cDriver);

	int8_t msb = tmp422_i2cRxBuffer[0];

	return ((float)msb + (float)(tmp422_i2cRxBuffer[1] >> 4) * 0.0625);
}

static void tmp422ProcessI2cError(void)
{
	tmp422_i2cErrors = i2cGetErrors(tmp422_i2cDriver);
	if (tmp422_i2cStatus == MSG_TIMEOUT)
		i2cStart(tmp422_i2cDriver, &i2cConfig);
	i2cReleaseBus(tmp422_i2cDriver);
}
