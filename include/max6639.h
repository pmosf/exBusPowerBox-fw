#pragma once

#define IIC_MAX6639_ADDR		(0x58 >> 1)

#define MAX6639_TEMP1_ADDR		0
#define MAX6639_TEMP2_ADDR		1
#define MAX6639_STATUS_ADDR		2
#define MAX6639_OUTMASK_ADDR	3
#define MAX6639_GCONF_ADDR		4
#define MAX6639_EXTENDED_TEMP1_ADDR		5
#define MAX6639_EXT_TEMP2_ADDR		6
#define MAX6639_FAN1_CONF_ADDR	0x10
#define MAX6639_FANTACH1_ADDR	0x20
#define MAX6639_FAN1_PPR_ADDR	0x24
#define MAX6639_DEVID_ADDR		0x3D
#define MAX6639_MFRID_ADDR		0x3E
#define MAX6639_DEVREV_ADDR		0x3F

#define MAX6639_DEVICE_ID		0x58
#define MAX6639_MFR_ID			0x4D
#define MAX6639_DEVICE_REV		0

#define MAX6639_TX_BUFFER_SIZE  4
#define MAX6639_RX_BUFFER_SIZE  4

bool max6639Init(void);
float max6639GetExtTemperature(uint8_t sensor_index);
float max6639_get_local_temperature(void);
uint16_t max6639_get_rpm(void);
