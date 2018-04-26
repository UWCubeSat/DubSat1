#define MOCK_TLE 1

#include <adcs_estim.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "core/debugtools.h"
#include "tle.h"

#include "autocode/MSP_env_estim.h"

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

FILE_STATIC void rt_OneStep(void);

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
    tleInit(&tle, MOCK_TLE);
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
    MSP_env_estim_initialize();

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

        // step autocode
        rt_OneStep();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

void rt_OneStep(void)
{
  static boolean_T OverrunFlags[2] = { 0, 0 };

  static boolean_T eventFlags[2] = { 0, 0 };/* Model has 2 rates */

  static int_T taskCounter[2] = { 0, 0 };

  /* Disable interrupts here */
  __disable_interrupt();

  /* Check base rate for overrun */
  if (OverrunFlags[0]) {
    rtmSetErrorStatus(rtM, "Overrun");
    return;
  }

  OverrunFlags[0] = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  __enable_interrupt();

  /*
   * For a bare-board target (i.e., no operating system), the
   * following code checks whether any subrate overruns,
   * and also sets the rates that need to run this time step.
   */
  if (taskCounter[1] == 0) {
    if (eventFlags[1]) {
      OverrunFlags[0] = false;
      OverrunFlags[1] = true;

      /* Sampling too fast */
      rtmSetErrorStatus(rtM, "Overrun");
      return;
    }

    eventFlags[1] = true;
  }

  taskCounter[1]++;
  if (taskCounter[1] == 2) {
    taskCounter[1]= 0;
  }

  /* Set model inputs associated with base rate here */

  /* Step the model for base rate */
  MSP_env_estim_step0();

  /* Get model outputs here */
  sendTelemOverBackchannel();
  sendTelemOverCAN();

  /* Indicate task for base rate complete */
  OverrunFlags[0] = false;

  /* If task 1 is running, don't run any lower priority task */
  if (OverrunFlags[1]) {
    return;
  }

  /* Step the model for subrate */
  if (eventFlags[1]) {
    OverrunFlags[1] = true;

    /* Set model inputs associated with subrates here */
    setInputs();

    /* Step the model for subrate 1 */
    MSP_env_estim_step1();

    /* Get model outputs here */

    /* Indicate task complete for subrate */
    OverrunFlags[1] = false;
    eventFlags[1] = false;
  }

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
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
        rtU.orbit_TLE[0] = tle.year;
        rtU.orbit_TLE[1] = tle.day;
        rtU.orbit_TLE[2] = tle.bstar;
        rtU.orbit_TLE[3] = tle.inc;
        rtU.orbit_TLE[4] = tle.raan;
        rtU.orbit_TLE[5] = tle.ecc;
        rtU.orbit_TLE[6] = tle.aop;
        rtU.orbit_TLE[7] = tle.mna;
        rtU.orbit_TLE[8] = tle.mnm;
    }
    __enable_interrupt();
}

FILE_STATIC void sendTelemOverBackchannel()
{
    // send input TLE
    input_tle_segment tle;
    tle.year = rtU.orbit_TLE[0];
    tle.day = rtU.orbit_TLE[1];
    tle.bstar = rtU.orbit_TLE[2];
    tle.inc = rtU.orbit_TLE[3];
    tle.raan = rtU.orbit_TLE[4];
    tle.ecc = rtU.orbit_TLE[5];
    tle.aop = rtU.orbit_TLE[6];
    tle.mna = rtU.orbit_TLE[7];
    tle.mnm = rtU.orbit_TLE[8];
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
        out.sc2sun_unit[i] = rtY.sc2sun_unit[i];
        out.mag_eci_unit[i] = rtY.mag_eci_unit[i];
        out.pos_eci_m[i] = rtY.pos_eci_m[i];
        out.vel_eci_mps[i] = rtY.vel_eci_mps[i];
    }
    out.SGP4_flag = rtY.SGP4_flag;
    out.sc_in_sun = rtY.sc_in_sun;
    out.sc_above_gs = rtY.sc_above_gs;
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
