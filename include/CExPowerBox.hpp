/*
 * CExPowerBox.h
 *
 *  Created on: Jan 31, 2018
 *      Author: pperonna
 */

#ifndef SRC_CEXPOWERBOX_H_
#define SRC_CEXPOWERBOX_H_

#include <vectors.h>

#include "ch.hpp"
#include "hal.h"

#include "CExBus.hpp"
#include "CExBusUart.hpp"

#define NB_EX_UART  3
#define NB_PWM      5

namespace ExPowerBox {
  class CExPowerBox: public chibios_rt::BaseStaticThread<128> {
  public:
    CExPowerBox();
    virtual ~CExPowerBox();
    virtual void main();

  private:
    void initPins();
    void initPwm();
    void startPeripherals();
    void updateServoPositions();

    Jeti::ExBus::CExBus exBus_;
    CExBusUart exBusUart_[NB_EX_UART];
    event_listener_t exBusEvtListener_[NB_EX_UART];
    uint16_t servoPosition_[EX_NB_SERVOS];
    PWMDriver *pwmDriver_[NB_PWM];
    PWMConfig pwmConfig_[NB_PWM];
    uint32_t nbPositionTimeout_[NB_EX_UART];
    uint32_t nbTotalPositionTimeout_[NB_EX_UART];
  };
}

#endif /* SRC_CEXPOWERBOX_H_ */
