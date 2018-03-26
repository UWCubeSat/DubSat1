/*
 * i2c.h
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */
#ifndef I2C_SUPPORT_H_
#define I2C_SUPPORT_H_

#ifdef __MSP432P401R__
#include <msp.h>
#elif defined __MSP430FR5994__
#include <msp430.h>
#endif

#include <stdint.h>

#include "bsp/bsp.h"
#include "utils.h"
#include "config/config_bus_i2c.h"

// Keeping it sane with a 1-based count
typedef enum _bus_instance_i2c {
    I2CBus1 = 1,
    I2CBus2 = 2,
} bus_instance_i2c;

typedef struct _bus_registers_i2c {
    volatile uint16_t * UCBxCTLW0;
    volatile uint8_t * UCBxCTL1;
    volatile uint16_t * UCBxCTLW1;
    volatile uint16_t * UCBxBRW;
    volatile uint16_t * UCBxSTATW;
    volatile uint16_t * UCBxTBCNT;
    volatile uint16_t * UCBxRXBUF;
    volatile uint16_t * UCBxTXBUF;
    volatile uint16_t * UCBxI2CSA;
    volatile uint16_t * UCBxIE;
    volatile uint16_t * UCBxIFG;
    volatile uint16_t * UCBxIV;
} bus_registers_i2c;

extern bus_registers_i2c busregs[CONFIGM_i2c_maxperipheralinstances];

typedef struct {

} status_i2c;

typedef struct {
    BOOL initialized;
    uint8_t num_devices;
} bus_context_i2c;

typedef struct {
    bus_instance_i2c bus;
    uint16_t slaveaddr;
} device_context_i2c;

// Core helper definitions
#define I2C_MASTER_RECEIVE_INTERRUPT_MASK     (UCRXEIE | UCNACKIE )
#define I2C_MASTER_TRANSMIT_INTERRUPT_MASK    (UCTXIE2 | UCNACKIE )

// Some quasi-unpleasant hackery to make for clean dual-bus support in general
#define I2CREG(n,x)       (*(busregs[(uint8_t)n].x))

/***************************/
/* I2C LOW-LEVEL FUNCTIONS */
/***************************/
FILE_STATIC void inline i2cDisable(bus_instance_i2c bus)  { I2CREG(bus, UCBxCTLW0) |= UCSWRST; }
void inline i2cEnable(bus_instance_i2c bus)  { I2CREG(bus, UCBxCTLW0) &= ~UCSWRST; }
FILE_STATIC void inline i2cMasterTransmitStart(bus_instance_i2c bus)  { I2CREG(bus, UCBxCTLW0) |= UCTR | UCTXSTT ; }
FILE_STATIC void inline i2cMasterReceiveStart(bus_instance_i2c bus)  { I2CREG(bus, UCBxCTLW0) = I2CREG(bus, UCBxCTLW0) & ~UCTR | UCTXSTT;  }
FILE_STATIC void inline i2cLoadTransmitBuffer(bus_instance_i2c bus, uint8_t input)  {  I2CREG(bus, UCBxTXBUF) = input; }
FILE_STATIC uint8_t inline i2cRetrieveReceiveBuffer(bus_instance_i2c bus)  {  return I2CREG(bus, UCBxRXBUF);  }
FILE_STATIC void inline i2cAutoStopSetTotalBytes(bus_instance_i2c bus, uint8_t count)  { I2CREG(bus, UCBxTBCNT) = count; }  // NOTE: must be called under reset!

// Synchronous (busy-waiting) version of I2C calls
// TODO:  Add NACK and other edge-case support to busy/waits
#ifndef DISABLE_SYNC_I2C_CALLS

// TODO:  Re-enable this warning once there is actually an option to do async :)
//#warning Synchronous calls to I2C are inefficient - consider disabling sync calls and \
//using the async interrupt-based API instead, with DISABLE_SYNC_I2C_CALLS.
FILE_STATIC void inline i2cWaitForStopComplete(bus_instance_i2c bus)  { while (I2CREG(bus, UCBxCTLW0) & UCTXSTP); }
FILE_STATIC void inline i2cWaitForStartComplete(bus_instance_i2c bus) { while (I2CREG(bus, UCBxCTLW0) & UCTXSTT); }
FILE_STATIC void inline i2cWaitReadyToTransmitByte(bus_instance_i2c bus)  { while ( (I2CREG(bus, UCBxIFG) & UCTXIFG0) == 0); }
FILE_STATIC void inline i2cWaitReadyToReceiveByte(bus_instance_i2c bus)  { while ( (I2CREG(bus, UCBxIFG) & UCRXIFG) == 0); }

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
hDev i2cInit(bus_instance_i2c instance, uint8_t slaveaddr);

#endif /* I2C_SUPPORT_H_ */
