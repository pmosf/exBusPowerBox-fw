#include "CLTC2943.hpp"
#include "pca9543a.h"

namespace LTC2943 {
  CLTC2943::CLTC2943(I2CDriver *i2cDriver, std::uint8_t batIndex) :
      i2cDriver_(i2cDriver) {
    batIndex_ = batIndex;
    isInitialized_ = false;
  }

  CLTC2943::~CLTC2943() {

  }

  void CLTC2943::init() {
    // select slave
    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, PCA9543A_I2C_ADDR,
                                          &batIndex_, 1, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }

    // power down to set capacity register
    txBuffer_[0] = LTC2943_CTRL_ADDR;
    txBuffer_[1] = 1;

    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, LTC2943_I2C_ADDR,
                                          txBuffer_, 2, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }

    // set capacity register to max
    txBuffer_[0] = LTC2943_ACC_MSB_ADDR;
    txBuffer_[1] = 0xFF;
    txBuffer_[2] = 0xFF;

    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, LTC2943_I2C_ADDR,
                                          txBuffer_, 3, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }

    // set ADC in automatic mode and prescaler
    txBuffer_[0] = LTC2943_CTRL_ADDR;
    txBuffer_[1] = LTC2943_ADC_MODE_AUTO | LTC2943_PRESCALER_1024;

    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, LTC2943_I2C_ADDR,
                                          txBuffer_, 2, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }

    isInitialized_ = true;
  }

  void CLTC2943::StartConv() {
    if (!isInitialized_)
      return;

    // select slave
    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, PCA9543A_I2C_ADDR,
                                          &batIndex_, 1, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }

    txBuffer_[0] = LTC2943_CTRL_ADDR;
    txBuffer_[1] = LTC2943_ADC_MODE_MAN | LTC2943_PRESCALER_1024;

    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, LTC2943_I2C_ADDR,
                                          txBuffer_, 2, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return;
    }
  }

  float CLTC2943::GetCapacity() {
    if (!isInitialized_)
      return 0.;

    std::int16_t capacity = 0xFFFF - GetData(LTC2943_ACC_MSB_ADDR);

    return (LTC2943_Q_LSB * (float)capacity);
  }

  float CLTC2943::GetVoltage() {
    if (!isInitialized_)
      return 0.;

    return (23.6 * GetData(LTC2943_VOLTAGE_MSB_ADDR) / 0xFFFF);
  }

  float CLTC2943::GetCurrent() {
    if (!isInitialized_)
      return 0.;

    std::int16_t current = 32767 - GetData(LTC2943_CURRENT_MSB_ADDR);
    float d = 60.0 / LTC2943_RSENSE * (float)current / 32767.0;

    return d;
  }

  float CLTC2943::GetTemperature() {
    if (!isInitialized_)
      return 0.;

    return (510 * (std::uint16_t)GetData(LTC2943_TEMP_MSB_ADDR) / 0xFFFF
        - 273.15);
  }

  std::int16_t CLTC2943::GetData(std::uint8_t addr) {
    // select slave
    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, PCA9543A_I2C_ADDR,
                                          &batIndex_, 1, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return 0.;
    }

    // read data
    i2cAcquireBus(i2cDriver_);
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, LTC2943_I2C_ADDR, &addr,
                                          1, rxBuffer_, 2, TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isInitialized_ = false;
      return 0.;
    }

    return ((rxBuffer_[0] << 8) | rxBuffer_[1]);
  }
}
