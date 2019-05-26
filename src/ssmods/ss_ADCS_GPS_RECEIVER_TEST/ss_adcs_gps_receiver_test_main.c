// time to wait between LED blink
#define UPDATE_DELAY_MS 2000
#include <ss_adcs_gps_receiver_test.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "core/timer.h"
#include "interfaces/canwrap.h"

#include "adcs_sensorproc_ids.h"
#include "gps_io.h"

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

FILE_STATIC int timerHandle;
FILE_STATIC void startSensorprocTimer();

FILE_STATIC GPSDegMin gpsDM;

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
    // TODO:  Add the correct bus filters
    canWrapInit();

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize timers
    initializeTimer();
    startSensorprocTimer();

    // initialize the reader
    gpsioInit_Receiver();

    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;

    while (1)
    {
        if (checkTimer(timerHandle)) {
            // send data using uart
            sendGPSDMUart(&gpsDM);

            LED_OUT ^= LED_BIT;     // blink LED
            P1OUT ^= BIT0;
            startSensorprocTimer();
        }
        getGPSDM(&gpsDM);
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

    return 0;
}

void startSensorprocTimer()
{
    timerHandle = timerPollInitializer(UPDATE_DELAY_MS);
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
        // if the command is handled by the GPS, return
        if (gpsioHandleCommand(cmdstr))
        {
            return 1;
        }

        // handle non-gps commands
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


