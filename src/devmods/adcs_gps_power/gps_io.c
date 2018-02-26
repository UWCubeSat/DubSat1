/*
 * gps_io.c
 *
 *  Created on: Feb 11, 2018
 *      Author: djdup
 */

#include <math.h>

#include "gps_io.h"
#include "adcs_sensorproc_ids.h"
#include "core/debugtools.h"
#include "core/utils.h"
#include "interfaces/canwrap.h"

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC gpshealth_segment gpshealthSeg;
FILE_STATIC gpspower_segment gpspowerSeg;

FILE_STATIC volatile gps_power_state gpsPowerState = State_GPSOff;

FILE_STATIC flag_t triggerGPSOn;
FILE_STATIC flag_t triggerGPSOff;

void gpsioInit()
{
    bcbinPopulateHeader(&gpshealthSeg.header, TLM_ID_GPSHEALTH, sizeof(gpshealthSeg));
    bcbinPopulateHeader(&gpspowerSeg.header, TLM_ID_GPSPOWER, sizeof(gpspowerSeg));

    triggerGPSOn = FALSE;
    triggerGPSOff = FALSE;

    gpsInit();
}

void gpsioUpdate()
{
    switch (gpsPowerState)
    {
    case State_GPSOff:
        if (triggerGPSOff)
        {
            triggerGPSOff = FALSE;
        }
        else if (triggerGPSOn)
        {
            // enable the buck converter
            triggerGPSOn = FALSE;
            gpsBuckOn();
            gpsPowerState = State_BuckWaitOn;
        }
        break;
    case State_BuckWaitOn:
        if (triggerGPSOff)
        {
            // abort boot and wait for buck converter to turn off
            triggerGPSOff = FALSE;
            gpsPowerState = State_BuckWaitOff;
        }
        else if (triggerGPSOn)
        {
            triggerGPSOn = FALSE;
        }
        else if (gpsBuckGood())
        {
            // buck converter signal is good, enable the GPS
            gpsPowerOn();
            gpsPowerState = State_GPSWait;
        }
        break;
    case State_GPSWait:
        if (triggerGPSOff)
        {
            // abort boot and turn GPS switch off
            triggerGPSOff = FALSE;
            gpsPowerOff();
            gpsPowerState = State_BuckWaitOff;
        }
        else if (triggerGPSOn)
        {
            triggerGPSOn = FALSE;
        }
        else {
            static uint32_t i = 0;
            i++;
            if (i > 50000)
            {
                gpsPowerState = State_GPSOn;
                i = 0;
            }
        }
        break;
    case State_GPSOn:
        // TODO add state transition for !gpsBuckGood()?
        if (triggerGPSOff)
        {
            // switch the GPS off
            triggerGPSOff = FALSE;
            gpsPowerOff();
            gpsPowerState = State_BuckWaitOff;
        }
        else if (triggerGPSOn)
        {
            triggerGPSOn = FALSE;
        }
        break;
    case State_BuckWaitOff:
    {
        static uint16_t count = 0;

        if (triggerGPSOff)
        {
            triggerGPSOff = FALSE;
        }
        else if (triggerGPSOn)
        {
            // abort shutdown and turn the GPS switch back on
            triggerGPSOn = FALSE;
            gpsPowerOn();
            gpsPowerState = State_GPSWait;
        }
        else if (count++ > 50000)
        {
            // wait a while before disabling the buck converter
            gpsBuckOff();
            gpsPowerState = State_GPSOff;
            count = 0;
        }
        break;
    }
    default:
        break;
    }
}

void gpsioPowerOn()
{
    triggerGPSOn = TRUE;
    triggerGPSOff = FALSE;
}

void gpsioPowerOff()
{
    triggerGPSOn = FALSE;
    triggerGPSOff = TRUE;
}

void gpsioSendPowerStatus()
{
    gpspowerSeg.buckEnabled = gpsBuckEnabled();
    gpspowerSeg.gpsEnabled = gpsPowerEnabled();
    gpspowerSeg.state = gpsPowerState;
    bcbinSendPacket((uint8_t *) &gpspowerSeg, sizeof(gpspowerSeg));
}

void gpsioSendHealth()
{
    gpsGetHealth(&gpshealthSeg.health);
    bcbinSendPacket((uint8_t *) &gpshealthSeg, sizeof(gpshealthSeg));
}

bool gpsioHandleCommand(uint8_t *cmdstr)
{
    enable_segment *enableSegment;

    switch(cmdstr[0])
    {
        case OPCODE_ENABLE:
            enableSegment = (enable_segment *) (cmdstr + 1);

            // set enable/disable trigger and overwrite previous command
            if (enableSegment->enable)
            {
                triggerGPSOn = TRUE;
                triggerGPSOff = FALSE;
            }
            else
            {
                triggerGPSOn = FALSE;
                triggerGPSOff = TRUE;
            }
            break;
        default:
            return 0;
    }
    return 1;
}
