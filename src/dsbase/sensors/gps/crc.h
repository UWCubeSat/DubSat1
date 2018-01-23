#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

// Calculate a CRC value to be used by CRC calculation functions.
uint32_t crc32Value(int32_t);

/**
 * Calculates the CRC-32 of a block of data all at once
 *
 * Arguments:
 *   -- ulCount: number of bytes in the data block
 *   -- ucBuffer: data block
 */
uint32_t calculateBlockCrc32(uint32_t ulCount, uint8_t *ucBuffer);

/**
 * Continue to calculate the CRC-32 of a block of data starting from a partial
 * calculation.
 *
 * Arguments:
 *   -- ulCRC: the partial calculation
 *   -- ulCount: number of bytes in the data block
 *   -- ucBuffer: data block
 */
uint32_t continueCrc32(uint32_t ulCRC, uint32_t ulCount, uint8_t *ucBuffer);

#endif /* CRC_H_ */
