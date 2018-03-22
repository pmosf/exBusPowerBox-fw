#include <string.h>
#include <stdlib.h>

#include "hal.h"
#include "CGps.hpp"

namespace GPS {

  CGps::CGps(SerialDriver *uartDriver) :
      serialDriver_(uartDriver), serialConfig_(
          {57600, 0, USART_CR2_STOP_1 | USART_CR2_LINEN, 0}) {

    parserInit();
  }

  CGps::~CGps() {
  }

  void CGps::main() {
    setName("GPS");

    sdStart(serialDriver_, &serialConfig_);

    while (true) {
      chnRead((BaseChannel * )serialDriver_, &rxData_, 1);
      parse(rxData_);
    }
  }

  void CGps::parserInit() {
    sentenceType_ = GPS_SENTENCE_UNKNOWN;
    valid_ = false;
    sentenceOffset_ = termOffset_ = 0;
    memset(term_, 0, sizeof(term_));
  }

  bool CGps::parse(uint8_t c) {
    switch (c) {
      case ',':
        // end of term
        parseTerm();
        sentenceOffset_++;
        termOffset_ = 0;
        checksum_ ^= c;
        break;

      case '*':
        // checksum
        sentenceOffset_++;
        termOffset_ = 0;
        break;

      case '\r':
      case '\n':
        // end of sentence/term
        parseTerm();
        parseSentence();
        sentenceOffset_ = termOffset_ = 0;
        break;

      case '$':
        // start of sentence
        sentenceType_ = GPS_SENTENCE_UNKNOWN;
        sentenceOffset_ = termOffset_ = checksum_ = 0;
        valid_ = false;
        break;

      default:
        // Regular data
        if (termOffset_ < sizeof(term_) - 1)
          term_[termOffset_++] = c;
        checksum_ ^= c;
        break;
    }

    return valid_;
  }

  void CGps::parseTerm() {
    // First term is always sentence type
    if (sentenceOffset_ == 0) {
      if (strncmp(term_, GPS_GPGGA_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPGGA;
      }
      else if (strncmp(term_, GPS_GPRMC_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPRMC;
      }
      else if (strncmp(term_, GPS_GPGLL_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPGLL;
      }
      else if (strncmp(term_, GPS_GPGSA_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPGSA;
      }
      else if (strncmp(term_, GPS_GPGSV_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPGSV;
      }
      else if (strncmp(term_, GPS_GPVTG_TERM, termOffset_) == 0) {
        sentenceType_ = GPS_SENTENCE_GPVTG;
      }
      return;
    }

    // Null-terminate the term
    if (termOffset_ < sizeof(term_))
      term_[termOffset_] = '\0';

    // Other terms vary depending on sentence type
    switch (sentenceType_) {
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

  void CGps::parseGGA() {
    switch (sentenceOffset_) {
      case 1: // UTC
        break;
      case 2: // Latitude
        gpsDataTmp_.lat = parseLatLonTerm(gpsDataTmp_.latitude.degree,
                                          gpsDataTmp_.latitude.minute);
        break;
      case 3: // N or S (North or South)
        if (strncmp(term_, "S", 1) == 0)
          gpsDataTmp_.lat *= -1;
        gpsDataTmp_.latitude.hemisphere = *term_;
        break;
      case 4: // Longitude
        gpsDataTmp_.lon = parseLatLonTerm(gpsDataTmp_.longitude.degree,
                                          gpsDataTmp_.longitude.minute);
        break;
      case 5: // E or W (East or West)
        if (strncmp(term_, "W", 1) == 0)
          gpsDataTmp_.lon *= -1;
        gpsDataTmp_.longitude.hemisphere = *term_;
        break;
      case 6: // GPS Quality Indicator (fix type)
        gpsDataTmp_.dataGood = (strncmp(term_, "0", 1) > 0) ? 1 : 0;
        if (strncmp(term_, "0", 1) == 0)
          gpsDataTmp_.fixType = 0;
        break;
      case 7: // Number of satellites in view
        gpsSatTmp_.numVisible = atoi(term_);
        break;
      case 8: // Horizontal Dilution of precision (meters)
        break;
      case 9: // Antenna Altitude above/below mean-sea-level (geoid) (in meters)
        gpsDataTmp_.altitude = atoi(term_);
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
        gpsDataTmp_.checksum = atoi(term_);
        break;
      default:
        break;
    }
  }

  void CGps::parseRMC() {
    switch (sentenceOffset_) {
      case 1: // UTC
        break;
      case 2: // Status, V=Navigation receiver warning A=Valid
        gpsDataTmp_.dataGood = (strncmp(term_, "A", 1) == 0) ? 1 : 0;
        break;
      case 3: // Latitude
        break;
      case 4: // N or S (North or South)
        if (strncmp(term_, "S", 1) == 0)
          gpsDataTmp_.lat *= -1;
        break;
      case 5: // Longitude
        break;
      case 6: // E or W (East or West)
        if (strncmp(term_, "W", 1) == 0)
          gpsDataTmp_.lon *= -1;
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
        gpsDataTmp_.checksum = atoi(term_);
        break;
      default:
        break;
    }
  }

  void CGps::parseGLL() {
    switch (sentenceOffset_) {
      case 1: // Latitude
        break;
      case 2: // N or S (North or South)
        if (strncmp(term_, "S", 1) == 0)
          gpsDataTmp_.lat *= -1;
        break;
      case 3: // Longitude
        break;
      case 4: // E or W (East or West)
        if (strncmp(term_, "W", 1) == 0)
          gpsDataTmp_.lon *= -1;
        break;
      case 5: // UTC
        break;
      case 6: // Status A - Data Valid, V - Data Invalid
        gpsDataTmp_.dataGood = (strncmp(term_, "A", 1) == 0) ? 1 : 0;
        break;
      case 7: // FAA mode indicator (NMEA 2.3 and later)
        break;
      case 8: // checksum
        gpsDataTmp_.checksum = atoi(term_);
        break;
      default:
        break;
    }
  }

  void CGps::parseGSA() {
    switch (sentenceOffset_) {
      case 1: // Selection mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 3D/2D
        break;
      case 2: // Mode (1 = no fix, 2 = 2D fix, 3 = 3D fix)
        gpsDataTmp_.dataGood = (strncmp(term_, "0", 1) > 0) ? 1 : 0;
        gpsDataTmp_.fixType = term_[0];
        break;
      case 3: // ID of 1st satellite used for fix
        gpsSatTmp_.prn[0] = atoi(term_);
        break;
      case 4: // ID of 2nd satellite used for fix
        gpsSatTmp_.prn[1] = atoi(term_);
        break;
      case 5: // ID of 3rd satellite used for fix
        gpsSatTmp_.prn[2] = atoi(term_);
        break;
      case 6: // ID of 4th satellite used for fix
        gpsSatTmp_.prn[3] = atoi(term_);
        break;
      case 7: // ID of 5th satellite used for fix
        gpsSatTmp_.prn[4] = atoi(term_);
        break;
      case 8: // ID of 6th satellite used for fix
        gpsSatTmp_.prn[5] = atoi(term_);
        break;
      case 9: // ID of 7th satellite used for fix
        gpsSatTmp_.prn[6] = atoi(term_);
        break;
      case 10: // ID of 8th satellite used for fix
        gpsSatTmp_.prn[7] = atoi(term_);
        break;
      case 11: // ID of 9th satellite used for fix
        gpsSatTmp_.prn[8] = atoi(term_);
        break;
      case 12: // ID of 10th satellite used for fix
        gpsSatTmp_.prn[9] = atoi(term_);
        break;
      case 13: // ID of 11th satellite used for fix
        gpsSatTmp_.prn[10] = atoi(term_);
        break;
      case 14: // ID of 12th satellite used for fix
        gpsSatTmp_.prn[11] = atoi(term_);
        break;
      case 15: // PDOP
        break;
      case 16: // HDOP
        break;
      case 17: // VDOP
        break;
      case 18: // checksum
        gpsDataTmp_.checksum = atoi(term_);
        break;
      default:
        break;
    }
  }

  void CGps::parseGSV() {
    switch (sentenceOffset_) {
      case 1: // total number of GSV messages to be transmitted in this group
        break;
      default:
        break;
    }
  }

  void CGps::parseVTG() {
    switch (sentenceOffset_) {
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
        gpsDataTmp_.speed = atoi(term_);
        break;
      case 8: // K = Kilometers Per Hour
        // Ignored, always K
        break;
      case 9: // FAA mode indicator (NMEA 2.3 and later)
        break;
      case 10: // checksum
        gpsDataTmp_.checksum = atoi(term_);
        break;
      default:
        break;
    }
  }

  int32_t CGps::parseLatLonTerm(uint8_t& degree, uint32_t& minute) {
    // Convert characters to the left of the decimal point to a number
    unsigned long left = atol(term_);

    // Extract the minutes (eg in 4533, the minutes are 33)
    unsigned long tenk_minutes = (left % 100UL) * 10000UL;

    // Move pointer to the first non-digit
    char *p;
    for (p = term_; isDigit(*p); ++p)
      ;

    // If we found a decimal point, extract seconds
    if (*p == '.') {
      unsigned long mult = 1000;
      while (isDigit(*++p)) {
        tenk_minutes += mult * (*p - '0'); // Add the seconds after the minutes
        mult /= 10;
      }
    }

    degree = left / 100;
    minute = tenk_minutes / 6;

    return (left / 100) * 100000 + tenk_minutes / 6; // Extract degrees from left (45), stick it at the front, add minutes converted
  }

  /*
   * Convert an array of characters to a long
   */
  uint32_t CGps::atol(const char *str) {
    long ret = 0;
    while (isDigit(*str))
      ret = 10 * ret + *str++ - '0';
    return ret;
  }

  /*
   * Is this character a digit from 0-9?
   */
  char CGps::isDigit(char c) {
    return c >= '0' && c <= '9';
  }

  /*
   * Parse a just-completed sentence, verify checksum, and copy working storage to public storage in a thread-safe manner
   * Sets gps_state.valid if sentence was valid and copied to public storage
   */
  bool CGps::parseSentence() {
    if (gpsDataTmp_.checksum != checksum_) {
      gpsDataTmp_.valid = false;
      gpsData_.mtx.lock();
      gpsData_.valid = false;
      gpsData_.mtx.unlock();
      return false;
    }

    gpsDataTmp_.valid = true;

    switch (sentenceType_) {
      case GPS_SENTENCE_GPGGA:
        gpsData_.mtx.lock();
        memcpy(&gpsData_, &gpsDataTmp_, sizeof(gps_data_t));
        gpsData_.mtx.unlock();
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
}

