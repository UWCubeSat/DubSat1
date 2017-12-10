#ifndef GPSREADER_H_
#define GPSREADER_H_

#include <stdint.h>
#include <stdbool.h>

#include "GPSPackage.h"

/**
 * Initialize the reader to read from uart. The maximum number of packages in
 * the buffer will be equal to bufferLength / sizeof(GPSPackage).
 */
void GPSReaderInit(hBus uartHandle, GPSPackage *buffer, uint32_t bufferLength);

/**
 * Get a pointer to the next GPSPackage. After reading, call popGPSPackage to
 * advance to the next package. Returns NULL if there is nothing to read.
 */
GPSPackage *ReadGPSPackage();

/**
 * Free up the memory from the last message and move onto the next. Do not use
 * the pointer returned from readGPSPackage after calling this function.
 */
void PopGPSPackage();

#endif /* GPSREADER_H_ */
