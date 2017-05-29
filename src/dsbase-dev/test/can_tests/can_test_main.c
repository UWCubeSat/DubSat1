#include <msp430.h>
#include "../../core/can.h"
#include "../../core/spi.h"

/*
 * main.c
 */

int transmitNum;

// Send back the same reply
void SendbackSameMessage(uint8_t length, uint8_t* data){
    uint8_t tech[5] = {0,0,0,0,0x08};
    uint8_t msg[8];

    uint8_t i;
    for( i = 0; i < length; i++) {
        msg[i] = data[i];
    }

    canSend(0,tech, msg);
}

// Test Sending over CAN
void testSend() {
    // TXB0SIDH pg 33
    // TXB0SIDL pg 34
    // TXB0EID8 pg 34
    // TXB0EID0 pg 34
    // TXB0DLC  pg 35
    uint8_t tech[5] = {0,0,0,0,0x08};

    // TXB0D0 -> TXB0D7, data to send
    uint8_t msg[8];
    uint8_t msgt[8];
    int i;

    msg[0] = 0x48;
    msg[1] = 0x45;
    msg[2] = 0x4C;
    msg[3] = 0x4C;
    msg[4] = 0x4F;
    msg[5] = 0x57;
    msg[6] = 0x4F;
    msg[7] = 0x52;
    msgt[0] = 0x4C;
    msgt[1] = 0x44;
    msgt[2] = 0xED;
    msgt[3] = 0xA0;
    msgt[4] = 0xBD;
    msgt[5] = 0xED;
    msgt[6] = 0xB8;
    msgt[7] = 0x83;

    canSend(0,tech, msg);

    // Stop gap measure to wait for first message to finish
    for(i=10000;i>0;i--){

    }
    canSend(0,tech, msgt);
    // Stop gap measure to wait for second message to finish
    for(i=10000;i>0;i--){

    }
}

// Send over CAN and Read Status Register
uint8_t testReadStatus() {
    uint8_t status = 0x0;
    readStatus(&status);

    // CANINTF = Interrupt flag
        // RXnIF = Receive buffer n full interrupt flag bit
            // Must be cleared by MCU to reset interrupt
        // TxnIF = Transmit Buffer n Empty Interrupt Flag bit
    // TXBnCTRL = Transmit Buffer n Control Register
        // TXREQ = Msg Transmit Req bit
           // 1 = buffer is currently pending transmission
           // 0 = buffer not pending transmission

    // Bits
    // 0 RX0IF (CANINTF) pg 50
    // 1 RX1IF (CANINTFL)
    // 2 TXREQ (TXB0CNTRL) pg 32
    // 3 TX0IF (CANINTF)
    // 4 TXREQ (TXB1CNTRL)
    // 5 TX1IF (CANINTF)
    // 6 TXREQ (TXB2CNTRL)
    // 7 TX2IF (CANINTF)
    uint8_t rx0if, rx1if, txreq0, tx0if, txreq1, tx1if,txreq2,tx2if;
    printf("Status: %x", status);
    /*
    rx0if = status & 0x01;
    rx1if = status & 1<<1;
    txreq0 = status & 1<<2;
    tx0if = status & 1<<3;
    txreq1 = status & 1<<4;
    tx1if = status & 1<<5;
    txreq2 =status & 1<<6;
    tx2if = status & 1<<7;

    printf("RX0IF:%x ",rx0if);
    printf("RX1IF:%x ",rx1if);
    printf("TXREQ for TXB0CNTRL:%x ",txreq0);
    printf("TX0IF:%x ",tx0if);
    printf("TXREQ for TXB1CNTRL:%x ",txreq1);
    printf("TX1IF:%x ",tx1if);
    printf("TXREQ for TXB2CNTRL:%x ",txreq2);
    printf("TX2IF:%x \n",tx2if);
*/
    int i;
    for(i = 0; i < 10; ++i) {
        testSend();
        readStatus(&status);

        if (status != 0) {
            printf("Something changed \n");
        }
    }

    return 0;
}

// Test read and write to a register
uint8_t writeAndReadAddr() {
    uint8_t addr = 0x2C;
    uint8_t value = 0xEF;

    setRegister(addr, value);

    uint8_t rvalue = 0;
    readRegister(addr, &rvalue);
    if (value == rvalue) {
        return 1;
    }
    return 0;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // For the GPIO pins
    canInit();

    //testReadStatus();


    transmitNum = 0;
    //Set in Interrupt
    // TODO: Enable individual interrupt for Port 1
    //       pg 26 in Interrupts Workshop
    P1IE |= 0x10; // P1.4 interrupt enabled
    P1IES |= 0x10; // P1.4 Hi/lo edge
    P1IFG &= ~0x10; // P1.4 IFG cleared
    P1DIR |= 0x01; // Set P1.0 to output direction
    P1DIR &= 0xEF; // Set P1.4 to input direction

    // TODO: Need to set general interrupt enable (GIE) in status register(SR)
    __bis_SR_register(GIE);


    // Set the receive buffer callback
    setReceiveCallback(SendbackSameMessage);

    while (1) {
        if (transmitNum > 0) {
            printf("%x", transmitNum);
        }
    }


    return 0;
}

/*
Appendix at the end of MSP430FR5994 Launchpad Interrupt Worksheet
has useful information.
Interrupt Macros may come in handy
#define VECTOR_NAME(name)             name##_ptr
#define EMIT_PRAGMA(x)                _Pragma(#x)
#define CREATE_VECTOR(name)           void * const VECTOR_NAME(name) = (void *)(long)&name
#define PLACE_VECTOR(vector,section)  EMIT_PRAGMA(DATA_SECTION(vector,section))
#define PLACE_INTERRUPT(func)         EMIT_PRAGMA(CODE_SECTION(func,".text:_isr"))
#define ISR_VECTOR(func,offset)       CREATE_VECTOR(func); \
                                      PLACE_VECTOR(VECTOR_NAME(func), offset) \
                                      PLACE_INTERRUPT(func)
 */


