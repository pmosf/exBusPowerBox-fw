#ifndef __EXSENSOR_H
#define __EXSENSOR_H

#include <cstdint>
#include <vector>
#include <string>

#define JETI_EX_ID_CHANNEL			0x31
#define JETI_EX_ID_TELEMETRY		0x3A
#define JETI_EX_ID_JETIBOX			0x3B

#define JETI_MENU_HEADER			0xFE
#define JETI_MENU_FOOTER			0xFF

#define JETI_SENSOR_HEADER			0x7E
#define JETI_SENSOR_EX_ID			0x9F

#define JETI_SENSOR_TXT_LEN			8
#define JETI_SENSOR_DATA_LEN		10
#define JETI_SENSOR_PKT_TXT_TYPE	0
#define JETI_SENSOR_PKT_DATA_TYPE   0x40

#define JETI_SENSOR_INT6_T			0
#define JETI_SENSOR_INT14_T			1
#define JETI_SENSOR_INT22_T			4
#define JETI_SENSOR_TIMEDATE_T		5
#define JETI_SENSOR_INT30_T			8
#define JETI_SENSOR_GPS_T			9
#define JETI_SENSOR_NEG_VALUE		0x80
#define JETI_SENSOR_POS_VALUE		0

#define JETI_GPS_LONGITUDE			0x20000000
#define JETI_GPS_LATITUDE			0
#define JETI_GPS_WEST				0x40000000
#define JETI_GPS_EAST				0
#define JETI_GPS_SOUTH				0x40000000
#define JETI_GPS_NORTH				0

namespace Jeti
{
	namespace Sensor
	{
		enum class Type
			: std::uint8_t
			{
				int6 = 0, int14 = 1, int22 = 4, timeDate = 5, int30 = 8, gps = 9,
		};

		typedef struct
		{
				struct
				{
						std::uint8_t degree;
						std::uint32_t minute;
						char hemisphere;
				} longitude;
				struct
				{
						std::uint8_t degree;
						std::uint32_t minute;
						char hemisphere;
				} latitude;
		} gps_t;

		typedef struct
		{
				std::uint8_t dataType;
				std::uint8_t id;
				std::uint8_t *formattedValue;
		} data_desc_t;

		class CExSensor
		{
			public:
				CExSensor();
				CExSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::string unit, Type type);
				~CExSensor();
				std::vector<std::uint8_t>& GetTextDescriptor();
				virtual void SetValue(float val);
				virtual void SetValue(std::int8_t val);
				virtual void SetValue(std::int16_t val);
				virtual void SetValue(std::int32_t val);
				virtual void SetGpsValue(gps_t *val);

			protected:
				std::vector<std::uint8_t> text_;
				std::uint8_t *formattedValue_;
				int precision_;
		};

		class CExVoltageSensor: public CExSensor
		{
			public:
				CExVoltageSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::uint8_t* formattedValue);
				void SetValue(float val);
		};

		class CExCurrentSensor: public CExSensor
		{
			public:
				CExCurrentSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::string unit, std::uint8_t* formattedValue);
				void SetValue(float val);
		};

		class CExTemperatureSensor: public CExSensor
		{
			public:
				CExTemperatureSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::uint8_t* formattedValue);
				void SetValue(float val);
		};

		class CExGPS: public CExSensor
		{
			public:
				CExGPS(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id);
				void CExGpsSetCoordinates(gps_t* coordinates);
				void CExGpsGetLongitude(std::uint8_t* formattedValue);
				void CExGpsGetLattitude(std::uint8_t* formattedValue);
				void CExGpsGetSpeed(std::uint8_t* formattedValue);

			private:
				gps_t gpsCoordinates_;
		};
	}
}

#endif
