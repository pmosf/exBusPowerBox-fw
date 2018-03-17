#pragma once
#include "CExBus.hpp"

namespace Jeti {
  namespace Device {
    class CJetibox {
      public:
        CJetibox();
        ~CJetibox();
        void init();
        packet_t* getPacket(uint8_t button);

      private:
        bool isInitialized_;
        packet_t exPacket_ __attribute__((aligned(4)));
        char text_[2][16];
        static const char *menu_[2];
    };
  }
}
