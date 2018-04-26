#define AUTOCODE_UPDATE_DELAY_US 4000 // 250 Hz (wow)
#define BACKCHANNEL_DELAY_MS 1000

#include <adcs_mpc.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "interfaces/canwrap.h"
#include "core/timer.h"

#include "autocode/MSP_FSW.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;

// Autocode steps and timing
FILE_STATIC flag_t triggerStepFlag = FALSE;
FILE_STATIC void triggerStep();
FILE_STATIC void rt_OneStep();
FILE_STATIC void acceptInputs();

// CAN input
FILE_STATIC void canRxCallback(CANPacket *p);
FILE_STATIC sp2fsw tmpSP2FSW;

// CAN output
FILE_STATIC void sendCANVelocityPointing();
FILE_STATIC void sendCANOutputs();

// Backchannel telemerty
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

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

    // populate segment headers
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

    // init temperature sensor
    asensorInit(Ref_2p5V);

    // init autocode
    MSP_FSW_initialize();

    // initialize timers
    initializeTimer();
    int timerHandle = timerCallbackInitializer(&triggerStep,
                                               AUTOCODE_UPDATE_DELAY_US);
    startCallback(timerHandle);
    int slowTimerHandle = timerPollInitializer(BACKCHANNEL_DELAY_MS);

    while (1)
    {
        // wait for quick timer to trigger for autocode
        while (!triggerStepFlag);
        triggerStepFlag = FALSE;

        // check slow timer for non-autocode functions
        if (checkTimer(slowTimerHandle))
        {
            slowTimerHandle = timerPollInitializer(BACKCHANNEL_DELAY_MS);

            LED_OUT ^= LED_BIT;

            // send basic subsystem telemetry
            // TODO move this to rollcall
            sendHealthSegment();
            sendMetaSegment();
        }

        // step autocode
        rt_OneStep();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

FILE_STATIC void triggerStep()
{
    triggerStepFlag = TRUE;
}

void rt_OneStep(void)
{
  static boolean_T OverrunFlags[3] = { 0, 0, 0 };

  static boolean_T eventFlags[3] = { 0, 0, 0 };/* Model has 3 rates */

  static int_T taskCounter[3] = { 0, 0, 0 };

  int_T i;

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
  for (i = 1; i < 3; i++) {
    if (taskCounter[i] == 0) {
      if (eventFlags[i]) {
        OverrunFlags[0] = false;
        OverrunFlags[i] = true;

        /* Sampling too fast */
        rtmSetErrorStatus(rtM, "Overrun");
        return;
      }

      eventFlags[i] = true;
    }
  }

  taskCounter[1]++;
  if (taskCounter[1] == 25) {
    taskCounter[1]= 0;
  }

  taskCounter[2]++;
  if (taskCounter[2] == 50) {
    taskCounter[2]= 0;
  }

  /* Set model inputs associated with base rate here */
  // (base rate has no inputs other than in substep)

  /* Step the model for base rate */
  /*
   * rate: 250 Hz
   * outputs: point_true
   *
   * substep:
   *    rate: 10 Hz
   *    outputs: sc_quat, body_rates, sc_mode, sc_above_gsb, sc_modeb,
   *             cmd_MT_fsw_dv
   *    inputs:  MT_valid
   */
  MSP_FSW_step0();

  /* Get model outputs here */
  sendCANVelocityPointing();

  /* Indicate task for base rate complete */
  OverrunFlags[0] = false;

  /* Step the model for any subrate */
  for (i = 1; i < 3; i++) {
    /* If task "i" is running, don't run any lower priority task */
    if (OverrunFlags[i]) {
      return;
    }

    if (eventFlags[i]) {
      OverrunFlags[i] = true;

      /* Set model inputs associated with subrates here */
      acceptInputs();

      /* Step the model for subrate "i" */
      switch (i) {
       case 1 :
       /*
        * rate: 10 Hz
        * inputs: mag_vec_body_T, sun_vec_body_sunsensor, sc_in_sun,
        *         mag_eci_unit, sc2sun_unit, gyro_omega_body_radps, sc_above_gs
        * outputs: the outputs from the base rate's substep
        */
        MSP_FSW_step1();

        /* Get model outputs here */
        sendCANOutputs();
        break;

       case 2 :
       /*
        * rate: 5 Hz
        * inputs: CAN
        */
        MSP_FSW_step2();

        /* Get model outputs here */
        // (none)
        break;

       default :
        break;
      }

      /* Indicate task complete for sample time "i" */
      OverrunFlags[i] = false;
      eventFlags[i] = false;
    }
  }

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void canRxCallback(CANPacket *p)
{
    sensorproc_mag mag;
    sensorproc_imu imu;
    sensorproc_sun sun;

    real32_T *tmp_mag = tmpSP2FSW.mag_vec_body_T;
    real32_T *tmp_imu = tmpSP2FSW.gyro_omega_body_radps;
    real32_T *tmp_sun = tmpSP2FSW.sun_vec_body_sunsensor;

    switch (p->id)
    {
    case CAN_ID_SENSORPROC_MAG:
        decodesensorproc_mag(p, &mag);
        tmp_mag[0] = mag.sensorproc_mag_x;
        tmp_mag[1] = mag.sensorproc_mag_y;
        tmp_mag[2] = mag.sensorproc_mag_z;
        tmp_mag[3] = mag.sensorproc_mag_valid;
        break;
    case CAN_ID_SENSORPROC_IMU:
        decodesensorproc_imu(p, &imu);
        tmp_imu[0] = imu.sensorproc_imu_x;
        tmp_imu[1] = imu.sensorproc_imu_y;
        tmp_imu[2] = imu.sensorproc_imu_z;
        tmp_imu[3] = imu.sensorproc_imu_valid;
        break;
    case CAN_ID_SENSORPROC_SUN:
        decodesensorproc_sun(p, &sun);
        tmp_sun[0] = sun.sensorproc_sun_x / INT16_MAX;
        tmp_sun[1] = sun.sensorproc_sun_y / INT16_MAX;
        tmp_sun[2] = sun.sensorproc_sun_z / INT16_MAX;
        tmp_sun[3] = sun.sensorproc_sun_valid;
        break;
    // TODO get other CAN inputs
    }
}

FILE_STATIC void acceptInputs()
{
    __disable_interrupt();
    /*
     * TODO missing MT_valid, sc_in_sun, sc_above_gs, mag_eci_unit, sc2sun_unit
     * (i.e. MT_valid and ESTIM's outputs)
     */

    memcpy(rtU.mag_vec_body_T, tmpSP2FSW.mag_vec_body_T,
           4 * sizeof(tmpSP2FSW.mag_vec_body_T[0]));
    memcpy(rtU.gyro_omega_body_radps, tmpSP2FSW.gyro_omega_body_radps,
           4 * sizeof(tmpSP2FSW.gyro_omega_body_radps[0]));
    memcpy(rtU.sun_vec_body_sunsensor, tmpSP2FSW.sun_vec_body_sunsensor,
           4 * sizeof(tmpSP2FSW.sun_vec_body_sunsensor[0]));
    __enable_interrupt();
}

FILE_STATIC void sendCANVelocityPointing()
{
    CANPacket p;
    mpc_vp vp;
    vp.mpc_vp_status = rtY.point_true
            ? CAN_ENUM_BOOL_TRUE : CAN_ENUM_BOOL_FALSE;
    encodempc_vp(&vp, &p);
    canSendPacket(&p);
}

FILE_STATIC void sendCANOutputs()
{
    /*
     * TODO send sc_quat, body_rates, sc_mode, sc_above_gsb, sc_modeb, and
     * cmd_MT_fsw_dv.
     */
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

