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

void SendbackSameMessage(uint8_t length, uint8_t* data, uint32_t id){
    PJDIR |= 0x02;
    PJOUT ^= 0x02;
    CANPacket *p;
//    if(data[0] == BOARD_NUM){
//        p->data[0] = 0xF0 | BOARD_NUM;
//        uint8_t i;
//        for(i = 1; i < 8; i++) {
//            p->data[i] = i;
//        }
//        canSendPacket(p);
//    }
    BatteryStatus pst = {0};
    CANPacket ppst = {0};
    CANPacket *pps = &ppst;
    pst.batteryFullChargeCount = 0xffff;
    pst.batteryTemperature = 0;
    pst.batteryVoltage = 0;
    pst.LowestBatteryVoltage = 0;
    pst.underVoltageEvents = 4;
    encodeBatteryStatus(&pst,pps);
    canSendPacket(pps);



}
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    canInit();

    setReceiveCallback0(SendbackSameMessage);
    setReceiveCallback1(SendbackSameMessage);

    PJDIR |= 0x04;
    PJOUT |= 0x04;




#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
