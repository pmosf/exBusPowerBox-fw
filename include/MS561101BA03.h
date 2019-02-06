#pragma once

#include <stdint.h>
#include <stdbool.h>

#define IIC_MS5611_ADDR		0x4C

#define MS5611_LTEMP_ADDR		0
#define MS5611_RTEMP1_ADDR		1
#define MS5611_RTEMP2_ADDR		2
#define MS5611_RTEMP3_ADDR		3
#define MS5611_STATUS_ADDR		8
#define MS5611_CONF1_ADDR		9
#define MS5611_CONF2_ADDR		0x0A
#define MS5611_RATE_ADDR		0x0B
#define MS5611_ONESHOT_ADDR		0x0F
#define MS5611_LTEMP_LB_ADDR	0x10
#define MS5611_RTEMP1_LB_ADDR	0x11
#define MS5611_RTEMP2_LB_ADDR	0x12
#define MS5611_RTEMP3_LB_ADDR	0x13
#define MS5611_N1_ADDR			0x21
#define MS5611_N2_ADDR			0x22
#define MS5611_N3_ADDR			0x23
#define MS5611_SOFTRESET_ADDR	0xFC
#define MS5611_MID_ADDR			0xFE
#define MS5611_DEVID_ADDR		0xFF

#define TMP4xx_MID				0x55
#define TMP421_DEVID			0x21
#define MS5611_DEVID			0x22
#define TMP423_DEVID			0x23


#define MS5611_TX_BUFFER_SIZE  4
#define MS5611_RX_BUFFER_SIZE  4

bool ms5611Init(void);
float ms5611GetExtTemperature(uint8_t sensor_index);
float ms5611GetLocalTemperature(void);
