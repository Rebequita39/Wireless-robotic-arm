#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// CRC8: poly 0x07, init 0x00, no reflection, no final XOR.
// Test vector: "123456789" -> 0xF4. Bitwise (no table) to save flash on AVR.
uint8_t crc8(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif
