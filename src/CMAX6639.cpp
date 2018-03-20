#include "CMAX6639.hpp"
#include "pca9543a.h"

namespace MAX6639 {
  CMAX6639::CMAX6639(I2CDriver *i2cDriver) :
      i2cDriver_(i2cDriver), isPresent_(true) {
  }

  bool CMAX6639::init() {
    txBuffer_[0] = MAX6639_DEVID_ADDR;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, rxBuffer_, 1,
                                          TIME_MS2I(10));
    cacheBufferInvalidate(rxBuffer_, sizeof(rxBuffer_));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK || *rxBuffer_ != MAX6639_DEVICE_ID) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return false;
    }

    // set 2nd sensor as external
    txBuffer_[0] = MAX6639_GCONF_ADDR;
    txBuffer_[1] = 0x20;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 2, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return false;
    }

    return true;
  }

  float CMAX6639::GetExtTemperature(std::uint8_t sensorIndex) {
    if (!isPresent_)
      return 254;

    txBuffer_[0] = MAX6639_TEMP1_ADDR + sensorIndex;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, rxBuffer_, 1,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return 254;
    }

    txBuffer_[0] = MAX6639_EXT_TEMP1_ADDR + sensorIndex;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, &rxBuffer_[1], 1,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return 254;
    }

    // checking diode fault flag
    if (rxBuffer_[1] & 1)
      return 255;

    cacheBufferInvalidate(rxBuffer_, sizeof(rxBuffer_));

    return (rxBuffer_[0] + (rxBuffer_[1] >> 5)) * 0.125;
  }

  float CMAX6639::GetLocalTemperature(void) {
    if (!isPresent_)
      return 254;

    // set 2nd sensor as local
    *txBuffer_ = 0x30;
    txBuffer_[0] = MAX6639_GCONF_ADDR;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, nullptr, 0,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return 254;
    }

    *txBuffer_ = MAX6639_TEMP2_ADDR;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, &rxBuffer_[0], 1,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return 254;
    }

    *txBuffer_ = MAX6639_EXT_TEMP2_ADDR;
    i2cAcquireBus(i2cDriver_);
    cacheBufferFlush(txBuffer_, sizeof(txBuffer_));
    i2cStatus_ = i2cMasterTransmitTimeout(i2cDriver_, IIC_MAX6639_ADDR,
                                          txBuffer_, 1, &rxBuffer_[1], 1,
                                          TIME_MS2I(10));
    i2cReleaseBus(i2cDriver_);

    if (i2cStatus_ != MSG_OK) {
      i2cErrors_ = i2cGetErrors(i2cDriver_);
      isPresent_ = false;
      return 254;
    }

    cacheBufferInvalidate(rxBuffer_, sizeof(rxBuffer_));

    return (rxBuffer_[0] + (rxBuffer_[1] >> 5)) * 0.125;
  }

  std::uint16_t CMAX6639::GetRpm(void) {
    if (!isPresent_)
      return 0;
    return 0;
  }
}
