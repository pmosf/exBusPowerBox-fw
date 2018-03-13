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

  event_timer_t CExPowerBox::lowSpeedTimer_;
  event_timer_t CExPowerBox::highSpeedTimer_;
  batteryMonitoring_t CExPowerBox::battery_;
  temperatureMonitoring_t CExPowerBox::temperature_;

  CExPowerBox::CExPowerBox() :
      exBus_ {CExBusUart(&SD4, "exBusUart4"), CExBusUart(&SD5, "exBusUart5"),
              CExBusUart(&SD6, "exBusUart6")}, gps_(&SD1), i2cDriver_(
          I2C_DRIVER), i2cConfig_(
          {STM32_TIMINGR_PRESC(3U) |
          STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
          STM32_TIMINGR_SCLH(0xF) | STM32_TIMINGR_SCLL(0x13)/*0x00303D5D*/,
           0, 0}), ltc2943_ {LTC2943::CLTC2943(I2C_DRIVER, 1),
                             LTC2943::CLTC2943(
                             I2C_DRIVER,
                                               2)}, max6639_(I2C_DRIVER), pwmDriver_ {
          &PWMD1, &PWMD2, &PWMD3, &PWMD5, &PWMD12} {
    // initialize variables
    // set fail-safe and initial servo values to middle position
    for (int i = 0; i < EX_NB_SERVOS; ++i) {
      servoFailSafePosition_[i] = 1.5E-3 * EX_PWM_FREQ;
      servoPosition_[i] = 0;
    }
    // 1st exbus uart is selected by default
    exBusSel_ = 0;
    validEvtReceived_ = false;
    isExBusLinkUp_ = false;

    // initialize timeout counters
    for (int i = 0; i < NB_EX_UART; ++i) {
      nbExBusTimeout_[i] = 0;
      nbTotalExBusTimeout_[i] = 0;
    }

    // set initial pwm frequency and period
    updatePwmSettings(2000000UL, 20E-3);
    initPwm();

    // intialize I2C peripheral
    i2cStart(i2cDriver_, &i2cConfig_);

    // initialize LTC2943
    for (size_t i = 0; i < ltc2943_.size(); ++i) {
      if (!ltc2943_[i].init()) {
        i2cDriver_->state = I2C_READY;
      }
    }

    // initialize MAX6639
    if (!max6639_.init()) {
      i2cDriver_->state = I2C_READY;
    }

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
      exBus_[i].getEvent()->registerMask(&exBusEvent_[i], EVENT_MASK(i));
      addEvents(EVENT_MASK(i));
      exBus_[i].start(NORMALPRIO);
    }

    // start low-speed sensor acquisition thread
    chThdCreateStatic(lowSpeedWA, sizeof(lowSpeedWA),
    NORMALPRIO - 2,
                      lowAcqThread, &max6639_);

    // start high-speed acquisition thread
    chThdCreateStatic(highSpeedWA, sizeof(highSpeedWA),
    NORMALPRIO - 1,
                      fastAcqThread, &ltc2943_);

    while (true) {
      // set servos to fail-safe positions if no activity on exbus uarts
      if (exBusSel_ == 3) {
        updateServoPositions(true);
        exBusSel_ = 0;
      }

      // Waiting for ExBus events
      eventmask_t evt = waitOneEventTimeout(
          ((eventmask_t)1 << (eventmask_t)(exBusSel_)), TIME_MS2I(30));

      // in case of 4 consecutive timeouts, we select another exbus receiver
      if (evt == 0) {
        // wait for one valid event before timeout detection
        if (!validEvtReceived_ || !isExBusLinkUp_)
          continue;

        processExBusTimeout();
        continue;
      }
      validEvtReceived_ = true;

      evt = getAndClearEvents(ALL_EVENTS);
      eventflags_t flags = exBusEvent_[exBusSel_].getAndClearFlags();

      if (flags & EXBUS_SERVO_POSITIONS) {
        isExBusLinkUp_ = true;
        nbExBusTimeout_[exBusSel_] = 0;
        exBus_[exBusSel_].getServoPosition(&servoPosition_[0]);
        updateServoPositions(false);
        continue;
      }

      if (flags & EXBUS_TELEMETRY) {
        isExBusLinkUp_ = true;
        nbExBusTimeout_[exBusSel_] = 0;
        continue;
      }

      if (flags & EXBUS_JETIBOX) {
        isExBusLinkUp_ = true;
        nbExBusTimeout_[exBusSel_] = 0;
        continue;
      }

      if (flags & EXBUS_TIMEOUT) {
        if (isExBusLinkUp_)
          processExBusTimeout();
        continue;
      }
    }
  }

  __attribute__((noreturn))
  void CExPowerBox::fastAcqThread(void* arg) {
    chRegSetThreadName("HSpeedAcq");

    std::array<LTC2943::CLTC2943, NB_BAT> driver = *static_cast<std::array<
        LTC2943::CLTC2943, NB_BAT>*>(arg);
    event_listener_t el;

    evtObjectInit(&lowSpeedTimer_, TIME_MS2I(500));

    chEvtRegisterMask((event_source_t*)&lowSpeedTimer_.et_es, &el,
    EVENT_HSA_TIMER);

    evtStart(&lowSpeedTimer_);

    while (TRUE) {
      chEvtWaitAny(EVENT_HSA_TIMER);
      battery_.m.lock();
      for (size_t i = 0; i < driver.size(); ++i) {
        battery_.voltage[i] = driver[i].GetVoltage();
        battery_.current[i] = driver[i].GetCurrent();
      }
      battery_.m.unlock();
    }
  }

  __attribute__((noreturn))
  void CExPowerBox::lowAcqThread(void* arg) {
    chRegSetThreadName("LSpeedAcq");

    MAX6639::CMAX6639 *driver = static_cast<MAX6639::CMAX6639*>(arg);
    event_listener_t el;

    evtObjectInit(&highSpeedTimer_, TIME_S2I(2));

    chEvtRegisterMask((event_source_t*)&highSpeedTimer_.et_es, &el,
    EVENT_LSA_TIMER);

    evtStart(&highSpeedTimer_);

    while (TRUE) {
      chEvtWaitAny(EVENT_LSA_TIMER);
    }
  }

  void CExPowerBox::processExBusTimeout() {
    nbTotalExBusTimeout_[exBusSel_]++;
    if (++nbExBusTimeout_[exBusSel_] == 4) {
      nbExBusTimeout_[exBusSel_] = 0;
      exBusSel_++;
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
      for (size_t i = 0; i < pwmDriver_.size(); ++i) {
        for (int j = 0; j < pwmDriver_[i]->channels; ++j) {
          if (pwmDriver_[i]->config->channels[j].mode) {
            pwmEnableChannel(
                pwmDriver_[i],
                j,
                servoFailSafePosition_[servoPositionIndex++]
                    / pwmSettings_.widthDivider);
          }
        }
      }
      return;
    }

    // update servo with refreshed values
    for (size_t i = 0; i < pwmDriver_.size(); ++i) {
      for (int j = 0; j < pwmDriver_[i]->channels; ++j) {
        if (pwmDriver_[i]->config->channels[j].mode) {
          pwmEnableChannel(
              pwmDriver_[i], j,
              servoPosition_[servoPositionIndex++] / pwmSettings_.widthDivider);
        }
      }
    }
  }

  void CExPowerBox::initPwm() {

    // TIM1
  pwmConfig_[0] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr},
      { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM2
  pwmConfig_[1] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr},
      { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM3
  pwmConfig_[2] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr},
      { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM5
  pwmConfig_[3] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr},
      { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM12
  pwmConfig_[4] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr},
      { PWM_OUTPUT_DISABLED, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  for (size_t i = 0; i < pwmDriver_.size(); ++i) {
    pwmStart(pwmDriver_[i], &pwmConfig_[i]);
  }
}
}
/* namespace ExPowerBox */
