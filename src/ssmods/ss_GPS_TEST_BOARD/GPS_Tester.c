#include <msp430.h> 

#include "GPS_Tester.h"
#include "bsp/bsp.h"
#include "core/uart.h"


#define SEND_DATA_UART_TIME_MS (2000)


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
FILE_STATIC GPSDegMin gpsData;

FILE_STATIC void init_uart();




/*
 * main.c
 */
int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    mod_status.startup_type = Startup_Unknown;  // <<DO NOT DELETE or MOVE>>

    //ACTUAL CODE STARTS HERE
    ledInit();
    init_uart();

    while (1)
    {
        getGPSDM(&(gpsData));
        ledTest();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

/*void rxCallBack(uint8_t data){
    rxData.data_set[rxData.length] = data;
    rxData.length++;
    ledTest();
}*/

FILE_STATIC void ledTest(){
    if(gpsData.degLat != 0){
        P1OUT |= 0x01;
    }
}

FILE_STATIC void ledInit(){
    WDTCTL = WDTPW | WDTHOLD;    // Stop WDT
    P1DIR |= (0x01);      // Set P1.0 to output direction
    PM5CTL0 &= ~LOCKLPM5;
}

FILE_STATIC void init_uart()
{
    gpsInit_Receiver();
}

