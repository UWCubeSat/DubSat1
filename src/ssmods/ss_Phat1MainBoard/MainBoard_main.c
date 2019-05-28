#include <msp430.h> 
#include <MainBoard.h>

#include "bsp/bsp.h"
#include "core/i2c.h"
#include "core/uart.h"
#include "sensors/altimeter.h"

#define SEND_DATA_UART_TIME_MS (2000)

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC AltimeterData *altitudeData;
//unique address assigned by uart.h for GPS UART
FILE_STATIC hBus uartHandle;
//Data stored here
FILE_STATIC rxDataHolder rxGPSData;
//TODO:Make a Struct to hold the data sent to coms

FILE_STATIC TIMER_HANDLE send_uart_timer = 0;
FILE_STATIC UartTXData txData;

FILE_STATIC void init_uart();
FILE_STATIC void start_uart_timer();
FILE_STATIC void send_uart_data();
/*
 * main.c
 */
int main(void)
{
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    mod_status.startup_type = Startup_Unknown;  // <<DO NOT DELETE or MOVE>>

    /*  Added Code  */
    initLEDs();
    initializeTimer();

    initAltimeter();

    init_uart();

    rxGPSData.length = 0;
    start_uart_timer();

    while (1)
    {
        if (checkTimer(send_uart_timer)) {
            readAltimeterData();//returns pointer to struct
            send_uart_data();
            uartLED();
            start_uart_timer();
        }
        getGPSDM(&(txData.gps));

    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

    return 0;
}

//void rxCallBack(uint8_t data){
//    rxGPSData.data_set[rxGPSData.length] = data;
//    rxGPSData.length++;
//}

void initAltimeter(){
    baromInit(I2CBus2);
}

void readAltimeterData()
{
    altitudeData = readAltitudeData(12);
    txData.altimeter.altitude = altitudeData->altitude;
    txData.altimeter.pressure = altitudeData->pressure;
    txData.altimeter.temperature = altitudeData->temperature;
    i2cLED();
}

void transmitUART(){
    //TODO: use backend uart library to send packets to the buffer
    //also check if there is a built in buffer or if you need to use the
    //library. And see what they want in packets
}

void uartLED(){
    P1OUT ^= 0x01;
}
void i2cLED(){
    P1OUT ^= 0x02;
}

void initLEDs(){
    WDTCTL = WDTPW | WDTHOLD;    // Stop WDT
    P1DIR |= (0x01 | 0x02);      // Set P1.0 to output direction
    PM5CTL0 &= ~LOCKLPM5;
}
//UART transmission via backend for COMS
//UART receiving via GPS
//I2C for the Sensors (Same line) (look at Eli's code)

FILE_STATIC void init_uart()
{
    gpsInit_Receiver();
}

FILE_STATIC void send_uart_data()
{
    gpsSendCommand(&txData);
}

FILE_STATIC void start_uart_timer()
{
    send_uart_timer = timerPollInitializer(SEND_DATA_UART_TIME_MS);
}




