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

void blinkLight(uint8_t lightNum){
    int i3;
    for(i3 = 400; i3 > 0; i3--){
        PJOUT |= lightNum;

        int i31;
        for(i31 = 400-i3; i31 > 0; i31--){
            __delay_cycles(1);
        }
        PJOUT &= ~lightNum;
        for(i31 = i3; i31 > 0; i31--){
            __delay_cycles(1);
        }
    }
    int i4;
    for(i4 = 400; i4 > 0; i4--){
        PJOUT |= lightNum;

        int i41;
        for(i41 = i4; i41 > 0; i41--){
            __delay_cycles(1);
        }
        PJOUT &= ~lightNum;
        for(i41 = 400-i4; i41 > 0; i41--){
            __delay_cycles(1);
        }
    }
}
void SendbackSameMessage(uint8_t length, uint8_t* data, uint32_t id){
    CANPacket *p;
    blinkLight(LED_BLUE);
    if(data[0] == BOARD_NUM){
        p->data[0] = 0xF0 | BOARD_NUM;
        uint8_t i;
        for(i = 1; i < 8; i++) {
            p->data[i] = i;
        }
        canSendPacket(p);
        blinkLight(LED_YELLOW);
    }
}
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    canInit();

    setReceiveCallback0(SendbackSameMessage);
    setReceiveCallback1(SendbackSameMessage);

    PJDIR |= 0x07;
    PJOUT &= ~0x07;
    blinkLight(LED_RED);



#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
