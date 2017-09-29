#include <msp430.h> 

#include "cosmos_client.h"
#include "bsp/bsp.h"
#include "core/timers.h"
#include "core/utils.h"

/*
 * main.c
 */

//uint8_t outstr[] = "This is a test.\r\n";
soh_t mySoh;
hBus h;

void sendTlm()
{
    mySoh.length = sizeof(mySoh);
    uartTransmit(h, (const uint8_t *) &mySoh, sizeof(mySoh));
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    h = uartInit(ApplicationUART, 1);

    // Enable for output
    LED_PORT_DIR |= LED_BITS;

    mySoh.id = 0x50;
    mySoh.delay = 250;
    mySoh.mode = MODE_ON;
    mySoh.syncpattern = 0xDE;

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__

    while(1)
    {
        //uartTransmit(h, outstr, strlen(outstr));
        sendTlm();
        LED_PORT_OUT ^= LED_BITS;
        __delay_cycles(0.25 * SEC);
    }

    return 0;
}
