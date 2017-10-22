/*
 * i2c.h
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */
#ifndef I2C_SUPPORT_H_
#define I2C_SUPPORT_H_

#include <msp430.h>
#include <stdint.h>

#include "bsp/bsp.h"
#include "utils.h"

typedef enum _bus_instance_i2c {
    I2CBus1 = 0,
    I2CBus2 = 1,
} bus_instance_i2c;

typedef struct {

} status_i2c;

typedef struct {


} bus_context_i2c;

// Core helper definitions
#define I2C_MASTER_RECEIVE_INTERRUPT_MASK     (UCRXEIE | UCNACKIE )
#define I2C_MASTER_TRANSMIT_INTERRUPT_MASK    (UCTXIE2 | UCNACKIE )

/***************************/
/* I2C LOW-LEVEL FUNCTIONS */
/***************************/
FILE_STATIC void inline i2cDisable()  { UCB2CTLW0 |= UCSWRST; }
FILE_STATIC void inline i2cEnable()  { UCB2CTL1 &= ~UCSWRST; }
FILE_STATIC void inline i2cMasterTransmitStart()  { UCB2CTL1 |= UCTR | UCTXSTT; }
FILE_STATIC void inline i2cMasterReceiveStart()  { UCB2CTL1 &= ~UCTR;  UCB2CTL1 |= UCTXSTT;  }
FILE_STATIC void inline i2cLoadTransmitBuffer(uint8_t input)  {  UCB2TXBUF = input; }
FILE_STATIC uint8_t inline i2cRetrieveReceiveBuffer()  {  return UCB2RXBUF;  }
FILE_STATIC void inline i2cAutoStopSetTotalBytes(uint8_t count)  { UCB2TBCNT = count; }  // NOTE: must be called under reset!

// Synchronous (busy-waiting) version of I2C calls
// TODO:  Add NACK and other edge-case support to busy/waits
#ifndef DISABLE_SYNC_I2C_CALLS

// TODO:  Re-enable this warning once there is actually an option to do async :)
//#warning Synchronous calls to I2C are inefficient - consider disabling sync calls and \
//using the async interrupt-based API instead, with DISABLE_SYNC_I2C_CALLS.
FILE_STATIC void inline i2cWaitForStopComplete()  { while (UCB2CTLW0 & UCTXSTP); }
FILE_STATIC void inline i2cWaitForStartComplete() { while (UCB2CTLW0 & UCTXSTT); }
FILE_STATIC void inline i2cWaitReadyToTransmitByte()  { while ( (UCB2IFG & UCTXIFG0) == 0); }
FILE_STATIC void inline i2cWaitReadyToReceiveByte()  { while ( (UCB2IFG & UCRXIFG) == 0); }

/***************************/
/* I2C MID-LEVEL FUNCTIONS */
/***************************/
void i2cMasterRead(hDev device, uint8_t * buff, uint8_t szToRead);
void i2cMasterWrite(hDev device, uint8_t * buff, uint8_t szToWrite);
void i2cMasterRegisterRead(hDev device, uint8_t registeraddr, uint8_t * buff, uint8_t szToRead);
void i2cMasterCombinedWriteRead(hDev device, uint8_t * wbuff, uint8_t szToWrite, uint8_t * rbuff, uint8_t szToRead);


#endif /* DISABLE_SYNC_I2C_CALLS */

// TODO:  Add "async" interrupt-based alternative to synchronous versions

// Core functions
hDev i2cInit(bus_instance_i2c instance, uint8_t slaveaddr);  // TODO: change this to return a handle?

#endif /* I2C_SUPPORT_H_ */
