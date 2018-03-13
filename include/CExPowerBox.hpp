/*
 * CExPowerBox.h
 *
 *  Created on: Jan 31, 2018
 *      Author: pperonna
 */

#pragma once

#include <array>

#include "ch.hpp"
#include "hal.h"
#include "evtimer.h"

#include "CExDevice.hpp"
#include "CExBus.hpp"
#include "CGps.hpp"
#include "CLTC2943.hpp"
#include "CMAX6639.hpp"

#define NB_BAT              2
#define NB_EX_UART          3
#define NB_TIM_PWM          5
#define EX_PWM_FREQ         8000000UL
#define EX_MANUFACTURER_ID  0xA400
#define EX_DEVICE_ID        0

#define I2C_DRIVER          &I2CD1

#define EVENT_EXBUS0              EVENT_MASK(0)
#define EVENT_EXBUS1              EVENT_MASK(1)
#define EVENT_EXBUS2              EVENT_MASK(2)
#define EVENT_LSA_TIMER     EVENT_MASK(3)
#define EVENT_HSA_TIMER     EVENT_MASK(4)

namespace ExPowerBox {

  typedef struct {
      uint32_t freq;
      uint32_t periodTick;
      float period;
      uint32_t widthDivider;
  } pwm_settings_t;

  typedef struct {
      float voltage[NB_BAT];
      float current[NB_BAT];
      float capacity[NB_BAT];
      chibios_rt::Mutex m;
  } batteryMonitoring_t;

  typedef struct {
      float local;
      float ext[2];
      chibios_rt::Mutex m;
  } temperatureMonitoring_t;

  class CExPowerBox: public chibios_rt::BaseStaticThread<512> {
    public:
      CExPowerBox();
      virtual ~CExPowerBox();
      virtual void main();

    private:
      void initPwm();
      void updateServoPositions(bool failSafe);
      void updatePwmSettings(uint32_t freq, float period);
      void processExBusTimeout();
      static void fastAcqThread(void *arg);
      static void lowAcqThread(void *arg);

      CExBusUart exBus_[NB_EX_UART];
      GPS::CGps gps_;
      I2CDriver *i2cDriver_;
      I2CConfig i2cConfig_;
      std::array<LTC2943::CLTC2943, NB_BAT> ltc2943_;
      MAX6639::CMAX6639 max6639_;THD_WORKING_AREA(lowSpeedWA, 512);THD_WORKING_AREA(highSpeedWA, 512);
      static event_timer_t lowSpeedTimer_;
      static event_timer_t highSpeedTimer_;
      static batteryMonitoring_t battery_;
      static temperatureMonitoring_t temperature_;
      chibios_rt::EvtListener exBusEvent_[NB_EX_UART];
      std::array<uint16_t, EX_NB_SERVOS> servoPosition_;
      std::array<uint16_t, EX_NB_SERVOS> servoFailSafePosition_;
      std::array<PWMDriver*, NB_TIM_PWM> pwmDriver_;
      std::array<PWMConfig, NB_TIM_PWM> pwmConfig_;
      std::array<uint32_t, NB_EX_UART> nbExBusTimeout_;
      std::array<uint32_t, NB_EX_UART> nbTotalExBusTimeout_;
      pwm_settings_t pwmSettings_;
      uint32_t exBusSel_;
      bool validEvtReceived_;
      bool isExBusLinkUp_;
      USBConfig usbCfg_;
      SerialUSBConfig serialUsbCfg_;
      SerialUSBDriver serialUsbDriver_;
  };
}
