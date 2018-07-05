#include <eps_batt.h>
#include <msp430.h>
#include "bsp/bsp.h"

#include "sensors/pcvsensor.h"       //INA219
#include "sensors/coulomb_counter.h" //Coulomb counter
#include "core/i2c.h"
#include "interfaces/canwrap.h"
#include "core/MET.h"
#include "core/agglib.h"

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
FILE_STATIC volatile uint8_t balancerIsChecking;

FILE_STATIC float previousTemp;

FILE_STATIC volatile int autoHeating = 1;

FILE_STATIC aggVec_f mspTempAg;
FILE_STATIC aggVec_i tempAg;
FILE_STATIC aggVec_i voltageAg;
FILE_STATIC aggVec_i currentAg;
FILE_STATIC aggVec_i nodeVoltageAg;
FILE_STATIC aggVec_i nodeCurrentAg;
FILE_STATIC aggVec_i accChargeAg;

FILE_STATIC uint64_t lastFullChargeTime = 0;
#pragma PERSISTENT(lastFullChargeTime);

FILE_STATIC uint8_t rcFlag = 0;

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
        lastFullChargeTime = metConvertToInt(getMETTimestamp());
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
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rcFlag = 8;
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
            aggVec_reset((aggVec *)&mspTempAg);
            aggVec_reset((aggVec *)&tempAg);
            aggVec_reset((aggVec *)&voltageAg);
            aggVec_reset((aggVec *)&currentAg);
            aggVec_reset((aggVec *)&nodeVoltageAg);
            aggVec_reset((aggVec *)&nodeCurrentAg);
            aggVec_reset((aggVec *)&accChargeAg);
            break;
        case CAN_ID_GCMD_EPS_BATT_FULLDEF:
            decodegcmd_eps_batt_fulldef(packet, &fullDefPkt);
            CCSetFullCurrent(fullDefPkt.gcmd_eps_batt_fulldef_chg_curr);
            CCSetFullVoltage(fullDefPkt.gcmd_eps_batt_fulldef_const_volt);
            break;
        default:
            break;
    }
}

void sendRC()
{
    while(rcFlag && (canTxCheck() != CAN_TX_BUSY))
    {
        CANPacket rollcallPkt = {0};
        if(rcFlag == 8)
        {
            rc_eps_batt_h1 rollcallPkt1_info = {0};
            float newVal = asensorReadIntTempC();
            rollcallPkt1_info.rc_eps_batt_h1_sysrstiv = SYSRSTIV;
            rollcallPkt1_info.rc_eps_batt_h1_reset_count = bspGetResetCount();
            rollcallPkt1_info.rc_eps_batt_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
            rollcallPkt1_info.rc_eps_batt_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
            rollcallPkt1_info.rc_eps_batt_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
            rollcallPkt1_info.rc_eps_batt_h1_reset_count = bspGetResetCount();
            encoderc_eps_batt_h1(&rollcallPkt1_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&mspTempAg);
        }
        else if(rcFlag == 7)
        {
            rc_eps_batt_h2 healthPkt2 = {0};
            healthPkt2.rc_eps_batt_h2_canrxerror = canRxErrorCheck();
            encoderc_eps_batt_h2(&healthPkt2, &rollcallPkt);
        }
        else if(rcFlag == 6)
        {
            rc_eps_batt_2 rollcallPkt2_info = {0};
            rollcallPkt2_info.rc_eps_batt_2_node_v_avg = aggVec_avg_i_i(&nodeVoltageAg);
            rollcallPkt2_info.rc_eps_batt_2_node_v_max = aggVec_max_i(&nodeVoltageAg);
            rollcallPkt2_info.rc_eps_batt_2_node_v_min = aggVec_min_i(&nodeVoltageAg);
            encoderc_eps_batt_2(&rollcallPkt2_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&nodeVoltageAg);
        }
        else if(rcFlag == 5)
        {
            rc_eps_batt_3 rollcallPkt3_info = {0};
            rollcallPkt3_info.rc_eps_batt_3_batt_temp_avg = aggVec_avg_i_i(&tempAg);
            rollcallPkt3_info.rc_eps_batt_3_current_avg = aggVec_avg_i_i(&currentAg);
            rollcallPkt3_info.rc_eps_batt_3_current_max = aggVec_max_i(&currentAg);
            rollcallPkt3_info.rc_eps_batt_3_current_min = aggVec_min_i(&currentAg);
            encoderc_eps_batt_3(&rollcallPkt3_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&tempAg);
            aggVec_as_reset((aggVec *)&currentAg);
        }
        else if(rcFlag == 4)
        {
            rc_eps_batt_4 rollcallPkt4_info = {0};
            rollcallPkt4_info.rc_eps_batt_4_balancer_state = (BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) != 0;
            rollcallPkt4_info.rc_eps_batt_4_heater_state = (HEATER_ENABLE_OUT & HEATER_ENABLE_BIT) != 0;
            rollcallPkt4_info.rc_eps_batt_4_voltage_avg = aggVec_avg_i_i(&voltageAg);
            rollcallPkt4_info.rc_eps_batt_4_voltage_max = aggVec_max_i(&voltageAg);
            rollcallPkt4_info.rc_eps_batt_4_voltage_min = aggVec_min_i(&voltageAg);
            encoderc_eps_batt_4(&rollcallPkt4_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&voltageAg);
        }
        else if(rcFlag == 3)
        {
            rc_eps_batt_5 rollcallPkt5_info = {0};
            rollcallPkt5_info.rc_eps_batt_5_batt_temp_max = aggVec_max_i(&tempAg);
            rollcallPkt5_info.rc_eps_batt_5_batt_temp_min = aggVec_min_i(&tempAg);
            rollcallPkt5_info.rc_eps_batt_5_node_c_avg = aggVec_avg_i_i(&nodeCurrentAg);
            rollcallPkt5_info.rc_eps_batt_5_node_c_max = aggVec_max_i(&nodeCurrentAg);
            rollcallPkt5_info.rc_eps_batt_5_node_c_min = aggVec_min_i(&nodeCurrentAg);
            encoderc_eps_batt_5(&rollcallPkt5_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&nodeCurrentAg);
        }
        else if(rcFlag == 2)
        {
            rc_eps_batt_6 rollcallPkt6_info = {0};
            rollcallPkt6_info.rc_eps_batt_6_last_charge = lastFullChargeTime;
            rollcallPkt6_info.rc_eps_batt_6_status = CCGetStatusReg();
            rollcallPkt6_info.rc_eps_batt_6_ctrl = CCGetControlReg();
            encoderc_eps_batt_6(&rollcallPkt6_info, &rollcallPkt);
        }
        else if(rcFlag == 1)
        {
            rc_eps_batt_7 rollcallPkt7_info = {0};
            rollcallPkt7_info.rc_eps_batt_7_acc_charge_avg = aggVec_avg_i_i(&accChargeAg);
            rollcallPkt7_info.rc_eps_batt_7_acc_charge_max = aggVec_max_i(&accChargeAg);
            rollcallPkt7_info.rc_eps_batt_7_acc_charge_min = aggVec_min_i(&accChargeAg);
            encoderc_eps_batt_7(&rollcallPkt7_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&accChargeAg);
        }
        canSendPacket(&rollcallPkt);
        rcFlag--;
    }
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

    //canWrapInitWithFilter();
    canWrapInit();
    setCANPacketRxCallback(can_packet_rx_callback);

    previousTemp = asensorReadSingleSensorV(hTempC);
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

            if(heaterIsChecking)
            {
                float temp = asensorReadSingleSensorV(hTempC); //<-- this is batt temp
                if(previousTemp >= 0.5f && temp < 0.5f) //not heating & < 0C
                    HEATER_ENABLE_OUT |= HEATER_ENABLE_BIT; //turn on

                else if (previousTemp <= 0.6f && temp > 0.6f) //heating & > 10C
                    HEATER_ENABLE_OUT &= ~HEATER_ENABLE_BIT; //turn off
                previousTemp = temp;
            }
        }

        // Stuff running 4Hz
        if (counter % 32 ==0)
        {
            battBcSendMeta();
        }
        sendRC();
     }
}
