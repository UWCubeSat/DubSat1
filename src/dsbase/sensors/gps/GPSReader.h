#ifndef GPSREADER_H_
#define GPSREADER_H_

#include <stdint.h>
#include <stdbool.h>

#include "GPSPackage.h"

typedef struct {
    gps_message_id msgId; // message ID of the log
    uint8_t arrayIndex;   // index of the start of the array
    uint8_t size;         // size of each element
} array_parser;

typedef struct PACKED_STRUCT {
    uint8_t skipped;
    uint8_t bad_crc;
} gpsreader_health;

/**
 * Initialize the reader to read from uart.
 */
void GPSReaderInit(hBus uartHandle);

/**
 * Returns NULL if no package is available yet
 */
GPSPackage *GPSReaderUpdate();

void GPSReaderGetHealth(gpsreader_health *health);

#endif /* GPSREADER_H_ */
