#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"

#include "board_conf.h"
#include "main_thread.h"
#include "exBus.h"
#include "usbcfg.h"
#include "servos.h"
#include "events.h"
#include "ltc2943.h"
#include "max6639.h"

static void processExBusTimeout(void);
static void processTelemetryRequest(void);
static void processJetibox(void);
static void processExBusError(void);

static bool init_done = false;
static bool uartDataReceived;
static uint32_t nbExBusTimeout[NB_EX_UART];
static uint32_t nbTotalExBusTimeout[NB_EX_UART];
static SerialDriver *sd[NB_EX_UART] = EX_UARTS;
static exbus_t exbus;
static uint16_t servoPosition[EX_NB_SERVOS];
static uint16_t servoFailSafePosition[EX_NB_SERVOS];

void mt_init(void)
{
	chDbgCheck(init_done != true);

	SerialConfig sd_conf = { EXBUS_UART_SPEED, 0, USART_CR2_STOP_1, USART_CR3_HDSEL };

	uartDataReceived = false;

	// UART and associated variables
	for (int i = 0; i < NB_EX_UART; ++i) {
		nbExBusTimeout[i] = 0;
		nbTotalExBusTimeout[i] = 0;
		sdStart(sd[i], &sd_conf);
	}

	for (int i = 0; i < EX_NB_SERVOS; i++) {
		servoPosition[i] = 0;
		servoFailSafePosition[i] = 1.5E-3 * EXBUS_PWM_FREQ;
	}

	exbus_init(&exbus);

	init_done = true;
}

void main_thread(void)
{
	chRegSetThreadName("main_thread");
	chDbgCheck(init_done != false);

	while (true) {
		// set servos to fail-safe positions if no activity on exbus uarts
		if (exbus.busSel == NB_EX_UART) {
			servoSetPositions(servoFailSafePosition);
			exbus.busSel = 0;
		}

		// wait for data
		size_t n = chnReadTimeout((BaseChannel *)sd[exbus.busSel],
		                          &exbus.rx_data, 1, TIME_MS2I(20));

		// process timeout
		if (!n) {
			exbus_reset(&exbus);
			// wait for one valid event before timeout detection
			if (!exbus.nbExValidPacket || !exbus.isLinkUp) {
				continue;
			}
			// in case of 4 consecutive timeouts, we select another exbus receiver
			processExBusTimeout();
			continue;
		}

		// frame complete ?
		if (!exbus_decode(&exbus)) {
			continue;
		}

		exbus.nbExPacket++;

		if (exbus_check_crc(&exbus) == JETI_EXBUS_PKT_CRC_OK) {
			exbus.nbExValidPacket++;

			if (exbus.pkt.dataId == JETI_EXBUS_ID_CHANNEL) {
				uint16_t *p = (__packed uint16_t *)(exbus.pkt.data);
				for (size_t i = 0; i < EX_NB_SERVOS; i++) {
					servoPosition[i] = *p++;
				}
				exbus.isLinkUp = true;
				nbExBusTimeout[exbus.busSel] = 0;
				servoSetPositions(servoPosition);
				// channel data with telemetry request
				if (exbus.pkt.header[1] == 0x01) {
					processTelemetryRequest();
				}
				exbus_reset(&exbus);
				continue;
			}
			if (exbus.pkt.dataId == JETI_EXBUS_ID_TELEMETRY
			    && exbus.pkt.header[1] == 1) {
				exbus.isLinkUp = true;
				processTelemetryRequest();
				exbus_reset(&exbus);
				continue;
			}
			if (exbus.pkt.dataId == JETI_EXBUS_ID_JETIBOX
			    && exbus.pkt.header[1] == 1) {
				exbus.isLinkUp = true;
				processJetibox();
				exbus_reset(&exbus);
				continue;
			}
		}

		if (exbus.isLinkUp) {
			processExBusError();
		}
		exbus.nbExInvalidPacket++;
		exbus_reset(&exbus);
	}
}

static void processExBusError(void)
{

}

static void processJetibox(void)
{
	exbusGetJetibox(&exbus.pkt);
	chnWrite(sd[exbus.busSel], (const uint8_t *)&exbus.pkt, exbus.pkt.pktLen);
}

static void processTelemetryRequest(void)
{
	static uint8_t nbTxtPkt = 0;
	// send the text packets only the first 256 requests
	if (nbTxtPkt < 0xFF) {
		exbusGetNextTxtPkt(&exbus.pkt);
		nbTxtPkt++;
	} else {
		exbusGetNextDataPkt(&exbus.pkt);
	}

	chnWrite(sd[exbus.busSel], (const uint8_t *)&exbus.pkt, exbus.pkt.pktLen);
#if USE_USB && defined(USB_DEBUG)
	chnWriteTimeout(&PORTAB_SDU1, (const uint8_t*)&exbus.pkt,
	                 exbus.pkt.pktLen, TIME_IMMEDIATE);
#endif
}

static void processExBusTimeout(void)
{
	nbTotalExBusTimeout[exbus.busSel]++;
	if (++nbExBusTimeout[exbus.busSel] == 4) {
		nbExBusTimeout[exbus.busSel] = 0;
		exbus.busSel++;
	}
}
