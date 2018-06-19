#include <eps_gen.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"
#include "core/agglib.h"
#include "interfaces/canwrap.h"
#include "core/MET.h"

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

FILE_STATIC aggVec_f mspTempAg;
FILE_STATIC aggVec_i panel1VoltageAg;
FILE_STATIC aggVec_i panel2VoltageAg;
FILE_STATIC aggVec_i panel3VoltageAg;
FILE_STATIC aggVec_i panel1CurrentAg;
FILE_STATIC aggVec_i panel2CurrentAg;
FILE_STATIC aggVec_i panel3CurrentAg;
FILE_STATIC aggVec_i panel1PwrAg;
FILE_STATIC aggVec_i panel2PwrAg;
FILE_STATIC aggVec_i panel3PwrAg;
FILE_STATIC aggVec_i panel1TempAg;
FILE_STATIC aggVec_i panel2TempAg;
FILE_STATIC aggVec_i panel3TempAg;

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

    asensorUpdateAllSensors();

    pdata = pcvsensorRead(panels[0].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    aggVec_push_i(&panel1VoltageAg, pdata->rawBusVoltage);
    aggVec_push_i(&panel1CurrentAg, pdata->rawCurrent);
    aggVec_push_i(&panel1PwrAg, pdata->rawPower);
    //addData_uint16_t(panel1Temp, asensorGetLastValueV(hTempSensors[0]));

    pdata = pcvsensorRead(panels[1].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    aggVec_push_i(&panel2VoltageAg, pdata->rawBusVoltage);
    aggVec_push_i(&panel2CurrentAg, pdata->rawCurrent);
    aggVec_push_i(&panel2PwrAg, pdata->rawPower);
    //addData_uint16_t(panel2Temp, asensorGetLastValueV(hTempSensors[1]));

    pdata = pcvsensorRead(panels[2].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    aggVec_push_i(&panel3VoltageAg, pdata->rawBusVoltage);
    aggVec_push_i(&panel3CurrentAg, pdata->rawCurrent);
    aggVec_push_i(&panel3PwrAg, pdata->rawPower);
    //addData_uint16_t(panel3Temp, asensorGetLastValueV(hTempSensors[2]));

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
    aggVec_push_f(&mspTempAg, hseg.inttemp);
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
    debugInvokeStatusHandlers();
}

void can_packet_rx_callback(CANPacket *packet)
{
    cmd_rollcall rcPkt;
    gcmd_gen_set_pt_state ptStatePkt;
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            decodecmd_rollcall(packet, &rcPkt);
            updateMET(constructTimestamp(rcPkt.cmd_rollcall_met, rcPkt.cmd_rollcall_met_overflow));
            rcFlag = 10;
            break;
        case CAN_ID_GCMD_GEN_SET_PT_STATE:
            decodegcmd_gen_set_pt_state(packet, &ptStatePkt);
            if(ptStatePkt.gcmd_gen_set_pt_state_1 != CAN_ENUM_NBOOL_NULL)
                genSetPowerTracker(PowerTracker1, ptStatePkt.gcmd_gen_set_pt_state_1);
            if(ptStatePkt.gcmd_gen_set_pt_state_2 != CAN_ENUM_NBOOL_NULL)
                genSetPowerTracker(PowerTracker2, ptStatePkt.gcmd_gen_set_pt_state_2);
            if(ptStatePkt.gcmd_gen_set_pt_state_3 != CAN_ENUM_NBOOL_NULL)
                genSetPowerTracker(PowerTracker3, ptStatePkt.gcmd_gen_set_pt_state_3);
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
            aggVec_reset((aggVec *)&mspTempAg);
            aggVec_reset((aggVec *)&panel1VoltageAg);
            aggVec_reset((aggVec *)&panel2VoltageAg);
            aggVec_reset((aggVec *)&panel3VoltageAg);
            aggVec_reset((aggVec *)&panel1CurrentAg);
            aggVec_reset((aggVec *)&panel2CurrentAg);
            aggVec_reset((aggVec *)&panel3CurrentAg);
            aggVec_reset((aggVec *)&panel1PwrAg);
            aggVec_reset((aggVec *)&panel2PwrAg);
            aggVec_reset((aggVec *)&panel3PwrAg);
            aggVec_reset((aggVec *)&panel1TempAg);
            aggVec_reset((aggVec *)&panel2TempAg);
            aggVec_reset((aggVec *)&panel3TempAg);
            break;
        default:
            break;
    }
}

void sendRC() //TODO: use if'else for each and do rc while once implemented on CAN
{
    while(rcFlag && (canTxCheck() != CAN_TX_BUSY))
    {
        CANPacket rollcallPkt = {0};
        if(rcFlag == 10)
        {
            rc_eps_gen_h1 rollcallPkt1_info = {0};
            rollcallPkt1_info.rc_eps_gen_h1_sysrstiv = SYSRSTIV;
            rollcallPkt1_info.rc_eps_gen_h1_reset_count = bspGetResetCount();
            rollcallPkt1_info.rc_eps_gen_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
            rollcallPkt1_info.rc_eps_gen_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
            rollcallPkt1_info.rc_eps_gen_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
            encoderc_eps_gen_h1(&rollcallPkt1_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&mspTempAg);
        }
        else if(rcFlag == 9)
        {
            rc_eps_gen_h2 rc ={0};
            rc.rc_eps_gen_h2_canrxerror = canRxErrorCheck();
            encoderc_eps_gen_h2(&rc, &rollcallPkt);
        }
        else if(rcFlag == 8)
        {
            rc_eps_gen_2 rollcallPkt2_info = {0};
            rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_avg = aggVec_avg_i_i(&panel1VoltageAg);
            rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_max = aggVec_max_i(&panel1VoltageAg);
            rollcallPkt2_info.rc_eps_gen_2_pnl_1_voltage_min = aggVec_min_i(&panel1VoltageAg);
            rollcallPkt2_info.rc_eps_gen_2_pnl_2_voltage_min = aggVec_min_i(&panel2VoltageAg);
            encoderc_eps_gen_2(&rollcallPkt2_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel1VoltageAg);
        }
        else if(rcFlag == 7)
        {
            rc_eps_gen_3 rollcallPkt3_info = {0};
            rollcallPkt3_info.rc_eps_gen_3_pnl_2_voltage_avg = aggVec_avg_i_i(&panel2VoltageAg);
            rollcallPkt3_info.rc_eps_gen_3_pnl_3_voltage_max = aggVec_max_i(&panel2VoltageAg);
            rollcallPkt3_info.rc_eps_gen_3_pnl_3_voltage_min = aggVec_min_i(&panel2VoltageAg);
            encoderc_eps_gen_3(&rollcallPkt3_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel2VoltageAg);
        }
        else if(rcFlag == 6)
        {
            rc_eps_gen_4 rollcallPkt4_info = {0};
            rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_avg = aggVec_avg_i_i(&panel1CurrentAg);
            rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_max = aggVec_max_i(&panel1CurrentAg);
            rollcallPkt4_info.rc_eps_gen_4_pnl_1_current_min = aggVec_min_i(&panel1CurrentAg);
            rollcallPkt4_info.rc_eps_gen_4_pnl_3_voltage_avg = aggVec_avg_i_i(&panel3VoltageAg);
            encoderc_eps_gen_4(&rollcallPkt4_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel1CurrentAg);
            aggVec_as_reset((aggVec *)&panel3VoltageAg);
        }
        else if(rcFlag == 5)
        {
            rc_eps_gen_5 rollcallPkt5_info = {0};
            rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_avg = aggVec_avg_i_i(&panel2CurrentAg);
            rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_max = aggVec_max_i(&panel2CurrentAg);
            rollcallPkt5_info.rc_eps_gen_5_pnl_2_current_min = aggVec_min_i(&panel2CurrentAg);
            rollcallPkt5_info.rc_eps_gen_5_pnl_3_current_min = aggVec_min_i(&panel3CurrentAg);
            encoderc_eps_gen_5(&rollcallPkt5_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel2CurrentAg);
        }
        else if(rcFlag == 4)
        {
            rc_eps_gen_6 rollcallPkt6_info = {0};
            rollcallPkt6_info.rc_eps_gen_6_pnl_1_power_max = aggVec_max_i(&panel1PwrAg);
            rollcallPkt6_info.rc_eps_gen_6_pnl_1_power_min = aggVec_min_i(&panel1PwrAg);
            rollcallPkt6_info.rc_eps_gen_6_pnl_3_current_avg = aggVec_avg_i_i(&panel3CurrentAg);
            rollcallPkt6_info.rc_eps_gen_6_pnl_3_current_max = aggVec_max_i(&panel3CurrentAg);
            encoderc_eps_gen_6(&rollcallPkt6_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel3CurrentAg);
        }
        else if(rcFlag == 3)
        {
            rc_eps_gen_7 rollcallPkt7_info = {0};
            rollcallPkt7_info.rc_eps_gen_7_pnl_1_power_avg = aggVec_avg_i_i(&panel1PwrAg);
            rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_avg = aggVec_avg_i_i(&panel2PwrAg);
            rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_max = aggVec_max_i(&panel2PwrAg);
            rollcallPkt7_info.rc_eps_gen_7_pnl_2_power_min = aggVec_min_i(&panel2PwrAg);
            encoderc_eps_gen_7(&rollcallPkt7_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel1PwrAg);
            aggVec_as_reset((aggVec *)&panel2PwrAg);
        }
        else if(rcFlag == 2)
        {
            rc_eps_gen_8 rollcallPkt8_info = {0};
            rollcallPkt8_info.rc_eps_gen_8_pnl_1_temp_min = 0;
            rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_avg = aggVec_avg_i_i(&panel3PwrAg);
            rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_max = aggVec_max_i(&panel3PwrAg);
            rollcallPkt8_info.rc_eps_gen_8_pnl_3_power_min = aggVec_min_i(&panel3PwrAg);
            encoderc_eps_gen_8(&rollcallPkt8_info, &rollcallPkt);
            aggVec_as_reset((aggVec *)&panel3PwrAg);
        }
        else if(rcFlag == 1)
        {
            rc_eps_gen_9 rollcallPkt9_info = {0};
            rollcallPkt9_info.rc_eps_gen_9_pnl_1_temp_avg = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_1_temp_max = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_avg = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_max = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_2_temp_min = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_avg = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_max = 0;
            rollcallPkt9_info.rc_eps_gen_9_pnl_3_temp_min = 0;
            encoderc_eps_gen_9(&rollcallPkt9_info, &rollcallPkt);
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
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    //mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>
    bspBackpowerPulldown();

    genTempSensorsInit();
    genPCVSensorsInit();
    genPanelsTrackersInit();

    LED_DIR |= LED_BIT;

    // Setup segments to be able to serve as COSMOS telemetry packets
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&(gseg.header), TLM_ID_EPS_GEN_GENERAL, sizeof(gseg));
    bcbinPopulateHeader(&(sseg.header), TLM_ID_EPS_GEN_SENSORDAT, sizeof(sseg));

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
    aggVec_init_f(&mspTempAg);
    aggVec_init_i(&panel1VoltageAg);
    aggVec_init_i(&panel2VoltageAg);
    aggVec_init_i(&panel3VoltageAg);
    aggVec_init_i(&panel1CurrentAg);
    aggVec_init_i(&panel2CurrentAg);
    aggVec_init_i(&panel3CurrentAg);
    aggVec_init_i(&panel1PwrAg);
    aggVec_init_i(&panel2PwrAg);
    aggVec_init_i(&panel3PwrAg);
    aggVec_init_i(&panel1TempAg);
    aggVec_init_i(&panel2TempAg);
    aggVec_init_i(&panel3TempAg);

    //canWrapInitWithFilter();
    canWrapInit();
    setCANPacketRxCallback(can_packet_rx_callback);
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

        if(rcFlag)
            sendRC();
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
