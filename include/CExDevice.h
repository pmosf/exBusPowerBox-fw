#ifndef __EXDEVICE_H
#define __EXDEVICE_H

#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "CExSensor.h"

namespace Jeti
{
	namespace Device
	{
		typedef struct
		{
			std::uint8_t header[2];
			std::uint8_t lengthType;
			std::uint8_t sn[4];
			std::uint8_t rsvd;
			std::uint8_t data[29];
		} dataPacket_t;
		
		class CExDevice
		{
			public:
				CExDevice(std::string name, std::uint16_t manufacturerId, std::uint16_t deviceId);
				~CExDevice();
				
				const std::vector<std::uint8_t>& GetTextDescriptor();
				const std::vector<std::uint8_t>& GetDataDescriptor(int index);
				std::vector<std::vector<std::uint8_t>>& GetDataDescriptor();
				std::vector<std::shared_ptr<Sensor::CExSensor>>& GetSensorCollection();
				
				void AddSensor(std::string name,  std::string unit, Sensor::Type type);
				void AddVoltageSensor(std::string name, bool last);
				void AddCurrentSensor(std::string name, std::string unit, bool last);
				void AddCapacitySensor(std::string name, bool last);
				void AddGpsSensor(std::string name, bool last);
				void AddFuelSensor(std::string name, bool last);
				void AddTemperatureSensor(std::string name, bool last);
				void AddRpmSensor(std::string name, bool last);
				
				Sensor::CExSensor* GetSensor(int index);
				Sensor::CExSensor* GetSensor(std::string name);
				
				void SetSensorValue(int index, float value);
				void SetSensorValue(int index, std::int32_t value);
				void SetSensorValue(int index, std::int16_t value);
				void SetSensorValue(int index, std::int8_t value);
				void SetGPSPosition();
			
			private:
				void AddDataVector(bool updateCRC);
				void AddDescLengthCRC();
				std::uint16_t manufacturerId_;
				std::uint16_t deviceId_;
				std::vector<std::uint8_t> text_;
				std::vector<std::vector<std::uint8_t>> data_;
				std::vector<std::shared_ptr<Sensor::CExSensor>> sensorCollection_;
				std::map<std::string, Sensor::CExSensor*> sensorMap_;
				std::uint8_t sensorId_;
				std::uint8_t dataDescriptorIndex_;
				std::uint8_t dataIndex_;
		};
	}
}

#endif
