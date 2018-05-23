#include <eps_dist.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "core/dataArray.h"
#include "interfaces/rollcall.h"

#define WDT_CONFIG WDTPW | WDTCNTCL | WDTTMSEL_0 | WDTSSEL_0 | WDTIS_2

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
FILE_STATIC general_segment gseg = {0};
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

FILE_STATIC rcCount_segment rcCount;

#pragma PERSISTENT(gseg)

FILE_STATIC hDev hBattV;

#define MAX_BUFF_SIZE   0x10
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

FILE_STATIC uint16_t startupDelay = 1800;
#pragma PERSISTENT(startupDelay)

FILE_STATIC uint8_t rcFlag = 0; //use this one for sending own rollcall
FILE_STATIC uint8_t rcSendFlag = 0;  //use this one for sending rcCmd

FILE_STATIC uint8_t subSystemsToToggle[16] = {0};
FILE_STATIC int rcTimerID = 0;


//**********Data Stuff**********************
FILE_STATIC uint16_t mspTempArray[480] = {0};
FILE_STATIC uint16_t battVArray[480] = {0};

#pragma PERSISTENT(mspTempArray)
#pragma PERSISTENT(battVArray)

FILE_STATIC uint16_t mspTemp;
FILE_STATIC uint16_t battV;

FILE_STATIC uint8_t rebootCount = 60;

#define PD_COM1_FLAG 1
#define PD_COM2_FLAG 2
#define PD_RAHS_FLAG 4

#define MOD_BDOT_FLAG 8
#define MOD_MTQ_FLAG 16
#define PD_BDOT_FLAG 24

#define MOD_ESTIM_FLAG 32
#define MOD_MPC_FLAG 64
#define MOD_SENSORPROC_FLAG 128
#define PD_ESTIM_FLAG 224

#define MOD_GEN_FLAG 256
#define MOD_BATT_FLAG 512
#define PD_EPS_FLAG 768

#define PD_PPT_FLAG 1024

FILE_STATIC uint16_t rcResponseFlag = 0; //this is zero when no responses are pending

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
    addData_uint16_t(battV, asensorReadSingleSensorRaw(hSensor)); //adds raw voltage to battV array
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
            if(i != PD_COM1)
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
                if(osegment->newBattVThreshold)
                    gseg.undervoltagethreshold = osegment->newBattVThreshold;

                for (i = 0; i < NUM_POWER_DOMAINS; i++)
                {
                    if(osegment->newCurrentThreshold[i])
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

void sendRollCallHandler()
{
    rcSendFlag = 1;
    rcCount.timeSinceRC = 0;
}

void sendRCCmd()
{
    distDomainSwitch(PD_WHEELS, PD_CMD_Enable);
    CANPacket rcPkt = {0};
    cmd_rollcall rc_info = {0};
    rc_info.cmd_rollcall_met = getMETPrimary();
    rc_info.cmd_rollcall_met_overflow = getMETOverflow();
    encodecmd_rollcall(&rc_info, &rcPkt);
    canSendPacket(&rcPkt);
    rcFlag = 17; //TODO: this should be the number of packets
    if(rebootCount)
        rebootCount--;
    else
    {
        //WDTCTL = 0; //reboot
    }

    //TODO: uncomment this when automatic shutoff is ready to go!
    /*if(rcResponseFlag)
    {
        if(rcResponseFlag & PD_COM1_FLAG)
        {
            distDomainSwitch(PD_COM1, PD_CMD_Disable);
            rcResponseFlag &= ~PD_COM1_FLAG;
        }
        if(rcResponseFlag & PD_COM2_FLAG)
        {
            distDomainSwitch(PD_COM2, PD_CMD_Disable);
            rcResponseFlag &= ~PD_COM2_FLAG;
        }
        if(rcResponseFlag & PD_RAHS_FLAG)
        {
            distDomainSwitch(PD_RAHS, PD_CMD_Disable);
            rcResponseFlag &= ~PD_RAHS_FLAG;
        }
        if(rcResponseFlag & PD_BDOT_FLAG)
        {
            distDomainSwitch(PD_BDOT, PD_CMD_Disable);
            rcResponseFlag &= ~PD_BDOT_FLAG;
        }
        if(rcResponseFlag & PD_ESTIM_FLAG)
        {
            distDomainSwitch(PD_ESTIM, PD_CMD_Disable);
            rcResponseFlag &= ~PD_ESTIM_FLAG;
        }
        if(rcResponseFlag & PD_EPS_FLAG)
        {
            distDomainSwitch(PD_EPS, PD_CMD_Disable);
            rcResponseFlag &= ~PD_EPS_FLAG;
        }
        if(rcResponseFlag & PD_PPT_FLAG)
        {
            distDomainSwitch(PD_PPT, PD_CMD_Disable);
            rcResponseFlag &= ~PD_PPT_FLAG;
        }
    }*/

    if(distQueryDomainSwitch(PD_COM1))
        rcResponseFlag |= PD_COM1_FLAG;
    if(distQueryDomainSwitch(PD_COM2))
        rcResponseFlag |= PD_COM2_FLAG;
    if(distQueryDomainSwitch(PD_RAHS))
        rcResponseFlag |= PD_RAHS_FLAG;
    if(distQueryDomainSwitch(PD_BDOT))
        rcResponseFlag |= PD_BDOT_FLAG;
    if(distQueryDomainSwitch(PD_ESTIM))
        rcResponseFlag |= PD_ESTIM_FLAG;
    if(distQueryDomainSwitch(PD_EPS))
        rcResponseFlag |= PD_EPS_FLAG;
    if(distQueryDomainSwitch(PD_PPT))
        rcResponseFlag |= PD_PPT_FLAG;
    rcSendFlag = 0;
    distDomainSwitch(PD_WHEELS, PD_CMD_Disable);
}

void sendRC()
{
    while(rcFlag && (canTxCheck() != CAN_TX_BUSY))
    {
        CANPacket rollcallPkt = {0};
        if(rcFlag == 17)
        {
            rc_eps_dist_1 rollcallPkt1_info = {0};
            rollcallPkt1_info.rc_eps_dist_1_reset_count = bspGetResetCount();
            rollcallPkt1_info.rc_eps_dist_1_sysrstiv = SYSRSTIV;
            rollcallPkt1_info.rc_eps_dist_1_temp_avg = 0;
            encoderc_eps_dist_1(&rollcallPkt1_info, &rollcallPkt);
        }
        else if(rcFlag == 16)
        {
            rc_eps_dist_2 rollcallPkt2_info = {0};
            rollcallPkt2_info.rc_eps_dist_2_met = getMETPrimary();
            rollcallPkt2_info.rc_eps_dist_2_met_overflow = getMETOverflow();
            rollcallPkt2_info.rc_eps_dist_2_uv_state = gseg.uvmode;
            encoderc_eps_dist_2(&rollcallPkt2_info, &rollcallPkt);
        }
        else if(rcFlag == 15)
        {
            rc_eps_dist_3 rollcallPkt3_info = {0};
            rollcallPkt3_info.rc_eps_dist_3_batt_v_avg = 0; //TODO: ags
            rollcallPkt3_info.rc_eps_dist_3_batt_v_max = 0;
            rollcallPkt3_info.rc_eps_dist_3_batt_v_min = 0;
            encoderc_eps_dist_3(&rollcallPkt3_info, &rollcallPkt);
        }
        else if(rcFlag == 14)
        {
            rc_eps_dist_4 rollcallPkt4_info = {0};
            rollcallPkt4_info.rc_eps_dist_4_com1_c_avg = 0; //TODO: ags
            rollcallPkt4_info.rc_eps_dist_4_com1_c_max = 0;
            rollcallPkt4_info.rc_eps_dist_4_com1_c_min = 0;
            rollcallPkt4_info.rc_eps_dist_4_com1_state = distQueryDomainSwitch(PD_COM1);
            encoderc_eps_dist_4(&rollcallPkt4_info, &rollcallPkt);
        }
        else if(rcFlag == 13)
        {
            rc_eps_dist_5 rollcallPkt5_info = {0};
            rollcallPkt5_info.rc_eps_dist_5_com1_v_avg = 0; //TODO: ags
            rollcallPkt5_info.rc_eps_dist_5_com1_v_max = 0;
            rollcallPkt5_info.rc_eps_dist_5_com1_v_min = 0;
            encoderc_eps_dist_5(&rollcallPkt5_info, &rollcallPkt);
        }
        else if(rcFlag == 12)
        {
            rc_eps_dist_6 rollcallPkt6_info = {0};
            rollcallPkt6_info.rc_eps_dist_6_com2_c_avg = 0; //TODO: ags
            rollcallPkt6_info.rc_eps_dist_6_com2_c_max = 0;
            rollcallPkt6_info.rc_eps_dist_6_com2_c_min = 0;
            rollcallPkt6_info.rc_eps_dist_6_com2_state = distQueryDomainSwitch(PD_COM2);
            encoderc_eps_dist_6(&rollcallPkt6_info, &rollcallPkt);
        }
        else if(rcFlag == 11)
        {
            rc_eps_dist_7 rollcallPkt7_info = {0};
            rollcallPkt7_info.rc_eps_dist_7_com2_v_avg = 0; //TODO: ags
            rollcallPkt7_info.rc_eps_dist_7_com2_v_max = 0;
            rollcallPkt7_info.rc_eps_dist_7_com2_v_min = 0;
            encoderc_eps_dist_7(&rollcallPkt7_info, &rollcallPkt);
        }
        else if(rcFlag == 10)
        {
            rc_eps_dist_8 rollcallPkt8_info = {0};
            rollcallPkt8_info.rc_eps_dist_8_rahs_c_avg = 0; //TODO: ags
            rollcallPkt8_info.rc_eps_dist_8_rahs_c_max = 0;
            rollcallPkt8_info.rc_eps_dist_8_rahs_c_min = 0;
            rollcallPkt8_info.rc_eps_dist_8_rahs_state = distQueryDomainSwitch(PD_RAHS);
            encoderc_eps_dist_8(&rollcallPkt8_info, &rollcallPkt);
        }
        else if(rcFlag == 9)
        {
            rc_eps_dist_9 rollcallPkt9_info = {0};
            rollcallPkt9_info.rc_eps_dist_9_rahs_v_avg = 0; //TODO: ags
            rollcallPkt9_info.rc_eps_dist_9_rahs_v_max = 0;
            rollcallPkt9_info.rc_eps_dist_9_rahs_v_min = 0;
            encoderc_eps_dist_9(&rollcallPkt9_info, &rollcallPkt);
        }
        else if(rcFlag == 8)
        {
            rc_eps_dist_10 rollcallPkt10_info = {0};
            rollcallPkt10_info.rc_eps_dist_10_bdot_c_avg = 0; //TODO: ags
            rollcallPkt10_info.rc_eps_dist_10_bdot_c_max = 0;
            rollcallPkt10_info.rc_eps_dist_10_bdot_c_min = 0;
            rollcallPkt10_info.rc_eps_dist_10_bdot_state = distQueryDomainSwitch(PD_BDOT);
            encoderc_eps_dist_10(&rollcallPkt10_info, &rollcallPkt);
        }
        else if(rcFlag == 7)
        {
            rc_eps_dist_11 rollcallPkt11_info = {0};
            rollcallPkt11_info.rc_eps_dist_11_bdot_v_avg = 0; //TODO: ags
            rollcallPkt11_info.rc_eps_dist_11_bdot_v_max = 0;
            rollcallPkt11_info.rc_eps_dist_11_bdot_v_min = 0;
            encoderc_eps_dist_11(&rollcallPkt11_info, &rollcallPkt);
        }
        else if(rcFlag == 6)
        {
            rc_eps_dist_12 rollcallPkt12_info = {0};
            rollcallPkt12_info.rc_eps_dist_12_estim_c_avg = 0; //TODO: ags
            rollcallPkt12_info.rc_eps_dist_12_estim_c_max = 0;
            rollcallPkt12_info.rc_eps_dist_12_estim_c_min = 0;
            rollcallPkt12_info.rc_eps_dist_12_estim_state = distQueryDomainSwitch(PD_ESTIM);
            encoderc_eps_dist_12(&rollcallPkt12_info, &rollcallPkt);
        }
        else if(rcFlag == 5)
        {
            rc_eps_dist_13 rollcallPkt13_info = {0};
            rollcallPkt13_info.rc_eps_dist_13_estim_v_avg = 0; //TODO: ags
            rollcallPkt13_info.rc_eps_dist_13_estim_v_max = 0;
            rollcallPkt13_info.rc_eps_dist_13_estim_v_min = 0;
            encoderc_eps_dist_13(&rollcallPkt13_info, &rollcallPkt);
        }
        else if(rcFlag == 4)
        {
            rc_eps_dist_14 rollcallPkt14_info = {0};
            rollcallPkt14_info.rc_eps_dist_14_eps_c_avg = 0; //TODO: ags
            rollcallPkt14_info.rc_eps_dist_14_eps_c_max = 0;
            rollcallPkt14_info.rc_eps_dist_14_eps_c_min = 0;
            rollcallPkt14_info.rc_eps_dist_14_eps_state = distQueryDomainSwitch(PD_EPS);
            encoderc_eps_dist_14(&rollcallPkt14_info, &rollcallPkt);
        }
        else if(rcFlag == 3)
        {
            rc_eps_dist_15 rollcallPkt15_info = {0};
            rollcallPkt15_info.rc_eps_dist_15_eps_v_avg = 0; //TODO: ags
            rollcallPkt15_info.rc_eps_dist_15_eps_v_max = 0;
            rollcallPkt15_info.rc_eps_dist_15_eps_v_min = 0;
            encoderc_eps_dist_15(&rollcallPkt15_info, &rollcallPkt);
        }
        else if(rcFlag == 2)
        {
            rc_eps_dist_16 rollcallPkt16_info = {0};
            rollcallPkt16_info.rc_eps_dist_16_ppt_c_avg = 0; //TODO: ags
            rollcallPkt16_info.rc_eps_dist_16_ppt_c_max = 0;
            rollcallPkt16_info.rc_eps_dist_16_ppt_c_min = 0;
            rollcallPkt16_info.rc_eps_dist_16_ppt_state = distQueryDomainSwitch(PD_PPT);
            encoderc_eps_dist_16(&rollcallPkt16_info, &rollcallPkt);
        }
        else if(rcFlag == 1)
        {
            rc_eps_dist_17 rollcallPkt17_info = {0};
            rollcallPkt17_info.rc_eps_dist_17_ppt_v_avg = 0; //TODO: ags
            rollcallPkt17_info.rc_eps_dist_17_ppt_v_max = 0;
            rollcallPkt17_info.rc_eps_dist_17_ppt_v_min = 0;
            encoderc_eps_dist_17(&rollcallPkt17_info, &rollcallPkt);
        }
        canSendPacket(&rollcallPkt);
        rcFlag--;
    }
}

void can_packet_rx_callback(CANPacket *packet)
{
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            break;
        /*case CAN_ID_CMD_GRNDROLLCALL:
            //start the same process for usual rollcall (shutoff)
            break;*/
        case CAN_ID_RC_ADCS_BDOT_1:
            rcResponseFlag &= ~MOD_BDOT_FLAG;
            break;
        case CAN_ID_RC_ADCS_MTQ_1:
            rcResponseFlag &= ~MOD_MTQ_FLAG;
            break;
        case CAN_ID_RC_ADCS_SP_1:
            rcResponseFlag &= ~MOD_SENSORPROC_FLAG;
            break;
        case CAN_ID_RC_EPS_BATT_1:
            rcResponseFlag &= ~MOD_BATT_FLAG;
            break;
        case CAN_ID_RC_EPS_GEN_1:
            rcResponseFlag &= ~MOD_GEN_FLAG;
            break;
        case CAN_ID_RC_PPT_1:
            rcResponseFlag &= ~PD_PPT_FLAG;
        default:
            break;
    }
}

void autoStart()
{
    __delay_cycles(2 * SEC);
    distDomainSwitch(PD_EPS, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_BDOT, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_COM1, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_COM2, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_ESTIM, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_RAHS, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    //distDomainSwitch(PD_WHEELS, PD_CMD_AutoStart);
    __delay_cycles(0.5 * SEC);
    distDomainSwitch(PD_PPT, PD_CMD_AutoStart);
}

void initData()
{
    mspTemp = init_uint16_t(mspTempArray, 480);
    battV = init_uint16_t(battVArray, 480);
}

void intermediateRollcall()
{
    bcbinSendPacket((uint8_t *) &rcCount, sizeof(rcCount));
    rcCount.timeSinceRC++;
}

/*
 * main.c
 */
int main(void)
{
    P3DIR |= BIT4;
    P3OUT |= BIT4;
    /* ----- INITIALIZATION -----*/
    WDTCTL = WDTPW | WDTCNTCL | WDTTMSEL_0 | WDTSSEL_0 | WDTIS_1;
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the family of __SS_etc predefined symbols - see bsp.h

    // Spin up the ADC, for the temp sensor and battery voltage
    asensorInit(Ref_2p5V);
    hBattV = asensorActivateChannel(CHAN_A0, Type_GeneralV);
    distDomainInit();
    distDeployInit();
    canWrapInit();
    setCANPacketRxCallback(can_packet_rx_callback);

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
    autoStart();

    initData();

    initializeTimer();
    startCallback(timerCallbackInitializer(&sendRollCallHandler, 6000000));
    //TODO: this is test code:
    bcbinPopulateHeader(&(rcCount.header), 25, sizeof(rcCount));
    startCallback(timerCallbackInitializer(&intermediateRollcall, 1000000));

    uint16_t counter = 0;
    while (1)
    {
        WDTCTL = WDT_CONFIG;
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
        if(rcSendFlag && (canTxCheck() != CAN_TX_BUSY))
            sendRCCmd();
        //sendRC();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}


