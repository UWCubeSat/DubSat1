#include <epsdisttest.h>
#include <msp430.h> 

#include "bsp/bsp.h"

/*
 * main.c
 */

// Capture power domain info, IDs, addresses, and enable pins
typedef enum {
    PD_COM2,
    PD_PPT,
    PD_BDOT,
    PD_COM1,
    PD_RAHS,
    PD_ESTIM,
    PD_EPS,
    PD_WHEELS
} PowerDomainID;

FILE_STATIC uint8_t *powerDomainNames[] =
                                           { "COM2",
                                             "PPT",
                                             "BDOT",
                                             "COM1",
                                             "RAHS",
                                             "ESTIM",
                                             "EPS",
                                             "WHEELS"};

typedef enum {
    Disable,
    Enable
} DomainEnable;


typedef struct _power_domain_info {
    PowerDomainID id;
    uint8_t i2caddr;
    hDev hpcvsensor;
    float lastShuntCurrent;

    // eventually add other stuff, like handle to the averaging queues, etc.
} PowerDomainInfo;


#define NUM_POWER_DOMAINS  8
#define DOMAIN_ENABLE_COM2_DIR  P7DIR
#define DOMAIN_ENABLE_COM2_OUT  P7OUT
#define DOMAIN_ENABLE_COM2_BIT  BIT4

#define DOMAIN_ENABLE_PPT_DIR  P2DIR
#define DOMAIN_ENABLE_PPT_OUT  P2OUT
#define DOMAIN_ENABLE_PPT_BIT  BIT5

#define DOMAIN_ENABLE_BDOT_DIR  P4DIR
#define DOMAIN_ENABLE_BDOT_OUT  P4OUT
#define DOMAIN_ENABLE_BDOT_BIT  BIT1

#define DOMAIN_ENABLE_COM1_DIR  P7DIR
#define DOMAIN_ENABLE_COM1_OUT  P7OUT
#define DOMAIN_ENABLE_COM1_BIT  BIT3

#define DOMAIN_ENABLE_RAHS_DIR  P4DIR
#define DOMAIN_ENABLE_RAHS_OUT  P4OUT
#define DOMAIN_ENABLE_RAHS_BIT  BIT0

#define DOMAIN_ENABLE_ESTIM_DIR  P4DIR
#define DOMAIN_ENABLE_ESTIM_OUT  P4OUT
#define DOMAIN_ENABLE_ESTIM_BIT  BIT2

#define DOMAIN_ENABLE_EPS_DIR  P1DIR
#define DOMAIN_ENABLE_EPS_OUT  P1OUT
#define DOMAIN_ENABLE_EPS_BIT  BIT6

#define DOMAIN_ENABLE_WHEELS_DIR  P4DIR
#define DOMAIN_ENABLE_WHEELS_OUT  P4OUT
#define DOMAIN_ENABLE_WHEELS_BIT  BIT3


#define DOMAIN_ENABLE_CURRENT_SENSORS_DIR   P3DIR
#define DOMAIN_ENABLE_CURRENT_SENSORS_OUT   P3OUT
#define DOMAIN_ENABLE_CURRENT_SENSORS_BIT   BIT4

#define LED_DIR P3DIR
#define LED_OUT P3OUT
#define LED_BIT BIT5

FILE_STATIC PowerDomainInfo powerdomains[NUM_POWER_DOMAINS];
FILE_STATIC uint8_t domainsSensorAddresses[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x4E };

void domainInit()
{
    // Setup GPIO pin used to turn on all INA219's
    DOMAIN_ENABLE_CURRENT_SENSORS_DIR |= DOMAIN_ENABLE_CURRENT_SENSORS_BIT;

    // Setup GPIO pins used to turn off/on each power domain
    DOMAIN_ENABLE_COM2_DIR  |=  DOMAIN_ENABLE_COM2_BIT;
    DOMAIN_ENABLE_PPT_DIR   |=  DOMAIN_ENABLE_PPT_BIT;
    DOMAIN_ENABLE_BDOT_DIR  |=  DOMAIN_ENABLE_BDOT_BIT;
    DOMAIN_ENABLE_COM1_DIR  |=  DOMAIN_ENABLE_COM1_BIT;
    DOMAIN_ENABLE_RAHS_DIR  |=  DOMAIN_ENABLE_RAHS_BIT;
    DOMAIN_ENABLE_ESTIM_DIR |=  DOMAIN_ENABLE_ESTIM_BIT;
    DOMAIN_ENABLE_EPS_DIR   |=  DOMAIN_ENABLE_EPS_BIT;
    DOMAIN_ENABLE_WHEELS_DIR |= DOMAIN_ENABLE_WHEELS_BIT;

//    int d;
//    for (d = 0; d < NUM_POWER_DOMAINS; d++)
//    {
//        powerdomains[d].id = (PowerDomainID)d;
//        powerdomains[d].i2caddr = domainsSensorAddresses[d];
//        powerdomains[d].hpcvsensor = pcvsensorInit(I2CBus2, powerdomains[d].i2caddr, 0.1f, 16.0f);
//
//    }
}

void domainSwitch(PowerDomainID domain, DomainEnable enable)
{
    switch (domain)
    {
        case PD_COM2:
            if (enable == Enable)
                DOMAIN_ENABLE_COM2_OUT |= DOMAIN_ENABLE_COM2_BIT;
            else
                DOMAIN_ENABLE_COM2_OUT &= ~DOMAIN_ENABLE_COM2_BIT;
            break;
        case PD_PPT:
            if (enable == Enable)
                DOMAIN_ENABLE_PPT_OUT |= DOMAIN_ENABLE_PPT_BIT;
            else
                DOMAIN_ENABLE_PPT_OUT &= ~DOMAIN_ENABLE_PPT_BIT;
            break;
        case PD_BDOT:
            if (enable == Enable)
                DOMAIN_ENABLE_BDOT_OUT |= DOMAIN_ENABLE_BDOT_BIT;
            else
                DOMAIN_ENABLE_BDOT_OUT &= ~DOMAIN_ENABLE_BDOT_BIT;
            break;
        case PD_COM1:
            if (enable == Enable)
                DOMAIN_ENABLE_COM1_OUT |= DOMAIN_ENABLE_COM1_BIT;
            else
                DOMAIN_ENABLE_COM1_OUT &= ~DOMAIN_ENABLE_COM1_BIT;
            break;
        case PD_RAHS:
            if (enable == Enable)
                DOMAIN_ENABLE_RAHS_OUT |= DOMAIN_ENABLE_RAHS_BIT;
            else
                DOMAIN_ENABLE_RAHS_OUT &= ~DOMAIN_ENABLE_RAHS_BIT;
            break;
        case PD_ESTIM:
            if (enable == Enable)
                DOMAIN_ENABLE_ESTIM_OUT |= DOMAIN_ENABLE_ESTIM_BIT;
            else
                DOMAIN_ENABLE_ESTIM_OUT &= ~DOMAIN_ENABLE_ESTIM_BIT;
            break;
        case PD_EPS:
            if (enable == Enable)
                DOMAIN_ENABLE_EPS_OUT |= DOMAIN_ENABLE_EPS_BIT;
            else
                DOMAIN_ENABLE_EPS_OUT &= ~DOMAIN_ENABLE_EPS_BIT;
            break;
        case PD_WHEELS:
            if (enable == Enable)
                DOMAIN_ENABLE_WHEELS_OUT |= DOMAIN_ENABLE_WHEELS_BIT;
            else
                DOMAIN_ENABLE_WHEELS_OUT &= ~DOMAIN_ENABLE_WHEELS_BIT;
            break;
        default:
            break;

    }

}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_EPS_Dist);

    P1OUT = 0;
    P2OUT = 0;
    P4OUT = 0;
    P7OUT = 0;
    domainInit();
    LED_DIR |= LED_BIT;

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
    DOMAIN_ENABLE_CURRENT_SENSORS_OUT |= DOMAIN_ENABLE_CURRENT_SENSORS_BIT;

    int domindex = 0;
    int onoff = 1;  // 0 means we are disabling power domains, 1 means enabling
    PowerDomainID currdom;
    while (1)
    {
        currdom = (PowerDomainID)domindex;
        __delay_cycles(0.5 * SEC);
        LED_OUT ^= LED_BIT;

        debugPrintF("Toggling power domain %s ...\r\n", powerDomainNames[domindex]);
        domainSwitch(currdom, (onoff==1 ? Enable : Disable));

        if (domindex >= NUM_POWER_DOMAINS-1)
        {
            domindex = 0;
            onoff = ( onoff==1 ? 0 : 1);
        }
        else
            domindex++;
    }

	return 0;
}
