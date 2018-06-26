#include "autosequence.h"
#include "MET.h"
#include "../ssmods/ss_EPS_Dist/eps_dist.h"

sequenceEvent events[NUM_POWER_DOMAINS]; //TODO: increase this as necessary
//#pragma PERSISTENT(events)

uint32_t recentMET;
uint8_t metUpdatedFlag = 0;

void populateDefaultValues()
{
	CANPacket pkt;
	uint8_t i;
	for(i = 0; i < NUM_POWER_DOMAINS; i++)
	{
		gcmd_dist_set_pd_state cmd = {0};
		switch(i)
		{
			case 0:
				cmd.gcmd_dist_set_pd_state_bdot = 1;
				break;
			case 1:
				cmd.gcmd_dist_set_pd_state_com1 = 1;
				break;
			case 2:
				cmd.gcmd_dist_set_pd_state_com2 = 1;
				break;
			case 3:
				cmd.gcmd_dist_set_pd_state_eps = 1;
				break;
			case 4:
				cmd.gcmd_dist_set_pd_state_estim = 1;
				break;
			case 5:
				cmd.gcmd_dist_set_pd_state_ppt = 1;
				break;
			case 6:
				cmd.gcmd_dist_set_pd_state_rahs = 1;
				break;
			case 7:
				cmd.gcmd_dist_set_pd_state_wheels = 1;
				break;
		}
		encodegcmd_dist_set_pd_state(&cmd, &pkt);
		events[i].pkt = pkt;
		events[i].sendFlag = 0;
		events[i].time = i + 1; //sequentially activated
	}
}

void seqUpdateMET(uint32_t met)
{
    recentMET = met;
    metUpdatedFlag = 1;
}

void seqInit()
{
    populateDefaultValues();
}

uint8_t seqAddEvent(sequenceEvent e)
{
    uint8_t i;
    uint8_t numElem = sizeof(events) / sizeof(sequenceEvent);
    for(i = 0; i < numElem; i++)
    {
        if(!events[i].time) //time is not in use
        {
            events[i] = e;
            return 0;
        }
    }
    return 1;
}

void checkSequence()
{
    if(metUpdatedFlag)
    {
        uint8_t i;
        for(i = sizeof(events) / sizeof(sequenceEvent); i; i--) //use (i - 1)
        {
            if(events[i-1].time && events[i-1].time <= recentMET)
            {
                if(events[i-1].sendFlag) //actually send
                {
                    while(canTxCheck() == CAN_TX_BUSY);
                    canSendPacket(&events[i-1].pkt);
                }
                else
                {
                	can_packet_rx_callback(&events[i-1].pkt);
                }
                events[i-1].time = 0;
            }
        }
        metUpdatedFlag = 0;
    }
}

void removeEventAtIndex(uint8_t index)
{
    events[index].time = 0;
}

void removeEventsWithID(uint32_t canPktId)
{
    uint8_t i;
    for(i = sizeof(events) / sizeof(sequenceEvent) - 1; i + 1; i--)
        if (events[i].pkt.id == canPktId)
            removeEventAtIndex(i);
}
