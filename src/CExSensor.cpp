#include <string>

#include "ch.hpp"
#include "CExSensor.hpp"
#include "crc.h"

namespace Jeti {
  namespace Sensor {
    CExSensor::CExSensor() {

    }

    CExSensor::CExSensor(uint16_t manufacturerId, uint16_t deviceId, uint8_t id,
                         std::string name, std::string unit, Type type,
                         DataType dataType) :
        id_(id), name_(name), type_(type), dataType_(dataType) {
      // prepare text descriptor
      int idx = 0;
      text_[idx++] = JETI_SENSOR_HEADER;
      text_[idx++] = JETI_SENSOR_EX_ID;
      text_[idx++] = JETI_SENSOR_PKT_TXT_TYPE
          | (JETI_SENSOR_TXT_LEN + name.size() + unit.size());
      text_[idx++] = manufacturerId;
      text_[idx++] = manufacturerId >> 8;
      text_[idx++] = deviceId;
      text_[idx++] = deviceId >> 8;
      text_[idx++] = 0; // reserved
      text_[idx++] = id_; // sensor ID
      text_[idx++] = (name.size() << 3) + unit.size();
      for (size_t i = 0; i < name.size(); ++i) {
        text_[idx++] = name.c_str()[i];
      }
      for (size_t i = 0; i < unit.size(); ++i) {
        text_[idx++] = unit.c_str()[i];
      }

      text_[idx] = get_crc8(&text_[2], idx - 2);

      textDescSize_ = idx + 1;

      formattedValueSize_ = 0;
      formattedValue_ = nullptr;
    }

    CExSensor::~CExSensor() {
    }

    std::array<uint8_t, JETI_EX_TEXT_DESC_SIZE>& CExSensor::getTextDescriptor() {
      return text_;
    }

    uint8_t& CExSensor::getTextDescriptorSize() {
      return textDescSize_;
    }

    uint8_t& CExSensor::getId() {
      return id_;
    }

    DataType& CExSensor::getDataType() {
      return dataType_;
    }

    std::string& CExSensor::getName() {
      return name_;
    }

    void CExSensor::setFormattedValuePtr(uint8_t *p) {
      osalDbgCheck(p != nullptr);
      formattedValue_ = p;
    }

    int CExSensor::getFormattedValueSize() {
      return formattedValueSize_;
    }

    CExVoltageSensor::CExVoltageSensor(uint16_t manufacturerId,
                                       uint16_t deviceId, uint8_t id,
                                       const std::string name) :
        CExSensor(manufacturerId, deviceId, id, name, "V",
                  Sensor::Type::voltage, Sensor::DataType::int14) {
      formattedValueSize_ = 2;
    }

    void CExSensor::setVoltage(float val) {
      uint16_t formattedValue = 0;

      if (val < 8.0) {
        formattedValue = static_cast<uint16_t>(val * 1000.0);
        formattedValue |= 0x6000;
      }
      else {
        formattedValue = static_cast<uint16_t>(val * 100.0);
        formattedValue |= 0x4000;
      }

      osalDbgCheck(formattedValue_ != nullptr);
      osalDbgCheck(formattedValueSize_ == 2);
      formattedValue_[0] = formattedValue;
      formattedValue_[1] = formattedValue >> 8;
    }

    CExCurrentSensor::CExCurrentSensor(uint16_t manufacturerId,
                                       uint16_t deviceId, uint8_t id,
                                       const std::string name) :
        CExSensor(manufacturerId, deviceId, id, name, "A",
                  Sensor::Type::current, Sensor::DataType::int14) {
      formattedValueSize_ = 2;
    }

    void CExSensor::setCurrent(float val) {
      uint16_t formattedValue = 0;

      if (val < 8.0) {
        formattedValue = static_cast<uint16_t>(val * 1000.0);
        formattedValue |= 0x6000;
      }
      else if (val < 80.0) {
        formattedValue = static_cast<uint16_t>(val * 100.0);
        formattedValue |= 0x4000;
      }
      else {
        formattedValue = static_cast<uint16_t>(val * 1000.0);
        formattedValue |= 0x2000;
      }

      osalDbgCheck(formattedValue_ != nullptr);
      osalDbgCheck(formattedValueSize_ == 2);

      formattedValue_[0] = formattedValue;
      formattedValue_[1] = formattedValue >> 8;
    }

    CExCapacitySensor::CExCapacitySensor(uint16_t manufacturerId,
                                         uint16_t deviceId, uint8_t id,
                                         const std::string name) :
        CExSensor(manufacturerId, deviceId, id, name, "mAh",
                  Sensor::Type::current, Sensor::DataType::int14) {
      formattedValueSize_ = 2;
    }

    void CExSensor::setCapacity(float val) {
      uint16_t formattedValue = 0;

      if (val < 8000.0) {
        formattedValue = static_cast<uint16_t>(val);
      }
      else {
        formattedValue = 8191;
        formattedValue |= 0x2000;
      }

      osalDbgCheck(formattedValue_ != nullptr);
      osalDbgCheck(formattedValueSize_ == 2);

      formattedValue_[0] = formattedValue;
      formattedValue_[1] = formattedValue >> 8;
    }

    CExTemperatureSensor::CExTemperatureSensor(uint16_t manufacturerId,
                                               uint16_t deviceId, uint8_t id,
                                               const std::string name) :
        CExSensor(manufacturerId, deviceId, id, name, "°C",
                  Sensor::Type::temperature, Sensor::DataType::int14) {
      formattedValueSize_ = 2;
    }

    void CExSensor::setTemperature(float val) {
      int16_t formattedValue = 0;

      formattedValue = static_cast<int16_t>(val * 10.0);
      if (val < 0.0) {
        formattedValue |= 0x8000;
      }
      formattedValue |= 0x2000;

      osalDbgCheck(formattedValue_ != nullptr);
      osalDbgCheck(formattedValueSize_ == 2);

      formattedValue_[0] = formattedValue;
      formattedValue_[1] = formattedValue >> 8;
    }

    CExGPS::CExGPS(uint16_t manufacturerId, uint16_t deviceId, uint8_t id) :
        CExSensor() {
      const std::string s[3] = {"Longitude", "Lattitude", "GSpeed"};
      int idx = 0;

      dataType_ = DataType::gps;

      text_[idx++] = JETI_SENSOR_HEADER;
      text_[idx++] = JETI_SENSOR_EX_ID;
      text_[idx++] = JETI_SENSOR_PKT_TXT_TYPE
          | (JETI_SENSOR_TXT_LEN + s[0].size());
      text_[idx++] = manufacturerId;
      text_[idx++] = manufacturerId >> 8;
      text_[idx++] = deviceId;
      text_[idx++] = deviceId >> 8;
      text_[idx++] = 0; // reserved
      text_[idx++] = id++; // sensor ID
      text_[idx++] = (s[0].size() << 3);
      for (size_t i = 0; i < s[0].size(); ++i) {
        text_[idx++] = s[0].c_str()[i];
      }
      text_[idx] = get_crc8(&text_[2], idx - 2);
      idx++;
      textDescSize_ = idx;

      text_[idx++] = JETI_SENSOR_HEADER;
      text_[idx++] = JETI_SENSOR_EX_ID;
      int j = idx;
      text_[idx++] = JETI_SENSOR_PKT_TXT_TYPE
          | (JETI_SENSOR_TXT_LEN + s[1].size());
      text_[idx++] = manufacturerId;
      text_[idx++] = manufacturerId >> 8;
      text_[idx++] = deviceId;
      text_[idx++] = deviceId >> 8;
      text_[idx++] = 0; // reserved
      text_[idx++] = id++; // sensor ID
      text_[idx++] = (s[1].size() << 3);
      for (size_t i = 0; i < s[1].size(); ++i) {
        text_[idx++] = s[1].c_str()[i];
      }
      text_[idx] = get_crc8(&text_[j], idx - j);
      idx++;
      textDescSize_ += idx;

      text_[idx++] = JETI_SENSOR_HEADER;
      text_[idx++] = JETI_SENSOR_EX_ID;
      j = idx;
      text_[idx++] = JETI_SENSOR_PKT_TXT_TYPE
          | (JETI_SENSOR_TXT_LEN + s[2].size());
      text_[idx++] = manufacturerId;
      text_[idx++] = manufacturerId >> 8;
      text_[idx++] = deviceId;
      text_[idx++] = deviceId >> 8;
      text_[idx++] = 0; // reserved
      text_[idx++] = id++; // sensor ID
      text_[idx++] = (s[2].size() << 3);
      for (size_t i = 0; i < s[2].size(); ++i) {
        text_[idx++] = s[2].c_str()[i];
      }
      text_[idx] = get_crc8(&text_[j], idx - j);
      idx++;
      textDescSize_ += idx;
    }
  }
}
