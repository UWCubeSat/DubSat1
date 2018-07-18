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
#include "core/utils.h"



#define MAG_BEST_FIT_OVERRIDE 1
#define MAG_BEST_FIT_AUTO 0

#define NORMAL_READING_OPERATION    CAN_ENUM_BDOT_CONTROL_MODE_NORMALMODE
#define NAP_OPERATION               CAN_ENUM_BDOT_CONTROL_MODE_SLEEPMODE
#define SPAM_OPERATION              CAN_ENUM_BDOT_CONTROL_MODE_SPAM

#define POP_ON CAN_ENUM_BOOL_TRUE
#define POP_OFF CAN_ENUM_BOOL_FALSE

#define POP_ON_SCALE_FACTOR -1
#define POP_OFF_SCALE_FACTOR 1

#define MINUTES_TO_MILLISEC_CONVERSION_FACTOR 60000

#define RTY_TO_DIPOLE_FACTOR 0.7874015748

#define MAX_SPAM_AVG_COUNT 5

typedef enum mag_src {
    BDOT_MAG,
    SP_MAG1,
    SP_MAG2
} mag_src;

typedef enum mtq_phase {
    MTQ_MEASUREMENT_PHASE,
    MTQ_ACTUATION_PHASE,
    MTQ_PMS_PHASE
} mtq_phase;

typedef enum bdot_state_mode {
    NORMAL_MODE,
    SLEEP_MODE,
    SPAM_MODE
} bdot_state_mode;

typedef enum spam_control {
    SPAM_OFF,
    SPAM_ON
} spam_control_state;

typedef enum spam_axis {
    SPAM_X,
    SPAM_Y,
    SPAM_Z
} spam_axis;

/******************COSMOS Telemetry******************************/
FILE_STATIC health_segment hseg;
FILE_STATIC meta_segment metaSeg;
/* stores the magnetometer data read every 10 hz */
FILE_STATIC magnetometer_segment continuous_mag_data_cosmos;
/* stores the VALID bdot magnetometer data */
FILE_STATIC magnetometer_segment bdot_magnetometer_data_cosmos;

/* stores state of mtq when continuous mag measurement is taken */
FILE_STATIC mtq_phase mag_measure_mtq_state = MTQ_MEASUREMENT_PHASE;

/* stores the VALID sensor proc magnetometer data. Right now, this isn't
 * being used, but may need to be used when for debugging purposes when
 * implementing best fit mag functionality.
 */
FILE_STATIC magnetometer_segment sp_mag1_data_cosmos;
FILE_STATIC magnetometer_segment sp_mag2_data_cosmos;
/* Stores the last dipole commands bdot sent to MTQ */
FILE_STATIC mtq_info_segment cosmos_bdot_persp_mtq_info;
/* Stores what simulink/rt_onestep outputs of dipoles and tumble status */
FILE_STATIC simulink_segment mySimulink;
/* Outputs all the debugging info about all the timers, right now isn't being used */
FILE_STATIC polling_timer_info_segment polling_timer_info;
/* Stores the last state that bdot is in */
FILE_STATIC bdot_state_status bdot_state_cosmos;

/****************************************************************/

/******************COSMOS Commands*******************************/
/****************************************************************/

/************ MTQ Info From Bdot Perspective*********************/
/* Stores the state that MTQ reported that it's in */
FILE_STATIC mtq_phase mtq_state = MTQ_MEASUREMENT_PHASE;
/* Stores the last info about the dipoles bdot sent to mtq. This is for
 * debugging purposes. */
FILE_STATIC mtq_info bdot_perspective_mtq_info;
/* Stores the last known state/dipoles that mtq is listening to */
FILE_STATIC mtq_info mtq_last_known_state;
/****************************************************************/


/****************Magnetometer Variables*************************/
/* keeps track of the bdot's state */
FILE_STATIC bdot_state_mode bdot_state = NORMAL_MODE;
/* stores the alst state bdot is in, disregarding SPAM states. This is
 * used so that when you're exiting SPAM mode, you'll know what state to
 * return to, NORMAL or SLEEP. */
FILE_STATIC bdot_state_mode last_bdot_state = NORMAL_MODE;
/* Stores the ground command bdot state */
FILE_STATIC bdot_state_mode gcmd_next_bdot_state = NORMAL_MODE;
/* Stores if there was a ground command */
FILE_STATIC uint8_t gcmd_bdot_state_flag = 0;
/* Stores the ground command to turn on or off SPAM */
FILE_STATIC spam_control_state gcmd_spam_control_switch = SPAM_ON;
#pragma PERSISTENT(gcmd_spam_control_switch)

FILE_STATIC uint8_t gcmd_spam_x_dipole = 100;
FILE_STATIC uint8_t gcmd_spam_y_dipole = 100;
FILE_STATIC uint8_t gcmd_spam_z_dipole = 100;

FILE_STATIC int8_t override_factor_x = POP_OFF_SCALE_FACTOR;
FILE_STATIC int8_t override_factor_y = POP_OFF_SCALE_FACTOR;
FILE_STATIC int8_t override_factor_z = POP_OFF_SCALE_FACTOR;

FILE_STATIC float gcmd_dipole_gain_factor_x = 1.0;
FILE_STATIC float gcmd_dipole_gain_factor_y = 1.0;
FILE_STATIC float gcmd_dipole_gain_factor_z = 1.0;

FILE_STATIC uint8_t spam_control_time_change_flag = 0;
FILE_STATIC uint8_t spam_control_switch_change_flag = 0;
FILE_STATIC spam_axis current_spam_axis = SPAM_X;

FILE_STATIC uint8_t max_tumbling_time_change_flag = 0;

FILE_STATIC uint8_t mtq_phase_change_flag = 0;

/* holds magnetometer data read every 10 hz */
FILE_STATIC MagnetometerData* continuous_bdot_mag_data;

/* holds only the magnetometer that are "valid", meaning collected
 * only under valid conditions.
 * Valid conditions: 1. Magnetorquer is not on
 *                   2. Magnetometer  is set to be in normal configurations */
FILE_STATIC MagnetometerData valid_bdot_mag_data;

/* sensor proc magnetometer data. These data were decided to be unfiltered data,
 * meaning the data can be data collected when the magnetorquer is on. */
FILE_STATIC MagnetometerData sp_mag1_data;
FILE_STATIC MagnetometerData sp_mag2_data;

/* flag set to see when new magnetometer data are sent over to bdot from sensorproc */
FILE_STATIC uint8_t sp_mag1_new_data_flag;
FILE_STATIC uint8_t sp_mag2_new_data_flag;

/* stores which magnetometer bdot is curently "listening" to */
FILE_STATIC mag_src current_listening_mag = BDOT_MAG;

/* lets ground station pick which mag to use */
FILE_STATIC uint8_t mag_selection_mode = MAG_BEST_FIT_OVERRIDE;

FILE_STATIC uint8_t mag_selection_switch_flag = 0;

/* mag number for magnetometer reading. for questions, contact david */
FILE_STATIC hMag mag_num;
/***************************************************************/


/******************Simulink Flags and Information***************/
FILE_STATIC uint8_t rt_flag = 0;
/***************************************************************/


/******************Timer Information***************************/
FILE_STATIC TIMER_HANDLE rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;

/* this is the timer that will check go off when it's time to check
 * to see which magnetometer to use. */
FILE_STATIC TIMER_HANDLE check_best_mag_timer = -1;
FILE_STATIC uint32_t check_best_mag_timer_ms = 60000; // 3 min
FILE_STATIC uint8_t check_best_mag_timer_on_flag = 0;

/* this is the timer that will check to see whether or not bdot has
 * reported tumbling continuously for a configurable amount of time.
 * If that time has reached, bdot should send dipoles of 0 to magnetorquer
 * until commands from ground are sent to "wake up" bdot OR rt onestep reports
 * not tumbling */
FILE_STATIC TIMER_HANDLE check_nap_status_timer = -1;
FILE_STATIC uint32_t check_nap_status_timer_ms = 1200000; // 20 min
//FILE_STATIC uint32_t check_nap_status_timer_ms = 5000; // 5 sec
FILE_STATIC uint8_t nap_status_timer_on_flag = 0;
#pragma PERSISTENT(check_nap_status_timer_ms);

/* this is the timer that will keep track of how long SPAM is */
FILE_STATIC TIMER_HANDLE spam_timer = -1;
FILE_STATIC uint32_t spam_off_timer_ms = 3600000; // 1 hour;
FILE_STATIC uint32_t spam_on_timer_ms = 120000; // 6 min
//FILE_STATIC uint32_t spam_off_timer_ms = 15000; // 2 min
//FILE_STATIC uint32_t spam_on_timer_ms = 20000; // 20 secs

FILE_STATIC uint8_t spam_timer_on_flag = 0;
#pragma PERSISTENT(spam_off_timer_ms)
#pragma PERSISTENT(spam_on_timer_ms)

/* this is the timer that will indicate when to collect spam */
FILE_STATIC TIMER_HANDLE spam_avg_timer = -1;
FILE_STATIC uint8_t spam_avg_timer_on_flag = 0;
FILE_STATIC uint32_t spam_on_avg_timer_ms = 500;
FILE_STATIC uint32_t spam_off_avg_timer_ms = 500;
/***************************************************************/


/*******************RollCall***********************************/
FILE_STATIC aggVec_f rc_temp;
FILE_STATIC aggVec_i magX;
FILE_STATIC aggVec_i magY;
FILE_STATIC aggVec_i magZ;

FILE_STATIC aggVec_i dipole_x_agg;
FILE_STATIC aggVec_i dipole_y_agg;
FILE_STATIC aggVec_i dipole_z_agg;

FILE_STATIC aggVec_i spam_on_x_avg_agg[3];
FILE_STATIC aggVec_i spam_on_y_avg_agg[3];
FILE_STATIC aggVec_i spam_on_z_avg_agg[3];

FILE_STATIC int16_t spam_on_x_avg[3];
FILE_STATIC int16_t spam_on_y_avg[3];
FILE_STATIC int16_t spam_on_z_avg[3];

FILE_STATIC aggVec_i spam_off_x_avg_agg[3];
FILE_STATIC aggVec_i spam_off_y_avg_agg[3];
FILE_STATIC aggVec_i spam_off_z_avg_agg[3];

FILE_STATIC int16_t spam_off_x_avg[3];
FILE_STATIC int16_t spam_off_y_avg[3];
FILE_STATIC int16_t spam_off_z_avg[3];

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulateH1, rcPopulateH2, rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5,
 rcPopulate6, rcPopulate7, rcPopulate8, rcPopulate9, rcPopulate10
};
/**************************************************************/


/***************************************************************/
/*******************Agg Library***********************************/
/***************************************************************/

int main(void)
{
    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, NULL);
#endif  //  __DEBUG__

    /* Sets up can, and all the other steps */
    initial_setup();
    /* Turn on rt onestep callback timer */
    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
    startCallback(rtOneStep_timer);
    /* Turns on timer for checking best fit magnetometer */
    /* Turns on spam timer for the time it will not be in spam mode */
    start_spam_timer(spam_off_timer_ms);
    /* Turns on the timer that will report 1 if satellite is tumbling for too long */
    start_check_nap_status_timer();
    start_check_best_mag_timer();
    while (1)
    {
        /* determine the state of bdot */
        determine_bdot_state();
        /* update sensor proc magnetometer values */
        process_sp_mag();
        /* determine the best magnetometer to listen to */
        determine_best_fit_mag();

        if(rt_flag)
        {
            P3OUT ^= BIT5;

            /* read magnetometer data every 10hz, but only feed in magnetometer data to rt onestep
             * that's "valid". */
            read_magnetometer_data();

            update_valid_mag_data();

            /* update data that will be fed into rt onestep */
            update_simulink_info();

            /* perform rt onestep after getting new bdot magnetometer readings */
            rt_OneStep();

            /* determine next mtq command to send out */
            determine_mtq_commands();

            /* send backchannel telemetry */
            send_cosmos_telem();

            /* Determine if mtq is ready to actuate */
            if(mtq_state == MTQ_MEASUREMENT_PHASE)
            {
                /* get new magnetometer data from bdot magnetometer. */
                send_dipole_packet(bdot_perspective_mtq_info.xDipole,
                                   bdot_perspective_mtq_info.yDipole,
                                   bdot_perspective_mtq_info.zDipole);
                // debugging purposes
                mtq_last_known_state.xDipole = bdot_perspective_mtq_info.xDipole;
                mtq_last_known_state.yDipole = bdot_perspective_mtq_info.yDipole;
                mtq_last_known_state.zDipole = bdot_perspective_mtq_info.zDipole;
            }

            /* update rollcall data */
            updateRCData();
            /* clear rt_flag */
            rt_flag = 0;
        }
        /* update rollcall "aggressively" - David */
        rollcallUpdate();
    }
    return 0;
}

FILE_STATIC void initial_setup()
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
    bcbinPopulateHeader(&bdot_state_cosmos.header, TLM_ID_BDOT_STATE_STATUS, sizeof(bdot_state_cosmos));
    bcbinPopulateMeta(&metaSeg, sizeof(metaSeg));

    // not using these right now
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));
    bcbinPopulateHeader(&polling_timer_info.header, TLM_ID_POLLING_TIMER, sizeof(polling_timer_info));

    initialize_aggregate();

    /* initialize rollcall */
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    /* initialize timer */
    initializeTimer();
}

void determine_bdot_state()
{
    clear_update_bdot_state_flags();
    switch (bdot_state)
    {
        case NORMAL_MODE:
            /* if rt onestep reports tumbling for too long, put bdot into sleep mode */
            if(check_check_nap_status_timer())
            {
                end_spam_timer();
                start_spam_timer(spam_off_timer_ms);
                bdot_state = SLEEP_MODE;
            }
            /* if rt onestep reports not tumbling, restart check nap status timer */
            /* if rt onestep does report tumbling, then there's no reason to go into SPAM mode */
            if(!rtY.tumble)
            {
                end_check_nap_status_timer();
                start_check_nap_status_timer();
                /* ground commands determines that spam mode is enable, then check if spam timer is done */
                if(check_spam_timer() && gcmd_spam_control_switch == SPAM_ON)
                {
                    last_bdot_state = NORMAL_MODE;
                    bdot_state = SPAM_MODE;
                    current_spam_axis = SPAM_X;
                    reset_spam_avg_agg();
                    start_spam_timer(spam_on_timer_ms);
                }
            }
            break;
        case SLEEP_MODE:
            /* make sure nap timer is off. if it is off, then flag is 0 and the function does nothing */
            end_check_nap_status_timer();
            /* ground commands determines that spam mode is enable, then check if spam timer is done */
            if(check_spam_timer() && gcmd_spam_control_switch == SPAM_ON)
            {
                last_bdot_state = SLEEP_MODE;
                bdot_state = SPAM_MODE;
                current_spam_axis = SPAM_X;
                reset_spam_avg_agg();
                start_spam_timer(spam_on_timer_ms);
            }
            break;
        case SPAM_MODE:
            if(check_spam_avg_timer())
            {
                handle_spam_average(mag_measure_mtq_state);
            }
            /* if the amount of time spam is on is done, go back to last normal state (either NORMAL or SLEEP) */
            if(check_spam_timer())
            {
                determine_spam_axis();
                // done spamming all three axis
                if(current_spam_axis == SPAM_X)
                {
                    bdot_state = last_bdot_state;
                    /* make sure spam average timer is off */
                    end_spam_avg_timer();
                    /* start timer to time how long spam is off for */
                    start_spam_timer(spam_off_timer_ms);
                } else
                {
                    start_spam_timer(spam_on_timer_ms);
                }
            }
            break;
    }
}

void clear_update_bdot_state_flags()
{
    if(mag_selection_switch_flag)
    {
        if(mag_selection_mode == MAG_BEST_FIT_OVERRIDE)
        {
            end_check_best_mag_timer();
        } else if(mag_selection_mode == MAG_BEST_FIT_AUTO)
        {
            start_check_best_mag_timer();
        }
        mag_selection_switch_flag = 0;
    }
    if(spam_control_switch_change_flag)
    {
        if(gcmd_spam_control_switch == SPAM_OFF)
        {
            if(bdot_state == SPAM_MODE)
            {
                // go back to the previous state if current state is in spam: either normal or sleep.
                bdot_state = last_bdot_state;
            }
            end_spam_timer();
            end_spam_avg_timer();
        } else if(gcmd_spam_control_switch == SPAM_ON)
        {
            if(bdot_state == NORMAL_MODE || bdot_state == SLEEP_MODE)
            {
                last_bdot_state = bdot_state;
            }
            bdot_state = SPAM_MODE;
            end_spam_timer();
            start_spam_timer(spam_on_timer_ms);
            reset_spam_avg_agg();
        }
        spam_control_switch_change_flag = 0;
    }
    if(spam_control_time_change_flag)
    {
        end_spam_timer();
        if(bdot_state == SPAM_MODE)
        {
            start_spam_timer(spam_on_timer_ms);
            reset_spam_avg_agg();
            current_spam_axis = SPAM_X;
        } else if(bdot_state != SPAM_MODE)
        {
            start_spam_timer(spam_off_timer_ms);
        }
        spam_control_time_change_flag = 0;
    }

    if(max_tumbling_time_change_flag)
    {
        if(bdot_state == NORMAL_MODE)
        {
            end_check_nap_status_timer();
            start_check_nap_status_timer();
        }
        max_tumbling_time_change_flag = 0;
    }

    /* if there was a command from ground. flag is only set when the command from ground is different
     * from current bdot state */
    if(gcmd_bdot_state_flag)
    {
        /* in any case, when there's a command sent from ground to change state to
         * NORMAL or SLEEP, spam timer has to end and restart. */

        if(bdot_state == SPAM_MODE)
        {
            end_spam_timer();
        }
        if (gcmd_spam_control_switch == SPAM_ON)
        {
            if (gcmd_next_bdot_state == SPAM_MODE)
            {
                end_spam_timer();
                start_spam_timer(spam_on_timer_ms);
            } else
            {
                start_spam_timer(spam_off_timer_ms);
            }
        }
        end_check_nap_status_timer();

        if(bdot_state != SPAM_MODE)
        {
            last_bdot_state = bdot_state;
        }

        if(!(gcmd_spam_control_switch == SPAM_OFF && gcmd_next_bdot_state == SPAM_MODE))
        {
            bdot_state = gcmd_next_bdot_state;
        }

        /* if the next bdot state is not sleep mode, start the nap timer again */
        if(bdot_state == NORMAL_MODE)
        {
            start_check_nap_status_timer();
        }
        /* reset flag */
        gcmd_bdot_state_flag = 0;
    }

    if(mtq_phase_change_flag)
    {
        if(bdot_state == SPAM_MODE)
        {
            if(mtq_state == MTQ_MEASUREMENT_PHASE)
            {
                end_spam_avg_timer();
                start_spam_avg_timer(spam_on_avg_timer_ms);
            } else if(mtq_state == MTQ_ACTUATION_PHASE)
            {
                end_spam_avg_timer();
                start_spam_avg_timer(spam_off_avg_timer_ms);
            }
        }
        mtq_phase_change_flag = 0;
    }
}

void determine_spam_axis()
{
    uint8_t i;
    switch(current_spam_axis)
    {
        case SPAM_X:
            for(i = 0; i < 3; i++)
            {
                spam_on_x_avg[i] = aggVec_avg_i_i(&spam_on_x_avg_agg[i]);
                spam_off_x_avg[i] = aggVec_avg_i_i(&spam_off_x_avg_agg[i]);

            }
            current_spam_axis = SPAM_Y;
            break;
        case SPAM_Y:
            for(i = 0; i < 3; i++)
            {
                spam_on_y_avg[i] = aggVec_avg_i_i(&spam_on_y_avg_agg[i]);
                spam_off_y_avg[i] = aggVec_avg_i_i(&spam_off_y_avg_agg[i]);
            }
            current_spam_axis = SPAM_Z;
            break;
        case SPAM_Z:
            for(i = 0; i < 3; i++)
            {
                spam_on_z_avg[i] = aggVec_avg_i_i(&spam_on_z_avg_agg[i]);
                spam_off_z_avg[i] = aggVec_avg_i_i(&spam_off_z_avg_agg[i]);
            }
            current_spam_axis = SPAM_X;
            break;
    }
}

void reset_spam_avg_agg()
{
    uint8_t i;
    for(i = 0; i < 3; i++)
    {
        aggVec_reset((aggVec *) &spam_on_x_avg_agg[i]);
        aggVec_reset((aggVec *) &spam_on_y_avg_agg[i]);
        aggVec_reset((aggVec *) &spam_on_z_avg_agg[i]);
        aggVec_reset((aggVec *) &spam_off_x_avg_agg[i]);
        aggVec_reset((aggVec *) &spam_off_y_avg_agg[i]);
        aggVec_reset((aggVec *) &spam_off_z_avg_agg[i]);
    }
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
        /* B_meas_valid is actually thrown away in rt one step so it doesn't matter what value you put in */
        case BDOT_MAG:
            rtU.B_body_in_T[0] = valid_bdot_mag_data.convertedX;
            rtU.B_body_in_T[1] = valid_bdot_mag_data.convertedY;
            rtU.B_body_in_T[2] = valid_bdot_mag_data.convertedZ;
            break;
        case SP_MAG1:
            rtU.B_body_in_T[0] = sp_mag1_data.convertedX;
            rtU.B_body_in_T[1] = sp_mag1_data.convertedY;
            rtU.B_body_in_T[2] = sp_mag1_data.convertedZ;
            break;
        case SP_MAG2:
            rtU.B_body_in_T[0] = sp_mag2_data.convertedX;
            rtU.B_body_in_T[1] = sp_mag2_data.convertedY;
            rtU.B_body_in_T[2] = sp_mag2_data.convertedZ;
            break;
        default:
            rtU.B_body_in_T[0] = valid_bdot_mag_data.convertedX;
            rtU.B_body_in_T[1] = valid_bdot_mag_data.convertedY;
            rtU.B_body_in_T[2] = valid_bdot_mag_data.convertedZ;
    }
}

/* determines the best fit magnetometer */
void determine_best_fit_mag()
{
    /* if its time to check best fit mag, then restart the timer and if bdot state is in normal mode
     * and mag selction mode is in auto, then do the calculations */
    if(check_check_best_mag_timer())
    {
        if(bdot_state != NORMAL_MODE || (mag_selection_mode == MAG_BEST_FIT_OVERRIDE)) return;
        calc_best_fit_mag();
        start_check_best_mag_timer();
    }
}

void process_sp_mag()
{
    if(sp_mag1_new_data_flag)
    {
        if (mtq_state == MTQ_MEASUREMENT_PHASE)
        {
            convert_mag_data_raw_to_teslas(&sp_mag1_data);
        }
        sp_mag1_new_data_flag = 0;
    }
    if(sp_mag2_new_data_flag)
    {
        if (mtq_state == MTQ_MEASUREMENT_PHASE)
        {
            convert_mag_data_raw_to_teslas(&sp_mag2_data);
        }
        sp_mag2_new_data_flag = 0;
    }
}

/* Read magnetometer data continuously every 10Hz, ignoring whether or not mtq is actuating.
 * Function is for debugging purposes. */
void read_magnetometer_data()
{
   continuous_bdot_mag_data = magReadXYZData(mag_num, ConvertToTeslas);
   mag_measure_mtq_state = mtq_state;
}


/* Update valid bdot magnetometer. This function is only called when mtq is in measurement phase */
void update_valid_mag_data()
{
    if(!continuous_bdot_mag_data->isValid) return;
    if ((mtq_state == MTQ_MEASUREMENT_PHASE) && (bdot_state == NORMAL_MODE || bdot_state == SLEEP_MODE))
    {
        valid_bdot_mag_data.rawX = continuous_bdot_mag_data->rawX;
        valid_bdot_mag_data.rawY = continuous_bdot_mag_data->rawY;
        valid_bdot_mag_data.rawZ = continuous_bdot_mag_data->rawZ;
        valid_bdot_mag_data.convertedX = continuous_bdot_mag_data->convertedX;
        valid_bdot_mag_data.convertedY = continuous_bdot_mag_data->convertedY;
        valid_bdot_mag_data.convertedZ = continuous_bdot_mag_data->convertedZ;
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
void calc_best_fit_mag()
{
    /* find the median of the norm to determine best magnetometer to use. TODO: Think of a better, less costly method */
    float bdot_mag_norm = sqrt(abs(valid_bdot_mag_data.convertedX)^2 + abs(valid_bdot_mag_data.convertedY)^2 + abs(valid_bdot_mag_data.convertedZ)^2);
    float sp_mag1_norm =  sqrt(abs(sp_mag1_data.convertedX)^2 + abs(sp_mag1_data.convertedY)^2 + abs(sp_mag1_data.convertedZ)^2);
    float sp_mag2_norm = sqrt(abs(sp_mag2_data.convertedX)^2 + abs(sp_mag2_data.convertedY)^2 + abs(sp_mag2_data.convertedZ)^2);

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
    if(!check_best_mag_timer_on_flag)
    {
        check_best_mag_timer = timerPollInitializer(check_best_mag_timer_ms);
        check_best_mag_timer_on_flag = 0;
    }
}

uint8_t check_check_best_mag_timer()
{
    if(check_best_mag_timer_on_flag)
    {
        if(checkTimer(check_best_mag_timer))
        {
            check_best_mag_timer_on_flag = 0;
            return 1;
        }
    }
    return 0;
}

void end_check_best_mag_timer()
{
    if(check_best_mag_timer_on_flag)
    {
        endPollingTimer(check_best_mag_timer);
        check_best_mag_timer_on_flag = 0;
    }
}

void start_spam_timer(uint32_t spam_timer_ms)
{
    /* if timer is not on, then turn it on, otherwise do nothing */
    if(!spam_timer_on_flag)
    {
        spam_timer = timerPollInitializer(spam_timer_ms);
        spam_timer_on_flag = 1;
    }
}

void end_spam_timer()
{
    /* if timer is on, then end it, otherwise if it's already off, then do nothing */
    if(spam_timer_on_flag)
    {
        endPollingTimer(spam_timer);
        spam_timer_on_flag = 0;
    }
}

uint8_t check_spam_timer()
{
    /* if timer is on, then check timer, otherwise if it's off, then do nothing */
    if(spam_timer_on_flag)
    {
        if(checkTimer(spam_timer))
        {
            spam_timer_on_flag = 0;
            return 1;
        }
    }
    return 0;
}

void start_spam_avg_timer(uint32_t spam_avg_timer_ms)
{
    if(!spam_avg_timer_on_flag)
    {
        spam_avg_timer = timerPollInitializer(spam_avg_timer_ms);
        spam_avg_timer_on_flag = 1;
    }
}

void end_spam_avg_timer()
{
    if(spam_avg_timer_on_flag)
    {
        endPollingTimer(spam_avg_timer);
        spam_avg_timer_on_flag = 0;
    }
}

uint8_t check_spam_avg_timer()
{
    /* if timer is on, then check timer, otherwise if it's off, then do nothing */
    if(spam_avg_timer_on_flag)
    {
        if(checkTimer(spam_avg_timer))
        {
            spam_avg_timer_on_flag = 0;
            return 1;
        }
    }
    return 0;
}

/* Start the timer that goes off when bdot has been tumbling for a certain amount of time */
void start_check_nap_status_timer()
{
    if(!nap_status_timer_on_flag)
    {
        check_nap_status_timer = timerPollInitializer(check_nap_status_timer_ms);
        nap_status_timer_on_flag = 1;
    }
}

void end_check_nap_status_timer()
{
    if(nap_status_timer_on_flag)
    {
        nap_status_timer_on_flag = 0;
        endPollingTimer(check_nap_status_timer);
    }
}

uint8_t check_check_nap_status_timer()
{
    if(nap_status_timer_on_flag)
    {
        if(checkTimer(check_nap_status_timer))
        {
            nap_status_timer_on_flag = 0;
            return 1;
        }
    }
    return 0;
}

void handle_spam_average(mtq_phase current_phase)
{
    if(current_phase == MTQ_ACTUATION_PHASE)
    {

        switch(current_spam_axis)
        {
            case SPAM_X:
                if(spam_on_x_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_on_x_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_on_x_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_on_x_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
            case SPAM_Y:
                if(spam_on_y_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_on_y_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_on_y_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_on_y_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
            case SPAM_Z:
                if(spam_on_z_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_on_z_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_on_z_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_on_z_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
        }
    } else if (current_phase == MTQ_MEASUREMENT_PHASE)
    {
        switch(current_spam_axis)
        {
            case SPAM_X:
                if(spam_off_x_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_off_x_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_off_x_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_off_x_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
            case SPAM_Y:
                if(spam_off_y_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_off_y_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_off_y_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_off_y_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
            case SPAM_Z:
                if(spam_off_z_avg_agg[0].meta.avgSumCount > MAX_SPAM_AVG_COUNT) return;
                aggVec_push_i(&spam_off_z_avg_agg[0], continuous_bdot_mag_data->rawX);
                aggVec_push_i(&spam_off_z_avg_agg[1], continuous_bdot_mag_data->rawY);
                aggVec_push_i(&spam_off_z_avg_agg[2], continuous_bdot_mag_data->rawZ);
                break;
        }
    }
}

/* determine that dipoles to send to mtq */
void determine_mtq_commands()
{
    /* rtY is a struct that rt onestep outputs that carries dipole and tumble status
       information. Sometimes rt onestep will spit out small numbers for dipoles but tumble
       status will be 0, in which case dipoles of value 0 should be sent to mtq. This is because
       tumble status is not sent to mtq, just the dipoles. */
    switch(bdot_state)
    {
        case NORMAL_MODE:
            bdot_perspective_mtq_info.tumble_status  = (uint8_t) rtY.tumble;
            if(bdot_perspective_mtq_info.tumble_status)
            {
                bdot_perspective_mtq_info.xDipole = find_max_dipole((int16_t) rtY.Dig_val[0] * RTY_TO_DIPOLE_FACTOR * override_factor_x * gcmd_dipole_gain_factor_x);
                bdot_perspective_mtq_info.yDipole = find_max_dipole((int16_t) rtY.Dig_val[1] * RTY_TO_DIPOLE_FACTOR * override_factor_y * gcmd_dipole_gain_factor_y);
                bdot_perspective_mtq_info.zDipole = find_max_dipole((int16_t) rtY.Dig_val[2] * RTY_TO_DIPOLE_FACTOR * override_factor_z * gcmd_dipole_gain_factor_z);
            } else
            {
                bdot_perspective_mtq_info.xDipole = 0;
                bdot_perspective_mtq_info.yDipole = 0;
                bdot_perspective_mtq_info.zDipole = 0;
            }
            break;
        case SLEEP_MODE:
            bdot_perspective_mtq_info.xDipole = 0;
            bdot_perspective_mtq_info.yDipole = 0;
            bdot_perspective_mtq_info.zDipole = 0;
            break;
        case SPAM_MODE:
            if(current_spam_axis == SPAM_X)
            {
                bdot_perspective_mtq_info.xDipole = gcmd_spam_x_dipole;
                bdot_perspective_mtq_info.yDipole = 0;
                bdot_perspective_mtq_info.zDipole = 0;
            } else if(current_spam_axis == SPAM_Y)
            {
                bdot_perspective_mtq_info.xDipole = 0;
                bdot_perspective_mtq_info.yDipole = gcmd_spam_y_dipole;
                bdot_perspective_mtq_info.zDipole = 0;
            } else if(current_spam_axis == SPAM_Z)
            {
                bdot_perspective_mtq_info.xDipole = 0;
                bdot_perspective_mtq_info.yDipole = 0;
                bdot_perspective_mtq_info.zDipole = gcmd_spam_z_dipole;
            } else
            {
                bdot_perspective_mtq_info.xDipole = 0;
                bdot_perspective_mtq_info.yDipole = 0;
                bdot_perspective_mtq_info.zDipole = 0;
            }
            break;
        default:
            bdot_perspective_mtq_info.xDipole = 0;
            bdot_perspective_mtq_info.yDipole = 0;
            bdot_perspective_mtq_info.zDipole = 0;
    }
    aggVec_push_i(&dipole_x_agg, bdot_perspective_mtq_info.xDipole);
    aggVec_push_i(&dipole_y_agg, bdot_perspective_mtq_info.yDipole);
    aggVec_push_i(&dipole_z_agg, bdot_perspective_mtq_info.zDipole);
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
    send_bdot_state_status_cosmos();
    send_simulink_segment_cosmos();
//    send_sp_mag1_reading_cosmos();
//    send_sp_mag2_reading_cosmos();
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

int8_t find_max_dipole(int16_t dipole)
{
    if(dipole > 100) return 100;
    if(dipole < -100) return -100;
    return ((int8_t) dipole);
}

/* Can packet receive callback function */
void can_rx_callback(CANPacket *packet)
{
    mtq_ack ack = {0};
    gcmd_mtq_pwm_time mtq_pwm = {0};
    sensorproc_mag mag1 = {0};
    sensorproc_mag2 mag2 = {0};
    gcmd_bdot_spam spam = {0};
    gcmd_bdot_max_tumble tumble = {0};
    gcmd_bdot_control control_bdot_state = {0};
    gcmd_bdot_pole_override pop = {0};
    gcmd_bdot_mag_control mag_control = {0};
    switch(packet->id)
    {
        case CAN_ID_MTQ_ACK:
            // measurement_phase = 0;
            // actuation_phase = 1;
            mtq_phase_change_flag = 1;
            decodemtq_ack(packet, &ack);
            if(ack.mtq_ack_phase == CAN_ENUM_MTQ_PHASE_MEASUREMENT_PHASE)
            {
                mtq_state = MTQ_MEASUREMENT_PHASE;
            }
            else if(ack.mtq_ack_phase == CAN_ENUM_MTQ_PHASE_ACTUATION_PHASE)
            {
                mtq_state = MTQ_ACTUATION_PHASE;
            } else if(ack.mtq_ack_phase == CAN_ENUM_MTQ_PHASE_PMS_PHASE)
            {
                mtq_state = MTQ_PMS_PHASE;
            }
            break;
        case CAN_ID_SENSORPROC_MAG:
            decodesensorproc_mag(packet, &mag1);
            if(mag1.sensorproc_mag_valid)
            {
                sp_mag1_data.rawX = mag1.sensorproc_mag_x;
                sp_mag1_data.rawY = mag1.sensorproc_mag_y;
                sp_mag1_data.rawZ = mag1.sensorproc_mag_z;
                sp_mag1_new_data_flag = 1;
            }
            break;
        case CAN_ID_SENSORPROC_MAG2:
            decodesensorproc_mag2(packet, &mag2);
            if(mag2.sensorproc_mag2_valid)
            {
                sp_mag2_data.rawX = mag2.sensorproc_mag2_x;
                sp_mag2_data.rawY = mag2.sensorproc_mag2_y;
                sp_mag2_data.rawZ = mag2.sensorproc_mag2_z;
                sp_mag2_new_data_flag = 1;
            }
            break;
        case CAN_ID_GCMD_BDOT_CONTROL:
            decodegcmd_bdot_control(packet, &control_bdot_state);
            select_mode_operation((bdot_state_mode) control_bdot_state.gcmd_bdot_control_mode);
            break;
        case CAN_ID_GCMD_BDOT_SPAM:
            decodegcmd_bdot_spam(packet, &spam);
            spam_control_operation(spam.gcmd_bdot_spam_time_off, spam.gcmd_bdot_spam_time_on, spam.gcmd_bdot_spam_control,
                                   spam.gcmd_bdot_spam_magnitude_x, spam.gcmd_bdot_spam_magnitude_y, spam.gcmd_bdot_spam_magnitude_z);
            break;
        case CAN_ID_GCMD_BDOT_MAX_TUMBLE:
            decodegcmd_bdot_max_tumble(packet, &tumble);
            change_max_tumble_time(tumble.gcmd_bdot_max_tumble_time);
            break;
        case CAN_ID_GCMD_BDOT_POLE_OVERRIDE:
            decodegcmd_bdot_pole_override(packet, &pop);
            bdot_pop_operation(pop.gcmd_bdot_pole_override_x, pop.gcmd_bdot_pole_override_y, pop.gcmd_bdot_pole_override_z);
            break;
        case CAN_ID_GCMD_MTQ_PWM_TIME:
            decodegcmd_mtq_pwm_time(packet, &mtq_pwm);
            change_spam_avg_time(mtq_pwm.gcmd_mtq_pwm_time_measurement, mtq_pwm.gcmd_mtq_pwm_time_actuation);
            break;
        case CAN_ID_GCMD_BDOT_MAG_CONTROL:
            decodegcmd_bdot_mag_control(packet, &mag_control);
            mag_select_switch(mag_control.gcmd_bdot_mag_control_mode);
            break;
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
        {
            gcmd_reset_minmax pktRst;
            decodegcmd_reset_minmax(packet, &pktRst);
            if(pktRst.gcmd_reset_minmax_bdot)
                reset_aggregate();
        }
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
    bdot_magnetometer_data_cosmos.xMag = valid_bdot_mag_data.convertedX * 1e9;
    bdot_magnetometer_data_cosmos.yMag = valid_bdot_mag_data.convertedY * 1e9;
    bdot_magnetometer_data_cosmos.zMag = valid_bdot_mag_data.convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &bdot_magnetometer_data_cosmos, sizeof(bdot_magnetometer_data_cosmos));
}

/* send magnetometer reading segment through backchannel */
void send_sp_mag1_reading_cosmos()
{
    sp_mag1_data_cosmos.xMag = sp_mag1_data.convertedX * 1e9;
    sp_mag1_data_cosmos.yMag = sp_mag1_data.convertedY * 1e9;
    sp_mag1_data_cosmos.zMag = sp_mag1_data.convertedZ * 1e9;

    bcbinSendPacket((uint8_t *) &sp_mag1_data_cosmos, sizeof(sp_mag1_data_cosmos));
}

/* send magnetometer reading segment through backchannel */
void send_sp_mag2_reading_cosmos()
{
    sp_mag2_data_cosmos.xMag = sp_mag2_data.convertedX * 1e9;
    sp_mag2_data_cosmos.yMag = sp_mag2_data.convertedY * 1e9;
    sp_mag2_data_cosmos.zMag = sp_mag2_data.convertedZ * 1e9;

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

void send_bdot_state_status_cosmos()
{
    bdot_state_cosmos.state_status = bdot_state;
    bdot_state_cosmos.mag_selection_mode = mag_selection_mode;
    bdot_state_cosmos.current_listening_mag = current_listening_mag;
    bcbinSendPacket((uint8_t *) &bdot_state_cosmos, sizeof(bdot_state_cosmos));
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

/* update rollcall packet */
void updateRCData()
{
    aggVec_push_i(&magX, valid_bdot_mag_data.rawX);
    aggVec_push_i(&magY, valid_bdot_mag_data.rawY);
    aggVec_push_i(&magZ, valid_bdot_mag_data.rawZ);
    aggVec_push_f(&rc_temp, asensorReadIntTempC());
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

void select_mode_operation(uint8_t reading_mode_selection)
{
    switch(reading_mode_selection)
    {
        case NORMAL_READING_OPERATION:
            if(bdot_state == NORMAL_MODE) return;
            gcmd_next_bdot_state = NORMAL_MODE;
            gcmd_bdot_state_flag = 1;
            break;
        case NAP_OPERATION:
            if(bdot_state == SLEEP_MODE) return;
            gcmd_next_bdot_state = SLEEP_MODE;
            gcmd_bdot_state_flag = 1;
            break;
        case SPAM_OPERATION:
            if(bdot_state == SPAM_MODE) return;
            gcmd_next_bdot_state = SPAM_MODE;
            gcmd_bdot_state_flag = 1;
            break;
    }
}



/* Supporting function that carries out ground command */
void mag_select_switch(uint8_t mag_selection)
{
    switch(mag_selection)
    {
        case CMD_SELECT_AUTO:
            if(mag_selection_mode == MAG_BEST_FIT_OVERRIDE)
            {
                mag_selection_switch_flag = 1;
            }
            mag_selection_mode = MAG_BEST_FIT_AUTO;
            break;
        case CMD_SELECT_BDOT_MAG:
            if(mag_selection_mode == MAG_BEST_FIT_AUTO)
            {
                mag_selection_switch_flag = 1;
            }
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            current_listening_mag = BDOT_MAG;
            break;
        case CMD_SELECT_SP_MAG1:
            if(mag_selection_mode == MAG_BEST_FIT_AUTO)
            {
                mag_selection_switch_flag = 1;
            }
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            current_listening_mag = SP_MAG1;
            break;
        case CMD_SELECT_SP_MAG2:
            if(mag_selection_mode == MAG_BEST_FIT_AUTO)
            {
                mag_selection_switch_flag = 1;
            }
            mag_selection_mode = MAG_BEST_FIT_OVERRIDE;
            current_listening_mag = SP_MAG2;
            break;
        default:
            break;
    }
}

void change_max_tumble_time(uint16_t max_tumble_time_min)
{
    check_nap_status_timer_ms = ((uint32_t) max_tumble_time_min) * MINUTES_TO_MILLISEC_CONVERSION_FACTOR;
    max_tumbling_time_change_flag = 1;
}

void spam_control_operation(uint16_t off_time_min, uint16_t on_time_min, uint8_t spam_switch, int8_t x_dipole, int8_t y_dipole, int8_t z_dipole)
{
    if (off_time_min > 0 && on_time_min > 0 && spam_switch == SPAM_ON)
    {
        spam_control_time_change_flag = 1;
        spam_off_timer_ms = ((uint32_t) off_time_min * MINUTES_TO_MILLISEC_CONVERSION_FACTOR);
        spam_on_timer_ms = ((uint32_t) on_time_min * MINUTES_TO_MILLISEC_CONVERSION_FACTOR) / 3; // divide by three for three axis
    }
    gcmd_spam_x_dipole = x_dipole;
    gcmd_spam_y_dipole = y_dipole;
    gcmd_spam_z_dipole = z_dipole;

    if(spam_switch != gcmd_spam_control_switch)
    {
        spam_control_switch_change_flag = 1;
        if(spam_switch == SPAM_OFF)
        {
            gcmd_spam_control_switch = SPAM_OFF;
        }
        else if(spam_switch == SPAM_ON)
        {
            gcmd_spam_control_switch = SPAM_ON;
        }
    }
}

void bdot_pop_operation(uint8_t pop_control_x, uint8_t pop_control_y, uint8_t pop_control_z)
{
    if(pop_control_x == POP_ON)
    {
        override_factor_x = POP_ON_SCALE_FACTOR;
    } else if(pop_control_x == POP_OFF)
    {
        override_factor_x = POP_OFF_SCALE_FACTOR;
    }

    if(pop_control_y == POP_ON)
    {
        override_factor_y = POP_ON_SCALE_FACTOR;
    } else if(pop_control_y == POP_OFF)
    {
        override_factor_y = POP_OFF_SCALE_FACTOR;
    }

    if(pop_control_x == POP_ON)
    {
        override_factor_x = POP_ON_SCALE_FACTOR;
    } else if(pop_control_x == POP_OFF)
    {
        override_factor_x = POP_OFF_SCALE_FACTOR;
    }
}

void bdot_dipole_gain_operation(uint8_t gain_control_x, uint8_t gain_control_y, uint8_t gain_control_z)
{
    gcmd_dipole_gain_factor_x = (float) gain_control_x / 100;
    gcmd_dipole_gain_factor_y = (float) gain_control_y / 100;
    gcmd_dipole_gain_factor_z = (float) gain_control_z / 100;
}

void change_spam_avg_time(uint8_t mtq_pwm_measurement, uint8_t mtq_pwm_actuation)
{
    spam_off_avg_timer_ms = mtq_pwm_measurement / 2;
    spam_on_avg_timer_ms = mtq_pwm_actuation / 2;
}

void initialize_aggregate()
{
    /* TODO: Add comment */
    aggVec_init_f(&rc_temp);
    aggVec_init_i(&magX);
    aggVec_init_i(&magY);
    aggVec_init_i(&magZ);
    aggVec_init_i_Var(&dipole_x_agg);
    aggVec_init_i_Var(&dipole_y_agg);
    aggVec_init_i_Var(&dipole_z_agg);
    uint8_t i;
    for(i = 0; i < 3; i++)
    {
        aggVec_init_i(&spam_on_x_avg_agg[i]);
        aggVec_init_i(&spam_on_y_avg_agg[i]);
        aggVec_init_i(&spam_on_z_avg_agg[i]);
        aggVec_init_i(&spam_off_x_avg_agg[i]);
        aggVec_init_i(&spam_off_y_avg_agg[i]);
        aggVec_init_i(&spam_off_z_avg_agg[i]);
    }
}

void reset_aggregate()
{
    aggVec_reset((aggVec *)&rc_temp);
    aggVec_reset((aggVec *)&magX);
    aggVec_reset((aggVec *)&magY);
    aggVec_reset((aggVec *)&magZ);
    aggVec_reset((aggVec *)&dipole_x_agg);
    aggVec_reset((aggVec *)&dipole_y_agg);
    aggVec_reset((aggVec *)&dipole_z_agg);
    uint8_t i;
    for(i = 0; i < 3; i++)
    {
        aggVec_reset((aggVec *)&spam_on_x_avg_agg[i]);
        aggVec_reset((aggVec *)&spam_on_y_avg_agg[i]);
        aggVec_reset((aggVec *)&spam_on_z_avg_agg[i]);
        aggVec_reset((aggVec *)&spam_off_x_avg_agg[i]);
        aggVec_reset((aggVec *)&spam_off_y_avg_agg[i]);
        aggVec_reset((aggVec *)&spam_off_z_avg_agg[i]);
    }
}

void rcPopulateH1(CANPacket *out)
{
    rc_adcs_bdot_h1 rc;
    rc.rc_adcs_bdot_h1_reset_count = bspGetResetCount();
    rc.rc_adcs_bdot_h1_sysrstiv = SYSRSTIV;
    rc.rc_adcs_bdot_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&rc_temp));
    rc.rc_adcs_bdot_h1_temp_min = compressMSPTemp(aggVec_min_f(&rc_temp));
    rc.rc_adcs_bdot_h1_reset_count = 0;
    aggVec_as_reset((aggVec *)&rc_temp);
    encoderc_adcs_bdot_h1(&rc, out);
}

void rcPopulateH2(CANPacket *out)
{
    rc_adcs_bdot_h2 rc;
    rc.rc_adcs_bdot_h2_canrxerror = canRxErrorCheck();
    encoderc_adcs_bdot_h2(&rc, out);
}

void rcPopulate1(CANPacket *out)
{

    rc_adcs_bdot_1 rc;
    rc.rc_adcs_bdot_1_spam_on_x_mtq_x = spam_on_x_avg[0];
    rc.rc_adcs_bdot_1_spam_on_x_mtq_y = spam_on_x_avg[1];
    rc.rc_adcs_bdot_1_spam_on_x_mtq_z = spam_on_x_avg[2];

    rc.rc_adcs_bdot_1_spam_on_y_mtq_x = spam_on_y_avg[0];

    encoderc_adcs_bdot_1(&rc, out);
}

void rcPopulate2(CANPacket *out)
{
    rc_adcs_bdot_2 rc;
    rc.rc_adcs_bdot_2_mag_x_min = aggVec_min_i(&magX);
    rc.rc_adcs_bdot_2_mag_x_max = aggVec_max_i(&magX);
    rc.rc_adcs_bdot_2_mag_x_avg = aggVec_avg_i_i(&magX);
    rc.rc_adcs_bdot_2_mag_y_min = aggVec_min_i(&magY);
    aggVec_as_reset((aggVec *)&magX);
    encoderc_adcs_bdot_2(&rc, out);
}

void rcPopulate3(CANPacket *out)
{
    rc_adcs_bdot_3 rc = {0};
    rc.rc_adcs_bdot_3_mag_y_max = aggVec_max_i(&magY);
    rc.rc_adcs_bdot_3_mag_y_avg = aggVec_avg_i_i(&magY);
    rc.rc_adcs_bdot_3_mag_z_min = aggVec_min_i(&magZ);
    rc.rc_adcs_bdot_3_mag_z_max = aggVec_max_i(&magZ);
    aggVec_as_reset((aggVec *)&magY);
    encoderc_adcs_bdot_3(&rc, out);
}


void rcPopulate4(CANPacket *out)
{
    rc_adcs_bdot_4 rc = {0};
    rc.rc_adcs_bdot_4_mag_z_avg = aggVec_avg_i_i(&magZ);
    rc.rc_adcs_bdot_4_tumble = rtY.tumble;
    rc.rc_adcs_bdot_4_spam_on_y_mtq_y = spam_on_y_avg[1];
    rc.rc_adcs_bdot_4_spam_on_y_mtq_z = spam_on_y_avg[2];
    aggVec_as_reset((aggVec *)&magZ);
    encoderc_adcs_bdot_4(&rc, out);
}

void rcPopulate5(CANPacket *out)
{
    rc_adcs_bdot_5 rc = {0};
    rc.rc_adcs_bdot_5_spam_on_z_mtq_x = spam_on_z_avg[0];
    rc.rc_adcs_bdot_5_spam_on_z_mtq_y = spam_on_z_avg[1];
    rc.rc_adcs_bdot_5_spam_on_z_mtq_z = spam_on_z_avg[2];
    encoderc_adcs_bdot_5(&rc, out);
}

void rcPopulate6(CANPacket *out)
{
    rc_adcs_bdot_6 rc = {0};
    rc.rc_adcs_bdot_6_current_state = (uint8_t) bdot_state;
    rc.rc_adcs_bdot_6_gain_ovr_status_x = (gcmd_dipole_gain_factor_x != 1.0);
    rc.rc_adcs_bdot_6_gain_ovr_status_y = (gcmd_dipole_gain_factor_y != 1.0);
    rc.rc_adcs_bdot_6_gain_ovr_status_z = (gcmd_dipole_gain_factor_z != 1.0);
    rc.rc_adcs_bdot_6_mag_control = mag_selection_mode;
    rc.rc_adcs_bdot_6_max_tumble_time = (uint16_t)(check_nap_status_timer_ms / MINUTES_TO_MILLISEC_CONVERSION_FACTOR);
    rc.rc_adcs_bdot_6_pop_status_x = !(override_factor_x == POP_OFF_SCALE_FACTOR);
    rc.rc_adcs_bdot_6_pop_status_y = !(override_factor_y == POP_OFF_SCALE_FACTOR);
    rc.rc_adcs_bdot_6_pop_status_z = !(override_factor_z == POP_OFF_SCALE_FACTOR);
    rc.rc_adcs_bdot_6_spam_control = (gcmd_spam_control_switch == SPAM_ON);
    rc.rc_adcs_bdot_6_spam_off_time = spam_off_timer_ms / MINUTES_TO_MILLISEC_CONVERSION_FACTOR;
    rc.rc_adcs_bdot_6_spam_on_time = spam_on_timer_ms / MINUTES_TO_MILLISEC_CONVERSION_FACTOR;
    encoderc_adcs_bdot_6(&rc, out);
}

void rcPopulate7(CANPacket *out)
{
    rc_adcs_bdot_7 rc = {0};
    rc.rc_adcs_bdot_7_spam_magnitude_x = gcmd_spam_x_dipole;
    rc.rc_adcs_bdot_7_spam_magnitude_y = gcmd_spam_y_dipole;
    rc.rc_adcs_bdot_7_spam_magnitude_z = gcmd_spam_z_dipole;
    rc.rc_adcs_bdot_7_spam_off_x_mtq_x = spam_off_x_avg[0];
    rc.rc_adcs_bdot_7_spam_off_x_mtq_y = spam_off_x_avg[1];
    encoderc_adcs_bdot_7(&rc, out);
}

void rcPopulate8(CANPacket *out)
{
    rc_adcs_bdot_8 rc = {0};
    rc.rc_adcs_bdot_8_spam_off_x_mtq_z = spam_off_x_avg[2];
    rc.rc_adcs_bdot_8_spam_off_y_mtq_x = spam_off_y_avg[0];
    rc.rc_adcs_bdot_8_spam_off_y_mtq_y = spam_off_y_avg[1];
    rc.rc_adcs_bdot_8_spam_off_y_mtq_z = spam_off_z_avg[2];
    encoderc_adcs_bdot_8(&rc, out);
}

void rcPopulate9(CANPacket * out)
{
    rc_adcs_bdot_9 rc = {0};
    rc.rc_adcs_bdot_9_spam_off_z_mtq_x = spam_off_z_avg[0];
    rc.rc_adcs_bdot_9_spam_off_z_mtq_y = spam_off_z_avg[1];
    rc.rc_adcs_bdot_9_spam_off_z_mtq_z = spam_off_z_avg[2];
    encoderc_adcs_bdot_9(&rc, out);

}

void rcPopulate10(CANPacket *out)
{
    rc_adcs_bdot_10 rc = {0};
    rc.rc_adcs_bdot_10_dipole_var_x = compressVariance(aggVec_var_i_f(&dipole_x_agg));
    rc.rc_adcs_bdot_10_dipole_var_y = compressVariance(aggVec_var_i_f(&dipole_y_agg));
    rc.rc_adcs_bdot_10_dipole_var_z = compressVariance(aggVec_var_i_f(&dipole_z_agg));
    encoderc_adcs_bdot_10(&rc, out);
    aggVec_as_reset((aggVec *)&dipole_x_agg);
    aggVec_as_reset((aggVec *)&dipole_y_agg);
    aggVec_as_reset((aggVec *)&dipole_z_agg);
}


// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}

