#define TLM_ID_RAW 2
#define DELAY_MS 500

// Debug LED
#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#include "bsp/bsp.h"

#include "core/debugtools.h"
#include "core/i2c.h"
#include "core/timer.h"

#include "sensors/imu.h"

TLM_SEGMENT {
    BcTlmHeader header; // All COSMOS TLM packets must have this

    uint16_t x;
    uint16_t y;
    uint16_t z;
} imuraw_segment;

// Segment instances
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC imuraw_segment rawSeg;

int timerHandle;

FILE_STATIC void resetTimer()
{
    timerHandle = timerPollInitializer(DELAY_MS);
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
void sendHealthSegment()
{
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);
}

FILE_STATIC void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

    asensorInit(Ref_2p5V);
    LED_DIR |= LED_BIT;
    LED_OUT |= LED_BIT;

    // register an action callback so we can take commands over the backchannel
    debugRegisterEntity(Entity_Test, NULL, NULL, NULL);

    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&rawSeg.header, TLM_ID_RAW, sizeof(rawSeg));

    initializeTimer();
    resetTimer();

//    imuInit(I2CBus1);

    IMUData *data;
    uint8_t i = 0;
    while (1)
    {
        if (checkTimer(timerHandle))
        {
            LED_OUT ^= LED_BIT;

            if (i++ == 0)
            {
                sendMetaSegment();
            }

//            data = imuReadGyroAccelData();
//
//            rawSeg.x = data->rawGyroX;
//            rawSeg.y = data->rawGyroY;
//            rawSeg.z = data->rawGyroZ;
//            bcbinSendPacket((uint8_t *) &rawSeg, sizeof(rawSeg));
            sendHealthSegment();

            resetTimer();
        }
    }

    return 0;
}
