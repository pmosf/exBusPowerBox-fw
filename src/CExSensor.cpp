#include "CExSensor.hpp"
#include "crc.h"

namespace Jeti
{
	namespace Sensor
	{
		CExSensor::CExSensor()
		{
		}

		CExSensor::CExSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::string unit, Type type)
		{
			// prepare text descriptor
			text_.reserve(11 + name.length() + unit.length());
			text_.push_back(JETI_SENSOR_HEADER);
			text_.push_back(JETI_SENSOR_EX_ID);
			text_.push_back(
			JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + name.length() + unit.length()));
			text_.push_back(manufacturerId);
			text_.push_back(manufacturerId >> 8);
			text_.push_back(deviceId);
			text_.push_back(deviceId >> 8);
			text_.push_back(0); // reserved
			text_.push_back(id); // sensor ID
			text_.push_back((name.length() << 3) + unit.length());
			for (size_t i = 0; i < name.length(); ++i)
			{
				text_.push_back(name.c_str()[i]);
			}
			for (size_t i = 0; i < unit.length(); ++i)
			{
				text_.push_back(unit.c_str()[i]);
			}
			text_.push_back(get_crc8(&text_[2], text_.size() - 2));
		}

		CExSensor::~CExSensor()
		{
		}

		std::vector<std::uint8_t>& CExSensor::GetTextDescriptor()
		{
			return text_;
		}

		void CExSensor::SetValue(float val)
		{
		}

		void CExSensor::SetValue(std::int8_t val)
		{
		}

		void CExSensor::SetValue(std::int16_t val)
		{
		}

		void CExSensor::SetValue(std::int32_t val)
		{
		}

		void CExSensor::SetGpsValue(gps_t *val)
		{
		}

		CExVoltageSensor::CExVoltageSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::uint8_t* formattedData) :
				CExSensor(manufacturerId, deviceId, id, name, "V", Type::int14)
		{
			formattedValue_ = formattedData;
			formattedValue_[0] = 0;
			formattedValue_[1] = 0x60;
		}

		void CExVoltageSensor::SetValue(float val)
		{
			std::int16_t formattedValue;

			if (val < 8.0)
			{
				formattedValue = (uint16_t) (val * 1000.0);
				formattedValue |= 0x6000;
			}
			else
			{
				formattedValue = (uint16_t) (val * 100.0);
				formattedValue |= 0x4000;
			}

			formattedValue_[0] = formattedValue;
			formattedValue_[1] = formattedValue >> 8;
		}

		CExCurrentSensor::CExCurrentSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::string unit,
				std::uint8_t* formattedData) :
				CExSensor(manufacturerId, deviceId, id, name, unit, Type::int14)
		{
			formattedValue_ = formattedData;
			formattedValue_[0] = 0;
			formattedValue_[1] = 0x60;
		}

		void CExCurrentSensor::SetValue(float val)
		{
			std::int16_t formattedValue;

			if (val < 8.0)
			{
				formattedValue = (uint16_t) (val * 1000.0);
				formattedValue |= 0x6000;
			}
			else if (val < 80.0)
			{
				formattedValue = (uint16_t) (val * 100.0);
				formattedValue |= 0x4000;
			}
			else
			{
				formattedValue = (uint16_t) (val * 1000.0);
				formattedValue |= 0x2000;
			}

			formattedValue_[0] = formattedValue;
			formattedValue_[1] = formattedValue >> 8;
		}

		CExTemperatureSensor::CExTemperatureSensor(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id, std::string name, std::uint8_t* formattedData) :
				CExSensor(manufacturerId, deviceId, id, name, "°C", Type::int14)
		{
			formattedValue_ = formattedData;
			formattedValue_[0] = 0;
			formattedValue_[1] = 0x60;
		}

		void CExTemperatureSensor::SetValue(float val)
		{
			std::int16_t formattedValue;

			formattedValue = static_cast<std::int16_t>(val * 10.0);

			if (val < 0.0)
			{
				formattedValue |= 0x8000;
			}

			formattedValue |= 0x2000;

			formattedValue_[0] = formattedValue;
			formattedValue_[1] = formattedValue >> 8;
		}

		CExGPS::CExGPS(std::uint16_t manufacturerId, std::uint16_t deviceId, std::uint8_t id)
		{
			std::string s[3] = {"Longitude", "Lattitude", "GSpeed"};

			// prepare text descriptor
			text_.reserve(11 * 3 + s[0].length() + s[1].length() + s[2].length());

			text_.push_back(JETI_SENSOR_HEADER);
			text_.push_back(JETI_SENSOR_EX_ID);
			text_.push_back(
			JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + s[0].length()));
			text_.push_back(manufacturerId);
			text_.push_back(manufacturerId >> 8);
			text_.push_back(deviceId);
			text_.push_back(deviceId >> 8);
			text_.push_back(0); // reserved
			text_.push_back(id++); // sensor ID
			text_.push_back(s[0].length() << 3);
			for (size_t i = 0; i < s[0].length(); ++i)
			{
				text_.push_back(s[0].c_str()[i]);
			}
			text_.push_back(get_crc8(&text_[2], text_.size() - 2));

			text_.push_back(JETI_SENSOR_HEADER);
			text_.push_back(JETI_SENSOR_EX_ID);
			text_.push_back(
			JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + s[1].length()));
			text_.push_back(manufacturerId);
			text_.push_back(manufacturerId >> 8);
			text_.push_back(deviceId);
			text_.push_back(deviceId >> 8);
			text_.push_back(0); // reserved
			text_.push_back(id++); // sensor ID
			text_.push_back(s[1].length() << 3);
			for (size_t i = 0; i < s[1].length(); ++i)
			{
				text_.push_back(s[1].c_str()[i]);
			}
			text_.push_back(get_crc8(&text_[2], text_.size() - 2));

			text_.push_back(JETI_SENSOR_HEADER);
			text_.push_back(JETI_SENSOR_EX_ID);
			text_.push_back(
			JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + s[2].length()));
			text_.push_back(manufacturerId);
			text_.push_back(manufacturerId >> 8);
			text_.push_back(deviceId);
			text_.push_back(deviceId >> 8);
			text_.push_back(0); // reserved
			text_.push_back(id++); // sensor ID
			text_.push_back(s[2].length() << 3);
			for (size_t i = 0; i < s[2].length(); ++i)
			{
				text_.push_back(s[2].c_str()[i]);
			}
			text_.push_back(get_crc8(&text_[2], text_.size() - 2));
		}
	}
}
