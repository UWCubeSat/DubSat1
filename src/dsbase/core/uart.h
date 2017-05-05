/*
 * uart.h
 *
 *  Created on: Apr 29, 2017
 *      Author: jeffc
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>


#define MAX_BUFFER_SIZE     32
#define INDEX_OP_COMPLETE  MAX_BUFFER_SIZE+1

void uartInit();
void uartTransmit(uint8_t * buff, uint8_t szBuff);

typedef struct {
    uint8_t initialized;

    uint8_t txOverrunCount;
    uint8_t txBufferOverflowCount;
} UART_Status;


#endif /* UART_H_ */
