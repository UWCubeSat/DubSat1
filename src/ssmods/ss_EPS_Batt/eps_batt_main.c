#include <eps_batt.h>
#include <msp430.h>
#include "bsp/bsp.h"

#include "sensors/pcvsensor.h"       //INA219
#include "sensors/coulomb_counter.h" //Coulomb counter
#include "core/i2c.h"

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

// Telemetry and commands
FILE_STATIC meta_segment mseg;
FILE_STATIC general_segment gseg;
FILE_STATIC sensordat_segment sseg;
FILE_STATIC health_segment hseg;

//Sensor data
FILE_STATIC PCVSensorData *INAData;
FILE_STATIC CoulombCounterData CCData;
FILE_STATIC hDev hTempC;

FILE_STATIC volatile uint8_t isChecking;

FILE_STATIC volatile int autoHeating = 1;

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
                isChecking = ((setCheckState_segment *) &cmdstr[1])->isChecking;
                break;
        }
    }
    return 1;
}

// Packetizes and sends backchannel GENERAL packet
FILE_STATIC void battBcSendGeneral()
{
    gseg.isChecking = isChecking;
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
}

void readINA(hDev hSensor){
    //Reading INA219
    INAData = pcvsensorRead(hSensor, Read_BusV | Read_CurrentA);
    sseg.battNodeVolt = INAData->busVoltageV;
    sseg.battNodeCurr = INAData->calcdCurrentA;
}

void readTempSensor(){
    float tempVoltageV =  asensorReadSingleSensorV(hTempC); //Assuming in V (not mV)
    //sseg.battTemp = ((tempVoltageV/100.0) - 500) / 10.0; //Temp in Celcius
    //conversion from voltage to temperatur, has a slope of 10mv per degree celcius
    //and an intercept of 50 degrees at 0 volts
    sseg.battTemp = (tempVoltageV /0.010) - 50;
}



/*
 * main.c
 */



int main(void)
 {
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

            if(isChecking)
            {
                float temp = asensorReadIntTempC();
                if(!(BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) && (temp < 0.5)) //not heating & < 0C
                    BATTERY_BALANCER_ENABLE_OUT |= BATTERY_BALANCER_ENABLE_BIT; //turn on

                else if ((BATTERY_BALANCER_ENABLE_OUT & BATTERY_BALANCER_ENABLE_BIT) && (temp > 0.6)) //heating & > 10C
                    BATTERY_BALANCER_ENABLE_OUT &= ~BATTERY_BALANCER_ENABLE_BIT; //turn off
            }
        }

        // Stuff running 4Hz
        if (counter % 32 ==0)
        {
            battBcSendMeta();
        }
     }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

    return 0;
}
