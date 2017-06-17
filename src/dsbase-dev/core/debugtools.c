/*
 * debugtools.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include "debugtools.h"
#include "uart.h"
#include "utils.h"

// NOTE:  Virtually everything in this file should be inside of the
// __DEBUG__ #ifdef, as the implementation of all these things
// should go away when compiled for release
#ifdef __DEBUG__

FILE_STATIC char debugStringBuff[MAX_DEBUG_BUFFER_SIZE];

void debugInit()
{
    uartInit();
    // TODO:  setup "debug console"
}

void debugPrint(uint8_t * buff, uint8_t szBuff)
{
    uartTransmit(buff, szBuff);
}

void debugPrintF(const char *_format, ...)
{
    int numBytes;

    va_list argptr;
    va_start(argptr, _format);
    numBytes = vsnprintf(debugStringBuff, MAX_DEBUG_BUFFER_SIZE, _format, argptr);
    uartTransmit(debugStringBuff, numBytes + 1);
}


#else  /* __DEBUG__ not specified, therefore nop debug operations */

void debugInit() {}
void debugPrint(uint8_t * buff, uint8_t szBuff) {}
void debugPrintF(const char *_format, ...) {}


#endif /* __DEBUG__ */
