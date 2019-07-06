// Debug LED
#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT BIT0

#define UPDATE_DELAY_US 1000000

#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/MET.h"
#include "core/debugtools.h"
#include "core/timer.h"

void handlePPTFiringNotification();
void handleRollCall();

void update();

/*
 * main.c
 */
int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);
    coreStartup(handlePPTFiringNotification, handleRollCall);

#if defined(__DEBUG__)

    debugSetMode(Mode_ASCIIInteractive);
    debugRegisterEntity(Entity_Test, NULL, NULL, NULL);

#endif  //  __DEBUG__

    // set LED gpio pin to output
    LED_DIR |= LED_BIT;

    initializeTimer();
    int timerHandle = timerCallbackInitializer(update, UPDATE_DELAY_US);
    startCallback(timerHandle);

    while (1);

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

void update()
{
    LED_OUT ^= LED_BIT;

    double t = getTimeStampSeconds();
    debugPrintF("met (s): %f\r\n", t);
}

// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}

// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
void handleRollCall()
{
    __no_operation();
}
