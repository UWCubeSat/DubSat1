/*
 * rollcall.c
 *
 *  Created on: May 9, 2018
 *      Author: djdup
 */

#include "rollcall.h"
#include "core/utils.h"

FILE_STATIC uint8_t rcFlag;

FILE_STATIC const rollcall_fn *functions;
FILE_STATIC uint8_t numFunctions;

void rollcallInit(const rollcall_fn *fns, uint8_t num)
{
    rcFlag = 0;
    functions = fns;
    numFunctions = num;
}

void rollcallStart()
{
    rcFlag = numFunctions;
}

void rollcallUpdate()
{
    while(rcFlag && (canTxCheck() != CAN_TX_BUSY))
    {
        // populate the next rollcall packet
        uint8_t idx = rcFlag - 1;
        CANPacket p;
        functions[idx](&p);

        // send the packet
        // If the packet fails to send, keep trying until it works or until CAN
        // says it's busy.
        uint8_t notSent;
        do
        {
            notSent = canSendPacket(&p);
        } while (notSent && (canTxCheck() != CAN_TX_BUSY));

        // decrement to move on to next packet
        rcFlag--;
    }
}
