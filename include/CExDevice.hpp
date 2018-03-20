#pragma once

#include <array>
#include <string>

#include "CExSensor.hpp"

#define EX_NB_SENSORS       9
#define EX_MAX_NB_SENSORS   31
#define EX_MAX_PKT_LEN      31
#define EX_TYPE_ID_SIZE     2

namespace Jeti {
  namespace Device {
    enum class DeviceSensorLUT {
      BAT_VOLTAGE1 = 0,
      BAT_VOLTAGE2,
      BAT_CURRENT1,
      BAT_CURRENT2,
      BAT_CAPACITY1,
      BAT_CAPACITY2,
      T_LOCAL,
      T_EXT1,
      T_EXT2
    };

    class CExDevice {
      public:
        CExDevice();
        ~CExDevice();
        void init(chibios_rt::Mutex *mutex);
        const std::array<uint8_t, EX_MAX_PKT_LEN>& getTextDescriptor();
        uint8_t getTextDescriptorSize();
        uint8_t* getDataDescriptor(int index);
        uint8_t getDataDescriptorSize(int index);
        uint8_t** getDataDescriptorCollection();
        uint8_t getDataDescCollectionSize();
        std::array<Sensor::CExSensor*, EX_MAX_NB_SENSORS>& getSensorCollection();
        Sensor::CExSensor* getSensor(int index);

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

        static Sensor::CExVoltageSensor vBat1_;
        static Sensor::CExVoltageSensor vBat2_;
        static Sensor::CExCurrentSensor iBat1_;
        static Sensor::CExCurrentSensor iBat2_;
        static Sensor::CExCapacitySensor cBat1_;
        static Sensor::CExCapacitySensor cBat2_;
        static Sensor::CExTemperatureSensor tLocal_;
        static Sensor::CExTemperatureSensor tExt1_;
        static Sensor::CExTemperatureSensor tExt2_;

        static std::array<Sensor::CExSensor*, EX_MAX_NB_SENSORS> sensorCollection_;
        std::array<uint8_t, EX_MAX_PKT_LEN> textDesc_;
        uint8_t textDescSize_;
        uint8_t dataPkt_[EX_MAX_NB_SENSORS][EX_MAX_PKT_LEN];
        uint8_t dataPktIndex_;
        uint8_t dataIndex_;
        std::array<uint8_t, EX_MAX_NB_SENSORS> dataPktLen_;
        uint8_t sensorCollectionIndex_;
        bool isInitialized_;
    };
  }
}
