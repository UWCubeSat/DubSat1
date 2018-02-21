/*
 * bsp.c
 *
 *  Created on: May 5, 2017
 *      Author: jeffc
 */

#include "bsp/bsp.h"
#include "interfaces/systeminfo.h"
#include "core/uart.h"
#include "core/i2c.h"

void bspUARTInit(bus_instance_UART instance);
void bspI2CInit( bus_instance_i2c instance);

FILE_STATIC SubsystemModule ssModule;

// TEMPLATE FUNCTION:  All functions in this file should look roughly like this
/*
void bspExampleInit(SubsystemModule mod)
{
    // General config, shared by all board types

    // Specific board configuration steps

#if defined(__BSP_Board_MSP430FR5994LaunchPad__)
    // LaunchPad for MSP430FR5994

#else
#error Unspecified board hardware, unable to determine correct BSP implementation.  Please specify board.
#endif

}
*/

FILE_STATIC hwsw_match_state hwsw_mstate;
hwsw_match_state bspGetHWSWMatchState()
{
    return hwsw_mstate;
}

uint8_t infoReport(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        debugPrintF("**Subsystem Module: \t%s\r\n", getSubsystemModulePath());
        debugPrintF("Compiler Version:\t%d\r\n", __TI_COMPILER_VERSION__);
        debugPrintF("Standard C Version:\t%d\r\n", __STDC_VERSION__);
        debugPrintF("Date Compiled:\t\t%s\r\n", __DATE__);
        debugPrintF("Time Compiled:\t\t%s\r\n", __TIME__);
    }
    else
    {
        // Output raw data in CSV form, without field names
    }
    return 1;
}

FILE_STATIC uint64_t chipID;
uint64_t bspGetChipID()
{
    return chipID;
}

FILE_STATIC hwsw_match_state enforceHWSWLock()
{

#if defined(__ENABLE_HWSW_MATCH__)

    if (NUM_HWKEYS == 0)
    {
        hwsw_mstate = HWSW_NoKeysProvided;
        return HWSW_NoKeysProvided;
    }

    // Keys are provided, so check them
    int i;
    for (i = 0; i < NUM_HWKEYS; i++)
    {
        if (chipID == hw_keys[i])
        {
            hwsw_mstate = HWSW_Matched;
            return HWSW_Matched;
        }

    }

    // Fall-through - no match found
    hwsw_mstate = HWSW_LockViolation;
    return HWSW_LockViolation;

#else

    hwsw_mstate = HWSW_LockNotEnabled;
    return HWSW_LockNotEnabled;

#endif
}

FILE_STATIC meta_segment mseg;   // Used if something fails before init complete
void bspInit(SubsystemModule mod)
{
    ssModule = mod;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // NOW, CHECK HARDWARE KEY - if it doesn't match, this never returns ...
    chipID = *((uint64_t *)0x1A0A);
    enforceHWSWLock();

    //Selection bits for external crystal and ACLK
    PJSEL0 = BIT4 | BIT5;

    // SAFE way of setting clock to 8Mhz, from
    // per-device errata:  must set divider to 4 before changing frequency to
    // prevent out of spec operation from overshoot transient
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
    // Set SMCLK = MCLK = DCO, ACLK = LFXTCLK (was VLOCLK earlier)
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;

    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all corresponding clk sources to divide by 4 for errata
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz

    // Delay by ~10us to let DCO settle. 60 cycles = 20 cycles buffer + (10us / (1/4MHz))
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1 for 8MHz operation
    
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
 
 
    CSCTL0_H = 0;                           // Lock CS Registers

    // Force all outputs to be 0, so we don't get spurious signals when we unlock
    P1OUT = 0;
    P2OUT = 0;
    P3OUT = 0;
    P4OUT = 0;
    P5OUT = 0;
    P6OUT = 0;
    P7OUT = 0;
    P8OUT = 0;
    PJOUT = 0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

#if defined(__DEBUG__)

    debugInit();

    // Now lock up, since the serial port now works but not much else has happened
    if (hwsw_mstate == HWSW_LockViolation)
    {
        bcbinPopulateMeta(&mseg, sizeof(mseg));

        while(1)
        {
            bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
            __delay_cycles(3 * SEC);
        }
    }

    // Register the system info report function
    // TODO:  Merge systeminfo and BSP, they aren't both needed
    debugRegisterEntity(Entity_BSP, infoReport, NULL, NULL);

    debugTraceF(1,"\r\n-------------------------------------------------------\r\nBSP initialization routine complete.\r\n");

#endif // __DEBUG__

    __enable_interrupt();

}

void bspUARTInit(bus_instance_UART instance)
{
    // LaunchPad for MSP430FR5994
    if (instance == BackchannelUART)
    {
        BACKCHANNEL_UART_SEL0 &= ~BACKCHANNEL_UART_BITS;
        BACKCHANNEL_UART_SEL1 |= BACKCHANNEL_UART_BITS;
    }
    else if (instance == ApplicationUART)
    {
        APP_UART_SEL0 &= ~APP_UART_BITS;
        APP_UART_SEL1 |= APP_UART_BITS;
    }
}

// NOTE:  The LaunchPad only exposes pins for the I2C bus we call "Bus2", hence the macro usage
void bspI2CInit(bus_instance_i2c instance)
{
    if (instance == I2CBus2)
    {
        I2C2_PORTSEL1 &= ~(I2C2_SDA_BIT | I2C2_SCL_BIT);
        I2C2_PORTSEL0 |= (I2C2_SDA_BIT | I2C2_SCL_BIT);
    }
#if !defined(__BSP_Board_MSP430FR5994LaunchPad__)
    else if (instance == I2CBus1)
    {
        I2C1_PORTSEL1 &= ~(I2C1_SDA_BIT | I2C1_SCL_BIT);
        I2C1_PORTSEL0 |= (I2C1_SDA_BIT | I2C1_SCL_BIT);
    }
#endif

}


SubsystemModule bspGetModule()
{
    return ssModule;
}


