/*
 * CSensorAcquisition.hpp
 *
 *  Created on: Feb 9, 2018
 *      Author: pperonna
 */

#ifndef CSENSORACQUISITION_HPP_
#define CSENSORACQUISITION_HPP_

#include "ch.hpp"
#include "hal.h"

#include "CGps.hpp"

namespace ExPowerBox {

  class CSensorAcquisition: public chibios_rt::BaseStaticThread<128> {
  public:
    CSensorAcquisition(SerialDriver *gpsSerialDriver);
    virtual ~CSensorAcquisition();
    virtual void main();

  private:
    GPS::CGps gps_;
  };

} /* namespace ExPowerBox */

#endif /* CSENSORACQUISITION_HPP_ */
