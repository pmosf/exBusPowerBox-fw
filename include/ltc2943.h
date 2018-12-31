#pragma once

#include "hal.h"

#define LTC2943_I2C_ADDR	0x64

#define LTC2943_STATUS_ADDR			0
#define LTC2943_CTRL_ADDR			1
#define LTC2943_ACC_MSB_ADDR		2
#define LTC2943_ACC_LSB_ADDR		3
#define LTC2943_VOLTAGE_MSB_ADDR	8
#define LTC2943_VOLTAGE_LSB_ADDR	9
#define LTC2943_CURRENT_MSB_ADDR	0x0E
#define LTC2943_CURRENT_LSB_ADDR	0x0F
#define LTC2943_TEMP_MSB_ADDR		0x14
#define LTC2943_TEMP_LSB_ADDR		0x15

#define LTC2943_ADC_MODE_AUTO		0xC0
#define LTC2943_ADC_MODE_SCAN		0x80
#define LTC2943_ADC_MODE_MAN		0x40
#define LTC2943_PRESCALER_1			(0 << 3)
#define LTC2943_PRESCALER_1024		(5 << 3)

#define LTC2943_PRESCALER			1024.0
#define LTC2943_RSENSE				10.0
#define LTC2943_Q_LSB				(0.34 * 50.0 / LTC2943_RSENSE * LTC2943_PRESCALER / 4096.0)

#define LTC2943_TX_BUFFER_SIZE  4
#define LTC2943_RX_BUFFER_SIZE  4

bool ltc2943Init(void);
void ltc2943StartSingleConversion(uint8_t channel);
float ltc2943GetCapacity(uint8_t channel);
float ltc2943GetVoltage(uint8_t channel);
float ltc2943GetCurrent(uint8_t channel);
float ltc2943GetTemperature(void);
