#include <string.h>
#include "board_conf.h"
#include "acquisition.h"
#include "evtimer.h"
#include "data_types.h"
#include "events.h"
#include "ltc2943.h"
#include "max6639.h"

#define RPM_CB(pin) chSysLockFromISR();\
    if (palReadLine(pin)) {\
        if (!flag) {\
            start = chVTGetSystemTimeX();\
            flag = true;\
        } else {\
            period = chVTTimeElapsedSinceX(start);\
            flag = false;\
        }\
    }\
    chSysUnlockFromISR();

static THD_FUNCTION(acq_fast_thread, arg);
static THD_WORKING_AREA(acq_fast_wa, 512);
static THD_FUNCTION(acq_slow_thread, arg);
static THD_WORKING_AREA(acq_slow_wa, 512);
static bool init_done = false;
static mutex_t acqMutex;
static acq_t acq;
static acqMinMax_t acqMinMax;
static sysinterval_t rpm_usperiod[2];

static void rpm1CB(void *arg);
static void rpm2CB(void *arg);

void acqInit(void)
{
	chDbgCheck(init_done != true);

	chMtxObjectInit(&acqMutex);

	max6639Init();
	ltc2943Init();

	memset(&acq, 0, sizeof(acq));
	memset(&acqMinMax, 0, sizeof(acqMinMax));

	chThdCreateStatic(acq_fast_wa, sizeof(acq_fast_wa), LOWPRIO,
	                  acq_fast_thread, NULL);
	chThdCreateStatic(acq_slow_wa, sizeof(acq_slow_wa), LOWPRIO,
	                  acq_slow_thread, NULL);

	/* Enabling events on both edges of the RPM inputs */
	palEnableLineEvent(GPIOA_RPM1, PAL_EVENT_MODE_RISING_EDGE);
	palSetLineCallback(GPIOA_RPM1, rpm1CB, NULL);
	palEnableLineEvent(GPIOA_RPM2, PAL_EVENT_MODE_RISING_EDGE);
	palSetLineCallback(GPIOA_RPM2, rpm2CB, NULL);

	rpm_usperiod[0] = 0;
	rpm_usperiod[1] = 0;

	init_done = true;
}

int32_t acqGetRpm(uint8_t chan)
{
	chDbgCheck(init_done != false);
	int32_t rpm = chTimeI2US(rpm_usperiod[chan]) * 1E-6 * 60;
	return rpm;
}

int32_t acqGetVoltage(uint8_t chan)
{
	chDbgCheck(init_done != false);
	return acq.voltage[chan];
}

int32_t acqGetCurrent(uint8_t chan)
{
	chDbgCheck(init_done != false);
	return acq.current[chan];
}

int32_t acqGetCapacity(uint8_t chan)
{
	chDbgCheck(init_done != false);
	return acq.capacity[chan];
}

int32_t acqGetLocalTemp(void)
{
	chDbgCheck(init_done != false);
	return acq.local_temp;
}

int32_t acqGetExtTemp(uint8_t chan)
{
	chDbgCheck(init_done != false);
	return acq.ext_temp[chan];
}

__attribute__((noreturn))
static THD_FUNCTION(acq_fast_thread, arg)
{
	chRegSetThreadName("HSpeedAcq");
	chDbgCheck(init_done != false);

	(void) arg;
	event_listener_t el;
	event_timer_t timer;

	evtObjectInit(&timer, TIME_S2I(1));
	chEvtRegisterMask((event_source_t *) &timer.et_es, &el,
	                  EVENT_HSA_TIMER);
	evtStart(&timer);

	while (true) {
		chEvtWaitAny(EVENT_HSA_TIMER);
		//chMtxLock(&acqMutex);
		for (int i = 0; i < NB_LTC2943; i++) {
			acq.voltage[i] = ltc2943GetVoltage(i) * 100;
			acq.current[i] = ltc2943GetCurrent(i) * 100;
		}
		//chMtxUnlock(&acqMutex);
	}
}

__attribute__((noreturn))
static THD_FUNCTION(acq_slow_thread, arg)
{
	chRegSetThreadName("LSpeedAcq");
	chDbgCheck(init_done != false);

	(void) arg;
	event_listener_t el;
	event_timer_t timer;

	evtObjectInit(&timer, TIME_S2I(2));
	chEvtRegisterMask((event_source_t *) &timer.et_es, &el,
	                  EVENT_LSA_TIMER);
	evtStart(&timer);

	while (true) {
		chEvtWaitAny(EVENT_LSA_TIMER);

		//chMtxLock(&acqMutex);
		for (int i = 0; i < NB_LTC2943; i++) {
			acq.capacity[i] = ltc2943GetCapacity(i);
		}
		acq.local_temp = ltc2943GetTemperature();
		acq.ext_temp[0] = max6639GetExtTemperature(0);
		acq.ext_temp[1] = max6639GetExtTemperature(1);
		//chMtxUnlock(&acqMutex);
	}
}

static void rpm1CB(void *arg)
{
	(void) arg;
	static systime_t start = 0;
	sysinterval_t period = 0;
	static bool flag = false;

	RPM_CB(GPIOA_RPM1);
	rpm_usperiod[0] = period;
}

static void rpm2CB(void *arg)
{
	(void) arg;
	static systime_t start = 0;
	sysinterval_t period = 0;
	static bool flag = false;

	RPM_CB(GPIOA_RPM2);
	rpm_usperiod[1] = period;
}
