#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "board_conf.h"

static THD_FUNCTION(gps_thread, arg);
static THD_WORKING_AREA(gps_wa, 256);

static void parserInit(void);
static bool parse(uint8_t c);
static void parseTerm(void);
static int32_t parseLatLonTerm(uint8_t *degree, uint32_t *minute);
static char isDigit(char c);
static bool parseSentence(void);
static void parseGGA(void);
static void parseRMC(void);
static void parseGLL(void);
static void parseGSA(void);
static void parseGSV(void);
static void parseVTG(void);
//static uint32_t atol(const char *str);
static int getLatitude(uint8_t idx, gps_coordinates_t *coordinates);
static int getLongitude(uint8_t idx, gps_coordinates_t *coordinates);
static int getAltitude(uint8_t idx, uint16_t *alt);
static int getSpeed(uint8_t idx, uint16_t *speed);
static uint8_t getNbSat(uint8_t idx);

static bool init_done = false;
static uint8_t rxData;
static gps_data_t gpsData;
static gps_data_t gpsDataTmp;
static gps_satellites_t gpsSat;
static gps_satellites_t gpsSatTmp;
static uint8_t checksum;
static uint8_t sentenceType;
static char term[GPS_MAX_FIELD_SIZE];
static uint8_t sentenceOffset;
static uint8_t termOffset;
static bool valid;

SerialDriver *sd = GPS_UART_DRIVER;

void gps_init(void)
{
	chDbgCheck(init_done != true);

	SerialConfig sd_conf = { 57600, 0, USART_CR2_STOP_1 | USART_CR2_LINEN, 0 };
	sdStart(sd, &sd_conf);

	parserInit();

	chMtxObjectInit(&gpsData.mtx);

	chThdCreateStatic(gps_wa, sizeof(gps_wa), LOWPRIO, gps_thread, NULL);

	init_done = true;
}

static THD_FUNCTION(gps_thread, arg)
{
	chDbgCheck(init_done != false);

	(void) arg;

	chRegSetThreadName("GPS");

	while (true) {
		chnRead((BaseChannel * )sd, &rxData, 1);
		parse(rxData);
	}
}

static void parserInit(void)
{
	sentenceType = GPS_SENTENCE_UNKNOWN;
	valid = false;
	sentenceOffset = termOffset = 0;
	memset(term, 0, sizeof(term));
}

static bool parse(uint8_t c)
{
	switch (c) {
	case ',':
// end of term
		parseTerm();
		sentenceOffset++;
		termOffset = 0;
		checksum ^= c;
		break;

	case '*':
// checksum
		sentenceOffset++;
		termOffset = 0;
		break;

	case '\r':
	case '\n':
// end of sentence/term
		parseTerm();
		parseSentence();
		sentenceOffset = termOffset = 0;
		break;

	case '$':
// start of sentence
		sentenceType = GPS_SENTENCE_UNKNOWN;
		sentenceOffset = termOffset = checksum = 0;
		valid = false;
		break;

	default:
// Regular data
		if (termOffset < sizeof(term) - 1)
			term[termOffset++] = c;
		checksum ^= c;
		break;
	}

	return valid;
}

static void parseTerm(void)
{
	// First term is always sentence type
	if (sentenceOffset == 0) {
		if (strncmp(term, GPS_GPGGA_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPGGA;
		} else if (strncmp(term, GPS_GPRMC_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPRMC;
		} else if (strncmp(term, GPS_GPGLL_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPGLL;
		} else if (strncmp(term, GPS_GPGSA_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPGSA;
		} else if (strncmp(term, GPS_GPGSV_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPGSV;
		} else if (strncmp(term, GPS_GPVTG_TERM, termOffset) == 0) {
			sentenceType = GPS_SENTENCE_GPVTG;
		}
		return;
	}

	// Null-terminate the term
	if (termOffset < sizeof(term))
		term[termOffset] = '\0';

// Other terms vary depending on sentence type
	switch (sentenceType) {
	case GPS_SENTENCE_GPGGA:
		parseGGA();
		break;

	case GPS_SENTENCE_GPRMC:
		parseRMC();
		break;

	case GPS_SENTENCE_GPGLL:
		parseGLL();
		break;

	case GPS_SENTENCE_GPGSA:
		parseGSA();
		break;

	case GPS_SENTENCE_GPGSV:
		parseGSV();
		break;

	case GPS_SENTENCE_GPVTG:
		parseVTG();
		break;

	case GPS_SENTENCE_UNKNOWN:
	default:
		break;
	}
}

static void parseGGA(void)
{
	switch (sentenceOffset) {
	case 1: // UTC
		break;
	case 2: // Latitude
		gpsDataTmp.lat = parseLatLonTerm(&gpsDataTmp.latitude.degree,
		                                 &gpsDataTmp.latitude.minute);
		break;
	case 3: // N or S (North or South)
		if (strncmp(term, "S", 1) == 0)
			gpsDataTmp.lat *= -1;
		gpsDataTmp.latitude.hemisphere = *term;
		break;
	case 4: // Longitude
		gpsDataTmp.lon = parseLatLonTerm(&gpsDataTmp.longitude.degree,
		                                 &gpsDataTmp.longitude.minute);
		break;
	case 5: // E or W (East or West)
		if (strncmp(term, "W", 1) == 0)
			gpsDataTmp.lon *= -1;
		gpsDataTmp.longitude.hemisphere = *term;
		break;
	case 6: // GPS Quality Indicator (fix type)
		gpsDataTmp.dataGood = (strncmp(term, "0", 1) > 0) ? 1 : 0;
		if (strncmp(term, "0", 1) == 0)
			gpsDataTmp.fixType = 0;
		break;
	case 7: // Number of satellites in view
		gpsSatTmp.numVisible = atoi(term);
		break;
	case 8: // Horizontal Dilution of precision (meters)
		break;
	case 9: // Antenna Altitude above/below mean-sea-level (geoid) (in meters)
		gpsDataTmp.altitude = atoi(term);
		break;
	case 10: // Units of antenna altitude, meters
// Ignored, always M
		break;
	case 11: // Geoidal separation
		break;
	case 12: // Units of geoidal separation, meters
// Ignored, always M
		break;
	case 13: // Age of differential GPS data
		break;
	case 14: // Differential reference station ID
		break;
	case 15: // checksum
		gpsDataTmp.checksum = atoi(term);
		break;
	default:
		break;
	}
}

static void parseRMC(void)
{
	switch (sentenceOffset) {
	case 1: // UTC
		break;
	case 2: // Status, V=Navigation receiver warning A=Valid
		gpsDataTmp.dataGood = (strncmp(term, "A", 1) == 0) ? 1 : 0;
		break;
	case 3: // Latitude
		break;
	case 4: // N or S (North or South)
		if (strncmp(term, "S", 1) == 0)
			gpsDataTmp.lat *= -1;
		break;
	case 5: // Longitude
		break;
	case 6: // E or W (East or West)
		if (strncmp(term, "W", 1) == 0)
			gpsDataTmp.lon *= -1;
		break;
	case 7: // Speed over ground, knots
		break;
	case 8: // Track made good, degrees true
		break;
	case 9: // Date, ddmmyy
		break;
	case 10: // Magnetic Variation, degrees
// I don't appear to have this on my GPS
		break;
	case 11: // E or W
// I don't appear to have this on my GPS
		break;
	case 12: // FAA mode indicator (NMEA 2.3 and later)
		break;
	case 13: // checksum
		gpsDataTmp.checksum = atoi(term);
		break;
	default:
		break;
	}
}

static void parseGLL(void)
{
	switch (sentenceOffset) {
	case 1: // Latitude
		break;
	case 2: // N or S (North or South)
		if (strncmp(term, "S", 1) == 0)
			gpsDataTmp.lat *= -1;
		break;
	case 3: // Longitude
		break;
	case 4: // E or W (East or West)
		if (strncmp(term, "W", 1) == 0)
			gpsDataTmp.lon *= -1;
		break;
	case 5: // UTC
		break;
	case 6: // Status A - Data Valid, V - Data Invalid
		gpsDataTmp.dataGood = (strncmp(term, "A", 1) == 0) ? 1 : 0;
		break;
	case 7: // FAA mode indicator (NMEA 2.3 and later)
		break;
	case 8: // checksum
		gpsDataTmp.checksum = atoi(term);
		break;
	default:
		break;
	}
}

static void parseGSA(void)
{
	switch (sentenceOffset) {
	case 1: // Selection mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 3D/2D
		break;
	case 2: // Mode (1 = no fix, 2 = 2D fix, 3 = 3D fix)
		gpsDataTmp.dataGood = (strncmp(term, "0", 1) > 0) ? 1 : 0;
		gpsDataTmp.fixType = term[0];
		break;
	case 3: // ID of 1st satellite used for fix
		gpsSatTmp.prn[0] = atoi(term);
		break;
	case 4: // ID of 2nd satellite used for fix
		gpsSatTmp.prn[1] = atoi(term);
		break;
	case 5: // ID of 3rd satellite used for fix
		gpsSatTmp.prn[2] = atoi(term);
		break;
	case 6: // ID of 4th satellite used for fix
		gpsSatTmp.prn[3] = atoi(term);
		break;
	case 7: // ID of 5th satellite used for fix
		gpsSatTmp.prn[4] = atoi(term);
		break;
	case 8: // ID of 6th satellite used for fix
		gpsSatTmp.prn[5] = atoi(term);
		break;
	case 9: // ID of 7th satellite used for fix
		gpsSatTmp.prn[6] = atoi(term);
		break;
	case 10: // ID of 8th satellite used for fix
		gpsSatTmp.prn[7] = atoi(term);
		break;
	case 11: // ID of 9th satellite used for fix
		gpsSatTmp.prn[8] = atoi(term);
		break;
	case 12: // ID of 10th satellite used for fix
		gpsSatTmp.prn[9] = atoi(term);
		break;
	case 13: // ID of 11th satellite used for fix
		gpsSatTmp.prn[10] = atoi(term);
		break;
	case 14: // ID of 12th satellite used for fix
		gpsSatTmp.prn[11] = atoi(term);
		break;
	case 15: // PDOP
		break;
	case 16: // HDOP
		break;
	case 17: // VDOP
		break;
	case 18: // checksum
		gpsDataTmp.checksum = atoi(term);
		break;
	default:
		break;
	}
}

static void parseGSV(void)
{
	switch (sentenceOffset) {
	case 1: // total number of GSV messages to be transmitted in this group
		break;
	default:
		break;
	}
}

static void parseVTG(void)
{
	switch (sentenceOffset) {
	case 1: // Track Degrees
		break;
	case 2: // T = True
// Ignored, always T
		break;
	case 3: // Track Degrees
		break;
	case 4: // M = Magnetic
// Ignored, always M
		break;
	case 5: // Speed Knots
		break;
	case 6: // N = Knots
// Ignored, always N
		break;
	case 7: // Speed Kilometers Per Hour
		gpsDataTmp.speed = atoi(term);
		break;
	case 8: // K = Kilometers Per Hour
// Ignored, always K
		break;
	case 9: // FAA mode indicator (NMEA 2.3 and later)
		break;
	case 10: // checksum
		gpsDataTmp.checksum = atoi(term);
		break;
	default:
		break;
	}
}

static int32_t parseLatLonTerm(uint8_t *degree, uint32_t *minute)
{
	// Convert characters to the left of the decimal point to a number
	unsigned long left = atol(term);

	// Extract the minutes (eg in 4533, the minutes are 33)
	unsigned long tenk_minutes = (left % 100UL) * 10000UL;

	// Move pointer to the first non-digit
	char *p;
	for (p = term; isDigit(*p); ++p)
		;

	// If we found a decimal point, extract seconds
	if (*p == '.') {
		unsigned long mult = 1000;
		while (isDigit(*++p)) {
			tenk_minutes += mult * (*p - '0'); // Add the seconds after the minutes
			mult /= 10;
		}
	}

	*degree = left / 100;
	*minute = tenk_minutes / 6;

	return (left / 100) * 100000 + tenk_minutes /
	       6; // Extract degrees from left (45), stick it at the front, add minutes converted
}

/*
 * Convert an array of characters to a long
 */
/*static uint32_t atol(const char *str) {
    long ret = 0;
    while (isDigit(*str))
        ret = 10 * ret + *str++ - '0';
    return ret;
}*/

/*
 * Is this character a digit from 0-9?
 */
static char isDigit(char c)
{
	return c >= '0' && c <= '9';
}

/*
 * Parse a just-completed sentence, verify checksum, and copy working storage to public storage in a thread-safe manner
 * Sets gps_state.valid if sentence was valid and copied to public storage
 */
static bool parseSentence(void)
{
	if (gpsDataTmp.checksum != checksum) {
		gpsDataTmp.valid = false;
		chMtxLock(&gpsData.mtx);
		gpsData.valid = false;
		chMtxUnlock(&gpsData.mtx);
		return false;
	}

	gpsDataTmp.valid = true;

	switch (sentenceType) {
	case GPS_SENTENCE_GPGGA:
		chMtxLock(&gpsData.mtx);
		memcpy(&gpsData, &gpsDataTmp, sizeof(gps_data_t));
		chMtxUnlock(&gpsData.mtx);
		break;
	case GPS_SENTENCE_GPRMC:
		break;
	case GPS_SENTENCE_GPGLL:
		break;
	case GPS_SENTENCE_GPGSA:
		break;
	case GPS_SENTENCE_GPGSV:
		break;
	case GPS_SENTENCE_GPVTG:
		break;
	default:
		break;
	}
	return true;
}
