#define AUTOCODE_UPDATE_DELAY_US 100000

#include <adcs_mpc.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"
#include "core/agglib.h"
#include "core/timer.h"

#include "sensors/magnetometer.h" // include for unit conversions
#include "sensors/imu.h"

#include "autocode/MSP_FSW.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;

// Autocode steps and timing
FILE_STATIC flag_t triggerStepFlag = FALSE;

// CAN temporary input
FILE_STATIC sp2fsw tmpSP2FSW;
FILE_STATIC mtq_ack tmpMtqAck;
FILE_STATIC estim_state tmpEstimState;
FILE_STATIC estim_sun_unit_x tmpESunx;
FILE_STATIC estim_sun_unit_y tmpESuny;
FILE_STATIC estim_sun_unit_z tmpESunz;
FILE_STATIC estim_mag_unit_x tmpEMagx;
FILE_STATIC estim_mag_unit_y tmpEMagy;
FILE_STATIC estim_mag_unit_z tmpEMagz;

// rollcall
FILE_STATIC aggVec_i rc_pointTrue;
FILE_STATIC aggVec_f rc_mspTemp;

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 // TODO
};

// Backchannel telemerty
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

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
    mod_status.startup_type = coreStartup(handlePPTFiringNotification, NULL);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)

    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
    //                               handleDebugStatusCallback,
    //                               handleDebugActionCallback);

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // set LED gpio pin to output
    LED_DIR |= LED_BIT;

    // populate segment headers
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

    // init temperature sensor
    asensorInit(Ref_2p5V);

    // init rollcall
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    // init autocode
    MSP_FSW_initialize();

    // initialize timers
    initializeTimer();
    int timerHandle = timerCallbackInitializer(&triggerStep,
                                               AUTOCODE_UPDATE_DELAY_US);
    startCallback(timerHandle);

    while (1)
    {
        // wait for quick timer to trigger for autocode
        while (!triggerStepFlag);
        triggerStepFlag = FALSE;

        LED_OUT ^= LED_BIT;

        // do non-autocode functions at 1 Hz (every 10 steps)
        static uint8_t i = 0;
        if (i++ == 10)
        {
            i = 0;

            // send basic subsystem telemetry
            // TODO move this to rollcall
            sendHealthSegment();
            sendMetaSegment();

            sendBackchannelTelem();
        }

        rollcallUpdate();

        // step autocode
        rt_OneStep();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

void triggerStep()
{
    triggerStepFlag = TRUE;
}

void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */
  __disable_interrupt();

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(rtM, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  __enable_interrupt();

  /* Set model inputs here */
  acceptInputs();

  /* Step the model */
  MSP_FSW_step();

  /* Get model outputs here */
  sendCANVelocityPointing();
  sendCANMtqCmd();

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

void canRxCallback(CANPacket *p)
{
    sensorproc_mag mag;
    sensorproc_imu imu;
    sensorproc_sun sun;

    real32_T *tmp_mag = tmpSP2FSW.mag_vec_body_T;
    real32_T *tmp_imu = tmpSP2FSW.gyro_omega_body_radps;
    real32_T *tmp_sun = tmpSP2FSW.sun_vec_body_sunsensor;

    switch (p->id)
    {
    case CAN_ID_CMD_ROLLCALL:
        rollcallStart();
        break;
    case CAN_ID_SENSORPROC_MAG:
        decodesensorproc_mag(p, &mag);
        tmp_mag[0] = magConvertRawToTeslas(mag.sensorproc_mag_x);
        tmp_mag[1] = magConvertRawToTeslas(mag.sensorproc_mag_y);
        tmp_mag[2] = magConvertRawToTeslas(mag.sensorproc_mag_z);
        tmp_mag[3] = mag.sensorproc_mag_valid;
        break;
    case CAN_ID_SENSORPROC_IMU:
        decodesensorproc_imu(p, &imu);
        tmp_imu[0] = imuConvertRawToRPS(imu.sensorproc_imu_x);
        tmp_imu[1] = imuConvertRawToRPS(imu.sensorproc_imu_y);
        tmp_imu[2] = imuConvertRawToRPS(imu.sensorproc_imu_z);
        tmp_imu[3] = imu.sensorproc_imu_valid;
        break;
    case CAN_ID_SENSORPROC_SUN:
        decodesensorproc_sun(p, &sun);
        tmp_sun[0] = sun.sensorproc_sun_x / INT16_MAX; // convert to unit vector
        tmp_sun[1] = sun.sensorproc_sun_y / INT16_MAX;
        tmp_sun[2] = sun.sensorproc_sun_z / INT16_MAX;
        tmp_sun[3] = sun.sensorproc_sun_valid;
        break;
    case CAN_ID_MTQ_ACK:
        decodemtq_ack(p, &tmpMtqAck);
        break;
    case CAN_ID_ESTIM_STATE:
        decodeestim_state(p, &tmpEstimState);
        break;
    case CAN_ID_ESTIM_MAG_UNIT_X:
        decodeestim_mag_unit_x(p, &tmpEMagx);
        break;
    case CAN_ID_ESTIM_MAG_UNIT_Y:
        decodeestim_mag_unit_y(p, &tmpEMagy);
        break;
    case CAN_ID_ESTIM_MAG_UNIT_Z:
        decodeestim_mag_unit_z(p, &tmpEMagz);
        break;
    case CAN_ID_ESTIM_SUN_UNIT_X:
        decodeestim_sun_unit_x(p, &tmpESunx);
        break;
    case CAN_ID_ESTIM_SUN_UNIT_Y:
        decodeestim_sun_unit_y(p, &tmpESuny);
        break;
    case CAN_ID_ESTIM_SUN_UNIT_Z:
        decodeestim_sun_unit_z(p, &tmpESunz);
        break;
    }
}

// copy CAN inputs from temporary storage to the autocode inputs
void acceptInputs()
{
    __disable_interrupt();

    // sensor proc outputs
    memcpy(rtU.mag_vec_body_T, tmpSP2FSW.mag_vec_body_T,
           4 * sizeof(tmpSP2FSW.mag_vec_body_T[0]));
    memcpy(rtU.gyro_omega_body_radps, tmpSP2FSW.gyro_omega_body_radps,
           4 * sizeof(tmpSP2FSW.gyro_omega_body_radps[0]));
    memcpy(rtU.sun_vec_body_sunsensor, tmpSP2FSW.sun_vec_body_sunsensor,
           4 * sizeof(tmpSP2FSW.sun_vec_body_sunsensor[0]));

    // MTQ outputs
    // TODO confirm interpretation that measurement phase == MT_valid
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        rtU.MT_valid[i] = tmpMtqAck.mtq_ack_phase != MEASUREMENT_PHASE;
    }

    // ESTIM outputs
    rtU.sc_above_gs = tmpEstimState.estim_state_above_gs;
    rtU.sc_in_sun = tmpEstimState.estim_state_in_sun;
    rtU.sc2sun_unit[0] = tmpESunx.estim_sun_unit_x_val;
    rtU.sc2sun_unit[1] = tmpESuny.estim_sun_unit_y_val;
    rtU.sc2sun_unit[2] = tmpESunz.estim_sun_unit_z_val;
    rtU.mag_eci_unit[0] = tmpEMagx.estim_mag_unit_x_val;
    rtU.mag_eci_unit[1] = tmpEMagy.estim_mag_unit_y_val;
    rtU.mag_eci_unit[2] = tmpEMagz.estim_mag_unit_z_val;

    __enable_interrupt();
}

void sendCANVelocityPointing()
{
    // send CAN packet
    CANPacket p;
    mpc_vp vp;
    vp.mpc_vp_status = rtY.point_true
            ? CAN_ENUM_BOOL_TRUE : CAN_ENUM_BOOL_FALSE;
    encodempc_vp(&vp, &p);
    canSendPacket(&p);

    // update rollcall data
    aggVec_i_push(&rc_pointTrue, rtY.point_true);
}

void sendCANMtqCmd()
{
    CANPacket p;
    cmd_mtq_fsw cmd;
    cmd.cmd_mtq_fsw_sc_mode = rtY.sc_mode;
    cmd.cmd_mtq_fsw_x = rtY.cmd_MT_fsw_dv[0];
    cmd.cmd_mtq_fsw_y = rtY.cmd_MT_fsw_dv[1];
    cmd.cmd_mtq_fsw_z = rtY.cmd_MT_fsw_dv[2];
    encodecmd_mtq_fsw(&cmd, &p);
    canSendPacket(&p);
}

void sendBackchannelTelem()
{
    output_segment out;
    memcpy(out.sc_quat, rtY.sc_quat, 4 * sizeof(rtY.sc_quat[0]));
    memcpy(out.body_rates, rtY.body_rates, 3 * sizeof(rtY.body_rates[0]));
    out.sc_mode = rtY.sc_mode; // TODO sc_mode or sc_modeb?
    out.point_true = rtY.point_true;
    out.sc_above_gs = rtY.sc_above_gsb;
    bcbinPopulateHeader(&out.header, TLM_ID_OUTPUT, sizeof(out));
    bcbinSendPacket((uint8_t *) &out, sizeof(out));

    mtqcmd_segment cmd;
    memcpy(cmd.cmd_MT_fsw_dv, rtY.cmd_MT_fsw_dv,
           3 * sizeof(rtY.cmd_MT_fsw_dv[0]));
    cmd.sc_mode = rtY.sc_modeb; // TODO sc_mode or sc_modeb?
    bcbinPopulateHeader(&cmd.header, TLM_ID_MTQCMD, sizeof(cmd));
    bcbinSendPacket((uint8_t *) &cmd, sizeof(cmd));
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
void sendHealthSegment()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // TODO determine overall health based on querying sensors for their health
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);

    // update temperature (in deci-Kelvin)
    aggVec_f_push(&rc_mspTemp, (hseg.inttemp + 273.15f) * 10);
}

void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}
