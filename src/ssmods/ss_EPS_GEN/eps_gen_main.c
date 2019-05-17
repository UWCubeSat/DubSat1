#include <msp430.h> 

#include "eps_gen.h"
#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"
#include "core/agglib.h"
#include "interfaces/canwrap.h"
#include "core/MET.h"
#include "interfaces/rollcall.h"

FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 rcPopulateH1, rcPopulateH2, rcPopulate1, rcPopulate2, rcPopulate3, rcPopulate4, rcPopulate5, rcPopulate6, rcPopulate7, rcPopulate8, rcPopulate9, rcPopulate10
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
    aggVec_push_i(&panel1TempAg, asensorReadSingleSensorV(hTempSensors[0]) * 100);

    pdata = pcvsensorRead(panels[1].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    aggVec_push_i(&panel2VoltageAg, pdata->rawBusVoltage);
    aggVec_push_i(&panel2CurrentAg, pdata->rawCurrent);
    aggVec_push_i(&panel2PwrAg, pdata->rawPower);
    aggVec_push_i(&panel2TempAg, asensorReadSingleSensorV(hTempSensors[1]) * 100);

    pdata = pcvsensorRead(panels[2].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
    aggVec_push_i(&panel3VoltageAg, pdata->rawBusVoltage);
    aggVec_push_i(&panel3CurrentAg, pdata->rawCurrent);
    aggVec_push_i(&panel3PwrAg, pdata->rawPower);
    aggVec_push_i(&panel3TempAg, asensorReadSingleSensorV(hTempSensors[2]) * 100);

    for (i=0; i < NUM_PANELS; i++)
    {
        // Get power/current/bus voltage readings
        pdata = pcvsensorRead(panels[i].hpcvsensor, Read_CurrentA | Read_BusV | Read_PowerW);
        sseg.panelcurrents[i] = pdata->calcdCurrentA;
        sseg.panelpowers[i] = pdata->calcdPowerW;
        sseg.panelvoltages[i] = pdata->busVoltageV;

        // Get temperature readings
        sseg.paneltempsC[i] = asensorReadSingleSensorV(hTempSensors[i]) * 100; //asensorGetLastValueV(hTempSensors[i]);
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

        gseg.ptchargingactual[i] = ( chargingactual != 0 ? FALSE : TRUE);
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
    gcmd_gen_set_pt_state ptStatePkt;
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            rollcallStart();
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
        {
            gcmd_reset_minmax pktRst;
            decodegcmd_reset_minmax(packet, &pktRst);
            if(pktRst.gcmd_reset_minmax_gen)
            {
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
            }
        }
            break;
        case CAN_ID_GCMD_DIST_RESET_MISSION: //clear persistent flags here
            bspClearResetCount();
            break;
        case CAN_ID_GCMD_RESET_I2C:
            i2cReset();
            break;
        default:
            break;
    }
}

void rcPopulateH1(CANPacket *out)
{
    rc_eps_gen_h1 rc = {0};
    rc.rc_eps_gen_h1_sysrstiv = bspGetResetReason();
    rc.rc_eps_gen_h1_reset_count = bspGetResetCount();
    rc.rc_eps_gen_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
    rc.rc_eps_gen_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
    rc.rc_eps_gen_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
    encoderc_eps_gen_h1(&rc, out);
    aggVec_as_reset((aggVec *)&mspTempAg);
}

void rcPopulateH2(CANPacket *out)
{
    rc_eps_gen_h2 rc ={0};
    rc.rc_eps_gen_h2_canrxerror = canRxErrorCheck();
    encoderc_eps_gen_h2(&rc, out);
}

void rcPopulate1(CANPacket *out)
{
    rc_eps_gen_1 rc = {0};
    rc.rc_eps_gen_1_pnl_1_charging = gseg.ptchargingactual[0];//(CHARGING_PTRACKER1_IN & CHARGING_PTRACKER1_BIT != 0);
    rc.rc_eps_gen_1_pnl_1_enabled = gseg.ptchargingenablesw[0];//(DISABLE_PTRACKER1_OUT & DISABLE_PTRACKER1_BIT != 0);
    rc.rc_eps_gen_1_pnl_2_charging = gseg.ptchargingactual[1];//(CHARGING_PTRACKER2_IN & CHARGING_PTRACKER2_BIT != 0);
    rc.rc_eps_gen_1_pnl_2_enabled = gseg.ptchargingenablesw[1];//(DISABLE_PTRACKER2_OUT & DISABLE_PTRACKER2_BIT != 0);
    rc.rc_eps_gen_1_pnl_3_charging = gseg.ptchargingactual[2];//(CHARGING_PTRACKER3_IN & CHARGING_PTRACKER3_BIT != 0);
    rc.rc_eps_gen_1_pnl_3_enabled = gseg.ptchargingenablesw[2];//(DISABLE_PTRACKER3_OUT & DISABLE_PTRACKER3_BIT != 0);
    encoderc_eps_gen_1(&rc, out);
}

void rcPopulate2(CANPacket *out)
{
    rc_eps_gen_2 rc = {0};
    rc.rc_eps_gen_2_pnl_1_voltage_avg = aggVec_avg_i_i(&panel1VoltageAg);
    rc.rc_eps_gen_2_pnl_1_voltage_max = aggVec_max_i(&panel1VoltageAg);
    rc.rc_eps_gen_2_pnl_1_voltage_min = aggVec_min_i(&panel1VoltageAg);
    rc.rc_eps_gen_2_pnl_2_voltage_min = aggVec_min_i(&panel2VoltageAg);
    encoderc_eps_gen_2(&rc, out);
    aggVec_as_reset((aggVec *)&panel1VoltageAg);
}

void rcPopulate3(CANPacket *out)
{
    rc_eps_gen_3 rc = {0};
    rc.rc_eps_gen_3_pnl_2_voltage_avg = aggVec_avg_i_i(&panel2VoltageAg);
    rc.rc_eps_gen_3_pnl_2_voltage_max = aggVec_max_i(&panel2VoltageAg);
    rc.rc_eps_gen_3_pnl_3_voltage_max = aggVec_max_i(&panel2VoltageAg);
    rc.rc_eps_gen_3_pnl_3_voltage_min = aggVec_min_i(&panel2VoltageAg);
    encoderc_eps_gen_3(&rc, out);
    aggVec_as_reset((aggVec *)&panel2VoltageAg);
}

void rcPopulate4(CANPacket *out)
{
    rc_eps_gen_4 rc = {0};
    rc.rc_eps_gen_4_pnl_1_current_avg = aggVec_avg_i_i(&panel1CurrentAg);
    rc.rc_eps_gen_4_pnl_1_current_max = aggVec_max_i(&panel1CurrentAg);
    rc.rc_eps_gen_4_pnl_1_current_min = aggVec_min_i(&panel1CurrentAg);
    rc.rc_eps_gen_4_pnl_3_voltage_avg = aggVec_avg_i_i(&panel3VoltageAg);
    encoderc_eps_gen_4(&rc, out);
    aggVec_as_reset((aggVec *)&panel1CurrentAg);
    aggVec_as_reset((aggVec *)&panel3VoltageAg);
}

void rcPopulate5(CANPacket *out)
{
    rc_eps_gen_5 rc = {0};
    rc.rc_eps_gen_5_pnl_2_current_avg = aggVec_avg_i_i(&panel2CurrentAg);
    rc.rc_eps_gen_5_pnl_2_current_max = aggVec_max_i(&panel2CurrentAg);
    rc.rc_eps_gen_5_pnl_2_current_min = aggVec_min_i(&panel2CurrentAg);
    rc.rc_eps_gen_5_pnl_3_current_min = aggVec_min_i(&panel3CurrentAg);
    encoderc_eps_gen_5(&rc, out);
    aggVec_as_reset((aggVec *)&panel2CurrentAg);
}

void rcPopulate6(CANPacket *out)
{
    rc_eps_gen_6 rc = {0};
    rc.rc_eps_gen_6_pnl_1_power_max = aggVec_max_i(&panel1PwrAg);
    rc.rc_eps_gen_6_pnl_1_power_min = aggVec_min_i(&panel1PwrAg);
    rc.rc_eps_gen_6_pnl_3_current_avg = aggVec_avg_i_i(&panel3CurrentAg);
    rc.rc_eps_gen_6_pnl_3_current_max = aggVec_max_i(&panel3CurrentAg);
    encoderc_eps_gen_6(&rc, out);
    aggVec_as_reset((aggVec *)&panel3CurrentAg);
}

void rcPopulate7(CANPacket *out)
{
    rc_eps_gen_7 rc = {0};
    rc.rc_eps_gen_7_pnl_1_power_avg = 0; //aggVec_avg_i_i(&panel1PwrAg);
    rc.rc_eps_gen_7_pnl_2_power_avg = 0; //aggVec_avg_i_i(&panel2PwrAg);
    rc.rc_eps_gen_7_pnl_2_power_max = aggVec_max_i(&panel2PwrAg);
    rc.rc_eps_gen_7_pnl_2_power_min = aggVec_min_i(&panel2PwrAg);
    encoderc_eps_gen_7(&rc, out);
}

void rcPopulate8(CANPacket *out)
{
    rc_eps_gen_8 rc = {0};
    rc.rc_eps_gen_8_pnl_1_temp_min = aggVec_min_i(&panel1TempAg);
    rc.rc_eps_gen_8_pnl_3_power_avg = 0;
    rc.rc_eps_gen_8_pnl_3_power_max = aggVec_max_i(&panel3PwrAg);
    rc.rc_eps_gen_8_pnl_3_power_min = aggVec_min_i(&panel3PwrAg);
    encoderc_eps_gen_8(&rc, out);
}

void rcPopulate9(CANPacket *out)
{
    rc_eps_gen_9 rc = {0};
    rc.rc_eps_gen_9_pnl_1_temp_avg = aggVec_avg_i_i(&panel1TempAg);
    rc.rc_eps_gen_9_pnl_1_temp_max = aggVec_max_i(&panel1TempAg);
    rc.rc_eps_gen_9_pnl_2_temp_avg = aggVec_avg_i_i(&panel2TempAg);
    rc.rc_eps_gen_9_pnl_2_temp_max = aggVec_max_i(&panel2TempAg);
    rc.rc_eps_gen_9_pnl_2_temp_min = aggVec_min_i(&panel2TempAg);
    rc.rc_eps_gen_9_pnl_3_temp_avg = aggVec_avg_i_i(&panel3TempAg);
    rc.rc_eps_gen_9_pnl_3_temp_max = aggVec_max_i(&panel3TempAg);
    rc.rc_eps_gen_9_pnl_3_temp_min = aggVec_min_i(&panel3TempAg);
    encoderc_eps_gen_9(&rc, out);
    aggVec_as_reset((aggVec *)&panel1TempAg);
    aggVec_as_reset((aggVec *)&panel2TempAg);
    aggVec_as_reset((aggVec *)&panel3TempAg);
}

void rcPopulate10(CANPacket *out)
{
    rc_eps_gen_10 rc = {0};
    rc.rc_eps_gen_10_pnl_1_power_avg = aggVec_avg_i_i(&panel1PwrAg);
    rc.rc_eps_gen_10_pnl_2_power_avg = aggVec_avg_i_i(&panel2PwrAg);
    rc.rc_eps_gen_10_pnl_3_power_avg = aggVec_avg_i_i(&panel3PwrAg);
    encoderc_eps_gen_10(&rc, out);
    aggVec_as_reset((aggVec *)&panel1PwrAg);
    aggVec_as_reset((aggVec *)&panel2PwrAg);
    aggVec_as_reset((aggVec *)&panel3PwrAg);
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

    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));

    canWrapInitWithFilter();
    //canWrapInit();
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
        rollcallUpdate();
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
