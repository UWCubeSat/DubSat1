//////////////////////////////////////////////////////////////////////
//DO NOT MODIFY THIS FILE DIRECTLY
//Instead, change DubSat1/teams/cdh/CAN/Experiments/CANModelTemplate.*
//////////////////////////////////////////////////////////////////////

/*
* CANModel
*
*  Created on: Dec 16, 2018
*      Author: Nathan Wacker
*/

#include <CAN_Model.h>
#include <msp430.h> 
#include <stdint.h>

#include "bsp/bsp.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"

#define VERSION_NUMBER 2

uint32_t distArray[] = {307823323, 308871788, 308871779, 307823186, 307823179, 307823177, 307823176, 307823175, 307823173, 307823172, 307823187, 307823184, 307823183, 307823181, 307823182, 307823185, 307823180, 307823178, 308871750, 309330499, };
uint32_t com2Array[] = {};
uint32_t rahsArray[] = {};
uint32_t bdotArray[] = {308871901, 308871795, 308871794, 308871793, 307823216, 307823196, 308871789, 308871783, 308871780, 308871774, 307823130, 307823129, 307823111, 307823110, 308871695, 308871694, 308871693, 308871692, };
uint32_t estimArray[] = {308871787, 307823210, 308871778, 307823201, 307823193, 307823192, 307823191, 307823190, 307823189, 307823188, 307823166, 307823162, 307823163, 307823164, 307823160, 307823161, 307823159, 307823158, 307823170, 307823167, 307823168, 307823169, 307823165, 307823157, 307823156, 307823155, 307823154, 307823153, 307823152, 307823151, 307823149, 307823150, 307823148, };
uint32_t epsArray[] = {309920562, 308871790, 308871784, 308871781, 308871775, 307823194, 307823128, 307823127, 307823126, 307823125, 307823124, 307823123, 307823122, 307823121, 307823120, 308871685, 308871684, 307823107, 308871682, 308871681, 309920256, };
uint32_t pptArray[] = {308871782, 308871773, 307823114, 307823113, 307823112, };

FILE_STATIC uint32_t * rollcallIDs[] =
{
    distArray, com2Array, rahsArray, bdotArray, estimArray, epsArray, pptArray, 
};

uint8_t distLens[] = {7, 1, 8, 7, 6, 6, 7, 6, 6, 6, 6, 8, 6, 6, 7, 6, 7, 7, 7, 6, };
uint8_t com2Lens[] = {};
uint8_t rahsLens[] = {};
uint8_t bdotLens[] = {6, 8, 8, 8, 8, 6, 1, 1, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, };
uint8_t estimLens[] = {8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, };
uint8_t epsLens[] = {6, 1, 8, 8, 8, 6, 8, 7, 8, 8, 8, 8, 8, 8, 6, 7, 8, 8, 7, 6, 3, };
uint8_t pptLens[] = {2, 8, 6, 7, 7, };

FILE_STATIC uint8_t * rollcallLengths[] =
{
	distLens, com2Lens, rahsLens, bdotLens, estimLens, epsLens, pptLens, 
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

        if(checkTimeElapsed(last_estim_sun_unit_z_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_z_time = realtimeCounter;
            while(sendCANPacket(302449332, 8));
        }
        if(checkTimeElapsed(last_estim_sun_unit_y_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_y_time = realtimeCounter;
            while(sendCANPacket(302449331, 8));
        }
        if(checkTimeElapsed(last_estim_sun_unit_x_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_sun_unit_x_time = realtimeCounter;
            while(sendCANPacket(302449330, 8));
        }
        if(checkTimeElapsed(last_estim_mag_unit_z_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_z_time = realtimeCounter;
            while(sendCANPacket(302449335, 8));
        }
        if(checkTimeElapsed(last_estim_mag_unit_y_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_y_time = realtimeCounter;
            while(sendCANPacket(302449334, 8));
        }
        if(checkTimeElapsed(last_estim_mag_unit_x_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_mag_unit_x_time = realtimeCounter;
            while(sendCANPacket(302449333, 8));
        }
        if(checkTimeElapsed(last_estim_state_time, 28) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_estim_state_time = realtimeCounter;
            while(sendCANPacket(302449336, 1));
        }
        if(checkTimeElapsed(last_com2_state_time, 100) && PD_IN_COM2 & PD_BIT_COM2)
        {
            last_com2_state_time = realtimeCounter;
            while(sendCANPacket(307757552, 8));
        }
        if(checkTimeElapsed(last_cmd_mtq_fsw_time, 1) && PD_IN_ESTIM & PD_BIT_ESTIM)
        {
            last_cmd_mtq_fsw_time = realtimeCounter;
            while(sendCANPacket(302252067, 4));
        }
        if(checkTimeElapsed(last_mtq_ack_time, 20) && PD_IN_BDOT & PD_BIT_BDOT)
        {
            last_mtq_ack_time = realtimeCounter;
            while(sendCANPacket(307691568, 8));
        }
        if(checkTimeElapsed(last_cmd_mtq_bdot_time, 2) && PD_IN_BDOT & PD_BIT_BDOT)
        {
            last_cmd_mtq_bdot_time = realtimeCounter;
            while(sendCANPacket(307691553, 3));
        }
        
    }
}