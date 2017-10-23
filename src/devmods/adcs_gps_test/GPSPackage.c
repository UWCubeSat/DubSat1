#include <stdbool.h>

#include "GPSPackage.h"
#include "crc.h"

bool isGPSPackageValid(GPSPackage *p)
{
    uint32_t actualCrc = calculateBlockCrc32(
            p->header.headerLength + p->header.messageLength, (uint8_t *) p);
    return p->crc == actualCrc;
}
