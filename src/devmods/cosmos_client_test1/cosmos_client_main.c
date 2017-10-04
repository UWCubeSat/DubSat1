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

cmdstate callbackState = STATE_START;
cmdtype currentCmd = CMD_NONE;

void myReadCallback(uint8_t rcvdbyte)
{
    switch (callbackState)
    {
        case STATE_START:
            if (rcvdbyte == OPCODE_DELAY)
            {
                callbackState = STATE_PARAMWAIT;
                currentCmd = CMD_DELAY;
            }
            else if (rcvdbyte == OPCODE_MODE)
            {
                callbackState = STATE_PARAMWAIT;
                currentCmd = CMD_MODE;
            }
            else
            {
                callbackState = STATE_START;
                currentCmd = CMD_NONE;
            }
            break;
        case STATE_PARAMWAIT:
            if (currentCmd == CMD_DELAY)
                mySoh.delay = rcvdbyte;
            else if (currentCmd == CMD_MODE)
                mySoh.mode = rcvdbyte;

            callbackState = STATE_START;
            currentCmd = CMD_NONE;
            break;
        default:
            callbackState = STATE_START;
            currentCmd = CMD_NONE;
    }
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    h = uartInit(ApplicationUART, 1, Speed_9600);
    uartRegisterRxCallback(h, myReadCallback);

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
        if (mySoh.mode == MODE_ON)
            LED_PORT_OUT ^= LED_BITS;
        spinWait_ms(mySoh.delay);
    }

    return 0;
}
