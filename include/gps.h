#pragma once

#include <stdbool.h>
#include "ch.h"
#include "hal.h"

// https://github.com/biiont/gpsd/blob/master/www/NMEA.txt

//#define GPS_UBX_PROTOCOL
#define GPS_UBX_FRAME_SIZE	128
#define GPS_UBX_SYNC1		0xB5
#define GPS_UBX_SYNC2		0x62
#define GPS_UBX_CLASS_NAV	1
#define GPS_UBX_ID_POSLLH	2
#define GPS_UBX_ID_SOL		6
#define GPS_UBX_ID_VELNED	0x12

#define GPS_MPH_PER_KNOT    1.15077945
#define GPS_MPS_PER_KNOT    0.51444444
#define GPS_KMPH_PER_KNOT   1.852
#define GPS_MILES_PER_METER 0.00062137112
#define GPS_KM_PER_METER    0.001
#define GPS_FEET_PER_METER  3.2808399

#define GPS_SENTENCE_UNKNOWN 0
#define GPS_SENTENCE_GPGGA 1 // Time, position, and fix/satellite info
#define GPS_SENTENCE_GPRMC 2 // Time, position, some speed/heading
#define GPS_SENTENCE_GPGLL 3 // Time, position
#define GPS_SENTENCE_GPGSA 4 // GPS DOP and active satellites
#define GPS_SENTENCE_GPGSV 5 // Satellites in view
#define GPS_SENTENCE_GPVTG 6 // Track made good and Ground speed

#define GPS_GPGGA_TERM      "GPGGA"
#define GPS_GPRMC_TERM      "GPRMC"
#define GPS_GPGLL_TERM      "GPGLL"
#define GPS_GPGSA_TERM      "GPGSA"
#define GPS_GPGSV_TERM      "GPGSV"
#define GPS_GPVTG_TERM      "GPVTG"

#define GPS_MAX_FIELD_SIZE 15

typedef struct {
	uint8_t sync[2];
	uint8_t id;
	uint16_t size;
	uint8_t payload[GPS_UBX_FRAME_SIZE];
	uint8_t checksum[2];
} gps_ubx_msg_t;

typedef struct {
	uint8_t degree;
	uint32_t minute;
	char hemisphere;
} gps_coordinates_t;

typedef struct {
	uint16_t speed;
	uint16_t altitude;
	gps_coordinates_t longitude;
	gps_coordinates_t latitude;
	uint8_t nb_sat;
	uint8_t fix;
	bool valid;
	uint8_t fixType;
	uint8_t dataGood;
	int32_t lat;
	int32_t lon;
	int32_t alt;
	uint16_t eph;
	uint16_t epv;
	uint16_t vel;
	uint16_t cog;
	uint8_t checksum;
	mutex_t mtx;
} gps_data_t;

typedef struct {
	uint8_t numVisible; // Number of satellites visible
	uint8_t prn[12]; // Global satellite ID
	uint8_t elevation[12]; // Elevation (0: right on top of receiver, 90: on the horizon) of satellite
	uint8_t azimuth[12]; // Direction of satellite, 0: 0 deg, 255: 360 deg.
	uint8_t snr[12]; // Signal to noise ratio of satellite
	mutex_t mtx;
} gps_satellites_t;

void gps_init(void);
void gps_getCoordinates(void);
int gps_get(gps_data_t* gps_data);
