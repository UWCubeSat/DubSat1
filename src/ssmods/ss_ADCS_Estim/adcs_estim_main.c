#define NUM_FIELDS_IN_TLE 9

#include <adcs_estim.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"

#include "autocode/env_estimation_lib.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;

// backchannel telemetry segments
health_segment hseg;

// The latest TLE from CAN is held here and passed to autocode on each step
// TODO initialize it with a recent TLE before launch?
FILE_STATIC real_T orbit_tle[NUM_FIELDS_IN_TLE] = { 0 };

// util functions to convert weird timeStamp type
FILE_STATIC double getTimeStampSeconds();
FILE_STATIC uint64_t getTimeStampInt();

FILE_STATIC void setInputs();
FILE_STATIC void useOutputs();

FILE_STATIC void sendHealthSegment();
FILE_STATIC void sendMetaSegment();

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
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // set LED gpio pin to output
    LED_DIR |= LED_BIT;

    // init temperature sensor
    asensorInit(Ref_2p5V);

    // init autocode
    env_estimation_lib_initialize();

    /*
     * TODO consider using a callback timer instead of a while loop. We'll
     * likely want to run as fast as possible, but it may also be worthwhile to
     * run at a fixed rate (using timers instead).
     */
    while (1)
    {
        LED_OUT ^= LED_BIT;

        // send basic subsystem telemetry
        // TODO move this to rollcall
        sendHealthSegment();
        sendMetaSegment();

        // set autocode inputs
        setInputs();

        // step autocode
        env_estimation_lib_step();

        // send autocode outputs
        useOutputs();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
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

FILE_STATIC void setInputs()
{
    // TODO verify units
    rtU.MET = getTimeStampSeconds();

    // TODO how will the autocode handle unintialized TLEs?
    memcpy(rtU.orbit_tle, orbit_tle, NUM_FIELDS_IN_TLE * sizeof(real_T));
}

FILE_STATIC void useOutputs()
{
    // TODO send CAN packets
    // use rtY
}

void canRxCallback(CANPacket *packet)
{
    switch (packet->id)
    {
    // TODO add MET case when available (unless bsp handles this automatically)
    // TODO add TLE case when available
    }
}

// combine timeStamp fields into one int
FILE_STATIC uint64_t getTimeStampInt()
{
    timeStamp t = getTimeStamp();
    uint64_t res = (uint64_t) t.count1;
    res |= ((uint64_t) t.count2) << 8;
    res |= ((uint64_t) t.count3) << 16;
    res |= ((uint64_t) t.count4) << 24;
    res |= ((uint64_t) t.count5) << 32;
    return res;
}

FILE_STATIC double getTimeStampSeconds()
{
    // res is in units of 2^-8s, or about 4 ms
    return ((double) getTimeStampInt()) / 256.0;
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
FILE_STATIC void sendHealthSegment()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // TODO determine overall health based on querying sensors for their health
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);

    // send CAN packet of temperature (in deci-Kelvin)
    msp_temp temp = { (hseg.inttemp + 273.15f) * 10 };
    CANPacket packet;
    encodemsp_temp(&temp, &packet);
    canSendPacket(&packet);
}

FILE_STATIC void sendMetaSegment()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    meta_segment mseg;
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}
