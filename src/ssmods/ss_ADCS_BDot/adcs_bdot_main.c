#include <adcs_bdot.h>
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "sensors/magnetometer.h"
#include "core/timer.h"
#include "bdot_controller_lib.h"
#include "core/dataArray.h"

/******************COSMOS Telemetry******************************/
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC magnetometer_segment myTelemMagnetometer;
FILE_STATIC mtq_info_segment myTelemMtqInfo;
FILE_STATIC simulink_segment mySimulink;
/****************************************************************/


FILE_STATIC mtq_info mtqInfo;
FILE_STATIC mtq_info lastKnownState;

FILE_STATIC MagnetometerData* magData;
FILE_STATIC hMag magNum;

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
FILE_STATIC volatile magDataStatus mag_data     = mag_invalid;

FILE_STATIC uint8_t mtq_state = MTQ_MEASUREMENT_PHASE;

FILE_STATIC uint8_t send_dipole = 0;

FILE_STATIC int telem_timer;
FILE_STATIC uint16_t telem_timer_ms = 200; // 1 s

FILE_STATIC int update_rt = 0;

FILE_STATIC int rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;

FILE_STATIC int normalOperation = 1;

FILE_STATIC int rcFlag =0;
FILE_STATIC uint16_t mspTempArray[600] = {0};
FILE_STATIC uint16_t mag_xArray[600] = {0};
FILE_STATIC uint16_t mag_yArray[600] = {0};
FILE_STATIC uint16_t mag_zArray[600] = {0};
#pragma PERSISTENT(mspTempArray);
#pragma PERSISTENT(mag_xArray);
#pragma PERSISTENT(mag_yArray);
#pragma PERSISTENT(mag_zArray);
FILE_STATIC uint16_t mspTemp;
FILE_STATIC uint16_t mag_x;
FILE_STATIC uint16_t mag_y;
FILE_STATIC uint16_t mag_z;


/*
 * main.c
 */
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

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");


    initial_setup();

    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
    startCallback(rtOneStep_timer);
    /* Attach rt_OneStep to a timer or interrupt service routine with
     * period 0.1 seconds (the model's base sample time) here.  The
     * call syntax for rt_OneStep is
     *
     *  rt_OneStep();
    */
    // Disable rt_OneStep() here
    // Terminate model
    fflush((NULL));
    P3DIR |= BIT5;
    start_telem_timer();
    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
    {
        if(checkTimer(telem_timer))
        {
            sendTelemetry();
            start_telem_timer();
        }
        if(update_rt)
        {
            P3OUT ^= BIT5;
            getMagnetometerData();
            addData_uint16_t(mag_x, magData->rawX);
            addData_uint16_t(mag_y, magData->rawY);
            addData_uint16_t(mag_z, magData->rawZ);
            rtU.B_body_in_T[0] = magData->convertedX;
            rtU.B_body_in_T[1] = magData->convertedY;
            rtU.B_body_in_T[2] = magData->convertedZ;
            rtU.B_meas_valid = normalOperation;
            rtU.MT_on = 0;
            rt_OneStep();
            updateMtqInfo();
            sendTelemetry();
            update_rt = 0;
        }
        if(send_dipole && mtq_state == mag_valid)
        {

            sendDipolePacket(mtqInfo.xDipole, mtqInfo.yDipole, mtqInfo.zDipole);
            lastKnownState.xDipole = mtqInfo.xDipole;
            lastKnownState.yDipole = mtqInfo.yDipole;
            lastKnownState.zDipole = mtqInfo.zDipole;
            send_dipole = 0;
        }
        rollCall();

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

    magNum = magInit(1); // I2C bus 1

    asensorInit(Ref_2p5V);

    bdot_controller_lib_initialize();

    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));

    mspTemp = init_uint16_t(mspTempArray, 600);
    mag_x = init_uint16_t(mag_xArray, 600);
    mag_y = init_uint16_t(mag_yArray, 600);
    mag_z = init_uint16_t(mag_zArray, 600);

    initializeTimer();
}

void start_telem_timer()
{
    telem_timer = timerPollInitializer(telem_timer_ms);
}


void getMagnetometerData()
{
    magData = testing_magReadXYZData(magNum, ConvertToTeslas);
}

void performSelfTest()
{
    selfTestConfig(magNum);
    normalOperation = 0;
}

void performNormalOp()
{
    normalOperationConfig(magNum);
    normalOperation = 1;
}

void simulink_compute()
{
    update_rt = 1;
    send_dipole = 1;
}


void sendTelemetry()
{
    sendHealthSegment();
    sendMagReadingSegment();
    sendMtqInfoSegment();
    sendSimulinkSegment();
}

void updateMtqInfo()
{
    mtqInfo.tumble_status  = (uint8_t) rtY.tumble;
    if(mtqInfo.tumble_status)
    {
        mtqInfo.xDipole = map((int8_t) rtY.Dig_val[0]);
        mtqInfo.yDipole = map((int8_t) rtY.Dig_val[1]);
        mtqInfo.zDipole = map((int8_t) rtY.Dig_val[2]);
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
    myTelemMagnetometer.xMag = magData->convertedX * 10e9;
    myTelemMagnetometer.yMag = magData->convertedY * 10e9;
    myTelemMagnetometer.zMag = magData->convertedZ * 10e9;
    myTelemMagnetometer.tempMag = magData->convertedTemp;

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
    mySimulink.tumble = rtY.tumble;
    mySimulink.sim_xDipole = rtY.Dig_val[0];
    mySimulink.sim_yDipole = rtY.Dig_val[1];
    mySimulink.sim_zDipole = rtY.Dig_val[2];
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
    }
    if(packet->id == CAN_ID_CMD_ROLLCALL)
    {
        rcFlag = 2;
    }
}

void rollCall()
{
    if (rcFlag == 2)
    {
        rcFlag=1;
        CANPacket rollcallPkt1 = {0};
        rc_adcs_bdot_1 rollcallPkt1_info = {0};
        CANPacket rollcallPkt2 = {0};
        rc_adcs_bdot_2 rollcallPkt2_info = {0};
        CANPacket rollcallPkt3 = {0};
        rc_adcs_bdot_3 rollcallPkt3_info = {0};
        rollcallPkt1_info.rc_adcs_bdot_1_sysrstiv = bspGetResetCount();
        rollcallPkt1_info.rc_adcs_bdot_1_temp_avg = getAvg_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
        rollcallPkt1_info.rc_adcs_bdot_1_temp_max = getMax_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
        rollcallPkt1_info.rc_adcs_bdot_1_temp_min = getMin_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
        rollcallPkt2_info.rc_adcs_bdot_2_mag_x_min = getMin_uint16_t(mag_x);
        rollcallPkt2_info.rc_adcs_bdot_2_mag_x_max = getMax_uint16_t(mag_x);
        rollcallPkt2_info.rc_adcs_bdot_2_mag_x_avg = getAvg_uint16_t(mag_x);
        rollcallPkt2_info.rc_adcs_bdot_2_mag_y_min = getMin_uint16_t(mag_y);
        rollcallPkt3_info.rc_adcs_bdot_3_mag_y_max = getMax_uint16_t(mag_y);
        rollcallPkt3_info.rc_adcs_bdot_3_mag_y_avg = getAvg_uint16_t(mag_y);
        rollcallPkt3_info.rc_adcs_bdot_3_mag_z_min = getMin_uint16_t(mag_z);
        rollcallPkt3_info.rc_adcs_bdot_3_mag_z_max = getMax_uint16_t(mag_y);
        encoderc_adcs_bdot_1(&rollcallPkt1_info, &rollcallPkt1);
        canSendPacket(&rollcallPkt1);
        encoderc_adcs_bdot_2(&rollcallPkt2_info, &rollcallPkt2);
        canSendPacket(&rollcallPkt2);
        encoderc_adcs_bdot_3(&rollcallPkt3_info, &rollcallPkt3);
        canSendPacket(&rollcallPkt3);
    }
    if(rcFlag == 1)
    {
       CANPacket rollcallPkt4 = {0};
       rc_adcs_bdot_4 rollcallPkt4_info = {0};
       rollcallPkt4_info.rc_adcs_bdot_4_mag_z_avg = getAvg_uint16_t(mag_z);
       rollcallPkt4_info.rc_adcs_bdot_4_tumble = rtY.tumble;
       encoderc_adcs_bdot_4(&rollcallPkt4_info, &rollcallPkt4);
       canSendPacket(&rollcallPkt4);
       rcFlag=0;
    }
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
void respondToRollCall (){
    if(rcFlag>0){
            if(rcFlag == 2){
                rcFlag=1;
                CANPacket rollcallPkt1 = {0};
                rc_adcs_bdot_1 rollcallPkt1_info = {0};
                CANPacket rollcallPkt2 = {0};
                rc_adcs_bdot_2 rollcallPkt2_info = {0};
                CANPacket rollcallPkt3 = {0};
                rc_adcs_bdot_3 rollcallPkt3_info = {0};
                rollcallPkt1_info.rc_adcs_bdot_1_sysrstiv = bspGetResetCount();
                rollcallPkt1_info.rc_adcs_bdot_1_temp_avg = getAvg_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
                rollcallPkt1_info.rc_adcs_bdot_1_temp_max = getMax_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
                rollcallPkt1_info.rc_adcs_bdot_1_temp_min = getMin_uint16_t(mspTemp);//asensorReadIntTempC(); //TODO: this
                rollcallPkt2_info.rc_adcs_bdot_2_mag_x_min = getMin_uint16_t(mag_x);
                rollcallPkt2_info.rc_adcs_bdot_2_mag_x_max = getMax_uint16_t(mag_x);
                rollcallPkt2_info.rc_adcs_bdot_2_mag_x_avg = getAvg_uint16_t(mag_x);
                rollcallPkt2_info.rc_adcs_bdot_2_mag_y_min = getMin_uint16_t(mag_y);
                rollcallPkt3_info.rc_adcs_bdot_3_mag_y_max = getMax_uint16_t(mag_y);
                rollcallPkt3_info.rc_adcs_bdot_3_mag_y_avg = getAvg_uint16_t(mag_y);
                rollcallPkt3_info.rc_adcs_bdot_3_mag_z_min = getMin_uint16_t(mag_z);
                rollcallPkt3_info.rc_adcs_bdot_3_mag_z_max = getMax_uint16_t(mag_y);
                encoderc_adcs_bdot_1(&rollcallPkt1_info, &rollcallPkt1);
                canSendPacket(&rollcallPkt1);
                encoderc_adcs_bdot_2(&rollcallPkt2_info, &rollcallPkt2);
                canSendPacket(&rollcallPkt2);
                encoderc_adcs_bdot_3(&rollcallPkt3_info, &rollcallPkt3);
                canSendPacket(&rollcallPkt3);
            }
            if(rcFlag ==1){
                CANPacket rollcallPkt4 = {0};
                rc_adcs_bdot_4 rollcallPkt4_info = {0};
                rollcallPkt4_info.rc_adcs_bdot_4_mag_z_avg = getAvg_uint16_t(mag_z);
                rollcallPkt4_info.rc_adcs_bdot_4_tumble = rtY.tumble;
                encoderc_adcs_bdot_4(&rollcallPkt4_info, &rollcallPkt4);
                canSendPacket(&rollcallPkt4);
                rcFlag=0;
            }
        }

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
