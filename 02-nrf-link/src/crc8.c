#include "crc8.h"

#define CRC8_POLY 0x07u

uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80u) ? (uint8_t)((crc << 1) ^ CRC8_POLY)
                                : (uint8_t)(crc << 1);
        }
    }
    return crc;
}
