#pragma once

#include <stdint.h>
#include "hal.h"

typedef struct _pwm_settings {
	uint32_t freq;
	uint32_t periodTick;
	float period;
	uint32_t widthDivider;
} pwm_settings_t;

typedef struct _thread_params {
	//Jeti::Device::CExDevice *exDevice;
	I2CDriver *i2cDriver;
	mutex_t *mutServoPos;
	mutex_t *mutSensors;
} thread_params_t;
