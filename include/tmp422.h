#pragma once

#include <stdint.h>
#include <stdbool.h>

#define IIC_TMP422_ADDR		0x4C

#define TMP422_LTEMP_ADDR		0
#define TMP422_RTEMP1_ADDR		1
#define TMP422_RTEMP2_ADDR		2
#define TMP422_RTEMP3_ADDR		3
#define TMP422_STATUS_ADDR		8
#define TMP422_CONF1_ADDR		9
#define TMP422_CONF2_ADDR		0x0A
#define TMP422_RATE_ADDR		0x0B
#define TMP422_ONESHOT_ADDR		0x0F
#define TMP422_LTEMP_LB_ADDR	0x10
#define TMP422_RTEMP1_LB_ADDR	0x11
#define TMP422_RTEMP2_LB_ADDR	0x12
#define TMP422_RTEMP3_LB_ADDR	0x13
#define TMP422_N1_ADDR			0x21
#define TMP422_N2_ADDR			0x22
#define TMP422_N3_ADDR			0x23
#define TMP422_SOFTRESET_ADDR	0xFC
#define TMP422_MID_ADDR			0xFE
#define TMP422_DEVID_ADDR		0xFF

#define TMP4xx_MID				0x55
#define TMP421_DEVID			0x21
#define TMP422_DEVID			0x22
#define TMP423_DEVID			0x23


#define TMP422_TX_BUFFER_SIZE  4
#define TMP422_RX_BUFFER_SIZE  4

bool tmp422Init(void);
float tmp422GetExtTemperature(uint8_t sensor_index);
float tmp422GetLocalTemperature(void);
