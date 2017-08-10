#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/can.h"

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
    blinkLight(LED_BLUE);
    if(data[0] == BOARD_NUM){
        uint32_t identifier = 0x000CAFE0 | BOARD_NUM;
        uint8_t length = 0x08;
        uint8_t tech[5] = {
           (uint8_t) (identifier >> 21),
           (uint8_t) (identifier >> 16) & 0x03 | (uint8_t) (identifier >> 13) & 0xE0 | 0x08,
           (uint8_t) (identifier >> 8),
           (uint8_t) identifier,
           length
        };
        uint8_t msg[8];
        uint8_t i;
        msg[0] = 0xF0 | BOARD_NUM;
        for( i = 1; i < 5; i++) {
            msg[i] = (uint8_t) (id >> 8 * (4 - i));
        }
        for( i = 5; i < length; i++) {
            msg[i] = data[i];
        }
        canSend(0,tech, msg);
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

    setTheFilter(CAN_MASK_0, 0x00000001);
    setTheFilter(CAN_FILTER_0, 0x00000001);
    setTheFilter(CAN_FILTER_1, 0x00000001);
    setTheFilter(CAN_MASK_1, 0x00000001);
    setTheFilter(CAN_FILTER_2, 0x00000001);
    setTheFilter(CAN_FILTER_3, 0x00000001);
    setTheFilter(CAN_FILTER_4, 0x00000001);
    setTheFilter(CAN_FILTER_5, 0x00000001);


#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
