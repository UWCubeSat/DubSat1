#include <msp430.h> 
#include "PPT.h"

#include "bsp/bsp.h"
#include <core/timers.h>
#include "interfaces/canwrap.h"
#include "core/agglib.h"

#define OPCODE_COMMON_CMD 0
#define OPCODE_START_FIRE 2
#define OPCODE_STOP_FIRE 3
#define OPCODE_GET_TIMING 4
#define OPCODE_SET_TIMING 5

#define LED_DIR P1DIR
#define LED_OUT P1OUT
#define LED_BIT_IDLE BIT0
#define LED_BIT_FIRING BIT1
#define CHARGE_DIR P4DIR
#define CHARGE_OUT P4OUT
#define MAIN_CHARGE_BIT BIT3
#define IGN_CHARGE_BIT BIT1
#define SMT_IN P4IN
#define SMT_IN_BIT BIT0
#define MAIN_DONE_BIT BIT6

#define BATTERY_CHARGE_SUFFICIENT_STATE 31000 //TODO: verify this value

//Things that determine whether the PPT can fire:
uint8_t aboveGrndStation = 0;
uint8_t battChargeOK = 0;

//FLAGS:
uint8_t sendRcFlag = 0;

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC volatile ModuleStatus mod_status;

FILE_STATIC uint8_t firing;
FILE_STATIC uint8_t currTimeout;
FILE_STATIC uint8_t withFiringPulse;
FILE_STATIC uint8_t smtOverride;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;

#pragma PERSISTENT(mainChargeTime)
#pragma PERSISTENT(mainIgniterDelay)
#pragma PERSISTENT(igniterChargeTime)
#pragma PERSISTENT(cooldownTime)

FILE_STATIC uint16_t mainChargeTime = 36045;
FILE_STATIC uint16_t mainIgniterDelay = 32;
FILE_STATIC uint16_t igniterChargeTime = 655;
FILE_STATIC uint16_t smtWaitTime = 262; //~8ms
FILE_STATIC uint16_t cooldownTime = 28461;

//MEASURED Values:
#pragma PERSISTENT(lastMainChargeTime)
uint16_t lastMainChargeTime = 0;

//firing flag (used to determine state after reset)
#pragma PERSISTENT(fireAttempt)
volatile uint8_t fireAttempt = 0;

FILE_STATIC ppt_main_done mainDone;
FILE_STATIC meta_segment mseg;
FILE_STATIC health_segment hseg;
FILE_STATIC timing currTiming;
FILE_STATIC fireInfo fireSeg; //ID: 7

aggVec_f mspTempAg;
#pragma PERSISTENT(fireCount)
uint16_t fireCount = 0;
#pragma PERSISTENT(faultCount)
uint16_t faultCount = 0;
#pragma PERSISTENT(lastFireResult)
LastFireResult lastFireResult = 3;//Result_FireSuccessful;

void initData()
{
    aggVec_init_f(&mspTempAg);
}

FILE_STATIC void sendMainDone()
{
	if(mod_status.ss_state == State_Main_Charging)
	{
		mainDone.timeDone = mainChargeTime + TB0R - TB0CCR1;
		lastMainChargeTime = mainDone.timeDone;
		bcbinSendPacket((uint8_t *)&mainDone, sizeof(mainDone));
	}
	else
	{
		mainDone.timeDone = 0;
	}
}

FILE_STATIC void sendMeta()
{
    // TODO:  Add call through debug registrations for INFO on subentities (like the buses)
    bcbinPopulateMeta(&mseg, sizeof(mseg));
    bcbinSendPacket((uint8_t *) &mseg, sizeof(mseg));
}

FILE_STATIC void sendHealth()
{
    // For now, everythingis always marginal ...
    hseg.oms = OMS_Unknown;
    hseg.reset_count = bspGetResetCount();
    hseg.inttemp = asensorReadIntTempC();
    aggVec_push_f(&mspTempAg, hseg.inttemp);
    bcbinSendPacket((uint8_t *) &hseg, sizeof(hseg));
}

FILE_STATIC void sendTiming()
{
    currTiming.mainChargeTime = mainChargeTime;
    currTiming.mainIgniterDelay = mainIgniterDelay;
    currTiming.igniterChargeTime = igniterChargeTime;
    currTiming.cooldownTime = cooldownTime;
    bcbinSendPacket((uint8_t *)&currTiming, sizeof(currTiming));
}

///////////////////////////////////////////////////////////////

void initPins()
{
    LED_DIR |= (LED_BIT_FIRING | LED_BIT_IDLE);
    CHARGE_DIR |= (IGN_CHARGE_BIT| MAIN_CHARGE_BIT);

    //main done interrupt
    P2IFG = 0; //clear the interrupt
    P2REN |= MAIN_DONE_BIT; //enable pullup/down resistor
    P2IES |= MAIN_DONE_BIT; //falling edge capture mode
    P2IE |= MAIN_DONE_BIT; //enable interrupt
}

void checkFireState()
{
    if(fireAttempt)
    {
        if(SMT_IN & SMT_IN_BIT)
        {
            lastFireResult = Result_MainFailedDischarge;
            faultCount++;
        }
        else
        {
            lastFireResult = Result_FireSuccessful;
            fireCount++;
        }
        fireAttempt = 0;
    }
}

void sendRC()
{
    while(sendRcFlag && canTxCheck() != CAN_TX_BUSY)
    {
        CANPacket pkt = {0};
        if(sendRcFlag == 4)
        {
            rc_ppt_h1 rc = {0};
            rc.rc_ppt_h1_reset_count = bspGetResetCount();
            rc.rc_ppt_h1_sysrstiv = bspGetResetReason();
            rc.rc_ppt_h1_temp_avg = compressMSPTemp(aggVec_avg_f(&mspTempAg));
            rc.rc_ppt_h1_temp_max = compressMSPTemp(aggVec_max_f(&mspTempAg));
            rc.rc_ppt_h1_temp_min = compressMSPTemp(aggVec_min_f(&mspTempAg));
            encoderc_ppt_h1(&rc, &pkt);
            aggVec_as_reset((aggVec *)&mspTempAg);
        }
        else if(sendRcFlag == 3)
        {
            rc_ppt_h2 rc = {0};
            rc.rc_ppt_h2_canrxerror = canRxErrorCheck();
            rc.rc_ppt_h2_last_fire_result = lastFireResult;
            encoderc_ppt_h2(&rc, &pkt);
        }
        if(sendRcFlag == 2)
        {
            rc_ppt_1 rc = {0};
            rc.rc_ppt_1_fault_count = faultCount;
            rc.rc_ppt_1_fire_count = fireCount;
            rc.rc_ppt_1_last_main_charge = lastMainChargeTime;
            rc.rc_ppt_1_smt_wait_time = smtWaitTime;
            encoderc_ppt_1(&rc, &pkt);
        }
        else if(sendRcFlag == 1)
        {
            rc_ppt_2 rc = {0};
            rc.rc_ppt_2_cooldown_time = cooldownTime;
            rc.rc_ppt_2_ign_charge_time = igniterChargeTime;
            rc.rc_ppt_2_main_charge_time = mainChargeTime;
            rc.rc_ppt_2_main_ign_delay = mainIgniterDelay;
            encoderc_ppt_2(&rc, &pkt);
        }
        canSendPacket(&pkt);
        sendRcFlag--;
    }
}

void sendFire()
{
    fireSeg.faultCount = faultCount;
    fireSeg.fireCount = fireCount;
    fireSeg.lastFireResult = (uint8_t)lastFireResult;
    bcbinSendPacket((uint8_t *)&fireSeg, sizeof(fireSeg));
}

void blinkLED()
{
    if(firing)
        LED_OUT ^= LED_BIT_FIRING;
    else
        LED_OUT ^= LED_BIT_IDLE;
}

void startFiring(uint8_t timeout);
void stopFiring();
void mainLow();
void igniterHigh();
void fire();
void can_packet_rx_callback(CANPacket *packet);

/*
 * main.c
 */
int main(void)
{

    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code, as
    // it sets up critical hardware settings for board specified by the __BSP_Board... definition used.
    // If module not yet available in enum, add to SubsystemModule enumeration AND
    // SubsystemModulePaths (a string name) in systeminfo.c/.h
    //bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    bspInit(Module_PPT);
    asensorInit(Ref_2p5V);
    canWrapInitWithFilter();
    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up sync pulse handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.ss_mode = Mode_Undetermined;
    mod_status.ss_state = State_Uncommissioned;

    canWrapInitWithFilter();
    setCANPacketRxCallback(can_packet_rx_callback);

    initPins();
    initData();
    checkFireState();

    TB0CTL = TBSSEL__ACLK | MC__CONTINOUS | TBCLR | TBIE;

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, NULL,
                                          NULL,
                                          handleDebugActionCallback);

    //hBus handle = uartInit(BackchannelUART, 1, DEBUG_UART_SPEED);
    //uartRegisterRxCallback(handle, handleDebugActionCallback); //debugReadCallback

#endif  //  __DEBUG__

    /* ----- CAN BUS/MESSAGE CONFIG -----*/
    // TODO:  Add the correct bus filters and register CAN message receive handlers

    debugTraceF(1, "CAN message bus configured.\r\n");

    /* ----- SUBSY\STEM LOGIC -----*/
    // TODO:  Finally ... NOW, implement the actual subsystem logic!
    // In general, follow the demonstrated coding pattern, where action flags are set in interrupt handlers,
    // and then control is returned to this main loop

    debugTraceF(1, "Commencing subsystem module execution ...\r\n");

    uint32_t counter = 0;

    bcbinPopulateHeader(&currTiming.header, 5, sizeof(currTiming));
    bcbinPopulateHeader(&(hseg.header), TLM_ID_SHARED_HEALTH, sizeof(hseg));
    bcbinPopulateHeader(&mainDone.header, 3, sizeof(mainDone));
    bcbinPopulateHeader(&fireSeg.header, 6, sizeof(fireSeg));

    withFiringPulse = 1;
    smtOverride = 0;

    while (1)
    {
        __delay_cycles(SEC * 0.1);
        if (!(counter % 8))
        {
            blinkLED();
        }
        if (!(counter % 16))
        {
            sendMeta();
            sendHealth();
            sendFire();
        }
        sendRC();
        counter++;
    }
}

void startFiring(uint8_t timeout)
{
    if(!firing && timeout)
    {
        LED_OUT &= ~LED_BIT_IDLE;
        currTimeout = timeout - 1;
        mod_status.ss_state = State_Cooldown;
        TB0CCR1 = TB0R + cooldownTime;
        TB0CCTL1 = CCIE;
        firing = 1;
    }
}

void stopFiring()
{
    if(firing)
    {
        TB0CCTL1 &= ~CCIE;
        LED_OUT &= ~LED_BIT_FIRING;
        mod_status.ss_state = State_Uncommissioned;
        firing = 0;
        CHARGE_OUT &= ~(MAIN_CHARGE_BIT | IGN_CHARGE_BIT);
        withFiringPulse = 1;
        smtOverride = 0;
    }
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr) //this should be the one that gets called when a cmd comes in
{
    if (mode == Mode_BinaryStreaming)
    {
        //this is the one
        switch(cmdstr[0])
        {
            case OPCODE_COMMON_CMD:
                break;
            case OPCODE_START_FIRE://OPCODE_MY_CMD:
                startFiring(((start_firing *) (cmdstr + 1))->timeout);
                break;
            case OPCODE_STOP_FIRE:
                stopFiring();
                break;
            case OPCODE_GET_TIMING:
                sendTiming();
                break;
            case OPCODE_SET_TIMING:
            {
                set_timing *fireTiming = (set_timing *)(cmdstr + 1);
                if(fireTiming->mainChargeTime)
                    mainChargeTime = fireTiming->mainChargeTime;
                if(fireTiming->mainIgniterDelay)
                    mainIgniterDelay = fireTiming->mainIgniterDelay;
                if(fireTiming->igniterChargeTime)
                    igniterChargeTime = fireTiming->igniterChargeTime;
                if(fireTiming->cooldownTime)
                    cooldownTime = fireTiming->cooldownTime;
                sendTiming();
                break;
            }
            default:
                break;
        }

        // handle actions, any output should be ground-segment friendly
        // "packet" format
    }
    return 1;
}

void can_packet_rx_callback(CANPacket *packet)
{
    gcmd_ppt_halt pktHalt = {0};
    cmd_ppt_time_upd pktTime = {0};
    cmd_ppt_single_fire pktFireSingle = {0};
    rc_adcs_estim_8 pktEstim = {0};
    rc_eps_batt_7 pktBatt = {0};
    gcmd_ppt_multiple_fire pktMultFire = {0};
    switch(packet->id)
    {
        case CAN_ID_CMD_ROLLCALL:
            sendRcFlag = 5;
            break;
        case CAN_ID_GCMD_PPT_HALT:
            //stop firing, but with a flag
            decodegcmd_ppt_halt(packet, &pktHalt);
            if(pktHalt.gcmd_ppt_halt_confirm)
                stopFiring();
            break;
        case CAN_ID_CMD_PPT_SINGLE_FIRE:
            //fire once, with flags for pulse and override
            decodecmd_ppt_single_fire(packet, &pktFireSingle);
            if (pktFireSingle.cmd_ppt_single_fire_override || readyToFire())
            {
                withFiringPulse = pktFireSingle.cmd_ppt_single_fire_with_pulse;
                smtOverride = pktFireSingle.cmd_ppt_single_fire_override_smt;
                startFiring(1);
            }
            break;
        case CAN_ID_CMD_PPT_TIME_UPD:
            //updates all times
            decodecmd_ppt_time_upd(packet, &pktTime);
            if(pktTime.cmd_ppt_time_upd_charge)
                mainChargeTime = pktTime.cmd_ppt_time_upd_charge;
            if(pktTime.cmd_ppt_time_upd_ign_delay)
                mainIgniterDelay = pktTime.cmd_ppt_time_upd_ign_delay;
            if(pktTime.cmd_ppt_time_upd_ign_charge)
                igniterChargeTime = pktTime.cmd_ppt_time_upd_ign_charge;
            if(pktTime.cmd_ppt_time_upd_cooldown)
                cooldownTime = pktTime.cmd_ppt_time_upd_cooldown;
            break;
        case CAN_ID_RC_ADCS_ESTIM_8:
            decoderc_adcs_estim_8(packet, &pktEstim);
            aboveGrndStation = pktEstim.rc_adcs_estim_8_sc_above_gs;
            break;
        case CAN_ID_RC_EPS_BATT_7:
            decoderc_eps_batt_7(packet, &pktBatt);
            //determine whether the charge is in an acceptable range
            battChargeOK = pktBatt.rc_eps_batt_7_acc_charge_avg > BATTERY_CHARGE_SUFFICIENT_STATE;
            break;
        case CAN_ID_GCMD_RESET_MINMAX:
        {
            gcmd_reset_minmax pktRst;
            decodegcmd_reset_minmax(packet, &pktRst);
            if(pktRst.gcmd_reset_minmax_ppt)
            {
                aggVec_reset((aggVec *)&mspTempAg);
            }
        }
            break;
        case CAN_ID_GCMD_PPT_MULTIPLE_FIRE:
            decodegcmd_ppt_multiple_fire(packet, &pktMultFire);
            if(pktMultFire.gcmd_ppt_multiple_fire_override || readyToFire())
                startFiring(pktMultFire.gcmd_ppt_multiple_fire_count);
            break;
        case CAN_ID_GCMD_DIST_RESET_MISSION: //clear persistent flags here
            bspClearResetCount();
            break;
    }
}

BOOL readyToFire() //okay to fire
{
    return aboveGrndStation & battChargeOK;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch(P2IV)
    {
        case P2IV__P2IFG6:
        	sendMainDone();
        	break;
        default:
            break;
    }
    P2IFG = 0; //clear the interrupt flag
}

void fire()
{
    fireAttempt = 1;
    CHARGE_OUT |= IGN_CHARGE_BIT;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void)
{
    switch(TB0IV)
    {
        case TBIV__NONE: break;
        case TBIV1:
            switch(mod_status.ss_state)
            {
                case State_Main_Charging:
                    CHARGE_OUT &= ~MAIN_CHARGE_BIT;
                    TB0CCR1 += mainIgniterDelay;
                    mod_status.ss_state = State_Main_Igniter_Cooldown;
                    break;
                case State_Main_Igniter_Cooldown:
                    mod_status.ss_state = State_Igniter_Charging;
                    TB0CCR1 += igniterChargeTime;
                    if(smtOverride || SMT_IN & SMT_IN_BIT) //smt trigger high
                	{
                		if(withFiringPulse)
                			fire();
                		else
                            stopFiring();
                	}
                	else //fault: main didn't charge
                	{
                		faultCount++;
                		lastFireResult = Result_MainFailedCharge;
                		stopFiring();
                	}
                    break;
                case State_Igniter_Charging:
                    CHARGE_OUT &= ~IGN_CHARGE_BIT; //set igniter low
                    fireAttempt = 0;
                    mod_status.ss_state = State_SMT_Wait;
                    TB0CCR1 += smtWaitTime;
                    break;
                case State_SMT_Wait:
                    if(smtOverride == 0 && SMT_IN & SMT_IN_BIT) //fault: main didn't discharge
                    {
                        stopFiring();
                        lastFireResult = Result_MainFailedDischarge;
                        faultCount++;
                    }
                    else
                    {
                        lastFireResult = Result_FireSuccessful;
                        fireCount++;
                        if(currTimeout)
                        {
                            currTimeout--;
                            mod_status.ss_state = State_Cooldown;
                            TB0CCR1 += cooldownTime;
                        }
                        else
                            stopFiring();
                    }
                    break;
                case State_Cooldown:
                    CHARGE_OUT |= MAIN_CHARGE_BIT;
                    mod_status.ss_state = State_Main_Charging;
                    TB0CCR1 += mainChargeTime;
                default:
                    break;
            }
            break;
        default: break;
    }
}
