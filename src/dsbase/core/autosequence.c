#include "autosequence.h"
#include "MET.h"
#include "../ssmods/ss_EPS_Dist/eps_dist.h"

sequenceEvent *events;
uint8_t eventSize;

uint32_t recentMET;
uint8_t metUpdatedFlag = 0;

void seqUpdateMET(uint32_t met)
{
    recentMET = met;
    metUpdatedFlag = 1;
}

void seqInit(sequenceEvent *initEvents, uint8_t size)
{
	events = initEvents;
	eventSize = size;
}

uint8_t seqAddEvent(sequenceEvent e)
{
    uint8_t i;
    for(i = 0; i < eventSize; i++)
    {
        if(!events[i].time) //time is not in use
        {
            events[i] = e;
            return 0;
        }
    }
    return 1;
}

void checkSequence(uint8_t enabled)
{
    if(enabled && metUpdatedFlag)
    {
        uint8_t i;
        for(i = 0; i < eventSize; i++)
        {
            if(events[i].time && events[i].time <= recentMET)
            {
                if(events[i].sendFlag) //actually send
                {
                    while(canTxCheck() == CAN_TX_BUSY);
                    canSendPacket(&events[i].pkt);
                }
                else
                {
                	can_packet_rx_callback(&events[i].pkt);
                }
                events[i].time = 0;
            }
        }
        metUpdatedFlag = 0;
    }
}

void seqRemoveEventAtIndex(uint8_t index)
{
    events[index].time = 0;
}

void seqRemoveEventsWithID(uint32_t canPktId)
{
    uint8_t i;
    for(i = 0; i < eventSize; i++)
        if (events[i].pkt.id == canPktId)
            seqRemoveEventAtIndex(i);
}

uint64_t seqGetIndicesForId(uint32_t canPktId)
{
    uint64_t indices = 0;
    uint8_t i, currCount = 0;
    for(i = 0; i < eventSize; i++) //populate "array" with indices
        if(events[i].pkt.id == canPktId)
            indices |= (i << (currCount++ * 8));

    while(currCount < 8) //fill in the remaining elements with 0xFF
        indices |= (0xFF << (currCount++ * 8));

    return indices;
}

uint32_t seqGetMETForIndex(uint8_t index)
{
    return events[index].time;
}
