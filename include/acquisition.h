#pragma once

#include "ch.h"
#include "hal.h"

#define NB_LTC2943	2

typedef struct {
	float voltage[2];
	float current[2];
	uint32_t capacity[2];
	float localTemp;
	float extTemp[2];
} acq_t;

typedef struct{
	float voltage[2];
	float current[2];
} acqMinMax_t;

void acqInit(void);
int32_t acqGetRpm(uint8_t chan);
int32_t acqGetExtTemp(uint8_t chan);
int32_t acqGetLocalTemp(void);
int32_t acqGetCapacity(uint8_t chan);
int32_t acqGetCurrent(uint8_t chan);
int32_t acqGetVoltage(uint8_t chan);
uint8_t acqResetCapacity(uint8_t chan);
