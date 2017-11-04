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
        SunSensorAngle *angleData = sunSensorReadAngle();
        if (angleData != NULLP)
        {
            debugPrintF("alpha: %f, beta: %f, error: %u\r\n", angleData->alpha,
                        angleData->beta, angleData->error);
        }

        __delay_cycles(10 * MSEC); // 10 ms recommended
    }

    return 0;
}
