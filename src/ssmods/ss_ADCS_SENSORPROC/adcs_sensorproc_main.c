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
	 NULL,
    },
#if ENABLE_MAG1
    {
     magioInit1,
     magioUpdate1,
     magioSendBackchannel1,
     magioSendCAN1,
     NULL,
    },
#endif /* ENABLE_MAG1 */
#if ENABLE_MAG2
    {
     magioInit2,
     magioUpdate2,
     magioSendBackchannel2,
     magioSendCAN2,
     NULL,
    },
#endif /* ENABLE_MAG2 */
    {
     imuioInit,
     imuioUpdate,
     imuioSendBackchannel,
     imuioSendCAN,
	 NULL,
    },
};

#define NUM_INTERFACES (sizeof(sensorInterfaces) / sizeof(SensorInterface))

/* Rollcall */

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5,
 rcPopulate6, rcPopulate9, rcPopulate10, rcPopulate11, rcPopulate12,
 rcPopulate13, rcPopulate14, rcPopulate15, rcPopulate16, rcPopulate17
};

FILE_STATIC aggVec_f rc_temp;

/* Backchannel */

FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

/* Autocode */

FILE_STATIC flag_t triggerStepFlag = FALSE;
/**
 * Takes one step of autocode.
 */
FILE_STATIC void rt_OneStep();

int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    StartupType starttype = coreStartup(NULL, NULL);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, NULL);
#endif  //  __DEBUG__

    LED_DIR |= LED_BIT;

    // Fix phantom power over uart
    P3DIR |= BIT4;
    P3OUT |= BIT4;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

    // initialize sensors
	initSensorInterfaces();
	asensorInit(Ref_2p5V); // temperature sensor
	aggVec_init_f(&rc_temp);

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize rollcall
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    // initialize autocode
    MSP_SP_initialize();

    __delay_cycles(SEC * 1); //this wait is necessary for the sun sensor to start up

    /*
     * Prime the sensor readings (initialize the data).
     * This is necessary because the autocode's base rate reads from every
     * sensor at the first update before the subrates' inputs have a chance to
     * read from their sensors.
     */
    updateSensorInterfaces(); //this function is the one causing the hanging issues with the sun sensor


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

void triggerStep()
{
    triggerStepFlag = TRUE;
}

void step()
{
    // counter to trigger operations that don't happen every step
    static uint16_t i = 0;
    i++;

    // send periodic backchannel telemetry and blink LED
    if (i % 4 == 0) // 10 Hz
    {
        // blink LED
        LED_OUT ^= LED_BIT;

        if (i % 40 == 0) // 1 Hz
        {
        	sendHealthSegment();
			sendMetaSegment();
        }

        sendSensorBackchannel();
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
FILE_STATIC void rt_OneStep(void)
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
#if ENABLE_MAG1
        magioUpdate1();
#endif /* ENABLE_MAG1 */
#if ENABLE_MAG2
        magioUpdate2();
#endif /* ENABLE_MAG2 */
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

// Packetizes and sends backchannel health packet
// also invokes uart status handler
void sendHealthSegment()
{
    // TODO determine overall health
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);

    // update rollcall temperature
    aggVec_push_f(&rc_temp, hseg.inttemp);
}

void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

void initSensorInterfaces()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].init();
    }
}

void updateSensorInterfaces()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].update();
    }
}

void sendSensorBackchannel()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].sendBackchannel();
    }
}

void sendSensorCAN()
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
    else if(p->id == CAN_ID_MTQ_ACK) {
    	mtq_ack ackpacket;
    	decodemtq_ack(p,&ackpacket);
    	magioUpdatePhase(ackpacket.mtq_ack_phase);
    }
    else if(p->id == CAN_ID_GCMD_RESET_MINMAX)
    {
        gcmd_reset_minmax pktRst;
        decodegcmd_reset_minmax(p, &pktRst);
        if(pktRst.gcmd_reset_minmax_sensorproc)
            aggVec_reset((aggVec *)&rc_temp);
    }
    else if(p->id == CAN_ID_GCMD_DIST_RESET_MISSION)
    {
        //clear persistent flags here
        bspClearResetCount();
    }
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
    	if (sensorInterfaces[i].handleCan)
    	{
        	sensorInterfaces[i].handleCan(p);
    	}
    }
}

void rcPopulate1(CANPacket *out)
{
    rc_adcs_sp_h1 rc;
    rc.rc_adcs_sp_h1_reset_count = bspGetResetCount();
    rc.rc_adcs_sp_h1_sysrstiv = SYSRSTIV;
    rc.rc_adcs_sp_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&rc_temp));
    rc.rc_adcs_sp_h1_temp_max = compressMSPTemp(aggVec_max_f(&rc_temp));
    rc.rc_adcs_sp_h1_temp_min = compressMSPTemp(aggVec_min_f(&rc_temp));
    aggVec_as_reset((aggVec *) &rc_temp);
    encoderc_adcs_sp_h1(&rc, out);
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
    rc_adcs_sp_6 rc = { 0 };
    sunsensorioRcPopulate6(&rc);
#if ENABLE_MAG1
    magio1RcPopulate6(&rc);
#endif
#if ENABLE_MAG2
    magio2RcPopulate6(&rc);
#endif
    encoderc_adcs_sp_6(&rc, out);
}

void rcPopulate9(CANPacket *out)
{
    rc_adcs_sp_9 rc = { 0 };
#if ENABLE_MAG1
    magio1RcPopulate9(&rc);
#endif
    encoderc_adcs_sp_9(&rc, out);
}

void rcPopulate10(CANPacket *out)
{
    rc_adcs_sp_10 rc = { 0 };
#if ENABLE_MAG1
    magio1RcPopulate10(&rc);
#endif
    encoderc_adcs_sp_10(&rc, out);
}

void rcPopulate11(CANPacket *out)
{
    rc_adcs_sp_11 rc = { 0 };
#if ENABLE_MAG2
    magio2RcPopulate11(&rc);
#endif
    encoderc_adcs_sp_11(&rc, out);
}

void rcPopulate12(CANPacket *out)
{
    rc_adcs_sp_12 rc = { 0 };
#if ENABLE_MAG2
    magio2RcPopulate12(&rc);
#endif
    encoderc_adcs_sp_12(&rc, out);
}

void rcPopulate13(CANPacket *out)
{
    rc_adcs_sp_13 rc = { 0 };
    sunsensorioRcPopulate13(&rc);
#if ENABLE_MAG2
    magio2RcPopulate13(&rc);
#endif
    encoderc_adcs_sp_13(&rc, out);
}

void rcPopulate14(CANPacket *out)
{
    rc_adcs_sp_14 rc;
    sunsensorioRcPopulate14(&rc);
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
