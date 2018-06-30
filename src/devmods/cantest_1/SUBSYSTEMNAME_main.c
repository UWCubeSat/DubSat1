#include <msp430.h> 

#include "SUBSYSTEMNAME_MODULENAME.h"
#include "bsp/bsp.h"
#include "core/can.h"
#include "interfaces/canwrap.h"
#include "core/agglib.h"
#include "interfaces/rollcall.h"

// Directives for timer stuff.
#include "core/timers.h"
#define FREQ_TIMER(reg)         TA4##reg
#define FREQ_ROOT_TIMER(bits)   TA##bits
FILE_STATIC uint32_t overflowCount = 0;


#define BOARD_NUM (0x05)

#define LED_BLUE (0x01)
#define LED_RED (0x04)
#define LED_YELLOW (0x02)

/*
 * main.c
 */
// Send back the same reply
void reverseArrah(uint8_t arr[], uint8_t start, uint8_t end) {
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArrah(arr, start+1, end-1);
}

uint64_t sentCount=0;

void bdotrcPopulateH1(CANPacket *out)
{
    rc_adcs_bdot_2 rc ;
    rc.rc_adcs_bdot_2_mag_x_min = 0;
    rc.rc_adcs_bdot_2_mag_x_max = 0;
    rc.rc_adcs_bdot_2_mag_x_avg = 0;
    rc.rc_adcs_bdot_2_mag_y_min = 0;
    encoderc_adcs_bdot_2(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void bdotrcPopulateH2(CANPacket *out)
{
    rc_adcs_bdot_h2 rc;
    rc.rc_adcs_bdot_h2_canrxerror = 0;
    encoderc_adcs_bdot_h2(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void bdotrcPopulate1(CANPacket *out)
{
    rc_adcs_bdot_1 rc;
    rc.rc_adcs_bdot_1_last_spam_x_mtq_x = 0;
    rc.rc_adcs_bdot_1_last_spam_x_mtq_y = 0;
    rc.rc_adcs_bdot_1_last_spam_x_mtq_z = 0;
    rc.rc_adcs_bdot_1_last_spam_y_mtq_x = 0;
    encoderc_adcs_bdot_1(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}
void bdotrcPopulate2(CANPacket *out)
{
    rc_adcs_bdot_2 rc ;
      rc.rc_adcs_bdot_2_mag_x_min = 0;
      rc.rc_adcs_bdot_2_mag_x_max = 0;
      rc.rc_adcs_bdot_2_mag_x_avg = 0;
      rc.rc_adcs_bdot_2_mag_y_min = 0;
      encoderc_adcs_bdot_2(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void bdotrcPopulate3(CANPacket *out)
{
    rc_adcs_bdot_3 rc = {0};
      rc.rc_adcs_bdot_3_mag_y_max = 0;
      rc.rc_adcs_bdot_3_mag_y_avg = 0;
      rc.rc_adcs_bdot_3_mag_z_min = 0;
      rc.rc_adcs_bdot_3_mag_z_max = 0;
      encoderc_adcs_bdot_3(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}


void bdotrcPopulate4(CANPacket *out)
{
    rc_adcs_bdot_4 rc = {0};
      rc.rc_adcs_bdot_4_mag_z_avg = 0;
      rc.rc_adcs_bdot_4_tumble = 0;
      rc.rc_adcs_bdot_4_last_spam_y_mtq_y = 0;
      rc.rc_adcs_bdot_4_last_spam_y_mtq_z = 0;
      encoderc_adcs_bdot_4(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}

void bdotrcPopulate5(CANPacket *out)
{
    rc_adcs_bdot_5 rc = {0};
    rc.rc_adcs_bdot_5_last_spam_z_mtq_x = 0;
    rc.rc_adcs_bdot_5_last_spam_z_mtq_y = 0;
    rc.rc_adcs_bdot_5_last_spam_z_mtq_z = 0;
    encoderc_adcs_bdot_5(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void mtqrcPopulate1(CANPacket *out){
    rc_adcs_mtq_h1 rc = {0};
       rc.rc_adcs_mtq_h1_sysrstiv = 0;
       rc.rc_adcs_mtq_h1_reset_count = 0;
       rc.rc_adcs_mtq_h1_temp_avg = 0;
       rc.rc_adcs_mtq_h1_temp_max = 0;
       rc.rc_adcs_mtq_h1_temp_min = 0;
       encoderc_adcs_mtq_h1(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void mtqrcPopulate0(CANPacket *out){
    rc_adcs_mtq_h2 rc;
       rc.rc_adcs_mtq_h2_canrxerror = 0;
       encoderc_adcs_mtq_h2(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

// TODO: description
void mtqrcPopulate2(CANPacket *out){
    rc_adcs_mtq_2 rc = {0};
       rc.rc_adcs_mtq_2_bdot_x_min = 0;
       rc.rc_adcs_mtq_2_bdot_x_max = 0;
       rc.rc_adcs_mtq_2_bdot_x_avg = 0;
       rc.rc_adcs_mtq_2_bdot_y_min = 0;
       rc.rc_adcs_mtq_2_bdot_y_max = 0;
       rc.rc_adcs_mtq_2_bdot_y_avg = 0;
       rc.rc_adcs_mtq_2_bdot_z_max = 0;
       rc.rc_adcs_mtq_2_bdot_z_avg = 0;
       encoderc_adcs_mtq_2(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);


}

// TODO: description
void mtqrcPopulate3(CANPacket *out){
    rc_adcs_mtq_3 rc = {0};
        rc.rc_adcs_mtq_3_bdot_z_min = 0;
        rc.rc_adcs_mtq_3_fsw_x_min = 0;
        rc.rc_adcs_mtq_3_fsw_x_max = 0;
        rc.rc_adcs_mtq_3_fsw_x_avg = 0;
        rc.rc_adcs_mtq_3_fsw_y_min = 0;
        rc.rc_adcs_mtq_3_fsw_y_max = 0;
        rc.rc_adcs_mtq_3_fsw_y_avg = 0;
        rc.rc_adcs_mtq_3_fsw_z_avg = 0;
        encoderc_adcs_mtq_3(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}

// TODO: description
void mtqrcPopulate4(CANPacket *out){
    rc_adcs_mtq_4 rc = {0};
       rc.rc_adcs_mtq_4_fsw_z_min = 0;
       rc.rc_adcs_mtq_4_fsw_y_max = 0;
       rc.rc_adcs_mtq_4_duty_x1_avg = 0;
       rc.rc_adcs_mtq_4_duty_x2_avg = 0;
       rc.rc_adcs_mtq_4_duty_y1_avg = 0;
       rc.rc_adcs_mtq_4_duty_y2_avg = 0;
       rc.rc_adcs_mtq_4_duty_z1_avg = 0;
       rc.rc_adcs_mtq_4_duty_z2_avg = 0;
       encoderc_adcs_mtq_4(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

// TODO: description
void mtqrcPopulate5(CANPacket *out){
    rc_adcs_mtq_5 rc = {0};
      rc.rc_adcs_mtq_5_fsw_ignore = 0;
      rc.rc_adcs_mtq_5_reset_counts = 0;
      encoderc_adcs_mtq_5(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}

void sprcPopulate1(CANPacket *out)
{
    rc_adcs_sp_h1 rc;
       rc.rc_adcs_sp_h1_reset_count = 0;
       rc.rc_adcs_sp_h1_sysrstiv = 0;
       rc.rc_adcs_sp_h1_temp_avg = 0;
       rc.rc_adcs_sp_h1_temp_max = 0;
       rc.rc_adcs_sp_h1_temp_min = 0;
       encoderc_adcs_sp_h1(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate2(CANPacket *out)
{
    rc_adcs_sp_2 rc;
       encoderc_adcs_sp_2(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate3(CANPacket *out)
{
    rc_adcs_sp_3 rc;
     encoderc_adcs_sp_3(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate4(CANPacket *out)
{
    rc_adcs_sp_4 rc;
     encoderc_adcs_sp_4(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate5(CANPacket *out)
{

    rc_adcs_sp_5 rc;
     encoderc_adcs_sp_5(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate6(CANPacket *out)
{

    rc_adcs_sp_6 rc;
     encoderc_adcs_sp_6(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate9(CANPacket *out)
{

    rc_adcs_sp_9 rc;
     encoderc_adcs_sp_9(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate10(CANPacket *out)
{

    rc_adcs_sp_10 rc;
     encoderc_adcs_sp_10(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate11(CANPacket *out)
{

    rc_adcs_sp_11 rc;
     encoderc_adcs_sp_11(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate12(CANPacket *out)
{

    rc_adcs_sp_12 rc;
     encoderc_adcs_sp_12(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate13(CANPacket *out)
{

    rc_adcs_sp_13 rc;
     encoderc_adcs_sp_13(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate14(CANPacket *out)
{

    rc_adcs_sp_14 rc;
     encoderc_adcs_sp_14(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate15(CANPacket *out)
{

    rc_adcs_sp_15 rc;
     encoderc_adcs_sp_15(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate16(CANPacket *out)
{

    rc_adcs_sp_16 rc;
     encoderc_adcs_sp_16(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void sprcPopulate17(CANPacket *out)
{

    rc_adcs_sp_17 rc;
     encoderc_adcs_sp_17(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void battrcPopulateH1(CANPacket *out)
{
    rc_adcs_bdot_h1 rc;
       rc.rc_adcs_bdot_h1_reset_count = 0;
       rc.rc_adcs_bdot_h1_sysrstiv = 0;
       rc.rc_adcs_bdot_h1_temp_avg = 0;
       rc.rc_adcs_bdot_h1_temp_min = 0;
       rc.rc_adcs_bdot_h1_reset_count = 0;
       encoderc_adcs_bdot_h1(&rc, out);
    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);
}

void battrcPopulate3(CANPacket *out)
{
    rc_adcs_bdot_3 rc = {0};
      rc.rc_adcs_bdot_3_mag_y_max = 0;
      rc.rc_adcs_bdot_3_mag_y_avg = 0;
      rc.rc_adcs_bdot_3_mag_z_min = 0;
      rc.rc_adcs_bdot_3_mag_z_max = 0;
      encoderc_adcs_bdot_3(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}


void battrcPopulate4(CANPacket *out)
{
    rc_adcs_bdot_4 rc = {0};
       rc.rc_adcs_bdot_4_mag_z_avg = 0;
       rc.rc_adcs_bdot_4_tumble = 0;
       rc.rc_adcs_bdot_4_last_spam_y_mtq_y = 0;
       rc.rc_adcs_bdot_4_last_spam_y_mtq_z = 0;
       encoderc_adcs_bdot_4(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}

void battrcPopulate5(CANPacket *out)
{
    rc_adcs_bdot_5 rc = {0};
    rc.rc_adcs_bdot_5_last_spam_z_mtq_x = 0;
    rc.rc_adcs_bdot_5_last_spam_z_mtq_y = 0;
    rc.rc_adcs_bdot_5_last_spam_z_mtq_z = 0;
    encoderc_adcs_bdot_5(&rc, out);

    out->length=8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((sentCount))));
    uint64_t *thePointer = (uint64_t *) (&(out -> data));
    *thePointer = fullPacketData;
    reverseArrah((out->data), 0, 7);

}


FILE_STATIC const rollcall_fn rollcallFunctions[] =
{
 bdotrcPopulateH1, bdotrcPopulateH2, bdotrcPopulate1, bdotrcPopulate2, bdotrcPopulate3, bdotrcPopulate4, bdotrcPopulate5,
 mtqrcPopulate1, mtqrcPopulate0, mtqrcPopulate2, mtqrcPopulate3, mtqrcPopulate4, mtqrcPopulate5,
 sprcPopulate1, sprcPopulate2, sprcPopulate3, sprcPopulate4, sprcPopulate5,
 sprcPopulate6, sprcPopulate9, sprcPopulate10, sprcPopulate11, sprcPopulate12,
 sprcPopulate13, sprcPopulate14, sprcPopulate15, sprcPopulate16, sprcPopulate17,
 battrcPopulateH1, battrcPopulate3, battrcPopulate4,
 battrcPopulate5

};



void rxCb(CANPacket *p){
    // PPT Single Fire
    if(p -> id == CAN_ID_CMD_ROLLCALL){
        rollcallStart();
        sentCount++;
    }
}

int main(void) {

    // ALWAYS START main() with bspInit(<systemname>) as the FIRST line of code
    bspInit(Module_Test);
    setCANPacketRxCallback(rxCb);
    canWrapInit();

    P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    P3DIR |= BIT0 | BIT1 | BIT2 | BIT3;

    P1OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
    P3OUT |= BIT0 | BIT1 | BIT2 | BIT3;

    P1DIR &= ~(BIT7 | BIT6);
    P3DIR &= ~(BIT7 | BIT6);
    P2DIR &= ~(BIT2 | BIT6);

    P1OUT &= ~(BIT7 | BIT6);
    P3OUT &= ~(BIT7 | BIT6);
    P2OUT &= ~(BIT2 | BIT6);

    P1REN |= BIT7 | BIT6;
    P3REN |= BIT7 | BIT6;
    P2REN |= BIT2 | BIT6;

    PJDIR |= BIT0 | BIT1 | BIT2;

    rollcallInit(rollcallFunctions, sizeof(rollcallFunctions) / sizeof(rollcall_fn));
//    rollcallInit(rollcallFunctions, 8);
    while(1){
        rollcallUpdate();
        __delay_cycles(100);
    }

#if defined(__DEBUG__)

    // TODO:  Insert debug-specific stuff here, including registering info/status/action handlers, etc.

#endif  //  __DEBUG__
	
	return 0;
}
