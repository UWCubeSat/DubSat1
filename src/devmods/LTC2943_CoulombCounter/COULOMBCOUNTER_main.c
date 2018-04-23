#include <msp430.h> 

#include "POWER_LTC2943_COULOMBCOUNTER.h"
#include "bsp/bsp.h"
#include "core/i2c.h"

/*
 * main.c
 */
int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
	
    LTC2943Init(I2CBus2, 6.0);
    CoulombCounterData device;

    while(1) {
        device= readCoulombCounter();
        debugPrintF("%.3f\r\n", device.busVoltageV);
        debugPrintF("%.3f\r\n", device.calcdCurrentA);
        debugPrintF("%.3f\r\n", device.SOC);
    }

	return 0;
}
