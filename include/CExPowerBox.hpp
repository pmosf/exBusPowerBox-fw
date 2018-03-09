/*
 * CExPowerBox.h
 *
 *  Created on: Jan 31, 2018
 *      Author: pperonna
 */

#pragma once

//#include <vectors>

#include "ch.hpp"
#include "hal.h"

#include "CExDevice.hpp"
#include "CExBus.hpp"
#include "CSensorAcquisition.hpp"

#define NB_EX_UART          3
#define NB_TIM_PWM          5
#define EXBUS0              EVENT_MASK(0)
#define EXBUS1              EVENT_MASK(1)
#define EXBUS2              EVENT_MASK(2)
#define EX_PWM_FREQ         8000000UL
#define EX_MANUFACTURER_ID  0xA400
#define EX_DEVICE_ID        0

namespace ExPowerBox {

  typedef struct {
      uint32_t freq;
      uint32_t periodTick;
      float period;
      uint32_t widthDivider;
  } pwm_settings_t;

  class CExPowerBox: public chibios_rt::BaseStaticThread<512> {
    public:
      CExPowerBox();
      virtual ~CExPowerBox();
      virtual void main();

    private:
      void initPwm();
      void updateServoPositions(bool failSafe);
      void updatePwmSettings(uint32_t freq, float period);

      CExBusUart exBus_[NB_EX_UART];
      CSensorAcquisition sensorAcq_;
      chibios_rt::EvtListener exBusEvtListener_[NB_EX_UART];
      std::array<uint16_t, EX_NB_SERVOS> servoPosition_;
      std::array<uint16_t, EX_NB_SERVOS> servoFailSafePosition_;
      std::array<PWMDriver*, NB_TIM_PWM> pwmDriver_;
      std::array<PWMConfig, NB_TIM_PWM> pwmConfig_;
      std::array<uint32_t, NB_EX_UART> nbExBusTimeout_;
      std::array<uint32_t, NB_EX_UART> nbTotalExBusTimeout_;
      pwm_settings_t pwmSettings_;
      uint32_t exBusSel_;
      USBConfig usbCfg_;
      SerialUSBConfig serialUsbCfg_;
      SerialUSBDriver serialUsbDriver_;
  };
}
