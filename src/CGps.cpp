#include "hal.h"
#include "CGps.hpp"

namespace GPS {
  std::uint8_t CGps::frame_[128];
  std::uint8_t CGps::frameIdx_;
  bool CGps::isFrameStarted_;
  bool CGps::isFrameRdy_;

  CGps::CGps(SerialDriver *uartDriver) :
    serialDriver_(uartDriver), serialConfig_({9600, 0, USART_CR2_STOP_1, 0}) {
#ifndef GPS_UBX_PROTOCOL
    isFrameRdy_ = 0;
    isFrameStarted_ = 0;
    frameIdx_ = 0;
#else
    frame_idx_ = 0;
    frame_started_ = 0;
    frame_rdy_ = 0;
#endif
  }

  CGps::~CGps() {
  }

  void CGps::main() {
    sdStart(serialDriver_, &serialConfig_);
  }

  int CGps::getLatitude(uint8_t idx, gps_coordinates_t* coordinates) {
    uint8_t i;

    coordinates->degree = 0;
    coordinates->minute = 0;
    coordinates->hemisphere = 'N';
#ifndef GPS_UBX_PROTOCOL
    if (frame_[idx + 1] == ',')
      return -1;

    i = 1;
    while (frame_[idx] != '.' && frame_[idx] != ',') {
      if (i < 3) {
        coordinates->degree = coordinates->degree * 10 + frame_[idx] - 48;
      }
      else
        coordinates->minute = coordinates->minute * 10 + frame_[idx] - 48;
      i++;
      idx++;
    }

    if (frame_[idx] == ',')
      return -1;

    idx++;
    i = 0;
    while (i < 3 && frame_[idx] != ',') {
      coordinates->minute = coordinates->minute * 10 + frame_[idx] - 48;
      idx++;
      i++;
    }

    while (frame_[idx++] != ',')
      if (idx > 127)
        return -1;

    coordinates->hemisphere = frame_[idx];
#else
#endif
    return 0;
  }

  int CGps::getLongitude(uint8_t idx, gps_coordinates_t* coordinates) {
    uint8_t i;
    coordinates->degree = 0;
    coordinates->minute = 0;
    coordinates->hemisphere = 'E';
#ifndef GPS_UBX_PROTOCOL
    if (frame_[idx + 1] == ',')
      return -1;

    i = 1;
    while (frame_[idx] != '.' && frame_[idx] != ',') {
      if (i < 4) {
        coordinates->degree = coordinates->degree * 10 + frame_[idx] - 48;
      }
      else
        coordinates->minute = coordinates->minute * 10 + frame_[idx] - 48;
      i++;
      idx++;
    }

    if (frame_[idx] == ',')
      return -1;

    idx++;
    i = 0;
    while (i < 3 && frame_[idx] != ',') {
      coordinates->minute = coordinates->minute * 10 + frame_[idx] - 48;
      idx++;
      i++;
    }

    while (frame_[idx++] != ',')
      if (idx > 127)
        return -1;

    coordinates->hemisphere = frame_[idx];
#else
#endif	
    return 0;
  }

  int CGps::getAltitude(uint8_t idx, uint16_t* alt) {
    uint8_t i;
    uint16_t altitude = 0;
#ifndef GPS_UBX_PROTOCOL	
    if (frame_[idx + 1] == ',') {
      return -1;
    }

    i = 1;
    while (idx < 128) {
      if (frame_[idx++] == ',')
        break;
      i++;
    }
    if (idx > 127)
      return -1;

    idx -= i;
    while (i > 1) {
      if (frame_[idx] != '.') {
        altitude = altitude * 10 + frame_[idx] - 48;
      }
      idx++;
      i--;
    }
#else
#endif	
    *alt = altitude;

    return 0;
  }

  std::uint8_t CGps::getNbSat(uint8_t idx) {
    uint8_t i;
    uint8_t nb_sat;
#ifndef GPS_UBX_PROTOCOL
    if (frame_[idx] == ',') {
      return 0;
    }

    i = 1;
    while (idx < 128) {
      if (frame_[idx++] == ',')
        break;
      i++;
    }
    if (idx > 127)
      return 0;

    idx -= i;
    if (i == 1) {
      nb_sat = frame_[idx] - 48;
    }
    else
      while (i--) {
        nb_sat = nb_sat * 10 + frame_[idx++] - 48;
      }
#else
#endif
    return nb_sat;
  }

  int CGps::getSpeed(uint8_t idx, uint16_t* speed) {
    uint16_t tmp = 0;
#ifndef GPS_UBX_PROTOCOL
    while (frame_[idx] != ',') {
      if (frame_[idx] != '.') {
        tmp = tmp * 10 + frame_[idx] - 48;
      }
      else {
        tmp = tmp * 10 + frame_[idx + 1] - 48;
        break;
      }
      idx++;
      if (idx > 127)
        return -1;
    }
#else
#endif	
    *speed = tmp;

    return 0;
  }

  int CGps::getInfo(gps_info_t* gps_info) {
    uint16_t idx, i;
#ifndef GPS_UBX_PROTOCOL
    uint8_t longitude_idx;
    uint8_t latitude_idx;
    uint8_t altitude_idx;
    uint8_t nb_sat_idx;
    uint8_t fix_idx;
    uint8_t speed_idx;
#else
#endif
    uint8_t invalid_info = 1;
    gps_coordinates_t gps_coordinates;

    gps_info->valid = false;
#ifndef GPS_UBX_PROTOCOL
    if (!isFrameRdy_)
      return -1;

    i = 0;
    idx = 0;

    for (;;) {
      // $GPGGA
      if (frame_[1] == 'G' && frame_[2] == 'P' && frame_[3] == 'G'
          && frame_[4] == 'G' && frame_[5] == 'A') {
        // latitude
        while (i != 2 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        latitude_idx = idx;

        // longitude
        while (i != 4 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        longitude_idx = idx;

        // fix
        while (i != 6 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        fix_idx = idx;

        // finding number of satelites
        while (i != 7 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        nb_sat_idx = idx;

        // altitude
        while (i != 9 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        altitude_idx = idx;

        if (!getAltitude(altitude_idx, &i)) {
          gps_info->altitude = i;
        }
        if (!getLatitude(latitude_idx, &gps_coordinates)) {
          gps_info->latitude.degree = gps_coordinates.degree;
          gps_info->latitude.hemisphere = gps_coordinates.hemisphere;
          gps_info->latitude.minute = gps_coordinates.minute;
        }
        if (!getLongitude(longitude_idx, &gps_coordinates)) {
          gps_info->longitude.degree = gps_coordinates.degree;
          gps_info->longitude.hemisphere = gps_coordinates.hemisphere;
          gps_info->longitude.minute = gps_coordinates.minute;
        }
        invalid_info = 0;
        break;
      }

      // GPVTG
      if (frame_[1] == 'G' && frame_[2] == 'P' && frame_[3] == 'V'
          && frame_[4] == 'T' && frame_[5] == 'G') {
        i = 0;
        while (i != 7 && idx < 128) {
          if (frame_[idx++] == ',')
            i++;
        }
        if (idx > 127)
          break;
        speed_idx = idx;

        if (!getSpeed(speed_idx, &i)) {
          gps_info->speed = i;
        }
        invalid_info = 0;
      }

      break;
    }

    for (i = 0; i < 6; i++)
      frame_[i] = 0;

    isFrameRdy_ = 0;
#else
    if (!frame_rdy || ubx_msg.class != GPS_UBX_CLASS_NAV) return -1;
    switch (ubx_msg.id)
    {
      case GPS_UBX_ID_POSLLH:
      gps_info->altitude = (uint16_t)((double)((ubx_msg.payload[19] << 24) | (ubx_msg.payload[18] << 16) | (ubx_msg.payload[17] << 8) | ubx_msg.payload[16]) / 10.0);
      gps_info->longitude.degree = (uint16_t)((double)((ubx_msg.payload[19] << 24) | (ubx_msg.payload[18] << 16) | (ubx_msg.payload[17] << 8) | ubx_msg.payload[16]) / 10.0);
      invalid_info = 0;
      break;
      case GPS_UBX_ID_SOL:
      invalid_info = 0;
      break;
      case GPS_UBX_ID_VELNED:
      gps_info->speed = (uint16_t)((double)((ubx_msg.payload[23] << 24) | (ubx_msg.payload[22] << 16) | (ubx_msg.payload[21] << 8) | ubx_msg.payload[20]) / 3600.);
      invalid_info = 0;
      break;
      default: break;
    }
#endif
    gps_info->valid = !invalid_info;

    return invalid_info;
  }
#if 0
  void CGps::rxendCB(UARTDriver *uartp) {
    int c = 0;

    (void)uartp;

    if ((frameIdx_ > 127) || isFrameRdy_) {
      isFrameStarted_ = 0;
      frameIdx_ = 0;
      goto _exit;
    }
#ifndef GPS_UBX_PROTOCOL
    if (c == '$') {
      frame_[frameIdx_++] = '$';
      isFrameStarted_ = 1;
      goto _exit;
    }

    if (!isFrameStarted_)
      goto _exit;

    if (c == '\n' || c == '\r') {
      isFrameStarted_ = 0;
      frameIdx_ = 0;
      goto _exit;
    }

    frame_[frameIdx_++] = c;
    if (c == '*') {
      isFrameStarted_ = 0;
      isFrameRdy_ = 1;
      frameIdx_ = 0;
    }
#else
    frame[frame_idx++] = c;
    if (frame_idx == 2)
    {
      if (ubx_msg.sync[0] == GPS_UBX_SYNC1 && ubx_msg.sync[1] == GPS_UBX_SYNC2) frame_started = 1;
      else frame_idx = 0;
      goto exit;
    }

    if (frame_idx < 6) break;

    if (frame_idx == 6)
    {
      ubx_msg.size = (frame[5] << 8) | frame[4];
      goto exit;
    }

    if (frame_idx == ubx_msg.size + 2)
    {
      frame_started = 0;
      frame_rdy = 1;
      frame_idx = 0;
    }
#endif
    _exit: c = 0;
  }

  void CGps::txEnd1CB(UARTDriver *uartp) {
    (void)uartp;
  }

  void CGps::txEnd2CB(UARTDriver *uartp) {
    (void)uartp;
  }

  void CGps::rxCharCB(UARTDriver *uartp, uint16_t c) {
    (void)uartp;
    (void)c;
  }

  void CGps::rxErrCB(UARTDriver *uartp, uartflags_t e) {
    (void)uartp;
    (void)e;
  }
#endif
}

