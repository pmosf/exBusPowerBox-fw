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

static void busTimeout(void);
static void busError(void);
static void processTelemetryRequest(void);
static void processJetibox(void);

static bool mtInitDone = false;
static bool uartDataReceived;
static uint32_t nbExBusTimeout[NB_EXBUS_UART];
static SerialDriver *sd[NB_EXBUS_UART] = EXBUS_UARTS;
static exbus_t exbus;
static uint16_t servoPosition[NB_SERVOS];
static uint16_t servoFailSafePosition[NB_SERVOS];
static const uint8_t servoLUT[NB_SERVOS] = SERVO_LUT;

void mtInit(void)
{
	chDbgCheck(mtInitDone != true);

	SerialConfig sd_conf = { EXBUS_UART_SPEED, 0, USART_CR2_STOP_1, USART_CR3_HDSEL };

	uartDataReceived = false;

	// UART and associated variables
	for (int i = 0; i < NB_EXBUS_UART; ++i) {
		nbExBusTimeout[i] = 0;
		sdStart(sd[i], &sd_conf);
	}

	for (int i = 0; i < NB_SERVOS; i++) {
		servoPosition[i] = 0;
		servoFailSafePosition[i] = 1.5E-3 * EXBUS_PWM_FREQ;
	}

	exbusInit(&exbus);

	mtInitDone = true;
}

void mainThread(void)
{
	chRegSetThreadName("main_thread");
	chDbgCheck(mtInitDone != false);

	while (true) {
		// set servos to fail-safe positions if no activity on exbus uarts
		if (exbus.uartSel == NB_EXBUS_UART) {
			servoSetPositions(servoFailSafePosition);
			exbus.uartSel = 0;
		}

		// wait for data
		size_t n = chnReadTimeout((BaseChannel *)sd[exbus.uartSel], &exbus.rx_data, 1, TIME_MS2I(20));

		// process timeout
		if (!n) {
			exbusReset(&exbus);
			// wait for one valid event before timeout detection
			if (!exbus.nbExValidPacket || !exbus.isLinkUp) {
				continue;
			}
			// in case of 4 consecutive timeouts, we select another exbus receiver
			busTimeout();
			continue;
		}

		// frame complete ?
		if (!exbusDecode(&exbus)) {
			continue;
		}

		exbus.nbExPacket++;

		if (exbusCheckCRC(&exbus) == JETI_EXBUS_PKT_CRC_OK) {
			exbus.nbExValidPacket++;

			if (exbus.pkt.dataId == JETI_EXBUS_ID_CHANNEL) {
				const uint16_t *p = (const uint16_t *)exbus.pkt.data;
				for (uint8_t i = 0; i < NB_SERVOS; i++) {
					servoPosition[servoLUT[i]] = p[i];
				}
				exbus.isLinkUp = true;
				nbExBusTimeout[exbus.uartSel] = 0;
				servoSetPositions(servoPosition);
				// channel data with telemetry request
				if (exbus.pkt.header[1] == 0x01) {
					processTelemetryRequest();
				}
			} else if (exbus.pkt.dataId == JETI_EXBUS_ID_TELEMETRY && exbus.pkt.header[1] == 1) {
				exbus.isLinkUp = true;
				processTelemetryRequest();
			} else if (exbus.pkt.dataId == JETI_EXBUS_ID_JETIBOX && exbus.pkt.header[1] == 1) {
				exbus.isLinkUp = true;
				processJetibox();
			}
		} else {
			if (exbus.isLinkUp) {
				busError();
			}
			exbus.nbCrcError[exbus.uartSel]++;
		}
		exbusReset(&exbus);
	}
}

static void busError(void)
{

}

static void processJetibox(void)
{
	exbusGetJetibox(&exbus.pkt);
	chnWrite(sd[exbus.uartSel], (const uint8_t *)&exbus.pkt, exbus.pkt.pktLen);
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

	chnWrite(sd[exbus.uartSel], (const uint8_t *)&exbus.pkt, exbus.pkt.pktLen);
#if USE_USB && defined(USB_DEBUG)
	chnWriteTimeout(&PORTAB_SDU1, (const uint8_t *)&exbus.pkt,
	                exbus.pkt.pktLen, TIME_IMMEDIATE);
#endif
}

static void busTimeout(void)
{
	exbus.nbTimeout[exbus.uartSel]++;
	if (++nbExBusTimeout[exbus.uartSel] == 4) {
		nbExBusTimeout[exbus.uartSel] = 0;
		exbus.uartSel++;
	}
}
