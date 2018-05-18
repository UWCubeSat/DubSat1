// 40 Hz
#define AUTOCODE_UPDATE_DELAY_US 25000

#include <adcs_sensorproc.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/timer.h"
#include "core/agglib.h"
#include "interfaces/rollcall.h"
#include "interfaces/canwrap.h"

#include "adcs_sensorproc_ids.h"
#include "sensorInterface.h"
#include "sunsensor_io.h"
#include "mag_io.h"
#include "imu_io.h"

#include "autocode/MSP_SP.h"

/* Sensor interfaces */

FILE_STATIC const SensorInterface sensorInterfaces[] =
{
    {
     sunsensorioInit,
     sunsensorioUpdate,
     sunsensorioSendBackchannel,
     sunsensorioSendCAN,
    },
    {
     magioInit,
     magioUpdate,
     magioSendBackchannel,
     magioSendCAN,
    },
    {
     imuioInit,
     imuioUpdate,
     imuioSendBackchannel,
     imuioSendCAN,
    },
};

#define NUM_INTERFACES (sizeof(sensorInterfaces) / sizeof(SensorInterface))

FILE_STATIC void initSensorInterfaces();
FILE_STATIC void updateSensorInterfaces();
FILE_STATIC void sendSensorBackchannel();
FILE_STATIC void sendSensorCAN();

/* Rollcall */

FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC void rcPopulate5(CANPacket *out);
FILE_STATIC void rcPopulate6(CANPacket *out);
FILE_STATIC void rcPopulate7(CANPacket *out);
FILE_STATIC void rcPopulate8(CANPacket *out);
FILE_STATIC void rcPopulate9(CANPacket *out);
FILE_STATIC void rcPopulate10(CANPacket *out);
FILE_STATIC void rcPopulate11(CANPacket *out);
FILE_STATIC void rcPopulate12(CANPacket *out);
FILE_STATIC void rcPopulate13(CANPacket *out);
FILE_STATIC void rcPopulate14(CANPacket *out);
FILE_STATIC void rcPopulate15(CANPacket *out);
FILE_STATIC void rcPopulate16(CANPacket *out);
FILE_STATIC void rcPopulate17(CANPacket *out);

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5,
 rcPopulate6, rcPopulate7, rcPopulate8, rcPopulate9, rcPopulate10,
 rcPopulate11, rcPopulate12, rcPopulate13, rcPopulate14, rcPopulate15,
 rcPopulate16, rcPopulate17
};

FILE_STATIC aggVec_i rc_temp;

/* Backchannel */

FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

/* Autcode */

FILE_STATIC flag_t triggerStepFlag = FALSE;
FILE_STATIC void triggerStep();
FILE_STATIC void step();
FILE_STATIC void rt_OneStep();

int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    StartupType starttype = coreStartup(handlePPTFiringNotification, NULL);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, NULL);
#endif  //  __DEBUG__

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize sensors
    initSensorInterfaces();
    asensorInit(Ref_2p5V); // temperature sensor
    aggVec_init_i(&rc_temp);

    // initialize rollcall
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    // initialize autocode
    MSP_SP_initialize();

    /*
     * Prime the sensor readings (initialize the data).
     * This is necessary because the autocode's base rate reads from every
     * sensor at the first update before the subrates' inputs have a chance to
     * read from their sensors.
     */
    updateSensorInterfaces();

    // initialize timer
    initializeTimer();
    int timerHandle = timerCallbackInitializer(&triggerStep,
                                               AUTOCODE_UPDATE_DELAY_US);
    startCallback(timerHandle);

    while (1)
    {
        while (!triggerStepFlag);
        triggerStepFlag = FALSE;
        step();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

FILE_STATIC void triggerStep()
{
    triggerStepFlag = TRUE;
}

FILE_STATIC void step()
{
    // counter to trigger operations that don't happen every step
    static uint16_t i = 0;
    i++;

    // send periodic backchannel telemetry and blink LED
    if (i % 40 == 0) // 10 Hz
    {
        // blink LED
        LED_OUT ^= LED_BIT;

        sendHealthSegment();
        sendMetaSegment();
        sendSensorBackchannel();
        magioSendBackchannelVector();
    }

    rollcallUpdate();

    // step autocode
    if (rtmGetErrorStatus(rtM) != (NULL))
    {
        // TODO handle autocode errors
        hseg.oms = OMS_MajorFaults;
    }
    rt_OneStep();
}

/*
 * Step function originally copied from autocode/ert_main.c, now with inputs
 * and outputs filled out
 */
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
  if (taskCounter[1] == 2) {
    taskCounter[1]= 0;
  }

  taskCounter[2]++;
  if (taskCounter[2] == 4) {
    taskCounter[2]= 0;
  }

  /* Set model inputs associated with base rate here */
  imuioUpdate();

  /* Step the model for base rate */
  /*
   * Though this lists magnetomers and sun sensor as input, it only reads from
   * them at the rate which they are updated (20 Hz, 10 Hz). Because they are
   * read during their subrate steps we can avoid reading from those sensors
   * during the base rate.
   *
   * rate: 40 Hz
   * inputs: imu, mag1, mag2, sun
   * outputs: imu
   */
  MSP_SP_step0();

  /* Get model outputs here */
  // (none)
  // save outputs for the 10 Hz step

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

      /* Step the model for subrate "i" */
      switch (i) {
       case 1 :
        magioUpdate();
        /*
         * rate: 20 Hz
         * inputs: mag
         * outputs: mag
         */
        MSP_SP_step1();

        /* Get model outputs here */
        // (none)
        // save outputs for the 10 Hz step
        break;

       case 2 :
        sunsensorioUpdate();
        /*
         * rate: 10 Hz
         * inputs: sun
         * outputs: sun
         */
        MSP_SP_step2();

        /* Get model outputs here */
        // update them all at the 10 Hz step
        sendSensorCAN();
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

// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
void sendHealthSegment()
{
    // TODO determine overall health
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);

    // update rollcall temperature (in deci-Kelvin)
    aggVec_push_i(&rc_temp, (hseg.inttemp + 273.15f) * 10);
}

void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

FILE_STATIC void initSensorInterfaces()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].init();
    }
}

FILE_STATIC void updateSensorInterfaces()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].update();
    }
}

FILE_STATIC void sendSensorBackchannel()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].sendBackchannel();
    }
}

FILE_STATIC void sendSensorCAN()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].sendCan();
    }
}

void canRxCallback(CANPacket *p)
{
    if (p->id == CAN_ID_CMD_ROLLCALL)
    {
        rollcallStart();
    }
}

void rcPopulate1(CANPacket *out)
{
    rc_adcs_sp_1 rc;
    rc.rc_adcs_sp_1_reset_count = bspGetResetCount();
    rc.rc_adcs_sp_1_sysrstiv = SYSRSTIV;
    rc.rc_adcs_sp_1_temp_avg = aggVec_avg_i_i(&rc_temp);
    rc.rc_adcs_sp_1_temp_max = aggVec_max_i(&rc_temp);
    rc.rc_adcs_sp_1_temp_min = aggVec_min_i(&rc_temp);
    aggVec_reset((aggVec *) &rc_temp);
    encoderc_adcs_sp_1(&rc, out);
}

void rcPopulate2(CANPacket *out)
{
    rc_adcs_sp_2 rc;
    imuioRcPopulate2(&rc);
    encoderc_adcs_sp_2(&rc, out);
}

void rcPopulate3(CANPacket *out)
{
    rc_adcs_sp_3 rc;
    imuioRcPopulate3(&rc);
    encoderc_adcs_sp_3(&rc, out);
}

void rcPopulate4(CANPacket *out)
{
    rc_adcs_sp_4 rc;
    imuioRcPopulate4(&rc);
    sunsensorioRcPopulate4(&rc);
    encoderc_adcs_sp_4(&rc, out);
}

void rcPopulate5(CANPacket *out)
{
    rc_adcs_sp_5 rc;
    sunsensorioRcPopulate5(&rc);
    encoderc_adcs_sp_5(&rc, out);
}

void rcPopulate6(CANPacket *out)
{
    rc_adcs_sp_6 rc;
    sunsensorioRcPopulate6(&rc);
    magioRcPopulate6(&rc);
    encoderc_adcs_sp_6(&rc, out);
}

void rcPopulate7(CANPacket *out)
{
    rc_adcs_sp_7 rc;
    magioRcPopulate7(&rc);
    encoderc_adcs_sp_7(&rc, out);
}

void rcPopulate8(CANPacket *out)
{
    rc_adcs_sp_8 rc;
    magioRcPopulate8(&rc);
    encoderc_adcs_sp_8(&rc, out);
}

void rcPopulate9(CANPacket *out)
{
    rc_adcs_sp_9 rc;
    magioRcPopulate9(&rc);
    encoderc_adcs_sp_9(&rc, out);
}

void rcPopulate10(CANPacket *out)
{
    rc_adcs_sp_10 rc;
    magioRcPopulate10(&rc);
    encoderc_adcs_sp_10(&rc, out);
}

void rcPopulate11(CANPacket *out)
{
    rc_adcs_sp_11 rc;
    magioRcPopulate11(&rc);
    encoderc_adcs_sp_11(&rc, out);
}

void rcPopulate12(CANPacket *out)
{
    rc_adcs_sp_12 rc;
    magioRcPopulate12(&rc);
    encoderc_adcs_sp_12(&rc, out);
}

void rcPopulate13(CANPacket *out)
{
    rc_adcs_sp_13 rc;
    sunsensorioRcPopulate13(&rc);
    magioRcPopulate13(&rc);
    encoderc_adcs_sp_13(&rc, out);
}

void rcPopulate14(CANPacket *out)
{
    rc_adcs_sp_14 rc;
    sunsensorioRcPopulate14(&rc);
    magioRcPopulate14(&rc);
    encoderc_adcs_sp_14(&rc, out);
}

void rcPopulate15(CANPacket *out)
{
    rc_adcs_sp_15 rc;
    imuioRcPopulate15(&rc);
    encoderc_adcs_sp_15(&rc, out);
}

void rcPopulate16(CANPacket *out)
{
    rc_adcs_sp_16 rc;
    imuioRcPopulate16(&rc);
    encoderc_adcs_sp_16(&rc, out);
}

void rcPopulate17(CANPacket *out)
{
    rc_adcs_sp_17 rc;
    imuioRcPopulate17(&rc);
    encoderc_adcs_sp_17(&rc, out);
}
