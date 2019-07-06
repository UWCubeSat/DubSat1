#ifndef AUTOSEQUENCE_H
#define AUTOSEQUENCE_H

#include <stdint.h>
#include "../interfaces/canwrap.h"
#include "MET.h"

typedef struct {
    CANPacket pkt;
    uint8_t sendFlag; //0 is self call, 1 is send CAN
    uint32_t time; //seconds
} sequenceEvent;

#define WK_TO_SEC 604800

////////////////////////////////////////////////////////////////////////

uint8_t seqAddEvent(sequenceEvent e); //returns 0 for successful adding
void checkSequence(uint8_t enabled); //need to call this frequently to send CAN cmds
void seqRemoveEventAtIndex(uint8_t index); //removes the event at a specific index (array is not sorted, so need to know the index)
void seqRemoveEventsWithID(uint32_t canPktId); //removes all events that send a CAN packet with the specified Id

void seqUpdateMET(uint32_t met); //needs to be called frequently
void seqInit(sequenceEvent *initEvents, uint8_t size);

uint64_t seqGetIndicesForId(uint32_t canPktId); //returns a concatenated array of uint8_t indices, with 0xFF if they one is empty
uint32_t seqGetMETForIndex(uint8_t index); //returns the time for the seqEvent at the specified index

#endif
