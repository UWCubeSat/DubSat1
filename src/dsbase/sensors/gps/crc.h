#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

uint32_t crc32Value(int32_t);
uint32_t calculateBlockCrc32(uint32_t ulCount, uint8_t *ucBuffer);
uint32_t continueCrc32(uint32_t ulCRC, uint32_t ulCount, uint8_t *ucBuffer);

#endif /* CRC_H_ */
