/*
 * debugtools.c
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#include "debugtools.h"
#include "uart.h"

// NOTE:  Virtually everything in this file should be inside of the
// __DEBUG__ #ifdef, as the implementation of all these things
// should go away when compiled for release
#ifdef __DEBUG__

void debug_print(char * buff, uint8_t szBuff)
{
    uartInit();
    uartTransmit(buff, szBuff);
}

#else  /* __DEBUG__ not specified, therefore nop debug operations */

void debug_print(char * buff, uint8_t szBuff) {}


#endif /* __DEBUG__ */
