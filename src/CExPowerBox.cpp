/*
 * CExPowerBox.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: pperonna
 */

#include "ch.hpp"
#include "hal.h"
#include "CExPowerBox.hpp"
#include "CExBus.hpp"
#include "CGps.hpp"

namespace ExPowerBox {
  CExPowerBox::CExPowerBox() :
      exBus_("exPowerBox"), exBusUart_ {CExBusUart(this->thread_ref, &SD4,
                                                   "exBusUart4"),
                                        CExBusUart(this->thread_ref, &SD5,
                                                   "exBusUart5"),
                                        CExBusUart(this->thread_ref, &SD6,
                                                   "exBusUart6")}, pwmDriver_ {
          &PWMD1, &PWMD2, &PWMD3, &PWMD5, &PWMD12} {

    initPwm();
    initPins();

    for (int i = 0; i < NB_EX_UART; ++i) {
      nbPositionTimeout_[i] = 0;
      nbTotalPositionTimeout_[i] = 0;

      chEvtRegisterMaskWithFlags(&exBusUart_[i].getEvtServoPosition(),
                                 &exBusEvtListener_[i], EVENT_MASK(i),
                                 EXBUS_SERVO_POSITIONS | EXBUS_TIMEOUT | EXBUS_CRC_ERR);

      exBusUart_[i].start(NORMALPRIO);
    }

    /*exBus_.AddTelemetryVoltage("V Bat1", false);
     exBus_.AddTelemetryVoltage("V Bat2", false);
     exBus_.AddTelemetryCurrent("I Bat1", false);
     exBus_.AddTelemetryCurrent("I Bat2", false);
     exBus_.AddTelemetryCurrent("C Bat1", "mAh", false);
     exBus_.AddTelemetryCurrent("C Bat2", "mAh", false);
     exBus_.AddTelemetryTemperature("T Local", false);
     exBus_.AddTelemetryTemperature("T Ext1", false);
     exBus_.AddTelemetryTemperature("T Ext2", true);*/
  }

  CExPowerBox::~CExPowerBox() {

  }

  void CExPowerBox::main() {
    setName("ExPowerBox");

    while (true) {
      // Waiting for ExBus events
      eventmask_t evt = chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(50));

      // timeout
      if (evt == 0) {
        continue;
      }

      evt = chEvtGetAndClearEvents(ALL_EVENTS);

      // process ExBus 0
      if (evt & EVENT_MASK(0)) {
        eventflags_t flags = chEvtGetAndClearFlags(&exBusEvtListener_[0]);
        if (flags & EXBUS_SERVO_POSITIONS) {
          exBusUart_[0].getServoPosition(servoPosition_);
          continue;
        }
      }
    }
  }

  void CExPowerBox::updateServoPositions() {
    int servoPositionIndex = 0;

    for (int i = 0; i < NB_PWM; ++i) {
      for (int j = 0; j < pwmDriver_[i]->channels; ++j) {
        pwmEnableChannel(pwmDriver_[i], j,
                         servoPosition_[servoPositionIndex++]);
      }
    }
  }

  void CExPowerBox::initPins() {
    // Servo outputs
    palSetPadMode(GPIOA, GPIOA_SERVO1, PAL_MODE_ALTERNATE(2)); // TIM5_CH1 - servo 1
    palSetPadMode(GPIOA, GPIOA_SERVO2, PAL_MODE_ALTERNATE(1)); // TIM2_CH2 - servo 2
    palSetPadMode(GPIOA, GPIOA_SERVO3, PAL_MODE_ALTERNATE(2)); // TIM5_CH3 - servo 3
    palSetPadMode(GPIOA, GPIOA_SERVO4, PAL_MODE_ALTERNATE(2)); // TIM5_CH4 - servo 4

    palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(1)); // TIM2_CH3 - servo 5
    palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(1)); // TIM2_CH4 - servo 6

    palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1)); // TIM1_CH1 - servo 7
    palSetPadMode(GPIOA, 11, PAL_MODE_ALTERNATE(1)); // TIM1_CH2 - servo 8
    palSetPadMode(GPIOA, 13, PAL_MODE_ALTERNATE(1)); // TIM1_CH3 - servo 9
    palSetPadMode(GPIOA, 14, PAL_MODE_ALTERNATE(1)); // TIM1_CH4 - servo 10

    palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(1)); // TIM2_CH1 - servo 11
    palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(2)); // TIM3_CH1 - servo 12
    palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(2)); // TIM3_CH2 - servo 13

    palSetPadMode(GPIOB, 5, PAL_MODE_ALTERNATE(2)); // TIM3_CH3 - servo 14
    palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(2)); // TIM3_CH4 - servo 15
    palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(9)); // TIM3_CH4 - servo 16

    // I2C1
    palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4));
    palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4));

    // USB
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DP, PAL_MODE_ALTERNATE(10));

    // USART1 - GPS
    palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));

    // USART4 - Jeti receiver 1
    palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(8));

    // USART5 - Jeti receiver 2
    palSetPadMode(GPIOC, 12, PAL_MODE_ALTERNATE(8));

    // USART6 - Bootloader / Jeti receiver 3
    palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
  }

  void CExPowerBox::initPwm() {
    for (int i = 0; i < NB_PWM; ++i) {
    pwmConfig_[i] = {8000000, /* 8MHz PWM clock frequency */
      20000, /* PWM period 20 milli  second */
      nullptr, /* No callback */
      { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
          PWM_OUTPUT_ACTIVE_HIGH,
          nullptr},
        { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
          PWM_OUTPUT_ACTIVE_HIGH,
          nullptr},},
      0, 0};
    pwmStart(pwmDriver_[i], &pwmConfig_[i]);
  }
}
}
/* namespace ExPowerBox */
