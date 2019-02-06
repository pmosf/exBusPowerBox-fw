#pragma once
#include "ch.h"
#include "hal.h"

#include "board_conf.h"

#define NB_SERVOS                16

void servoSetPwmSettings(uint32_t freq, float period, uint32_t widthDivider);
void servoSetPositions(uint16_t *pos);
void servoInit(void);
