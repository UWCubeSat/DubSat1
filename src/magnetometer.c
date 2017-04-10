/*
 * magnetometer.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include <stdint.h>
#include "magnetometer.h"


#define MAX_BUFF_SIZE   25

void magInit()
{
    uint8_t buff[MAX_BUFF_SIZE];
    uint8_t szBuff;
#if HW_MAGTOM == 5883
    // Ready for configuration data
    buff[0] = MAG_ADDRESS_CRA;
    buff[1] = MAG_AVERAGE_1_SAMPLE | MAG_CONTINUOUS_OUTPUT_RATE_75 | MAG_MEASURE_MODE_NORMAL;
    buff[2] = MAG_ADDRESS_CRB;
    buff[3] = MAG_GAIN_1370;
    buff[4] = MAG_ADDRESS_MR;
    buff[5] = MAG_OPERATING_MODE_CONTINUOUS;
    szBuff = 6;
#elif HW_MAGTOM == 3110
    // TODO:  Do the MAG3110 setup
    // ...
    // szBuff = whatever;
#error Unknown - or no - magnetometer specified.  Use MAGNETOMETER to set type.
#endif  /* MAGNETOMETER == HMC5883 */

    i2cHoldReset();
    i2cAutoStopSetTotalBytes(szBuff);
    i2cReleaseReset();

    i2cWaitForStopComplete();
    i2cMasterTransmitStart();
    i2cWaitForStartComplete();

    uint8_t i;
    for (i = 0; i < szBuff; i++)
    {
        i2cWaitReadyToTransmitByte();
        i2cLoadTransmitBuffer(buff[i]);
    }

    // Auto-Stop should kick in after this and set Stop condition on the bus
}

void magReadBytesFromRegisters(uint8_t registeraddr, uint8_t * buff, uint8_t szToRead)
{
    uint8_t indexBuff = 0;

    i2cWaitForStopComplete();

    // Now read some data!
    i2cHoldReset();
    i2cAutoStopSetTotalBytes(szToRead);
    i2cReleaseReset();

    // First, send "cursor move" write -> an address, but no payload data
    // TODO:  Add hw-conditional logic if slave doesn't support auto-advance of address
    i2cMasterTransmitStart();
    i2cWaitForStartComplete();
    i2cWaitReadyToTransmitByte();
    i2cLoadTransmitBuffer(registeraddr);
    i2cWaitReadyToTransmitByte();

    // Now send Restart condition, switching over to receive mode
    i2cMasterReceiveStart();
    i2cWaitForStartComplete();

    //  Stop bit will be auto-set once we read szToRead bytes
    // TODO:  Add logic to handle edge case issues
    while ( (UCB2IFG & UCSTPIFG) == 0)
    {
        i2cWaitReadyToReceiveByte();
        buff[indexBuff++] = i2cRetrieveReceiveBuffer();
    }
}
