#include <msp430.h> 
#include "pcvtest.h"
#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"

PCVSensorData *pdata1, *pdata2;

/*
 * main.c
 */
int main(void) {

    hDev hSensor1, hSensor2;

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
    // Show that multiple sensors work
    hSensor1 = pcvsensorInit(I2CBus2, 0x40, 0.1, 16);
    hSensor2 = pcvsensorInit(I2CBus2, 0x44, 0.1, 16);


    float calcdcurrentA1, calcdcurrentA2;
    while (1)
    {
        __delay_cycles(0.5 * SEC);

        pdata1 = pcvsensorRead(hSensor1);
        pdata2 = pcvsensorRead(hSensor2);
        //debugPrintF("Raw voltage output - Bus: %d V, Shunt: %d \r\n", pdata->rawBusVoltage, pdata->rawShuntVoltage);
        calcdcurrentA1 = pdata1->shuntVoltageV / 0.1;
        calcdcurrentA2 = pdata2->shuntVoltageV / 0.1;
        debugPrintF("Bus #1: %.3f V, Shunt #1: %.3f, Current #1: %.4f (soft), %.4f (hard); Bus #2: %.3f V, Shunt #2: %.3f, Current #2: %.4f (soft), %.4f (hard)\r\n",
                    pdata1->busVoltageV, pdata1->shuntVoltageV, calcdcurrentA1, pdata1->calcdCurrentA,
                    pdata2->busVoltageV, pdata2->shuntVoltageV, calcdcurrentA2, pdata2->calcdCurrentA);
    }

	return 0;
}
