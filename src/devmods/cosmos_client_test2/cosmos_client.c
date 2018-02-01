#include <msp430.h> 

#include "cosmos_client.h"
#include "bsp/bsp.h"

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC my_telem myTelemSeg;

/**
 * Populates an example telemetry segment
 */
FILE_STATIC void readMySensor()
{
    myTelemSeg.alpha = 123.0;
    myTelemSeg.beta = 456.0;
}

/**
 * Sends the example telemetry segment over the backchannel
 */
FILE_STATIC void sendMySensorTelem()
{
    bcbinSendPacket((uint8_t *) &myTelemSeg, sizeof(myTelemSeg));
}

/**
 * Populates and sends the meta segment over the backchannel.
 */
FILE_STATIC void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

/**
 * Populates and sends health segments over the backchannel.
 */
FILE_STATIC void sendHealthSegment()
{
    // Populate and send the health segment for this entity
    hseg.inttemp = asensorReadIntTempC();
    hseg.oms = OMS_Unknown;
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));

    // Also invoke the status handler for UART to send its health segments
    debugInvokeStatusHandler(Entity_UART);
}

int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);

    // init the MSP's sensor to read internal temperature
    asensorInit(Ref_2p5V);

    // register an action callback so we can take commands over the backchannel
    debugRegisterEntity(Entity_Test, NULL, NULL, handleDebugActionCallback);

    /*
     * Set up segments to send COSMOS telemetry packets. The meta segment is
     * omitted because its header is populated along with the rest of it in the
     * "bcbinPopulateMeta" function.
     */
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemSeg.header, TLM_ID_MY_TELEM, sizeof(myTelemSeg));

    while (1)
    {
        // read imaginary hardware
        readMySensor();

        // send telemetry
        sendMetaSegment();
        sendHealthSegment();
        sendMySensorTelem();

        __delay_cycles(0.5 * SEC);
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // handle actions in a user-friendly way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // handle actions in a low-output way
    }
    else if (mode == Mode_BinaryStreaming)
    {
        /*
         * Handle actions, any output should be ground-segment friendly "packet"
         * format. This is the only mode we care about for demonstrating COSMOS.
         */

        // cmdstr[0] represents the opcode. Everything after is the payload.
        enable_segment *enableSegment;
        switch(cmdstr[0])
        {
            case OPCODE_MY_CMD:
                // cast the payload to our command segment
                enableSegment = (enable_segment *) (cmdstr + 1);
                if (enableSegment->enable)
                {
                    myTelemSeg.status = MY_TELEM_ENABLED;
                }
                else
                {
                    myTelemSeg.status = MY_TELEM_DISABLED;
                }
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
}
