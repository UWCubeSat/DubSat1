#include <msp430.h> 
#include "pcvtest.h"
#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"

PCVSensorData *pdata;

/*
 * main.c
 */
int main(void) {

    hDev hSensor;

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
    hSensor = pcvsensorInit(INA219_CURRENT_SENSOR_I2C_ADDRESS);

    float calcdcurrentA;
    while (1)
    {
        __delay_cycles(0.5 * SEC);

        pdata = pcvsensorRead();
        //debugPrintF("Raw voltage output - Bus: %d V, Shunt: %d \r\n", pdata->rawBusVoltage, pdata->rawShuntVoltage);
        calcdcurrentA = pdata->shuntVoltageV / 0.1;
        debugPrintF("---> Converted voltage output - Bus: %.3f V, Shunt: %.3f (calc'd current: %f)\r\n",
                    pdata->busVoltageV, pdata->shuntVoltageV, calcdcurrentA);
    }

	return 0;
}
