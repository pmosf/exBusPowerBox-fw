#pragma once

#include "ch.h"
#include "hal.h"

#define NB_LTC2943	2

typedef struct {
	float voltage[2];
	float current[2];
	float capacity[2];
	float local_temp;
	float ext_temp[2];
} acq_t;

typedef struct{
	float Voltage[2];
	float Current[2];
} acqMinMax_t;

void acqInit(void);
int32_t acqGetRpm(uint8_t chan);
int32_t acqGetExtTemp(uint8_t chan);
int32_t acqGetLocalTemp(void);
int32_t acqGetCapacity(uint8_t chan);
int32_t acqGetCurrent(uint8_t chan);
int32_t acqGetVoltage(uint8_t chan);
