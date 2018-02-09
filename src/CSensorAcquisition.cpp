/*
 * CSensorAcquisition.cpp
 *
 *  Created on: Feb 9, 2018
 *      Author: pperonna
 */

#include "CSensorAcquisition.hpp"

namespace ExPowerBox {

  CSensorAcquisition::CSensorAcquisition(SerialDriver *gpsSerialDriver) :
      gps_(gpsSerialDriver) {
  }

  CSensorAcquisition::~CSensorAcquisition() {
  }

  void CSensorAcquisition::main() {

  }

} /* namespace ExPowerBox */
