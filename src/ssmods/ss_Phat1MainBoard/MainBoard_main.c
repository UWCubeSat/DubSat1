#include <msp430.h> 
#include <MainBoard.h>

#include "bsp/bsp.h"
#include "core/i2c.h"
#include "core/uart.h"
#include "sensors/altimeter.h"
#include "sensors/magnetometer.h"

#define SEND_DATA_UART_TIME_MS (2000)
#define PPT_RANGE_ALT (90) //15 miles

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC AltimeterData *altitudeData;
//unique address assigned by uart.h for GPS UART
FILE_STATIC hBus uartHandle;
//Data stored here
FILE_STATIC rxDataHolder rxGPSData;

//magnetometer handle and structure
FILE_STATIC hMag magHandle;
FILE_STATIC MagnetometerData *magData;

FILE_STATIC TIMER_HANDLE send_uart_timer = 0;
FILE_STATIC UartTXData txData;

FILE_STATIC uint8_t altitudeReached;

FILE_STATIC void init_uart();
FILE_STATIC void start_uart_timer();
FILE_STATIC void send_uart_data();
FILE_STATIC void init_GPIO();
FILE_STATIC void check_PPT_alt();
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
    altitudeReached = 0;
    initAltimeter();
    init_GPIO();

//    initMagnetometer();

    init_uart();

    rxGPSData.length = 0;
    start_uart_timer();

    while (1)
    {
        if (checkTimer(send_uart_timer)) {
            readAltimeterData();//returns pointer to struct
//            readMagnetometerData();
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

void initMagnetometer(){
    magHandle = magInit(I2CBus2);
}

void readMagnetometerData(){
    magData = magReadXYZData(magHandle, ConvertToTeslas);
}

void readAltimeterData()
{
    altitudeData = readAltitudeData(12);
    txData.altimeter.altitude = altitudeData->altitude;
    txData.altimeter.pressure = altitudeData->pressure;
    txData.altimeter.temperature = altitudeData->temperature;
    i2cLED();
    check_PPT_alt();
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

FILE_STATIC void init_GPIO(){
    //6.2
    P6DIR |= (0x02);
}

FILE_STATIC void check_PPT_alt(){
    if(altitudeReached == 1){
        P1OUT |= 0x02;
    }
    else{
        if((altitudeData->altitude) >= PPT_RANGE_ALT){
            altitudeReached = 1;
        }
    }
}





