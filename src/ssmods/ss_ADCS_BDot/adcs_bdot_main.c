#include <adcs_bdot.h>
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "sensors/magnetometer.h"
#include "core/timer.h"
#include "bdot_controller_lib.h"


FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC my_telem myTelemSeg;
FILE_STATIC magnetometer_segment myTelemMagnetometer;
FILE_STATIC mtq_info_segment myTelemMtqInfo;
FILE_STATIC simulink_segment mySimulink;
FILE_STATIC mtq_info mtqInfo;
FILE_STATIC mtq_info lastSentDipole;
FILE_STATIC mtq_state_segment mtqState;
FILE_STATIC MagnetometerData* magData;

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState mtq_state   = mtq_not_on;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;


// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;


FILE_STATIC int mtq_on_timer;
FILE_STATIC uint16_t mtq_on_timer_ms = 5000; // 1 s

FILE_STATIC int mtq_off_timer;
FILE_STATIC uint16_t mtq_off_timer_ms = 3000; // 1 s

FILE_STATIC int telem_timer;
FILE_STATIC uint16_t telem_timer_ms = 500; // 1 s


FILE_STATIC int rtOneStep_timer;
FILE_STATIC uint32_t rtOneStep_us = 100000;


FILE_STATIC int packet_timer;
FILE_STATIC uint32_t packet_ms = 5000;

FILE_STATIC int normalOperation = 1;

FILE_STATIC  int mtq_acknowledgement = 0;
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

    start_telem_timer();
    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
    {
        if(checkTimer(telem_timer))
        {
            sendTelemetry();
            start_telem_timer();
        }

        switch (mtq_state)
        {
        case mtq_not_on:
            if(rtY.tumble)
            {
                mtq_state = mtq_turn_on;

            }
//            else
//            {
//                mtqInfo.tumble_status = 1;  // TODO: DELETE THIS, ONLY FOR TESTING
//            }
            break;
        case mtq_turn_on:
            lastSentDipole.xDipole = mtqInfo.xDipole;
            lastSentDipole.yDipole = mtqInfo.yDipole;
            lastSentDipole.zDipole = mtqInfo.zDipole;
            lastSentDipole.tumble_status = mtqInfo.tumble_status;
            sendDipolePacket(mtqInfo.xDipole, mtqInfo.yDipole, mtqInfo.zDipole);
            start_mtq_on_timer();
            mtq_state = mtq_on_wait;
            P3OUT ^= BIT5;
            break;
        case mtq_on_wait:
            if(checkTimer(mtq_on_timer))
            {
                mtq_state = mtq_turn_off;
            }
            break;
        case mtq_turn_off:
            P3OUT ^= BIT5;
            lastSentDipole.xDipole = 0;
            lastSentDipole.yDipole = 0;
            lastSentDipole.zDipole = 0;
            lastSentDipole.tumble_status = 0;
            sendDipolePacket(0, 0, 0);
            mtq_state = mtq_turn_off_ack;
            start_packet_timer();
            break;
        case mtq_turn_off_ack:
//            mtq_acknowledgement = 1; // TODO: DELETE; ONLY HERE FOR TESTING
            if(mtq_acknowledgement)
            {
                mtq_state = mtq_off_wait;
                start_mtq_off_timer();
                endPollingTimer(packet_timer);
                mtq_acknowledgement = 0;
            }
            else if(checkTimer(packet_timer))
            {
                mtq_state = mtq_turn_off;
            }
            else
            {
                mtq_state = mtq_turn_off_ack;
            }
            break;
        case mtq_off_wait:
            if(checkTimer(mtq_off_timer))
            {
                mtq_state = mtq_not_on;
            }
        default:
            mod_status.state_transition_errors++;
            mod_status.in_unknown_state++;
            break;
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

    magInit(1); // I2C bus 1

    asensorInit(Ref_2p5V);

    bdot_controller_lib_initialize();

    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&myTelemSeg.header, TLM_ID_MY_TELEM, sizeof(myTelemSeg));
    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));
    bcbinPopulateHeader(&mtqState.header, TLM_ID_MTQ_STATE, sizeof(mtqState));

    initializeTimer();
}

void start_mtq_on_timer()
{
    mtq_on_timer = timerPollInitializer(mtq_on_timer_ms);
}

void start_mtq_off_timer()
{
    mtq_off_timer = timerPollInitializer(mtq_off_timer_ms);
}

void start_telem_timer()
{
    telem_timer = timerPollInitializer(telem_timer_ms);
}

void start_packet_timer()
{
    packet_timer = timerPollInitializer(packet_ms);
}

void getMagnetometerData()
{
//    magData->conversionMode = ConvertToTeslas;
//    magData->convertedX = 1;
//    magData->convertedY = 2;
//    magData->convertedZ = 3;
    if(mtq_state == mtq_not_on || mtq_state == mtq_off_wait)
    {
        magData = magReadXYZData(ConvertToTeslas);
    }
}

void performSelfTest()
{
    selfTestConfig();
    normalOperation = 0;
}

void performNormalOp()
{
    normalOperationConfig();
    normalOperation = 1;
}

void simulink_compute()
{
    getMagnetometerData();
    rtU.B_body_in_T[0] = magData->convertedX;
    rtU.B_body_in_T[1] = magData->convertedY;
    rtU.B_body_in_T[2] = magData->convertedZ;
    rtU.B_meas_valid = normalOperation;
    rtU.MT_on = 0;
    rt_OneStep();
    updateMtqInfo();
}


void sendTelemetry()
{
    sendHealthSegment();
    sendMagReadingSegment();
    sendMtqInfoSegment();
    sendSimulinkSegment();
    sendMtqState();
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

void sendMetaSegment()
{
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

void sendMagReadingSegment()
{
    // TODO: Delete constants
    myTelemMagnetometer.xMag = magData->convertedX * 10e9;
    myTelemMagnetometer.yMag = magData->convertedY * 10e9;
    myTelemMagnetometer.zMag = magData->convertedZ * 10e9;
    myTelemMagnetometer.tempMag = magData->convertedTemp;

    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));

}

void sendMtqInfoSegment()
{
//    myTelemMtqInfo.xDipole = mtqInfo.xDipole;
//    myTelemMtqInfo.yDipole = mtqInfo.yDipole;
//    myTelemMtqInfo.zDipole = mtqInfo.zDipole;
//    myTelemMtqInfo.tumble_status = mtqInfo.tumble_status;
    myTelemMtqInfo.xDipole = lastSentDipole.xDipole;
    myTelemMtqInfo.yDipole = lastSentDipole.yDipole;
    myTelemMtqInfo.zDipole = lastSentDipole.zDipole;
    myTelemMtqInfo.tumble_status = lastSentDipole.tumble_status;
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

void sendMtqState()
{
    mtqState.mtq_state = mtq_state;
    bcbinSendPacket((uint8_t *) &mtqState, sizeof(mtqState));

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
    mtq_ack ack = {0};
    decodemtq_ack(packet, &ack);
    if(ack.mtq_ack_coils_state)
    {
        mtq_acknowledgement = 1;
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


//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//#include "bdot_controller_lib.h"
//
//
//FILE_STATIC meta_segment mseg;
//FILE_STATIC health_segment hseg;
//FILE_STATIC my_telem myTelemSeg;
//FILE_STATIC magnetometer_segment myTelemMagnetometer;
//FILE_STATIC mtq_info_segment myTelemMtqInfo;
//FILE_STATIC simulink_segment mySimulink;
//FILE_STATIC mtq_info mtqInfo;
//FILE_STATIC mtq_state_segment mtqState;
//FILE_STATIC MagnetometerData* magData;
//
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState mtq_state   = mtq_not_on;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//
//
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
//
//
//FILE_STATIC int mtq_on_timer;
//FILE_STATIC uint16_t mtq_on_timer_ms = 1000; // 1 s
//
//FILE_STATIC int mtq_off_timer;
//FILE_STATIC uint16_t mtq_off_timer_ms = 1000; // 1 s
//
//FILE_STATIC int telem_timer;
//FILE_STATIC uint16_t telem_timer_ms = 1000; // 1 s
//
//
//FILE_STATIC int rtOneStep_timer;
//FILE_STATIC uint32_t rtOneStep_us = 100000;
//
//FILE_STATIC int normalOperation = 1;
//
//FILE_STATIC volatile int mtq_acknowledgement = 0;
///*
// * main.c
// */
//int main(void)
//{
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SS_ADCS_BDOT__);  // <<DO NOT DELETE or MOVE>>
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    initial_setup();
//    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
//    startCallback(rtOneStep_timer);
//
//    /* Attach rt_OneStep to a timer or interrupt service routine with
//     * period 0.1 seconds (the model's base sample time) here.  The
//     * call syntax for rt_OneStep is
//     *
//     *  rt_OneStep();
//    */
//    // Disable rt_OneStep() here
//
//    // Terminate model
//    fflush((NULL));
//
//    start_telem_timer();
//    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
//    {
//        if(checkTimer(telem_timer))
//        {
//            sendTelemetry();
//            start_telem_timer();
//        }
//
//        switch (mtq_state)
//        {
//        case mtq_not_on:
//            if(rtY.tumble)
//            {
//                updateMtqInfo();
//                mtq_state = mtq_turn_on;
//
//            }
////            else
////            {
////                mtqInfo.tumble_status = 1;  // TODO: DELETE THIS, ONLY FOR TESTING
////            }
//            break;
//        case mtq_turn_on:
//
//            sendDipolePacket(mtqInfo.xDipole, mtqInfo.yDipole, mtqInfo.zDipole);
//            start_mtq_on_timer();
//            mtq_state = mtq_on_wait;
//            P3OUT ^= BIT5;
//            break;
//        case mtq_on_wait:
//            if(checkTimer(mtq_on_timer))
//            {
//                mtqInfo.xDipole = 0;
//                mtqInfo
//                sendDipolePacket(0, 0, 0);
//                mtq_state = mtq_turn_off_ack;
//                P3OUT ^= BIT5;
//            }
//            break;
//        case mtq_turn_off_ack:
//            mtq_acknowledgement = 1; // TODO: DELETE; ONLY HERE FOR TESTING
//            if(mtq_acknowledgement)
//            {
//                mtq_state = mtq_off_wait;
//            }
//            start_mtq_off_timer();
//            break;
//        case mtq_off_wait:
//            if(checkTimer(mtq_off_timer))
//            {
//                mtq_state = mtq_not_on;
//            }
//        default:
//            mod_status.state_transition_errors++;
//            mod_status.in_unknown_state++;
//            break;
//        }
//    }
//
//    bdot_controller_lib_terminate();
//
//    return 0;
//}
//
//
//void initial_setup()
//{
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    magInit(1); // I2C bus 1
//
//    asensorInit(Ref_2p5V);
//
//    bdot_controller_lib_initialize();
//
//    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
//    bcbinPopulateHeader(&myTelemSeg.header, TLM_ID_MY_TELEM, sizeof(myTelemSeg));
//    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
//    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
//    bcbinPopulateHeader(&mySimulink.header, TLM_ID_SIMULINK_INFO, sizeof(mySimulink));
//    bcbinPopulateHeader(&mtqState.header, TLM_ID_MTQ_STATE, sizeof(mtqState));
//
//    initializeTimer();
//}
//
//void start_mtq_on_timer()
//{
//    mtq_on_timer = timerPollInitializer(mtq_on_timer_ms);
//}
//
//void start_mtq_off_timer()
//{
//    mtq_off_timer = timerPollInitializer(mtq_off_timer_ms);
//}
//
//void start_telem_timer()
//{
//    telem_timer = timerPollInitializer(telem_timer_ms);
//}
//
//void getMagnetometerData()
//{
////    magData->conversionMode = ConvertToTeslas;
////    magData->convertedX = 1;
////    magData->convertedY = 2;
////    magData->convertedZ = 3;
//    magData = magReadXYZData(ConvertToTeslas);
//}
//
//void performSelfTest()
//{
//    selfTestConfig();
//    normalOperation = 0;
//}
//
//void performNormalOp()
//{
//    normalOperationConfig();
//    normalOperation = 1;
//}
//
//void simulink_compute()
//{
//    getMagnetometerData();
//    rtU.B_body_in_T[0] = magData->convertedX;
//    rtU.B_body_in_T[1] = magData->convertedY;
//    rtU.B_body_in_T[2] = magData->convertedZ;
//    rtU.B_meas_valid = normalOperation;
//    rtU.MT_on = (mtq_state == mtq_turn_on);
//    rt_OneStep();
//}
//
//
//void sendTelemetry()
//{
//    sendHealthSegment();
//    sendMagReadingSegment();
//    sendMtqInfoSegment();
//    sendSimulinkSegment();
//    sendMtqState();
//}
//
//void updateMtqInfo()
//{
//    mtqInfo.xDipole = (int8_t) rtY.Dig_val[0];
//    mtqInfo.yDipole = (int8_t) rtY.Dig_val[1];
//    mtqInfo.zDipole = (int8_t) rtY.Dig_val[2];
//    mtqInfo.tumble_status  = (uint8_t) rtY.tumble;
//}
//
//void sendHealthSegment()
//{
//    // Populate and send the health segment for this entity
//    hseg.inttemp = asensorReadIntTempC();
//    hseg.oms = OMS_Unknown;
//    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
//
//    // Also invoke the status handler for UART to send its health segments
//    debugInvokeStatusHandler(Entity_UART);
//
//}
//
//void sendMetaSegment()
//{
//    bcbinPopulateMeta(&mseg, sizeof(mseg));
//    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
//}
//
//void sendMagReadingSegment()
//{
//    // TODO: Delete constants
//    myTelemMagnetometer.xMag = magData->convertedX * 10e9;
//    myTelemMagnetometer.yMag = magData->convertedY * 10e9;
//    myTelemMagnetometer.zMag = magData->convertedZ * 10e9;
//    myTelemMagnetometer.tempMag = magData->convertedTemp;
//
//    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));
//
//}
//
//void sendMtqInfoSegment()
//{
//    myTelemMtqInfo.xDipole = mtqInfo.xDipole;
//    myTelemMtqInfo.yDipole = mtqInfo.yDipole;
//    myTelemMtqInfo.zDipole = mtqInfo.zDipole;
//    myTelemMtqInfo.tumble_status = mtqInfo.tumble_status;
//
//    bcbinSendPacket((uint8_t *) &myTelemMtqInfo, sizeof(myTelemMtqInfo));
//}
//
//
//void sendSimulinkSegment()
//{
//    mySimulink.sim_xDipole = rtY.Dig_val[0];
//    mySimulink.sim_yDipole = rtY.Dig_val[1];
//    mySimulink.sim_zDipole = rtY.Dig_val[2];
//    mySimulink.tumble = rtY.tumble;
//
//    bcbinSendPacket((uint8_t *) &mySimulink, sizeof(mySimulink));
//}
//
//void sendMtqState()
//{
//    mtqState.mtq_state = mtq_state;
//    bcbinSendPacket((uint8_t *) &mtqState, sizeof(mtqState));
//
//}
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//void sendTumblePacket(uint8_t status)
//{
//    CANPacket tumble_packet = {0};
//    bdot_tumble_status tumble_info = {0};
//
//    tumble_info.bdot_tumble_status_status = status;
//
//    encodebdot_tumble_status(&tumble_info, &tumble_packet);
//    canSendPacket(&tumble_packet);
//}
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//    mtq_acknowledgement = 1;
//}
//
//
///*
// * Associating rt_OneStep with a real-time clock or interrupt service routine
// * is what makes the generated code "real-time".  The function rt_OneStep is
// * always associated with the base rate of the model.  Subrates are managed
// * by the base rate from inside the generated code.  Enabling/disabling
// * interrupts and floating point context switches are target specific.  This
// * example code indicates where these should take place relative to executing
// * the generated code step function.  Overrun behavior should be tailored to
// * your application needs.  This example simply sets an error status in the
// * real-time model and returns from rt_OneStep.
// */
//void rt_OneStep(void)
//{
//  static boolean_T OverrunFlag = false;
//
//  /* Disable interrupts here */
//
//  /* Check for overrun */
//  if (OverrunFlag) {
//    rtmSetErrorStatus(rtM, "Overrun");
//    return;
//  }
//
//  OverrunFlag = true;
//
//  /* Save FPU context here (if necessary) */
//  /* Re-enable timer or interrupt here */
//  /* Set model inputs here */
//
//  /* Step the model */
//  bdot_controller_lib_step();
//
//  /* Get model outputs here */
//
//  /* Indicate task complete */
//  OverrunFlag = false;
//
//  /* Disable interrupts here */
//  /* Restore FPU context here (if necessary) */
//  /* Enable interrupts here */
//}
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}



//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//#include "MSP_BDOT.h"
//
//
//FILE_STATIC meta_segment mseg;
//FILE_STATIC health_segment hseg;
//FILE_STATIC my_telem myTelemSeg;
//FILE_STATIC magnetometer_segment myTelemMagnetometer;
//FILE_STATIC mtq_info_segment myTelemMtqInfo;
//
//
//FILE_STATIC mtq_info mtqInfo;
//FILE_STATIC MagnetometerData* magData;
//
//FILE_STATIC void sendDipolePacket(int8_t x, int8_t y, int8_t z);
//FILE_STATIC void sendTumblePacket(uint8_t status);
//FILE_STATIC void receive_packet(CANPacket *packet);
//FILE_STATIC void getMagnetometerData();
//FILE_STATIC void performSelfTest();
//FILE_STATIC void wait();
//FILE_STATIC void sendMetaSegment();
//FILE_STATIC void sendHealthSegment();
//FILE_STATIC void sendMagReadingSegment();
//FILE_STATIC void sendMtqInfoSegment();
//FILE_STATIC void simulink_compute();
//
//
//
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//
//
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
//
//FILE_STATIC int timer;
//FILE_STATIC int timer_ms = 50;
//
//FILE_STATIC int rtOneStep_timer;
//FILE_STATIC uint32_t rtOneStep_us = 100000;
//
//FILE_STATIC int normalOperation = 1;
//
///*
// * main.c
// */
//int main(void)
//{
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SS_ADCS_BDOT__);  // <<DO NOT DELETE or MOVE>>
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    magInit(1); // I2C bus 1
//
//    MSP_BDOT_initialize();
//
//    asensorInit(Ref_2p5V);
//
//
//    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
//    bcbinPopulateHeader(&myTelemSeg.header, TLM_ID_MY_TELEM, sizeof(myTelemSeg));
//    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
//    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
//
//    initializeTimer();
//
//    rtOneStep_timer = timerCallbackInitializer(&simulink_compute, rtOneStep_us); // 100 ms
//    startCallback(rtOneStep_timer);
//
//    /* Attach rt_OneStep to a timer or interrupt service routine with
//     * period 0.1 seconds (the model's base sample time) here.  The
//     * call syntax for rt_OneStep is
//     *
//     *  rt_OneStep();
//    */
//    // Disable rt_OneStep() here
//
//    // Terminate model
//    fflush((NULL));
//
//    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
//    {
//        wait();
//        updateMtqInfo();
//        getMagnetometerData();
//        sendHealthSegment();
//        sendMagReadingSegment();
//        sendMtqInfoSegment();
//    }
//
//    MSP_BDOT_terminate();
//
//    return 0;
//}
//
//void wait()
//{
//    timer = timerPollInitializer(timer_ms);
//    while(!checkTimer(timer)) {};
//}
//
//void updateMtqInfo()
//{
//    mtqInfo.xDipole = (int8_t) rtY.cmd_MAG_bdot[0];
//    mtqInfo.yDipole = (int8_t) rtY.cmd_MAG_bdot[1];
//    mtqInfo.zDipole = (int8_t) rtY.cmd_MAG_bdot[2];
//    mtqInfo.tumble_status  = (uint8_t) rtY.tumble;
//    if(mtqInfo.tumble_status)
//    {
//        sendDipolePacket(mtqInfo.xDipole, mtqInfo.yDipole, mtqInfo.zDipole);
//    }
//}
//
//void getMagnetometerData()
//{
//    magData = magReadXYZData(ConvertToTeslas);
//}
//
//void performSelfTest()
//{
//    selfTestConfig();
//    normalOperation = 0;
//}
//
//
///* TODO: Questions
// * 1. Biggest and smallest value that simulink code outputs for rtY?
// * 2. Sensor can read at high rates. How fast can the i2C bus run?
// * 3. In example main, while(rtmGetErrorStatus(rtM) == (NULL)) was used. Is that safe?
// * 4. fflush(NULL) ??
// * 5.
//*/
//
//
//
//void simulink_compute()
//{
//    P3OUT |= BIT5;
//    rtU.mag_vec_body_T[0] = magData->convertedX;
//    rtU.mag_vec_body_T[1] = magData->convertedY;
//    rtU.mag_vec_body_T[2] = magData->convertedZ;
//    rtU.mag_vec_body_T[3] = normalOperation;
//    rt_OneStep();
//}
//
//
///*
// * Associating rt_OneStep with a real-time clock or interrupt service routine
// * is what makes the generated code "real-time".  The function rt_OneStep is
// * always associated with the base rate of the model.  Subrates are managed
// * by the base rate from inside the generated code.  Enabling/disabling
// * interrupts and floating point context switches are target specific.  This
// * example code indicates where these should take place relative to executing
// * the generated code step function.  Overrun behavior should be tailored to
// * your application needs.  This example simply sets an error status in the
// * real-time model and returns from rt_OneStep.
// */
//void rt_OneStep(void)
//{
//
//  P3OUT ^= BIT5;
//
//  static boolean_T OverrunFlags[2] = { 0, 0 };
//
//  static boolean_T eventFlags[2] = { 0, 0 };/* Model has 2 rates */
//
//  static int_T taskCounter[2] = { 0, 0 };
//
//  /* Disable interrupts here */
//
//  /* Check base rate for overrun */
//  if (OverrunFlags[0]) {
//    rtmSetErrorStatus(rtM, "Overrun");
//    return;
//  }
//
//  OverrunFlags[0] = true;
//
//  /* Save FPU context here (if necessary) */
//  /* Re-enable timer or interrupt here */
//
//  /*
//   * For a bare-board target (i.e., no operating system), the
//   * following code checks whether any subrate overruns,
//   * and also sets the rates that need to run this time step.
//   */
//  if (taskCounter[1] == 0) {
//    if (eventFlags[1]) {
//      OverrunFlags[0] = false;
//      OverrunFlags[1] = true;
//
//      /* Sampling too fast */
//      rtmSetErrorStatus(rtM, "Overrun");
//      return;
//    }
//
//    eventFlags[1] = true;
//  }
//
//  taskCounter[1]++;
//  if (taskCounter[1] == 5) {
//    taskCounter[1]= 0;
//  }
//
//  /* Set model inputs associated with base rate here */
//
//  /* Step the model for base rate */
//  MSP_BDOT_step0();
//
//  /* Get model outputs here */
//
//  /* Indicate task for base rate complete */
//  OverrunFlags[0] = false;
//
//  /* If task 1 is running, don't run any lower priority task */
//  if (OverrunFlags[1]) {
//    return;
//  }
//
//  /* Step the model for subrate */
//  if (eventFlags[1]) {
//    OverrunFlags[1] = true;
//
//    /* Set model inputs associated with subrates here */
//
//    /* Step the model for subrate 1 */
//    MSP_BDOT_step1();
//
//    /* Get model outputs here */
//
//    /* Indicate task complete for subrate */
//    OverrunFlags[1] = false;
//    eventFlags[1] = false;
//  }
//
//  /* Disable interrupts here */
//  /* Restore FPU context here (if necessary) */
//  /* Enable interrupts here */
//}
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//void sendTumblePacket(uint8_t status)
//{
//    CANPacket tumble_packet = {0};
//    bdot_tumble_status tumble_info = {0};
//
//    tumble_info.bdot_tumble_status_status = status;
//
//    encodebdot_tumble_status(&tumble_info, &tumble_packet);
//    canSendPacket(&tumble_packet);
//}
//
//
//FILE_STATIC void sendHealthSegment()
//{
//    // Populate and send the health segment for this entity
//    hseg.inttemp = asensorReadIntTempC();
//    hseg.oms = OMS_Unknown;
//    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
//
//    // Also invoke the status handler for UART to send its health segments
//    debugInvokeStatusHandler(Entity_UART);
//
//}
//
//FILE_STATIC void sendMetaSegment()
//{
//    bcbinPopulateMeta(&mseg, sizeof(mseg));
//    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
//}
//
//FILE_STATIC void sendMagReadingSegment()
//{
//    // TODO: Delete constants
//    myTelemMagnetometer.xMag = magData->convertedX * 10e9;
//    myTelemMagnetometer.yMag = magData->convertedY * 10e9;
//    myTelemMagnetometer.zMag = magData->convertedZ * 10e9;
//    myTelemMagnetometer.tempMag = magData->convertedTemp;
//
//    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));
//
//}
//
//FILE_STATIC void sendMtqInfoSegment()
//{
//    myTelemMtqInfo.xDipole = mtqInfo.xDipole;
//    myTelemMtqInfo.yDipole = mtqInfo.yDipole;
//    myTelemMtqInfo.zDipole = mtqInfo.zDipole;
//    myTelemMtqInfo.tumble_status = mtqInfo.tumble_status;
//
//    bcbinSendPacket((uint8_t *) &myTelemMtqInfo, sizeof(myTelemMtqInfo));
//}
//
//
//
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//}
//
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}



//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//#include "bdot_controller_lib.h"
//
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z);
//void sendTumblePacket(uint8_t status);
//void receive_packet(CANPacket *packet);
//void getMagnetometerData();
//void performSelfTest();
//void rt_OneStep(void);
//void wait();
//
//
//
//
//
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//
//
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
//
//FILE_STATIC int timer;
//FILE_STATIC int timer_ms = 2000;
//
//FILE_STATIC int rtOneStep_timer;
//FILE_STATIC int rtOneStep_us = 25000;
//
//FILE_STATIC int normalOperation = 1;
//
///*
// * main.c
// */
//int main(void)
//{
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SS_ADCS_BDOT__);  // <<DO NOT DELETE or MOVE>>
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    magInit(1); // I2C bus 1
//
//    bdot_controller_lib_initialize();
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    initializeTimer();
//
//    rtOneStep_timer = timerCallbackInitializer(&rt_OneStep, rtOneStep_us); // .025 ms
//    startCallback(rtOneStep_timer);
//
//    // Attach rt_OneStep to a timer or interrupt service routine with
//     // * period 0.025 seconds (the model's base sample time) here.  The
//     // * call syntax for rt_OneStep is
//     // *
//     // *  rt_OneStep();
//    // Disable rt_OneStep() here
//
//    // Terminate model
//    fflush((NULL));
//
//    while (rtmGetErrorStatus(rtM) == (NULL) || 1)
//    {
//    // Perform other application tasks here
//        wait();
//        getMagnetometerData();
//        int8_t xVal = (int8_t) rtY.Dig_val[0];
//        int8_t yVal = (int8_t) rtY.Dig_val[1];
//        int8_t zVal = (int8_t) rtY.Dig_val[2];
//
////        sendTumblePacket(1);
//        sendDipolePacket(xVal, yVal, zVal);
//    }
//
//    bdot_controller_lib_terminate();
//    return 0;
//}
//
//void wait()
//{
//    timer = timerPollInitializer(timer_ms);
//    while(!checkTimer(timer)) {};
//}
//
//void getMagnetometerData()
//{
//    MagnetometerData* data = magReadXYZData(ConvertToTeslas);
//    rtU.B_body_in_T[0] = data->convertedX;
//    rtU.B_body_in_T[1] = data->convertedY;
//    rtU.B_body_in_T[2] = data->convertedZ;
//    rtU.B_meas_valid = normalOperation;
//    rtU.MT_on = !(rtY.Dig_val[0] == 0.0 && rtY.Dig_val[1] == 0.0 && rtY.Dig_val[2] == 0.0);
//}
//
//void performSelfTest()
//{
//    selfTestConfig();
//    normalOperation = 0;
//}
//
//
///* TODO: Questions
// * 1. Biggest and smallest value that simulink code outputs for rtY?
// * 2. Determining the value for rtU.MT_on: Based on if previous rtY outputs are 0?
// *      - 0.0? 0.1??
// * 3. In example main, while(rtmGetErrorStatus(rtM) == (NULL)) was used. Is that safe?
// * 4. fflush(NULL) ??
// * 5. 2 mag on same MSP?
//*/
//
//
///*
// * Associating rt_OneStep with a real-time clock or interrupt service routine
// * is what makes the generated code "real-time".  The function rt_OneStep is
// * always associated with the base rate of the model.  Subrates are managed
// * by the base rate from inside the generated code.  Enabling/disabling
// * interrupts and floating point context switches are target specific.  This
// * example code indicates where these should take place relative to executing
// * the generated code step function.  Overrun behavior should be tailored to
// * your application needs.  This example simply sets an error status in the
// * real-time model and returns from rt_OneStep.
// */
//void rt_OneStep(void)
//{
//    static boolean_T OverrunFlags[2] = { 0, 0 };
//
//    static boolean_T eventFlags[2] = { 0, 0 };/* Model has 2 rates */
//
//    static int_T taskCounter[2] = { 0, 0 };
//
//    /* Disable interrupts here */
//
//    /* Check base rate for overrun */
//    if (OverrunFlags[0]) {
//        rtmSetErrorStatus(rtM, "Overrun");
//        return;
//    }
//
//    OverrunFlags[0] = true;
//
//    /* Save FPU context here (if necessary) */
//    /* Re-enable timer or interrupt here */
//
//    /*
//     * For a bare-board target (i.e., no operating system), the
//     * following code checks whether any subrate overruns,
//     * and also sets the rates that need to run this time step.
//     */
//    if (taskCounter[1] == 0)
//    {
//        if (eventFlags[1])
//        {
//            OverrunFlags[0] = false;
//            OverrunFlags[1] = true;
//
//            /* Sampling too fast */
//            rtmSetErrorStatus(rtM, "Overrun");
//            return;
//        }
//
//        eventFlags[1] = true;
//    }
//
//    taskCounter[1]++;
//    if (taskCounter[1] == 4)
//    {
//        taskCounter[1]= 0;
//    }
//
//    /* Set model inputs associated with base rate here */
//
//    /* Step the model for base rate */
//    bdot_controller_lib_step0();
//
//    /* Get model outputs here */
//
//    /* Indicate task for base rate complete */
//    OverrunFlags[0] = false;
//
//    /* If task 1 is running, don't run any lower priority task */
//    if (OverrunFlags[1])
//    {
//        return;
//    }
//
//    /* Step the model for subrate */
//    if (eventFlags[1])
//    {
//        OverrunFlags[1] = true;
//
//        /* Set model inputs associated with subrates here */
//
//        /* Step the model for subrate 1 */
//        bdot_controller_lib_step1();
//
//        /* Get model outputs here */
//
//        /* Indicate task complete for subrate */
//        OverrunFlags[1] = false;
//        eventFlags[1] = false;
//    }
//
//    /* Disable interrupts here */
//    /* Restore FPU context here (if necessary) */
//    /* Enable interrupts here */
//}
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//void sendTumblePacket(uint8_t status)
//{
//    CANPacket tumble_packet = {0};
//    bdot_tumble_status tumble_info = {0};
//
//    tumble_info.bdot_tumble_status_status = status;
//
//    encodebdot_tumble_status(&tumble_info, &tumble_packet);
//    canSendPacket(&tumble_packet);
//}
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//}
//
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}






// BDOT VERSION FOR COSMO TELEMETRY
//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//
//
//FILE_STATIC meta_segment mseg;
//FILE_STATIC health_segment hseg;
//FILE_STATIC my_telem myTelemSeg;
//FILE_STATIC magnetometer_segment myTelemMagnetometer;
//FILE_STATIC mtq_info_segment myTelemMtqInfo;
//
//FILE_STATIC int timer;
//FILE_STATIC uint16_t ms = 2000;
//
//FILE_STATIC mtq_info mtqInfo;
//FILE_STATIC MagnetometerData* data;
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//FILE_STATIC void wait(uint16_t milli);
//FILE_STATIC void sendDipolePacket(int8_t x, int8_t y, int8_t z);
//FILE_STATIC void sendTumblePacket(uint8_t x);
//FILE_STATIC void receive_packet(CANPacket *packet);
//FILE_STATIC void sendMetaSegment();
//FILE_STATIC void sendHealthSegment();
//FILE_STATIC void sendMagReadingSegment();
//FILE_STATIC void sendMtqInfoSegment();
//
//
//
///*
// * main.c
// */
//int main(void)
//{
//
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
//
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    asensorInit(Ref_2p5V);
//
//    bcbinPopulateHeader(&hseg.header, TLM_ID_SHARED_HEALTH, sizeof(hseg));
//    bcbinPopulateHeader(&myTelemSeg.header, TLM_ID_MY_TELEM, sizeof(myTelemSeg));
//    bcbinPopulateHeader(&myTelemMagnetometer.header, TLM_ID_MAGNETOMETER, sizeof(myTelemMagnetometer));
//    bcbinPopulateHeader(&myTelemMtqInfo.header, TLM_ID_MTQ_INFO, sizeof(myTelemMtqInfo));
//
////    magInit(1); // i2c bus 1
//    initializeTimer();
//
//    while (1)
//    {
//        wait(ms);
//        P3OUT ^= BIT5;
//        sendHealthSegment();
//        sendMagReadingSegment();
//
//    }
//    return 0;
//}
//
//FILE_STATIC void wait(uint16_t milli)
//{
//    timer = timerPollInitializer(milli);
//    while(!checkTimer(timer)) {}
//}
//
//FILE_STATIC void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//FILE_STATIC void sendTumblePacket(uint8_t status)
//{
//    CANPacket tumble_packet = {0};
//    bdot_tumble_status tumble_info = {0};
//
//    tumble_info.bdot_tumble_status_status = status;
//
//    encodebdot_tumble_status(&tumble_info, &tumble_packet);
//    canSendPacket(&tumble_packet);
//}
//
//
//FILE_STATIC void sendHealthSegment()
//{
//    // Populate and send the health segment for this entity
//    hseg.inttemp = asensorReadIntTempC();
//    hseg.oms = OMS_Unknown;
//    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
//
//    // Also invoke the status handler for UART to send its health segments
//    debugInvokeStatusHandler(Entity_UART);
//
//}
//
//FILE_STATIC void sendMetaSegment()
//{
//    bcbinPopulateMeta(&mseg, sizeof(mseg));
//    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
//}
//
//FILE_STATIC void sendMagReadingSegment()
//{
//    // TODO: Delete constants
//    myTelemMagnetometer.xMag = 30.0;
//    myTelemMagnetometer.yMag = 25.0;
//    myTelemMagnetometer.zMag = 2.0;
//    myTelemMagnetometer.tempMag = 26.0;
//
//    bcbinSendPacket((uint8_t *) &myTelemMagnetometer, sizeof(myTelemMagnetometer));
//
//}
//
//FILE_STATIC void sendMtqInfoSegment()
//{
//    myTelemMtqInfo.xDipole = mtqInfo.xDipole + 30;
//    myTelemMtqInfo.yDipole = mtqInfo.yDipole + 25;
//    myTelemMtqInfo.zDipole = mtqInfo.zDipole + 2;
//    myTelemMtqInfo.tumble_status = mtqInfo.tumble_status;
//
//    bcbinSendPacket((uint8_t *) &myTelemMtqInfo, sizeof(myTelemMtqInfo));
//}
//
//
//
//
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//}
//
//
//
//
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}





//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//#include "bdot_controller_lib.h"
//
//
//
//
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//
//
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
//
//
//
//FILE_STATIC int timer;
//FILE_STATIC uint16_t ms = 2000;
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z);
//void sendTumblePacket(uint8_t x);
//void receive_packet(CANPacket *packet);
//void getMag();
//void changeX();
//void changeY();
//void changeZ();
//
//
///*
// * main.c
// */
//int main(void)
//{
//
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
//
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    asensorInit(Ref_2p5V);
//
//    magInit(1); // i2c bus 1
//    MagnetometerData* data;
//    initializeTimer();
//
//    while (1)
//    {
//        getMag();
//        P3OUT ^= BIT5;
//    }
//    return 0;
//}
//
//
//void getMag(MagnetometerData* data)
//{
//    data = magReadXYZData(ConvertToNanoTeslas);
//
//    double xVal1 = data->convertedX;
//    int16_t xVal = (int16_t) xVal1;
//    if(xVal < 0)
//    {
//        xVal = xVal * -1;
//    }
//    int8_t x1 = ((int16_t) xVal) % 100;
//    int8_t x2 = (((int16_t) xVal) / 100) % 100;
//    int8_t x3 = ((int16_t) xVal / 10000) % 100;
//
//    double yVal1 = data->convertedY;
//    int16_t yVal = (int16_t) yVal1;
//    if(yVal < 0)
//    {
//        yVal = yVal * -1;
//    }
//    int8_t y1 = ((int16_t) yVal) % 100;
//    int8_t y2 = (((int16_t) yVal) / 100) % 100;
//    int8_t y3 = ((int16_t) yVal / 10000) % 100;
//
//    double zVal1 = data->convertedZ;
//    int32_t zVal = (int32_t) zVal1;
//    if(zVal < 0)
//    {
//        zVal = zVal * -1;
//    }
//    int8_t z1 = ((int32_t) zVal) % 100;
//    int8_t z2 = (((int32_t) zVal) / 100) % 100;
//    int8_t z3 = ((int32_t) zVal / 10000) % 100;
//
//
//
//    double magTemp = data->convertedTemp;
//    float mspTemp =  asensorReadIntTempC();
//
//    int8_t sendx = (int8_t)xVal;
//
//    int8_t sendy = (int8_t)yVal;
//    int8_t sendz = (int8_t)zVal;
//    int8_t sendMagTemp = (int8_t)magTemp;
//
//    int xneg = 0;
//    int yneg = 0;
//    int zneg = 0;
//    if(xVal < 0)
//    {
//        xneg = 1;
//        sendx = sendx * -1;
//    }
//    if(yVal < 0)
//    {
//        yneg = 1;
//        sendy = sendy * -1;
//    }
//    if(zVal < 0)
//    {
//        zneg = 1;
//        sendz = sendz * -1;
//    }
//
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(x3, x2, x1);
//    P3OUT ^= BIT5;
//
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(y3, y2, y1);
//    P3OUT ^= BIT5;
//
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(z3, z2, z1);
//    P3OUT ^= BIT5;
//
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(xneg, yneg, zneg);
//    P3OUT ^= BIT5;
//
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket((int8_t)sendMagTemp, 0, (int8_t) mspTemp);
//    P3OUT ^= BIT5;
//}
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//void sendTumblePacket(uint8_t status)
//{
//    CANPacket tumble_packet = {0};
//    bdot_tumble_status tumble_info = {0};
//
//    tumble_info.bdot_tumble_status_status = status;
//
//    encodebdot_tumble_status(&tumble_info, &tumble_packet);
//    canSendPacket(&tumble_packet);
//}
//
//void changeX()
//{
//    int i = 0;
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer))
//    {
//    }
//    sendTumblePacket(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        P3OUT ^= BIT5;
//        timer = timerPollInitializer(ms);
//        while(!checkTimer(timer)) {}
//        sendDipolePacket(10 * i, 0, 0);
//    }
//}
//
//
//void changeY()
//{
//    int i = 0;
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer))
//    {
//    }
//    sendTumblePacket(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        timer = timerPollInitializer(ms);
//        while(!checkTimer(timer)){}
//        sendDipolePacket(0, 10 * i, 0);
//    }
//}
//
//void changeZ()
//{
//    int i = 0;
//    timer = timerPollInitializer(ms);
//    while(!checkTimer(timer)) {}
//    sendTumblePacket(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        timer = timerPollInitializer(ms);
//        while(!checkTimer(timer)) {}
//        sendDipolePacket(0, 0, 10 * i);
//    }
//}
//
//
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//}
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}




//#include <adcs_bdot.h>
//#include <msp430.h>
//#include <stddef.h>
//#include "interfaces/canwrap.h"
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//#include "core/timer.h"
//#include "bdot_controller_lib.h"
//
//
//
//
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//
//
//
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
//
//
//
//FILE_STATIC int timer;
//FILE_STATIC int count = 1;
//FILE_STATIC int TARval = 0; // Timer goes off every seconds
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z);
//void receive_packet(CANPacket *packet);
//void getMag();
//void changeX();
//void changeY();
//void changeZ();
//
///*
// * main.c
// */
//int main(void)
//{
//
//    /* ----- INITIALIZATION -----*/
//    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
//    // __SUBSYSTEM_MODULE__ is set in bsp.h based on the __SS_<subsystemmodule>__ passed in
//    // as a predefined symbol
//    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
//
//    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
//    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
//    // Also hooks up special notification handlers.  Note that actual pulse interrupt handlers will update the
//    // firing state structures before calling the provided handler function pointers.
//    mod_status.startup_type = coreStartup(handlePPTFiringNotification, handleRollCall);  // <<DO NOT DELETE or MOVE>>
//
//#if defined(__DEBUG__)
//
//    // Insert debug-build-only things here, like status/info/command handlers for the debug
//    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
//    // debugtools.h.  Also, be sure to change the "path char"
//    // e.g. debugRegisterEntity(Entity_Test, handleDebugInfoCallback,
//    //                               handleDebugStatusCallback,
//    //                               handleDebugActionCallback);
//
//#endif  //  __DEBUG__
//
//    /* ----- CAN BUS/MESSAGE CONFIG -----*/
//    // TODO:  Add the correct bus filters and register CAN message receive handlers
//
//    debugTraceF(1, "CAN message bus configured.\r\n");
//
//    /* ----- SUBSYSTEM LOGIC -----*/
//    // TODO:  Finally ... NOW, implement the actual subsystem logic!
//    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
//    // and then control is returned to this main loop
//
//    // See ss_EPS_Dist for ideas on how to structure creating telemetry and command packets, etc.
//
//    debugTraceF(1, "Commencing subsystem module execution ...\r\n");
//
//    P3DIR |= BIT5;
//    P3OUT |= BIT5;
//
//    canWrapInit();
//    setCANPacketRxCallback(receive_packet);
//
//    asensorInit(Ref_2p5V);
//
//    magInit(1); // i2c bus 1
//    MagnetometerData* data;
//    initializeTimer();
//
//    while (1)
//    {
////        getMag();
//        changeX();
//        changeY();
//        changeZ();
//        P3OUT ^= BIT5;
//    }
//
//    return 0;
//}
//
//
//void getMag(MagnetometerData* data)
//{
//    data = magReadXYZData(ConvertToNanoTeslas);
//    double xVal = data->convertedX * 10e-4;
//    double yVal = data->convertedY * 10e-4;
//    double zVal = data->convertedZ * 10e-4;
//    double magTemp = data->convertedTemp;
//    float mspTemp =  asensorReadIntTempC();
//
//    int8_t sendx = (int8_t)xVal;
//    int8_t sendy = (int8_t)yVal;
//    int8_t sendz = (int8_t)zVal;
//    int8_t sendMagTemp = (int8_t)magTemp;
//
//    int xneg = 0;
//    int yneg = 0;
//    int zneg = 0;
//    if(xVal < 0)
//    {
//        xneg = 1;
//        sendx = sendx * -1;
//    }
//    if(yVal < 0)
//    {
//        yneg = 1;
//        sendy = sendy * -1;
//    }
//    if(zVal < 0)
//    {
//        zneg = 1;
//        sendz = sendz * -1;
//    }
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(sendx, sendy, sendz);
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket(xneg, yneg, zneg);
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer)) {}
//    sendDipolePacket((int8_t)sendMagTemp, 0, (int8_t) mspTemp);
//}
//
//
//void sendDipolePacket(int8_t x, int8_t y, int8_t z)
//{
//    CANPacket dipole_packet = {0};
//    bdot_command_dipole dipole_info = {0};
//
//    dipole_info.bdot_command_dipole_x = x;
//    dipole_info.bdot_command_dipole_y = y;
//    dipole_info.bdot_command_dipole_z = z;
//
//    encodebdot_command_dipole(&dipole_info, &dipole_packet);
//    canSendPacket(&dipole_packet);
//}
//
//
//void changeX()
//{
//    int i = 0;
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer))
//    {
//    }
//    toggleTumbleStatus(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        P3OUT ^= BIT5;
//        timer = timerPollInitializer(count, TARval);
//        while(!checkTimer(timer)) {}
//        sendDipolePacket(10 * i, 0, 0);
//    }
//}
//
//
//void changeY()
//{
//    int i = 0;
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer))
//    {
//    }
//    toggleTumbleStatus(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        timer = timerPollInitializer(count, TARval);
//        while(!checkTimer(timer)){}
//        sendDipolePacket(0, 10 * i, 0);
//    }
//}
//
//void changeZ()
//{
//    int i = 0;
//    timer = timerPollInitializer(count, TARval);
//    while(!checkTimer(timer))
//    {
//    }
//    toggleTumbleStatus(1);
//
//    for(i = -10; i < 11; i++)
//    {
//        timer = timerPollInitializer(count, TARval);
//        while(!checkTimer(timer)) {}
//        sendDipolePacket(0, 0, 10 * i);
//    }
//}
//
//
//
//
///*No op function */
//void receive_packet(CANPacket *packet)
//{
//}
//
//// Will be called when PPT firing cycle is starting (sent via CAN by the PPT)
//void handlePPTFiringNotification()
//{
//    __no_operation();
//}
//
//// Will be called when the subsystem gets the distribution board's CAN message that asks for check-in
//// Likely calling frequency is probably once every couple of minutes, but the code shouldn't work with
//// any period (in particular for testing, where we might spam the CAN bus with roll call queries)
//void handleRollCall()
//{
//    __no_operation();
//}
//
