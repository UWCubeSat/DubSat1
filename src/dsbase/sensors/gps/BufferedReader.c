/*
 * BufferedReader.c
 *
 *  Created on: Jan 20, 2018
 *      Author: djdup
 *
 * TODO handle race conditions
 */

#include <stdint.h>

#include "BufferedReader.h"
#include "core/utils.h"
#include "core/uart.h"

FILE_STATIC hbus handle;
FILE_STATIC uint8_t *buf;
FILE_STATIC reader_index bufSize;

FILE_STATIC reader_index head;
FILE_STATIC reader_index num;

FILE_STATIC void readCallback(uint8_t byte);

void BufferedReaderInit(hbus uartHandle,
                        uint8_t *buffer,
                        reader_index bufferSize)
{
    uartRegisterRxCallback(readCallback);
    handle = uartHandle;
    buf = buffer;
    bufSize = bufferSize;
    head = 0;
    num = 0;
}

reader_index BufferedReaderRead(uint8_t *buffer,
                                reader_index numToRead,
                                reader_index offset)
{
    reader_index numRead;
    while (numToRead-- && num)
    {
        buffer[offset + numToRead] = buf[(head + num) % bufSize];
        num--;
        head++;
        numRead++;
    }
    return numRead;
}

void BufferedReaderReadFully(uint8_t *buffer,
                             reader_index numToRead,
                             reader_index offset)
{
    reader_index numRead = 0;
    while (numRead < numToRead)
    {
        numRead += BufferedReaderRead(buffer,
                                      numToRead - numRead,
                                      offset + numRead);
    }
}

FILE_STATIC void readCallback(uint8_t byte)
{
    if (num >= bufSize)
    {
        // TODO handle buffer full errors
    }
    buf[(head + num) % bufSize] = byte;
    num++;
}
