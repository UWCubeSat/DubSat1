#include <msp430.h> 

#include "GPS_Tester.h"
#include "bsp/bsp.h"
#include "core/uart.h"
//#include "BufferedReader.h" //Do I need?

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
//unique address assigned by uart.h
FILE_STATIC hBus uartHandle;
//Data stored here
FILE_STATIC rxDataHolder rxData;




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

    //ACTUAL CODE STARTS HERE
    rxData.length = 0;

    uartHandle = uartInit(ApplicationUART, 0, Speed_9600);
    uartRegisterRxCallback(uartHandle,&rxCallBack);
    ledInit();
    ledTest();
    //BufferedReaderInit(uartHandle, buffer, GPS_RX_BUFFER_LENGTH); //needed?

    while (1);
/*    {
        rxCallBack();
    }*/

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

void rxCallBack(uint8_t data){
    rxData.data_set[rxData.length] = data;
    rxData.length++;
    ledTest();
}

void ledTest(){
    P1OUT ^= 0x01;

}

void ledInit(){
    WDTCTL = WDTPW | WDTHOLD;    // Stop WDT
    P1DIR |= (0x01);      // Set P1.0 to output direction
    PM5CTL0 &= ~LOCKLPM5;
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

