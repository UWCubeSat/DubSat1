#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/can.h"
#include "interfaces/canwrap.h"
#include "core/agglib.h"


// Directives for timer stuff.
#include "core/timers.h"
#define FREQ_TIMER(reg)         TA4##reg
#define FREQ_ROOT_TIMER(bits)   TA##bits
FILE_STATIC uint32_t overflowCount = 0;


#define BOARD_NUM (0x05)

#define LED_BLUE (0x01)
#define LED_RED (0x04)
#define LED_YELLOW (0x02)

/*
 * main.c
 */
// Send back the same reply

void pause(uint64_t i){
    uint64_t j=0;
    for(j=0; j <i; j++){
        __delay_cycles(1);
    }
}
void reverseArrah(uint8_t arr[], uint8_t start, uint8_t end) {
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArrah(arr, start+1, end-1);
}


void rxCb(CANPacket *p){
    // PPT Single Fire
    if(p -> id == 303300864 ){
        CANPacket j = {0};
        j.id =  1234567;
        uint64_t data =3400535326 ;
        uint64_t *thePointer = (uint64_t *) (j.data);
        j.length=4;
        *thePointer = data;
        reverseArrah((j.data), 0, 3);
        canSendPacket(&j);
    }
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    setCANPacketRxCallback(rxCb);
    canWrapInit();

    P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    P3DIR |= BIT0 | BIT1 | BIT2 | BIT3;

    P1OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    P3OUT |= BIT0 | BIT1 | BIT2 | BIT3;

    P1DIR &= ~(BIT7 | BIT6);
    P3DIR &= ~(BIT7 | BIT6);
    P2DIR &= ~(BIT2 | BIT6);

    P1OUT &= ~(BIT7 | BIT6);
    P3OUT &= ~(BIT7 | BIT6);
    P2OUT &= ~(BIT2 | BIT6);

    P1REN |= BIT7 | BIT6;
    P3REN |= BIT7 | BIT6;
    P2REN |= BIT2 | BIT6;

    PJDIR |= BIT0 | BIT1 | BIT2;

    uint64_t delay = 30000;

    while(1){

        if (P1IN & BIT7){
            delay = 20;
        }
        else {
            delay = 60000;
        }
        if (P1IN & BIT6){
            //GND_NONE
            PJOUT |= BIT0;
            CANPacket cmd_com2_run_packet = {0};
            cmd_com2_run cmd_com2_run_info = {0};
            encodecmd_com2_run(&cmd_com2_run_info, &cmd_com2_run_packet);
            uint64_t data = 3735928559;
            uint64_t *thePointer = (uint64_t *) (cmd_com2_run_packet.data);
            cmd_com2_run_packet.length=4;
            *thePointer = data;
            reverseArrah((cmd_com2_run_packet.data), 0, 3);
            canSendPacket(&cmd_com2_run_packet);
        }
        else {
            PJOUT &= ~BIT0;
        }
        pause(delay);
        if (P3IN & BIT7){
            //GND_REALTIME
            PJOUT |= BIT1;
            CANPacket cmd_gen_rst_packet = {0};
            cmd_gen_rst cmd_gen_rst_info = {0};
            encodecmd_gen_rst(&cmd_gen_rst_info, &cmd_gen_rst_packet);
            uint64_t data = 789514;
            uint64_t *thePointer = (uint64_t *) (cmd_gen_rst_packet.data);
            cmd_gen_rst_packet.length=3;
            *thePointer = data;
            reverseArrah((cmd_gen_rst_packet.data), 0, 2);
            canSendPacket(&cmd_gen_rst_packet);
        }
        else {
            PJOUT &= ~BIT1;
        }
        pause(delay);
        if (P3IN & BIT6){
            //GND_HEALTH
            PJOUT |= BIT2;
            CANPacket com2_state_packet = {0};
            com2_state com2_state_info = {0};
            encodecom2_state(&com2_state_info, &com2_state_packet);
            uint64_t data = 900517141665;
            uint64_t *thePointer = (uint64_t *) (com2_state_packet.data);
            com2_state_packet.length=5;
            *thePointer = data;
            reverseArrah((com2_state_packet.data), 0, 4);
            canSendPacket(&com2_state_packet);
        }
        else {
            PJOUT &= ~BIT2;
        }
        pause(delay);
        if (P2IN & BIT2){
            //GND_WOD
            CANPacket mpc_vp_packet = {0};
            mpc_vp mpc_vp_info = {0};
            encodempc_vp(&mpc_vp_info, &mpc_vp_packet);
            uint64_t data = 3201966622;
            uint64_t *thePointer = (uint64_t *) (mpc_vp_packet.data);
            mpc_vp_packet.length=4;
            *thePointer = data;
            reverseArrah((mpc_vp_packet.data), 0, 3);
            canSendPacket(&mpc_vp_packet);
        }
        pause(delay);
    }



#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
