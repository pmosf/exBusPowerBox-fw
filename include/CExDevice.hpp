#pragma once

#include <map>
#include <array>
#include <string>

#include "CExSensor.hpp"

#define EX_NB_SENSORS       9
#define EX_MAX_NB_SENSORS   31
#define EX_MAX_PKT_LEN      31
#define EX_TYPE_ID_SIZE     2

namespace Jeti {
  namespace Device {
    typedef struct {
        uint8_t header[2];
        uint8_t lengthType;
        uint8_t sn[4];
        uint8_t rsvd;
        uint8_t data[29];
    } dataPacket_t;

    class CExDevice {
      public:
        CExDevice();
        ~CExDevice();

        const std::array<uint8_t, EX_MAX_PKT_LEN>& getTextDescriptor();
        uint8_t getTextDescriptorSize();
        const std::array<uint8_t, EX_MAX_PKT_LEN>& getDataDescriptor(int index);
        std::array<std::array<uint8_t, EX_MAX_PKT_LEN>, EX_NB_SENSORS>& getDataDescriptorCollection();
        uint8_t getDataDescCollectionSize();
        std::array<Sensor::CExSensor, EX_NB_SENSORS>& getSensorCollection();
        const Sensor::CExSensor* getSensor(int index);
        const Sensor::CExSensor* getSensor(const std::string& name);

        void SetSensorValue(int index, float value);
        void SetSensorValue(int index, int32_t value);
        void SetSensorValue(int index, int16_t value);
        void SetSensorValue(int index, int8_t value);
        void SetGPSPosition();

      private:
        void AddDataVector(bool updateCRC);
        void AddDescLengthCRC();
        void initDataDesc();

        const static std::string deviceName_;
        const static uint16_t manufacturerId_;
        const static uint16_t deviceId_;
        static std::array<Sensor::CExSensor, EX_NB_SENSORS> sensorCollection_;
        //const static std::map<const char*, const Sensor::CExSensor*> sensorMap_;
        std::array<uint8_t, EX_MAX_PKT_LEN> textDesc_;
        uint8_t textDescSize_;
        std::array<std::array<uint8_t, EX_MAX_PKT_LEN>, EX_NB_SENSORS> dataPkt_;
        uint8_t dataPktIndex_;
        uint8_t dataIndex_;
        std::array<uint8_t, EX_MAX_NB_SENSORS> dataPktLen_;
        uint8_t sensorCollectionIndex_;
    };
  }
}
