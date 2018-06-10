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

void pause(uint32_t i){
    while(i > 0){
        __delay_cycles(1);
    }
}

void rxCb(CANPacket *p){
    // PPT Single Fire
    if(p -> id == 303300864){
        CANPacket p = {0};
        canSendPacket(&p);
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

    uint32_t delay = 0;

    while(1){

        if (P1IN & BIT7){
            delay = 0;
        }
        else {
            delay = 200000;
        }
        if (P1IN & BIT6){
            PJOUT |= BIT0;
            CANPacket cmd_com2_run_packet = {0};
            cmd_com2_run cmd_com2_run_info = {0};
            encodecmd_com2_run(&cmd_com2_run_info, &cmd_com2_run_packet);
            canSendPacket(&cmd_com2_run_packet);
        }
        else {
            PJOUT &= ~BIT0;
        }
        pause(delay);
        if (P3IN & BIT7){
            PJOUT |= BIT1;
            CANPacket cmd_gen_rst_packet = {0};
            cmd_gen_rst cmd_gen_rst_info = {0};
            encodecmd_gen_rst(&cmd_gen_rst_info, &cmd_gen_rst_packet);
            canSendPacket(&cmd_gen_rst_packet);
        }
        else {
            PJOUT &= ~BIT1;
        }
        pause(delay);
        if (P3IN & BIT6){
            PJOUT |= BIT2;
            CANPacket com2_state_packet = {0};
            com2_state com2_state_info = {0};
            encodecom2_state(&com2_state_info, &com2_state_packet);
            canSendPacket(&com2_state_packet);
        }
        else {
            PJOUT &= ~BIT2;
        }
        pause(delay);
        if (P2IN & BIT2){
            CANPacket mpc_vp_packet = {0};
            mpc_vp mpc_vp_info = {0};
            encodempc_vp(&mpc_vp_info, &mpc_vp_packet);
            canSendPacket(&mpc_vp_packet);
        }
        pause(delay);
    }



#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
