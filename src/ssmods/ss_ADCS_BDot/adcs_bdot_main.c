
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

#define MAG_READING_NORM_OP 1
#define MAG_READING_TEST_OP 0
#define MAG_READING_CURR_OP MAG_READING_TEST_OP

#define BDOT_MAG 0
#define SP_MAG1 1
#define SP_MAG2 2

/******************COSMOS Telemetry******************************/
FILE_STATIC health_segment hseg;
FILE_STATIC meta_segment metaSeg;
FILE_STATIC magnetometer_segment myTelemMagnetometer;
FILE_STATIC mtq_info_segment cosmos_bdot_persp_mtq_info;
FILE_STATIC simulink_segment mySimulink;
FILE_STATIC polling_timer_info_segment polling_timer_info;
/****************************************************************/


/************ MTQ Info From Bdot Perspective*********************/
FILE_STATIC uint8_t mtq_state = MTQ_MEASUREMENT_PHASE;
FILE_STATIC mtq_info bdot_perspective_mtq_info;
FILE_STATIC mtq_info mtq_last_known_state;
/****************************************************************/


/****************Magnetometer Variables*************************/
FILE_STATIC MagnetometerData* bdot_mag_data;
FILE_STATIC MagnetometerData* sp_mag1_data;
FILE_STATIC MagnetometerData* sp_mag2_data;
FILE_STATIC uint8_t sp_mag1_new_data_flag;
FILE_STATIC uint8_t sp_mag2_new_data_flag;
FILE_STATIC uint8_t current_listening_mag = BDOT_MAG;
FILE_STATIC hMag mag_num;
FILE_STATIC uint8_t mag_norm_op = 1;
/***************************************************************/

/******************Simulink Flags and Information***************/
FILE_STATIC uint8_t rt_flag = 0;
/***************************************************************/

/******************Timer Information***************************/
FILE_STATIC TIMER_HANDLE rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;

FILE_STATIC TIMER_HANDLE check_best_mag_timer;
FILE_STATIC uint32_t check_best_mag_timer_ms = 180000; // 3 min
/***************************************************************/

/*******************RollCall***********************************/
FILE_STATIC aggVec_i rc_temp;
FILE_STATIC aggVec_i magX;
FILE_STATIC aggVec_i magY;
FILE_STATIC aggVec_i magZ;
FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4
};
/*******************RollCall***********************************/


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
    start_check_best_mag_timer();

    fflush((NULL));

    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
    {
        if(sp_mag1_new_data_flag)
        {
            convert_mag_data_raw_to_teslas(sp_mag1_data);
            sp_mag1_new_data_flag = 0;
        }
        if(sp_mag2_new_data_flag)
        {
            convert_mag_data_raw_to_teslas(sp_mag2_data);
            sp_mag2_new_data_flag = 0;
        }
        if(checkTimer(check_best_mag_timer))
        {
            determine_best_fit_mag();
            start_check_best_mag_timer();
        }
        if(rt_flag)
        {
            P3OUT ^= BIT5;

            /* get new magnetometer data from bdot magnetometer. */
            read_magnetometer_data();

            /* update data that will be fed into rt onestep */
            update_simulink_info();

            /* perform rt onestep after getting new bdot magnetometer readings */
            rt_OneStep();

            /* determine next mtq command to send out */
            determine_mtq_commands();

            /* send backchannel telemetry */
            send_cosmos_telem();

            /* update rollcall data */
            updateRCData();

            /* Determine if mtq is ready to actuate */
            if(mtq_state == MTQ_MEASUREMENT_PHASE)
            {
                send_dipole_packet(bdot_perspective_mtq_info.xDipole,
                                   bdot_perspective_mtq_info.yDipole,
                                   bdot_perspective_mtq_info.zDipole);
                mtq_last_known_state.xDipole = bdot_perspective_mtq_info.xDipole;
                mtq_last_known_state.yDipole = bdot_perspective_mtq_info.yDipole;
                mtq_last_known_state.zDipole = bdot_perspective_mtq_info.zDipole;
            }

            /* clear rt_flag */
            rt_flag = 0;
        }

        /* update rollcall "aggressively" - David */
        rollcallUpdate();
    }

    bdot_controller_lib_terminate();

    return 0;
}

void initial_setup()
{
    P3OUT |= BIT5;
    P3OUT |= BIT4;
    P3DIR |= BIT4;
    P3DIR |= BIT5;

    /* initialize  bdot magnetometer i2c */
    mag_num = magInit(I2CBus1); // I2C bus 1

    /* initialize can */
    canWrapInitWithFilter();
    setCANPacketRxCallback(can_rx_callback);

    /* temp */
    asensorInit(Ref_2p5V);

    /* initialize rt onestep */
    bdot_controller_lib_initialize();

    /* populate header for backchannel  */
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
    bcbinPopulateHeader(&cosmos_bdot_persp_mtq_info.header, TLM_ID_MTQ_INFO, sizeof(cosmos_bdot_persp_mtq_info));
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));
    bcbinPopulateHeader(&polling_timer_info.header, TLM_ID_POLLING_TIMER, sizeof(polling_timer_info));
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));

    /* TODO: Add comment */
    aggVec_init_i(&rc_temp);
    aggVec_init_i(&magX);
    aggVec_init_i(&magY);
    aggVec_init_i(&magZ);

    /* initialize rollcall */
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    /* initialize timer */
    initializeTimer();
}

/* Update simulink information to send to rt onestep.
 * B_body_inT[0] = magnetometer x
 * B_body_inT[1] = magnetometer y
 * B_body_inT[2] = magnetometer z
 * Notice the order: x -> y -> z
 * B_meas_valid = does not get processed
 * MT_on = 0 always. Always send in the latest VALID magnetometer data*/
void update_simulink_info()
{
    switch(current_listening_mag)
    {
        case BDOT_MAG:
            rtU.B_body_in_T[0] = bdot_mag_data->convertedX;
            rtU.B_body_in_T[1] = bdot_mag_data->convertedY;
            rtU.B_body_in_T[2] = bdot_mag_data->convertedZ;
            rtU.B_meas_valid = mag_norm_op;
            rtU.MT_on = 0;
            break;
        case SP_MAG1:
            rtU.B_body_in_T[0] = sp_mag1_data->convertedX;
            rtU.B_body_in_T[1] = sp_mag1_data->convertedY;
            rtU.B_body_in_T[2] = sp_mag1_data->convertedZ;
            rtU.B_meas_valid = mag_norm_op;
            rtU.MT_on = 0;
            break;
        case SP_MAG2:
            rtU.B_body_in_T[0] = sp_mag2_data->convertedX;
            rtU.B_body_in_T[1] = sp_mag2_data->convertedY;
            rtU.B_body_in_T[2] = sp_mag2_data->convertedZ;
            rtU.B_meas_valid = mag_norm_op;
            rtU.MT_on = 0;
            break;
        default:
            rtU.B_body_in_T[0] = bdot_mag_data->convertedX;
            rtU.B_body_in_T[1] = bdot_mag_data->convertedY;
            rtU.B_body_in_T[2] = bdot_mag_data->convertedZ;
            rtU.B_meas_valid = mag_norm_op;
            rtU.MT_on = 0;
    }
}

/* Read magnetometer data based on the current operation.
 * For debugging purposes, use testing_magReadXYZData (for aardvark).
 * Function to read magnetometer data returns a struct with xyz magnetometer and temp. */
void read_magnetometer_data()
{
    switch(MAG_READING_CURR_OP)
    {
        case MAG_READING_NORM_OP:
            bdot_mag_data = magReadXYZData(mag_num, ConvertToTeslas);
            break;
        case MAG_READING_TEST_OP:
            bdot_mag_data = testing_magReadXYZData(mag_num, ConvertToTeslas);
            break;
        default: magReadXYZData(mag_num, ConvertToTeslas);
    }
}

void convert_mag_data_raw_to_teslas(MagnetometerData * mag)
{
    mag->convertedX = magConvertRawToTeslas(mag->rawX);
    mag->convertedZ = magConvertRawToTeslas(mag->rawZ);
    mag->convertedY = magConvertRawToTeslas(mag->rawY);
}

void determine_best_fit_mag()
{
    float bdot_mag_norm = sqrt(abs(bdot_mag_data->convertedX)^2 + abs(bdot_mag_data->convertedY)^2 + abs(bdot_mag_data->convertedZ)^2);
    float sp_mag1_norm =  sqrt(abs(sp_mag1_data->convertedX)^2 + abs(sp_mag1_data->convertedY)^2 + abs(sp_mag1_data->convertedZ)^2);
    float sp_mag2_norm = sqrt(abs(sp_mag2_data->convertedX)^2 + abs(sp_mag2_data->convertedY)^2 + abs(sp_mag2_data->convertedZ)^2);

    // find the median of the norm to determine best magnetometer to use. TODO: Think of a better, less costly method
    if(bdot_mag_norm <= sp_mag1_norm && sp_mag1_norm <= sp_mag2_norm)
    {
        current_listening_mag = SP_MAG1;
        return;
    }
    if(sp_mag1_norm <= bdot_mag_norm && bdot_mag_norm <= sp_mag2_norm)
    {
        current_listening_mag = BDOT_MAG;
    }
    if(sp_mag1_norm <= sp_mag2_norm && sp_mag2_norm <= bdot_mag_norm)
    {
        current_listening_mag = SP_MAG2;
    }
}

void start_check_best_mag_timer()
{
    check_best_mag_timer = timerPollInitializer(check_best_mag_timer_ms);
}


void determine_mtq_commands()
{
    bdot_perspective_mtq_info.tumble_status  = (uint8_t) rtY.tumble;
    if(bdot_perspective_mtq_info.tumble_status)
    {
        bdot_perspective_mtq_info.xDipole = map((int8_t) rtY.Dig_val[0]);
        bdot_perspective_mtq_info.yDipole = map((int8_t) rtY.Dig_val[1]);
        bdot_perspective_mtq_info.zDipole = map((int8_t) rtY.Dig_val[2]);
    } else
    {
        bdot_perspective_mtq_info.xDipole = 0;
        bdot_perspective_mtq_info.yDipole = 0;
        bdot_perspective_mtq_info.zDipole = 0;
    }
}

/* Callback function tied to the one interrupt based timer. Sets flag at 10hz to
 * call this function that sets flag for main loop to processed new magnetometer data and
 * process data with rt onestep */
void simulink_compute()
{
    rt_flag = 1;
}

/* send cosmos telemetry */
void send_cosmos_telem()
{
    send_health_segment_cosmos();
    send_bdot_mag_reading_cosmos();
    send_mtq_info_segment_cosmos();
    send_simulink_segment_cosmos();
    bcbinSendPacket((uint8_t *) &metaSeg, sizeof(metaSeg));
}


void send_dipole_packet(int8_t x, int8_t y, int8_t z)
{
    CANPacket dipole_packet = {0};
    cmd_mtq_bdot dipole_info = {0};

    dipole_info.cmd_mtq_bdot_x = x;
    dipole_info.cmd_mtq_bdot_y = y;
    dipole_info.cmd_mtq_bdot_z = z;

    encodecmd_mtq_bdot(&dipole_info, &dipole_packet);
    canSendPacket(&dipole_packet);
}

/* Switch magnetometer reading operation to do self test...lol */
void perform_self_test()
{
    selfTestConfig(mag_num);
    mag_norm_op = 0;
}

/* Switch magnetometer reading operation to do normal operation */
void perform_norm_op()
{
    normalOperationConfig(mag_num);
    mag_norm_op = 1;
}

/* convert val range from -127-127 to -100-100 */
int map(int val)
{
    return map_general(val, -127, 127, -100, 100);
}

int map_general(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Can packet receive callback function */
void can_rx_callback(CANPacket *packet)
{
    mtq_ack ack = {0};
    sensorproc_mag mag1 = {0};
    sensorproc_mag2 mag2 = {0};

    switch(packet->id)
    {
        case CAN_ID_MTQ_ACK:
            // measurement_phase = 0;
            // actuation_phase = 1;
            decodemtq_ack(packet, &ack);
            if(!ack.mtq_ack_phase)
            {
                mtq_state = MTQ_MEASUREMENT_PHASE;
            } else
            {
                mtq_state = MTQ_ACTUATION_PHASE;
            }
            break;
        case CAN_ID_SENSORPROC_MAG:
            decodesensorproc_mag(packet, &mag1);
            if(mag1.sensorproc_mag_valid)
            {
                sp_mag1_data->rawX = mag1.sensorproc_mag_x;
                sp_mag1_data->rawY = mag1.sensorproc_mag_y;
                sp_mag1_data->rawZ = mag1.sensorproc_mag_z;
                sp_mag1_new_data_flag = 1;
            }
            break;
        case CAN_ID_SENSORPROC_MAG2:
            decodesensorproc_mag2(packet, &mag2);
            if(mag2.sensorproc_mag2_valid)
            {
                sp_mag2_data->rawX = mag2.sensorproc_mag2_x;
                sp_mag2_data->rawY = mag2.sensorproc_mag2_y;
                sp_mag2_data->rawZ = mag2.sensorproc_mag2_z;
                sp_mag2_new_data_flag = 1;
            }
            break;
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
            break;
    }
}

/* send health segment through backchannel */
void send_health_segment_cosmos()
{
    // Populate and send the health segment for this entity
    hseg.inttemp = asensorReadIntTempC();
    hseg.oms = OMS_Unknown;
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));

    // Also invoke the status handler for UART to send its health segments
    debugInvokeStatusHandler(Entity_UART);

}

/* send magnetometer reading segment through backchannel */
void send_bdot_mag_reading_cosmos()
{
    myTelemMagnetometer.xMag = bdot_mag_data->convertedX * 1e9;
    myTelemMagnetometer.yMag = bdot_mag_data->convertedY * 1e9;
    myTelemMagnetometer.zMag = bdot_mag_data->convertedZ * 1e9;
    myTelemMagnetometer.tempMag = bdot_mag_data->convertedTemp;

    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));
}

/* Send magnetorquer's state based on bdot's perspective */
void send_mtq_info_segment_cosmos()
{
    cosmos_bdot_persp_mtq_info.xDipole = mtq_last_known_state.xDipole;
    cosmos_bdot_persp_mtq_info.yDipole = mtq_last_known_state.yDipole;
    cosmos_bdot_persp_mtq_info.zDipole = mtq_last_known_state.zDipole;

    bcbinSendPacket((uint8_t *) &cosmos_bdot_persp_mtq_info, sizeof(cosmos_bdot_persp_mtq_info));
}

/* Send rt onestep output */
void send_simulink_segment_cosmos()
{
    mySimulink.tumble = rtY.tumble;
    mySimulink.sim_xDipole = rtY.Dig_val[0];
    mySimulink.sim_yDipole = rtY.Dig_val[1];
    mySimulink.sim_zDipole = rtY.Dig_val[2];
    bcbinSendPacket((uint8_t *) &mySimulink, sizeof(mySimulink));
}


/* debugging purpose function */
void send_all_polling_timers_segment()
{
    user_timer_polling_info polling_info[NUM_SUPPORTED_DURATIONS_POLLING];
    debug_polling_timer_info(polling_info);
    bcbinSendPacket((uint8_t *) &polling_timer_info, sizeof(polling_timer_info));
}


/* update rollcall packet */
void updateRCData()
{
    aggVec_push_i(&magX, bdot_mag_data->rawX);
    aggVec_push_i(&magY, bdot_mag_data->rawY);
    aggVec_push_i(&magZ, bdot_mag_data->rawZ);
    aggVec_push_i(&rc_temp, asensorReadIntTempRawC());
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
  bdot_controller_lib_step();
}


void rcPopulate1(CANPacket *out)
{
    rc_adcs_bdot_1 rc;
    rc.rc_adcs_bdot_1_reset_count = bspGetResetCount();
    rc.rc_adcs_bdot_1_sysrstiv = SYSRSTIV;
    rc.rc_adcs_bdot_1_temp_avg = aggVec_avg_i_i(&rc_temp);
    rc.rc_adcs_bdot_1_temp_max = aggVec_max_i(&rc_temp);
    rc.rc_adcs_bdot_1_temp_min = aggVec_min_i(&rc_temp);
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
