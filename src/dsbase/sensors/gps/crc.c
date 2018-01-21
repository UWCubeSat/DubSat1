#define CRC32_POLYNOMIAL 0xEDB88320L

#include "crc.h"

// Calculate a CRC value to be used by CRC calculation functions.
uint32_t crc32Value(int32_t i)
{
    int32_t j;
    uint32_t ulCRC;
    ulCRC = i;
    for (j = 8; j > 0; j--)
    {
        if (ulCRC & 1)
        {
            ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
        }
        else
        {
            ulCRC >>= 1;
        }
    }
    return ulCRC;
}

/**
 * Calculates the CRC-32 of a block of data all at once
 *
 * Arguments:
 *   -- ulCount: number of bytes in the data block
 *   -- ucBuffer: data block
 */
uint32_t calculateBlockCrc32(uint32_t ulCount, uint8_t *ucBuffer)
{
    return continueCrc32(0, ulCount, ucBuffer);
}

uint32_t continueCrc32(uint32_t ulCRC, uint32_t ulCount, uint8_t *ucBuffer)
{
    uint32_t ulTemp1;
    uint32_t ulTemp2;
    while (ulCount-- != 0)
    {
        ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
        ulTemp2 = crc32Value(((int16_t) ulCRC ^ *ucBuffer++) & 0xff);
        ulCRC = ulTemp1 ^ ulTemp2;
    }
    return (ulCRC);
}
