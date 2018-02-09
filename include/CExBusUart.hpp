/*
 * CExBusUart.hpp
 *
 *  Created on: Feb 2, 2018
 *      Author: pperonna
 */

#ifndef CEXBUSUART_HPP_
#define CEXBUSUART_HPP_

#include "ch.hpp"
#include "hal.h"
#include "CExBus.hpp"

#define EXBUS_SERVO_POSITIONS   EVENT_MASK(0)
#define EXBUS_TIMEOUT           EVENT_MASK(1)
#define EXBUS_CRC_ERR           EVENT_MASK(2)

#define EX_NB_SERVOS                16
#define EX_SERVO_POSITION_POOL_SIZE 8

namespace ExPowerBox {

  typedef struct {
    std::uint8_t header[2];
    std::uint8_t pktLen;
    std::uint8_t packetId;
    std::uint8_t dataId;
    std::uint8_t dataLength;
    std::uint8_t data[58];
  } packet_t;

  class CExBusUart: public chibios_rt::BaseStaticThread<128> {
  public:
    CExBusUart(thread_t *parent, SerialDriver *driver, const char *threadName);
    virtual ~CExBusUart();
    virtual void main(void);
    event_source_t& getEvtServoPosition();
    void getServoPosition(uint16_t* dest);
    uint16_t getServoPosition(int ch);

  private:
    thread_t *parentThread_;
    SerialDriver *driver_;
    SerialConfig serialConfig_;
    bool isClassInitialized_;
    uint8_t rxData_;
    const char *threadName_;
    packet_t exPacket_ __attribute__((aligned(4)));
    uint32_t nbExPacket_;
    uint32_t nbExValidPacket_;
    uint32_t nbExInvalidPacket_;
    int8_t state_;
    uint16_t servoPosition_[EX_NB_SERVOS];
    //mailbox_t servoPosition_;
    //msg_t servoPositionQueue_[EX_SERVO_POSITION_POOL_SIZE];
    event_source_t evtServoPosition_;

    bool exDecode(int8_t data);
    void initPacket();
    bool checkCRC();
    void setServoPositionValues();
  };

} /* namespace ExPowerBox */

#endif /* CEXBUSUART_HPP_ */
