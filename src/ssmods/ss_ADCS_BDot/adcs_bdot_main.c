#include <adcs_bdot.h>
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "sensors/magnetometer.h"
#include "core/timer.h"
#include "bdot_controller_lib.h"
#include "interfaces/rollcall.h"
#include "core/agglib.h"

/******************COSMOS Telemetry******************************/
FILE_STATIC health_segment hseg;
FILE_STATIC meta_segment metaSeg;
FILE_STATIC magnetometer_segment myTelemMagnetometer;
FILE_STATIC mtq_info_segment myTelemMtqInfo;
FILE_STATIC simulink_segment mySimulink;
FILE_STATIC polling_timer_info_segment polling_timer_info;
/****************************************************************/


/************ MTQ Info From Bdot Perspective*********************/
FILE_STATIC uint8_t mtq_state = MTQ_MEASUREMENT_PHASE;
FILE_STATIC mtq_info mtqInfo;
FILE_STATIC mtq_info lastKnownState;
FILE_STATIC volatile magDataStatus mag_data = mag_invalid;
/****************************************************************/


/****************Magnetometer Variables*************************/
FILE_STATIC MagnetometerData* magData;
FILE_STATIC hMag magNum;
FILE_STATIC uint8_t magNormalOperation = 1;
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
FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC aggVec_f rc_temp;
FILE_STATIC aggVec_i magX;
FILE_STATIC aggVec_i magY;
FILE_STATIC aggVec_i magZ;
FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4
};

/***************************************************************/
/*******************Agg Library***********************************/
/***************************************************************/

/*******************Miscellaneous*******************************/
FILE_STATIC ModuleStatus mod_status;
/***************************************************************/


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
    int count = 0;
    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
    {

        if(update_rt_flag)
        {
            P3OUT ^= BIT5;
            getMagnetometerData();
            rtU.B_body_in_T[0] = magData->convertedX;
            rtU.B_body_in_T[1] = magData->convertedY;
            rtU.B_body_in_T[2] = magData->convertedZ;
            rtU.B_meas_valid = magNormalOperation;
            rtU.MT_on = 0;
            rt_OneStep();
            updateMtqInfo();
            sendTelemetry();
            updateRCData();
            update_rt_flag = 0;
            count++;
        }

        if(count == 100)
        {
            send_all_polling_timers_segment();
            count = 0;
        }
        if(send_dipole_flag && mtq_state == mag_valid)
        {
            sendDipolePacket(mtqInfo.xDipole, mtqInfo.yDipole, mtqInfo.zDipole);
            lastKnownState.xDipole = mtqInfo.xDipole;
            lastKnownState.yDipole = mtqInfo.yDipole;
            lastKnownState.zDipole = mtqInfo.zDipole;
            send_dipole_flag = 0;
        }

        rollcallUpdate();

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

    magNum = magInit(I2CBus1); // I2C bus 1

    asensorInit(Ref_2p5V);

    bdot_controller_lib_initialize();

    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));
    bcbinPopulateHeader(&polling_timer_info.header, TLM_ID_POLLING_TIMER, sizeof(polling_timer_info));
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));
    aggVec_init_f(&rc_temp);
    aggVec_init_i(&magX);
    aggVec_init_i(&magY);
    aggVec_init_i(&magZ);
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    initializeTimer();
}

void getMagnetometerData()
{
    magData = testing_magReadXYZData(magNum, ConvertToTeslas);
}

void performSelfTest()
{
    selfTestConfig(magNum);
    magNormalOperation = 0;
}

void performNormalOp()
{
    normalOperationConfig(magNum);
    magNormalOperation = 1;
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
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
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
    myTelemMagnetometer.xMag = magData->convertedX * 1e9;
    myTelemMagnetometer.yMag = magData->convertedY * 1e9;
    myTelemMagnetometer.zMag = magData->convertedZ * 1e9;
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
        rollcallStart();
    }
}

void send_all_polling_timers_segment()
{
    user_timer_polling_info polling_info[NUM_SUPPORTED_DURATIONS_POLLING];
    get_polling_timer_info(polling_info);
    bcbinSendPacket((uint8_t *) &polling_timer_info, sizeof(polling_timer_info));
}


void updateRCData()
{
    aggVec_push_i(&magX, magData->rawX);
    aggVec_push_i(&magY, magData->rawY);
    aggVec_push_i(&magZ, magData->rawZ);
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

void rcPopulate1(CANPacket *out)
{
    rc_adcs_bdot_1 rc;
//    rc.rc_adcs_bdot_1_reset_count = bspGetResetCount();
//    rc.rc_adcs_bdot_1_sysrstiv = SYSRSTIV;
    rc.rc_adcs_bdot_1_temp_avg = 2732+((uint16_t)(10*aggVec_avg_f(&rc_temp)));
    rc.rc_adcs_bdot_1_temp_max = 2732+((uint16_t)(10*aggVec_max_f(&rc_temp)));
    rc.rc_adcs_bdot_1_temp_min = 2732+((uint16_t)(10*aggVec_min_f(&rc_temp)));
    rc.rc_adcs_bdot_1_reset_count = 0;
    aggVec_reset((aggVec *)&rc_temp);
    encoderc_adcs_bdot_1(&rc, out);
}
void rcPopulate2(CANPacket *out)
{
    rc_adcs_bdot_2 rc ;
    rc.rc_adcs_bdot_2_mag_x_min = aggVec_min_i(&magX);
    rc.rc_adcs_bdot_2_mag_x_max = aggVec_max_i(&magX);
    rc.rc_adcs_bdot_2_mag_x_avg = aggVec_avg_i_i(&magX);
    rc.rc_adcs_bdot_2_mag_y_min = aggVec_min_i(&magY);
    aggVec_reset((aggVec *)&magX);
    encoderc_adcs_bdot_2(&rc, out);
}
void rcPopulate3(CANPacket *out)
{
    rc_adcs_bdot_3 rc = {0};
    rc.rc_adcs_bdot_3_mag_y_max = aggVec_max_i(&magY);
    rc.rc_adcs_bdot_3_mag_y_avg = aggVec_avg_i_i(&magY);
    rc.rc_adcs_bdot_3_mag_z_min = aggVec_min_i(&magZ);
    rc.rc_adcs_bdot_3_mag_z_max = aggVec_max_i(&magZ);
    aggVec_reset((aggVec *)&magY);
    encoderc_adcs_bdot_3(&rc, out);
}
void rcPopulate4(CANPacket *out)
{
    rc_adcs_bdot_4 rc = {0};
    rc.rc_adcs_bdot_4_mag_z_avg = aggVec_avg_i_i(&magZ);
    rc.rc_adcs_bdot_4_tumble = rtY.tumble;
    aggVec_reset((aggVec *)&magZ);
    encoderc_adcs_bdot_4(&rc, out);
}

//void start_telem_timer()
//{
//    telem_timer = timerPollInitializer(telem_timer_ms);
//}

//FILE_STATIC int telem_timer;
//FILE_STATIC uint16_t telem_timer_ms = 200; // 1 s
