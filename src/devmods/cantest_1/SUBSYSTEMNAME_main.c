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
    msg[0] = 0xF0;
    uint8_t i;
    for( i = 1; i < length; i++) {
        msg[i] = data[i];
    }

   canSend(0,tech, msg);
}
void SendbackSameMessageb(uint8_t length, uint8_t* data){
    uint8_t tech[5] = {0,0,0,0,0x08};
    uint8_t msg[8];
    readRegister(0x24, msg);
    uint8_t i;
    msg[1] = 0xF1;
    for( i = 2; i < length; i++) {
        msg[i] = data[i];
    }

   canSend(0,tech, msg);
}
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    canInit();

    setReceiveCallback0(SendbackSameMessage);
    setReceiveCallback1(SendbackSameMessageb);
    while(0x01){
        uint8_t tech[5] = {0,0,0,0,0x08};
        uint8_t msg[8];
        uint8_t i;
        for( i = 0; i < 8; i++) {
            msg[i] = i;
        }
        //canSend(0,tech, msg);
    }
#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
