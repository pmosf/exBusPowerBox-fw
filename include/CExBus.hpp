#ifndef __EXBUS_H
#define __EXBUS_H

#include <cstdint>
#include <vector>
#include <string>

#include "CExDevice.hpp"

#define JETI_EXBUS_PKT_CRC_OK		true
#define JETI_EXBUS_PKT_HEADER_OK	true

namespace Jeti {
  namespace ExBus {
    typedef struct {
      std::uint8_t header[2];
      std::uint8_t pktLen;
      std::uint8_t packetId;
      std::uint8_t dataId;
      std::uint8_t dataLength;
      std::uint8_t data[58];
    } packet_t;

    class CExBus {
    public:
      CExBus(std::string exTelemetryDeviceName);
      ~CExBus();
      Device::CExDevice& GetTelemetryDevice();
      std::uint8_t* GetExTelemetryTextPacket();
      std::uint8_t GetExTelemetryTextPacketSize();
      std::uint8_t* GetExTelemetryDataPacket();
      std::uint8_t GetExTelemetryDataPacketSize();
      static bool checkCRC(packet_t& pkt);
      static bool checkHeader(packet_t& pkt);
      std::uint16_t& GetServoValue(int index);
      void AddTelemetryVoltage(std::string name, bool last);
      void AddTelemetryCurrent(std::string name, bool last);
      void AddTelemetryCurrent(std::string name, std::string unit, bool last);
      void AddTelemetryCapacity(std::string name, bool last);
      void AddTelemetryTemperature(std::string name, bool last);

    private:
      void AddTelemetryText();

      Device::CExDevice telemetryDevice_;
      std::uint8_t textDescriptorIndex_;
      std::uint8_t dataDescriptorIndex_;
      static packet_t telemetryData_;
      static packet_t telemetryText_[];
      std::uint8_t exTelemetryTextPacketLen_;
      static volatile packet_t packet_;
      std::uint16_t* servoValues_;
    };
  }
}

#endif
