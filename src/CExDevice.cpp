#include "CExDevice.hpp"
#include "crc.h"

namespace Jeti {
  namespace Device {

    const std::string CExDevice::deviceName_ = "exPowerBox";
    const uint16_t CExDevice::manufacturerId_ = 0xA401;
    const uint16_t CExDevice::deviceId_ = 0x0001;

    Sensor::CExVoltageSensor CExDevice::vBat1_(CExDevice::manufacturerId_,
                                               CExDevice::deviceId_, 1,
                                               "V Bat1");
    Sensor::CExVoltageSensor CExDevice::vBat2_(CExDevice::manufacturerId_,
                                               CExDevice::deviceId_, 2,
                                               "V Bat2");
    Sensor::CExCurrentSensor CExDevice::iBat1_(CExDevice::manufacturerId_,
                                               CExDevice::deviceId_, 3,
                                               "I Bat1");
    Sensor::CExCurrentSensor CExDevice::iBat2_(CExDevice::manufacturerId_,
                                               CExDevice::deviceId_, 4,
                                               "I Bat2");
    Sensor::CExCapacitySensor CExDevice::cBat1_(CExDevice::manufacturerId_,
                                                CExDevice::deviceId_, 5,
                                                "C Bat1");
    Sensor::CExCapacitySensor CExDevice::cBat2_(CExDevice::manufacturerId_,
                                                CExDevice::deviceId_, 6,
                                                "C Bat2");
    Sensor::CExTemperatureSensor CExDevice::tLocal_(CExDevice::manufacturerId_,
                                                    CExDevice::deviceId_, 7,
                                                    "T Local");
    Sensor::CExTemperatureSensor CExDevice::tExt1_(CExDevice::manufacturerId_,
                                                   CExDevice::deviceId_, 8,
                                                   "T Ext1");
    Sensor::CExTemperatureSensor CExDevice::tExt2_(CExDevice::manufacturerId_,
                                                   CExDevice::deviceId_, 9,
                                                   "T Ext2");

    std::array<Sensor::CExSensor*, EX_MAX_NB_SENSORS> CExDevice::sensorCollection_ =
        {&vBat1_, &vBat2_, &iBat1_, &iBat2_, &cBat1_, &cBat2_, &tLocal_,
         &tExt1_, &tExt2_};

    CExDevice::CExDevice() {
      isInitialized_ = false;
      dataPktIndex_ = 0;
      dataIndex_ = 0;
      for (size_t i = 0; i < dataPktLen_.size(); ++i)
        dataPktLen_[i] = 0;
    }

    CExDevice::~CExDevice() {
    }

    void CExDevice::init(chibios_rt::Mutex *mutex) {
      osalDbgCheck(isInitialized_ != true);
      // initialize device text descriptor
      int idx = 0;
      textDesc_[idx++] = JETI_SENSOR_HEADER;
      textDesc_[idx++] = JETI_SENSOR_EX_ID;
      textDesc_[idx++] =
      JETI_SENSOR_PKT_TXT_TYPE | (JETI_SENSOR_TXT_LEN + deviceName_.length());
      textDesc_[idx++] = static_cast<uint8_t>(manufacturerId_);
      textDesc_[idx++] = manufacturerId_ >> 8;
      textDesc_[idx++] = deviceId_;
      textDesc_[idx++] = deviceId_ >> 8;
      // reserved
      textDesc_[idx++] = 0;
      // device identifier, sensor identifiers start from 1
      textDesc_[idx++] = 0;
      // device name length
      textDesc_[idx++] = deviceName_.length() << 3;
      // device name
      for (size_t i = 0; i < deviceName_.length(); ++i) {
        textDesc_[idx++] = deviceName_.c_str()[i];
      }
      // crc8, separator and ex packet id are not included
      textDesc_[idx] = get_crc8(&textDesc_[2], idx - 2);
      textDescSize_ = idx + 1;

      // initialize data descriptor
      initDataDesc();

      isInitialized_ = true;
    }

    const std::array<uint8_t, EX_MAX_PKT_LEN>& CExDevice::getTextDescriptor() {
      osalDbgCheck(isInitialized_ != false);
      return textDesc_;
    }

    uint8_t CExDevice::getTextDescriptorSize() {
      osalDbgCheck(isInitialized_ != false);
      return textDescSize_;
    }

    uint8_t* CExDevice::getDataDescriptor(int index) {
      osalDbgCheck(isInitialized_ != false);
      osalDbgCheck(index >= 0);
      osalDbgCheck(index < EX_MAX_NB_SENSORS);

      uint8_t* data = &(dataPkt_[index][2]);
      uint8_t size = dataPkt_[index][2] & JETI_SENSOR_DATA_LEN_MSK;
      osalDbgCheck(size < 31);
      data[size - 1] = get_crc8(data, size - 1);

      return dataPkt_[index];
    }

    uint8_t CExDevice::getDataDescriptorSize(int index) {
      osalDbgCheck(isInitialized_ != false);
      osalDbgCheck(index >= 0);
      osalDbgCheck(index < EX_MAX_NB_SENSORS);

      return dataPktLen_[index];
    }

    uint8_t** CExDevice::getDataDescriptorCollection() {
      osalDbgCheck(isInitialized_ != false);
      return (uint8_t**)dataPkt_;
    }

    uint8_t CExDevice::getDataDescCollectionSize() {
      osalDbgCheck(isInitialized_ != false);
      return dataPktIndex_;
    }

    std::array<Sensor::CExSensor*, EX_MAX_NB_SENSORS>& CExDevice::getSensorCollection() {
      osalDbgCheck(isInitialized_ != false);
      return sensorCollection_;
    }

    Sensor::CExSensor* CExDevice::getSensor(int index) {
      osalDbgCheck(isInitialized_ != false);
      return sensorCollection_[index];
    }

    void CExDevice::initDataDesc() {
      osalDbgCheck(isInitialized_ != true);

      dataPktIndex_ = 0;
      AddDataVector(false);

      for (int i = 0; i < EX_NB_SENSORS; ++i) {
        Sensor::CExSensor* s = sensorCollection_[i];
        if (dataIndex_ + EX_TYPE_ID_SIZE + s->getFormattedValueSize()
            > EX_MAX_PKT_LEN) {
          dataPktLen_[dataPktIndex_] = dataIndex_ + 1;
          AddDataVector(true);
        }

        if (s->getId() > 15) {
          // data type
          dataPkt_[dataPktIndex_][dataIndex_++] =
              static_cast<uint8_t>(s->getDataType()) & 0xF;
          // sensor ID
          dataPkt_[dataPktIndex_][dataIndex_++] = s->getId();
        }
        else {
          dataPkt_[dataPktIndex_][dataIndex_++] = (s->getId() << 4)
              | (static_cast<uint8_t>(s->getDataType()) & 0xF);
        }

        // pointer to the formatted data is passed to the sensor object
        s->setFormattedValuePtr(&(dataPkt_[dataPktIndex_][dataIndex_]));
        // initialize formatted data
        for (int i = 0; i < s->getFormattedValueSize(); ++i)
          dataPkt_[dataPktIndex_][dataIndex_++] = 0;
      }
      // reserve space for the crc8
      AddDescLengthCRC();
      dataPktLen_[dataPktIndex_] = dataIndex_;
      // index variable will be used to get the size of the collection
      dataPktIndex_++;
    }

    void CExDevice::AddDescLengthCRC() {
      osalDbgCheck(isInitialized_ != true);
      // reserve a byte for crc8
      dataPkt_[dataPktIndex_][dataIndex_++] = 0;
      // length (separator and ex ID bytes are not included)
      osalDbgCheck(dataIndex_ < 31);
      dataPkt_[dataPktIndex_][2] |= dataIndex_ - 2;
    }

    void CExDevice::AddDataVector(bool updateCRC) {
      osalDbgCheck(isInitialized_ != true);

      // update vector with length and CRC
      if (updateCRC) {
        AddDescLengthCRC();
        dataPktIndex_++;
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
