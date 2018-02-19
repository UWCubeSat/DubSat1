#include <eps_batt.h>
#include <msp430.h> 

#include "bsp/bsp.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;


FILE_STATIC void battInit()
{
    //Initialize battery balancer enable register pin
    BATT_BALANCER_ENABLE_DIR |= BATT_BALANCER_ENABLE_BIT;
    LED_DIR |= LED_BIT;
    LED_OUT |= LED_BIT;
}

FILE_STATIC void battControlBalancer(BattBalancerCmd cmd)
{
    if (cmd == BBCmd_ExplicitEnable || cmd == BBCmd_AutoEnable)
        BATT_BALANCER_ENABLE_OUT |= BATT_BALANCER_ENABLE_BIT;
}

FILE_STATIC uint8_t handleActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // handle actions in a user-friendly way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // handle actions in a low-output way
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // handle actions, any output should be ground-segment friendly
        // "packet" format
    }
    return 1;
}

/*
 * main.c
 */
int main(void)
{

    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

    // Other initialization
    battInit();


#if defined(__DEBUG__)

    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                          NULL,
                                          handleActionCallback);

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    debugTraceF(1, "Commencing subsystem module execution ...\r\n");

    battControlBalancer(BBCmd_AutoEnable);

    uint16_t counter;
    while (1)
    {
        counter++;
        LED_OUT ^= LED_BIT;
        __delay_cycles(0.125 * SEC);

        if (counter % 8 == 0)
        {

        }



     }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}
