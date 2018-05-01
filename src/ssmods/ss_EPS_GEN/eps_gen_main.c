#include <eps_gen.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"
#include "core/dataArray.h"
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

FILE_STATIC hDev hTempSensors[NUM_PANELS];
FILE_STATIC panel_info panels[NUM_PANELS];

FILE_STATIC uint8_t panelPCVSensorAddresses[] =  { PCVI2C_PANEL1, PCVI2C_PANEL2, PCVI2C_PANEL3 };
FILE_STATIC float panelShuntResistances[] =      { SHUNT_SIDE_PANELS, SHUNT_CENTER_PANEL, SHUNT_SIDE_PANELS };
FILE_STATIC ACHANNEL tempChannels[] =            { CHAN_TEMP1, CHAN_TEMP2, CHAN_TEMP3 };

FILE_STATIC meta_segment mseg;
FILE_STATIC general_segment gseg;
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

FILE_STATIC float mspTempArray[480] = {0};
FILE_STATIC float panel1VoltageArray[480] = {0};
FILE_STATIC float panel2VoltageArray[480] = {0};
FILE_STATIC float panel3VoltageArray[480] = {0};
FILE_STATIC float panel1CurrentArray[480] = {0};
FILE_STATIC float panel2CurrentArray[480] = {0};
FILE_STATIC float panel3CurrentArray[480] = {0};
FILE_STATIC float panel1PwrArray[480] = {0};
FILE_STATIC float panel2PwrArray[480] = {0};
FILE_STATIC float panel3PwrArray[480] = {0};
FILE_STATIC float panel1TempArray[480] = {0};
FILE_STATIC float panel2TempArray[480] = {0};
FILE_STATIC float panel3TempArray[480] = {0};

#pragma PERSISTENT(mspTempArray);
#pragma PERSISTENT(panel1VoltageArray);
#pragma PERSISTENT(panel2VoltageArray);
#pragma PERSISTENT(panel3VoltageArray);
#pragma PERSISTENT(panel1CurrentArray);
#pragma PERSISTENT(panel2CurrentArray);
#pragma PERSISTENT(panel3CurrentArray);
#pragma PERSISTENT(panel1PwrArray);
#pragma PERSISTENT(panel2PwrArray);
#pragma PERSISTENT(panel3PwrArray);
#pragma PERSISTENT(panel1TempArray);
#pragma PERSISTENT(panel2TempArray);
#pragma PERSISTENT(panel3TempArray);

FILE_STATIC uint16_t mspTemp;
FILE_STATIC uint16_t panel1Voltage;
FILE_STATIC uint16_t panel2Voltage;
FILE_STATIC uint16_t panel3Voltage;
FILE_STATIC uint16_t panel1Current;
FILE_STATIC uint16_t panel2Current;
FILE_STATIC uint16_t panel3Current;
FILE_STATIC uint16_t panel1Pwr;
FILE_STATIC uint16_t panel2Pwr;
FILE_STATIC uint16_t panel3Pwr;
FILE_STATIC uint16_t panel1Temp;
FILE_STATIC uint16_t panel2Temp;
FILE_STATIC uint16_t panel3Temp;

FILE_STATIC uint8_t rcFlag = 0;

FILE_STATIC void genTempSensorsInit()
{
    asensorInit(Ref_2p5V);
    int i;
    for (i = 0; i < NUM_PANELS; ++i)
    {
        hTempSensors[i] = asensorActivateChannel(tempChannels[i], Type_GeneralV);
    }

    return;
}

FILE_STATIC void genPCVSensorsInit()
{
    int i;
    for (i = 0; i < NUM_PANELS; ++i)
    {
        panels[i].panelnum = i;
        panels[i].shuntresistance = panelShuntResistances[i];
        panels[i].hpcvsensor = pcvsensorInit(I2CBus1, panelPCVSensorAddresses[i], panelShuntResistances[i], PANEL_CURRENT_LIMIT);
    }
}

FILE_STATIC void genPanelsTrackersInit()
{
    // Configure PT disable pins at outputs
    DISABLE_PTRACKER1_DIR |= DISABLE_PTRACKER1_BIT;
    DISABLE_PTRACKER2_DIR |= DISABLE_PTRACKER2_BIT;
    DISABLE_PTRACKER3_DIR |= DISABLE_PTRACKER3_BIT;

    // Configure charging pin monitors as inputs
    CHARGING_PTRACKER1_DIR &= ~CHARGING_PTRACKER1_BIT;
    CHARGING_PTRACKER2_DIR &= ~CHARGING_PTRACKER2_BIT;
    CHARGING_PTRACKER3_DIR &= ~CHARGING_PTRACKER3_BIT;

}

FILE_STATIC void genSetPowerTracker(PowTrackerNum ptnum, BOOL enable)
{
    gseg.ptlastcmds[(uint8_t)ptnum] = (enable ? (uint8_t)ExplicitEnable : (uint8_t)ExplicitDisable);

    switch (ptnum)
    {
        case PowerTracker1:
            if (enable) DISABLE_PTRACKER1_OUT &= ~DISABLE_PTRACKER1_BIT;
            else DISABLE_PTRACKER1_OUT |= DISABLE_PTRACKER1_BIT;
            break;
        case PowerTracker2:
            if (enable) DISABLE_PTRACKER2_OUT &= ~DISABLE_PTRACKER2_BIT;
            else DISABLE_PTRACKER2_OUT |= DISABLE_PTRACKER2_BIT;
            break;
        case PowerTracker3:
            if (enable) DISABLE_PTRACKER3_OUT &= ~DISABLE_PTRACKER3_BIT;
            else DISABLE_PTRACKER3_OUT |= DISABLE_PTRACKER3_BIT;
            break;
        default:
            break;
    }
}

FILE_STATIC void genMonitorPanels()
{
    int i;
    PCVSensorData *pdata;
    float temp;

    asensorUpdateAllSensors();

    pdata = pcvsensorRead(panels[0].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    addData_float(panel1Voltage, pdata->busVoltageV);
    addData_float(panel1Current, pdata->calcdCurrentA);
    addData_float(panel1Pwr, pdata->calcdPowerW);
    addData_float(panel1Temp, asensorGetLastValueV(hTempSensors[0]));

    pdata = pcvsensorRead(panels[1].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    addData_float(panel2Voltage, pdata->busVoltageV);
    addData_float(panel2Current, pdata->calcdCurrentA);
    addData_float(panel2Pwr, pdata->calcdPowerW);
    addData_float(panel2Temp, asensorGetLastValueV(hTempSensors[1]));

    pdata = pcvsensorRead(panels[2].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    addData_float(panel3Voltage, pdata->busVoltageV);
    addData_float(panel3Current, pdata->calcdCurrentA);
    addData_float(panel3Pwr, pdata->calcdPowerW);
    addData_float(panel3Temp, asensorGetLastValueV(hTempSensors[2]));

    for (i=0; i < NUM_PANELS; i++)
    {
        // Get power/current/bus voltage readings
        pdata = pcvsensorRead(panels[i].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
        sseg.panelcurrents[i] = pdata->calcdCurrentA;
        sseg.panelpowers[i] = pdata->calcdPowerW;
        sseg.panelvoltages[i] = pdata->busVoltageV;

        // Get temperature readings
        sseg.paneltempsC[i] = asensorGetLastValueV(hTempSensors[i]);
    }
}

FILE_STATIC void genMonitorPowerTrackers()
{
    int i;
    BOOL chargingactual, chargingdisable;
    for (i=0; i < NUM_PANELS; i++)
    {
        switch (i)
        {
            case 0:
                chargingactual = CHARGING_PTRACKER1_IN & CHARGING_PTRACKER1_BIT;
                chargingdisable = DISABLE_PTRACKER1_OUT & DISABLE_PTRACKER1_BIT;
                break;
            case 1:
                chargingactual = CHARGING_PTRACKER2_IN & CHARGING_PTRACKER2_BIT;
                chargingdisable = DISABLE_PTRACKER2_OUT & DISABLE_PTRACKER2_BIT;
                break;
            case 2:
                chargingactual = CHARGING_PTRACKER3_IN & CHARGING_PTRACKER3_BIT;
                chargingdisable = DISABLE_PTRACKER3_OUT & DISABLE_PTRACKER3_BIT;
                break;
        }

        gseg.ptchargingactual[i] = ( chargingactual != 0 ? TRUE : FALSE);
        gseg.ptchargingenablesw[i] = ( chargingdisable != 0 ? FALSE : TRUE );
    }
}

FILE_STATIC void genBcSendSensorDat()
{
    bcbinSendPacket((uint8_t *) &sseg, sizeof(sseg));
}

FILE_STATIC void genBcSendGeneral()
{
    bcbinSendPacket((uint8_t *) &gseg, sizeof(gseg));
}

FILE_STATIC void genBcSendMeta()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

FILE_STATIC void genBcSendHealth()
{
    // TODO:  Add call through debug registrations for STATUS on subentities (like the buses)

    // TODO:  Determine overall health based on querying various entities for their health
    // For now, everythingis always marginal ...
    hseg.oms = OMS_Unknown;
    hseg.inttemp = asensorReadIntTempC();
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandlers();
}

void can_packet_rx_callback(CANPacket *packet)
{
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rcFlag = 3;

            resetAvg_float(panel1Voltage);
            resetAvg_float(panel2Voltage);
            resetAvg_float(panel3Voltage);
            resetAvg_float(panel1Current);
            resetAvg_float(panel2Current);
            resetAvg_float(panel3Current);
            resetAvg_float(panel1Pwr);
            resetAvg_float(panel2Pwr);
            resetAvg_float(panel3Pwr);
            resetAvg_float(panel1Temp);
            resetAvg_float(panel2Temp);
            resetAvg_float(panel3Temp);
            break;
        default:
            break;
    }
}

/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    //mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>

    genTempSensorsInit();
    genPCVSensorsInit();
    genPanelsTrackersInit();

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&(gseg.header), TLM_ID_EPS_GEN_GENERAL, sizeof(gseg));
    bcbinPopulateHeader(&(sseg.header), TLM_ID_EPS_GEN_SENSORDAT, sizeof(sseg));

    mspTemp = init_float(mspTempArray, 480);
    panel1Voltage = init_float(panel1VoltageArray, 480);
    panel2Voltage = init_float(panel2VoltageArray, 480);
    panel3Voltage = init_float(panel3VoltageArray, 480);
    panel1Current = init_float(panel1CurrentArray, 480);
    panel2Current = init_float(panel2CurrentArray, 480);
    panel3Current = init_float(panel3CurrentArray, 480);
    panel1Pwr = init_float(panel1PwrArray, 480);
    panel2Pwr = init_float(panel2PwrArray, 480);
    panel3Pwr = init_float(panel3PwrArray, 480);
    panel1Temp = init_float(panel1TempArray, 480);
    panel2Temp = init_float(panel2TempArray, 480);
    panel3Temp = init_float(panel3TempArray, 480);

    canWrapInitWithFilter();
    setCANPacketRxCallback(can_packet_rx_callback);

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                     NULL,
                                     genActionCallback);

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSYSTEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");

//    genSetPowerTracker(PowerTracker1, FALSE);
//    genSetPowerTracker(PowerTracker2, FALSE);
//    genSetPowerTracker(PowerTracker3, FALSE);

    CANPacket rcPktTest = {0};
    cmd_rollcall rc_info = {0};
    encodecmd_rollcall(&rc_info, &rcPktTest);
    canSendPacket(&rcPktTest);

    uint16_t counter = 0;
    while (1)
    {
        __delay_cycles(0.125 * SEC);
        counter++;
        LED_OUT ^= LED_BIT;

        // Monitor everything
        genMonitorPanels();
        genMonitorPowerTrackers();

        // Report at correct rates
        genBcSendSensorDat();
        if (counter % 8 == 0)
        {
            genBcSendGeneral();
            genBcSendHealth();
        }
        if (counter % 32 == 0) genBcSendMeta();
    }
    if(rcFlag)
    {
        CANPacket rollcallPkt1 = {0};
        rc_eps_gen_1 rollcallPkt1_info = {0};
        CANPacket rollcallPkt2 = {0};
        rc_eps_gen_2 rollcallPkt2_info = {0};
        CANPacket rollcallPkt3 = {0};
        rc_eps_gen_3 rollcallPkt3_info = {0};
        CANPacket rollcallPkt4 = {0};
        rc_eps_gen_4 rollcallPkt4_info = {0};
        CANPacket rollcallPkt5 = {0};
        rc_eps_gen_5 rollcallPkt5_info = {0};
        CANPacket rollcallPkt6 = {0};
        rc_eps_gen_6 rollcallPkt6_info = {0};
        CANPacket rollcallPkt7 = {0};
        rc_eps_gen_7 rollcallPkt7_info = {0};
        CANPacket rollcallPkt8 = {0};
        rc_eps_gen_8 rollcallPkt8_info = {0};
        CANPacket rollcallPkt9 = {0};
        rc_eps_gen_9 rollcallPkt9_info = {0};
        switch(rcFlag)
        {
            case 1:
                //
                rollcallPkt1_info.rc_eps_gen_1_sysrstiv = bspGetResetCount();
                rollcallPkt1_info.rc_eps_gen_1_temp_avg = getAvg_float(mspTemp);
                rollcallPkt1_info.rc_eps_gen_1_temp_max = getMax_float(mspTemp);
                rollcallPkt1_info.rc_eps_gen_1_temp_min = getMin_float(mspTemp);
                rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_avg = getAvg_float(panel1Voltage);
                rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_max = getMax_float(panel1Voltage);
                rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_min = getMin_float(panel1Voltage);
                rollcallPkt2_info.rc_eps_gen_2_pnl_2_voltage_min = getMin_float(panel2Voltage);
                rollcallPkt3_info.rc_eps_gen_3_pnl_2_voltage_avg = getAvg_float(panel2Voltage);
                rollcallPkt3_info.rc_eps_gen_3_pnl_3_voltage_max = getMax_float(panel2Voltage);
                rollcallPkt3_info.rc_eps_gen_3_pnl_3_voltage_max = getMax_float(panel3Voltage);
                rollcallPkt3_info.rc_eps_gen_3_pnl_3_voltage_min = getMin_float(panel3Voltage);
                encoderc_eps_gen_1(&rollcallPkt1_info, &rollcallPkt1);
                encoderc_eps_gen_2(&rollcallPkt2_info, &rollcallPkt2);
                encoderc_eps_gen_3(&rollcallPkt3_info, &rollcallPkt3);
                canSendPacket(&rollcallPkt1);
                canSendPacket(&rollcallPkt2);
                canSendPacket(&rollcallPkt3);
                resetAvg_float(mspTemp);
                resetAvg_float(panel1Voltage);
                break;
            case 2:
                rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_avg = getAvg_float(panel1Current);
                rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_max = getMax_float(panel1Current);
                rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_min = getMin_float(panel1Current);
                rollcallPkt4_info.rc_eps_gen_4_pnl_3_voltage_avg = getAvg_float(panel3Voltage);
                rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_avg = getAvg_float(panel2Current);
                rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_max = getMax_float(panel2Current);
                rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_min = getMin_float(panel2Current);
                rollcallPkt5_info.rc_eps_gen_5_pnl_3_current_min = getMin_float(panel3Current);
                rollcallPkt6_info.rc_eps_gen_6_pnl_1_power_max = getMax_float(panel1Pwr);
                rollcallPkt6_info.rc_eps_gen_6_pnl_1_power_min = getMin_float(panel1Pwr);
                rollcallPkt6_info.rc_eps_gen_6_pnl_3_current_avg = getAvg_float(panel3Current);
                rollcallPkt6_info.rc_eps_gen_6_pnl_3_current_max = getMax_float(panel3Current);
                encoderc_eps_gen_4(&rollcallPkt4_info, &rollcallPkt4);
                encoderc_eps_gen_5(&rollcallPkt5_info, &rollcallPkt5);
                encoderc_eps_gen_6(&rollcallPkt6_info, &rollcallPkt6);
                canSendPacket(&rollcallPkt4);
                canSendPacket(&rollcallPkt5);
                canSendPacket(&rollcallPkt6);
                resetAvg_float(panel1Current);
                resetAvg_float(panel3Voltage);
                resetAvg_float(panel2Current);
                resetAvg_float(panel3Current);
                break;
            case 3:
                rollcallPkt7_info.rc_eps_gen_7_pnl_1_power_avg = getAvg_float(panel1Pwr);
                rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_avg = getAvg_float(panel2Pwr);
                rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_max = getMax_float(panel2Pwr);
                rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_min = getMin_float(panel2Pwr);
                rollcallPkt8_info.rc_eps_gen_8_pnl_1_temp_min = getMin_float(panel1Temp);
                rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_avg = getAvg_float(panel3Pwr);
                rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_max = getMax_float(panel3Pwr);
                rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_min = getMin_float(panel3Pwr);
                rollcallPkt9_info.rc_eps_gen_9_pnl_1_temp_avg = getAvg_float(panel1Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_1_temp_max = getMax_float(panel1Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_avg = getAvg_float(panel2Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_max = getMax_float(panel2Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_min = getMin_float(panel2Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_avg = getAvg_float(panel3Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_max = getMax_float(panel3Temp);
                rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_min = getMin_float(panel3Temp);
                encoderc_eps_gen_7(&rollcallPkt7_info, &rollcallPkt7);
                encoderc_eps_gen_8(&rollcallPkt8_info, &rollcallPkt8);
                encoderc_eps_gen_9(&rollcallPkt9_info, &rollcallPkt9);
                canSendPacket(&rollcallPkt7);
                canSendPacket(&rollcallPkt8);
                canSendPacket(&rollcallPkt9);
                resetAvg_float(panel1Pwr);
                resetAvg_float(panel2Pwr);
                resetAvg_float(panel3Pwr);
                resetAvg_float(panel1Temp);
                resetAvg_float(panel2Temp);
                resetAvg_float(panel3Temp);
                break;
        }
        rcFlag--;
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

    return 0;
}

uint8_t genActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    int i;
    chargecmd_segment *csegment;

    if (mode == Mode_BinaryStreaming)
    {
        // Handle the cmdstr as binary values
        switch (cmdstr[0])
        {
            case OPCODE_CHARGECMD:
                csegment = (chargecmd_segment *) &cmdstr[1];
                for (i = 0; i < NUM_PANELS; i++)
                {
                    if (csegment->enablecharge[i] != CMD_NOCHANGE)
                        genSetPowerTracker(i, csegment->enablecharge[i]);
                }
                break;
        }
    }
    return 1;
}
