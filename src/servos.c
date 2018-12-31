#include "servos.h"
#include "data_types.h"

static PWMDriver *pwmDriver[NB_TIM_PWM] = { &PWMD1, &PWMD2, &PWMD3, &PWMD8,
                                            &PWMD9, &PWMD12
                                          };

static pwm_settings_t pwmSettings;

static PWMConfig pwmConfig[NB_TIM_PWM] = {
	// TIM1
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	},
	// TIM2
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	},
	// TIM3
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	},
	// TIM8
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	},
	// TIM9
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_ACTIVE_HIGH, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	},
	// TIM12
	{
		0, 0, NULL, /* No callback */
		{	{ PWM_OUTPUT_ACTIVE_HIGH, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}, { PWM_OUTPUT_DISABLED, NULL }, {
				PWM_OUTPUT_DISABLED, NULL
			}
		}, 0, 0
	}
};

static bool servo_initDone = false;

void servoSetPwmSettings(uint32_t freq, float period, uint32_t widthDivider)
{
	pwmSettings.freq = freq;
	pwmSettings.period = period;
	pwmSettings.periodTick = pwmSettings.freq * pwmSettings.period;
	pwmSettings.widthDivider = widthDivider;

	for (int i = 0; i < NB_TIM_PWM; i++) {
		pwmConfig[i].frequency = pwmSettings.freq;
		pwmConfig[i].period = pwmSettings.periodTick;
	}
}

void servoSetPositions(uint16_t *pos)
{
	chDbgCheck(servo_initDone != false);

	int servoPositionIndex = 0;

	// update servo with refreshed values
	for (size_t i = 0; i < NB_TIM_PWM; i++) {
		for (int j = 0; j < pwmDriver[i]->channels; j++) {
			if (pwmDriver[i]->config->channels[j].mode) {
				pwmEnableChannel(pwmDriver[i], j,
				                 pos[servoPositionIndex++]
				                 / pwmSettings.widthDivider);
			}
		}
	}
}

void servoInit(void)
{
	chDbgCheck(servo_initDone != true);

	// set initial pwm frequency and period
	servoSetPwmSettings(2000000UL, 20E-3, 8000000UL / 2000000UL);

	// start pulse generation
	for (size_t i = 0; i < NB_TIM_PWM; ++i) {
		pwmStart(pwmDriver[i], &pwmConfig[i]);
	}

	servo_initDone = true;
}

