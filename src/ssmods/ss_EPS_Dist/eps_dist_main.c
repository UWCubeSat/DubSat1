#include <eps_dist.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"

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

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC general_segment gseg;
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

FILE_STATIC hDev hBattV;

#define MAX_BUFF_SIZE   0x10
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

FILE_STATIC uint16_t startupDelay = 1800;
#pragma PERSISTENT(startupDelay)

FILE_STATIC uint8_t rcFlag = 0;

void distDeployInit()
{
    DEPLOY_ENABLE_DIR |= DEPLOY_ENABLE_BIT;

    // Make sure output is low
    DEPLOY_ENABLE_OUT = 0;
}

void distFireDeploy()
{
    DEPLOY_ENABLE_OUT |= DEPLOY_ENABLE_BIT;
}

// Sets up all the power domain switch GPIO pins to be ready to enable/disable
void distDomainInit()
{
    // Set initial thresholds for undervoltage monitoring
    gseg.undervoltagethreshold = BATT_DEFAULT_FULL_THRESH;

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
    // Record last command for each domain
    gseg.powerdomainlastcmds[(uint8_t)domain] = (uint8_t)cmd;

    // Overcurrent latching  or low batt commands only useful as a "special" disable for reporting purposes
    if (cmd == PD_CMD_OCLatch || cmd == PD_CMD_BattVLow)
        cmd = PD_CMD_Disable;

    // Similarly, autostart is a just a "special" enable
    if (cmd == PD_CMD_AutoStart)
        cmd = PD_CMD_Enable;

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
        sseg.powerdomaincurrentlimited[(uint8_t)domain] = 0;
}

// Make a pass through all the sensors
FILE_STATIC void distMonitorDomains()
{
    int i;
    PCVSensorData *pdata;
    for (i=0; i < NUM_POWER_DOMAINS; i++)
    {
        pdata = pcvsensorRead(powerdomains[i].hpcvsensor, Read_CurrentA | Read_BusV);

        if (pdata->calcdCurrentA >= gseg.powerdomainocpthreshold[i])
        {
            distDomainSwitch((PowerDomainID)i, PD_CMD_OCLatch);  // Yes, this means Disable is ALWAYS sent if current too high
            if (sseg.powerdomaincurrentlimited[i] != 1)
            {
                sseg.powerdomaincurrentlimited[i] = 1;
                gseg.powerdomaincurrentlimitedcount[i] += 1;
            }
        }

        // Save data for each sensor, regardless of threshold
        sseg.currents[i] = pdata->calcdCurrentA;
        sseg.busV[i] = pdata->busVoltageV;
    }
}

// TODO: commands to change threshold on the fly
FILE_STATIC void distMonitorBattery()
{
    int i;
    float predivV = asensorReadSingleSensorV(hBattV);
    float newbattV = BATTV_CONV_FACTOR * predivV;
    float prevBattV = gseg.battV;
    gseg.battV = newbattV;

    uint8_t prevMode = (uint8_t)gseg.uvmode;

    if (newbattV <= gseg.undervoltagethreshold && prevBattV <= gseg.undervoltagethreshold)
        gseg.uvmode = (uint8_t)UV_FullShutdown;
    else
        gseg.uvmode = (uint8_t)UV_InRange;

    //shut down everything (COM1 is hard-wired to never shut down)
    if (gseg.uvmode != (uint8_t)UV_InRange && prevMode == (uint8_t)UV_InRange)
        for (i = NUM_POWER_DOMAINS; i; i--)
            distDomainSwitch((PowerDomainID)(i - 1), PD_CMD_BattVLow);
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void distBcSendGeneral()
{
    int i;

    for (i=0; i < NUM_POWER_DOMAINS; i++)
    {
        gseg.powerdomainswitchstate[i] = (uint8_t)distQueryDomainSwitch((PowerDomainID)i);

    }

    // Other packet fields filled out in other locations

    bcbinSendPacket((uint8_t *) &gseg, sizeof(gseg));
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void distBcSendHealth()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // TODO:  Determine overall health based on querying various entities for their health
    // For now, everythingis always marginal ...
    hseg.oms = OMS_Unknown;
    hseg.inttemp = asensorReadIntTempC();
    hseg.reset_count = bspGetResetCount();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandlers();
}

// Packetizes and sends backchannel SENSORDAT packet
FILE_STATIC void distBcSendSensorDat()
{
    bcbinSendPacket((uint8_t *) &sseg, sizeof(sseg));
}

FILE_STATIC void distBcSendMeta()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

void distSetOCPThreshold(PowerDomainID domain, float newval)
{
    int i;

    // 0.0f means don't change current value
    if (newval == 0.0f)
        return;

    gseg.powerdomainocpthreshold[(uint8_t)domain] = newval;
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
    domaincmd_segment *dsegment;
    commoncmd_segment *csegment;
    ocpthresh_segment *osegment;
    firedeploy_segment *fsegment;

    int i;
    float newval;

    if (mode == Mode_BinaryStreaming)
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_DOMAINSWITCH:
                dsegment = (domaincmd_segment *) &cmdstr[1];
                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    distDomainSwitch((PowerDomainID)i, (PowerDomainCmd)(dsegment->pd_cmds[i]));
                }
                break;
            case OPCODE_COMMONCMD:
                csegment = (commoncmd_segment *) &cmdstr[1];
                LED_OUT ^= LED_BIT;
                break;
            case OPCODE_OCPTHRESH:
                osegment = (ocpthresh_segment *) &cmdstr[1];

                gseg.undervoltagethreshold = osegment->newBattVThreshold;

                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    distSetOCPThreshold((PowerDomainID)i, osegment->newCurrentThreshold[i]);
                }
                break;
            case OPCODE_FIREDEPLOY:
                fsegment = (firedeploy_segment *) &cmdstr[1];
                if (fsegment->key == DEPLOYMENT_SYSTEM_KEY)
                    distFireDeploy();
                break;
            default:
                break;
        }
    }
    return 1;
}

uint32_t constructPrimaryTime(timeStamp* currTime)
{
    uint32_t baseStuff = (uint32_t) currTime->count1;
    baseStuff |= ((uint32_t) currTime->count2) << 8;
    baseStuff |= ((uint32_t) currTime->count3) << 16;
    baseStuff |= ((uint32_t) currTime->count4) << 24;
    return baseStuff;
}

void sendSubsystemRollCall(uint8_t ssID)
{
    CANPacket rcPkt = {0};
    cmd_rollcall rc_info = {0};
//    timeStamp currTime = getTimeStamp();
//    rc_info.cmd_rollcall_met = constructPrimaryTime(&currTime);
//    rc_info.cmd_rollcall_met_overflow = currTime.count5;
//    rc_info.cmd_rollcall_msp = ssID;
    encodecmd_rollcall(&rc_info, &rcPkt);
    canSendPacket(&rcPkt);
}

void sendRollCall()
{
    rcFlag = 1;
    //TODO: but not 2,3,4 (those are RWs
}

/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    // Spin up the ADC, for the temp sensor and battery voltage
    asensorInit(Ref_2p5V);
    hBattV = asensorActivateChannel(CHAN_A0, Type_GeneralV);
    distDomainInit();
    distDeployInit();
    canWrapInit();

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&(gseg.header), TLM_ID_EPS_DIST_GENERAL, sizeof(gseg));
    bcbinPopulateHeader(&(sseg.header), TLM_ID_EPS_DIST_SENSORDAT, sizeof(sseg));

    //mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>

#if defined(__DEBUG__)

    // Register to handle telecommands
    debugRegisterEntity(Entity_SUBSYSTEM, NULL, NULL, distActionCallback);
    __delay_cycles(0.5 * SEC);

#else  //  __DEBUG__
    while(startupDelay)
    {
        __delay_cycles(SEC); //wait for 30 minutes
        startupDelay--;
    }
#endif

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers


    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    // Autostart the EPS power domain for now
    __delay_cycles(2 * SEC);
    distDomainSwitch(PD_EPS, PD_CMD_AutoStart);

    initializeTimer();
    startCallback(timerCallbackInitializer(&sendRollCall, 6000000));



    uint16_t counter = 0;
    while (1)
    {
        // TODO:  eventually drive this with a timer
        //LED_OUT ^= LED_BIT;
        __delay_cycles(0.1 * SEC);

        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
            case State_FirstState:
                LED_OUT ^= LED_BIT;

                distMonitorDomains();

                counter++;
                distBcSendSensorDat();
                if (counter % 8 == 0)
                {
                    distBcSendGeneral();
                    distBcSendHealth();
                    distMonitorBattery();
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
        if(rcFlag)
        {
            /*uint8_t ssID;
            for(ssID = 15; ssID - 1; ssID--)
            {
                sendSubsystemRollCall(ssID);
            }*/
            sendSubsystemRollCall(8);
            rcFlag = 0;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}


