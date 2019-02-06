#pragma once

#define JETIBOX_LINE1			0
#define JETIBOX_LINE2			1
#define JETIBOX_MAX_CHAR_LINE	16

#define JETIBOX_KEY_UP      0xD0
#define JETIBOX_KEY_DOWN    0xB0
#define JETIBOX_KEY_LEFT    0x70
#define JETIBOX_KEY_RIGHT   0xE0
#define JETIBOX_KEY_LR      0x60
#define JETIBOX_KEY_UD      0x90
#define JETIBOX_KEY_REFRESH 0xFF

#define L 0x8    // left
#define R 0x4    // right
#define U 0x2    // up
#define D 0x1    // down

typedef struct
{
    const uint8_t state;
    const uint8_t input;
    const uint8_t nextstate;
} jetibox_menu_next_t;


typedef struct
{
    const uint8_t state;
    const char *txtLine1;
    const char *txtLine2;
    uint8_t (*func)(uint8_t arg);
    uint8_t funcArg;
} jetibox_menu_state_t;

void jetiboxInit(void);
uint8_t jetiboxProcessKey(uint8_t key, uint8_t *text);
