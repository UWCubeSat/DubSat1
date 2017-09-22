#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state = State_FirstState;
FILE_STATIC volatile SubsystemMode ss_mode = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;

/*
 * main.c
 */
int main(void) {

    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code, as
    // it sets up critical hardware settings for board specified by the __BSP_Board... defintion used.
    // If module not yet available in enum, add to SubsystemModule enumeration AND
    // SubsystemModulePaths (a string name) in systeminfo.c/.h
    bspInit(Module_Test);  // <<DO NOT DELETE or MOVE>>

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up sync pulse handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.
#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
    while (1)
    {
        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
        case State_FirstState:
            if (triggerState2)
            {
                triggerState2 = 0;
                ss_state = State_SecondState;
            }
            break;
        case State_SecondState:
            if (triggerState3)
            {
                triggerState3 = 0;
                ss_state = State_ThirdState;
            }
            break;
        case State_ThirdState:
            if (triggerState1)
            {
                triggerState1 = 0;
                ss_state = State_FirstState;
            }
            break;
        default:
            mod_status.state_transition_errors++;
            mod_status.in_unknown_state++;
            break;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

/* ----- SYNC PULSE INTERRUPT HANDLERS ----- */
// Both of these handlers are INTERRUPT HANDLERS, and run as such, which means that all OTHER
// interrupts are blocked while they are running.  This can cause all sorts of issues, so
// MAKE SURE TO MINIMIZE THE CODE RUNNING IN THESE FUNCTIONS.
// Sync pulse 1:  typically raised every 2 seconds while PPT firing, to help each subsystem module
// do the "correct thing" around the firing sequence.  This timing might
// not be exact, and may even change - don't rely on it being 2 seconds every time, and it may
// be shut off entirely during early or late stages of mission, so also do NOT use as a "heartbeat"
// for other, unrelated functionality.
void handleSyncPulse1()
{
    __no_operation();
}

// Sync pulse 2:  typically every 1-2 minutes, but again, don't count on any length.
// General semanatics are that this pulse means all subsystems should share accumulated
// status data on the CAN bus.  It is also the cue for the PPT to ascertain whether it
// will use the following period as an active or suspended firing period.  All subsystems
// will assume active until they are notified that firing has been suspended, but
// this determination will be reset (back to active) at each sync pulse 2.
void handleSyncPulse2()
{
    __no_operation();
}
