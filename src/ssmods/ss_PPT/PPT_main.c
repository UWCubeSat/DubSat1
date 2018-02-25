#include <msp430.h> 
#include "PPT.h"

#include "bsp/bsp.h"

/*
 * Output Pins:
 * 4.3 - Main
 * 4.2 - Igniter
 * 4.1 - Firing Pulse
 *
 * Input Pins:
 * 2.5 - Igniter Done
 * 2.6 - Main Done
 */


#define OPCODE_START_FIRE 2
#define OPCODE_STOP_FIRE 3

#define DEFAULT_FIRE_RATE 1
#define DEFAULT_TIMEOUT 100 //TODO: change these values

// Main status (a structure) and state and mode variables
// Make sure state and mode variables are declared as volatile
FILE_STATIC ModuleStatus mod_status; //TODO: make this volatile?

FILE_STATIC uint8_t firing;
FILE_STATIC uint8_t fireRate;
FILE_STATIC uint8_t currFireCount;
FILE_STATIC uint8_t currTimeout;

// These are sample "trigger" flags, used to indicate to the main loop
// that a transition should occur
FILE_STATIC flag_t triggerState1;
FILE_STATIC flag_t triggerState2;
FILE_STATIC flag_t triggerState3;

CMD_SEGMENT {
    uint8_t rate;
    uint8_t timeout;
} start_firing;

CMD_SEGMENT {
} stop_firing;

TLM_SEGMENT {
    BcTlmHeader header;
} ppt_operating;

TLM_SEGMENT {
    BcTlmHeader header;
    uint16_t timeDone;
} ppt_main_done;

TLM_SEGMENT {
    BcTlmHeader header;
    uint16_t timeDone;
} ppt_igniter_done;

FILE_STATIC ppt_main_done mainDone;
FILE_STATIC ppt_igniter_done igniterDone;
const uint32_t ledFreq = 200000;

FILE_STATIC void sendMainDone()
{
    mainDone.timeDone = TB0R;
    bcbinSendPacket((uint8_t *) &mainDone, sizeof(mainDone));
}
FILE_STATIC void sendIgniterDone()
{
    igniterDone.timeDone = TB0R;
    bcbinSendPacket((uint8_t *) &igniterDone, sizeof(igniterDone));
}

void startFiring(start_firing *startPkt);
void stopFiring();
void enableInterrupts();
void disableInterrupts();

/*
 * main.c
 */
int main(void)
{

    /* ----- INITIALIZATION -----*/
    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code, as
    // it sets up critical hardware settings for board specified by the __BSP_Board... defintion used.
    // If module not yet available in enum, add to SubsystemModule enumeration AND
    // SubsystemModulePaths (a string name) in systeminfo.c/.h
    //bspInit(__SUBSYSTEM_MODULE__);  // <<DO NOT DELETE or MOVE>>
    bspInit(Module_PPT);

    // This function sets up critical SOFTWARE, including "rehydrating" the controller as close to the
    // previous running state as possible (e.g. 1st reboot vs. power-up mid-mission).
    // Also hooks up sync pulse handlers.  Note that actual pulse interrupt handlers will update the
    // firing state structures before calling the provided handler function pointers.
    mod_status.startup_type = coreStartup(handleSyncPulse1, handleSyncPulse2);  // <<DO NOT DELETE or MOVE>>
    mod_status.ss_mode = Mode_Undetermined;
    mod_status.ss_state = State_Uncommissioned;

    P1DIR |= (BIT0 | BIT1);
    P2DIR &= ~(BIT5 | BIT6);
    P4DIR |= (BIT1 | BIT2 | BIT3);

    P1OUT = 0;
    P2OUT = 0;
    P4OUT = 0;

    P2IFG = 0; //clear the interrupt
    P2REN |= (BIT5 | BIT6);
    P2IES |= BIT1; //this represents capture mode (rising/falling/both)

    firing = 0;

    TB0CCR1 = 39322;
    TB0CCR2 = 39354;
    TB0CCR3 = 42632; //was 42621
    disableInterrupts();

    TB0CTL = TBSSEL__ACLK | MC__CONTINOUS | TBCLR | TBIE;

#if defined(__DEBUG__)
    // Insert debug-build-only things here, like status/info/command handlers for the debug
    // console, etc.  If an Entity_<module> enum value doesn't exist yet, please add in
    // debugtools.h.  Also, be sure to change the "path char"
    debugRegisterEntity(Entity_SUBSYSTEM, handleDebugInfoCallback,
                                          handleDebugStatusCallback,
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

    uint32_t ledCount = ledFreq;

    ppt_operating pptOp;

    bcbinPopulateHeader(&pptOp.header, 2, sizeof(pptOp)); // 2nd param is opcode
    bcbinPopulateHeader(&mainDone.header, 3, sizeof(mainDone));
    bcbinPopulateHeader(&igniterDone.header, 4, sizeof(igniterDone));

    bcbinSendPacket((uint8_t *) &pptOp, sizeof(pptOp));

    while (1)
    {
        switch(mod_status.ss_state)
        {
        case State_Uncommissioned:
            break;
        case State_Main_Charging:
            //check if main is done charging, then send telem
            break;
        case State_Igniter_Charging:
            //same as main charging
            break;
        case State_Firing:
            break;
        case State_Cooldown:
            break;
        case State_InitializingFire:
            break;
        default:
            mod_status.ss_state = State_Uncommissioned;
            break;
        }
        if(ledCount <= 0)
        {
            //this is the blinky light routine
            if(firing)
                P1OUT ^= BIT1;
            else
                P1OUT ^= BIT0;
            ledCount = ledFreq;
        }
        else
        {
            ledCount--;
        }
    }

    // NO CODE SHOULD BE PLACED AFTER EXIT OF while(1) LOOP!

	return 0;
}

void startFiring(start_firing *startPkt)
{
    if(!firing)
    {
        P1OUT &= ~BIT0;
        firing = 1;

        currTimeout = startPkt->timeout;// - 1;
        fireRate = startPkt->rate - 1;
        currFireCount = 0; //fireRate;
        mod_status.ss_state = State_InitializingFire;

        TB0CCTL3 |= CCIE;
        //enableInterrupts();
    }
}

void stopFiring()
{
    if(firing)
    {
        P1OUT &= ~BIT1;
        firing = 0;
        disableInterrupts();
        P4OUT &= ~(BIT1 | BIT2 | BIT3);
    }
}

/* ----- SYNC PULSE INTERRUPT HANDLERS ----- */
// Both of these handlers are INTERRUPT HANDLERS, and run as such, which means that all OTHER
// interrupts are blocked while they are running.  This can cause all sorts of issues, so
// MAKE SURE TO MINIMIZE THE CODE RUNNING IN THESE FUNCTIONS.
// Sync pulse 1:  typically raised every 2 seconds while PPT firing, to help each subsystem module
// do the "correct thing" around the firing sequence.  This timing might
// not be exact, and may even change - don't rely on it being 2 seconds every time, and it may
// be shut off entirely during early or late stages of mission, so also do NOT use as a "heartbeat"
// for other, unrelated functionality.
//
// FOR PPT:  Note that sync pulse 1 is SOURCED from the PPT, but for code cleanliness it might still make
// sense to handle certain other things here.
void handleSyncPulse1()
{
    __no_operation();
}

// Sync pulse 2:  typically every 1-2 minutes, but again, don't count on any length.
// General semanatics are that this pulse means all subsystems should share accumulated
// status data on the CAN bus.  It is also the cue for the PPT to ascertain whether it
// will use the following period as an active or suspended firing period.  All subsystems
// will assume active until they are notified that firing has been suspended, but
// this determination will be reset (back to active) at each sync pulse 2.
void handleSyncPulse2()
{
    __no_operation();
}

// Optional callback for the debug system.  "Info" is considered static information
// that doesn't change about the subsystem module code/executable, so this is most
// often left off.
uint8_t handleDebugInfoCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // debugPrintF information in a user-friendly, formatted way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // debugPrintF information without field names, as CSV
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // debugPrintF into a ground segment-friendly "packet" mode
    }
    return 1;
}

// Optional callback for the debug system.  "Status" is considered the
// current state of dynamic information about the subsystem module, and is the most
// common to be surfaced, particularly as "streaming telemetry".
uint8_t handleDebugStatusCallback(DebugMode mode)
{
    if (mode == Mode_ASCIIInteractive)
    {
        // debugPrintF status in a user-friendly, formatted way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // debugPrintF status without field names, as CSV
    }
    else if (mode == Mode_BinaryStreaming)
    {
        // debugPrintF status a ground segment-friendly "packet" format
    }
    return 1;
}

uint8_t handleDebugActionCallback(DebugMode mode, uint8_t * cmdstr) //this should be the one that gets called when a cmd comes in
{
    if (mode == Mode_ASCIIInteractive)
    {
        // handle actions in a user-friendly way
    }
    else if (mode == Mode_ASCIIHeadless)
    {
        // handle actions in a low-output way
    }
    else if (mode == Mode_BinaryStreaming)
    {
        //this is the one
        switch(cmdstr[0])
        {
            case OPCODE_START_FIRE://OPCODE_MY_CMD:
                startFiring((start_firing *) (cmdstr + 1));
                break;
            case OPCODE_STOP_FIRE:
                stopFiring();
                break;
            default:
                break;
        }

        // handle actions, any output should be ground-segment friendly
        // "packet" format
    }
    return 1;
}

BOOL readyToFire()
{
    // TODO:  Walk through all of the data items that are necessary to determine if it's
    // time to go into a firing sequence
    return 0;
}

void enableInterrupts()
{
    TB0CCTL1 = CCIE;
    TB0CCTL2 = CCIE;
    TB0CCTL3 = CCIE;
    P2IE = (BIT5 | BIT6);
}

void disableInterrupts()
{
    TB0CCTL1 &= ~CCIE;
    TB0CCTL2 &= ~CCIE;
    TB0CCTL3 &= ~CCIE;
    P2IE &= ~(BIT5 | BIT6);
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch(P2IV)
    {
        case P2IV__P2IFG5:
            sendIgniterDone();
            break;
        case P2IV__P2IFG6:
            sendMainDone();
            break;
        default:
            break;
    }
    P2IFG = 0; //clear the interrupt flag
}


#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void)
{
    switch(TB0IV)
    {
        case TBIV__NONE: break;
        case TBIV1:
            //set main charge to low
            P4OUT &= ~BIT3;
            mod_status.ss_state = State_Igniter_Charging;
            break;
        case TBIV2:
            //set igniter charge to high
            P4OUT |= BIT2;
            break;
        case TBIV3:
        case TBIV__TBCCR3:
            if(mod_status.ss_state != State_InitializingFire)
            {
                //set igniter charge to low
                P4OUT &= ~BIT2;
                mod_status.ss_state = State_Firing;
                //fire high
                P4OUT |= BIT1;
                __delay_cycles(795); //was 7964 for 1 ms (target is 100 us)
                //fire low
                P4OUT &= ~BIT1;
                if(currTimeout)
                    currTimeout--;
                else
                    stopFiring();
                disableInterrupts();
            }
            mod_status.ss_state = State_Cooldown;
            //TODO: send sync pulse here
            break;
        case TBIV4: break;
        case TBIV5: break;
        case TBIV6: break;
        case TBIV__TBIFG: //overflow case
            if(firing)
            {
                if(currFireCount)
                {
                    currFireCount--;
                    //disableInterrupts();

                }
                else if(mod_status.ss_state != State_InitializingFire)//currFireCount is zero && not the fake fire
                {
                    enableInterrupts();
                    currFireCount = fireRate;
                    //set main to high
                    P4OUT |= BIT3;
                    mod_status.ss_state = State_Main_Charging;
                }
            }
            break;
        default: break;
    }
}
