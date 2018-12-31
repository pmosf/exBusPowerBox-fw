#pragma once
#define JETI_EXBUS

#include <stdint.h>
#include <stdbool.h>
#include "exDevice.h"

#define EXBUS_UART_SPEED			125000
#define EXBUS_PWM_FREQ				8000000UL

#define JETI_EXBUS_ID_CHANNEL       0x31
#define JETI_EXBUS_ID_TELEMETRY     0x3A
#define JETI_EXBUS_ID_JETIBOX       0x3B

#define JETI_EXBUS_HEAD_CRC_LEN     (6 + 2)

#define JETI_EXBUS_PKT_CRC_OK       true
#define JETI_EXBUS_PKT_HEADER_OK    true

#define EX_SERVO_POSITION_POOL_SIZE 8

typedef struct {
	uint8_t header[2];
	uint8_t pktLen;
	uint8_t packetId;
	uint8_t dataId;
	uint8_t dataLength;
	uint8_t data[58];
} exbus_packet_t;

typedef struct {
	exbus_packet_t pkt;
	int8_t state;
	uint8_t rx_data;
	uint32_t nbExPacket;
	uint32_t nbExValidPacket;
	uint32_t nbExInvalidPacket;
	uint8_t busSel;
	bool isLinkUp;
} exbus_t;

void exbus_init(exbus_t *exbus);
bool exbus_decode(exbus_t *exbus);
bool exbus_check_crc(exbus_t *exbus);
void exbus_reset(exbus_t *exbus);
void exbusGetNextTxtPkt(exbus_packet_t *exbusPkt);
void exbusGetNextDataPkt(exbus_packet_t *exbusPkt);
void exbusGetJetibox(exbus_packet_t *exbusPkt);
