#include <adcs_bdot.h>
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "sensors/magnetometer.h"
#include "core/timer.h"
#include "bdot_controller_lib.h"





// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;





// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;



FILE_STATIC int timer;
FILE_STATIC uint16_t ms = 2000;


void sendDipolePacket(int8_t x, int8_t y, int8_t z);
void sendTumblePacket(uint8_t x);
void receive_packet(CANPacket *packet);
void getMag();
void changeX();
void changeY();
void changeZ();


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

    P3DIR |= BIT5;
    P3OUT |= BIT5;

    canWrapInit();
    setCANPacketRxCallback(receive_packet);

    asensorInit(Ref_2p5V);

    magInit(1); // i2c bus 1
    MagnetometerData* data;
    initializeTimer();

    while (1)
    {
        getMag();
        P3OUT ^= BIT5;
    }
    return 0;
}


void getMag(MagnetometerData* data)
{
    data = magReadXYZData(ConvertToNanoTeslas);

    double xVal1 = data->convertedX;
    int16_t xVal = (int16_t) xVal1;
    if(xVal < 0)
    {
        xVal = xVal * -1;
    }
    int8_t x1 = ((int16_t) xVal) % 100;
    int8_t x2 = (((int16_t) xVal) / 100) % 100;
    int8_t x3 = ((int16_t) xVal / 10000) % 100;

    double yVal1 = data->convertedY;
    int16_t yVal = (int16_t) yVal1;
    if(yVal < 0)
    {
        yVal = yVal * -1;
    }
    int8_t y1 = ((int16_t) yVal) % 100;
    int8_t y2 = (((int16_t) yVal) / 100) % 100;
    int8_t y3 = ((int16_t) yVal / 10000) % 100;

    double zVal1 = data->convertedZ;
    int32_t zVal = (int32_t) zVal1;
    if(zVal < 0)
    {
        zVal = zVal * -1;
    }
    int8_t z1 = ((int32_t) zVal) % 100;
    int8_t z2 = (((int32_t) zVal) / 100) % 100;
    int8_t z3 = ((int32_t) zVal / 10000) % 100;



    double magTemp = data->convertedTemp;
    float mspTemp =  asensorReadIntTempC();

    int8_t sendx = (int8_t)xVal;

    int8_t sendy = (int8_t)yVal;
    int8_t sendz = (int8_t)zVal;
    int8_t sendMagTemp = (int8_t)magTemp;

    int xneg = 0;
    int yneg = 0;
    int zneg = 0;
    if(xVal < 0)
    {
        xneg = 1;
        sendx = sendx * -1;
    }
    if(yVal < 0)
    {
        yneg = 1;
        sendy = sendy * -1;
    }
    if(zVal < 0)
    {
        zneg = 1;
        sendz = sendz * -1;
    }

    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendDipolePacket(x3, x2, x1);
    P3OUT ^= BIT5;

    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendDipolePacket(y3, y2, y1);
    P3OUT ^= BIT5;

    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendDipolePacket(z3, z2, z1);
    P3OUT ^= BIT5;

    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendDipolePacket(xneg, yneg, zneg);
    P3OUT ^= BIT5;

    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendDipolePacket((int8_t)sendMagTemp, 0, (int8_t) mspTemp);
    P3OUT ^= BIT5;
}


void sendDipolePacket(int8_t x, int8_t y, int8_t z)
{
    CANPacket dipole_packet = {0};
    bdot_command_dipole dipole_info = {0};

    dipole_info.bdot_command_dipole_x = x;
    dipole_info.bdot_command_dipole_y = y;
    dipole_info.bdot_command_dipole_z = z;

    encodebdot_command_dipole(&dipole_info, &dipole_packet);
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

void changeX()
{
    int i = 0;
    timer = timerPollInitializer(ms);
    while(!checkTimer(timer))
    {
    }
    sendTumblePacket(1);

    for(i = -10; i < 11; i++)
    {
        P3OUT ^= BIT5;
        timer = timerPollInitializer(ms);
        while(!checkTimer(timer)) {}
        sendDipolePacket(10 * i, 0, 0);
    }
}


void changeY()
{
    int i = 0;
    timer = timerPollInitializer(ms);
    while(!checkTimer(timer))
    {
    }
    sendTumblePacket(1);

    for(i = -10; i < 11; i++)
    {
        timer = timerPollInitializer(ms);
        while(!checkTimer(timer)){}
        sendDipolePacket(0, 10 * i, 0);
    }
}

void changeZ()
{
    int i = 0;
    timer = timerPollInitializer(ms);
    while(!checkTimer(timer)) {}
    sendTumblePacket(1);

    for(i = -10; i < 11; i++)
    {
        timer = timerPollInitializer(ms);
        while(!checkTimer(timer)) {}
        sendDipolePacket(0, 0, 10 * i);
    }
}




/*No op function */
void receive_packet(CANPacket *packet)
{
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
