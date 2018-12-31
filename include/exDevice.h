#pragma once
#include <stdint.h>
#include "utils.h"

#define EX_MAX_NB_SENSORS           31
#define EX_MAX_PKT_LEN              26
#define EX_TYPE_ID_SIZE             2

#define JETI_MENU_HEADER            0xFE
#define JETI_MENU_FOOTER            0xFF

#define JETI_SENSOR_HEADER          0x7E
#define JETI_SENSOR_EX_ID           0x9F

#define JETI_SENSOR_TXT_LEN         8
#define JETI_SENSOR_DATA_LEN        6
#define JETI_SENSOR_PKT_TXT_TYPE    0
#define JETI_SENSOR_PKT_DATA_TYPE   0x40
#define JETI_SENSOR_DATA_LEN_MSK    0x3F

#define JETI_SENSOR_NEG_VALUE       0x80
#define JETI_SENSOR_POS_VALUE       0

#define JETI_GPS_LONGITUDE          0x20000000
#define JETI_GPS_LATITUDE           0
#define JETI_GPS_WEST               0x40000000
#define JETI_GPS_EAST               0
#define JETI_GPS_SOUTH              0x40000000
#define JETI_GPS_NORTH              0

#define JETI_DECIMAL_MASK(decimals) (decimals << 5)
#define JETI_EX_SENSOR_COUNT        (ARRAYLEN(device)-1)

#define JETI_EX_DATA_LEN_MAX        (27 - JETI_SENSOR_DATA_LEN)

typedef struct {
	const uint8_t id;
	const char *label;
	const char *unit;
	const uint8_t dataType;
	const uint8_t decimals;
} exdevice_sensor_list_t;


typedef struct {
#ifdef JETI_EXBUS
	uint8_t header;
#else
	uint8_t header[2];
#endif
	uint8_t type_len;// tt-llllll
	uint8_t sn[4];
	uint8_t rsvd;
	uint8_t id;
	uint8_t length;// ddddd-uuu
	uint8_t data[JETI_EX_DATA_LEN_MAX];
} ex_pkt_t;

void jetiExInit(void);
uint8_t jetiExGetNextTxtFrame(ex_pkt_t *pkt);
uint8_t jetiExGetNextDataPkt(ex_pkt_t *pkt);
