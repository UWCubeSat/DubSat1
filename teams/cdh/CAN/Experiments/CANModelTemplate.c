#include <CAN_Model.h>
#include <msp430.h> 
#include <stdint.h>

#include "bsp/bsp.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"

#define VERSION_NUMBER /*[version number here]*/

/*[Function ID arrays here]*/
FILE_STATIC uint32_t * rollcallIDs[] =
{
    /*[Populate function IDs here]*/
};

/*[Packet DLC arrays here]*/
FILE_STATIC uint8_t * rollcallLengths[] =
{
	/*[Populate packet DLCs here]*/
};

uint16_t arraySizes[] = {sizeof(distArray) / sizeof(uint32_t), sizeof(com2Array) / sizeof(uint32_t), sizeof(rahsArray) / sizeof(uint32_t), sizeof(bdotArray) / sizeof(uint32_t), sizeof(estimArray) / sizeof(uint32_t), sizeof(epsArray) / sizeof(uint32_t), sizeof(pptArray) / sizeof(uint32_t), };
volatile TIMER_LENGTH realtimeCounter = 0;
uint8_t rcSendFlag = 0;
#pragma PERSISTENT(initSequenceFlag)
char initSequenceFlag = 1;

uint8_t sendCANPacket(uint32_t id, uint8_t length)
{
	CANPacket pkt;
    pkt.id = id;
    pkt.length = length;
    int i;
    for(i = 0; i < 4; i++)
    {
        pkt.data[3 - i] = pkt.data[7 - i] = (id >> 8 * i);
    }
    return canSendPacket(&pkt);
}

uint8_t i = 0; //power domain
uint8_t j = 0; //current index
void can_packet_rx_callback(CANPacket *packet)
{
		if(packet->id == CAN_ID_CMD_ROLLCALL)
    {
		rcSendFlag = 1;
        rcSendFlag |= !!(PD_IN_COM2 & PD_BIT_COM2) << PD_COM2;
        rcSendFlag |= !!(PD_IN_BDOT & PD_BIT_BDOT) << PD_BDOT;
        rcSendFlag |= !!(PD_IN_ESTIM & PD_BIT_ESTIM) << PD_ESTIM;
        rcSendFlag |= !!(PD_IN_EPS & PD_BIT_EPS) << PD_EPS;
        rcSendFlag |= !!(PD_IN_PPT & PD_BIT_PPT) << PD_PPT;

        lastLED1Time = realtimeCounter;
        LED_1_FREQ = 7;
        LED_OUT |= LED1_BIT;

        //reset rcResponse counters
        i = 0;
        j = 0;
    }
}

void incrementRealtimeCounter()
{
    realtimeCounter++;
}

void sendRCResponse()
{
    while(rcSendFlag && canTxCheck() != CAN_TX_BUSY)
    {
        while(!(rcSendFlag & (BIT0 << i)))
            i++;

        if(j < arraySizes[i])
        {
            sendCANPacket(rollcallIDs[i][j], rollcallLengths[i][j]);
			j++;
        }
        else
        {
            rcSendFlag &= ~(BIT0 << i);
            j = 0;
        }
    }
}

void playInitialLEDSequence() {
    LED_OUT |= LED0_BIT;
    __delay_cycles(SEC * 0.75);
    LED_OUT &= ~LED0_BIT;
    LED_OUT |= LED1_BIT;
    __delay_cycles(SEC * 0.75);
    LED_OUT &= ~LED1_BIT;
    LED_OUT |= LED2_BIT;
    __delay_cycles(SEC * 0.75);
    LED_OUT &= ~LED2_BIT;
    LED_OUT |= (VERSION_NUMBER & 0x7);
    __delay_cycles(SEC * 7.5);
    LED_OUT &= ~(VERSION_NUMBER & 0x7);
    __delay_cycles(SEC * 0.75);
}

int checkTimeElapsed(TIMER_LENGTH lastTime, int packetFreq)
{
    return realtimeCounter - packetFreq >= lastTime;
}

void bitInit()
{
	LED_DIR |= LED0_BIT | LED1_BIT | LED2_BIT;
	PD_DIR_COM2 &= ~PD_BIT_COM2;
	PD_DIR_RAHS &= ~PD_BIT_RAHS;
    PD_DIR_BDOT &= ~PD_BIT_BDOT;
	PD_DIR_ESTIM &= ~PD_BIT_ESTIM;
	PD_DIR_EPS &= ~PD_BIT_EPS;
	PD_DIR_PPT &= ~PD_BIT_PPT;
	
	PD_OUT_COM2 &= ~PD_BIT_COM2;
	PD_OUT_RAHS &= ~PD_BIT_RAHS;
	PD_OUT_BDOT &= ~PD_BIT_BDOT;
	PD_OUT_ESTIM &= ~PD_BIT_ESTIM;
	PD_OUT_EPS &= ~PD_BIT_EPS;
	PD_OUT_PPT &= ~PD_BIT_PPT;
}

/*
 * main.c
 */
int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);

    canWrapInitWithFilter();
    setCANPacketRxCallback(can_packet_rx_callback);

    initializeTimer();
    startCallback(timerCallbackInitializer(&incrementRealtimeCounter, 100000)); //increments every 100 ms

    bitInit();

    if(initSequenceFlag)
    {
        playInitialLEDSequence();
        initSequenceFlag = 0;
    }

    while (1)
    {
        //Rollcall Response:
        sendRCResponse();

        //Realtime stuff:
        if(checkTimeElapsed(lastLED0Time, LED_0_FREQ))
        {
            lastLED0Time = realtimeCounter;
            LED_OUT ^= LED0_BIT;
        }

        if(checkTimeElapsed(lastLED1Time, LED_1_FREQ))
        {
            LED_1_FREQ = 0xff;
            lastLED1Time = 0;
            LED_OUT &= ~LED1_BIT;
        }

        /*[Realtime Update Methods Here]*/
    }
}