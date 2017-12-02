#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/utils.h"

#include "sun_sensor_test.h"
#include "sun_sensor.h"

int main(void)
{

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);

    sunSensorInit();

    while (TRUE)
    {
        // measure voltage
//        sun_sensor_voltage *voltages = sunSensorReadUnfiltered();
//        if (voltages != NULLP)
//        {
//            debugPrintF("voltages\r\n");
//            uint8_t i;
//            for (i = 0; i < 4; i++) {
//                debugPrintF("\t%f\r\n", voltages[i]);
//            }
//        }

        // measure angle
        SunSensorAngle *angle = sunSensorReadAngle();
        if (angle != NULLP)
        {
            debugPrintF("alpha: %f, beta: %f, error: %u\r\n", angle->alpha,
                        angle->beta, angle->error);
        }

        __delay_cycles(SEC); // 10 ms recommended
    }

    return 0;
}
