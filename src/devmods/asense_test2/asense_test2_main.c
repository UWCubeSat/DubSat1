#include <msp430.h> 
#include "asense_test2.h"
#include "sensors/analogsensor.h"


/*
 * main.c
 */

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    asensorInit(Ref_2p5V);

    // Now configure the non-temperature ADC channels
    // Set to 1, 1 for SEL0,SEL1 for A2
    ASENSE_SEL1 |= ASENSE_BIT;
    ASENSE_SEL0 |= ASENSE_BIT;

    __bis_SR_register(GIE); // LPM4 with interrupts enabled

    float tempC;
    float raw1;
    hDev hSensor = asensorActivateChannel(CHAN_A2);

    asensorUpdateAllSensors();

    while(1)
    {
//        tempC = asensorReadTempC();
//        raw1 = asensorReadSingleSensorV(hSensor);
        tempC = asensorGetLastTempC();
        raw1 = asensorGetLastValueV(hSensor);
        debugPrintF("Temp:  %f C, Other voltage: %f V\r\n", tempC, raw1);
        __delay_cycles(0.5 * SEC);
    }
}



