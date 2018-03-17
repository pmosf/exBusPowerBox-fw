#include "CJetibox.hpp"
#include "crc.h"

namespace Jeti {
  namespace Device {
    const char* CJetibox::menu_[] = {"exBusPowerBox   ", "                "};

    CJetibox::CJetibox() {
      isInitialized_ = false;
    }

    CJetibox::~CJetibox() {
    }

    packet_t* CJetibox::getPacket(uint8_t button) {
      osalDbgCheck(isInitialized_ != false);
      return &exPacket_;
    }

    void CJetibox::init() {
      osalDbgCheck(isInitialized_ != true);

      exPacket_.header[0] = 0x3B;
      exPacket_.header[1] = 0x01;
      exPacket_.pktLen = 0x28;
      exPacket_.packetId = 0;
      exPacket_.dataId = 0x3B;
      exPacket_.dataLength = 0x20;

      int i = 0;
      for (int j = 0; j < 16; ++j) {
        exPacket_.data[i++] = menu_[0][j];
      }

      for (int j = 0; j < 16; ++j) {
        exPacket_.data[i++] = menu_[1][j];
      }

      isInitialized_ = true;
    }
  }
}
