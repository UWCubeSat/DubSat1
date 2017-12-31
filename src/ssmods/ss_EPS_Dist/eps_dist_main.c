#include <eps_dist.h>
#include <msp430.h> 

#include "bsp/bsp.h"

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status;
FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;

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

FILE_STATIC PowerDomainInfo powerdomains[NUM_POWER_DOMAINS];

// DO NOT REORDER
FILE_STATIC uint8_t domainsSensorAddresses[] =   { 0x43, 0x40, 0x44, 0x42, 0x45, 0x4E, 0x46, 0x41  };
FILE_STATIC float   domainShuntResistances[] =   { SHUNT_LOW_DRAW_DEVICE, SHUNT_HIGH_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE,
                                                   SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_LOW_DRAW_DEVICE, SHUNT_HIGH_DRAW_DEVICE };

FILE_STATIC float domainCurrentThresholdInitial[] = { OCP_THRESH_LOW_DRAW_DEVICE, OCP_THRESH_MED_DRAW_DEVICE, OCP_THRESH_LOW_DRAW_DEVICE,
                                               OCP_THRESH_LOW_DRAW_DEVICE, OCP_THRESH_LOW_DRAW_DEVICE, OCP_THRESH_LOW_DRAW_DEVICE,
                                               OCP_THRESH_LOW_DRAW_DEVICE, OCP_THRESH_HIGH_DRAW_DEVICE };

PCVSensorData *sensorData;
hDev i2cdev, hSensor;

// Packet instances
FILE_STATIC meta_packet mpkt;
FILE_STATIC general_packet gpkt;
FILE_STATIC sensordat_packet spkt;
FILE_STATIC health_packet hpkt;

#define MAX_BUFF_SIZE   0x10
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

// Sets up all the power domain switch GPIO pins to be ready to enable/disable
void distDomainInit()
{
    // Make sure all outputs are 0 before starting - this is MANDATORY
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
    __delay_cycles(0.1 * SEC);  // This timing is important, to allow time for the main sensor switch (which
                                // powers all the INA219's) to turn on fully

    int d;
    for (d = 0; d < NUM_POWER_DOMAINS; d++)
    {
        powerdomains[d].id = (PowerDomainID)d;
        powerdomains[d].i2caddr = domainsSensorAddresses[d];
        powerdomains[d].hpcvsensor = pcvsensorInit(I2CBus2, powerdomains[d].i2caddr, domainShuntResistances[d], 16.0f);
        __delay_cycles(1000);  // Helps make sure the initialization is received properly
    }

    distInitializeOCPThresholds();
}

// Uses the actual GPIO output value to determine what is "actually" happening with the switch
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

// Turns on/off switches for indicated domain
void distDomainSwitch(PowerDomainID domain, PowerDomainCmd cmd )
{
    gpkt.powerdomainlastcmds[(uint8_t)domain] = (uint8_t)cmd;

    // Overcurrent latching command only useful as a "special" disable for reporting purposes
    if (cmd == PD_CMD_OCLatch)
        cmd = PD_CMD_Disable;

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

    // Clear the current-limited flag if turning on
    if (distQueryDomainSwitch(domain) == Switch_Enabled)
        spkt.powerdomaincurrentlimited[(uint8_t)domain] = 0;
}

// Make a pass through all the sensors
FILE_STATIC void distMonitorDomains()
{
    int i;
    PCVSensorData *pdata;
    for (i=0; i < NUM_POWER_DOMAINS; i++)
    {
        pdata = pcvsensorRead(powerdomains[i].hpcvsensor, Read_CurrentOnly);

        if (pdata->calcdCurrentA >= gpkt.powerdomainocpthreshold[i])
        {
            distDomainSwitch((PowerDomainID)i, PD_CMD_OCLatch);  // Yes, this means Disable is ALWAYS sent if current too high
            if (spkt.powerdomaincurrentlimited[i] != 1)
            {
                spkt.powerdomaincurrentlimited[i] = 1;
                gpkt.powerdomaincurrentlimitedcount[i] += 1;

            }
        }

        // Save data for each sensor, regardless of threshold
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
    hpkt.oms = OMS_Unknown;
    bcbinSendPacket((uint8_t *) &hpkt, sizeof(hpkt));
}

// Packetizes and sends backchannel SENSORDAT packet
FILE_STATIC void distBcSendSensorDat()
{
    bcbinSendPacket((uint8_t *) &spkt, sizeof(spkt));
}

FILE_STATIC void distBcSendMeta()
{
    bcbinSendPacket((uint8_t *) &mpkt, sizeof(mpkt));
}

void distSetOCPThreshold(PowerDomainID domain, float newval)
{
    int i;

    // 0.0f means don't change current value
    if (newval == 0.0f)
        return;

    gpkt.powerdomainocpthreshold[(uint8_t)domain] = newval;
}

void distInitializeOCPThresholds()
{
    int i;
    for (i = 0; i < NUM_POWER_DOMAINS; i++)
    {
        distSetOCPThreshold((PowerDomainID)i, domainCurrentThresholdInitial[i]);
    }
}

// Called when command routing infrastructure detects a command "addressed" to the subsystem
uint8_t distActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    domaincmd_packet *dpacket;
    commoncmd_packet *cpacket;
    ocpthresh_packet *opacket;
    int i;
    float newval;

    if (mode == Mode_BinaryStreaming)
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_DOMAINSWITCH:
                dpacket = (domaincmd_packet *) &cmdstr[1];
                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    distDomainSwitch((PowerDomainID)i, (PowerDomainCmd)(dpacket->pd_cmds[i]));
                }
                break;
            case OPCODE_COMMONCMD:
                cpacket = (commoncmd_packet *) &cmdstr[1];
                LED_OUT ^= LED_BIT;
                break;
            case OPCODE_OCPTHRESH:
                opacket = (ocpthresh_packet *) &cmdstr[1];
                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    distSetOCPThreshold((PowerDomainID)i, opacket->newCurrentThreshold[i]);
                }
                break;
            default:
                break;
        }
    }
}

/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(Module_EPS_Dist);  // <<DO NOT DELETE or MOVE>>

    distDomainInit();

    LED_DIR |= LED_BIT;

    // Setup COSMOS telemetry packet structures
    bcbinPopulateMeta(&mpkt, sizeof(mpkt));
    bcbinPopulateHeader(&(hpkt.header), TLM_ID_SHARED_HEALTH, sizeof(hpkt));
    bcbinPopulateHeader(&(gpkt.header), TLM_ID_EPS_DIST_GENERAL, sizeof(gpkt));
    bcbinPopulateHeader(&(spkt.header), TLM_ID_EPS_DIST_SENSORDAT, sizeof(spkt));

    //mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)

    // Register to handle telecommands
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, distActionCallback);
    __delay_cycles(0.5 * SEC);



#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers


    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    uint16_t counter = 0;
    while (1)
    {
        // TODO:  eventually drive this with a timer
        LED_OUT ^= LED_BIT;
        __delay_cycles(0.1 * SEC);

        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
            case State_FirstState:
                distMonitorDomains();

                counter++;
                distBcSendSensorDat();
                if (counter % 8 == 0)
                {
                    distBcSendGeneral();
                    distBcSendHealth();
                }
                if (counter % 64 == 0)
                    distBcSendMeta();
                break;
            case State_SecondState:
                // fall through
            case State_ThirdState:
                // fall through
            default:
                mod_status.state_transition_errors++;
                mod_status.in_unknown_state++;
                break;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}


