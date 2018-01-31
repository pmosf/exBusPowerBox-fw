#ifndef GPS_H
#define	GPS_H

#include <cstdint>
#include <vector>
#include <string>

#include "stm32f4xx_hal.h"
#include "usart.h"

//#define GPS_UBX_PROTOCOL
#define GPS_UBX_FRAME_SIZE	128
#define GPS_UBX_SYNC1		0xB5
#define GPS_UBX_SYNC2		0x62
#define GPS_UBX_CLASS_NAV	1
#define GPS_UBX_ID_POSLLH	2
#define GPS_UBX_ID_SOL		6
#define GPS_UBX_ID_VELNED	0x12

namespace GPS
{
	typedef struct
	{
			std::uint8_t sync[2];
			std::uint8_t id;
			std::uint16_t size;
			std::uint8_t payload[GPS_UBX_FRAME_SIZE];
			std::uint8_t checksum[2];
	} gps_ubx_msg_t;

	typedef struct
	{
			std::uint8_t degree;
			std::uint32_t minute;
			char hemisphere;
	} gps_coordinates_t;

	typedef struct
	{
			std::uint16_t speed;
			std::uint16_t altitude;
			gps_coordinates_t longitude;
			gps_coordinates_t latitude;
			std::uint8_t nb_sat;
			std::uint8_t fix;
			bool valid;
	} gps_info_t;

	class CGps
	{
		public:
			CGps();
			~CGps();
			void GetCoordinates();
			static void IrqHandler(UART_HandleTypeDef *huart);

			static std::uint8_t frame_[128];

		private:
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
	};

}

#endif	/* GPS_H */

