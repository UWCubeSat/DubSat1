#define MOCK_TLE 1
#define NUM_FIELDS_IN_TLE 9

#include <adcs_estim.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"
#include "tle.h"

#include "autocode/env_estimation_lib.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;

// backchannel telemetry segments
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

// The latest TLE from CAN is held here and passed to autocode on each step
// TODO initialize it with a recent TLE before launch?
#if MOCK_TLE
// TLE taken from Wikipedia example
FILE_STATIC struct tle tle = {
                              2008,
                              264.51782528,
                              -.11606E-4,
                              51.6416,
                              247.4627,
                              .0006703,
                              130.5360,
                              325.0288,
                              15.72125391
};
#else
FILE_STATIC struct tle tle;
#endif /* MOCK_TLE */

FILE_STATIC void setInputs();
FILE_STATIC void sendTelemOverBackchannel();
FILE_STATIC void sendTelemOverCAN();

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
    tleInit(&tle);
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // set LED gpio pin to output
    LED_DIR |= LED_BIT;

    // populate segment headers
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

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
        sendTelemOverBackchannel();
        sendTelemOverCAN();
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

    // input the TLE unless we're in the middle of reading it from CAN
    // disable interrupts so the TLE isn't modified during read
    __disable_interrupt();
    if (tleIsComplete(&tle))
    {
        rtU.orbit_tle[0] = tle.year;
        rtU.orbit_tle[1] = tle.day;
        rtU.orbit_tle[2] = tle.bstar;
        rtU.orbit_tle[3] = tle.inc;
        rtU.orbit_tle[4] = tle.raan;
        rtU.orbit_tle[5] = tle.ecc;
        rtU.orbit_tle[6] = tle.aop;
        rtU.orbit_tle[7] = tle.mna;
        rtU.orbit_tle[8] = tle.mnm;
    }
    __enable_interrupt();
}

FILE_STATIC void sendTelemOverBackchannel()
{
    // send input TLE
    input_tle_segment tle;
    tle.year = rtU.orbit_tle[0];
    tle.day = rtU.orbit_tle[1];
    tle.bstar = rtU.orbit_tle[2];
    tle.inc = rtU.orbit_tle[3];
    tle.raan = rtU.orbit_tle[4];
    tle.ecc = rtU.orbit_tle[5];
    tle.aop = rtU.orbit_tle[6];
    tle.mna = rtU.orbit_tle[7];
    tle.mnm = rtU.orbit_tle[8];
    bcbinPopulateHeader(&tle.header, TLM_ID_INPUT_TLE, sizeof(tle));
    bcbinSendPacket((uint8_t *) &tle, sizeof(tle));

    // send MET
    input_met_segment metSeg;
    metSeg.met = rtU.MET;
    bcbinPopulateHeader(&metSeg.header, TLM_ID_INPUT_MET, sizeof(metSeg));
    bcbinSendPacket((uint8_t *) &metSeg, sizeof(metSeg));

    // send autocode outputs
    output_segment out;
    uint8_t i = 3;
    while (i-- > 0)
    {
        out.sc2gs_unit[i] = rtY.sc2gs_unit[i];
        out.sc2sun_unit[i] = rtY.sc2sun_unit[i];
        out.mag_unit_vector_eci[i] = rtY.mag_unit_vector_eci[i];
        out.mag_vector_eci[i] = rtY.mag_vector_eci[i];
        out.vel_eci_mps[i] = rtY.vel_eci_mps[i];
    }
    out.sc_above_gs = rtY.sc_above_gs;
    out.sc_in_fov = rtY.sc_in_fov;
    out.sc_in_sun = rtY.sc_in_sun;
    bcbinPopulateHeader(&out.header, TLM_ID_OUTPUT, sizeof(out));
    bcbinSendPacket((uint8_t *) &out, sizeof(out));

}

FILE_STATIC void sendTelemOverCAN()
{
    // TODO
}

void canRxCallback(CANPacket *packet)
{
    __disable_interrupt();
    tleUpdate(packet, &tle);
    __enable_interrupt();

    switch (packet->id)
    {
    // TODO add MET case when available (unless bsp handles this automatically)
    }
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
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}
