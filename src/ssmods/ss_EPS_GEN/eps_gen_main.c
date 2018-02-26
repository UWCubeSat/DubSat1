#include <eps_gen.h>
#include <msp430.h> 

#include "bsp/bsp.h"
#include "sensors/pcvsensor.h"

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



/*
 * main.c
 */
int main(void)
{
    /* ----- INITIALIZATION -----*/
    bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>

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
        if (counter % 16 == 0) genBcSendMeta();
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
