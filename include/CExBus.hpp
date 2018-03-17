/*
 * CExBusUart.hpp
 *
 *  Created on: Feb 2, 2018
 *      Author: pperonna
 */

#pragma once

#include "ch.hpp"
#include "hal.h"

#include "CExDevice.hpp"
#include "CExSensor.hpp"

#define JETI_EXBUS_PKT_CRC_OK       true
#define JETI_EXBUS_PKT_HEADER_OK    true

#define EXBUS_SERVO_POSITIONS       EVENT_MASK(0)
#define EXBUS_TELEMETRY             EVENT_MASK(1)
#define EXBUS_JETIBOX               EVENT_MASK(2)
#define EXBUS_TIMEOUT               EVENT_MASK(3)
#define EXBUS_CRC_ERR               EVENT_MASK(4)

#define EX_NB_SERVOS                16
#define EX_SERVO_POSITION_POOL_SIZE 8

typedef struct {
    uint8_t header[2];
    uint8_t pktLen;
    uint8_t packetId;
    uint8_t dataId;
    uint8_t dataLength;
    uint8_t data[58];
} packet_t;

namespace ExPowerBox {
  class CExBusUart: public chibios_rt::BaseStaticThread<128> {
    public:
      CExBusUart(SerialDriver *driver, const char *threadName,
                 Jeti::Device::CExDevice *exDevice);
      virtual ~CExBusUart();
      virtual void main(void);
      chibios_rt::EvtSource* getEvent();
      void getServoPosition(uint16_t* dest);
      uint16_t getServoPosition(int ch);
      void sendJetibox(packet_t *p);

    private:
      SerialDriver *driver_;
      SerialConfig serialConfig_;
      const char *threadName_;
      Jeti::Device::CExDevice *exDevice_;
      bool isClassInitialized_;
      uint8_t rxData_;
      packet_t exPacket_ __attribute__((aligned(4)));
      std::array<packet_t, EX_NB_SENSORS + 1> telemetryTextPkt_ __attribute__((aligned(4)));
      uint8_t telemetryTextPktIndex_;
      packet_t telemetryDataPkt_ __attribute__((aligned(4)));
      uint8_t telemetryDataPktIndex_;
      uint8_t telemetryDataPktArraySize_;
      uint32_t nbExPacket_;
      uint32_t nbExValidPacket_;
      uint32_t nbExInvalidPacket_;
      int8_t state_;
      std::array<uint16_t, EX_NB_SERVOS> servoPosition_;
      chibios_rt::EvtSource evt_;
      uint32_t nbExTelemetryPktSent_;

      bool exDecode(int8_t data);
      void initPacket();
      void initTextDesc();
      void initDataDesc();
      bool checkCRC();
      void setServoPositionValues();
      void processTelemetryRequest();
      void processJetiBoxRequest();
  };

} /* namespace ExPowerBox */
