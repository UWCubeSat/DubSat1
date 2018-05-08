// 40 Hz
#define AUTOCODE_UPDATE_DELAY_US 25000

#include <adcs_sensorproc.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"

#include "adcs_sensorproc_ids.h"
#include "sunsensor_io.h"
#include "mag_io.h"
#include "imu_io.h"

#include "autocode/MSP_SP.h"

/*
 * SensorInterface is a class-like struct for managing multiple sensors on one
 * microcontroller. Each interface should implement an init function and an
 * update function.
 */

typedef void (* sensorio_init_fn)(void);
typedef void (* sensorio_update_fn)(void);
typedef void (* sensorio_sendBc_fn)(void);
typedef void (* sensorio_sendCan_fn)(void);
typedef uint8_t (* sensorio_bc_handler)(uint8_t opcode, uint8_t *cmd);
typedef uint8_t (* sensorio_can_handler)(CANPacket *packet);

typedef struct {
    // initialize the sensor
    sensorio_init_fn init;

    // read from the sensor and set autocode inputs
    sensorio_update_fn update;

    // send raw sensor telemetry over the backchannel (uart)
    sensorio_sendBc_fn sendBackchannel;

    // send autocode outputs over CAN
    sensorio_sendCan_fn sendCan;

    // handle commands from the backchannel
    sensorio_bc_handler handleBackchannel;

    // handle commands from CAN
    sensorio_can_handler handleCan;
} SensorInterface;

FILE_STATIC const SensorInterface sensorInterfaces[] =
{
    {
     sunsensorioInit,
     sunsensorioUpdate,
     sunsensorioSendBackchannel,
     sunsensorioSendCAN,
     NULL,
     NULL,
    },
    {
     magioInit1,
     magioUpdate1,
     magioSendBackchannel1,
     magioSendCAN,
     NULL,
     NULL,
    },
    {
     magioInit2,
     magioUpdate2,
     magioSendBackchannel2,
     NULL,
     NULL,
     NULL,
    },
    {
     imuioInit,
     imuioUpdate,
     imuioSendBackchannel,
     imuioSendCAN,
     NULL,
     NULL,
    },
};

#define NUM_INTERFACES (sizeof(sensorInterfaces) / sizeof(SensorInterface))

// sensor-independent backchannel segment instances
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

FILE_STATIC void initSensorInterfaces();
FILE_STATIC void updateSensorInterfaces();
FILE_STATIC void sendSensorBackchannel();
FILE_STATIC void sendSensorCAN();

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
    StartupType starttype = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                          NULL,
                                          handleDebugActionCallback);
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

    if (i % 4 == 0) // 10 Hz
    {
        // blink LED
        LED_OUT ^= LED_BIT;

        // send a health and meta segments every 1 second
        // TODO move to rollcall when it is implemented
        sendHealthSegment();
        sendMetaSegment();

        // send backchannel telemetry
        sendSensorBackchannel();
        magioSendBackchannelVector();
    }

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
        magioUpdate1();
        magioUpdate2();
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

// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
void handleRollCall()
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

    // send CAN packet of temperature (in deci-Kelvin)
    msp_temp temp = { (hseg.inttemp + 273.15f) * 10 };
    CANPacket packet;
    encodemsp_temp(&temp, &packet);
    canSendPacket(&packet);
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
        if (sensorInterfaces[i].sendBackchannel)
        {
            sensorInterfaces[i].sendBackchannel();
        }
    }
}

FILE_STATIC void sendSensorCAN()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        if (sensorInterfaces[i].sendCan)
        {
            sensorInterfaces[i].sendCan();
        }
    }
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_BinaryStreaming)
    {
        // offer the command to each sensor interface for handling
        uint8_t i = NUM_INTERFACES;
        while (i-- != 0)
        {
            sensorio_bc_handler cmdHandler = sensorInterfaces[i].handleBackchannel;

            // if the interface has a command handler, use it
            if (cmdHandler)
            {
                // if the command was handled, stop searching and return
                if (cmdHandler(cmdstr[0], cmdstr + 1))
                {
                    return 1;
                }
            }
        }

        // the command was not handled by an sensor interface -- let the board
        // handle it.
        switch(cmdstr[0])
        {
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
}

void canRxCallback(CANPacket *packet)
{
    // send the packet to each sensor interface
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorio_can_handler canHandler = sensorInterfaces[i].handleCan;
        if (canHandler)
        {
            canHandler(packet);
        }
    }
}
