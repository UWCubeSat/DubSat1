#include <msp430.h>

#include "bsp/bsp.h"
#include "core/debugtools.h"

#include "ADCS_GPS.h"
#include "gps.h"

int main(void)
{
    bspInit(Module_Test);
    __bis_SR_register(GIE);

    const DebugMode mode = Mode_ASCIIInteractive;
    debugSetMode(mode);

    gpsInit();
    gpsPowerOn();

    while (1)
    {
        // if the GPS updated, send telemetry by invoking the status handler
        if(gpsUpdate() && mode == Mode_BinaryStreaming)
        {
            debugInvokeStatusHandler(Entity_Test);
        }
    }
}
