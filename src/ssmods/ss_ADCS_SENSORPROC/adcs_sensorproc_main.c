#include <adcs_sensorproc.h>
#include <msp430.h> 

#include <math.h>

#include "bsp/bsp.h"
#include "core/debugtools.h"
#include "interfaces/canwrap.h"

#include "sensors/sun_sensor.h"
#include "sensors/photodiode.h"

#include "gps_io.h"
#include "adcs_sensorproc_ids.h"

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC sunsensor_segment sunsensorSeg;
FILE_STATIC photodiode_segment photodiodeSeg;

// photodiode handles
FILE_STATIC uint8_t pdCenter;
FILE_STATIC uint8_t pdRight;
FILE_STATIC uint8_t pdLeft;

int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    asensorInit(Ref_2p5V);

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&sunsensorSeg.header, TLM_ID_SUNSENSOR, sizeof(sunsensorSeg));
    bcbinPopulateHeader(&photodiodeSeg.header, TLM_ID_PHOTODIODE, sizeof(photodiodeSeg));

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters
    canWrapInit();

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // initialize sensors
    gpsioInit();
    sunSensorInit(I2C_BUS_SUNSENSOR); // TODO prevent hanging here
//    photodiodeInitAll();

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
    while (1)
    {
        static uint32_t i = 0;
        i++;

        if (i % 65536 == 0)
        {
            LED_OUT ^= LED_BIT;
            gpsioSendPowerStatus();
            sendHealthSegment();
        }

        if (i % 524288 == 0)
        {
            sendMetaSegment();
        }

        if (i % 32768 == 0)
        {
            /*
             * TODO assert that the photodiodes are not being read multiple
             * times in the space of PHOTODIODE_DELAY_S.
             */
//            photodiodeUpdate();
            sunSensorUpdate(); // TODO don't let these block

            sendPhotodiodeData();
            sendSunSensorData();
        }

        gpsioUpdate();
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

// read the sun sensor and update its segments
void sunSensorUpdate()
{
    SunSensorAngle *angle = sunSensorReadAngle();
    if (angle != NULLP)
    {
        sunsensorSeg.alpha = angle->alpha;
        sunsensorSeg.beta = angle->beta;
        sunsensorSeg.error = angle->error;
    }
    else
    {
        // TODO log read error
    }
}

void sendSunSensorData()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &sunsensorSeg, sizeof(sunsensorSeg));

    // send CAN packet
    // converting degrees to arcminutes
    sensorproc_sun sun = { sunsensorSeg.error,
                           (int32_t) round(sunsensorSeg.alpha * 60),
                           (int32_t) round(sunsensorSeg.beta * 60) };
    CANPacket packet;
    encodesensorproc_sun(&sun, &packet);
    canSendPacket(&packet);
}

void photodiodeInitAll()
{
    pdCenter = photodiodeInit(PD_ADDR_HH, I2C_BUS_PHOTODIODES);
    pdRight = photodiodeInit(PD_ADDR_FF, I2C_BUS_PHOTODIODES);
    pdLeft = photodiodeInit(PD_ADDR_HF, I2C_BUS_PHOTODIODES);
}

void photodiodeUpdate()
{
    photodiodeSeg.center = photodiodeRead(defaultWrite, pdCenter);
    photodiodeSeg.right = photodiodeRead(defaultWrite, pdRight);
    photodiodeSeg.left = photodiodeRead(defaultWrite, pdLeft);
}

void sendPhotodiodeData()
{
    // send backchannel telemetry
    bcbinSendPacket((uint8_t *) &photodiodeSeg, sizeof(photodiodeSeg));

    // send CAN packet
    // TODO verify the order on these is correct
    // TODO how are these supposed to fit in a uint8_t?
    sensorproc_photodiode pd = { (uint8_t) photodiodeSeg.center,
                                 (uint8_t) photodiodeSeg.right,
                                 (uint8_t) photodiodeSeg.left };
    CANPacket packet;
    encodesensorproc_photodiode(&pd, &packet);
    canSendPacket(&packet);
}
