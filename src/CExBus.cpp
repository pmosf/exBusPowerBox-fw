/*
 * CExBusUart.cpp
 *
 *  Created on: Feb 2, 2018
 *      Author: pperonna
 */

#include "ch.hpp"
#include "hal.h"
#include "CExBus.hpp"
#include "crc.h"

namespace ExPowerBox {

  CExBusUart::CExBusUart(thread_t *parent, SerialDriver *driver,
                         const char *threadName) :
      exDevice_(), parentThread_(parent), driver_(driver), serialConfig_( {
          125000, 0,
          USART_CR2_STOP_1,
          USART_CR3_HDSEL}), threadName_(threadName) {

    state_ = 0;
    nbExPacket_ = 0;
    nbExValidPacket_ = 0;
    nbExInvalidPacket_ = 0;
    isClassInitialized_ = false;
    nbExTelemetryPktSent_ = 0;
    telemetryTextPktIndex_ = 0;

    initPacket();

    for (int i = 0; i < EX_NB_SERVOS; ++i)
      servoPosition_[i] = 0;
  }

  CExBusUart::~CExBusUart() {
    sdStop(driver_);
  }

  chibios_rt::EvtSource& CExBusUart::getEvtServoPosition() {
    return evtServoPosition_;
  }

  void CExBusUart::getServoPosition(uint16_t* dest) {
    chSysLock();
    for (int i = 0; i < EX_NB_SERVOS; ++i) {
      *dest++ = servoPosition_[i];
    }
    chSysUnlock();
  }

  uint16_t CExBusUart::getServoPosition(int ch) {
    chSysLock();
    return servoPosition_[ch];
    chSysUnlock();
  }

  __attribute__((noreturn))
  void CExBusUart::main(void) {
    setName(threadName_);

    sdStart(driver_, &serialConfig_);

    while (true) {
      size_t n = chnReadTimeout((BaseChannel * )driver_, &rxData_, 1,
                                TIME_MS2I(20));
      // timeout
      if (!n) {
        initPacket();
        // waiting for 1st packet before taking care of timeout
        if (!nbExValidPacket_) {
          continue;
        }
        signalEvents(EXBUS_TIMEOUT);
      }

      if (exDecode(rxData_)) {
        nbExPacket_++;

        if (checkCRC() == JETI_EXBUS_PKT_CRC_OK) {
          nbExValidPacket_++;

          if (exPacket_.dataId == JETI_EX_ID_CHANNEL) {
            uint8_t *p = exPacket_.data;

            chSysLock();
            for (int i = 0; i < EX_NB_SERVOS; ++i) {
              servoPosition_[i] = *(uint16_t*)p;
              p += 2;
            }
            chSysUnlock();

            signalEvents(EXBUS_SERVO_POSITIONS);
            continue;
          }
          else if (exPacket_.dataId == JETI_EX_ID_TELEMETRY) {
            signalEvents(EXBUS_TELEMETRY);
            processTelemetryRequest();
            continue;
          }
          else if (exPacket_.dataId == JETI_EX_ID_JETIBOX) {
            signalEvents(EXBUS_JETIBOX);
            processJetiBoxRequest();
            continue;
          }
        }

        signalEvents(EXBUS_CRC_ERR);
        nbExInvalidPacket_++;
        initPacket();
      }
    }
  }

  void CExBusUart::processTelemetryRequest() {
    // send the text descriptors only the first 128 requests
    if (nbExTelemetryPktSent_ < 128) {
      //sdWrite(driver_, static_cast<const uint8_t*>(&telemetryTextPkt_[telemetryTextPktIndex_]), 1);
      telemetryTextPktIndex_++;
      if (telemetryTextPktIndex_ == telemetryTextPkt_.size())
        telemetryTextPktIndex_ = 0;
      nbExTelemetryPktSent_++;
      return;
    }
  }

  void CExBusUart::processJetiBoxRequest() {

  }

  bool CExBusUart::exDecode(int8_t data) {
    switch (state_) {
      // header
      case 0:
        if (data == 0x3E || data == 0x3D) {
          exPacket_.header[0] = data;
          state_++;
        }
        break;

        // header
      case 1:
        if (data == 0x01 || data == 0x03) {
          exPacket_.header[1] = data;
          state_++;
        }
        else
          state_ = 0;
        break;

        // packet length
      case 3:
        exPacket_.pktLen = data;
        state_++;
        break;

        // packet ID
      case 4:
        exPacket_.packetId = data;
        state_++;
        break;

        // data ID
      case 5:
        exPacket_.dataId = data;
        state_++;
        break;

        // data length
      case 6:
        exPacket_.dataLength = data;
        state_++;
        break;

        // data
      default:
        exPacket_.data[state_ - 6] = rxData_;
        state_++;
        if (state_ == exPacket_.pktLen) {
          return true;
        }
        break;
    }

    return false;
  }

  bool CExBusUart::checkCRC() {
    uint16_t crc = (exPacket_.data[exPacket_.pktLen - 7] << 8)
        | exPacket_.data[exPacket_.pktLen - 8];

    if (crc != get_crc16((uint8_t*)&exPacket_, exPacket_.pktLen - 2))
      return !JETI_EXBUS_PKT_CRC_OK;
    else
      return JETI_EXBUS_PKT_CRC_OK;
  }

  void CExBusUart::initPacket() {
    int8_t *p = (int8_t*)&exPacket_;

    for (int i = 0; i < 4; ++i) {
      *p++ = 0;
    }

    // create telemetry text packet for device
    telemetryTextPkt_[0].header[0] = 0x3B;
    telemetryTextPkt_[0].header[1] = 0x01;
    telemetryTextPkt_[0].pktLen = 8 + exDevice_.getTextDescriptorSize() - 1; // 0x7E separator is not sent
    telemetryTextPkt_[0].packetId = 0;
    telemetryTextPkt_[0].dataId = 0x3A;
    telemetryTextPkt_[0].dataLength = exDevice_.getTextDescriptorSize() - 1;

    for (size_t i = 0; i < exDevice_.getTextDescriptorSize() - 1; ++i) {
      telemetryTextPkt_[0].data[i] = exDevice_.getTextDescriptor()[i + 1]; // 0x7E separator is skipped
    }

    // initialize telemetry text packet for device's sensors
    int idx = 1;
    for (auto& s : exDevice_.getSensorCollection()) {
      telemetryTextPkt_[idx].header[0] = 0x3B;
      telemetryTextPkt_[idx].header[1] = 0x01;
      telemetryTextPkt_[idx].pktLen = 8 + s.getTextDescriptorSize() - 1; // 0x7E separator is not sent
      telemetryTextPkt_[idx].packetId = 0;
      telemetryTextPkt_[idx].dataId = 0x3A;
      telemetryTextPkt_[idx].dataLength = s.getTextDescriptorSize() - 1;

      for (size_t i = 0; i < s.getTextDescriptorSize() - 1; ++i) {
        telemetryTextPkt_[idx].data[i] = s.getTextDescriptor()[i + 1]; // 0x7E separator is skipped
      }
      idx++;
    }

    // initialize telemetry data packet
    telemetryDataPkt_.header[0] = 0x3B;
    telemetryDataPkt_.header[1] = 0x01;
    telemetryDataPkt_.pktLen = 0;
    telemetryDataPkt_.packetId = 0;
    telemetryDataPkt_.dataId = 0x3A;
  }
} /* namespace ExPowerBox */
