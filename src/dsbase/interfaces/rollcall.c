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
FILE_STATIC CANPacket *packets = NULL;
FILE_STATIC uint8_t numFunctions;

void rollcallInit(const rollcall_fn *fns, uint8_t num)
{
    rcFlag = 0;
    functions = fns;
    numFunctions = num;
}

void rollcallInitWithBuffer(const rollcall_fn *fns, CANPacket *pkts, uint8_t num)
{
    rollcallInit(fns, num);
    packets = pkts;
}

void rollcallStart()
{
    rcFlag = numFunctions;
}

void rollcallStartMini()
{
    rcFlag=16;
}


FILE_STATIC uint8_t hasPacketBuffer()
{
    return packets != NULL;
}

/**
 * Use each function populate to populate a CAN packet in the buffer.
 * Only do this if we have a packet buffer.
 */
FILE_STATIC void populateAll()
{
    uint8_t i = numFunctions;
    while (i-- > 0)
    {
        functions[i](&packets[i]);
    }
}

uint8_t rollcallUpdate()
{
    while(rcFlag && (canTxCheck() != CAN_TX_BUSY))
    {
        /*
         * if there is a packet buffer and this is the first update, populate
         * the packets
         */
        if (hasPacketBuffer() && rcFlag == numFunctions)
        {
            populateAll();
        }

        // decrement to move on to next packet
        rcFlag--;

        /*
         * Get the next CAN packet. If there's a buffer, take from the buffer.
         * Otherwise populate the packet now.
         */
        CANPacket *pPtr;
        if (hasPacketBuffer())
        {
            // take a packet from the buffer
            pPtr = &packets[rcFlag];
        }
        else
        {
            // populate the next rollcall packet
            CANPacket p;
            functions[rcFlag](&p);
            pPtr = &p;
        }

        // send the packet
        // shouldn't fail because canTxCheck() says there is room in the buffer
        canSendPacket(pPtr);
    }

    return rollcallQueueLength();
}

uint8_t rollcallQueueLength()
{
    return rcFlag;
}
