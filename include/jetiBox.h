#pragma once

#define JETIBOX_KEY_UP      0xd0
#define JETIBOX_KEY_DOWN    0xb0
#define JETIBOX_KEY_LEFT    0x70
#define JETIBOX_KEY_RIGHT   0xe0
#define JETIBOX_KEY_LR      0x60
#define JETIBOX_KEY_UD      0x90
#define JETIBOX_KEY_REFRESH 0xFF

//      4 BIT zulässige Key-Richtungen
//      4 BIT NC
//      4 BIT X-Pos (<->)
//      4 BIT Y-Pos (^^)
#define __MTYP(Key,X,Y)  (uint16_t)(((Key)<<12) | (X<<4) | Y)
#define __MPOS(MTYP)     (MTYP&0x00ff)
#define __MPOSX(MTYP)   ((MTYP&0x00f0)>>4)
#define __MPOSY(MTYP)    (MTYP&0x0f)

#define L 0x8    // left
#define R 0x4    // right
#define U 0x2    // up
#define D 0x1    // down

union JETIBOX_U_MVAL {
	uint16_t Val;
	struct {
		uint16_t NC         : 12;
		uint16_t KeyDown    : 1;
		uint16_t KeyUp      : 1;
		uint16_t KeyRight   : 1;
		uint16_t KeyLeft    : 1;
	} s1;
	struct {
		uint16_t PosY       : 4;
		uint16_t PosX       : 4;
		uint16_t NC         : 4;
		uint16_t KeyAllowed : 4;
	} s2;
};

typedef struct {
	union JETIBOX_U_MVAL u;
	char line[16];
} jetibox_menu_t;


// JB Sonderzeichen
// 0x12 Pfeil ho-ru
// 0x1d Pfeil re-li
// 0x1a Pfeil rechts
// 0x1b Pfeil links
// 0x18 Pfeil hoch
// 0x7e Pfeil doppelt rechts
// 0x7f Pfeil doppelt links

#include "jetiBoxMenu.h"

void jetibox_init(void);
void jetiboxProcessKey(uint8_t key, uint8_t *text);
