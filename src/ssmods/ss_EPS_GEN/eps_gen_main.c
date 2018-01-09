#include <eps_gen.h>
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

FILE_STATIC hDev hTemp1;
FILE_STATIC hDev hTemp2;
FILE_STATIC hDev hTemp3;

FILE_STATIC panel_info panels[NUM_PANELS];
FILE_STATIC uint8_t panelPCVSensorAddresses[] =  { PCVI2C_PANEL1, PCVI2C_PANEL2, PCVI2C_PANEL3 };
FILE_STATIC float panelShuntResistances[] =      { SHUNT_SIDE_PANELS, SHUNT_CENTER_PANEL, SHUNT_SIDE_PANELS };

void genTempSensorsInit()
{
    asensorInit(Ref_2p5V);
    hTemp1 = asensorActivateChannel(CHAN_TEMP1, Type_ExtTempC);
    hTemp2 = asensorActivateChannel(CHAN_TEMP2, Type_ExtTempC);
    hTemp3 = asensorActivateChannel(CHAN_TEMP3, Type_ExtTempC);

    return;
}

void genPCVSensorsInit()
{
    int i;
    for (i = 0; i < NUM_PANELS; ++i)
    {
        panels[i].panelnum = i;
        panels[i].shuntresistance = panelShuntResistances[i];
        panels[i].hpcvsensor = pcvsensorInit(I2CBus2, panelPCVSensorAddresses[i], panelShuntResistances[i], PANEL_CURRENT_LIMIT);
    }
}

void genPanelsInit()
{

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

    genPanelsInit();

    LED_DIR |= LED_BIT;

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_Test, NULL,
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

    while (1)
    {
        // This assumes that some interrupt code will change the value of the triggerStaten variables
        switch (ss_state)
        {
            // TODO:  For now, assume we stay in FirstState all the time; to be changed when we
            // add full state machine for the subsystem.
            case State_FirstState:
                // TODO:  do stuff
                break;
            case State_SecondState:
                // TODO:  Implement full state machine
                break;
            case State_ThirdState:
                // TODO:  Implement full state machine
                break;
            default:
                mod_status.state_transition_errors++;
                mod_status.in_unknown_state++;
                break;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

uint8_t genActionCallback(DebugMode mode, uint8_t * cmdstr)
{
    if (mode == Mode_BinaryStreaming)
    {
        // TODO:  Handle COSMOS commands to do stuff
    }
    return 1;
}
