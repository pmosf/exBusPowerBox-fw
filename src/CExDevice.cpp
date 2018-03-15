#include "CExDevice.hpp"
#include "crc.h"

namespace Jeti {
  namespace Device {

    const std::string CExDevice::deviceName_ = "exPowerBox";
    const uint16_t CExDevice::manufacturerId_ = 0xA400;
    const uint16_t CExDevice::deviceId_ = 0;

    std::array<Sensor::CExSensor, EX_NB_SENSORS> CExDevice::sensorCollection_ =
        {Sensor::CExVoltageSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 1, "V Bat1"),
         Sensor::CExVoltageSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 2, "V Bat2"),
         Sensor::CExCurrentSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 3, "I Bat1"),
         Sensor::CExCurrentSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 4, "I Bat2"),
         Sensor::CExCurrentSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 5, "C Bat1"),
         Sensor::CExCurrentSensor(CExDevice::manufacturerId_,
                                  CExDevice::deviceId_, 6, "C Bat2"),
         Sensor::CExTemperatureSensor(CExDevice::manufacturerId_,
                                      CExDevice::deviceId_, 7, "T Local"),
         Sensor::CExTemperatureSensor(CExDevice::manufacturerId_,
                                      CExDevice::deviceId_, 8, "T Ext1"),
         Sensor::CExTemperatureSensor(CExDevice::manufacturerId_,
                                      CExDevice::deviceId_, 9, "T Ext2")};

    CExDevice::CExDevice() {

      dataPktIndex_ = 0;
      dataIndex_ = 0;
      sensorCollectionIndex_ = 0;
    }

    CExDevice::~CExDevice() {
    }

    void CExDevice::init() {
      // initialize device text descriptor
      int idx = 0;
      textDesc_[idx++] = JETI_SENSOR_HEADER;
      textDesc_[idx++] = JETI_SENSOR_EX_ID;
      textDesc_[idx++] =
      JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + deviceName_.size());
      textDesc_[idx++] = static_cast<uint8_t>(manufacturerId_);
      textDesc_[idx++] = manufacturerId_ >> 8;
      textDesc_[idx++] = deviceId_;
      textDesc_[idx++] = deviceId_ >> 8;
      // reserved
      textDesc_[idx++] = 0;
      // device identifier, sensor identifiers start from 1
      textDesc_[idx++] = 0;
      // device name length
      textDesc_[idx++] = deviceName_.size() << 3;
      // device name
      for (size_t i = 0; i < deviceName_.length(); ++i) {
        textDesc_[idx++] = deviceName_[i];
      }
      // crc8, separator and ex packet id are not included
      textDesc_[idx] = get_crc8(&textDesc_[2], idx - 2);
      textDescSize_ = idx + 1;

      // initialize data descriptor
      initDataDesc();
    }

    void CExDevice::initDataDesc() {
      dataPktIndex_ = 0;
      dataIndex_ = 0;
      sensorCollectionIndex_ = 0;

      AddDataVector(false);

      for (auto& s : sensorCollection_) {
        switch (s.getDataType()) {
          // 3 bytes needed for a int6 sensor (type, id, 1 byte of formatted data)
          case Sensor::DataType::int6:
            dataPktIndex_ =
                dataPktLen_[dataPktIndex_] + EX_TYPE_ID_SIZE
                    + s.getFormattedValueSize() > 27 ? dataPktIndex_ + 1 :
                                                       dataPktIndex_;
            AddDataVector(true);
            break;
            // 4 bytes needed for a int14 sensor (type, id, 2 bytes of formatted data)
          case Sensor::DataType::int14:
            dataPktIndex_ =
                dataPktLen_[dataPktIndex_] + EX_TYPE_ID_SIZE
                    + s.getFormattedValueSize() > 27 ? dataPktIndex_ + 1 :
                                                       dataPktIndex_;
            AddDataVector(true);
            break;

            // 5 bytes needed for a int22 sensor (type, id, 3 bytes of formatted data)
          case Sensor::DataType::int22:
            dataPktIndex_ =
                dataPktLen_[dataPktIndex_] + EX_TYPE_ID_SIZE
                    + s.getFormattedValueSize() > 27 ? dataPktIndex_ + 1 :
                                                       dataPktIndex_;
            AddDataVector(true);
            break;

            // 5 bytes needed for a gps sensor (type, id, 3 bytes of formatted data)
          case Sensor::DataType::gps:
            dataPktIndex_ =
                dataPktLen_[dataPktIndex_] + EX_TYPE_ID_SIZE
                    + s.getFormattedValueSize() > 27 ? dataPktIndex_ + 1 :
                                                       dataPktIndex_;
            AddDataVector(true);
            break;

          default:
            AddDataVector(true);
            break;
        }

        // data type
        dataPkt_[dataPktIndex_][dataIndex_++] =
            static_cast<uint8_t>(s.getDataType());
        // sensor ID
        dataPkt_[dataPktIndex_][dataIndex_++] = s.getId();
        // pointer to the formatted data is passed to the sensor object
        s.setFormattedValuePtr(&(dataPkt_[dataPktIndex_][dataIndex_]));
        // initialize formatted data
        for (int i = 0; i < s.getFormattedValueSize(); ++i)
          dataPkt_[dataPktIndex_][dataIndex_++] = 0;
      }
      // reserve space for the crc8
      AddDescLengthCRC();
    }

    const std::array<uint8_t, EX_MAX_PKT_LEN>& CExDevice::getTextDescriptor() {
      return textDesc_;
    }

    uint8_t CExDevice::getTextDescriptorSize() {
      return textDescSize_;
    }

    const std::array<uint8_t, EX_MAX_PKT_LEN>& CExDevice::getDataDescriptor(
        int index) {
      uint8_t* data = &(dataPkt_[index][2]);
      data[dataPktLen_[index] - 2] = get_crc8(data, dataPktLen_[index] - 3);

      return dataPkt_[index];
    }

    std::array<std::array<uint8_t, EX_MAX_PKT_LEN>, EX_NB_SENSORS>& CExDevice::getDataDescriptorCollection() {
      return dataPkt_;
    }

    uint8_t CExDevice::getDataDescCollectionSize() {
      return dataPktIndex_ + 1;
    }

    std::array<Sensor::CExSensor, EX_NB_SENSORS>& CExDevice::getSensorCollection() {
      return sensorCollection_;
    }

    Sensor::CExSensor* CExDevice::getSensor(int index) {
      return &sensorCollection_[index];
    }

    void CExDevice::AddDescLengthCRC() {
      // length (separator and ex ID bytes are not included)
      dataPkt_[dataPktIndex_][2] += dataPktLen_[dataPktIndex_] - 2;
      // crc8 (separator and ex ID bytes are not included)
      dataPkt_[dataPktIndex_][dataIndex_++] = 0;
    }

    void CExDevice::AddDataVector(bool updateCRC) {
      // update vector with length and CRC
      if (updateCRC) {
        AddDescLengthCRC();
      }

      dataIndex_ = 0;

      // setup data descriptor
      // message separator
      dataPkt_[dataPktIndex_][dataIndex_++] = JETI_SENSOR_HEADER;
      // EX packet identifier
      dataPkt_[dataPktIndex_][dataIndex_++] = JETI_SENSOR_EX_ID;
      // data packet type, length of the packet is still unknown
      dataPkt_[dataPktIndex_][dataIndex_++] = JETI_SENSOR_PKT_DATA_TYPE;
      // manufacturer ID, little endian
      dataPkt_[dataPktIndex_][dataIndex_++] =
          static_cast<uint8_t>(manufacturerId_);
      dataPkt_[dataPktIndex_][dataIndex_++] = manufacturerId_ >> 8;
      // device ID, little endian
      dataPkt_[dataPktIndex_][dataIndex_++] = deviceId_;
      dataPkt_[dataPktIndex_][dataIndex_++] = deviceId_ >> 8;
      // reserved
      dataPkt_[dataPktIndex_][dataIndex_++] = 0;
    }
  }
}
