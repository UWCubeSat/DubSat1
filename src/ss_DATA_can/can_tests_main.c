#include <msp430.h> 
#include <stdio.h>
#include "bsp/bsp.h"
#include "core/can.h"
#include "core/spi.h"

 volatile int transmitNum;

// Send back the same reply
void SendbackSameMessage(uint8_t length, uint8_t* data){
    if (transmitNum++ < 5) {
        printf("TransmitNum: %d\n", transmitNum);
        printf("Recv Msg Length %d, Data:%x,%x,%x,%x,%x,%x,%x,\n", length, data[0], data[1],data[2],data[3],data[4],data[5],data[6]);
        uint8_t tech[5] = {0,0,0,0,length};
        uint8_t msg[8] ={0,0,0,0,0,0,0,0};

        uint8_t i;
        for( i = 0; i < length; i++) {
            msg[i] = data[i];
        }

        canSend(0,tech, msg);
    }
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

    transmitNum = 0;
    printf("Test Print %d\n", transmitNum);

    // Enable individual interrupt for Port 1
    //       pg 26 in Interrupts Workshop
    P1IE |= 0x10; // P1.4 interrupt enabled
    P1IES |= 0x10; // P1.4 Hi/lo edge
    P1IFG &= ~0x10; // P1.4 IFG cleared
    P1DIR |= 0x01; // Set P1.0 to output direction
    P1DIR &= 0xEF; // Set P1.4 to input direction

    //  Need to set general interrupt enable (GIE) in status register(SR)
    __bis_SR_register(GIE);

    testSend();

    // Set the receive buffer callback
    setReceiveCallback(SendbackSameMessage);
    uint8_t msg[8] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
    SendbackSameMessage(8,msg);
    printf("Message Sent\n");

    int i = 0;
    for(i=10000;i>0;i--){}


    // TODO: Bug where INT pin never goes back up to high after
    // an initial interrupt
    int oldNum = transmitNum;
    while (transmitNum < 5) {
        if (oldNum != transmitNum) {
            printf("Value changed\n");
            oldNum = transmitNum;
        }
    }
    return 0;
}

