#define MOCK_TLE 1

#include <adcs_estim.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"
#include "core/agglib.h"
#include "core/debugtools.h"

#include "tle.h"

#include "autocode/MSP_env_estim0.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;

/* rollcall */

FILE_STATIC aggVec_f rc_mspTemp;
FILE_STATIC uint64_t metEpoch = 0;
FILE_STATIC uint64_t lastUsedMET = 0;

FILE_STATIC void rcPopulate1(CANPacket *out);
FILE_STATIC void rcPopulateH2(CANPacket *out);
FILE_STATIC void rcPopulate2(CANPacket *out);
FILE_STATIC void rcPopulate3(CANPacket *out);
FILE_STATIC void rcPopulate4(CANPacket *out);
FILE_STATIC void rcPopulate5(CANPacket *out);
FILE_STATIC void rcPopulate6(CANPacket *out);
FILE_STATIC void rcPopulate7(CANPacket *out);
FILE_STATIC void rcPopulate8(CANPacket *out);
FILE_STATIC void rcPopulate9(CANPacket *out);
FILE_STATIC void rcPopulate10(CANPacket *out);
FILE_STATIC void rcPopulate11(CANPacket *out);
FILE_STATIC void rcPopulate12(CANPacket *out);
FILE_STATIC void rcPopulate13(CANPacket *out);
FILE_STATIC void rcPopulate14(CANPacket *out);

FILE_STATIC rollcall_fn rollcallFunctions[] =
{
 rcPopulate1, rcPopulateH2, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5,
 rcPopulate6, rcPopulate7, rcPopulate8, rcPopulate9, rcPopulate10,
 rcPopulate11, rcPopulate12, rcPopulate13, rcPopulate14
};

#define NUM_ROLLCALL_PACKETS (sizeof(rollcallFunctions) / sizeof(rollcall_fn))

FILE_STATIC CANPacket rcCANPackets[NUM_ROLLCALL_PACKETS];

/* backchannel */

FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;

/* TLE */

// The latest TLE from CAN is held here and passed to autocode on each step
// TODO initialize it with a recent TLE before launch?
#pragma PERSISTENT(tle)
FILE_STATIC struct tle tle = { 0 };

/* functions */

FILE_STATIC void setInputs();
FILE_STATIC void sendTelemOverBackchannel();
FILE_STATIC void sendTelemOverCAN();

FILE_STATIC void sendHealthSegment();
FILE_STATIC void sendMetaSegment();

FILE_STATIC void rt_OneStep(void);

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
#if MOCK_TLE
    // TLE taken from SWISSCUBE.tle
    // https://github.com/tpreynolds/uw_hs1_adcs/blob/develop/adcs/sw/components/adcs_fsw/matlab/test/sgp4-test/SWISSCUBE/SWISSCUBE.tle
    // ID = 0
    tle.tle1.tle_1_bstar = 0.32923E-4;
    tle.tle1.tle_1_mna = 2.1593;
    tle.tle2.tle_2_day = (365.24 * 18) + 274.79166667;
    tle.tle3.tle_3_ecc = .0008911;
    tle.tle3.tle_3_inc = 98.5033;
    tle.tle4.tle_4_aop = 245.3514;
    tle.tle4.tle_4_raan = 67.1301;
    tle.tle5.tle_5_mnm = 14.56154823;

    rtU.MET_epoch = tle.tle2.tle_2_day * 24 * 60 * 60;
#endif
    tleInit(&tle, MOCK_TLE);
    canWrapInitWithFilter();
    setCANPacketRxCallback(canRxCallback);

    /* ----- SUBSYSTEM LOGIC -----*/
    // set LED gpio pin to output
    LED_DIR |= LED_BIT;

    // Fix phantom power over uart
	P3DIR |= BIT4;
	P3OUT |= BIT4;

    // populate segment headers
    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));

    // init temperature sensor
    asensorInit(Ref_2p5V);

    // init rollcall
    rollcallInitWithBuffer(rollcallFunctions, rcCANPackets, NUM_ROLLCALL_PACKETS);
    aggVec_init_f(&rc_mspTemp);

    // init autocode
    MSP_env_estim0_initialize();

    /*
     * TODO consider using a callback timer instead of a while loop. We'll
     * likely want to run as fast as possible, but it may also be worthwhile to
     * run at a fixed rate (using timers instead).
     */
    while (1)
    {
        LED_OUT ^= LED_BIT;

        // send basic subsystem telemetry
        // TODO move this to rollcall
        sendHealthSegment();
        sendMetaSegment();

        // update aggressively
        while (rollcallUpdate());

        // step autocode
        rt_OneStep();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
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
  setInputs();

  /* Step the model */
  MSP_env_estim0_step();

  /* Get model outputs here */
  sendTelemOverBackchannel();
  sendTelemOverCAN();

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
void handlePPTFiringNotification()
{
    __no_operation();
}

FILE_STATIC void setInputs()
{
    lastUsedMET = metConvertToInt(getMETTimestamp());
    rtU.MET = metConvertFromIntToSeconds(lastUsedMET);

    // input the TLE unless we're in the middle of reading it from CAN
    // disable interrupts so the TLE isn't modified during read
    __disable_interrupt();
    if (tleIsComplete(&tle))
    {
        double day = tleDay(&tle);
        rtU.orbit_tle[0] = 2000 + (uint8_t) (day / 365.24); // year is unused
        rtU.orbit_tle[1] = day;
        rtU.orbit_tle[2] = tleBStar(&tle);
        rtU.orbit_tle[3] = tleInc(&tle);
        rtU.orbit_tle[4] = tleRaan(&tle);
        rtU.orbit_tle[5] = tleEcc(&tle);
        rtU.orbit_tle[6] = tleAop(&tle);
        rtU.orbit_tle[7] = tleMna(&tle);
        rtU.orbit_tle[8] = tleMnm(&tle);
    }
    __enable_interrupt();
}

FILE_STATIC void sendTelemOverBackchannel()
{
    // send input TLE
    input_tle_segment tleSeg;
    tleSeg.year = rtU.orbit_tle[0];
    tleSeg.day = rtU.orbit_tle[1];
    tleSeg.bstar = rtU.orbit_tle[2];
    tleSeg.inc = rtU.orbit_tle[3];
    tleSeg.raan = rtU.orbit_tle[4];
    tleSeg.ecc = rtU.orbit_tle[5];
    tleSeg.aop = rtU.orbit_tle[6];
    tleSeg.mna = rtU.orbit_tle[7];
    tleSeg.mnm = rtU.orbit_tle[8];
    tleSeg.ack = tleAck(&tle);
    bcbinPopulateHeader(&tleSeg.header, TLM_ID_INPUT_TLE, sizeof(tleSeg));
    bcbinSendPacket((uint8_t *) &tleSeg, sizeof(tleSeg));

    // send MET
    input_met_segment metSeg;
    metSeg.met = rtU.MET;
    metSeg.epoch = rtU.MET_epoch;
    bcbinPopulateHeader(&metSeg.header, TLM_ID_INPUT_MET, sizeof(metSeg));
    bcbinSendPacket((uint8_t *) &metSeg, sizeof(metSeg));

    // send autocode outputs
    output_segment out;
    uint8_t i = 3;
    while (i-- > 0)
    {
        out.sc2sun_unit[i] = rtY.sc2sun_unit[i];
        out.mag_eci_unit[i] = rtY.mag_eci_unit[i];
        out.pos_eci_m[i] = rtY.pos_eci_m[i];
        out.vel_eci_mps[i] = rtY.vel_eci_mps[i];
    }
    out.SGP4_flag = rtY.SGP4_flag;
    out.sc_in_sun = rtY.sc_in_sun;
    out.sc_above_gs = rtY.sc_above_gs;
    bcbinPopulateHeader(&out.header, TLM_ID_OUTPUT, sizeof(out));
    bcbinSendPacket((uint8_t *) &out, sizeof(out));

}

/**
 * Keep trying to send a CAN packet until it really sends
 */
FILE_STATIC void waitAndSendCAN(CANPacket *p)
{
	while(canSendPacket(p));
}

FILE_STATIC void sendTelemOverCAN()
{
    CANPacket p;

    // send sc2sun_unit
    estim_sun_unit_x sunx = { rtY.sc2sun_unit[0] };
    encodeestim_sun_unit_x(&sunx, &p);
    waitAndSendCAN(&p);
    estim_sun_unit_y suny = { rtY.sc2sun_unit[1] };
    encodeestim_sun_unit_y(&suny, &p);
    waitAndSendCAN(&p);
    estim_sun_unit_z sunz = { rtY.sc2sun_unit[2] };
    encodeestim_sun_unit_z(&sunz, &p);
    waitAndSendCAN(&p);

    // send mag_eci_unit
    estim_mag_unit_x magx = { rtY.mag_eci_unit[0] };
    encodeestim_mag_unit_x(&magx, &p);
    waitAndSendCAN(&p);
    estim_mag_unit_y magy = { rtY.mag_eci_unit[1] };
    encodeestim_mag_unit_y(&magy, &p);
    waitAndSendCAN(&p);
    estim_mag_unit_z magz = { rtY.mag_eci_unit[2] };
    encodeestim_mag_unit_z(&magz, &p);
    waitAndSendCAN(&p);

    // send state
    estim_state state;
    state.estim_state_above_gs = rtY.sc_above_gs ? CAN_ENUM_BOOL_TRUE
            : CAN_ENUM_BOOL_FALSE;
    state.estim_state_in_sun = rtY.sc_in_sun ? CAN_ENUM_BOOL_TRUE
            : CAN_ENUM_BOOL_FALSE;
    encodeestim_state(&state, &p);
    waitAndSendCAN(&p);
}

void canRxCallback(CANPacket *p)
{
    __disable_interrupt();
    tleUpdate(p, &tle);
    __enable_interrupt();

    grnd_epoch ep;
    timeStamp t;

    switch (p->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
            break;
        case CAN_ID_GRND_EPOCH:
            decodegrnd_epoch(p, &ep);
            t = constructTimestamp(ep.grnd_epoch_val, ep.grnd_epoch_val_overflow);
            metEpoch = metConvertToInt(t);
            rtU.MET_epoch = metConvertFromIntToSeconds(metEpoch);
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
        {
            gcmd_reset_minmax pktRst;
            decodegcmd_reset_minmax(p, &pktRst);
            if(pktRst.gcmd_reset_minmax_estim)
                aggVec_reset((aggVec *)&rc_mspTemp);
        }
            break;
        case CAN_ID_RC_EPS_DIST_2:
        {
            rc_eps_dist_2 rc;
            decoderc_eps_dist_2(p, &rc);
            t = constructTimestamp(rc.rc_eps_dist_2_met,
                                   rc.rc_eps_dist_2_met_overflow);
            updateMET(t);
        }
            break;
        case CAN_ID_GCMD_DIST_RESET_MISSION: //clear persistent flags here
            bspClearResetCount();
            break;
    }
}

// Packetizes and sends backchannel health packet
// also invokes uart status handler
FILE_STATIC void sendHealthSegment()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // TODO determine overall health based on querying sensors for their health
    hseg.oms = OMS_Unknown;

    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandler(Entity_UART);

    // update temperature (in deci-Kelvin)
    aggVec_push_f(&rc_mspTemp, hseg.inttemp);
}

FILE_STATIC void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

FILE_STATIC void rcPopulate1(CANPacket *out)
{
    rc_adcs_estim_h1 rc;
    rc.rc_adcs_estim_h1_reset_count = bspGetResetCount();
    rc.rc_adcs_estim_h1_sysrstiv = bspGetResetReason();
    rc.rc_adcs_estim_h1_temp_min = compressMSPTemp(aggVec_min_f(&rc_mspTemp));
    rc.rc_adcs_estim_h1_temp_max = compressMSPTemp(aggVec_max_f(&rc_mspTemp));
    rc.rc_adcs_estim_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&rc_mspTemp));
    aggVec_reset((aggVec *) &rc_mspTemp);
    encoderc_adcs_estim_h1(&rc, out);
}

FILE_STATIC void rcPopulateH2(CANPacket *out)
{
    rc_adcs_estim_h2 rc;
    rc.rc_adcs_estim_h2_canrxerror = canRxErrorCheck();
    encoderc_adcs_estim_h2(&rc, out);
}

FILE_STATIC void rcPopulate2(CANPacket *out)
{
    rc_adcs_estim_2 rc;
    rc.rc_adcs_estim_2_pos_x = rtY.pos_eci_m[0];
    encoderc_adcs_estim_2(&rc, out);
}

FILE_STATIC void rcPopulate3(CANPacket *out)
{
    rc_adcs_estim_3 rc;
    rc.rc_adcs_estim_3_pos_y = rtY.pos_eci_m[1];
    encoderc_adcs_estim_3(&rc, out);
}

FILE_STATIC void rcPopulate4(CANPacket *out)
{
    rc_adcs_estim_4 rc;
    rc.rc_adcs_estim_4_pos_z = rtY.pos_eci_m[2];
    encoderc_adcs_estim_4(&rc, out);
}

FILE_STATIC void rcPopulate5(CANPacket *out)
{
    rc_adcs_estim_5 rc;
    rc.rc_adcs_estim_5_vel_x = rtY.vel_eci_mps[0];
    encoderc_adcs_estim_5(&rc, out);
}

FILE_STATIC void rcPopulate6(CANPacket *out)
{
    rc_adcs_estim_6 rc;
    rc.rc_adcs_estim_6_vel_y = rtY.vel_eci_mps[1];
    encoderc_adcs_estim_6(&rc, out);
}

FILE_STATIC void rcPopulate7(CANPacket *out)
{
    rc_adcs_estim_7 rc;
    rc.rc_adcs_estim_7_vel_z = rtY.vel_eci_mps[2];
    encoderc_adcs_estim_7(&rc, out);
}

FILE_STATIC void rcPopulate8(CANPacket *out)
{
    rc_adcs_estim_8 rc;

    /*
     * Calculate the J2000 epoch used in calculations by adding the MET epoch
     * from CAN to the MET last used in the autocode step.
     *
     * Don't use the rtY values because they're doubles and can lose precision.
     */
    uint64_t calcEpoch = metEpoch + lastUsedMET;
    metFromInt(calcEpoch, &rc.rc_adcs_estim_8_epoch,
               &rc.rc_adcs_estim_8_epoch_overflow);

    rc.rc_adcs_estim_8_sc_above_gs = rtY.sc_above_gs;
    rc.rc_adcs_estim_8_sc_in_sun = rtY.sc_in_sun;
    rc.rc_adcs_estim_8_sgp4_flag = rtY.SGP4_flag;
    rc.rc_adcs_estim_8_tle_id = tleAck(&tle);

    encoderc_adcs_estim_8(&rc, out);
}

FILE_STATIC void rcPopulate9(CANPacket *out)
{
    rc_adcs_estim_9 rc;
    rc.rc_adcs_estim_9_sun_x = rtY.sc2sun_unit[0];
    encoderc_adcs_estim_9(&rc, out);
}

FILE_STATIC void rcPopulate10(CANPacket *out)
{
    rc_adcs_estim_10 rc;
    rc.rc_adcs_estim_10_sun_y = rtY.sc2sun_unit[1];
    encoderc_adcs_estim_10(&rc, out);
}

FILE_STATIC void rcPopulate11(CANPacket *out)
{
    rc_adcs_estim_11 rc;
    rc.rc_adcs_estim_11_sun_z = rtY.sc2sun_unit[2];
    encoderc_adcs_estim_11(&rc, out);
}

FILE_STATIC void rcPopulate12(CANPacket *out)
{
    rc_adcs_estim_12 rc;
    rc.rc_adcs_estim_12_mag_x = rtY.mag_eci_unit[0];
    encoderc_adcs_estim_12(&rc, out);
}

FILE_STATIC void rcPopulate13(CANPacket *out)
{
    rc_adcs_estim_13 rc;
    rc.rc_adcs_estim_13_mag_y = rtY.mag_eci_unit[1];
    encoderc_adcs_estim_13(&rc, out);
}

FILE_STATIC void rcPopulate14(CANPacket *out)
{
    rc_adcs_estim_14 rc;
    rc.rc_adcs_estim_14_mag_z = rtY.mag_eci_unit[2];
    encoderc_adcs_estim_14(&rc, out);
}

