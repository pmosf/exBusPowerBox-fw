#include "CExDevice.hpp"
#include "crc.h"

namespace Jeti
{
	namespace Device
	{
		CExDevice::CExDevice(std::string name, std::uint16_t manufacturerId, std::uint16_t deviceId) :
				manufacturerId_(manufacturerId), deviceId_(deviceId)
		{
			sensorCollection_.reserve(32);
			dataDescriptorIndex_ = 0;
			dataIndex_ = 0;

			// prepare text descriptor
			text_.reserve(JETI_SENSOR_TXT_LEN + name.length() + 2);
			text_.push_back(JETI_SENSOR_HEADER);
			text_.push_back(JETI_SENSOR_EX_ID);
			text_.push_back(JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + name.length()));
			text_.push_back(manufacturerId_);
			text_.push_back(manufacturerId_ >> 8);
			text_.push_back(deviceId_);
			text_.push_back(deviceId_ >> 8);
			// reserved
			text_.push_back(0);
			// device identifier, sensor identifiers start from 1
			text_.push_back(0);
			// device name length
			text_.push_back(name.length() << 3);
			// device name
			for (size_t i = 0; i < name.length(); ++i)
			{
				text_.push_back(name.c_str()[i]);
			}
			// crc8, separator and ex packet id are not included
			text_.push_back(get_crc8(&text_[2], text_.size() - 2));

			// first sensor ID
			sensorId_ = 1;

			// reserve space for 16 descriptors
			data_.reserve(16);
			// create and initialize the 1st descriptor
			AddDataVector(false);
		}

		CExDevice::~CExDevice()
		{
		}

		const std::vector<std::uint8_t>& CExDevice::GetTextDescriptor()
		{
			return text_;
		}

		const std::vector<std::uint8_t>& CExDevice::GetDataDescriptor(int index)
		{
			uint8_t* data = &(data_[index][2]);
			data[data_[index].size() - 2] = get_crc8(data, data_[index].size() - 3);

			return data_[index];
		}

		std::vector<std::vector<std::uint8_t>>& CExDevice::GetDataDescriptor()
		{
			return data_;
		}

		std::vector<std::shared_ptr<Sensor::CExSensor>>& CExDevice::GetSensorCollection()
		{
			return sensorCollection_;
		}

		Sensor::CExSensor* CExDevice::GetSensor(int index)
		{
			return sensorCollection_[index].get();
		}

		Sensor::CExSensor* CExDevice::GetSensor(std::string name)
		{
			std::map<std::string, Sensor::CExSensor*>::iterator it = sensorMap_.find(name);
			if (it != sensorMap_.end())
				return it->second;
			else
				return nullptr;
		}

		void CExDevice::AddSensor(std::string name, std::string unit, Sensor::Type type)
		{
			sensorCollection_.push_back(std::make_shared<Sensor::CExSensor>(Sensor::CExSensor(manufacturerId_, deviceId_, sensorId_++, name, unit, type)));
			sensorMap_[name] = sensorCollection_.back().get();
		}

		void CExDevice::AddVoltageSensor(std::string name, bool last)
		{
			// allocate a new descriptor if necessary
			// max packet length is 31 bytes including separator, ex packet id and crc8
			// we need 4 bytes for a voltage sensor (type, id, 2 bytes of formatted data)
			if (data_.back().size() + 4 > 27)
			{
				AddDataVector(true);
			}

			// data type
			data_.back().push_back(static_cast<std::uint8_t>(Sensor::Type::int14));
			// sensor ID
			data_.back().push_back(sensorId_);
			// 2 bytes for formatted data
			data_.back().push_back(0);
			// pointer to this formatted data will be passed to the sensor object constructor
			std::uint8_t* dataPtr = &(data_.back().back());
			data_.back().push_back(0);
			// if last sensor is created, we reserve space for the crc8
			if (last)
				AddDescLengthCRC();

			// create the sensor
			sensorCollection_.push_back(std::make_shared<Sensor::CExVoltageSensor>(Sensor::CExVoltageSensor(manufacturerId_, deviceId_, sensorId_++, name, dataPtr)));
			// add pointer of created sensor to the map
			sensorMap_[name] = sensorCollection_.back().get();
		}

		void CExDevice::AddCurrentSensor(std::string name, std::string unit, bool last)
		{
			// allocate a new descriptor if necessary
			// max packet length is 31 bytes including separator, ex packet id and crc8
			// we need 4 bytes for a current sensor (type, id, 2 bytes of formatted data)
			if (data_.back().size() + 4 > 27)
			{
				AddDataVector(true);
			}

			// data type
			data_.back().push_back(static_cast<std::uint8_t>(Sensor::Type::int14));
			// sensor ID
			data_.back().push_back(sensorId_);
			// 2 bytes for formatted data
			data_.back().push_back(0);
			// pointer to this formatted data will be passed to the sensor object constructor
			std::uint8_t* dataPtr = &(data_.back().back());
			data_.back().push_back(0);
			// if last sensor is created, we reserve space for the crc8
			if (last)
				AddDescLengthCRC();

			sensorCollection_.push_back(
					std::make_shared<Sensor::CExCurrentSensor>(Sensor::CExCurrentSensor(manufacturerId_, deviceId_, sensorId_++, name, unit, dataPtr)));
			sensorMap_[name] = sensorCollection_.back().get();
		}

		void CExDevice::AddCapacitySensor(std::string name, bool last)
		{
		}

		void CExDevice::AddGpsSensor(std::string name, bool last)
		{
		}

		void CExDevice::AddFuelSensor(std::string name, bool last)
		{
		}

		void CExDevice::AddTemperatureSensor(std::string name, bool last)
		{
			// allocate a new descriptor if necessary
			// max packet length is 31 bytes including separator, ex packet id and crc8
			// we need 4 bytes for a temperature sensor (type, id, 2 bytes of formatted data)
			if (data_.back().size() + 4 > 27)
			{
				AddDataVector(true);
			}

			// data type
			data_.back().push_back(static_cast<std::uint8_t>(Sensor::Type::int14));
			// sensor ID
			data_.back().push_back(sensorId_);
			// 2 bytes for formatted data
			data_.back().push_back(0);
			// pointer to this formatted data will be passed to the sensor object constructor
			std::uint8_t* dataPtr = &(data_.back().back());
			data_.back().push_back(0);
			// if last sensor is created, we reserve space for the crc8
			if (last)
				AddDescLengthCRC();

			sensorCollection_.push_back(std::make_shared<Sensor::CExTemperatureSensor>(Sensor::CExTemperatureSensor(manufacturerId_, deviceId_, sensorId_++, name, dataPtr)));
			sensorMap_[name] = sensorCollection_.back().get();
		}

		void CExDevice::AddRpmSensor(std::string name, bool last)
		{
		}

		void CExDevice::AddDescLengthCRC()
		{
			// length (separator and ex ID bytes are not included)
			data_.back()[2] += data_.back().size() - 2;
			// crc8 (separator and ex ID bytes are not included)
			data_.back().push_back(0/*get_crc8(&data_.back()[2], data_.back().size() - 2)*/);
		}

		void CExDevice::AddDataVector(bool updateCRC)
		{
			// update vector with length and CRC
			if (updateCRC)
			{
				AddDescLengthCRC();
			}

			// create new vector
			data_.push_back(std::vector<std::uint8_t>());

			// setup data descriptor
			// message separator
			data_.back().reserve(31);
			data_.back().push_back(JETI_SENSOR_HEADER);
			// EX packet identifier
			data_.back().push_back(JETI_SENSOR_EX_ID);
			// data packet type, length of the packet is still unknown
			data_.back().push_back(JETI_SENSOR_PKT_DATA_TYPE);
			// manufacturer ID, little endian
			data_.back().push_back(manufacturerId_);
			data_.back().push_back(manufacturerId_ >> 8);
			// device ID, little endian
			data_.back().push_back(deviceId_);
			data_.back().push_back(deviceId_ >> 8);
			// reserved
			data_.back().push_back(0);
		}
	}
}
