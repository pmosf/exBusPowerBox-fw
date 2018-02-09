#ifndef GPS_H
#define	GPS_H

#include <cstdint>
#include <vector>
#include <string>

#include "ch.hpp"
#include "hal.h"

//#define GPS_UBX_PROTOCOL
#define GPS_UBX_FRAME_SIZE	128
#define GPS_UBX_SYNC1		0xB5
#define GPS_UBX_SYNC2		0x62
#define GPS_UBX_CLASS_NAV	1
#define GPS_UBX_ID_POSLLH	2
#define GPS_UBX_ID_SOL		6
#define GPS_UBX_ID_VELNED	0x12

namespace GPS {
  typedef struct {
    std::uint8_t sync[2];
    std::uint8_t id;
    std::uint16_t size;
    std::uint8_t payload[GPS_UBX_FRAME_SIZE];
    std::uint8_t checksum[2];
  } gps_ubx_msg_t;

  typedef struct {
    std::uint8_t degree;
    std::uint32_t minute;
    char hemisphere;
  } gps_coordinates_t;

  typedef struct {
    std::uint16_t speed;
    std::uint16_t altitude;
    gps_coordinates_t longitude;
    gps_coordinates_t latitude;
    std::uint8_t nb_sat;
    std::uint8_t fix;
    bool valid;
  } gps_info_t;

  class CGps: public chibios_rt::BaseStaticThread<128> {
  public:
    CGps(SerialDriver *uartDriver);
    ~CGps();
    virtual void main();
    void getCoordinates();
    int getInfo(gps_info_t* gps_info);

    static std::uint8_t frame_[128];

  private:

    SerialDriver *serialDriver_;
    SerialConfig serialConfig_;

#ifndef GPS_UBX_PROTOCOL
    static std::uint8_t frameIdx_;
    static bool isFrameStarted_;
    static bool isFrameRdy_;
#else
    static volatile uint8_t frame_idx;
    static volatile uint8_t frame_started;
    static volatile uint8_t frame_rdy;
    static volatile gps_ubx_msg_t ubx_msg;
#endif
    int getLatitude(uint8_t idx, gps_coordinates_t* coordinates);
    int getLongitude(uint8_t idx, gps_coordinates_t* coordinates);
    int getAltitude(uint8_t idx, uint16_t* alt);
    int getSpeed(uint8_t idx, uint16_t* speed);
    std::uint8_t getNbSat(uint8_t idx);
    /* static void txEnd1CB(UARTDriver *uartp);
     static void txEnd2CB(UARTDriver *uartp);
     static void rxendCB(UARTDriver *uartp);
     void rxCharCB(UARTDriver *uartp, uint16_t c);
     void rxErrCB(UARTDriver *uartp, uartflags_t e);*/
  };

}

#endif	/* GPS_H */

