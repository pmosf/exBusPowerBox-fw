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

    ltc2943_[0].init();
    ltc2943_[1].init();
    max6639_.init();

    while (true) {

    }
  }

} /* namespace ExPowerBox */
