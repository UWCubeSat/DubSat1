/*
 * BufferedReader.h
 *
 *  Created on: Jan 20, 2018
 *      Author: djdup
 */

#ifndef DSBASE_SENSORS_GPS_BUFFEREDREADER_H_
#define DSBASE_SENSORS_GPS_BUFFEREDREADER_H_

#include <stdint.h>

#include "core/utils.h"

typedef uint16_t reader_index;

/**
 * Registers a callback on the uartHandle.
 */
void BufferedReaderInit(hBus uartHandle,
                        uint8_t *buffer,
                        reader_index bufferSize);

/**
 * Returns the actual number of bytes read, which may be less than numToRead
 */
reader_index BufferedReaderRead(uint8_t *buffer,
                                reader_index numToRead,
                                reader_index offset);

/**
 * Block until numToRead has actually been read
 */
void BufferedReaderReadFully(uint8_t *buffer,
                             reader_index numToRead,
                             reader_index offset);

/**
 * Returns 1 iff incoming bytes were lost
 */
uint8_t BufferedReaderOverrun();

void BufferedReaderFlush();

#endif /* DSBASE_SENSORS_GPS_BUFFEREDREADER_H_ */
