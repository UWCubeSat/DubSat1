/*
 * i2c_support.c
 *
 *  Created on: Apr 9, 2017
 *      Author: jeffc
 */

#include "i2c.h"

FILE_STATIC status_i2c i2c_status;
FILE_STATIC bus_context_i2c buses[CONFIGM_i2c_maxperipheralinstances];
FILE_STATIC device_context_i2c devices[CONFIGM_i2c_maxdevices];
FILE_STATIC uint8_t numDevices = 0;

bus_registers_i2c busregs[CONFIGM_i2c_maxperipheralinstances];

uint16_t busErrorCount = 0;
uint16_t i2cGetBusErrorCount()
{
    return busErrorCount;
}

uint16_t bytesRead = 0;
uint16_t i2cGetBytesRead()
{
    return bytesRead;
}

uint16_t bytesWritten = 0;
uint16_t i2cGetBytesWritten()
{
    return bytesWritten;
}

uint8_t lastOperationFlag = 0;
uint8_t i2cGetLastOperationResult()
{
    return lastOperationFlag;
}

// Some quasi-unpleasant hackery to make for clean dual-bus support in general
static void populateBusRegisters(bus_instance_i2c instance)
{
    bus_registers_i2c * pRegs;

    if (instance == I2CBus1)
    {
        pRegs = &busregs[1];
        pRegs->UCBxCTLW0 =  &UCB1CTLW0;
        pRegs->UCBxCTL1 =   &UCB1CTL1;   // MSB of CTLW0
        pRegs->UCBxCTLW1 =  &UCB1CTLW1;
        pRegs->UCBxBRW =    &UCB1BRW;
        pRegs->UCBxSTATW =  &UCB1STATW;
        pRegs->UCBxTBCNT =  &UCB1TBCNT;
        pRegs->UCBxRXBUF =  &UCB1RXBUF;
        pRegs->UCBxTXBUF =  &UCB1TXBUF;
        pRegs->UCBxI2CSA =  &UCB1I2CSA;
        pRegs->UCBxIE =     &UCB1IE;
        pRegs->UCBxIFG =    &UCB1IFG;
        pRegs->UCBxIV =     &UCB1IV;
    }
    else if (instance == I2CBus2)
    {
        pRegs = &busregs[2];
        pRegs->UCBxCTLW0 =  &UCB2CTLW0;
        pRegs->UCBxCTL1 =   &UCB2CTL1;   // MSB of CTLW0
        pRegs->UCBxCTLW1 =  &UCB2CTLW1;
        pRegs->UCBxBRW =    &UCB2BRW;
        pRegs->UCBxSTATW =  &UCB2STATW;
        pRegs->UCBxTBCNT =  &UCB2TBCNT;
        pRegs->UCBxRXBUF =  &UCB2RXBUF;
        pRegs->UCBxTXBUF =  &UCB2TXBUF;
        pRegs->UCBxI2CSA =  &UCB2I2CSA;
        pRegs->UCBxIE =     &UCB2IE;
        pRegs->UCBxIFG =    &UCB2IFG;
        pRegs->UCBxIV =     &UCB2IV;
    }
}

hDev i2cInit(bus_instance_i2c bus, uint8_t slaveaddr)
{
    bus_context_i2c * pBus = &(buses[(uint8_t)bus]);

    // First handle bus initialization, if necessary
    if (!pBus->initialized)
    {
        pBus->initialized = TRUE;

        bspI2CInit(bus);
        populateBusRegisters(bus);

        // USCI Configuration:  Common for both TX and RX
        i2cDisable(bus);                                             // Software reset enabled
        I2CREG(bus, UCBxCTLW0) |= UCMODE_3 | UCMST | UCSYNC | UCSSEL__SMCLK;     // I2C, master, synchronous, use SMCLK
        I2CREG(bus, UCBxBRW) = 12;                                               // Baudrate = SMCLK / 12
        I2CREG(bus, UCBxCTLW1) |= UCASTP_2;                                      // Automatic stop generated after transmission complete
        I2CREG(bus, UCBxCTLW1) |= UCCLTO_3;                                      // clk low timeout @ 34ms
    }

    // Now setup the actual individual device
    uint8_t currindex = numDevices;
    numDevices++;
    pBus->num_devices++;

    devices[currindex].bus = bus;
    devices[currindex].slaveaddr = slaveaddr;

    return (hDev)currindex;
}

static uint8_t i2cCoreRead(hDev device, uint8_t * buff, uint8_t szToRead, BOOL initialAutoStopSetup)
{
    uint8_t index = 0;

    bus_instance_i2c bus = devices[device].bus;
    I2CREG(bus, UCBxI2CSA) = devices[device].slaveaddr;

    // Setup autostop if havne't already done so
    if (initialAutoStopSetup)
    {
        i2cDisable(bus);
        i2cAutoStopSetTotalBytes(bus, szToRead);
        i2cEnable(bus);
    }

    // Send start message, in receive mode
    i2cMasterReceiveStart(bus);
    i2cWaitForStartComplete(bus);
    if(I2CREG(bus, UCBxCTLW0) & UCTXSTT) //error state
    {
        busErrorCount++;
        lastOperationFlag = 1;
        return 1;
    }
    //  Stop bit will be auto-set once we read szToRead bytes
    while ( (I2CREG(bus, UCBxIFG) & UCSTPIFG) == 0 && index < szToRead)
    {
        if ( (I2CREG(bus, UCBxIFG) & UCRXIFG) != 0)
            buff[index++] = i2cRetrieveReceiveBuffer(bus);
    }
    bytesRead += szToRead;
    lastOperationFlag = 0;
    return 0;
}

static uint8_t i2cCoreWrite(hDev device, uint8_t * buff, uint8_t szToWrite, BOOL initialAutoStopSetup )
{
    uint8_t index = 0;

    bus_instance_i2c bus = devices[device].bus;
    I2CREG(bus, UCBxI2CSA) = devices[device].slaveaddr;

    if (initialAutoStopSetup)
    {
        i2cDisable(bus);
        i2cAutoStopSetTotalBytes(bus, szToWrite);
        i2cEnable(bus);
    }

    i2cMasterTransmitStart(bus);
    i2cWaitForStartComplete(bus);
    if(I2CREG(bus, UCBxCTLW0) & UCTXSTT) //error state
    {
        busErrorCount++;
        lastOperationFlag = 1;
        //TODO: send a stop
        return 1;
    }

    // Send in auto-stop or managed mode
    while ( (I2CREG(bus, UCBxIFG) & UCSTPIFG) == 0 && index < szToWrite)
    {
        if ( (I2CREG(bus, UCBxIFG) & UCTXIFG0) != 0)
        {
            I2CREG(bus, UCBxTXBUF) = buff[index++];
        }
    }
    lastOperationFlag = 0;
    bytesWritten += szToWrite;
    return 0;
}

uint8_t i2cMasterRead(hDev device, uint8_t * buff, uint8_t szToRead)
{
    return i2cCoreRead(device, buff, szToRead, TRUE);
}

uint8_t i2cMasterWrite(hDev device, uint8_t * buff, uint8_t szToWrite)
{
    return i2cCoreWrite(device, buff, szToWrite, TRUE);
}

uint8_t i2cMasterCombinedWriteRead(hDev device, uint8_t * wbuff, uint8_t szToWrite, uint8_t * rbuff, uint8_t szToRead)
{
    bus_instance_i2c bus = devices[device].bus;

    i2cWaitForStopComplete(bus);

    i2cDisable(bus);
    i2cAutoStopSetTotalBytes(bus, szToWrite + szToRead);
    i2cEnable(bus);
    if(i2cCoreWrite(device, wbuff, szToWrite, FALSE))
        return 1;

    i2cWaitReadyToTransmitByte(bus);
    if(i2cCoreRead(device, rbuff, szToRead, FALSE))
        return 1;

    return 0;
}

uint8_t i2cMasterRegisterRead(hDev device, uint8_t registeraddr, uint8_t * buff, uint8_t szToRead)
{
    bus_instance_i2c bus = devices[device].bus;

    i2cWaitForStopComplete(bus);

    return i2cMasterCombinedWriteRead(device, &registeraddr, 1, buff, szToRead);
}

// Primary interrupt vector for I2C on module B1 (I2C #1)
#pragma vector = EUSCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    switch(__even_in_range(UCB1IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:          break;     // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:   break;     // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG:            // Vector 4: NACKIFG
            UCB1CTL1 |= UCTXSTT;            // Received NACK ... try again, start
            break;
        case USCI_I2C_UCSTTIFG:  break;     // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:  break;     // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:  break;     // Vector 10: RXIFG3
        case USCI_I2C_UCTXIFG3:  break;     // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:  break;     // Vector 14: RXIFG2
        case USCI_I2C_UCTXIFG2:  break;     // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:  break;     // Vector 18: RXIFG1
        case USCI_I2C_UCTXIFG1:  break;     // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:  break;     // Vector 22: RXIFG0
        case USCI_I2C_UCTXIFG0:  break;     // Vector 24: TXIFG0
        case USCI_I2C_UCBCNTIFG: break;     // Vector 26: BCNTIFG
        case USCI_I2C_UCCLTOIFG: break;     // Vector 28: clock low timeout
        case USCI_I2C_UCBIT9IFG: break;     // Vector 30: 9th bit
        default: break;
    }
}

// Primary interrupt vector for I2C on module B2 (I2C #2)
#pragma vector = EUSCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{
    switch(__even_in_range(UCB2IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:          break;     // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:   break;     // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG:            // Vector 4: NACKIFG
            UCB2CTL1 |= UCTXSTT;            // Received NACK ... try again, start
            break;
        case USCI_I2C_UCSTTIFG:  break;     // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:  break;     // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:  break;     // Vector 10: RXIFG3
        case USCI_I2C_UCTXIFG3:  break;     // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:  break;     // Vector 14: RXIFG2
        case USCI_I2C_UCTXIFG2:  break;     // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:  break;     // Vector 18: RXIFG1
        case USCI_I2C_UCTXIFG1:  break;     // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:  break;     // Vector 22: RXIFG0
        case USCI_I2C_UCTXIFG0:  break;     // Vector 24: TXIFG0
        case USCI_I2C_UCBCNTIFG: break;     // Vector 26: BCNTIFG
        case USCI_I2C_UCCLTOIFG: break;     // Vector 28: clock low timeout
        case USCI_I2C_UCBIT9IFG: break;     // Vector 30: 9th bit
        default: break;
    }
}

