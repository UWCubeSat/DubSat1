#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/can.h"

/*
 * main.c
 */
// Send back the same reply
void SendbackSameMessage(uint8_t length, uint8_t* data){
    uint8_t tech[5] = {0,0,0,0,0x08};
    uint8_t msg[8];
    //msg[0] = 0xB1;
    //msg[1] = 0x6B;
    //msg[2] = 0x00;
    uint8_t mask;
    uint8_t filter;
    uint8_t testReg;
    readRegister(0x22, &mask);
    readRegister(0x02, &filter);
    readRegister(0x2B, &testReg);
    msg[0] = mask;
    msg[1] = filter;
    msg[2] = testReg;
    msg[3] = 0xB5;
    P1DIR |= 0x01;
    uint8_t i;
    for( i = 4; i < length; i++) {
        msg[i] = data[i];
    }

   canSend(0,tech, msg);
}
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    canInit();

    setReceiveCallback(SendbackSameMessage);

//    uint8_t a[1];
//    while(0x01){
//        spiTrancieve(a, a, 1, CS_1);
//    }

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
