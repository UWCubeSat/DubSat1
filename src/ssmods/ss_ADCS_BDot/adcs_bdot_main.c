/* BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT BDOT
 *
 * MAIN FUNCTIONALITY
 * - Reads magnetometer data using the i2c bus and feeds it into Simulink autocode rt_oneStep every 10hz.
 * - rt_oneStep will determine the xyz dipoles commands to send to the magnetorquer board (MTQ) via CAN.
 * - MTQ will send acknowledgement packets to Bdot via CAN that will inform Bdot of the current state of the magnetorquer (whether it is firing or not).
 * - Bdot only reads mangetometer data when MTQ is not firing.
 *
 * BEST FIT MAGNETOMETER
 * - Sensor Processing board (SP) will send two CAN packets, each with magnetometer readings from its own magnetometers.
 * - Upon first launch for a certain amount of time, Bdot will decide which magnetometer readings are most accurate relative to each other.
 * - After satellite has stabilized after launch, commands from ground will "lock" in one magnetometer as the best fit one.
 *
 * Detumble Safety Procedure Initialization (DESPIN)
 * - Bdot will determine/time how long the satellite has been tumbling for.
 * - If the amount of time is above a certain threshold, it is likely that the satellite will never de-tumble and that there are problems with hardware/software.
 * - Bdot will then initiate Detumble Safety Lockout, which mean it will send dipole commands to MTQ of zeros, effectively turning MTQ off.
 * - Commands from ground can be sent to exit this protocol and let Bdot function normally.
 *
 * rt_oneStep: Feed into rt onestep only VALID magnetometer data.
 *
 */

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

#define BDOT_MAG 0
#define SP_MAG1 1
#define SP_MAG2 2

#define MAG_BEST_FIT_OVERRIDE 1
#define MAG_BEST_FIT_AUTO 0

#define NORMAL_READING_OPERATION 1
#define SELF_TEST_READING_OPERATION 0

#define BDOT_AWAKE 1
#define BDOT_NAP 0


/******************COSMOS Telemetry******************************/
FILE_STATIC health_segment hseg;
FILE_STATIC meta_segment metaSeg;
FILE_STATIC magnetometer_segment continuous_mag_data_cosmos;
FILE_STATIC magnetometer_segment bdot_magnetometer_data_cosmos;
FILE_STATIC magnetometer_segment sp_mag1_data_cosmos;
FILE_STATIC magnetometer_segment sp_mag2_data_cosmos;
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
/* holds magnetometer data read every 10 hz */
FILE_STATIC MagnetometerData* continuous_bdot_mag_data;

/* holds only the magnetometer that are "valid", meaning collected
 * only under valid conditions.
 * Valid conditions: 1. Magnetorquer is not on
 *                   2. Magnetometer  is set to be in normal configurations
 */
FILE_STATIC MagnetometerData* valid_bdot_mag_data;

/* sensor proc magnetometer data. These data were decided to be unfiltered data,
 * meaning the data can be data collected when the magnetorquer is on.
 */
FILE_STATIC MagnetometerData* sp_mag1_data;
FILE_STATIC MagnetometerData* sp_mag2_data;

/* flag set to see when new magnetometer data are sent over to bdot from sensorproc */
FILE_STATIC uint8_t sp_mag1_new_data_flag;
FILE_STATIC uint8_t sp_mag2_new_data_flag;

/* stores which magnetometer bdot is curently "listening" to */
FILE_STATIC uint8_t current_listening_mag = BDOT_MAG;

/* lets ground station pick which mag to use */
FILE_STATIC uint8_t mag_selection_mode = MAG_BEST_FIT_OVERRIDE;

/* Ground station selection of which magnetomer to use */
FILE_STATIC uint8_t cmd_mag_selection = BDOT_MAG;

/* mag number for magnetometer reading. for questions, contact david */
FILE_STATIC hMag mag_num;

/* stores what magnetometer reading mode bdot is on, normal or testing */
FILE_STATIC uint8_t mag_reading_mode = NORMAL_READING_OPERATION;

FILE_STATIC uint8_t tumbling_too_long_flag = 0;
/***************************************************************/


/******************Simulink Flags and Information***************/
FILE_STATIC uint8_t rt_flag = 0;
/***************************************************************/


/******************Timer Information***************************/
FILE_STATIC TIMER_HANDLE rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;

/* this is the timer that will check go off when it's time to check
 * to see which magnetometer to use.
 */
FILE_STATIC TIMER_HANDLE check_best_mag_timer;
FILE_STATIC uint32_t check_best_mag_timer_ms = 180000; // 3 min

/* this is the timer that will check to see whether or not bdot has
 * reported tumbling continuously for a configurable amount of time.
 * If that time has reached, bdot should send dipoles of 0 to magnetorquer
 * until commands from ground are sent to "wake up" bdot OR rt onestep reports
 * not tumbling */
FILE_STATIC TIMER_HANDLE check_nap_status_timer;
FILE_STATIC uint32_t check_nap_status_timer_ms = 1200000; // 20 min
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
/**************************************************************/


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
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, handleDebugActionCallback);
#endif  //  __DEBUG__

    initial_setup();
    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
    startCallback(rtOneStep_timer);
    start_check_best_mag_timer();
    start_check_nap_status_timer();
    while (1)
    {
        process_sp_mag();

        if(checkTimer(check_best_mag_timer))
        {
            determine_best_fit_mag();
            start_check_best_mag_timer();
        }

        if(!tumbling_too_long_flag)
        {
            if(checkTimer(start_check_nap_status_timer))
            {
                tumbling_too_long_flag = 1;
            }
        }

        if(rt_flag)
        {
            P3OUT ^= BIT5;
            read_magnetometer_data();

            if(mtq_state == MTQ_MEASUREMENT_PHASE)
            {
            	update_valid_mag_data();
            }

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

                /* get new magnetometer data from bdot magnetometer. */
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
    bcbinPopulateHeader(&continuous_mag_data_cosmos.header, TLM_ID_CONTINUOUS_MAG, sizeof(continuous_mag_data_cosmos));
    bcbinPopulateHeader(&bdot_magnetometer_data_cosmos.header, TLM_ID_BDOT_MAGNETOMETER, sizeof(bdot_magnetometer_data_cosmos));
    bcbinPopulateHeader(&sp_mag1_data_cosmos.header, TLM_ID_SP_MAG1, sizeof(sp_mag1_data_cosmos));
    bcbinPopulateHeader(&sp_mag2_data_cosmos.header, TLM_ID_SP_MAG2, sizeof(sp_mag2_data_cosmos));
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
    // Data going into rt onestep depends on which magnetometer bdot is listening to
    switch(current_listening_mag)
    {
        case BDOT_MAG:
            rtU.B_body_in_T[0] = valid_bdot_mag_data->convertedX;
            rtU.B_body_in_T[1] = valid_bdot_mag_data->convertedY;
            rtU.B_body_in_T[2] = valid_bdot_mag_data->convertedZ;
            rtU.B_meas_valid = (mag_reading_mode == NORMAL_READING_OPERATION);
            rtU.MT_on = 0;
            break;
        case SP_MAG1:
            rtU.B_body_in_T[0] = sp_mag1_data->convertedX;
            rtU.B_body_in_T[1] = sp_mag1_data->convertedY;
            rtU.B_body_in_T[2] = sp_mag1_data->convertedZ;
            rtU.B_meas_valid = (mag_reading_mode == NORMAL_READING_OPERATION);
            rtU.MT_on = 0;
            break;
        case SP_MAG2:
            rtU.B_body_in_T[0] = sp_mag2_data->convertedX;
            rtU.B_body_in_T[1] = sp_mag2_data->convertedY;
            rtU.B_body_in_T[2] = sp_mag2_data->convertedZ;
            rtU.B_meas_valid = (mag_reading_mode == NORMAL_READING_OPERATION);
            rtU.MT_on = 0;
            break;
        default:
            rtU.B_body_in_T[0] = valid_bdot_mag_data->convertedX;
            rtU.B_body_in_T[1] = valid_bdot_mag_data->convertedY;
            rtU.B_body_in_T[2] = valid_bdot_mag_data->convertedZ;
            rtU.B_meas_valid = (mag_reading_mode == NORMAL_READING_OPERATION);
            rtU.MT_on = 0;
    }
}


void process_sp_mag()
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
}

/* Read magnetometer data continuously every 10Hz, ignoring whether or not mtq is actuating.
 * Function is for debugging purposes.
 */
void read_magnetometer_data()
{
    continuous_bdot_mag_data = magReadXYZData(mag_num, ConvertToTeslas);
}


/* Update valid bdot magnetometer. This function is only called when mtq is in measurement phase */
void update_valid_mag_data()
{
    if (mag_reading_mode == NORMAL_READING_OPERATION)
    {
        valid_bdot_mag_data->convertedX = continuous_bdot_mag_data->convertedX;
        valid_bdot_mag_data->convertedY = continuous_bdot_mag_data->convertedY;
        valid_bdot_mag_data->convertedZ = continuous_bdot_mag_data->convertedZ;
    }
}

void convert_mag_data_raw_to_teslas(MagnetometerData * mag)
{
    mag->convertedX = magConvertRawToTeslas(mag->rawX);
    mag->convertedZ = magConvertRawToTeslas(mag->rawZ);
    mag->convertedY = magConvertRawToTeslas(mag->rawY);
}

/* Function that does math to determine which magnetometer bdot should listen to.
 * This should not be called every time sampling.
 */
void determine_best_fit_mag()
{
    // find the median of the norm to determine best magnetometer to use. TODO: Think of a better, less costly method
    float bdot_mag_norm = sqrt(abs(continuous_bdot_mag_data->convertedX)^2 + abs(continuous_bdot_mag_data->convertedY)^2 + abs(continuous_bdot_mag_data->convertedZ)^2);
    float sp_mag1_norm =  sqrt(abs(sp_mag1_data->convertedX)^2 + abs(sp_mag1_data->convertedY)^2 + abs(sp_mag1_data->convertedZ)^2);
    float sp_mag2_norm = sqrt(abs(sp_mag2_data->convertedX)^2 + abs(sp_mag2_data->convertedY)^2 + abs(sp_mag2_data->convertedZ)^2);

    /* If magnetometer selection mode is on override, which is a command given from ground, bdot should select the magnetometer
       that ground saids to listen to */
    if(mag_selection_mode == MAG_BEST_FIT_OVERRIDE)
    {
        current_listening_mag = cmd_mag_selection;
        return;
    }
    if(bdot_mag_norm <= sp_mag1_norm && sp_mag1_norm <= sp_mag2_norm)
    {
        current_listening_mag = SP_MAG1;
        return;
    }
    if(sp_mag1_norm <= bdot_mag_norm && bdot_mag_norm <= sp_mag2_norm)
    {
        current_listening_mag = BDOT_MAG;
        return;
    }
    if(sp_mag1_norm <= sp_mag2_norm && sp_mag2_norm <= bdot_mag_norm)
    {
        current_listening_mag = SP_MAG2;
        return;
    }
}

/* Start the timer that goes off when it's time to check what magnetometer to listen to */
void start_check_best_mag_timer()
{
    check_best_mag_timer = timerPollInitializer(check_best_mag_timer_ms);
}

/* Start the timer that goes off when bdot has been tumbling for a certain amount of time */
void start_check_nap_status_timer()
{
    check_nap_status_timer = timerPollInitializer(check_nap_status_timer_ms);
}

/* determine that dipoles to send to mtq */
void determine_mtq_commands()
{
    bdot_perspective_mtq_info.tumble_status  = (uint8_t) rtY.tumble;
    if(bdot_perspective_mtq_info.tumble_status)
    {
        if(!tumbling_too_long_flag)
        {
            bdot_perspective_mtq_info.xDipole = map((int8_t) rtY.Dig_val[0]);
            bdot_perspective_mtq_info.yDipole = map((int8_t) rtY.Dig_val[1]);
            bdot_perspective_mtq_info.zDipole = map((int8_t) rtY.Dig_val[2]);
        }
    } else
    {
        tumbling_too_long_flag = 0;
        bdot_perspective_mtq_info.xDipole = 0;
        bdot_perspective_mtq_info.yDipole = 0;
        bdot_perspective_mtq_info.zDipole = 0;

        /* restart nap timer for bdot */
        endPollingTimer(check_nap_status_timer);
        start_check_nap_status_timer();
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
    send_continuous_mag_reading_cosmos();
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

/* convert val range from -127->127 to -100->100 */
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

void send_continuous_mag_reading_cosmos()
{
    continuous_mag_data_cosmos.xMag = continuous_bdot_mag_data->convertedX * 1e9;
    continuous_mag_data_cosmos.yMag = continuous_bdot_mag_data->convertedY * 1e9;
    continuous_mag_data_cosmos.zMag = continuous_bdot_mag_data->convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &continuous_mag_data_cosmos, sizeof(continuous_mag_data_cosmos));
}

/* send magnetometer reading segment through backchannel */
void send_bdot_mag_reading_cosmos()
{
    bdot_magnetometer_data_cosmos.xMag = valid_bdot_mag_data->convertedX * 1e9;
    bdot_magnetometer_data_cosmos.yMag = valid_bdot_mag_data->convertedY * 1e9;
    bdot_magnetometer_data_cosmos.zMag = valid_bdot_mag_data->convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &bdot_magnetometer_data_cosmos, sizeof(bdot_magnetometer_data_cosmos));
}

/* send magnetometer reading segment through backchannel */
void send_sp_mag1_reading_cosmos()
{
    sp_mag1_data_cosmos.xMag = sp_mag1_data->convertedX * 1e9;
    sp_mag1_data_cosmos.yMag = sp_mag1_data->convertedY * 1e9;
    sp_mag1_data_cosmos.zMag = sp_mag1_data->convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &sp_mag1_data_cosmos, sizeof(sp_mag1_data_cosmos));
}

/* send magnetometer reading segment through backchannel */
void send_sp_mag2_reading_cosmos()
{
    sp_mag2_data_cosmos.xMag = sp_mag2_data->convertedX * 1e9;
    sp_mag2_data_cosmos.yMag = sp_mag2_data->convertedY * 1e9;
    sp_mag2_data_cosmos.zMag = sp_mag2_data->convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &sp_mag2_data_cosmos, sizeof(sp_mag2_data_cosmos));
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
    aggVec_push_i(&magX, continuous_bdot_mag_data->rawX);
    aggVec_push_i(&magY, continuous_bdot_mag_data->rawY);
    aggVec_push_i(&magZ, continuous_bdot_mag_data->rawZ);
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

/* Switch magnetometer reading operation to do self test...lol */
void select_mode_operation(uint8_t reading_mode_selection)
{
    switch(reading_mode_selection)
    {
        case NORMAL_READING_OPERATION:
            mag_normal_reading_operation_config(mag_num);
            mag_reading_mode = NORMAL_READING_OPERATION;
            __delay_cycles(6000);
            break;
        case SELF_TEST_READING_OPERATION:
            mag_self_test_config(mag_num);
            mag_reading_mode = SELF_TEST_READING_OPERATION;
            __delay_cycles(6000);
            break;
        default:
            mag_normal_reading_operation_config(mag_num);
            mag_reading_mode = NORMAL_READING_OPERATION;
            __delay_cycles(6000);
            break;
    }
}

/* Supporting function that carries out ground command */
void mag_select_switch(uint8_t mag_selection)
{
    switch(mag_selection)
    {
        case CMD_SELECT_AUTO:
            mag_selection_mode = MAG_BEST_FIT_AUTO;
            break;
        case CMD_SELECT_BDOT_MAG:
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            cmd_mag_selection = BDOT_MAG;
            break;
        case CMD_SELECT_SP_MAG1:
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            cmd_mag_selection = SP_MAG1;
            break;
        case CMD_SELECT_SP_MAG2:
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            cmd_mag_selection = SP_MAG2;
            break;
        default:
            break;
    }
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    mag_select_cmd* mag_select;
    mode_operation_cmd* mode_operation_select;

    if (mode == Mode_BinaryStreaming)
    {
        switch(cmdstr[0])
        {
            case OPCODE_MAG_SELECT_CMD:
                mag_select = (mag_select_cmd *) (cmdstr + 1);
                mag_select_switch(mag_select->mag_select);
                break;
            case OPCODE_MODE_OPERATION_CMD:
                mode_operation_select = (mode_operation_cmd *)(cmdstr + 1);
                select_mode_operation(mode_operation_select->select_mode_operation);
                break;
            case OPCODE_COMMONCMD:
                break;
            default:
                break;
        }
    }
    return 1;
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
