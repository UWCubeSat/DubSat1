#define ENABLE_GPS         0
#define ENABLE_PHOTODIODES 0
#define ENABLE_SUNSENSOR   1
#define ENABLE_MAG1        1
#define ENABLE_MAG2        1
#define ENABLE_IMU         1

#include <adcs_sensorproc.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"

#include "adcs_sensorproc_ids.h"
#include "gps_io.h"
#include "sunsensor_io.h"
#include "photodiode_io.h"
#include "mag_io.h"
#include "imu_io.h"

/*
 * SensorInterface is a class-like struct for managing multiple (3 to 5) sensors
 * on one microcontroller. Each interface should implement an init function, an
 * update function, a backchannel command handler (optional), and a CAN packet
 * handler (optional). Sensor outputs can be sent during the update function.
 */

typedef void (* sensorio_init_fn)(void);
typedef void (* sensorio_update_fn)(void);
typedef uint8_t (* sensorio_cmd_handler)(uint8_t opcode, uint8_t *cmd);
typedef uint8_t (* sensorio_can_handler)(CANPacket *packet);

/*
 * Select an update rate for the sensor interface. If none of these fit, use
 * UpdateRate_ASAP and use a timer in the update function
 */
typedef enum {
    UpdateRate_ASAP,
    UpdateRate_5Hz,
    UpdateRate_1Hz,
} UpdateRate;

typedef struct {
    sensorio_init_fn init;
    sensorio_update_fn update;
    UpdateRate updateRate;
    sensorio_cmd_handler handleCmd;
    sensorio_can_handler handleCan;
} SensorInterface;

FILE_STATIC const SensorInterface sensorInterfaces[] =
{
#if ENABLE_GPS
    {
     gpsioInit,
     gpsioUpdate,
     UpdateRate_ASAP
     gpsioHandleCommand,
     gpsioHandleCan,
    },
#endif
#if ENABLE_SUNSENSOR
    {
     sunsensorioInit,
     sunsensorioUpdate,
     UpdateRate_5Hz,
     NULL,
     NULL,
    },
#endif
#if ENABLE_PHOTODIODES
    {
     photodiodeioInit,
     photodiodeioUpdate,
     UpdateRate_5Hz,
     NULL,
     NULL,
    },
#endif
#if ENABLE_MAG1
    {
     magioInit1,
     magioUpdate1,
     UpdateRate_5Hz,
     NULL,
     NULL,
    },
#endif
#if ENABLE_MAG2
    {
     magioInit2,
     magioUpdate2,
     UpdateRate_5Hz,
     NULL,
     NULL,
    },
#endif
#if ENABLE_IMU
    {
     imuioInit,
     imuioUpdate,
     UpdateRate_5Hz,
     NULL,
     NULL,
    },
#endif
};

#define NUM_INTERFACES (sizeof(sensorInterfaces) / sizeof(SensorInterface))

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

FILE_STATIC int timerHandle;
FILE_STATIC void start5HzTimer();

FILE_STATIC void initSensorInterfaces();
FILE_STATIC void updateSensorInterfaces(UpdateRate rate);

int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    asensorInit(Ref_2p5V);

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

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize timer
    initializeTimer();
    start5HzTimer();

    // initialize sensors
    initSensorInterfaces();

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");

    uint8_t num5HzUpdatesSince1HzUpdate = 0;
    uint8_t secondsSince0p1HzUpdate = 0;
    while (1)
    {
        // make ASAP updates
        updateSensorInterfaces(UpdateRate_ASAP);

        if (checkTimer(timerHandle))
        {
            start5HzTimer();

            // make 5 Hz updates
            LED_OUT ^= LED_BIT;
            updateSensorInterfaces(UpdateRate_5Hz);
            num5HzUpdatesSince1HzUpdate++;

            // make 1 Hz updates
            if (num5HzUpdatesSince1HzUpdate == 5)
            {
                updateSensorInterfaces(UpdateRate_1Hz);
                sendHealthSegment();

                num5HzUpdatesSince1HzUpdate = 0;
                secondsSince0p1HzUpdate++;
            }

            // make 0.1 Hz updates
            if (secondsSince0p1HzUpdate == 10)
            {
                sendMetaSegment();

                secondsSince0p1HzUpdate = 0;
            }
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

FILE_STATIC void initSensorInterfaces()
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        sensorInterfaces[i].init();
    }
}

FILE_STATIC void updateSensorInterfaces(UpdateRate rate)
{
    uint8_t i = NUM_INTERFACES;
    while (i-- != 0)
    {
        if (sensorInterfaces[i].updateRate == rate)
        {
            sensorInterfaces[i].update();
        }
    }
}

void start5HzTimer()
{
    timerHandle = timerPollInitializer(200);
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
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // send gps-specific health segment
    gpsioSendHealth();

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

void sendMetaSegment()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_BinaryStreaming)
    {
        // offer the command to each sensor interface for handling
        uint8_t i = NUM_INTERFACES;
        while (i-- != 0)
        {
            sensorio_cmd_handler cmdHandler = sensorInterfaces[i].handleCmd;

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
