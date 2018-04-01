#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "sensors/photodiode.h"
#include "sensors/magnetometer.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    uint8_t hPD = photodiodeInit(0x24, I2CBus2);






    //uint32_t v = photodiodeVoltage(hPD, GAIN1);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    LED_PORT_DIR |= LED0_BIT;

    while(1)
    {
        LED_PORT_OUT ^= LED0_BIT;

        uint32_t v = photodiodeVoltage(hPD, GAIN1);
        //__delay_cycles(0.12 * SEC);
    }
	
	return 0;
}
