#pragma once

#include <string>
#include <array>

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

#define JETI_SENSOR_NEG_VALUE		0x80
#define JETI_SENSOR_POS_VALUE		0

#define JETI_GPS_LONGITUDE			0x20000000
#define JETI_GPS_LATITUDE			0
#define JETI_GPS_WEST				0x40000000
#define JETI_GPS_EAST				0
#define JETI_GPS_SOUTH				0x40000000
#define JETI_GPS_NORTH				0

#define JETI_EX_TEXT_DESC_SIZE      64

namespace Jeti {
  namespace Sensor {
    enum class Type
      : int {
        voltage = 1,
      current,
      temperature,
      gps
    };
    enum class DataType
      : uint8_t
      {
        int6 = 0,
      int14 = 1,
      int22 = 4,
      timeDate = 5,
      int30 = 8,
      gps = 9
    };

    typedef struct {
        struct {
            uint8_t degree;
            uint32_t minute;
            char hemisphere;
        } longitude;
        struct {
            uint8_t degree;
            uint32_t minute;
            char hemisphere;
        } latitude;
    } gps_t;

    typedef struct {
        uint8_t dataType;
        uint8_t id;
        uint8_t *formattedValue;
    } data_desc_t;

    class CExSensor {
      public:
        CExSensor();
        CExSensor(uint16_t manufacturerId, uint16_t deviceId, uint8_t id,
                  std::string name, std::string unit, Type type,
                  DataType dataType);
        virtual ~CExSensor();
        std::array<uint8_t, JETI_EX_TEXT_DESC_SIZE>& getTextDescriptor();
        uint8_t& getTextDescLen();
        const uint8_t& getId() const;
        const DataType& getDataType() const;
        const std::string& getName() const;
        void setFormattedValuePtr(uint8_t *p);
        int getFormattedValueSize();
        virtual void setValue(float val);
        virtual void setValue(int8_t val);
        virtual void setValue(int16_t val);
        virtual void setValue(int32_t val);

      protected:
        uint8_t id_;
        std::string name_;
        Type type_;
        DataType dataType_;
        std::array<uint8_t, JETI_EX_TEXT_DESC_SIZE> text_;
        uint8_t textLen_;
        uint8_t *formattedValue_;
        int formattedValueSize_;
        int precision_;
    };

    class CExVoltageSensor: public CExSensor {
      public:
        CExVoltageSensor(uint16_t manufacturerId, uint16_t deviceId, uint8_t id,
                         std::string name);
        void setValue(float val);
    };

    class CExCurrentSensor: public CExSensor {
      public:
        CExCurrentSensor(uint16_t manufacturerId, uint16_t deviceId, uint8_t id,
                         std::string name);
        void setValue(float val);
    };

    class CExTemperatureSensor: public CExSensor {
      public:
        CExTemperatureSensor(uint16_t manufacturerId, uint16_t deviceId,
                             uint8_t id, std::string name);
        void setValue(float val);
    };

    class CExGPS: public CExSensor {
      public:
        CExGPS(uint16_t manufacturerId, uint16_t deviceId, uint8_t id);
        void setCoordinates(const gps_t* coordinates);
        void getLongitude();
        void getLattitude();
        void getSpeed();

      private:
        gps_t gpsCoordinates_;
    };
  }
}
