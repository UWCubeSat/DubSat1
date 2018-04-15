#define ENABLE_SUNSENSOR   0
#define ENABLE_MAG1        0
#define ENABLE_MAG2        1
#define ENABLE_IMU         0

#define AUTOCODE_UPDATE_DELAY_US 100000

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
#if ENABLE_SUNSENSOR
    {
     sunsensorioInit,
     sunsensorioUpdate,
     sunsensorioSendBackchannel,
     sunsensorioSendCAN,
     NULL,
     NULL,
    },
#endif
#if ENABLE_MAG1
    {
     magioInit1,
     magioUpdate1,
     magioSendBackchannel1,
     magioSendCAN1,
     NULL,
     NULL,
    },
#endif
#if ENABLE_MAG2
    {
     magioInit2,
     magioUpdate2,
     magioSendBackchannel2,
     magioSendCAN2,
     NULL,
     NULL,
    },
#endif
#if ENABLE_IMU
    {
     imuioInit,
     imuioUpdate,
     imuioSendBackchannel,
     imuioSendCAN,
     NULL,
     NULL,
    },
#endif
};

#define NUM_INTERFACES (sizeof(sensorInterfaces) / sizeof(SensorInterface))

// sensor-independent backchannel segment instances
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

FILE_STATIC void initSensorInterfaces();
FILE_STATIC void updateSensorInterfaces();
FILE_STATIC void sendSensorBackchannel();
FILE_STATIC void sendSensorCAN();
FILE_STATIC void step();

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

    // initialize timer
    initializeTimer();
    int timerHandle = timerCallbackInitializer(&step, AUTOCODE_UPDATE_DELAY_US);
    startCallback(timerHandle);

    /*
     * While loop is empty because all update code is in the step function.
     */
    while (1);

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

FILE_STATIC void step()
{
    // counter to trigger operations that don't happen every step
    static uint16_t i = 0;
    i++;

    // blink LED
    LED_OUT ^= LED_BIT;

    // send a health and meta segments every 1 second
    // TODO move to rollcall when it is implemented
    if (i % 10 == 0)
    {
        sendHealthSegment();
        sendMetaSegment();
    }

    // read sensors and set autocode inputs
    updateSensorInterfaces();

    // step autocode
    // TODO handle autocode errors
    MSP_SP_step();

    // send autocode outputs over CAN and/or backchannel
    sendSensorCAN();
    sendSensorBackchannel();
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

    // TODO determine overall health based on querying sensors for their health
    hseg.oms = OMS_Unknown;

//    hseg.inttemp = asensorReadIntTempC();
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
