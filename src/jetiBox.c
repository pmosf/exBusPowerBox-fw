#include <stdbool.h>
#include <ch.h>
#include "jetibox.h"
#include "acquisition.h"
#include "crc.h"

static bool jbInitDone = false;
static uint8_t jbText[2][JETIBOX_MAX_CHAR_LINE];

static void jbSetText(uint8_t line, const char *txt);
static uint8_t getNextState(uint8_t state, uint8_t key);

enum {
	JBSTATE_NULL = 0,
	JBSTATE_WELCOME = 1,
	JBSTATE_LOG,
	JBSTATE_LOG_COUNTER,
	JBSTATE_LOG_COUNTER_CRC,
	JBSTATE_LOG_COUNTER_CRC_BUS1,
	JBSTATE_LOG_COUNTER_CRC_BUS1_RESET,
	JBSTATE_LOG_COUNTER_CRC_BUS2,
	JBSTATE_LOG_COUNTER_CRC_BUS2_RESET,
	JBSTATE_LOG_COUNTER_TIMEOUT,
	JBSTATE_LOG_COUNTER_TIMEOUT_BUS1,
	JBSTATE_LOG_COUNTER_TIMEOUT_BUS1_RESET,
	JBSTATE_LOG_COUNTER_TIMEOUT_BUS2,
	JBSTATE_LOG_COUNTER_TIMEOUT_BUS2_RESET,
	JBSTATE_LOG_CAPACITY,
	JBSTATE_LOG_MINMAX,
	JBSTATE_RESET,
	JBSTATE_RESET_CAPACITY,
	JBSTATE_RESET_CBAT1,
	JBSTATE_RESET_CBAT1_FUNC,
	JBSTATE_RESET_CBAT2,
	JBSTATE_RESET_CBAT2_FUNC,
	JBSTATE_CONFIG,
	JBSTATE_INFO
};

static const char JBSCREEN_WELCOME1[] = "eBox";
static const char JBSCREEN_WELCOME2[] = "V1.0-2019 PP";

static const char JBSCREEN_LOG[] = "Log>";
static const char JBSCREEN_LOG_COUNTER[] = "Counter>";
static const char JBSCREEN_LOG_COUNTER_CRC[] = "CRC error>";
static const char JBSCREEN_LOG_COUNTER_TIMEOUT[] = "<Timeout";
static const char JBSCREEN_LOG_CAPACITY[] = "<Capacity>";
static const char JBSCREEN_LOG_MINMAX[] = "<Min/Max";

static const char JBSCREEN_CONFIG[] = "<Config>";


static const char JBSCREEN_BAT1[] = "Bat 1>";
static const char JBSCREEN_BAT2[] = "<Bat 2";
static const char JBSCREEN_EXBUS1[] = "exBus 1>";
static const char JBSCREEN_EXBUS2[] = "<exBus 2";
static const char JBSCREEN_VALID[] = "Press <- ->";

static const char JBSCREEN_INFO[] = "<Info";

static const jetibox_menu_next_t jbMenuNextstate[] = {
	//  STATE					INPUT				NEXT STATE
    {JBSTATE_WELCOME,			JETIBOX_KEY_DOWN,	JBSTATE_LOG},

	{JBSTATE_LOG,				JETIBOX_KEY_UP,		JBSTATE_WELCOME},
	{JBSTATE_LOG,				JETIBOX_KEY_RIGHT,	JBSTATE_CONFIG},
	{JBSTATE_LOG,				JETIBOX_KEY_DOWN,	JBSTATE_LOG_COUNTER},

	{JBSTATE_LOG_COUNTER,		JETIBOX_KEY_UP,		JBSTATE_LOG},
	{JBSTATE_LOG_COUNTER,		JETIBOX_KEY_RIGHT,	JBSTATE_LOG_CAPACITY},
	{JBSTATE_LOG_COUNTER,		JETIBOX_KEY_DOWN,	JBSTATE_LOG_COUNTER_CRC},

	{JBSTATE_LOG_CAPACITY,		JETIBOX_KEY_LEFT,	JBSTATE_LOG_COUNTER},
	{JBSTATE_LOG_CAPACITY,		JETIBOX_KEY_RIGHT,	JBSTATE_LOG_MINMAX},

	{JBSTATE_LOG_MINMAX,		JETIBOX_KEY_LEFT,	JBSTATE_LOG_CAPACITY},

	{JBSTATE_CONFIG,			JETIBOX_KEY_UP,		JBSTATE_WELCOME},
	{JBSTATE_CONFIG,			JETIBOX_KEY_LEFT,	JBSTATE_LOG},
    {JBSTATE_CONFIG,			JETIBOX_KEY_RIGHT,	JBSTATE_RESET},

	{JBSTATE_RESET,				JETIBOX_KEY_UP,		JBSTATE_WELCOME},
	{JBSTATE_RESET,				JETIBOX_KEY_LEFT,	JBSTATE_CONFIG},
	{JBSTATE_RESET,				JETIBOX_KEY_RIGHT,  JBSTATE_INFO},
	{JBSTATE_RESET,				JETIBOX_KEY_DOWN,	JBSTATE_RESET_CAPACITY},
	{JBSTATE_RESET_CAPACITY,	JETIBOX_KEY_UP,		JBSTATE_RESET},
	{JBSTATE_RESET_CAPACITY,	JETIBOX_KEY_LEFT,	JBSTATE_RESET_CBAT1},
	{JBSTATE_RESET_CBAT1,		JETIBOX_KEY_UP,		JBSTATE_RESET_CAPACITY},
	{JBSTATE_RESET_CBAT1,		JETIBOX_KEY_RIGHT,	JBSTATE_RESET_CBAT2},
	{JBSTATE_RESET_CBAT2,		JETIBOX_KEY_UP,		JBSTATE_RESET_CAPACITY},
	{JBSTATE_RESET_CBAT2,		JETIBOX_KEY_LEFT,	JBSTATE_RESET_CBAT1},

	{JBSTATE_INFO,				JETIBOX_KEY_UP,		JBSTATE_WELCOME},
	{JBSTATE_INFO,				JETIBOX_KEY_LEFT,	JBSTATE_RESET},

	{JBSTATE_NULL, 0, JBSTATE_NULL}
};

static const jetibox_menu_state_t jbMenuState[] = {
	//  state					line1, line2                					function	arg
	{JBSTATE_WELCOME,			JBSCREEN_WELCOME1, JBSCREEN_WELCOME2,			NULL, 0},

	{JBSTATE_LOG,				JBSCREEN_LOG, NULL,								NULL, 0},

	{JBSTATE_CONFIG,			JBSCREEN_CONFIG, NULL,							NULL, 0},

	{JBSTATE_RESET_CAPACITY,	JBSCREEN_LOG_CAPACITY, NULL,					NULL, 0},
	{JBSTATE_RESET_CBAT1,		JBSCREEN_BAT1, NULL,							NULL, 0},
	{JBSTATE_RESET_CBAT1_FUNC, 	NULL, NULL,										acqResetCapacity, 0},
	{JBSTATE_RESET_CBAT2,		JBSCREEN_BAT2, NULL,							NULL, 0},
	{JBSTATE_RESET_CBAT2_FUNC, 	NULL, NULL,										acqResetCapacity, 1},

	{JBSTATE_INFO, 				JBSCREEN_INFO, NULL,							NULL, 0},

	{JBSTATE_NULL, NULL, NULL, NULL, 0}
};

void jetiboxInit(void)
{
	chDbgCheck(jbInitDone != true);

	jbSetText(JETIBOX_LINE1, JBSCREEN_WELCOME1);
	jbSetText(JETIBOX_LINE2, JBSCREEN_WELCOME2);

	jbInitDone = true;
}

uint8_t jetiboxProcessKey(uint8_t key, uint8_t *text)
{
	static uint8_t state = JBSTATE_WELCOME;
	static uint8_t nextState = JBSTATE_WELCOME;
	static uint8_t (*stateFunc)(uint8_t arg) = NULL;
	uint8_t stateFuncArg = 0;

	if (key != JETIBOX_KEY_REFRESH) {
		nextState = getNextState(state, key);
	}

	if (nextState != state) {
		state = nextState;
		for (uint8_t i = 0; jbMenuState[i].state != JBSTATE_NULL; i++) {
			if (jbMenuState[i].state == state) {
				jbSetText(JETIBOX_LINE1, jbMenuState[i].txtLine1);
				jbSetText(JETIBOX_LINE2, jbMenuState[i].txtLine2);
				stateFunc = jbMenuState[i].func;
				stateFuncArg = jbMenuState[i].funcArg;
				break;
			}
		}
	}

	if (stateFunc){
		stateFunc(stateFuncArg);
		stateFunc = NULL;
	}

	for (uint8_t i = 0; i < (JETIBOX_MAX_CHAR_LINE << 1); i++) {
		text[i] = ((uint8_t *)jbText)[i];
	}

	return 0;
}

static uint8_t getNextState(uint8_t state, uint8_t key)
{
	uint8_t nextstate = state;    // Default stay in same state

	for (uint8_t i = 0; jbMenuNextstate[i].state != JBSTATE_NULL; i++) {
		if (jbMenuNextstate[i].state == state && jbMenuNextstate[i].input == key) {
			nextstate = jbMenuNextstate[i].nextstate;
			break;
		}
	}

	return nextstate;
}

static void jbSetText(uint8_t line, const char *txt)
{
	uint8_t *pTxt = line == JETIBOX_LINE1 ? &jbText[JETIBOX_LINE1][0] : &jbText[JETIBOX_LINE2][0];
	bool padding =  txt ? false : true;

	for (uint8_t i = 0; i < JETIBOX_MAX_CHAR_LINE; i++) {
		if (!padding && !txt[i]) {
			padding = true;
		}
		if (!padding) {
			pTxt[i] = txt[i];
		} else {
			pTxt[i] = ' ';
		}
	}
}
