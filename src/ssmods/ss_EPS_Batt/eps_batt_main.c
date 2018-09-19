#include <eps_batt.h>
#include <msp430.h>
#include "bsp/bsp.h"

#include "sensors/pcvsensor.h"       //INA219
#include "sensors/coulomb_counter.h" //Coulomb counter
#include "core/i2c.h"
#include "interfaces/canwrap.h"
#include "core/MET.h"
#include "core/agglib.h"
#include "core/timer.h"
#include "interfaces/rollcall.h"

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulateH1, rcPopulateH2, rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5, rcPopulate6, rcPopulate7
};

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC volatile SubsystemState ss_state    = State_FirstState;
FILE_STATIC volatile SubsystemMode ss_mode      = Mode_FirstMode;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;

// Telemetry and commands
FILE_STATIC meta_segment mseg;
FILE_STATIC general_segment gseg;
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

//Sensor data
FILE_STATIC PCVSensorData *INAData;
FILE_STATIC CoulombCounterData CCData;
FILE_STATIC hDev hTempC;

FILE_STATIC volatile uint8_t heaterIsChecking;
FILE_STATIC volatile uint8_t balancerIsChecking = 1;

FILE_STATIC float previousTemp;

FILE_STATIC volatile int autoHeating = 1;

FILE_STATIC aggVec_f mspTempAg;
FILE_STATIC aggVec_i tempAg;
FILE_STATIC aggVec_i voltageAg;
FILE_STATIC aggVec_i currentAg;
FILE_STATIC aggVec_i nodeVoltageAg;
FILE_STATIC aggVec_i nodeCurrentAg;
FILE_STATIC aggVec_i accChargeAg;

#pragma PERSISTENT(timeSinceLastFullCharge)
uint32_t timeSinceLastFullCharge = 0xFFFFFFFF;

/* ------BATTERY BALANCER------ */
FILE_STATIC void battInit()
{       BATTERY_BALANCER_ENABLE_DIR |= BATTERY_BALANCER_ENABLE_BIT; //Initialize battery balancer enable register pin
        HEATER_ENABLE_DIR |= HEATER_ENABLE_BIT;
        HEATER_ENABLE_OUT &= ~HEATER_ENABLE_BIT;                    //Initialize heater enable pin to off
        LED_DIR |= LED_BIT;
        LED_OUT |= LED_BIT;
        /*-------TMP36 TEMP SENSOR-------*/
        //    asensorInit(Ref_2p5V);
        hTempC = asensorActivateChannel(CHAN_A0, Type_GeneralV);
}

FILE_STATIC void battControlBalancer(Cmds cmd)
{
    gseg.lastbalancercmd = (uint8_t)cmd;

    if (cmd == Cmd_ExplicitEnable || cmd == Cmd_AutoEnable)
        BATTERY_BALANCER_ENABLE_OUT |= BATTERY_BALANCER_ENABLE_BIT;
    else if (cmd == Cmd_ExplicitDisable || cmd == Cmd_InitialDisable)
        BATTERY_BALANCER_ENABLE_OUT &= ~BATTERY_BALANCER_ENABLE_BIT;
}


/* ------BATTERY HEATERS------ */
FILE_STATIC void battControlHeater(Cmds cmd)
{
    gseg.lastheatercmd = (uint8_t)cmd;

    if (cmd == Cmd_ExplicitEnable || cmd == Cmd_AutoEnable)
        HEATER_ENABLE_OUT |= HEATER_ENABLE_BIT;
    else if (cmd == Cmd_ExplicitDisable || cmd == Cmd_InitialDisable)
        HEATER_ENABLE_OUT &= ~HEATER_ENABLE_BIT;
}

FILE_STATIC uint8_t handleActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    battmgmt_segment *bsegment;

    if (mode == Mode_BinaryStreaming)
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_BATTMGMT:
                bsegment = (battmgmt_segment *) &cmdstr[1];
                if (bsegment->enablebattbal != NOCHANGE)
                    battControlBalancer(bsegment->enablebattbal ? Cmd_ExplicitEnable : Cmd_ExplicitDisable);
                if (bsegment->enablebattheater != NOCHANGE)
                    battControlHeater(bsegment->enablebattheater ? Cmd_ExplicitEnable : Cmd_ExplicitDisable);
                break;

            case OPCODE_SET_CHECK_STATE:
                heaterIsChecking = ((setCheckState_segment *) &cmdstr[1])->isChecking;
                break;
        }
    }
    return 1;
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void battBcSendGeneral()
{
    gseg.isChecking = heaterIsChecking;
    bcbinSendPacket((uint8_t *) &gseg, sizeof(gseg));
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void battBcSendHealth()
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
FILE_STATIC void battBcSendSensorDat()
{
    bcbinSendPacket((uint8_t *) &sseg, sizeof(sseg));
}

FILE_STATIC void battBcSendMeta()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

void secondsInterrupt()
{
    if(timeSinceLastFullCharge ^= 0xFFFFFFFF)
        timeSinceLastFullCharge++;
}

void readCC (){
    //Reading Coulomb counter
    CCData = readCoulombCounter(); //reading the Coulomb counter (busVoltageV, calcdCurrentA, rawAccumCharge, (SOC?))
    sseg.battVolt = CCData.busVoltageV;
    sseg.SOC = CCData.SOC;
    sseg.battCurr = CCData.calcdCurrentA;
    sseg.battCharge = CCData.battCharge;
    sseg.accumulatedCharge = CCData.totalAccumulatedCharge;

    if(balancerIsChecking)
    {
        if((CCData.busVoltageV <= 6.1f || CCData.calcdCurrentA > 3.0f || CCData.calcdCurrentA < -3.0f) && (BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT))
            battControlBalancer(Cmd_ExplicitDisable); //on and out of range
        else if(!(BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) && CCData.busVoltageV > 6.1f && CCData.calcdCurrentA <= 3.0f && CCData.calcdCurrentA >= -3.0f)
			battControlBalancer(Cmd_ExplicitEnable); //off and in range
    }

    aggVec_push_i(&voltageAg, CCReadRawVoltage());
    aggVec_push_i(&currentAg, CCReadRawCurrent());
    aggVec_push_i(&accChargeAg, CCReadRawAccumulatedCharge());
    if(CCData.fullCharge)
        timeSinceLastFullCharge = 0;
}

void readINA(hDev hSensor){
    //Reading INA219
    INAData = pcvsensorRead(hSensor, Read_BusV | Read_CurrentA);
    sseg.battNodeVolt = INAData->busVoltageV;
    sseg.battNodeCurr = INAData->calcdCurrentA;
    aggVec_push_i(&nodeCurrentAg, INAData->rawCurrent); //TODO: get raw values here
    aggVec_push_i(&nodeVoltageAg, INAData->rawBusVoltage);
}

void readTempSensor(){
    float tempVoltageV =  asensorReadSingleSensorV(hTempC); //Assuming in V (not mV)
    //sseg.battTemp = ((tempVoltageV/100.0) - 500) / 10.0; //Temp in Celcius
    //conversion from voltage to temperatur, has a slope of 10mv per degree celcius
    //and an intercept of 50 degrees at 0 voltsx
    aggVec_push_i(&tempAg, (tempVoltageV /0.010));

    sseg.battTemp = (tempVoltageV /0.010) - 50;
}

void can_packet_rx_callback(CANPacket *packet)
{
    gcmd_eps_batt_fulldef fullDefPkt;
    gcmd_batt_set_heater_check heaterCheckPkt;
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
        {
            gcmd_reset_minmax pktRst;
            decodegcmd_reset_minmax(packet, &pktRst);
            if(pktRst.gcmd_reset_minmax_batt)
            {
                aggVec_reset((aggVec *)&mspTempAg);
                aggVec_reset((aggVec *)&tempAg);
                aggVec_reset((aggVec *)&voltageAg);
                aggVec_reset((aggVec *)&currentAg);
                aggVec_reset((aggVec *)&nodeVoltageAg);
                aggVec_reset((aggVec *)&nodeCurrentAg);
                aggVec_reset((aggVec *)&accChargeAg);
            }
        }
            break;
        case CAN_ID_GCMD_EPS_BATT_FULLDEF:
            decodegcmd_eps_batt_fulldef(packet, &fullDefPkt);
            CCSetFullCurrent(fullDefPkt.gcmd_eps_batt_fulldef_chg_curr);
            CCSetFullVoltage(fullDefPkt.gcmd_eps_batt_fulldef_const_volt);
            break;
        case CAN_ID_GCMD_BATT_SET_HEATER_CHECK:
            decodegcmd_batt_set_heater_check(packet, &heaterCheckPkt);
            heaterIsChecking = heaterCheckPkt.gcmd_batt_set_heater_check_state;
            break;
        case CAN_ID_GCMD_BATT_SET_BAL_AUTO:
        {
            gcmd_batt_set_bal_auto pkt;
            decodegcmd_batt_set_bal_auto(packet, &pkt);
            if(pkt.gcmd_batt_set_bal_auto_state != CAN_ENUM_NBOOL_NULL)
                balancerIsChecking = pkt.gcmd_batt_set_bal_auto_state;
        }
            break;
        case CAN_ID_GCMD_DIST_RESET_MISSION: //clear persistent flags here
            bspClearResetCount();
            break;
        default:
            break;
    }
}

void rcPopulateH1(CANPacket *out)
{
    rc_eps_batt_h1 rc = {0};
    rc.rc_eps_batt_h1_sysrstiv = bspGetResetReason();
    rc.rc_eps_batt_h1_reset_count = bspGetResetCount();
    rc.rc_eps_batt_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
    rc.rc_eps_batt_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
    rc.rc_eps_batt_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
    rc.rc_eps_batt_h1_reset_count = bspGetResetCount();
    encoderc_eps_batt_h1(&rc, out);
    aggVec_as_reset((aggVec *)&mspTempAg);
}

void rcPopulateH2(CANPacket *out)
{
    rc_eps_batt_h2 rc = {0};
    rc.rc_eps_batt_h2_canrxerror = canRxErrorCheck();
    encoderc_eps_batt_h2(&rc, out);
}

void rcPopulate1(CANPacket *out)
{
    rc_eps_batt_1 rc = {0};
    rc.rc_eps_batt_1_acc_charge_avg = aggVec_avg_i_i(&accChargeAg);
    rc.rc_eps_batt_1_voltage_avg = aggVec_avg_i_i(&voltageAg);
    encoderc_eps_batt_1(&rc, out);
    aggVec_as_reset((aggVec *)&accChargeAg);
    aggVec_as_reset((aggVec *)&voltageAg);
}

void rcPopulate2(CANPacket *out)
{
    rc_eps_batt_2 rc = {0};
    rc.rc_eps_batt_2_node_v_avg = aggVec_avg_i_i(&nodeVoltageAg);
    rc.rc_eps_batt_2_node_v_max = aggVec_max_i(&nodeVoltageAg);
    rc.rc_eps_batt_2_node_v_min = aggVec_min_i(&nodeVoltageAg);
    encoderc_eps_batt_2(&rc, out);
    aggVec_as_reset((aggVec *)&nodeVoltageAg);
}

void rcPopulate3(CANPacket *out)
{
    rc_eps_batt_3 rc = {0};
    rc.rc_eps_batt_3_batt_temp_avg = aggVec_avg_i_i(&tempAg);
    rc.rc_eps_batt_3_current_avg = aggVec_avg_i_i(&currentAg);
    rc.rc_eps_batt_3_current_max = aggVec_max_i(&currentAg);
    rc.rc_eps_batt_3_current_min = aggVec_min_i(&currentAg);
    encoderc_eps_batt_3(&rc, out);
    aggVec_as_reset((aggVec *)&tempAg);
    aggVec_as_reset((aggVec *)&currentAg);
}

void rcPopulate4(CANPacket *out)
{
    rc_eps_batt_4 rc = {0};
    rc.rc_eps_batt_4_balancer_state = (BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) != 0;
    rc.rc_eps_batt_4_heater_state = (HEATER_ENABLE_OUT & HEATER_ENABLE_BIT) != 0;
    rc.rc_eps_batt_4_voltage_avg = 0; //aggVec_avg_i_i(&voltageAg);
    rc.rc_eps_batt_4_voltage_max = aggVec_max_i(&voltageAg);
    rc.rc_eps_batt_4_voltage_min = aggVec_min_i(&voltageAg);
    rc.rc_eps_batt_4_bal_auto_state = balancerIsChecking;
    rc.rc_eps_batt_4_heater_auto_state = heaterIsChecking;
    encoderc_eps_batt_4(&rc, out);
}

void rcPopulate5(CANPacket *out)
{
    rc_eps_batt_5 rc = {0};
    rc.rc_eps_batt_5_batt_temp_max = aggVec_max_i(&tempAg);
    rc.rc_eps_batt_5_batt_temp_min = aggVec_min_i(&tempAg);
    rc.rc_eps_batt_5_node_c_avg = aggVec_avg_i_i(&nodeCurrentAg);
    rc.rc_eps_batt_5_node_c_max = aggVec_max_i(&nodeCurrentAg);
    rc.rc_eps_batt_5_node_c_min = aggVec_min_i(&nodeCurrentAg);
    encoderc_eps_batt_5(&rc, out);
    aggVec_as_reset((aggVec *)&nodeCurrentAg);
}

void rcPopulate6(CANPacket *out)
{
    rc_eps_batt_6 rc = {0};
    rc.rc_eps_batt_6_last_charge = timeSinceLastFullCharge;
    rc.rc_eps_batt_6_status = CCGetStatusReg();
    rc.rc_eps_batt_6_ctrl = CCGetControlReg();
    encoderc_eps_batt_6(&rc, out);
}

void rcPopulate7(CANPacket *out)
{
    rc_eps_batt_7 rc = {0};
    rc.rc_eps_batt_7_acc_charge_avg = 0; //aggVec_avg_i_i(&accChargeAg);
    rc.rc_eps_batt_7_acc_charge_max = aggVec_max_i(&accChargeAg);
    rc.rc_eps_batt_7_acc_charge_min = aggVec_min_i(&accChargeAg);
    encoderc_eps_batt_7(&rc, out);
}

/*
 * main.c
 */

int main(void)
{
    P3DIR |= BIT4; //this is for Paul's backpower fix
    P3OUT |= BIT4;
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    asensorInit(Ref_2p5V);
    battInit();

    /* ------INA219/PVC SENSOR------ */
    hDev hSensor;
    hSensor = pcvsensorInit(I2CBus2, 0x40, 1.0, 100); //ground ground, 1 ohm???, 100 mA

    /* ------COULOMB COUNTER---- */
    LTC2943Init(I2CBus2, 6.0);

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                          NULL,
                                          handleActionCallback);
#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    debugTraceF(1, "Commencing subsystem module execution ...\r\n");

    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&(gseg.header), TLM_ID_EPS_BATT_GENERAL, sizeof(gseg));
    bcbinPopulateHeader(&(sseg.header), TLM_ID_EPS_BATT_SENSORDAT, sizeof(sseg));

    battControlBalancer(Cmd_AutoEnable);
    //battControlHeater(Cmd_AutoEnable);

    aggVec_init_f(&mspTempAg);
    aggVec_init_i(&tempAg);
    aggVec_init_i(&voltageAg);
    aggVec_init_i(&currentAg);
    aggVec_init_i(&nodeVoltageAg);
    aggVec_init_i(&nodeCurrentAg);
    aggVec_init_i(&accChargeAg);

    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    canWrapInitWithFilter();
    setCANPacketRxCallback(can_packet_rx_callback);

    previousTemp = asensorReadSingleSensorV(hTempC);
    startCallback(timerCallbackInitializer(&secondsInterrupt, 1000000));
    uint16_t counter;
    while (1)
    {
        counter++;

        LED_OUT ^= LED_BIT;
        __delay_cycles(0.125 * SEC);

        // Reading data
        readCC(); // Read all of the I2C sensor data
        readINA(hSensor);
        sseg.heaterState = (HEATER_ENABLE_OUT & HEATER_ENABLE_BIT) != 0;
        sseg.balancerState = (BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) != 0;
        readTempSensor();

        battBcSendSensorDat();

        // Stuff running at 1Hz-ish
        if (counter % 8 == 0)
        {
            //check Coulomb counter
            readCoulombCounterStatus();
            gseg.CC_StatusReg = CCData.statusReg;
            readCoulombCounterControl();
            gseg.CC_ControlReg = CCData.controlReg;

            battBcSendGeneral();
            battBcSendHealth();

            float temp = asensorReadSingleSensorV(hTempC); //<-- this is batt temp
            if(heaterIsChecking)
            {
                if(previousTemp >= 0.5f && temp < 0.5f) //not heating & < 0C
                    HEATER_ENABLE_OUT |= HEATER_ENABLE_BIT; //turn on

                else if (previousTemp <= 0.6f && temp > 0.6f) //heating & > 10C
                    HEATER_ENABLE_OUT &= ~HEATER_ENABLE_BIT; //turn off
            }
            else if((HEATER_ENABLE_OUT & HEATER_ENABLE_BIT) && temp > 0.6f)
                HEATER_ENABLE_OUT |= HEATER_ENABLE_BIT;
            previousTemp = temp;
        }

        // Stuff running 4Hz
        if (counter % 32 ==0)
        {
            battBcSendMeta();
        }
        rollcallUpdate();
     }
}
