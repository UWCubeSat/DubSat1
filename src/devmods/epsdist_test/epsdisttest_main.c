#include <epsdisttest.h>
#include <msp430.h> 



/*
 * main.c
 */


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

#define SHUNT_HIGH_DRAW_DEVICE  0.01f
#define SHUNT_LOW_DRAW_DEVICE   0.1f


// Capture power domain info, IDs, addresses, and enable pins
// DO NOT REORDER
typedef enum {
    PD_COM1,
    PD_COM2,
    PD_RAHS,
    PD_BDOT,
    PD_ESTIM,
    PD_WHEELS,
    PD_EPS,
    PD_PPT,
} PowerDomainID;

// DO NOT REORDER
FILE_STATIC uint8_t *powerDomainNames[] =
                                           {
                                            "COM1",
                                            "COM2",
                                            "RAHS",
                                            "BDOT",
                                            "ESTIM",
                                            "WHEELS",
                                            "EPS",
                                            "PPT",
                                           };


typedef struct _power_domain_info {
    PowerDomainID id;
    uint8_t i2caddr;
    hDev hpcvsensor;
    float lastShuntCurrent;

    // eventually add other stuff, like handle to the averaging queues, etc.
} PowerDomainInfo;

COSMOS_TLM_PACKET {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    uint8_t powerdomainlastcmds[NUM_POWER_DOMAINS];
    uint8_t powerdomainswitchstate[NUM_POWER_DOMAINS];
} general_packet;

COSMOS_TLM_PACKET {
    BcTlmHeader header;  // All COSMOS TLM packets must have this

    float currents[NUM_POWER_DOMAINS];
} sensordat_packet;

COSMOS_CMD_PACKET {
    uint8_t pd_cmds[NUM_POWER_DOMAINS];
} domaincmd_packet;


FILE_STATIC PowerDomainInfo powerdomains[NUM_POWER_DOMAINS];
FILE_STATIC PCVSensorData *powerdomainData[NUM_POWER_DOMAINS];


PCVSensorData *sensorData;
hDev i2cdev, hSensor;
FILE_STATIC general_packet gpkt;
FILE_STATIC sensordat_packet spkt;
FILE_STATIC health_packet hpkt;

// DO NOT REORDER
FILE_STATIC uint8_t domainsSensorAddresses[] =   { 0x43, 0x40, 0x44, 0x42, 0x45, 0x4E, 0x46, 0x41  };
FILE_STATIC float   domainShuntResistances[] =   { SHUNT_LOW_DRAW_DEVICE, SHUNT_HIGH_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE,
                                                   SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_HIGH_DRAW_DEVICE };

#define MAX_BUFF_SIZE   0x10
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

void distDomainInit()
{
    // Make sure all outputs are 0 before starting
    DOMAIN_ENABLE_COM1_OUT = 0;
    DOMAIN_ENABLE_COM2_OUT = 0;
    DOMAIN_ENABLE_RAHS_OUT = 0;
    DOMAIN_ENABLE_BDOT_OUT = 0;
    DOMAIN_ENABLE_ESTIM_OUT = 0;
    DOMAIN_ENABLE_WHEELS_OUT = 0;
    DOMAIN_ENABLE_EPS_OUT = 0;
    DOMAIN_ENABLE_PPT_OUT = 0;

    // Setup GPIO pin used to turn on all INA219's and turn them on
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

    DOMAIN_ENABLE_CURRENT_SENSORS_OUT |= DOMAIN_ENABLE_CURRENT_SENSORS_BIT;
    __delay_cycles(0.1 * SEC);

    int d;
    for (d = 0; d < NUM_POWER_DOMAINS; d++)
    {
        powerdomains[d].id = (PowerDomainID)d;
        powerdomains[d].i2caddr = domainsSensorAddresses[d];
        powerdomains[d].hpcvsensor = pcvsensorInit(I2CBus2, powerdomains[d].i2caddr, domainShuntResistances[d], 16.0f);
        __delay_cycles(1000);  // Helps make sure the initialization is received properly
    }
}

PowerDomainSwitchState distQueryDomainSwitch(PowerDomainID domain)
{
    BOOL enabled = FALSE;

    switch (domain)
    {
        case PD_COM2:
            enabled = ((DOMAIN_ENABLE_COM2_OUT & DOMAIN_ENABLE_COM2_BIT) != 0);
            break;
        case PD_PPT:
            enabled = ((DOMAIN_ENABLE_PPT_OUT & DOMAIN_ENABLE_PPT_BIT) != 0);
            break;
        case PD_BDOT:
            enabled = ((DOMAIN_ENABLE_BDOT_OUT & DOMAIN_ENABLE_BDOT_BIT) != 0);
            break;
        case PD_COM1:
            enabled = ((DOMAIN_ENABLE_COM1_OUT & DOMAIN_ENABLE_COM1_BIT) != 0);
            break;
        case PD_RAHS:
            enabled = ((DOMAIN_ENABLE_RAHS_OUT & DOMAIN_ENABLE_RAHS_BIT) != 0);
            break;
        case PD_ESTIM:
            enabled = ((DOMAIN_ENABLE_ESTIM_OUT & DOMAIN_ENABLE_ESTIM_BIT) != 0);
            break;
        case PD_EPS:
            enabled = ((DOMAIN_ENABLE_EPS_OUT & DOMAIN_ENABLE_EPS_BIT) != 0);
            break;
        case PD_WHEELS:
            enabled = ((DOMAIN_ENABLE_WHEELS_OUT & DOMAIN_ENABLE_WHEELS_BIT) != 0);
            break;
        default:
            break;
    }

    return ( enabled == TRUE ? Switch_Enabled : Switch_Disabled);
}

void distDomainSwitch(PowerDomainID domain, PowerDomainCmd cmd )
{
    gpkt.powerdomainlastcmds[(uint8_t)domain] = (uint8_t)cmd;

    if (cmd == PD_CMD_NoChange)
        return;

    switch (domain)
    {
        case PD_COM2:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_COM2_OUT |= DOMAIN_ENABLE_COM2_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_COM2_OUT &= ~DOMAIN_ENABLE_COM2_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_COM2_OUT ^= DOMAIN_ENABLE_COM2_BIT;
            break;
        case PD_PPT:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_PPT_OUT |= DOMAIN_ENABLE_PPT_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_PPT_OUT &= ~DOMAIN_ENABLE_PPT_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_PPT_OUT ^= DOMAIN_ENABLE_PPT_BIT;
            break;
        case PD_BDOT:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_BDOT_OUT |= DOMAIN_ENABLE_BDOT_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_BDOT_OUT &= ~DOMAIN_ENABLE_BDOT_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_BDOT_OUT ^= DOMAIN_ENABLE_BDOT_BIT;
            break;
        case PD_COM1:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_COM1_OUT |= DOMAIN_ENABLE_COM1_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_COM1_OUT &= ~DOMAIN_ENABLE_COM1_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_COM1_OUT ^= DOMAIN_ENABLE_COM1_BIT;
            break;
        case PD_RAHS:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_RAHS_OUT |= DOMAIN_ENABLE_RAHS_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_RAHS_OUT &= ~DOMAIN_ENABLE_RAHS_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_RAHS_OUT ^= DOMAIN_ENABLE_RAHS_BIT;
            break;
        case PD_ESTIM:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_ESTIM_OUT |= DOMAIN_ENABLE_ESTIM_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_ESTIM_OUT &= ~DOMAIN_ENABLE_ESTIM_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_ESTIM_OUT ^= DOMAIN_ENABLE_ESTIM_BIT;
            break;
        case PD_EPS:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_EPS_OUT |= DOMAIN_ENABLE_EPS_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_EPS_OUT &= ~DOMAIN_ENABLE_EPS_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_EPS_OUT ^= DOMAIN_ENABLE_EPS_BIT;
            break;
        case PD_WHEELS:
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_WHEELS_OUT |= DOMAIN_ENABLE_WHEELS_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_WHEELS_OUT &= ~DOMAIN_ENABLE_WHEELS_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_WHEELS_OUT ^= DOMAIN_ENABLE_WHEELS_BIT;
            break;
        default:
            break;
    }
}

FILE_STATIC void distPopulateSensorData()
{
    int i;
    PCVSensorData *pdata;
    for (i=0; i < NUM_POWER_DOMAINS; i++)
    {
        pdata = pcvsensorRead(powerdomains[i].hpcvsensor, Read_CurrentOnly);
        powerdomainData[i] = pdata;
        spkt.currents[i] = pdata->calcdCurrentA;

    }
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void distBcSendGeneral()
{
    int i;

    for (i=0; i < NUM_POWER_DOMAINS; i++)
    {
        gpkt.powerdomainswitchstate[i] = (uint8_t)distQueryDomainSwitch((PowerDomainID)i);

    }
    // TODO:  Determine call function to determine true general status
    // For now, everythingis always marginal ...
    bcbinSendPacket((uint8_t *) &gpkt, sizeof(gpkt));
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void distBcSendHealth()
{
    // TODO:  Determine overall health based on querying various entities for their health
    // For now, everythingis always marginal ...
    hpkt.module = Module_EPS_Dist;
    hpkt.oms = OMS_Unknown;
    bcbinSendPacket((uint8_t *) &hpkt, sizeof(hpkt));
}

// Packetizes and sends backchannel SENSORDAT packet
FILE_STATIC void distBcSendSensorDat()
{
    bcbinSendPacket((uint8_t *) &spkt, sizeof(spkt));
}


uint8_t distActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    domaincmd_packet *packet;
    int i;

    if (mode == Mode_BinaryStreaming)
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_DOMAINSWITCH:
                packet = (domaincmd_packet *) &cmdstr[1];
                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    distDomainSwitch((PowerDomainID)i, (PowerDomainCmd)(packet->pd_cmds[i]));
                }
                break;
            default:
                break;
        }
    }
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_EPS_Dist);

    distDomainInit();

    LED_DIR |= LED_BIT;

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
    int domindex = 0;
    int onoff = 1;  // 0 means we are disabling power domains, 1 means enabling
    PowerDomainID currdom;

    // Populate the non-changing parts of the telemetry packets (e.g. fixed ID and size)
    bcbinPopulateHeader(&(hpkt.header), TLM_ID_SHARED_HEALTH, sizeof(hpkt));
    bcbinPopulateHeader(&(gpkt.header), TLM_ID_EPS_DIST_GENERAL, sizeof(gpkt));
    bcbinPopulateHeader(&(spkt.header), TLM_ID_EPS_DIST_SENSORDAT, sizeof(spkt));

    // Setup commanding callback
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, distActionCallback);

    __delay_cycles(0.5 * SEC);

    //debugPrintF("COM1Vb,Vs,A,COM2Vb,Vs,A,RAHSVb,Vs,A,BDOTVb,Vs,A,ESTIMVb,Vs,A,WHEELVb,Vs,A,EPSVb,Vs,A,PPTVb,Vs,A\r\n");

    uint16_t counter = 0;
    distDomainSwitch(PD_PPT, PD_CMD_Enable);
    while (1)
    {
        __delay_cycles(0.1 * SEC);
        LED_OUT ^= LED_BIT;

        currdom = (PowerDomainID)domindex;

        distPopulateSensorData();
//        debugPrintF("%.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f, %.4f,%.4f,%.4f\r\n",
//                    powerdomainData[0]->busVoltageV, powerdomainData[0]->shuntVoltageV, powerdomainData[0]->calcdCurrentA,
//                    powerdomainData[1]->busVoltageV, powerdomainData[1]->shuntVoltageV, powerdomainData[1]->calcdCurrentA,
//                    powerdomainData[2]->busVoltageV, powerdomainData[2]->shuntVoltageV, powerdomainData[2]->calcdCurrentA,
//                    powerdomainData[3]->busVoltageV, powerdomainData[3]->shuntVoltageV, powerdomainData[3]->calcdCurrentA,
//                    powerdomainData[4]->busVoltageV, powerdomainData[4]->shuntVoltageV, powerdomainData[4]->calcdCurrentA,
//                    powerdomainData[5]->busVoltageV, powerdomainData[5]->shuntVoltageV, powerdomainData[5]->calcdCurrentA,
//                    powerdomainData[6]->busVoltageV, powerdomainData[6]->shuntVoltageV, powerdomainData[6]->calcdCurrentA,
//                    powerdomainData[7]->busVoltageV, powerdomainData[7]->shuntVoltageV, powerdomainData[7]->calcdCurrentA);

        //distDomainSwitch(currdom, (onoff==1 ? Enable : Disable));

        if (domindex >= NUM_POWER_DOMAINS-1)
        {
            domindex = 0;
            onoff = ( onoff==1 ? 0 : 1);
        }
        else
            domindex++;

        counter++;
        distBcSendSensorDat();
        if (counter >= 10)
        {
            distBcSendGeneral();
            distBcSendHealth();
            counter = 0;
        }
    }

	return 0;
}
