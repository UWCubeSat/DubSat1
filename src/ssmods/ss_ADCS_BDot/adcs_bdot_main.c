#include <adcs_bdot.h>
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/dataArray.h"
#include "core/agglib.h"


/******************COSMOS Telemetry******************************/
FILE_STATIC health_segment hseg;
FILE_STATIC magnetometer_segment myTelemMagnetometer;
FILE_STATIC mtq_info_segment myTelemMtqInfo;
FILE_STATIC simulink_segment mySimulink;
/****************************************************************/


/************ MTQ Info From Bdot Perspective*********************/
FILE_STATIC uint8_t mtq_state = MTQ_MEASUREMENT_PHASE;
FILE_STATIC mtq_info mtqInfo;
FILE_STATIC mtq_info lastKnownState;
FILE_STATIC volatile magDataStatus mag_data = mag_invalid;
/****************************************************************/


/****************Magnetometer Variables*************************/
FILE_STATIC uint8_t MagNormalOperation = 1;
/***************************************************************/

/******************Simulink Flags and Information***************/
FILE_STATIC uint8_t send_dipole_flag = 0;
FILE_STATIC uint8_t update_rt_flag = 0;
/***************************************************************/

/******************Timer Information***************************/
FILE_STATIC TIMER_HANDLE rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;
/***************************************************************/

/*******************RollCall***********************************/
/***************************************************************/

/*******************Miscellaneous*******************************/
FILE_STATIC ModuleStatus mod_status;
/***************************************************************/
uint8_t receievedquestionmark =0;
FILE_STATIC uint64_t received=0;

void reverseArrah(uint8_t arr[], uint8_t start, uint8_t end) {
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArrah(arr, start+1, end-1);
}


int main(void)
{

    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
    // as a predefined symbol
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)

    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
    //                               handleDebugStatusCallback,
    //                               handleDebugActionCallback);

#endif  //  __DEBUG__

    debugTraceF(1, "CAN message bus configured.\r\n");
    debugTraceF(1, "Commencing subsystem module execution ...\r\n");


    initial_setup();
    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
    startCallback(rtOneStep_timer);

    fflush((NULL));
            uint8_t errorcount=0;
    while (1)
    {
        if(receievedquestionmark){
            received++;
            errorcount += canRxErrorCheck();
            CANPacket out;
            CANPacket* output = &out;
            output -> id = 304677443;
            output -> length = 8;
            uint64_t fullPacketData = 0x0000000000000000;
            fullPacketData = ((uint64_t)received) ;
            fullPacketData = (((uint64_t)received) | ((uint64_t)errorcount <<56)) ;
            uint64_t *thePointer = (uint64_t *) (&(output -> data));
            *thePointer = fullPacketData;
            reverseArrah((output->data), 0, 7);
            canSendPacket(output);
            receievedquestionmark=0;
        }
        if(update_rt_flag)
        {
            PJDIR |= BIT0;
            PJOUT ^= BIT0;
            //deteled this and worked
            rt_OneStep();
            updateMtqInfo();
            sendTelemetry();
            updateRCData();
            //end deletion
            update_rt_flag = 0;
        }

    }

    bdot_controller_lib_terminate();

    return 0;
}


void initial_setup()
{
    P3DIR |= BIT5;
    P3OUT |= BIT5;

    canWrapInit();
    setCANPacketRxCallback(receive_packet);


    asensorInit(Ref_2p5V);


    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));


    initializeTimer();
}

void getMagnetometerData()
{
}

void performSelfTest()
{

}

void performNormalOp()
{
}

void simulink_compute()
{
    update_rt_flag = 1;
    send_dipole_flag = 1;
}


void sendTelemetry()
{
    sendHealthSegment();
    sendMagReadingSegment();
    sendMtqInfoSegment();
    sendSimulinkSegment();
}
int funkyDipolesX[20] = {
                        12,53,-65,412,24,
                        -25,68,-26,89,43,
                        62,-56,25,20,-13,
                        -12,72,87,2,15
};
int funkyDipolesY[20] = {
                        12,72,65,-42,24,
                        25,68,26,21,43,
                        -54,44,-32,20,1,
                        43,72,87,2,17
};
int funkyDipolesZ[20] = {
                        19,53,65,-28,24,
                        25,68,-26,89,61,
                        62,56,24,39,13,
                        12,-72,26,-2,-31
};
int funkyCounter=0;
void updateMtqInfo()
{
    funkyCounter++;
    mtqInfo.tumble_status  = 1;
    if(mtqInfo.tumble_status)
    {
        mtqInfo.xDipole = funkyDipolesX[funkyCounter];
        mtqInfo.yDipole = funkyDipolesY[funkyCounter];
        mtqInfo.zDipole = funkyDipolesZ[funkyCounter];
    } else
    {
        mtqInfo.xDipole = 0;
        mtqInfo.yDipole = 0;
        mtqInfo.zDipole = 0;
    }
}

void sendHealthSegment()
{
    // Populate and send the health segment for this entity
    hseg.inttemp = asensorReadIntTempC();
    hseg.oms = OMS_Unknown;
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));

    // Also invoke the status handler for UART to send its health segments
    debugInvokeStatusHandler(Entity_UART);

}


void sendMagReadingSegment()
{
    myTelemMagnetometer.xMag = 0;
    myTelemMagnetometer.yMag = 0;
    myTelemMagnetometer.zMag = 0;
    myTelemMagnetometer.tempMag = 0;

    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));
}

void sendMtqInfoSegment()
{
    myTelemMtqInfo.xDipole = lastKnownState.xDipole;
    myTelemMtqInfo.yDipole = lastKnownState.yDipole;
    myTelemMtqInfo.zDipole = lastKnownState.zDipole;

    bcbinSendPacket((uint8_t *) &myTelemMtqInfo, sizeof(myTelemMtqInfo));
}


void sendSimulinkSegment()
{
    mySimulink.tumble = 1;
    mySimulink.sim_xDipole = 1;
    mySimulink.sim_yDipole = 1;
    mySimulink.sim_zDipole = 1;
    bcbinSendPacket((uint8_t *) &mySimulink, sizeof(mySimulink));
}

void sendDipolePacket(int8_t x, int8_t y, int8_t z)
{
    CANPacket dipole_packet = {0};
    cmd_mtq_bdot dipole_info = {0};

    dipole_info.cmd_mtq_bdot_x = x;
    dipole_info.cmd_mtq_bdot_y = y;
    dipole_info.cmd_mtq_bdot_z = z;

    encodecmd_mtq_bdot(&dipole_info, &dipole_packet);
    canSendPacket(&dipole_packet);
}

void sendTumblePacket(uint8_t status)
{
    CANPacket tumble_packet = {0};
    bdot_tumble_status tumble_info = {0};

    tumble_info.bdot_tumble_status_status = status;

    encodebdot_tumble_status(&tumble_info, &tumble_packet);
    canSendPacket(&tumble_packet);
}

int map(int val)
{
    return mapGeneral(val, -127, 127, -100, 100);
}

int mapGeneral(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*No op function */

void receive_packet(CANPacket *packet)
{
    if(packet->id == CAN_ID_MTQ_ACK)
    {
        // measurement_phase = 0;
        // actuation_phase = 1;
        mtq_ack ack = {0};
        decodemtq_ack(packet, &ack);
        if(!ack.mtq_ack_phase)
        {
            mtq_state = MTQ_MEASUREMENT_PHASE;
        } else
        {
            mtq_state = MTQ_ACTUATION_PHASE;
        }
        receievedquestionmark = 1;
    }
    if(packet->id == CAN_ID_CMD_ROLLCALL)
    {
    }
}


void updateRCData()
{
}


void rollCall()
{
}

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(void)
{
//  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
//  if (OverrunFlag) {
//    rtmSetErrorStatus(rtM, "Overrun");
//    return;
//  }

//  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  bdot_controller_lib_step();

  /* Get model outputs here */

  /* Indicate task complete */
//  OverrunFlag = false;

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


//void start_telem_timer()
//{
//    telem_timer = timerPollInitializer(telem_timer_ms);
//}

//FILE_STATIC int telem_timer;
//FILE_STATIC uint16_t telem_timer_ms = 200; // 1 s
