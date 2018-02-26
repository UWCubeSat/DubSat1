#include <adcs_bdot.h>
#include <msp430.h>

#include "interfaces/canwrap.h"
#include "bsp/bsp.h"
#include "sensors/magnetometer.h"
#include "core/timer.h"

void sendDipolePacket(int8_t x, int8_t y, int8_t z);
void sendTumblePacket(uint8_t status);
void changeX();
void changeY();
void changeZ();
void changeXY();
void changeXZ();
void changeYZ();
void toggleTumbleStatus(uint8_t status);





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
FILE_STATIC int count = 0;
FILE_STATIC int TARval = 10000; // Timer goes off every seconds




/*
 * main.c
 */
int main(void)
{
//    PJDIR |= 0x07;
//    PJOUT |= 0x07;
//    magInit(I2CBus1);
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




    magInit(1); // I2C bus 1
    MagnetometerData* data;
    double x;
    double y;
    double z;

    PJDIR |= 0x07;
    PJOUT |= 0x07;

    canWrapInit();
    setCANPacketRxCallback(receive_packet);

    initializeTimer();

    // TODO: function returns data in int16_t while the CANpacket has uint8_t. Which one?
    while (1)
    {
        timer = timerPollInitializer(count, TARval);
        while(!checkTimer(timer))
        {
        }
        data = getMagnetometerData();
        x = data->convertedX;
        y = data->convertedY;
        z = data->convertedZ;


        /*
         *  AUTO GENERATED CODE
        */
        // SEND PACKETS
    }

}


MagnetometerData* getMagnetometerData()
{
    return magReadXYZData(ConvertToNanoTeslas); // TODO: what conversion mode?
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


/*No op function */
void receive_packet(CANPacket *packet)
{
}





void toggleTumbleStatus(uint8_t status)
{
    int i = 0;
    timer = timerPollInitializer(count, TARval);
    while(i < 1)
    {
        if(checkTimer(timer))
        {
            sendTumblePacket(status);
            i++;
        }
    }
}

void changeX()
{
    int i = 0;
    for(i = 0; i < 1; i++)
    {
        toggleTumbleStatus(1);
        int k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(k + i * 10, 0, 0);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
        toggleTumbleStatus(1);
        k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(k + i * 10, 0, 0);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
    }
}


void changeY()
{
    int i = 0;
    for(i = 0; i < 1; i++)
    {
        toggleTumbleStatus(1);
        int k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(0, k + i * 10, 0);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
        toggleTumbleStatus(1);
        k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(0, k + i * 10, 0);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
    }
}

void changeZ()
{
    int i = 0;
    for(i = 0; i < 1; i++)
    {
        toggleTumbleStatus(1);
        int k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(0, 0, k + i * 10);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
        toggleTumbleStatus(1);
        k = 0;
        timer = timerPollInitializer(count, TARval);
        while(k < 10)
        {
            if(checkTimer(timer))
            {
                sendDipolePacket(0, 0, k + i * 10);
                k++;
                if(k < 10)
                {
                    timer = timerPollInitializer(count, TARval);
                }
            }
        }
    }
}

void changeXY()
{
    int i = 0;
       for(i = -10; i < 10; i++)
       {
           toggleTumbleStatus(0);
           int k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(k + i * 10, k + i * 10, 0);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
           toggleTumbleStatus(1);
           k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(k + i * 10, k + i * 10, 0);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
       }
}

void changeXZ()
{
    int i = 0;
       for(i = -10; i < 10; i++)
       {
           toggleTumbleStatus(0);
           int k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(k + i * 10, 0, k + i * 10);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
           toggleTumbleStatus(1);
           k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(k + i * 10, 0, k + i * 10);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
       }
}

void changeYZ()
{
    int i = 0;
       for(i = -10; i < 10; i++)
       {
           toggleTumbleStatus(0);
           int k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(0, k + i * 10, k + i * 10);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
           toggleTumbleStatus(1);
           k = 0;
           timer = timerPollInitializer(count, TARval);
           while(k < 10)
           {
               if(checkTimer(timer))
               {
                   sendDipolePacket(0, k + i * 10, k + i * 10);
                   k++;
                   if(k < 10)
                   {
                       timer = timerPollInitializer(count, TARval);
                   }
               }
           }
       }
}





//i = 0;
//while(i < 1)
//{
//    if(checkTimer(timer))
//    {
//        sendTumblePacket(status);
//        if(status == 0)
//        {
//            status = 1;
//        } else
//        {
//            status = 0;
//        }
//        i++;
//        timer = timerPollInitializer(count, TARval);
//    }
//}
//i = 0;
//while(i < 5)
//{
//   if(checkTimer(timer))
//   {
//       sendDipolePacket(x_, y_, z_);
//       x_++;
//       i++;
//       P1OUT ^= (BIT0 | BIT2);
//       timer = timerPollInitializer(count, TARval);
//   }
//
//}
//
//x_ = 0;
//y_ = -100;
//i = 0;
//while(i < 5)
//{
//    if(checkTimer(timer))
//    {
//        sendDipolePacket(x_, y_, z_);
//        y_++;
//        i++;
//        P1OUT ^= (BIT0 | BIT2);
//        timer = timerPollInitializer(count, TARval);
//    }
//}
//
//y_ = 0;
//z_ = -100;
//i = 0;
//while(i < 5)
//{
//    if(checkTimer(timer))
//    {
//        sendDipolePacket(x_, y_, z_);
//        z_++;
//        i++;
//        P1OUT ^= (BIT0 | BIT2);
//        timer = timerPollInitializer(count, TARval);
//    }
//}
//z_ = 0;



//if(checkTimer(timer))
//{
////            sendDipolePacket(100, 1, -100);
//    sendTumbleStatus(1);
//    timer = timerPollInitializer(2, 0);
//}

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
//
//#include "bsp/bsp.h"
//#include "sensors/magnetometer.h"
//
//// Main status (a structure) and state and mode variables
//// Make sure state and mode variables are declared as volatile
//FILE_STATIC ModuleStatus mod_status;
//FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
//FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;
//
//// These are sample "trigger" flags, used to indicate to the main loop
//// that a transition should occur
//FILE_STATIC flag_t triggerState1;
//FILE_STATIC flag_t triggerState2;
//FILE_STATIC flag_t triggerState3;
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
//    while (1)
//    {
//        // This assumes that some interrupt code will change the value of the triggerStaten variables
//        switch (ss_state)
//        {
//        case State_FirstState:
//            if (triggerState2)
//            {
//                triggerState2 = 0;
//                ss_state = State_SecondState;
//            }
//            break;
//        case State_SecondState:
//            if (triggerState3)
//            {
//                triggerState3 = 0;
//                ss_state = State_ThirdState;
//            }
//            break;
//        case State_ThirdState:
//            if (triggerState1)
//            {
//                triggerState1 = 0;
//                ss_state = State_FirstState;
//            }
//            break;
//        default:
//            mod_status.state_transition_errors++;
//            mod_status.in_unknown_state++;
//            break;
//        }
//    }
//
//    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!
//
//    return 0;
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

