/*
 * BufferedReader.c
 *
 *  Created on: Jan 20, 2018
 *      Author: djdup
 *
 */

#include <stdint.h>

#ifdef __MSP430FR5994__
#include <intrinsics.h>
#endif

#include "BufferedReader.h"
#include "core/utils.h"
#include "core/uart.h"

FILE_STATIC uint8_t *buf;
FILE_STATIC reader_index bufSize;

FILE_STATIC reader_index head;
FILE_STATIC reader_index num;

FILE_STATIC uint8_t overrun_flag;

FILE_STATIC void readCallback(uint8_t byte);

void BufferedReaderInit(hBus uartHandle,
                        uint8_t *buffer,
                        reader_index bufferSize)
{
    uartRegisterRxCallback(uartHandle, readCallback);
    buf = buffer;
    bufSize = bufferSize;
    head = 0;
    num = 0;
}

reader_index BufferedReaderRead(uint8_t *buffer,
                                reader_index numToRead,
                                reader_index offset)
{
    /*
     * Disable interrupts so the readCallback doesn't change head or num while
     * this reads. UART has a small read buffer, so we shouldn't miss any bytes
     * unless there is a lot to read (in which case this should be more fine-
     * grained concurrency with locks/unlocks happening inside the while loop)
     */
    unsigned short interruptState = __get_interrupt_state();
    __disable_interrupt();

    reader_index numRead = 0;
    while (numRead < numToRead && num)
    {
        buffer[offset + numRead] = buf[(head + num) % bufSize];
        num--;
        head++;
        numRead++;
    }

    __set_interrupt_state(interruptState);

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
        overrun_flag = 1;
        return;
    }
    buf[(head + num) % bufSize] = byte;
    num++;
}

uint8_t BufferedReaderOverrun()
{
    return overrun_flag;
}

void BufferedReaderFlush()
{
    unsigned short interruptState = __get_interrupt_state();
    __disable_interrupt();

    head = 0;
    num = 0;
    overrun_flag = 0;

    __set_interrupt_state(interruptState);
}
