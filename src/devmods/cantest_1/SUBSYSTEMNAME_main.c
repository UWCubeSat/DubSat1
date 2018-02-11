#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/can.h"
#include "interfaces/canwrap.h"

#define BOARD_NUM (0x05)

#define LED_BLUE (0x01)
#define LED_RED (0x04)
#define LED_YELLOW (0x02)

/*
 * main.c
 */
// Send back the same reply

void newCB(CANPacket *packet){

}

uint32_t testBICount;

void testBIGetPacket(CANPacket *packet){
    testBICount++;
}

void testBIInit(){
    testBICount = 0;
    canWrapInit();
    setCANPacketRxCallback(testBIGetPacket);
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    testBIInit();

    P1OUT &= 0x00;               // Shut down everything
    P1DIR &= 0x00;
    P1DIR |= BIT0 + BIT6;       // P1.0 and P1.6 pins output the rest are input
    P1REN |= BIT3;                 // Enable internal pull-up/down resistors
    P1OUT |= BIT3;                 //Select pull-up mode for P1.3
    P1IE |= BIT3;                    // P1.3 interrupt enabled
    P1IES |= BIT3;                  // P1.3 Hi/lo edge
    P1IFG &= ~BIT3;               // P1.3 IFG cleared

    P3DIR |= 0x20;
    P3OUT |= 0x20;




#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}


// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

   CANPacket p = {0};
   p.length = 8;
   p.data[4] = (uint8_t) testBICount;
   p.data[3] = (uint8_t) (testBICount >> 8);
   p.data[2] = (uint8_t) (testBICount >> 16);
   p.data[1] = (uint8_t) (testBICount >> 24);
   p.data[0] = 0x69;
   canSendPacket(&p);

   testBICount = 0;
   P1IFG &=~BIT3;                        // P1.3 IFG cleared
}
