//////////////////////////////////////////////////////////////////////
//DO NOT MODIFY THIS FILE DIRECTLY
//Instead, change DubSat1/teams/cdh/CAN/Experiments/CANModelTemplate.*
//////////////////////////////////////////////////////////////////////

/*
* CANModel
*
*  Created on: Oct 17, 2018
*      Author: Nathan Wacker
*/

#include <CAN_Model.h>
#include <msp430.h> 
#include <stdint.h>

#include "bsp/bsp.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"

#define VERSION_NUMBER 1

uint32_t distArray[] = {303629019, 304677484, 304677475, 303628882, 303628875, 303628873, 303628872, 303628871, 303628869, 303628868, 303628883, 303628880, 303628879, 303628877, 303628878, 303628881, 303628876, 303628874, 304677446, 305136195, };
uint32_t com2Array[] = {};
uint32_t rahsArray[] = {};
uint32_t bdotArray[] = {304677597, 304677491, 304677490, 304677489, 303628912, 303628892, 304677485, 304677479, 304677476, 304677470, 303628826, 303628825, 303628807, 303628806, 304677391, 304677390, 304677389, 304677388, 304677387, };
uint32_t estimArray[] = {304677483, 303628906, 304677474, 303628897, 303628889, 303628888, 303628887, 303628886, 303628885, 303628884, 303628862, 303628858, 303628859, 303628860, 303628856, 303628857, 303628855, 303628854, 303628866, 303628863, 303628864, 303628865, 303628861, 303628853, 303628852, 303628851, 303628850, 303628849, 303628848, 303628847, 303628845, 303628846, 303628844, };
uint32_t epsArray[] = {305726258, 304677486, 304677480, 304677477, 304677471, 303628890, 303628824, 303628823, 303628822, 303628821, 303628820, 303628819, 303628818, 303628817, 303628816, 304677381, 304677380, 303628803, 304677378, 304677377, 305725952, };
uint32_t pptArray[] = {304677478, 304677469, 303628810, 303628809, 303628808, };

FILE_STATIC uint32_t * rollcallIDs[] =
{
    distArray, com2Array, rahsArray, bdotArray, estimArray, epsArray, pptArray, 
};

uint16_t arraySizes[] = {sizeof(distArray) / sizeof(uint32_t), sizeof(com2Array) / sizeof(uint32_t), sizeof(rahsArray) / sizeof(uint32_t), sizeof(bdotArray) / sizeof(uint32_t), sizeof(estimArray) / sizeof(uint32_t), sizeof(epsArray) / sizeof(uint32_t), sizeof(pptArray) / sizeof(uint32_t), };
volatile TIMER_LENGTH realtimeCounter = 0;
uint8_t rcSendFlag = 0;
#pragma PERSISTENT(initSequenceFlag)
char initSequenceFlag = 1;

uint8_t sendCANPacket(uint32_t id)
{
	CANPacket pkt;
    pkt.id = id;
    pkt.length = 8;
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
            sendCANPacket(rollcallIDs[i][j]);
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

        if(checkTimeElapsed(last_estim_sun_unit_z_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_z_time = realtimeCounter;
            while(sendCANPacket(302449332));
        }
        if(checkTimeElapsed(last_estim_sun_unit_y_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_y_time = realtimeCounter;
            while(sendCANPacket(302449331));
        }
        if(checkTimeElapsed(last_estim_sun_unit_x_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_x_time = realtimeCounter;
            while(sendCANPacket(302449330));
        }
        if(checkTimeElapsed(last_estim_mag_unit_z_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_z_time = realtimeCounter;
            while(sendCANPacket(302449335));
        }
        if(checkTimeElapsed(last_estim_mag_unit_y_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_y_time = realtimeCounter;
            while(sendCANPacket(302449334));
        }
        if(checkTimeElapsed(last_estim_mag_unit_x_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_x_time = realtimeCounter;
            while(sendCANPacket(302449333));
        }
        if(checkTimeElapsed(last_estim_state_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_state_time = realtimeCounter;
            while(sendCANPacket(302449336));
        }
        if(checkTimeElapsed(last_com2_state_time, 100) && PD_IN_COM2 & PD_BIT_COM2)
        {
            last_com2_state_time = realtimeCounter;
            while(sendCANPacket(303563248));
        }
        if(checkTimeElapsed(last_cmd_mtq_fsw_time, 1) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_cmd_mtq_fsw_time = realtimeCounter;
            while(sendCANPacket(302252067));
        }
        if(checkTimeElapsed(last_mtq_ack_time, 20) && PD_IN_BDOT & PD_BIT_BDOT)
        {
            last_mtq_ack_time = realtimeCounter;
            while(sendCANPacket(303497264));
        }
        if(checkTimeElapsed(last_cmd_mtq_bdot_time, 2) && PD_IN_BDOT & PD_BIT_BDOT)
        {
            last_cmd_mtq_bdot_time = realtimeCounter;
            while(sendCANPacket(303497249));
        }
        
    }
}