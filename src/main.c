#include "ch.h"
#include "hal.h"
#include "main_thread.h"
#include "servos.h"
#include "acquisition.h"
#include "gps.h"

#if USE_USB
static USBConfig usbCfg;
static SerialUSBConfig serialUsbCfg;
static SerialUSBDriver serialUsbDriver;
#endif

I2CConfig i2cConfig = { I2C_100K, 0, 0 };

int main(void) {

	halInit();
	chSysInit();

	chThdSleepMilliseconds(1000);

#if USE_USB
	// USB initialization
	sduObjectInit(USB_PORT);
	sduStart(USB_PORT, &serusbcfg);
	/*
	 * Activates the USB driver and then the USB bus pull-up on D+.
	 * Note, a delay is inserted in order to not have to disconnect the cable
	 * after a reset.
	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1500);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);
#endif

	// I2C bus
	i2cStart(I2C_DRIVER, &i2cConfig);

	servoInit();
	acqInit();
	gps_init();
	mt_init();

	main_thread();
}
