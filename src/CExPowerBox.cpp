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
      exBus_ {CExBusUart(this->thread_ref, &SD4, "exBusUart4"), CExBusUart(
          this->thread_ref, &SD5, "exBusUart5"),
              CExBusUart(this->thread_ref, &SD6, "exBusUart6")}, sensorAcq_(
          &SD1, &I2CD1), pwmDriver_ {&PWMD1, &PWMD2, &PWMD3, &PWMD5, &PWMD12} {

    // set initial pwm frequency and period
    updatePwmSettings(2000000UL, 20E-3);
    // set fail-safe servo values to middle position
    for (int i = 0; i < EX_NB_SERVOS; ++i)
      servoFailSafePosition_[i] = 1.5E-3 * EX_PWM_FREQ;

    // 1st exbus uart is selected by default
    exBusSel_ = 0;
    // initialize timeout counters
    for (int i = 0; i < NB_EX_UART; ++i) {
      nbExBusTimeout_[i] = 0;
      nbTotalExBusTimeout_[i] = 0;
    }

    initPwm();
    updateServoPositions(true);
#if 0
    // USB initialization
    sduObjectInit(&serialUsbDriver_);
    sduStart(&serialUsbDriver_, &serialUsbCfg_);
    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus(serialUsbCfg_.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serialUsbCfg_.usbp, &usbCfg_);
    usbConnectBus(serialUsbCfg_.usbp);
#endif
  }

  CExPowerBox::~CExPowerBox() {

  }

  __attribute__((noreturn))
  void CExPowerBox::main() {
    setName("ExPowerBox");

    // register exbus events and start threads
    for (int i = 0; i < NB_EX_UART; ++i) {
      exBus_[i].getEvtServoPosition().registerMaskWithFlags(
          &exBusEvtListener_[i], EVENT_MASK(i),
          EXBUS_SERVO_POSITIONS | EXBUS_TIMEOUT | EXBUS_CRC_ERR);

      exBus_[i].start(NORMALPRIO);
    }

    // start sensor acquisition thread
    sensorAcq_.start(NORMALPRIO + 10);

    while (true) {
      // set servos to fail-safe positions if no activity on exbus uarts
      if (exBusSel_ == 3) {
        updateServoPositions(true);
        exBusSel_ = 0;
      }

      // Waiting for ExBus events
      eventmask_t evt = waitOneEventTimeout(
          ((eventmask_t)1 << (eventmask_t)(exBusSel_)), TIME_MS2I(25));

      // in case of 4 consecutive timeouts, we select another exbus receiver
      if (evt == 0) {
        nbTotalExBusTimeout_[exBusSel_]++;
        if (++nbExBusTimeout_[exBusSel_] == 4) {
          nbExBusTimeout_[exBusSel_] = 0;
          exBusSel_++;
        }
        continue;
      }

      nbExBusTimeout_[exBusSel_] = 0;
      evt = getAndClearEvents(ALL_EVENTS);
      eventflags_t flags = exBusEvtListener_[exBusSel_].getAndClearFlags();

      if (flags & EXBUS_SERVO_POSITIONS) {
        exBus_[exBusSel_].getServoPosition(&servoPosition_[0]);
        updateServoPositions(false);
        continue;
      }

      if (flags & EXBUS_TELEMETRY) {
        continue;
      }

      if (flags & EXBUS_JETIBOX) {
        continue;
      }
    }
  }

  void CExPowerBox::updatePwmSettings(uint32_t freq, float period) {
    pwmSettings_.freq = freq;
    pwmSettings_.period = period;
    pwmSettings_.periodTick = pwmSettings_.freq * pwmSettings_.period;
    pwmSettings_.widthDivider = EX_PWM_FREQ / pwmSettings_.freq;
  }

  void CExPowerBox::updateServoPositions(bool failSafe) {
    int servoPositionIndex = 0;

    // set servos to fail-safe positions
    if (failSafe) {
      for (int i = 0; i < NB_PWM; ++i) {
        for (int j = 0; j < pwmDriver_[i]->channels; ++j) {
          pwmEnableChannel(
              pwmDriver_[i],
              j,
              servoFailSafePosition_[servoPositionIndex++]
                  / pwmSettings_.widthDivider);
        }
      }
      return;
    }

    // update servo with refreshed values
    for (int i = 0; i < NB_PWM; ++i) {
      for (int j = 0; j < pwmDriver_[i]->channels; ++j) {
        pwmEnableChannel(
            pwmDriver_[i], j,
            servoPosition_[servoPositionIndex++] / pwmSettings_.widthDivider);
      }
    }
  }

  void CExPowerBox::initPwm() {
    for (int i = 0; i < NB_PWM; ++i) {
    pwmConfig_[i] = {pwmSettings_.freq,
      pwmSettings_.periodTick,
      nullptr, /* No callback */
      { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
          PWM_OUTPUT_ACTIVE_HIGH,
          nullptr},
        { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
          PWM_OUTPUT_ACTIVE_HIGH,
          nullptr}, {PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {PWM_OUTPUT_ACTIVE_HIGH, nullptr}},
      0, 0};
    pwmStart(pwmDriver_[i], &pwmConfig_[i]);
  }
}
}
/* namespace ExPowerBox */
