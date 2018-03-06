/*
 * CSensorAcquisition.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: pperonna
 */

#include "CSensorAcquisition.hpp"

namespace ExPowerBox {

  CSensorAcquisition::CSensorAcquisition(SerialDriver *gpsSerialDriver,
                                         I2CDriver *i2cDriver) :
      gps_(gpsSerialDriver), i2cDriver_(i2cDriver), i2cConfig_( {0, 0, 0}), ltc2943_ {
          LTC2943::CLTC2943(i2cDriver, 0), LTC2943::CLTC2943(i2cDriver, 1)}, max6639_(
          i2cDriver) {

  }

  CSensorAcquisition::~CSensorAcquisition() {
  }

  void CSensorAcquisition::main() {
    setName("SensorAcq");
    i2cStart(i2cDriver_, &i2cConfig_);

    if (!ltc2943_[0].init()) {
      i2cAcquireBus(i2cDriver_);
      i2cDriver_->state = I2C_READY;
      i2cReleaseBus(i2cDriver_);
    }

    if (!ltc2943_[1].init()) {
      i2cAcquireBus(i2cDriver_);
      i2cDriver_->state = I2C_READY;
      i2cReleaseBus(i2cDriver_);
    }

    if (!max6639_.init()) {
      i2cAcquireBus(i2cDriver_);
      i2cDriver_->state = I2C_READY;
      i2cReleaseBus(i2cDriver_);
    }

    while (true) {
      chibios_rt::BaseThread::sleep(TIME_MS2I(500));
    }
  }

} /* namespace ExPowerBox */
