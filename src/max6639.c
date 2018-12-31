#include <stdbool.h>
#include <hal.h>

#include "board_conf.h"
#include "max6639.h"


static I2CDriver *max6639_i2cDriver = I2C_DRIVER;
static __attribute__((aligned(32))) uint8_t max6639_i2cTxBuffer[MAX6639_TX_BUFFER_SIZE];
static __attribute__((aligned(32))) uint8_t max6639_i2cRxBuffer[MAX6639_RX_BUFFER_SIZE];
static bool max6639_initDone = false;
static msg_t max6639_i2cStatus;
static i2cflags_t max6639_i2cErrors;
static bool max6639_isPresent = true;
extern I2CConfig i2cConfig;

static void max6639ProcessI2cError(void);

bool max6639Init(void)
{
	chDbgCheck(max6639_initDone != true);
	max6639_initDone = true;

	i2cAcquireBus(max6639_i2cDriver);

	max6639_i2cTxBuffer[0] = MAX6639_DEVID_ADDR;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, max6639_i2cRxBuffer, 1, TIME_MS2I(10));
	cacheBufferInvalidate(max6639_i2cRxBuffer, MAX6639_RX_BUFFER_SIZE * sizeof(*max6639_i2cRxBuffer));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return false;
	}

	if (*max6639_i2cRxBuffer != MAX6639_DEVICE_ID) {
		max6639_isPresent = false;
	}

	// set 2nd sensor as external
	max6639_i2cTxBuffer[0] = MAX6639_GCONF_ADDR;
	max6639_i2cTxBuffer[1] = 0x20;

	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 2, NULL, 0, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return false;
	}

	i2cReleaseBus(max6639_i2cDriver);

	return true;
}

float max6639GetExtTemperature(uint8_t sensor_index)
{
	chDbgCheck(max6639_initDone != false);

	i2cAcquireBus(max6639_i2cDriver);

	max6639_i2cTxBuffer[0] = MAX6639_TEMP1_ADDR + sensor_index;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, max6639_i2cRxBuffer, 1, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return -255;
	}

	max6639_i2cTxBuffer[0] = MAX6639_EXTENDED_TEMP1_ADDR + sensor_index;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, &max6639_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(max6639_i2cRxBuffer, MAX6639_RX_BUFFER_SIZE * sizeof(*max6639_i2cRxBuffer));

	uint8_t temp = max6639_i2cRxBuffer[0];
	uint8_t ext_temp = max6639_i2cRxBuffer[1];

	i2cReleaseBus(max6639_i2cDriver);

	// checking diode fault flag
	if (ext_temp & 1)
		return 256;

	return temp + (float)(ext_temp >> 5) * 0.125;
}

float max6639_get_local_temperature(void)
{
	chDbgCheck(max6639_initDone != false);

	if (!max6639_isPresent)
		return -254;

	i2cAcquireBus(max6639_i2cDriver);

// set 2nd sensor as local
	*max6639_i2cTxBuffer = 0x30;
	max6639_i2cTxBuffer[0] = MAX6639_GCONF_ADDR;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, NULL, 0, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return -255;
	}

	*max6639_i2cTxBuffer = MAX6639_TEMP2_ADDR;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, &max6639_i2cRxBuffer[0], 1, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return -255;
	}

	*max6639_i2cTxBuffer = MAX6639_EXT_TEMP2_ADDR;
	cacheBufferFlush(max6639_i2cTxBuffer, MAX6639_TX_BUFFER_SIZE * sizeof(*max6639_i2cTxBuffer));
	max6639_i2cStatus = i2cMasterTransmitTimeout(max6639_i2cDriver, IIC_MAX6639_ADDR,
	                                             max6639_i2cTxBuffer, 1, &max6639_i2cRxBuffer[1], 1, TIME_MS2I(10));

	if (max6639_i2cStatus != MSG_OK) {
		max6639ProcessI2cError();
		return -255;
	}

	cacheBufferInvalidate(max6639_i2cRxBuffer, MAX6639_RX_BUFFER_SIZE * sizeof(*max6639_i2cRxBuffer));

	i2cReleaseBus(max6639_i2cDriver);

	return (max6639_i2cRxBuffer[0] + (max6639_i2cRxBuffer[1] >> 5)) * 0.125;
}

uint16_t max6639_get_rpm(void)
{
	chDbgCheck(max6639_initDone != false);

	if (!max6639_isPresent)
		return -254;

	return 0;
}

static void max6639ProcessI2cError(void)
{
	max6639_i2cErrors = i2cGetErrors(max6639_i2cDriver);
	if (max6639_i2cStatus == MSG_TIMEOUT)
		i2cStart(max6639_i2cDriver, &i2cConfig);
	i2cReleaseBus(max6639_i2cDriver);
}
