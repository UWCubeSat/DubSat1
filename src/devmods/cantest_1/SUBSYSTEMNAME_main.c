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
void blinkCallback(CANPacket *p){
    if(p->bufferNum){
        PJDIR |= 0x02;
        PJOUT ^= 0x02;
    }
    else{
        PJDIR |= 0x01;
        PJOUT ^= 0x01;
    }

}
void testFilterCB(CANPacket *packet){
    PJDIR |= 0x02;
    PJOUT ^= 0x02;
}

uint32_t testBICount;

void testBIGetPacket(CANPacket *packet){
    testBICount++;
}

void testBIInit(){
    testBICount = 0;
    canWrapInit();
    setCANPacketRxCallback(testBIGetPacket);

    P1OUT &= 0x00;               // Shut down everything
    P1DIR &= 0x00;
    P1DIR |= BIT0 + BIT6;       // P1.0 and P1.6 pins output the rest are input
    P1REN |= BIT3;                 // Enable internal pull-up/down resistors
    P1OUT |= BIT3;                 //Select pull-up mode for P1.3
    P1IE |= BIT3;                    // P1.3 interrupt enabled
    P1IES |= BIT3;                  // P1.3 Hi/lo edge
    P1IFG &= ~BIT3;               // P1.3 IFG cleared

}

void testTimerInit(){
    canWrapInit();
    // Init the timer
    FREQ_TIMER(CCTL0) = CM__RISING | CCIS__CCIB | SCS | CAP | CCIE;
    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__ACLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | TAIE;


}

void testFilter(){
    canWrapInitWithFilter();
    setCANPacketRxCallback(blinkCallback);
//    FREQ_TIMER(CCTL0) = CM__RISING | CCIS__CCIB | SCS | CAP | CCIE;
//    FREQ_TIMER(CTL) = FREQ_ROOT_TIMER(SSEL__ACLK) | MC__CONTINUOUS | FREQ_ROOT_TIMER(CLR) | TAIE;
}
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    setCANPacketRxCallback(testBIGetPacket);
//    testBIInit();
//    testTimerInit();
//    canWrapInit();
//     testFilter();
    canWrapInit();

    PJDIR |= 0x07;
    while(1){
        uint8_t a = 69;
        uint8_t b = 96;
        aggVec_i yolo;
        aggVec_init(&yolo);
        aggVec_i_push(&yolo, a);
        aggVec_i_push(&yolo, b);
        float avg = aggVec_i_avg_f(&yolo);
        PJOUT |= (uint8_t) avg & 0x07;
//        CANPacket p = {0};
//        p.id = 0x01;
//        p.length = 0;
//        canSendPacket(&p);
//        p.id = 0x02;
//        canSendPacket(&p);
//        p.id = 0x03;
//        canSendPacket(&p);
//        p.id = 0x04;
//        PJOUT ^= canSendPacket(&p);
//        p.id = 0x05;
//        PJOUT ^= canSendPacket(&p);
//        p.id = 0x06;
//        PJOUT ^= canSendPacket(&p);
//        __delay_cycles(100000);
//        CANPacket p = {0};
//        p.length = 7;
//        canSendPacket(&p);
    }



#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
void testBIPoll(){
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

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    // testBIPoll();
}

// Timer4_A1 (CCR=1..n) TA Interrupt Handler
#pragma vector = TIMER4_A1_VECTOR
__interrupt void Timer4_A1_ISR(void)
{
    switch (__even_in_range(TA4IV, TAIV__TAIFG))
    {
        case TAIV__TAIFG:                   // TA4.0 overflow
            // Deal with bogus initial overflow flag that pops on startup
            PJDIR |= 0x01;
            PJOUT ^= 0x01;
            overflowCount++;
            // ONCE EVERY TWO SECONDS
            CANPacket p = {0};
            p.id = 0xBADA55;
            p.data[0] = overflowCount;
            p.length = 1;
            canSendPacket(&p);
            break;
        default:
            break;
    }
}
