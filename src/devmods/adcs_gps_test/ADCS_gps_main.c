#include <msp430.h>

#include "ADCS_GPS.h"
#include "bsp/bsp.h"

#include "gps.h"

uint8_t actionHandler(DebugMode mode, uint8_t *command)
{
    gpsSendCommand(command);
    return 1;
}

int main(void)
{
    bspInit(Module_Test);
    __bis_SR_register(GIE); // Enter LPM3, interrupts enabled

    gpsInit();

#if defined(__DEBUG__)

    debugRegisterEntity(Entity_Test, 'g', NULL, gpsStatus, actionHandler);

    // send configuration and commands to receiver
    gpsSendCommand("interfacemode com1 novatel novatelbinary on\n\r");
    gpsSendCommand("unlogall\n\r");
    gpsSendCommand("log bestxyzb ontime 3\n\r");

#endif

    for (;;)
    {
        // TODO this empty forever-loop seems bad
    }
}
