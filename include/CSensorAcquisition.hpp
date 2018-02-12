/*
 * CSensorAcquisition.hpp
 *
 *  Created on: Feb 9, 2018
 *      Author: pperonna
 */

#pragma once

#include "ch.hpp"
#include "hal.h"

#include "CGps.hpp"
#include "CLTC2943.hpp"
#include "CMAX6639.hpp"

#define NB_BAT  2

namespace ExPowerBox {

  class CSensorAcquisition: public chibios_rt::BaseStaticThread<128> {
  public:
    CSensorAcquisition(SerialDriver *gpsSerialDriver, I2CDriver *i2cDriver);
    virtual ~CSensorAcquisition();
    virtual void main();

  private:
    GPS::CGps gps_;
    I2CDriver *i2cDriver_;
    I2CConfig i2cConfig_;
    LTC2943::CLTC2943 ltc2943_[NB_BAT];
    MAX6639::CMAX6639 max6639_;
  };

} /* namespace ExPowerBox */
