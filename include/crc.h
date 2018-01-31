
#ifndef __CRC_H
#define __CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

uint16_t get_crc16(uint8_t *p, uint16_t len);
uint8_t get_crc8(const uint8_t* data, uint8_t len);

#ifdef __cplusplus
}
#endif
#endif
