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
#include "usbcfg.h"

namespace ExPowerBox {

  event_timer_t CExPowerBox::lowSpeedTimer_;
  event_timer_t CExPowerBox::highSpeedTimer_;
  I2CDriver *CExPowerBox::i2cDriver_ = I2C_DRIVER;
  std::array<LTC2943::CLTC2943, NB_BAT> CExPowerBox::ltc2943_ = {
      LTC2943::CLTC2943(i2cDriver_, 1), LTC2943::CLTC2943(i2cDriver_, 2)};
  MAX6639::CMAX6639 CExPowerBox::max6639_(i2cDriver_);

  CExPowerBox::CExPowerBox() :
      exBus_(
          {CExBusUart(&SD4, "exBusUart4", &exDevice_), CExBusUart(&SD5,
                                                                  "exBusUart5",
                                                                  &exDevice_),
           CExBusUart(&SD6, "exBusUart6", &exDevice_)}), gps_(&SD1), i2cConfig_(
          {I2C_100K, 0, 0}), pwmDriver_ {&PWMD1, &PWMD2, &PWMD3, &PWMD8, &PWMD9,
                                         &PWMD12} {
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

    threadParams_.exDevice = &exDevice_;
    threadParams_.i2cDriver = i2cDriver_;
    threadParams_.mutSensors = &mutSensors_;
    threadParams_.mutServoPos = &mutServoPos_;

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
    sduObjectInit(&PORTAB_SDU1);
    sduStart(&PORTAB_SDU1, &serusbcfg);
    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
#endif
  }

  CExPowerBox::~CExPowerBox() {

  }

  __attribute__((noreturn))
  void CExPowerBox::main() {
    setName("ExPowerBox");

    exDevice_.init(&mutSensors_);
    jetibox_.init();

    // register exbus events and start threads
    for (int i = 0; i < NB_EX_UART; ++i) {
      exBus_[i].init(&mutServoPos_, &mutSensors_);
      exBus_[i].getEvent()->registerMask(&exBusEvent_[i], EVENT_MASK(i));
      addEvents(EVENT_MASK(i));
      exBus_[i].start(NORMALPRIO);
    }

    // start low-speed sensor acquisition thread
    chThdCreateStatic(lowSpeedWA, sizeof(lowSpeedWA),
    NORMALPRIO - 2,
                      lowAcqThread, &threadParams_);

    // start high-speed acquisition thread
    chThdCreateStatic(highSpeedWA, sizeof(highSpeedWA),
    NORMALPRIO - 1,
                      fastAcqThread, &threadParams_);

    while (true) {
      // set servos to fail-safe positions if no activity on exbus uarts
      if (exBusSel_ == 3) {
        updateServoPositions(true);
        exBusSel_ = 0;
      }

      // Waiting for ExBus events
      eventmask_t evt = waitOneEventTimeout(
          ((eventmask_t)1 << (eventmask_t)(exBusSel_)), TIME_MS2I(50));

      // in case of 4 consecutive timeouts, we select another exbus receiver
      if (evt == 0) {
        // wait for one valid event before timeout detection
        if (!validEvtReceived_ || !isExBusLinkUp_) {
          continue;
        }
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
        continue;
      }

      if (flags & EXBUS_JETIBOX) {
        isExBusLinkUp_ = true;
        updateJetibox();
        continue;
      }

      if (flags & EXBUS_TIMEOUT) {
        if (isExBusLinkUp_)
          processExBusTimeout();
        continue;
      }

      if ((flags & EXBUS_CRC_ERR) || (flags & EXBUS_UART_ERR)) {
        if (isExBusLinkUp_) {
          processExBusError();
        }
      }
    }
  }

  void CExPowerBox::updateTelemetryValues() {
  }

  void CExPowerBox::updateJetibox() {
    exBus_[exBusSel_].sendJetibox(jetibox_.getPacket(0));
  }

  __attribute__((noreturn))
  void CExPowerBox::fastAcqThread(void* arg) {
    chRegSetThreadName("HSpeedAcq");

    thread_params_t *tp = (thread_params_t*)arg;
    event_listener_t el;

    evtObjectInit(&lowSpeedTimer_, TIME_MS2I(500));

    chEvtRegisterMask((event_source_t*)&lowSpeedTimer_.et_es, &el,
    EVENT_HSA_TIMER);

    evtStart(&lowSpeedTimer_);

    while (TRUE) {
      chEvtWaitAny(EVENT_HSA_TIMER);
      tp->mutSensors->lock();
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_VOLTAGE1)->setValue(
          ltc2943_[0].GetVoltage());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_VOLTAGE2)->setValue(
          ltc2943_[1].GetVoltage());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_CURRENT1)->setValue(
          ltc2943_[0].GetCurrent());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_CURRENT2)->setValue(
          ltc2943_[1].GetCurrent());
      tp->mutSensors->unlock();
    }
  }

  __attribute__((noreturn))
  void CExPowerBox::lowAcqThread(void* arg) {
    chRegSetThreadName("LSpeedAcq");

    thread_params_t *tp = (thread_params_t*)arg;
    event_listener_t el;

    evtObjectInit(&highSpeedTimer_, TIME_S2I(2));

    chEvtRegisterMask((event_source_t*)&highSpeedTimer_.et_es, &el,
    EVENT_LSA_TIMER);

    evtStart(&highSpeedTimer_);

    while (TRUE) {
      chEvtWaitAny(EVENT_LSA_TIMER);
      tp->mutSensors->lock();
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_CAPACITY1)->setValue(
          ltc2943_[0].GetCapacity());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::BAT_CAPACITY2)->setValue(
          ltc2943_[1].GetCapacity());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::T_LOCAL)->setValue(
          ltc2943_[0].GetTemperature());
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::T_EXT1)->setValue(
          max6639_.GetExtTemperature(0));
      tp->exDevice->getSensor((int)Jeti::Device::DeviceSensorLUT::T_EXT2)->setValue(
          max6639_.GetExtTemperature(1));
      tp->mutSensors->unlock();
    }
  }

  void CExPowerBox::processExBusError() {

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

  // TIM8
  pwmConfig_[3] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr},
      { PWM_OUTPUT_DISABLED, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM9
  pwmConfig_[4] = {pwmSettings_.freq,
    pwmSettings_.periodTick,
    nullptr, /* No callback */
    { { PWM_OUTPUT_ACTIVE_HIGH, nullptr}, {
        PWM_OUTPUT_ACTIVE_HIGH,
        nullptr},
      { PWM_OUTPUT_DISABLED, nullptr}, {
        PWM_OUTPUT_DISABLED,
        nullptr}, {PWM_OUTPUT_DISABLED, nullptr}, {PWM_OUTPUT_DISABLED, nullptr}},
    0, 0};

  // TIM12
  pwmConfig_[5] = {pwmSettings_.freq,
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
