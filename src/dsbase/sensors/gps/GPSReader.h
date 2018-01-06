#ifndef GPSREADER_H_
#define GPSREADER_H_

#include <stdint.h>
#include <stdbool.h>

#include "GPSPackage.h"

/**
 * A function to override the default parsing behavior of placing the received
 * byte directly into the message struct.
 *
 * message:   the message to write into
 * rcvdbyte:  the received byte
 * bytesRead: the index of this byte within the raw input message
 */
typedef void (*gps_parser)(GPSMessage *message, uint8_t rcvdbyte, uint8_t bytesRead);

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

/**
 * Register a callback to parse a message. Returns true iff success.
 */
bool gpsRegisterParser(gps_message_id messageId, gps_parser handler);

#endif /* GPSREADER_H_ */
