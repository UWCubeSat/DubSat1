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

////////////////////////////////////////////////////////////////////////

uint8_t seqAddEvent(sequenceEvent e); //returns 0 for successful adding
void checkSequence(); //need to call this frequently to send CAN cmds
void removeEventAtIndex(uint8_t index); //removes the event at a specific index (array is not sorted, so need to know the index)
void removeEventsWithID(uint32_t canPktId); //removes all events that send a CAN packet with the specified Id

//these should be called from MET.c
void seqUpdateMET(uint32_t met);
void seqInit(sequenceEvent *initEvents, uint8_t size);

#endif
