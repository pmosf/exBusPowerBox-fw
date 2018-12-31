#include "board_conf.h"
#include "ltc2943.h"
#include "pca9543a.h"

static bool ltc2943_initDone[2] = {false, false};
static I2CDriver *ltc2943_i2cDriver = I2C_DRIVER;
static __attribute__((aligned(32))) uint8_t ltc2943_i2cTxBuffer[LTC2943_TX_BUFFER_SIZE];
static __attribute__((aligned(32))) uint8_t ltc2943_i2cRxBuffer[LTC2943_RX_BUFFER_SIZE];
static msg_t ltc2943_i2cStatus;
static i2cflags_t ltc2943_i2cErrors;
static float lastCapacity[2] = {0, 0};

static int16_t ltc2943GetData(uint8_t channel, uint8_t addr);
static bool ltc2943ChannelInit(uint8_t channel);
static void ltc2943ProcessI2cError(uint8_t channel);

extern I2CConfig i2cConfig;

bool ltc2943Init(void)
{
	chDbgCheck(ltc2943_initDone[0] != true);
	chDbgCheck(ltc2943_initDone[1] != true);

	ltc2943_i2cErrors = 0;

	ltc2943_initDone[0] = ltc2943ChannelInit(0);
	ltc2943_initDone[1] = ltc2943ChannelInit(1);

	return ltc2943_initDone[0] & ltc2943_initDone[1];
}

static bool ltc2943ChannelInit(uint8_t channel)
{
	i2cAcquireBus(ltc2943_i2cDriver);

	// select slave
	ltc2943_i2cTxBuffer[0] = channel + 1;
	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             PCA9543A_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 1, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return false;
	}
#if 0
	// power down to set capacity register
	ltc2943_i2cTxBuffer[0] = LTC2943_CTRL_ADDR;
	ltc2943_i2cTxBuffer[1] = 1;

	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             LTC2943_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 2, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return false;
	}

	// set capacity register to max
	ltc2943_i2cTxBuffer[0] = LTC2943_ACC_MSB_ADDR;
	ltc2943_i2cTxBuffer[1] = 0xFF;
	ltc2943_i2cTxBuffer[2] = 0xFF;

	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             LTC2943_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 3, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return false;
	}
#endif
	// set ADC in automatic mode and prescaler
	ltc2943_i2cTxBuffer[0] = LTC2943_CTRL_ADDR;
	ltc2943_i2cTxBuffer[1] = LTC2943_ADC_MODE_AUTO |
	                         LTC2943_PRESCALER_1024;

	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             LTC2943_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 2, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return false;
	}

	i2cReleaseBus(ltc2943_i2cDriver);

	return true;
}

void ltc2943StartSingleConversion(uint8_t channel)
{
	if (!ltc2943_initDone[channel]) {
		if (!ltc2943ChannelInit(channel)) {
			return;
		}
	}

	i2cAcquireBus(ltc2943_i2cDriver);

	// select slave
	ltc2943_i2cTxBuffer[0] = channel + 1;
	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             PCA9543A_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 1, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return;
	}

	ltc2943_i2cTxBuffer[0] = LTC2943_CTRL_ADDR;
	ltc2943_i2cTxBuffer[1] = LTC2943_ADC_MODE_MAN |
	                         LTC2943_PRESCALER_1024;

	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             LTC2943_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 2, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return;
	}

	i2cReleaseBus(ltc2943_i2cDriver);
}

float ltc2943GetCapacity(uint8_t channel)
{
	if (!ltc2943_initDone[channel]) {
		if (!ltc2943ChannelInit(channel)) {
			return 0;
		}
	}

	uint16_t data = ltc2943GetData(channel, LTC2943_ACC_MSB_ADDR);
	if (data == 0)
		return 0;

	lastCapacity[channel] = (LTC2943_Q_LSB * (float) (0x7FFF - data));

	return lastCapacity[channel];
}

float ltc2943GetVoltage(uint8_t channel)
{
	if (!ltc2943_initDone[channel]) {
		if (!ltc2943ChannelInit(channel)) {
			return 0;
		}
	}

	float rtn = (23.6 * ltc2943GetData(channel, LTC2943_VOLTAGE_MSB_ADDR) / 0xFFFF);

	return rtn;
}

float ltc2943GetCurrent(uint8_t channel)
{
	if (!ltc2943_initDone[channel]) {
		if (!ltc2943ChannelInit(channel)) {
			return 0;
		}
	}

	uint16_t data = ltc2943GetData(channel, LTC2943_CURRENT_MSB_ADDR);
	if (data == 0)
		return 0;

	float f = 60.0 / LTC2943_RSENSE * (float) (32767 - data) / 32767.0;

	return f;
}

float ltc2943GetTemperature(void)
{
	if (!ltc2943_initDone[0]) {
		if (!ltc2943ChannelInit(0)) {
			return 0;
		}
	}

	float f = (510 * (uint16_t) ltc2943GetData(0,
	                                           LTC2943_TEMP_MSB_ADDR) / 0xFFFF - 273.15);

	return f;
}

static int16_t ltc2943GetData(uint8_t channel, uint8_t addr)
{
	i2cAcquireBus(ltc2943_i2cDriver);

	// select slave
	ltc2943_i2cTxBuffer[0] = channel + 1;
	cacheBufferFlush(ltc2943_i2cTxBuffer, sizeof(ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             PCA9543A_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 1, NULL, 0, TIME_MS2I(10));

	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return 0;
	}

	// read data
	ltc2943_i2cTxBuffer[0] = addr;
	cacheBufferFlush(ltc2943_i2cTxBuffer, LTC2943_TX_BUFFER_SIZE * sizeof(*ltc2943_i2cTxBuffer));
	ltc2943_i2cStatus = i2cMasterTransmitTimeout(ltc2943_i2cDriver,
	                                             LTC2943_I2C_ADDR,
	                                             ltc2943_i2cTxBuffer, 1, ltc2943_i2cRxBuffer, 2, TIME_MS2I(10));
	if (ltc2943_i2cStatus != MSG_OK) {
		ltc2943ProcessI2cError(channel);
		return 0;
	}

	cacheBufferInvalidate(ltc2943_i2cRxBuffer,
	                      LTC2943_RX_BUFFER_SIZE * sizeof(*ltc2943_i2cRxBuffer));

	i2cReleaseBus(ltc2943_i2cDriver);

	return ((ltc2943_i2cRxBuffer[0] << 8) | ltc2943_i2cRxBuffer[1]);
}

static void ltc2943ProcessI2cError(uint8_t channel)
{
	ltc2943_initDone[channel] = false;
	ltc2943_i2cErrors = i2cGetErrors(ltc2943_i2cDriver);
	if (ltc2943_i2cStatus == MSG_TIMEOUT)
		i2cStart(ltc2943_i2cDriver, &i2cConfig);
	i2cReleaseBus(ltc2943_i2cDriver);
}
