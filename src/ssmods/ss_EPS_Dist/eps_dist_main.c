#include <eps_dist.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "core/timer.h"
#include "core/MET.h"
#include "interfaces/canwrap.h"
#include "interfaces/rollcall.h"
#include "core/agglib.h"
#include "core/autosequence.h"
#include "core/utils.h"

#define WDT_CONFIG WDTPW | WDTCNTCL | WDTTMSEL_0 | WDTSSEL_0 | WDTIS_2
#define ROLLCALL_WATCHDOG_TIMEOUT 255 //seconds
#define ROLLCALL_WATCHDOG_AUTOSHUTOFF 10 //seconds
#define AUTOSHUTOFF_DELAY 1 //number of rollcalls before the check for shutoff starts

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulateH1, rcPopulateH2, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5, rcPopulate6, rcPopulate7, rcPopulate8, rcPopulate9, rcPopulate10, rcPopulate11, rcPopulate12, rcPopulate13, rcPopulate14, rcPopulate15, rcPopulate16, rcPopulate17, rcPopulate18
};

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

#pragma PERSISTENT(shutoffEnabled)
FILE_STATIC AutoshutoffEnabled shutoffEnabled = {1, 1, 1, 1, 0, 0};
FILE_STATIC AutoshutoffDelay shutoffDelay = {0, 0, 0, 0, 0, 0};

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

FILE_STATIC float domainCurrentThresholdInitial[] = { OCP_THRESH_LOW_DRAW_DEVICE, //COM1
                                                      3.0f, //COM2
                                                      OCP_THRESH_LOW_DRAW_DEVICE, //RAHS
                                                      OCP_THRESH_VERY_HIGH_DRAW_DEVICE, //BDOT
                                                      OCP_THRESH_LOW_DRAW_DEVICE, //ESTIM
                                                      OCP_THRESH_LOW_DRAW_DEVICE, //WHEELS
                                                      OCP_THRESH_LOW_DRAW_DEVICE, //EPS
                                                      OCP_THRESH_HIGH_DRAW_DEVICE }; //PPT

PCVSensorData *sensorData;
hDev i2cdev, hSensor;

// Segment instances - used both to store information and as a structure for sending as telemetry/commands
FILE_STATIC meta_segment mseg;
FILE_STATIC general_segment gseg = {0};
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

FILE_STATIC hDev hBattV;

#define MAX_BUFF_SIZE   0x10
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];

FILE_STATIC uint16_t startupDelay = 1800;
#pragma PERSISTENT(startupDelay)

//**********Data Stuff**********************
FILE_STATIC uint8_t rebootCount = 60;
FILE_STATIC uint8_t rollcallWatchdog = 0;

FILE_STATIC aggVec_f mspTempAg;
FILE_STATIC aggVec_i battVAg;
FILE_STATIC aggVec_i coulombCounterAg;
FILE_STATIC aggVec_i ssCurrAgs[NUM_POWER_DOMAINS];
FILE_STATIC aggVec_i ssBusVAgs[NUM_POWER_DOMAINS];

#define PD_COM1_FLAG 1
#define PD_COM2_FLAG 2
#define PD_RAHS_FLAG 4

#define MOD_BDOT_FLAG 8
#define MOD_MTQ_FLAG 16
#define PD_BDOT_FLAG (MOD_BDOT_FLAG | MOD_MTQ_FLAG)

#define MOD_ESTIM_FLAG 32
#define MOD_MPC_FLAG 64
#define MOD_SENSORPROC_FLAG 128
#define PD_ESTIM_FLAG (MOD_ESTIM_FLAG | MOD_MPC_FLAG | MOD_SENSORPROC_FLAG)

#define MOD_GEN_FLAG 256
#define MOD_BATT_FLAG 512
#define PD_EPS_FLAG (MOD_GEN_FLAG | MOD_BATT_FLAG)

#define PD_PPT_FLAG 1024

#pragma PERSISTENT(persistentTime)
timeStamp persistentTime = {0};
#pragma PERSISTENT(persistentEvents)
sequenceEvent persistentEvents[100] = {0};
#pragma PERSISTENT(eventsInitialized)
uint8_t eventsInitialized = 0;
#pragma PERSISTENT(autoSequencerEnabled)
uint8_t autoSequencerEnabled = 0;

FILE_STATIC uint16_t rcResponseFlag = 0; //this is zero when no responses are pending
sequenceEvent pendingEvent = {0};

CANPacket autoseqMETResponsePkt = {0};
uint8_t autoseqMETResponsePktSendFlag = 0;
CANPacket autoseqIndicesResponsePkt = {0};
uint8_t autoseqIndicesResponsePktSendFlag = 0;

uint16_t compressOCPThresh(float thresh) {
    return (uint16_t)(thresh * 20);
}

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

    uint8_t i;
    for(i = NUM_POWER_DOMAINS; i; i--)
        gseg.powerdomainlastcmds[i - 1] = PD_CMD_OffInitial;
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
            if(distQueryDomainSwitch(PD_COM2) == 0)
                shutoffDelay.com2 = AUTOSHUTOFF_DELAY;
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_COM2_OUT |= DOMAIN_ENABLE_COM2_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_COM2_OUT &= ~DOMAIN_ENABLE_COM2_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_COM2_OUT ^= DOMAIN_ENABLE_COM2_BIT;
            break;
        case PD_PPT:
            if(distQueryDomainSwitch(PD_PPT) == 0)
                shutoffDelay.ppt = AUTOSHUTOFF_DELAY;
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_PPT_OUT |= DOMAIN_ENABLE_PPT_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_PPT_OUT &= ~DOMAIN_ENABLE_PPT_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_PPT_OUT ^= DOMAIN_ENABLE_PPT_BIT;
            break;
        case PD_BDOT:
            if(distQueryDomainSwitch(PD_BDOT) == 0)
                shutoffDelay.bdot = AUTOSHUTOFF_DELAY;
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
            if(distQueryDomainSwitch(PD_RAHS) == 0)
                shutoffDelay.rahs = AUTOSHUTOFF_DELAY;
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_RAHS_OUT |= DOMAIN_ENABLE_RAHS_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_RAHS_OUT &= ~DOMAIN_ENABLE_RAHS_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_RAHS_OUT ^= DOMAIN_ENABLE_RAHS_BIT;
            break;
        case PD_ESTIM:
            if(distQueryDomainSwitch(PD_ESTIM) == 0)
                shutoffDelay.estim = AUTOSHUTOFF_DELAY;
            if (cmd == PD_CMD_Enable)
                DOMAIN_ENABLE_ESTIM_OUT |= DOMAIN_ENABLE_ESTIM_BIT;
            else if (cmd == PD_CMD_Disable)
                DOMAIN_ENABLE_ESTIM_OUT &= ~DOMAIN_ENABLE_ESTIM_BIT;
            else if (cmd == PD_CMD_Toggle)
                DOMAIN_ENABLE_ESTIM_OUT ^= DOMAIN_ENABLE_ESTIM_BIT;
            break;
        case PD_EPS:
            if(distQueryDomainSwitch(PD_EPS) == 0)
                shutoffDelay.eps = AUTOSHUTOFF_DELAY;
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
        aggVec_push_i(&ssCurrAgs[i], pdata->rawCurrent);
        aggVec_push_i(&ssBusVAgs[i], pdata->rawBusVoltage);

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
    aggVec_push_i(&battVAg, asensorReadSingleSensorRaw(hBattV)); //adds raw voltage to battV array
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
    if (gseg.uvmode != (uint8_t)UV_InRange)
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
    aggVec_push_f(&mspTempAg, hseg.inttemp);
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

uint8_t getPDState(PowerDomainID pd)
{
    if(distQueryDomainSwitch(pd))
        return 0; //on
    else if(gseg.powerdomainlastcmds[(uint8_t)pd] == PD_CMD_Disable || gseg.powerdomainlastcmds[(uint8_t)pd] == PD_CMD_Toggle)
        return 1; //off manual
    else if(gseg.powerdomainlastcmds[(uint8_t)pd] == PD_CMD_OCLatch)
        return 2; //overcurrent latch
    else if(gseg.powerdomainlastcmds[(uint8_t)pd] == PD_CMD_BattVLow)
        return 3; //batt_undervoltage
    else if(gseg.powerdomainlastcmds[(uint8_t)pd] == PD_CMD_OffInitial)
        return 4;
    else
        return 5; //other
}

void rcPopulateH1(CANPacket *out)
{
    rc_eps_dist_h1 rc = {0};
    rc.rc_eps_dist_h1_reset_count = bspGetResetCount();
    rc.rc_eps_dist_h1_sysrstiv = SYSRSTIV;
    rc.rc_eps_dist_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
    rc.rc_eps_dist_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
    rc.rc_eps_dist_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
    encoderc_eps_dist_h1(&rc, out);
    aggVec_as_reset((aggVec *)&mspTempAg);
}

void rcPopulateH2(CANPacket *out)
{
    rc_eps_dist_h2 rc = {0};
    rc.rc_eps_dist_h2_canrxerror = canRxErrorCheck();
    encoderc_eps_dist_h2(&rc, out);
}

void rcPopulate2(CANPacket *out)
{
    rc_eps_dist_2 rc = {0};
    rc.rc_eps_dist_2_met = getMETPrimary();
    rc.rc_eps_dist_2_met_overflow = getMETOverflow();
    rc.rc_eps_dist_2_uv_state = gseg.uvmode;
    encoderc_eps_dist_2(&rc, out);
}

void rcPopulate3(CANPacket *out)
{
    rc_eps_dist_3 rc = {0};
    rc.rc_eps_dist_3_batt_v_avg = aggVec_avg_i_i(&battVAg);
    rc.rc_eps_dist_3_batt_v_max = aggVec_max_i(&battVAg);
    rc.rc_eps_dist_3_batt_v_min = aggVec_min_i(&battVAg);
    encoderc_eps_dist_3(&rc, out);
    aggVec_as_reset((aggVec *)&battVAg);
}

void rcPopulate4(CANPacket *out)
{
    rc_eps_dist_4 rc = {0};
    rc.rc_eps_dist_4_com1_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_COM1]);
    rc.rc_eps_dist_4_com1_c_max = aggVec_max_i(&ssCurrAgs[PD_COM1]);
    rc.rc_eps_dist_4_com1_c_min = aggVec_min_i(&ssCurrAgs[PD_COM1]);
    rc.rc_eps_dist_4_com1_state = getPDState(PD_COM1);
    encoderc_eps_dist_4(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_COM1]);
}

void rcPopulate5(CANPacket *out)
{
    rc_eps_dist_5 rc = {0};
    rc.rc_eps_dist_5_com1_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_COM1]);
    rc.rc_eps_dist_5_com1_v_max = aggVec_max_i(&ssBusVAgs[PD_COM1]);
    rc.rc_eps_dist_5_com1_v_min = aggVec_min_i(&ssBusVAgs[PD_COM1]);
    encoderc_eps_dist_5(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_COM1]);
}

void rcPopulate6(CANPacket *out)
{
    rc_eps_dist_6 rc = {0};
    rc.rc_eps_dist_6_com2_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_COM2]);
    rc.rc_eps_dist_6_com2_c_max = aggVec_max_i(&ssCurrAgs[PD_COM2]);
    rc.rc_eps_dist_6_com2_c_min = aggVec_min_i(&ssCurrAgs[PD_COM2]);
    rc.rc_eps_dist_6_com2_state = getPDState(PD_COM2);
    encoderc_eps_dist_6(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_COM2]);
}

void rcPopulate7(CANPacket *out)
{
    rc_eps_dist_7 rc = {0};
    rc.rc_eps_dist_7_com2_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_COM2]);
    rc.rc_eps_dist_7_com2_v_max = aggVec_max_i(&ssBusVAgs[PD_COM2]);
    rc.rc_eps_dist_7_com2_v_min = aggVec_min_i(&ssBusVAgs[PD_COM2]);
    encoderc_eps_dist_7(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_COM2]);
}

void rcPopulate8(CANPacket *out)
{
    rc_eps_dist_8 rc = {0};
    rc.rc_eps_dist_8_rahs_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_RAHS]);
    rc.rc_eps_dist_8_rahs_c_max = aggVec_max_i(&ssCurrAgs[PD_RAHS]);
    rc.rc_eps_dist_8_rahs_c_min = aggVec_min_i(&ssCurrAgs[PD_RAHS]);
    rc.rc_eps_dist_8_rahs_state = getPDState(PD_RAHS);
    encoderc_eps_dist_8(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_RAHS]);
}

void rcPopulate9(CANPacket *out)
{
    rc_eps_dist_9 rc = {0};
    rc.rc_eps_dist_9_rahs_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_RAHS]);
    rc.rc_eps_dist_9_rahs_v_max = aggVec_max_i(&ssBusVAgs[PD_RAHS]);
    rc.rc_eps_dist_9_rahs_v_min = aggVec_min_i(&ssBusVAgs[PD_RAHS]);
    encoderc_eps_dist_9(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_RAHS]);
}

void rcPopulate10(CANPacket *out)
{
    rc_eps_dist_10 rc = {0};
    rc.rc_eps_dist_10_bdot_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_BDOT]);
    rc.rc_eps_dist_10_bdot_c_max = aggVec_max_i(&ssCurrAgs[PD_BDOT]);
    rc.rc_eps_dist_10_bdot_c_min = aggVec_min_i(&ssCurrAgs[PD_BDOT]);
    rc.rc_eps_dist_10_bdot_state = getPDState(PD_BDOT);
    encoderc_eps_dist_10(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_BDOT]);
}

void rcPopulate11(CANPacket *out)
{
    rc_eps_dist_11 rc = {0};
    rc.rc_eps_dist_11_bdot_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_BDOT]);
    rc.rc_eps_dist_11_bdot_v_max = aggVec_max_i(&ssBusVAgs[PD_BDOT]);
    rc.rc_eps_dist_11_bdot_v_min = aggVec_min_i(&ssBusVAgs[PD_BDOT]);
    encoderc_eps_dist_11(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_BDOT]);
}

void rcPopulate12(CANPacket *out)
{
    rc_eps_dist_12 rc = {0};
    rc.rc_eps_dist_12_estim_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_ESTIM]);
    rc.rc_eps_dist_12_estim_c_max = aggVec_max_i(&ssCurrAgs[PD_ESTIM]);
    rc.rc_eps_dist_12_estim_c_min = aggVec_min_i(&ssCurrAgs[PD_ESTIM]);
    rc.rc_eps_dist_12_estim_state = getPDState(PD_ESTIM);
    encoderc_eps_dist_12(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_ESTIM]);
}

void rcPopulate13(CANPacket *out)
{
    rc_eps_dist_13 rc = {0};
    rc.rc_eps_dist_13_estim_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_ESTIM]);
    rc.rc_eps_dist_13_estim_v_max = aggVec_max_i(&ssBusVAgs[PD_ESTIM]);
    rc.rc_eps_dist_13_estim_v_min = aggVec_min_i(&ssBusVAgs[PD_ESTIM]);
    encoderc_eps_dist_13(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_ESTIM]);
}

void rcPopulate14(CANPacket *out)
{
    rc_eps_dist_14 rc = {0};
    rc.rc_eps_dist_14_eps_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_EPS]);
    rc.rc_eps_dist_14_eps_c_max = aggVec_max_i(&ssCurrAgs[PD_EPS]);
    rc.rc_eps_dist_14_eps_c_min = aggVec_min_i(&ssCurrAgs[PD_EPS]);
    rc.rc_eps_dist_14_eps_state = getPDState(PD_EPS);
    encoderc_eps_dist_14(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_EPS]);
}

void rcPopulate15(CANPacket *out)
{
    rc_eps_dist_15 rc = {0};
    rc.rc_eps_dist_15_eps_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_EPS]);
    rc.rc_eps_dist_15_eps_v_max = aggVec_max_i(&ssBusVAgs[PD_EPS]);
    rc.rc_eps_dist_15_eps_v_min = aggVec_min_i(&ssBusVAgs[PD_EPS]);
    encoderc_eps_dist_15(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_EPS]);
}

void rcPopulate16(CANPacket *out)
{
    rc_eps_dist_16 rc = {0};
    rc.rc_eps_dist_16_ppt_c_avg = aggVec_avg_i_i(&ssCurrAgs[PD_PPT]);
    rc.rc_eps_dist_16_ppt_c_max = aggVec_max_i(&ssCurrAgs[PD_PPT]);
    rc.rc_eps_dist_16_ppt_c_min = aggVec_min_i(&ssCurrAgs[PD_PPT]);
    rc.rc_eps_dist_16_ppt_state = getPDState(PD_PPT);
    encoderc_eps_dist_16(&rc, out);
    aggVec_as_reset((aggVec *)&ssCurrAgs[PD_PPT]);
}

void rcPopulate17(CANPacket *out)
{
    rc_eps_dist_17 rc = {0};
    rc.rc_eps_dist_17_ppt_v_avg = aggVec_avg_i_i(&ssBusVAgs[PD_PPT]);
    rc.rc_eps_dist_17_ppt_v_max = aggVec_max_i(&ssBusVAgs[PD_PPT]);
    rc.rc_eps_dist_17_ppt_v_min = aggVec_min_i(&ssBusVAgs[PD_PPT]);
    encoderc_eps_dist_17(&rc, out);
    aggVec_as_reset((aggVec *)&ssBusVAgs[PD_PPT]);
}

void rcPopulate18(CANPacket *out)
{
    rc_eps_dist_18 rc = {0};
    rc.rc_eps_dist_18_bdot_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_BDOT]);
    rc.rc_eps_dist_18_com2_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_COM2]);
    rc.rc_eps_dist_18_eps_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_EPS]);
    rc.rc_eps_dist_18_estim_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_ESTIM]);
    rc.rc_eps_dist_18_ppt_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_PPT]);
    rc.rc_eps_dist_18_rahs_ocp_thresh = compressOCPThresh(gseg.powerdomainocpthreshold[PD_RAHS]);
    encoderc_eps_dist_18(&rc, out);
}

void setPowerSwitchFromCAN(uint8_t cmd, PowerDomainID pd)
{
    if(cmd && gseg.uvmode != (uint8_t)UV_FullShutdown) //0 is nochange
    {
        if(cmd == 1)
            distDomainSwitch(pd, PD_CMD_Enable);
        else if(cmd == 2)
            distDomainSwitch(pd, PD_CMD_Disable);
        else if(cmd == 3)
            distDomainSwitch(pd, PD_CMD_Toggle);
    }
}

void autoShutoff()
{
    if(rcResponseFlag)
    {
        /*if(rcResponseFlag & PD_COM1_FLAG)
        {
            distDomainSwitch(PD_COM1, PD_CMD_Disable);
            rcResponseFlag &= ~PD_COM1_FLAG;
        }*/
        if(shutoffDelay.com2)
            shutoffDelay.com2--;
        else
            if(shutoffEnabled.com2 == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_COM2_FLAG && shutoffDelay.com2 == 0)
            {
                distDomainSwitch(PD_COM2, PD_CMD_Disable);
                rcResponseFlag &= ~PD_COM2_FLAG;
            }

        if(shutoffDelay.rahs)
            shutoffDelay.rahs--;
        else
            if(shutoffEnabled.rahs == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_RAHS_FLAG)
            {
                distDomainSwitch(PD_RAHS, PD_CMD_Disable);
                rcResponseFlag &= ~PD_RAHS_FLAG;
            }

        if(shutoffDelay.bdot)
            shutoffDelay.bdot--;
        else
            if(shutoffEnabled.bdot == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_BDOT_FLAG)
            {
                distDomainSwitch(PD_BDOT, PD_CMD_Disable);
                rcResponseFlag &= ~PD_BDOT_FLAG;
            }

        if(shutoffDelay.estim)
            shutoffDelay.estim--;
        else
            if(shutoffEnabled.estim == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_ESTIM_FLAG)
            {
                distDomainSwitch(PD_ESTIM, PD_CMD_Disable);
                rcResponseFlag &= ~PD_ESTIM_FLAG;
            }

        if(shutoffDelay.eps)
            shutoffDelay.eps--;
        else
            if(shutoffEnabled.eps == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_EPS_FLAG)
            {
                distDomainSwitch(PD_EPS, PD_CMD_Disable);
                rcResponseFlag &= ~PD_EPS_FLAG;
            }

        if(shutoffDelay.ppt)
            shutoffDelay.ppt--;
        else
            if(shutoffEnabled.ppt == CAN_ENUM_NBOOL_TRUE && rcResponseFlag & PD_PPT_FLAG)
            {
                distDomainSwitch(PD_PPT, PD_CMD_Disable);
                rcResponseFlag &= ~PD_PPT_FLAG;
            }
    }
}

void autoShutoffSetFlags()
{
    if(distQueryDomainSwitch(PD_COM1))
        rcResponseFlag |= PD_COM1_FLAG;
    else
        rcResponseFlag &= ~PD_COM1_FLAG;

    if(distQueryDomainSwitch(PD_COM2))
        rcResponseFlag |= PD_COM2_FLAG;
    else
        rcResponseFlag &= ~PD_COM2_FLAG;

    if(distQueryDomainSwitch(PD_RAHS))
        rcResponseFlag |= PD_RAHS_FLAG;
    else
        rcResponseFlag &= ~PD_RAHS_FLAG;

    if(distQueryDomainSwitch(PD_BDOT))
        rcResponseFlag |= PD_BDOT_FLAG;
    else
        rcResponseFlag &= ~PD_BDOT_FLAG;

    if(distQueryDomainSwitch(PD_ESTIM))
        rcResponseFlag |= PD_ESTIM_FLAG;
    else
        rcResponseFlag &= ~PD_ESTIM_FLAG;

    if(distQueryDomainSwitch(PD_EPS))
        rcResponseFlag |= PD_EPS_FLAG;
    else
        rcResponseFlag &= ~PD_EPS_FLAG;

    if(distQueryDomainSwitch(PD_PPT))
        rcResponseFlag |= PD_PPT_FLAG;
    else
        rcResponseFlag &= ~PD_PPT_FLAG;
}

void checkSelfReboot()
{
    if(rebootCount)
        rebootCount--;
    else
        WDTCTL = 0; //reboot
}

void can_packet_rx_callback(CANPacket *packet)
{
    gcmd_dist_set_pd_state pdCmd;
    gcmd_dist_set_pd_ovc_bdot ovcPktBDot;
    gcmd_dist_set_pd_ovc_com1 ovcPktCom1;
    gcmd_dist_set_pd_ovc_com2 ovcPktCom2;
    gcmd_dist_set_pd_ovc_eps ovcPktEPS;
    gcmd_dist_set_pd_ovc_estim ovcPktEstim;
    gcmd_dist_set_pd_ovc_ppt ovcPktPPT;
    gcmd_dist_set_pd_ovc_rahs ovcPktRAHS;
    cmd_reboot_request rebootRequest;
    gcmd_autoseq_add_1 autoseqAdd1;
    gcmd_autoseq_add_2 autoseqAdd2;
    gcmd_autoseq_remove_can_id rmCanId;
    gcmd_autoseq_rm_at_index rmIndex;
    gcmd_autoseq_get_met getMet;
    gcmd_autoseq_get_indices getIndices;
    eps_dist_autoseq_get_met_rsp getMetRsp;
    eps_dist_autoseq_get_ind_rsp getIndicesRsp;
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
            autoShutoffSetFlags();
            rollcallWatchdog = 0;
            break;
        case CAN_ID_RC_ADCS_BDOT_H1:
            rcResponseFlag &= ~MOD_BDOT_FLAG;
            break;
        case CAN_ID_RC_ADCS_MTQ_H1:
            rcResponseFlag &= ~MOD_MTQ_FLAG;
            break;
        case CAN_ID_RC_ADCS_SP_H1:
            rcResponseFlag &= ~MOD_SENSORPROC_FLAG;
            break;
        case CAN_ID_RC_EPS_BATT_H1:
            rcResponseFlag &= ~MOD_BATT_FLAG;
            break;
        case CAN_ID_RC_EPS_GEN_H1:
            rcResponseFlag &= ~MOD_GEN_FLAG;
            break;
        case CAN_ID_RC_PPT_H1:
            rcResponseFlag &= ~PD_PPT_FLAG;
            break;
        case CAN_ID_RC_ADCS_MPC_2:
            rcResponseFlag &= ~MOD_MPC_FLAG;
            break;
        case CAN_ID_RC_ADCS_ESTIM_H1:
            rcResponseFlag &= ~MOD_ESTIM_FLAG;
            break;
        case CAN_ID_GCMD_DIST_SET_PD_STATE:
            decodegcmd_dist_set_pd_state(packet, &pdCmd);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_bdot, PD_BDOT);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_com1, PD_COM1);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_com2, PD_COM2);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_eps, PD_EPS);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_estim, PD_ESTIM);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_ppt, PD_PPT);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_rahs, PD_RAHS);
            setPowerSwitchFromCAN(pdCmd.gcmd_dist_set_pd_state_wheels, PD_WHEELS);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_BDOT:
            decodegcmd_dist_set_pd_ovc_bdot(packet, &ovcPktBDot);
            distSetOCPThreshold(PD_BDOT, ovcPktBDot.gcmd_dist_set_pd_ovc_bdot_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_COM1:
            decodegcmd_dist_set_pd_ovc_com1(packet, &ovcPktCom1);
            distSetOCPThreshold(PD_COM1, ovcPktCom1.gcmd_dist_set_pd_ovc_com1_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_COM2:
            decodegcmd_dist_set_pd_ovc_com2(packet, &ovcPktCom2);
            distSetOCPThreshold(PD_COM2, ovcPktCom2.gcmd_dist_set_pd_ovc_com2_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_EPS:
            decodegcmd_dist_set_pd_ovc_eps(packet, &ovcPktEPS);
            distSetOCPThreshold(PD_EPS, ovcPktEPS.gcmd_dist_set_pd_ovc_eps_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_ESTIM:
            decodegcmd_dist_set_pd_ovc_estim(packet, &ovcPktEstim);
            distSetOCPThreshold(PD_ESTIM, ovcPktEstim.gcmd_dist_set_pd_ovc_estim_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_PPT:
            decodegcmd_dist_set_pd_ovc_ppt(packet, &ovcPktPPT);
            distSetOCPThreshold(PD_PPT, ovcPktPPT.gcmd_dist_set_pd_ovc_ppt_ovc);
            break;
        case CAN_ID_GCMD_DIST_SET_PD_OVC_RAHS:
            decodegcmd_dist_set_pd_ovc_rahs(packet, &ovcPktRAHS);
            distSetOCPThreshold(PD_RAHS, ovcPktRAHS.gcmd_dist_set_pd_ovc_rahs_ovc);
            break;
        case CAN_ID_CMD_REBOOT_REQUEST:
            decodecmd_reboot_request(packet, &rebootRequest);
            setPowerSwitchFromCAN(PD_CMD_Disable, rebootRequest.cmd_reboot_request_domain);
            __delay_cycles(1000); //TODO: verify/move this wait
            setPowerSwitchFromCAN(PD_CMD_Enable, rebootRequest.cmd_reboot_request_domain);
                break;
        case CAN_ID_GCMD_RESET_MINMAX:
        {
            gcmd_reset_minmax rstPkt = {0};
            decodegcmd_reset_minmax(packet, &rstPkt);
            if(rstPkt.gcmd_reset_minmax_dist)
            {
                aggVec_reset((aggVec *)&mspTempAg);
                aggVec_reset((aggVec *)&battVAg);
                aggVec_reset((aggVec *)&coulombCounterAg);
                uint8_t i;
                for(i = NUM_POWER_DOMAINS; i; i--)
                {
                    aggVec_reset((aggVec*)&ssCurrAgs[i - 1]);
                    aggVec_reset((aggVec *)&ssBusVAgs[i - 1]);
                }
            }
        }
        break;
        case CAN_ID_GCMD_AUTOSEQ_ADD_1:
            decodegcmd_autoseq_add_1(packet, &autoseqAdd1);
            pendingEvent.pkt.id = autoseqAdd1.gcmd_autoseq_add_1_can_id;
            pendingEvent.pkt.length = 8;
            pendingEvent.sendFlag = autoseqAdd1.gcmd_autoseq_add_1_sendflg;
            pendingEvent.time = autoseqAdd1.gcmd_autoseq_add_1_met;
            break;
        case CAN_ID_GCMD_AUTOSEQ_ADD_2:
            if(pendingEvent.time)
            {
                decodegcmd_autoseq_add_2(packet, &autoseqAdd2);
                uint8_t i;
                for(i = 0; i < 8; i++)
                    pendingEvent.pkt.data[i] = (uint8_t)(autoseqAdd2.gcmd_autoseq_add_2_data >> 8 * i);
                seqAddEvent(pendingEvent);
                pendingEvent.time = 0;
            }
            break;
        case CAN_ID_GCMD_AUTOSEQ_REMOVE_CAN_ID:
            decodegcmd_autoseq_remove_can_id(packet, &rmCanId);
            seqRemoveEventsWithID(rmCanId.gcmd_autoseq_remove_can_id_id);
            break;
        case CAN_ID_GCMD_AUTOSEQ_RM_AT_INDEX:
            decodegcmd_autoseq_rm_at_index(packet, &rmIndex);
            seqRemoveEventAtIndex(rmIndex.gcmd_autoseq_rm_at_index_index);
            break;
        case CAN_ID_GCMD_AUTOSEQ_GET_MET:
            decodegcmd_autoseq_get_met(packet, &getMet);
            getMetRsp.eps_dist_autoseq_get_met_rsp_met = seqGetMETForIndex(getMet.gcmd_autoseq_get_met_index);
            encodeeps_dist_autoseq_get_met_rsp(&getMetRsp, &autoseqMETResponsePkt);
            autoseqMETResponsePktSendFlag = 1;
            break;
        case CAN_ID_GCMD_AUTOSEQ_GET_INDICES:
            decodegcmd_autoseq_get_indices(packet, &getIndices);
            getIndicesRsp.eps_dist_autoseq_get_ind_rsp_ind = seqGetIndicesForId(getIndices.gcmd_autoseq_get_indices_id);
            encodeeps_dist_autoseq_get_ind_rsp(&getIndicesRsp, &autoseqIndicesResponsePkt);
            autoseqIndicesResponsePktSendFlag = 1;
            break;
        case CAN_ID_GCMD_AUTOSEQ_ENABLE:
        {
            gcmd_autoseq_enable pkt;
            decodegcmd_autoseq_enable(packet, &pkt);
            if(pkt.gcmd_autoseq_enable_enable != CAN_ENUM_NBOOL_NULL)
                autoSequencerEnabled = pkt.gcmd_autoseq_enable_enable;
        }
            break;
        case CAN_ID_GCMD_DIST_RESET_MISSION: //reset MET, clear events, reboot
            bspClearResetCount();
            persistentTime = (timeStamp){0, 0, 0, 0, 0};
            uint8_t j;
            for(j = sizeof(persistentEvents) / sizeof(sequenceEvent); j; j--)
                persistentEvents[j - 1].time = 0;
            eventsInitialized = 0;
        case CAN_ID_GCMD_DIST_SELF_RESTART:
            WDTCTL = 0; //reboot
            break;
        case CAN_ID_GCMD_DIST_AUTOSHUTOFF:
        {
            gcmd_dist_autoshutoff pktShutoff;
            decodegcmd_dist_autoshutoff(packet, &pktShutoff);
            if(pktShutoff.gcmd_dist_autoshutoff_bdot != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.bdot = pktShutoff.gcmd_dist_autoshutoff_bdot;
            if(pktShutoff.gcmd_dist_autoshutoff_com2 != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.com2 = pktShutoff.gcmd_dist_autoshutoff_com2;
            if(pktShutoff.gcmd_dist_autoshutoff_eps != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.eps = pktShutoff.gcmd_dist_autoshutoff_eps;
            if(pktShutoff.gcmd_dist_autoshutoff_estim != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.estim = pktShutoff.gcmd_dist_autoshutoff_estim;
            if(pktShutoff.gcmd_dist_autoshutoff_ppt != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.ppt = pktShutoff.gcmd_dist_autoshutoff_ppt;
            if(pktShutoff.gcmd_dist_autoshutoff_rahs != CAN_ENUM_NBOOL_NULL)
                shutoffEnabled.rahs = pktShutoff.gcmd_dist_autoshutoff_rahs;
        }
            break;
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
    aggVec_init_f(&mspTempAg);
    aggVec_init_i(&battVAg);
    aggVec_init_i(&coulombCounterAg);
    uint8_t i;
    for(i = NUM_POWER_DOMAINS; i; i--)
    {
        aggVec_init_i(&ssCurrAgs[i - 1]);
        aggVec_init_i(&ssBusVAgs[i - 1]);
    }
    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));
}

void initializeEvents()
{
	CANPacket pkt;

	////////////////////////////////////////////////////  TODO: this is test code for Con-Ops
	gcmd_dist_set_pd_state cmd = {0, 0, 0, 0, 1, 0, 0};
	encodegcmd_dist_set_pd_state(&cmd, &pkt);
	persistentEvents[0].pkt = pkt;
	persistentEvents[0].sendFlag = 0;
	persistentEvents[0].time = 15;
	////////////////////////////////////////////////////

	/*uint8_t i;
	for(i = 0; i < NUM_POWER_DOMAINS; i++)
	{
		gcmd_dist_set_pd_state cmd = {0};
		switch(i)
		{
			case 0:
				cmd.gcmd_dist_set_pd_state_bdot = PD_CMD_Enable;
				break;
			case 1:
				cmd.gcmd_dist_set_pd_state_com1 = PD_CMD_Enable;
				break;
			case 2:
				cmd.gcmd_dist_set_pd_state_com2 = PD_CMD_Enable;
				break;
			case 3:
				cmd.gcmd_dist_set_pd_state_eps = PD_CMD_Enable;
				break;
			case 4:
				cmd.gcmd_dist_set_pd_state_estim = PD_CMD_Enable;
				break;
			case 5:
				cmd.gcmd_dist_set_pd_state_ppt = PD_CMD_Enable;
				break;
			case 6:
				cmd.gcmd_dist_set_pd_state_rahs = PD_CMD_Enable;
				break;
			case 7:
				cmd.gcmd_dist_set_pd_state_wheels = PD_CMD_Enable;
				break;
		}
		encodegcmd_dist_set_pd_state(&cmd, &pkt);
		persistentEvents[i].pkt = pkt;
		persistentEvents[i].sendFlag = 0;
		persistentEvents[i].time = i + 1; //sequentially activated
	}*/
	eventsInitialized = 1;
}

void sendSequenceResponses()
{
    if(autoseqIndicesResponsePktSendFlag && canTxCheck() != CAN_TX_BUSY)
    {
        canSendPacket(&autoseqIndicesResponsePkt);
        autoseqIndicesResponsePktSendFlag = 0;
    }
    if(autoseqMETResponsePktSendFlag && canTxCheck() != CAN_TX_BUSY)
    {
        canSendPacket(&autoseqMETResponsePkt);
        autoseqMETResponsePktSendFlag = 0;
    }
}

void INAInit()
{
    INA_DIR |= INA_BIT;
    INA_OUT |= INA_BIT;
}

void restartINA() //TODO: send a restart command instead
{
    INA_OUT &= ~INA_BIT;
    __delay_cycles((uint16_t)(MSEC * 10));
    INA_OUT |= INA_BIT;
}

void checkRollcallWatchdog()
{
    if(rollcallWatchdog >= ROLLCALL_WATCHDOG_TIMEOUT)
    {
        canWrapInitWithFilter(); //also restarts the CAN controller
        rollcallWatchdog = 0;
    }
    else if(rollcallWatchdog >= ROLLCALL_WATCHDOG_AUTOSHUTOFF)
    {
        autoShutoff();
    }
}

void incrementRollcallWatchdog()
{
    rollcallWatchdog++;
}

/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    WDTCTL = WDTPW | WDTCNTCL | WDTTMSEL_0 | WDTSSEL_0 | WDTIS_1; //TODO: revert this to take watchdog out
    //WDTCTL = WDTPW | WDTHOLD;
    bspInit(__SUBSYSTEM_MODULE__);  // This uses the framily of __SS_etc predefined symbols - see bsp.h
    METInitWithTime(persistentTime);

    bspBackpowerPulldown();
    INAInit();

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
    if(!eventsInitialized)
    	initializeEvents();

    seqInit(persistentEvents, sizeof(persistentEvents) / sizeof(persistentEvents[0]));

    initializeTimer();
    initData();


    startCallback(timerCallbackInitializer(&incrementRollcallWatchdog, 1000000));

    uint16_t counter = 0;
    while (1)
    {
        WDTCTL = WDT_CONFIG;
        // TODO:  eventually drive this with a timer
        LED_OUT ^= LED_BIT;
        __delay_cycles(0.1 * SEC);

        // This assumes that some interrupt code will change the value of the triggerStaten variables
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
        rollcallUpdate();
        persistentTime = getMETTimestamp();
        seqUpdateMET(metConvertToSeconds(persistentTime));
        checkSequence(autoSequencerEnabled);
        sendSequenceResponses();
        checkRollcallWatchdog();

        if(i2cGetLastOperationResult())
            restartINA();
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}


