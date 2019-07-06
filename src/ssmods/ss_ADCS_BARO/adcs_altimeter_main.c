#include <adcs_altimeter.h>
#include <msp430.h> 
#include <sensors/altimeter.h>

#include "bsp/bsp.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC health_segment hseg;
FILE_STATIC altimeter_segment altimeter_data_cosmos;
FILE_STATIC AltimeterData *altitudeData;

FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

/*
 * main.c
 */
int main(void)
{

    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
    // as a predefined symbol
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)

    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
    //                               handleDebugStatusCallback,
    //                               handleDebugActionCallback);

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    initial_setup();

    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
    while (1)
    {
        // This assumes that some interrupt code will change the value of the triggerStaten variables
//        switch (ss_state)
//        {
//        case State_FirstState:
//            if (triggerState2)
//            {
//                triggerState2 = 0;
//                ss_state = State_SecondState;
//            }
//            break;
//        case State_SecondState:
//            if (triggerState3)
//            {
//                triggerState3 = 0;
//                ss_state = State_ThirdState;
//            }
//            break;
//        case State_ThirdState:
//            if (triggerState1)
//            {
//                triggerState1 = 0;
//                ss_state = State_FirstState;
//            }
//            break;
//        default:
//            mod_status.state_transition_errors++;
//            mod_status.in_unknown_state++;
//            break;
//        }
        read_altimeter_data();
        send_altimetry_reading_cosmos();
    }



    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

FILE_STATIC void initial_setup()
{
    baromInit(I2CBus2);

    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&altimeter_data_cosmos.header, TLM_ID_ALTIM_BAROMETER, sizeof(altimeter_data_cosmos));
}

FILE_STATIC void read_altimeter_data()
{
    altitudeData = readAltitudeData(12);
    altimeter_data_cosmos.pressure = altitudeData->pressure;
    altimeter_data_cosmos.temperature = altitudeData->temperature;
    altimeter_data_cosmos.altitude = altitudeData->altitude;
}

FILE_STATIC void send_altimetry_reading_cosmos()
{
    bcbinSendPacket((uint8_t *) &altimeter_data_cosmos, sizeof(altimeter_data_cosmos));
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

