/*
 * canwrap.c
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include "canwrap.h"
#include "../core/can.h"
#include "../bsp/bsp.h"


//void canPacketInit(uint8_t boardNum){
//    canInit();
//    setTheFilter(CAN_MASK_0, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_0, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_1, 0x01 << boardNum);
//    setTheFilter(CAN_MASK_1, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_2, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_3, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_4, 0x01 << boardNum);
//    setTheFilter(CAN_FILTER_5, 0x01 << boardNum);
//}

uint8_t CAN_WRAP_BUFFER_KEEPER_TRACKER;

void setMaskOrFilter(uint8_t addr, uint32_t filter){
    setTheFilter(addr, filter);
}

void wrapCB0(uint8_t length, uint8_t* data, uint32_t id){
    CANPacket packet = {0};
    CANPacket *p = &packet;
    p -> id = id;
    uint8_t i;
    for(i = 0 ; i < length; i++){
        p -> data[i] = data[i];
    }
    p -> bufferNum = 0;
    CANPacketReceived(p);
}

void wrapCB1(uint8_t length, uint8_t* data, uint32_t id){
    CANPacket packet = {0};
    CANPacket *p = &packet;
    p -> id = id;
    uint8_t i;
    for(i = 0 ; i < length; i++){
        p -> data[i] = data[i];
    }
    p -> bufferNum = 1;
    CANPacketReceived(p);
}

void canWrapInit(){
    canInit();
    setReceiveCallback0(wrapCB0);
    setReceiveCallback1(wrapCB1);
    CAN_WRAP_BUFFER_KEEPER_TRACKER = 0;
}

void canWrapInitWithFilter(){
    canWrapInit();
    SubsystemModule ss = bspGetModule();
    uint8_t filter_one = 0x0;
    uint8_t filter_two = 0x0;
    uint8_t filter_three = 0x0;
    uint8_t filter_four = 0x0;
    switch(ss) {
      case Module_Test :
        filter_one = 0x01;
        filter_two = 0x02;
        filter_three = 0x03;
        filter_four = 0x04;
        break;
      case Module_ADCS_BDot :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x07;
        break;
      case Module_ADCS_RWX :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_ADCS_RWY :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_ADCS_RWZ :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_PPT :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_EPS_Dist :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_EPS_Gen :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_EPS_Batt :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_ADCS_Estim :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x07;
        break;
      case Module_ADCS_MPC :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_ADCS_SensorProc :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_ADCS_MTQ :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x07;
        break;
      case Module_COM1 :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_COM2 :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      case Module_RAHS :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
        break;
      default :
        filter_one = 0x01;
        filter_two = 0x03;
        filter_three = 0x04;
        filter_four = 0x04;
  	}
  	setTheFilter(CAN_MASK_0, (uint32_t) 0x1e000000);
   	setTheFilter(CAN_FILTER_0, (uint32_t) 0x00);
   	setTheFilter(CAN_FILTER_1, (uint32_t) 0x00);

   	setTheFilter(CAN_MASK_1, filter_one);
   	setTheFilter(CAN_FILTER_2, (uint32_t) filter_one << 16);
    setTheFilter(CAN_FILTER_3, (uint32_t) filter_two << 16);
    setTheFilter(CAN_FILTER_4, (uint32_t) filter_three << 16);
    setTheFilter(CAN_FILTER_5, (uint32_t) filter_four << 16);
}


void reverseArray(uint8_t arr[], uint8_t start, uint8_t end)
{
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArray(arr, start+1, end-1);
}

void canSendPacket(CANPacket *packet){
    uint8_t tech[5] = {
       (uint8_t) (packet->id >> 21),
       (uint8_t) (packet->id >> 16) & 0x03 | (uint8_t) (packet->id >> 13) & 0xE0 | 0x08,
       (uint8_t) (packet->id >> 8),
       (uint8_t) packet->id,
       packet->length
    };
    CAN_WRAP_BUFFER_KEEPER_TRACKER = (CAN_WRAP_BUFFER_KEEPER_TRACKER + 1) % 3;
    canSend(CAN_WRAP_BUFFER_KEEPER_TRACKER,tech, packet->data);
}

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet)) {
    CANPacketReceived = ReceiveCallbackArg;
}

// AUTOGEN STUFF HERE

void decoderc_adcs_bdot_3(CANPacket *input, rc_adcs_bdot_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_3_tumble = (uint8_t) (((fullData & ((uint64_t) 0x1 << 31)) >> 31));
    output -> rc_adcs_bdot_3_mag_avg = (int32_t) (((fullData & ((uint64_t) 0xffffffff << 32)) >> 32));
}

void encoderc_adcs_bdot_3(rc_adcs_bdot_3 *input, CANPacket *output){
    output -> id = 304677401;
    output -> length = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_tumble))) & 0x1) << 31;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_mag_avg))) & 0xffffffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_2(CANPacket *input, rc_adcs_bdot_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_2_mag_min = (int32_t) (((fullData & ((uint64_t) 0xffffffff << 32)) >> 32));
    output -> rc_adcs_bdot_2_mag_max = (int32_t) (((fullData & ((uint64_t) 0xffffffff))));
}

void encoderc_adcs_bdot_2(rc_adcs_bdot_2 *input, CANPacket *output){
    output -> id = 304677383;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_min))) & 0xffffffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_max))) & 0xffffffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_1(CANPacket *input, rc_adcs_bdot_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_1_temp_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_1_temp_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_1_sysrstiv = (uint16_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_adcs_bdot_1(rc_adcs_bdot_1 *input, CANPacket *output){
    output -> id = 304677382;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_sysrstiv))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_sun_unit_z(CANPacket *input, estim_sun_unit_z *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_sun_unit_z_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_sun_unit_z_val = (*((double *)(&(tempestim_sun_unit_z_val))));
}

void encodeestim_sun_unit_z(estim_sun_unit_z *input, CANPacket *output){
    output -> id = 302449332;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_sun_unit_z_val = ((input -> estim_sun_unit_z_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_sun_unit_z_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_sun_unit_y(CANPacket *input, estim_sun_unit_y *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_sun_unit_y_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_sun_unit_y_val = (*((double *)(&(tempestim_sun_unit_y_val))));
}

void encodeestim_sun_unit_y(estim_sun_unit_y *input, CANPacket *output){
    output -> id = 302449331;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_sun_unit_y_val = ((input -> estim_sun_unit_y_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_sun_unit_y_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_sun_unit_x(CANPacket *input, estim_sun_unit_x *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_sun_unit_x_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_sun_unit_x_val = (*((double *)(&(tempestim_sun_unit_x_val))));
}

void encodeestim_sun_unit_x(estim_sun_unit_x *input, CANPacket *output){
    output -> id = 302449330;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_sun_unit_x_val = ((input -> estim_sun_unit_x_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_sun_unit_x_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_mag_unit_z(CANPacket *input, estim_mag_unit_z *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_mag_unit_z_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_mag_unit_z_val = (*((double *)(&(tempestim_mag_unit_z_val))));
}

void encodeestim_mag_unit_z(estim_mag_unit_z *input, CANPacket *output){
    output -> id = 302449335;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_mag_unit_z_val = ((input -> estim_mag_unit_z_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_mag_unit_z_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_mag_unit_y(CANPacket *input, estim_mag_unit_y *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_mag_unit_y_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_mag_unit_y_val = (*((double *)(&(tempestim_mag_unit_y_val))));
}

void encodeestim_mag_unit_y(estim_mag_unit_y *input, CANPacket *output){
    output -> id = 302449334;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_mag_unit_y_val = ((input -> estim_mag_unit_y_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_mag_unit_y_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_mag_unit_x(CANPacket *input, estim_mag_unit_x *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempestim_mag_unit_x_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> estim_mag_unit_x_val = (*((double *)(&(tempestim_mag_unit_x_val))));
}

void encodeestim_mag_unit_x(estim_mag_unit_x *input, CANPacket *output){
    output -> id = 302449333;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempestim_mag_unit_x_val = ((input -> estim_mag_unit_x_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempestim_mag_unit_x_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeestim_state(CANPacket *input, estim_state *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> estim_state_in_sun = (uint8_t) (((fullData & ((uint64_t) 0x1 << 62)) >> 62));
    output -> estim_state_above_gs = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodeestim_state(estim_state *input, CANPacket *output){
    output -> id = 302449336;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> estim_state_in_sun))) & 0x1) << 62;
    fullPacketData |= (((uint64_t)((input -> estim_state_above_gs))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_11(CANPacket *input, rc_eps_gen_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t temprc_eps_gen_11_pnl_1_power_min = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> rc_eps_gen_11_pnl_1_power_min = (*((float *)(&(temprc_eps_gen_11_pnl_1_power_min))));
    uint32_t temprc_eps_gen_11_pnl_1_power_max = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> rc_eps_gen_11_pnl_1_power_max = (*((float *)(&(temprc_eps_gen_11_pnl_1_power_max))));
}

void encoderc_eps_gen_11(rc_eps_gen_11 *input, CANPacket *output){
    output -> id = 304677402;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float temprc_eps_gen_11_pnl_1_power_min = ((input -> rc_eps_gen_11_pnl_1_power_min));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temprc_eps_gen_11_pnl_1_power_min))))) << 32;
    const float temprc_eps_gen_11_pnl_1_power_max = ((input -> rc_eps_gen_11_pnl_1_power_max));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temprc_eps_gen_11_pnl_1_power_max)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_5(CANPacket *input, rc_adcs_mtq_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_5_reset_counts = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_5_fsw_ignore = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_5(rc_adcs_mtq_5 *input, CANPacket *output){
    output -> id = 304677391;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_5_reset_counts))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_5_fsw_ignore))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_4(CANPacket *input, rc_adcs_mtq_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_4_fsw_z_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_4_fsw_y_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_adcs_mtq_4_duty_z2_avg = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_mtq_4_duty_z1_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_4_duty_y2_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_mtq_4_duty_y1_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_mtq_4_duty_x2_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_adcs_mtq_4_duty_x1_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
}

void encoderc_adcs_mtq_4(rc_adcs_mtq_4 *input, CANPacket *output){
    output -> id = 304677390;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_fsw_z_min))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_fsw_y_max))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_z2_avg))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_z1_avg))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_y2_avg))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_y1_avg))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_x2_avg))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_4_duty_x1_avg))) & 0xff) << 40;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_3(CANPacket *input, rc_adcs_mtq_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_3_fsw_z_avg = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_mtq_3_fsw_y_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_3_fsw_y_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_mtq_3_fsw_y_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_mtq_3_fsw_x_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_adcs_mtq_3_fsw_x_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_adcs_mtq_3_fsw_x_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_3_bdot_z_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_3(rc_adcs_mtq_3 *input, CANPacket *output){
    output -> id = 304677389;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_z_avg))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_y_min))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_y_max))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_y_avg))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_x_min))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_x_max))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_fsw_x_avg))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_3_bdot_z_min))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_2(CANPacket *input, rc_adcs_mtq_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_2_bdot_z_max = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_mtq_2_bdot_z_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_2_bdot_y_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_mtq_2_bdot_y_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_mtq_2_bdot_y_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_adcs_mtq_2_bdot_x_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_adcs_mtq_2_bdot_x_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_2_bdot_x_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_2(rc_adcs_mtq_2 *input, CANPacket *output){
    output -> id = 304677388;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_z_max))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_z_avg))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_y_min))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_y_max))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_y_avg))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_x_min))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_x_max))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_2_bdot_x_avg))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_1(CANPacket *input, rc_adcs_mtq_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_1_temp_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_mtq_1_temp_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_mtq_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_mtq_1_sysrstiv = (uint16_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_adcs_mtq_1(rc_adcs_mtq_1 *input, CANPacket *output){
    output -> id = 304677387;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_1_sysrstiv))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_3(CANPacket *input, rc_ppt_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_3_ign_chg_min = (uint8_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_ppt_3_ign_chg_max = (uint8_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_3_ign_chg_avg = (uint8_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_ppt_3(rc_ppt_3 *input, CANPacket *output){
    output -> id = 304677386;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_3_ign_chg_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_3_ign_chg_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_3_ign_chg_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_2(CANPacket *input, rc_ppt_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_2_total_fire_count = (uint8_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_ppt_2_main_chg_min = (uint8_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_ppt_2_main_chg_max = (uint8_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_2_main_chg_avg = (uint8_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_ppt_2(rc_ppt_2 *input, CANPacket *output){
    output -> id = 304677385;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_total_fire_count))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_main_chg_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_main_chg_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_main_chg_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_1(CANPacket *input, rc_ppt_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_1_temp_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_ppt_1_temp_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_ppt_1_sysrstiv = (uint16_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_ppt_1(rc_ppt_1 *input, CANPacket *output){
    output -> id = 304677384;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_sysrstiv))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_9(CANPacket *input, rc_eps_gen_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_9_pnl_3_temp_min = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_eps_gen_9_pnl_3_temp_max = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_eps_gen_9_pnl_3_temp_avg = (int8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_eps_gen_9_pnl_2_temp_min = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_eps_gen_9_pnl_2_temp_max = (int8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_eps_gen_9_pnl_2_temp_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_eps_gen_9_pnl_1_temp_max = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_gen_9_pnl_1_temp_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
}

void encoderc_eps_gen_9(rc_eps_gen_9 *input, CANPacket *output){
    output -> id = 304677400;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_3_temp_min))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_3_temp_max))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_3_temp_avg))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_2_temp_min))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_2_temp_max))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_2_temp_avg))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_1_temp_max))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_9_pnl_1_temp_avg))) & 0xff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_8(CANPacket *input, rc_eps_gen_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_8_pnl_3_power_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_8_pnl_3_power_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_8_pnl_3_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_8_pnl_1_temp_min = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_gen_8(rc_eps_gen_8 *input, CANPacket *output){
    output -> id = 304677399;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_8_pnl_3_power_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_8_pnl_3_power_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_8_pnl_3_power_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_8_pnl_1_temp_min))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_7(CANPacket *input, rc_eps_gen_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_7_pnl_2_power_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_7_pnl_2_power_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_7_pnl_2_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_7_pnl_1_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_gen_7(rc_eps_gen_7 *input, CANPacket *output){
    output -> id = 304677398;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_7_pnl_2_power_min))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_7_pnl_2_power_max))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_7_pnl_2_power_avg))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_7_pnl_1_power_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_6(CANPacket *input, rc_eps_gen_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_6_pnl_3_current_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_6_pnl_3_current_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_6_pnl_1_power_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_6_pnl_1_power_max = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
}

void encoderc_eps_gen_6(rc_eps_gen_6 *input, CANPacket *output){
    output -> id = 304677397;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_6_pnl_3_current_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_6_pnl_3_current_avg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_6_pnl_1_power_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_6_pnl_1_power_max))) & 0xffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_5(CANPacket *input, rc_eps_gen_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_5_pnl_3_current_min = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_5_pnl_2_current_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_5_pnl_2_current_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_5_pnl_2_current_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_gen_5(rc_eps_gen_5 *input, CANPacket *output){
    output -> id = 304677396;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_5_pnl_3_current_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_5_pnl_2_current_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_5_pnl_2_current_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_5_pnl_2_current_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_4(CANPacket *input, rc_eps_gen_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_4_pnl_3_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_4_pnl_1_current_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_4_pnl_1_current_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_4_pnl_1_current_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
}

void encoderc_eps_gen_4(rc_eps_gen_4 *input, CANPacket *output){
    output -> id = 304677395;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_4_pnl_3_voltage_avg))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_4_pnl_1_current_min))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_4_pnl_1_current_max))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_4_pnl_1_current_avg))) & 0xffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_3(CANPacket *input, rc_eps_gen_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_3_pnl_3_voltage_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_3_pnl_3_voltage_max = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_3_pnl_2_voltage_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_3_pnl_2_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_eps_gen_3(rc_eps_gen_3 *input, CANPacket *output){
    output -> id = 304677394;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_3_pnl_3_voltage_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_3_pnl_3_voltage_max))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_3_pnl_2_voltage_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_3_pnl_2_voltage_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_2(CANPacket *input, rc_eps_gen_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_2_pnl_2_voltage_min = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_2_pnl_1_voltage_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_2_pnl_1_voltage_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_2_pnl_1_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_gen_2(rc_eps_gen_2 *input, CANPacket *output){
    output -> id = 304677393;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_2_pnl_2_voltage_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_2_pnl_1_voltage_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_2_pnl_1_voltage_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_2_pnl_1_voltage_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_1(CANPacket *input, rc_eps_gen_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_1_temp_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_1_temp_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_1_sysrstiv = (uint16_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_gen_1(rc_eps_gen_1 *input, CANPacket *output){
    output -> id = 304677392;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_sysrstiv))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_6(CANPacket *input, rc_eps_batt_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_6_soc_min = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_batt_6_soc_max = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_eps_batt_6_soc_avg = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_eps_batt_6_last_charge = (uint64_t) (((fullData & ((uint64_t) 0xffffffffff))));
}

void encoderc_eps_batt_6(rc_eps_batt_6 *input, CANPacket *output){
    output -> id = 304677381;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_soc_min))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_soc_max))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_soc_avg))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_last_charge))) & 0xffffffffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_5(CANPacket *input, rc_eps_batt_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_5_node_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_5_node_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_5_node_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_5_batt_temp_min = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_eps_batt_5_batt_temp_max = (int8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_eps_batt_5(rc_eps_batt_5 *input, CANPacket *output){
    output -> id = 304677380;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_5_node_c_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_5_node_c_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_5_node_c_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_5_batt_temp_min))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_5_batt_temp_max))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_4(CANPacket *input, rc_eps_batt_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_4_voltage_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_4_voltage_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_4_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_4_heater_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 14)) >> 14));
    output -> rc_eps_batt_4_balancer_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encoderc_eps_batt_4(rc_eps_batt_4 *input, CANPacket *output){
    output -> id = 304677379;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_voltage_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_voltage_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_voltage_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_heater_state))) & 0x1) << 14;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_balancer_state))) & 0x1) << 15;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_3(CANPacket *input, rc_eps_batt_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_3_current_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_3_current_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_3_current_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_3_batt_temp_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_batt_3(rc_eps_batt_3 *input, CANPacket *output){
    output -> id = 304677378;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_3_current_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_3_current_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_3_current_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_3_batt_temp_avg))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_2(CANPacket *input, rc_eps_batt_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_2_node_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_2_node_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_2_node_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_batt_2(rc_eps_batt_2 *input, CANPacket *output){
    output -> id = 304677377;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_2_node_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_2_node_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_2_node_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_1(CANPacket *input, rc_eps_batt_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_1_temp_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_1_temp_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_1_sysrstiv = (uint16_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_batt_1(rc_eps_batt_1 *input, CANPacket *output){
    output -> id = 304677376;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_sysrstiv))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_ppt_set_count(CANPacket *input, cmd_ppt_set_count *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_set_count_count = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_ppt_set_count(cmd_ppt_set_count *input, CANPacket *output){
    output -> id = 302252294;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_set_count_count))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_ppt_time_upd(CANPacket *input, cmd_ppt_time_upd *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_time_upd_ign_charge = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> cmd_ppt_time_upd_cooldown = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> cmd_ppt_time_upd_ign_delay = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> cmd_ppt_time_upd_charge = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodecmd_ppt_time_upd(cmd_ppt_time_upd *input, CANPacket *output){
    output -> id = 302252293;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_time_upd_ign_charge))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_time_upd_cooldown))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_time_upd_ign_delay))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_time_upd_charge))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_ppt_halt(CANPacket *input, cmd_ppt_halt *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_halt_confirm = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_ppt_halt(cmd_ppt_halt *input, CANPacket *output){
    output -> id = 285475076;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_halt_confirm))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeppt_firing_result(CANPacket *input, ppt_firing_result *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> ppt_firing_result_panel_good = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> ppt_firing_result_batt_good = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> ppt_firing_result_coulmb_good = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> ppt_firing_result_ign_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> ppt_firing_result_main_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> ppt_firing_result_code = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodeppt_firing_result(ppt_firing_result *input, CANPacket *output){
    output -> id = 304677104;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_panel_good))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_batt_good))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_coulmb_good))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_ign_time))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_main_time))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> ppt_firing_result_code))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_6(CANPacket *input, tle_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temptle_6_mnm = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> tle_6_mnm = (*((double *)(&(temptle_6_mnm))));
}

void encodetle_6(tle_6 *input, CANPacket *output){
    output -> id = 309788745;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temptle_6_mnm = ((input -> tle_6_mnm));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temptle_6_mnm)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_5(CANPacket *input, tle_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> tle_5_id = (uint8_t) (((fullData & ((uint64_t) 0x1 << 31)) >> 31));
    uint32_t temptle_5_mna = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> tle_5_mna = (*((float *)(&(temptle_5_mna))));
}

void encodetle_5(tle_5 *input, CANPacket *output){
    output -> id = 309788744;
    output -> length = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> tle_5_id))) & 0x1) << 31;
    const float temptle_5_mna = ((input -> tle_5_mna));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_5_mna))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_4(CANPacket *input, tle_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t temptle_4_aop = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> tle_4_aop = (*((float *)(&(temptle_4_aop))));
    uint32_t temptle_4_raan = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> tle_4_raan = (*((float *)(&(temptle_4_raan))));
}

void encodetle_4(tle_4 *input, CANPacket *output){
    output -> id = 309788743;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float temptle_4_aop = ((input -> tle_4_aop));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_4_aop)))));
    const float temptle_4_raan = ((input -> tle_4_raan));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_4_raan))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_3(CANPacket *input, tle_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t temptle_3_ecc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> tle_3_ecc = (*((float *)(&(temptle_3_ecc))));
    uint32_t temptle_3_inc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> tle_3_inc = (*((float *)(&(temptle_3_inc))));
}

void encodetle_3(tle_3 *input, CANPacket *output){
    output -> id = 309788742;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float temptle_3_ecc = ((input -> tle_3_ecc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_3_ecc)))));
    const float temptle_3_inc = ((input -> tle_3_inc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_3_inc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_2(CANPacket *input, tle_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temptle_2_day = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> tle_2_day = (*((double *)(&(temptle_2_day))));
}

void encodetle_2(tle_2 *input, CANPacket *output){
    output -> id = 309788741;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temptle_2_day = ((input -> tle_2_day));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temptle_2_day)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodetle_1(CANPacket *input, tle_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> tle_1_id = (uint8_t) (((fullData & ((uint64_t) 0x1 << 23)) >> 23));
    uint32_t temptle_1_bstar = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 24)) >> 24);
    output -> tle_1_bstar = (*((float *)(&(temptle_1_bstar))));
    output -> tle_1_year = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodetle_1(tle_1 *input, CANPacket *output){
    output -> id = 302448708;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> tle_1_id))) & 0x1) << 23;
    const float temptle_1_bstar = ((input -> tle_1_bstar));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_1_bstar))))) << 24;
    fullPacketData |= (((uint64_t)((input -> tle_1_year))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_pd_undervoltage(CANPacket *input, eps_pd_undervoltage *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_pd_undervoltage_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodeeps_pd_undervoltage(eps_pd_undervoltage *input, CANPacket *output){
    output -> id = 304218338;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_pd_undervoltage_state))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_pd_batt_voltage(CANPacket *input, eps_pd_batt_voltage *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_pd_batt_voltage_agg = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> eps_pd_batt_voltage_val = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodeeps_pd_batt_voltage(eps_pd_batt_voltage *input, CANPacket *output){
    output -> id = 304218337;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_pd_batt_voltage_agg))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> eps_pd_batt_voltage_val))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_pd_rst(CANPacket *input, cmd_pd_rst *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_pd_rst_confirm = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_pd_rst(cmd_pd_rst *input, CANPacket *output){
    output -> id = 304218385;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_pd_rst_confirm))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_com2_run(CANPacket *input, cmd_com2_run *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_com2_run_clear = (uint8_t) (((fullData & ((uint64_t) 0x1 << 54)) >> 54));
    output -> cmd_com2_run_jump = (uint8_t) (((fullData & ((uint64_t) 0x1 << 55)) >> 55));
    output -> cmd_com2_run_fileno = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_com2_run(cmd_com2_run *input, CANPacket *output){
    output -> id = 302514673;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_com2_run_clear))) & 0x1) << 54;
    fullPacketData |= (((uint64_t)((input -> cmd_com2_run_jump))) & 0x1) << 55;
    fullPacketData |= (((uint64_t)((input -> cmd_com2_run_fileno))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecom2_state(CANPacket *input, com2_state *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> com2_state_health_state = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> com2_state_curr_file = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> com2_state_qfilesize = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> com2_state_qlen = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> com2_state_uptime = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodecom2_state(com2_state *input, CANPacket *output){
    output -> id = 304611824;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> com2_state_health_state))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> com2_state_curr_file))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> com2_state_qfilesize))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> com2_state_qlen))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> com2_state_uptime))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_pd_enable(CANPacket *input, cmd_pd_enable *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_pd_enable_enable = (uint8_t) (((fullData & ((uint64_t) 0x1 << 55)) >> 55));
    output -> cmd_pd_enable_domain = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_pd_enable(cmd_pd_enable *input, CANPacket *output){
    output -> id = 303300865;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_pd_enable_enable))) & 0x1) << 55;
    fullPacketData |= (((uint64_t)((input -> cmd_pd_enable_domain))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_batt_rst(CANPacket *input, cmd_batt_rst *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_batt_rst_confirm = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_batt_rst(cmd_batt_rst *input, CANPacket *output){
    output -> id = 304349442;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_batt_rst_confirm))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_batt_bal_enable(CANPacket *input, cmd_batt_bal_enable *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_batt_bal_enable_enable = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_batt_bal_enable(cmd_batt_bal_enable *input, CANPacket *output){
    output -> id = 304349443;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_batt_bal_enable_enable))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_reboot_request(CANPacket *input, cmd_reboot_request *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_reboot_request_domain = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_reboot_request(cmd_reboot_request *input, CANPacket *output){
    output -> id = 1310728;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_reboot_request_domain))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_ignore_fsw(CANPacket *input, cmd_ignore_fsw *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ignore_fsw_ignore = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_ignore_fsw(cmd_ignore_fsw *input, CANPacket *output){
    output -> id = 304349220;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ignore_fsw_ignore))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_mtq_fsw(CANPacket *input, cmd_mtq_fsw *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_mtq_fsw_sc_mode = (uint8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> cmd_mtq_fsw_z = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> cmd_mtq_fsw_y = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> cmd_mtq_fsw_x = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_mtq_fsw(cmd_mtq_fsw *input, CANPacket *output){
    output -> id = 304349219;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_fsw_sc_mode))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_fsw_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_fsw_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_fsw_x))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_domain_output(CANPacket *input, eps_domain_output *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_domain_output_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 6)) >> 6));
    output -> eps_domain_output_oc_trip = (uint8_t) (((fullData & ((uint64_t) 0x1 << 7)) >> 7));
    output -> eps_domain_output_v_c_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
    output -> eps_domain_output_v_c = (uint16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> eps_domain_output_v = (uint16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> eps_domain_output_num = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodeeps_domain_output(eps_domain_output *input, CANPacket *output){
    output -> id = 304218336;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_state))) & 0x1) << 6;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_oc_trip))) & 0x1) << 7;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_v_c_max))) & 0xffff) << 8;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_v_c))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_v))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> eps_domain_output_num))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_imu(CANPacket *input, sensorproc_imu *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_imu_z = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_imu_y = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_imu_x = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> sensorproc_imu_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encodesensorproc_imu(sensorproc_imu *input, CANPacket *output){
    output -> id = 335872066;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_imu_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> sensorproc_imu_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_imu_x))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> sensorproc_imu_valid))) & 0x1) << 15;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_mag(CANPacket *input, sensorproc_mag *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_mag_z = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_mag_y = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_mag_x = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> sensorproc_mag_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encodesensorproc_mag(sensorproc_mag *input, CANPacket *output){
    output -> id = 335872067;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag_x))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag_valid))) & 0x1) << 15;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_rollcall(CANPacket *input, cmd_rollcall *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_rollcall_type = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> cmd_rollcall_met_overflow = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> cmd_rollcall_met = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 24)) >> 24));
    output -> cmd_rollcall_msp = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_rollcall(cmd_rollcall *input, CANPacket *output){
    output -> id = 1114132;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_rollcall_type))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> cmd_rollcall_met_overflow))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> cmd_rollcall_met))) & 0xffffffff) << 24;
    fullPacketData |= (((uint64_t)((input -> cmd_rollcall_msp))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodemtq_ack(CANPacket *input, mtq_ack *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> mtq_ack_source = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> mtq_ack_phase = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> mtq_ack_last_fsw_z = (int8_t) (((fullData & ((uint64_t) 0xff))));
    output -> mtq_ack_last_fsw_y = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> mtq_ack_last_fsw_x = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> mtq_ack_last_bdot_z = (int8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> mtq_ack_last_bdot_y = (int8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> mtq_ack_last_bdot_x = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
}

void encodemtq_ack(mtq_ack *input, CANPacket *output){
    output -> id = 34013232;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_source))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_phase))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_fsw_z))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_fsw_y))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_fsw_x))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_bdot_z))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_bdot_y))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> mtq_ack_last_bdot_x))) & 0xff) << 40;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegen_panel_temp(CANPacket *input, gen_panel_temp *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gen_panel_temp_agg = (uint8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> gen_panel_temp_x_pos = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> gen_panel_temp_y_neg = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> gen_panel_temp_y_pos = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegen_panel_temp(gen_panel_temp *input, CANPacket *output){
    output -> id = 36241619;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gen_panel_temp_agg))) & 0xff) << 32;
    fullPacketData |= (((uint64_t)((input -> gen_panel_temp_x_pos))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> gen_panel_temp_y_neg))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> gen_panel_temp_y_pos))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_gen_rst(CANPacket *input, cmd_gen_rst *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_gen_rst_confirm = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_gen_rst(cmd_gen_rst *input, CANPacket *output){
    output -> id = 34865424;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_gen_rst_confirm))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_ppt_single_fire(CANPacket *input, cmd_ppt_single_fire *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_single_fire_override = (uint8_t) (((fullData & ((uint64_t) 0x1 << 62)) >> 62));
    output -> cmd_ppt_single_fire_with_pulse = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_ppt_single_fire(cmd_ppt_single_fire *input, CANPacket *output){
    output -> id = 34865408;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_single_fire_override))) & 0x1) << 62;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_single_fire_with_pulse))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegen_panel_pwr(CANPacket *input, gen_panel_pwr *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gen_panel_pwr_agg = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> gen_panel_pwr_x_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> gen_panel_pwr_y_neg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> gen_panel_pwr_y_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodegen_panel_pwr(gen_panel_pwr *input, CANPacket *output){
    output -> id = 35782866;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gen_panel_pwr_agg))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> gen_panel_pwr_x_pos))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> gen_panel_pwr_y_neg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> gen_panel_pwr_y_pos))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegen_panel_current(CANPacket *input, gen_panel_current *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gen_panel_current_x_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> gen_panel_current_y_neg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> gen_panel_current_y_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodegen_panel_current(gen_panel_current *input, CANPacket *output){
    output -> id = 35782865;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gen_panel_current_x_pos))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> gen_panel_current_y_neg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> gen_panel_current_y_pos))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegen_panel_voltage(CANPacket *input, gen_panel_voltage *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gen_panel_voltage_x_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> gen_panel_voltage_y_neg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> gen_panel_voltage_y_pos = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodegen_panel_voltage(gen_panel_voltage *input, CANPacket *output){
    output -> id = 34734288;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gen_panel_voltage_x_pos))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> gen_panel_voltage_y_neg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> gen_panel_voltage_y_pos))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodempc_vp(CANPacket *input, mpc_vp *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> mpc_vp_status = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodempc_vp(mpc_vp *input, CANPacket *output){
    output -> id = 37158946;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> mpc_vp_status))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_batt_state(CANPacket *input, eps_batt_state *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_batt_state_bal = (uint8_t) (((fullData & ((uint64_t) 0x1 << 46)) >> 46));
    output -> eps_batt_state_heat = (uint8_t) (((fullData & ((uint64_t) 0x1 << 47)) >> 47));
    output -> eps_batt_state_soc = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> eps_batt_state_temp = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodeeps_batt_state(eps_batt_state *input, CANPacket *output){
    output -> id = 35782850;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_batt_state_bal))) & 0x1) << 46;
    fullPacketData |= (((uint64_t)((input -> eps_batt_state_heat))) & 0x1) << 47;
    fullPacketData |= (((uint64_t)((input -> eps_batt_state_soc))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> eps_batt_state_temp))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_batt_current(CANPacket *input, eps_batt_current *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_batt_current_node_c = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> eps_batt_current_min_c = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> eps_batt_current_max_c = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> eps_batt_current_c = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodeeps_batt_current(eps_batt_current *input, CANPacket *output){
    output -> id = 35782849;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_batt_current_node_c))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> eps_batt_current_min_c))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> eps_batt_current_max_c))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> eps_batt_current_c))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_batt_voltage(CANPacket *input, eps_batt_voltage *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_batt_voltage_v_max_delta = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> eps_batt_voltage_node_v = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> eps_batt_voltage_min_v = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> eps_batt_voltage_v = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodeeps_batt_voltage(eps_batt_voltage *input, CANPacket *output){
    output -> id = 35782848;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_batt_voltage_v_max_delta))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> eps_batt_voltage_node_v))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> eps_batt_voltage_min_v))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> eps_batt_voltage_v))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_sun(CANPacket *input, sensorproc_sun *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_sun_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
    output -> sensorproc_sun_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_sun_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_sun_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodesensorproc_sun(sensorproc_sun *input, CANPacket *output){
    output -> id = 335872065;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_valid))) & 0x1) << 15;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecmd_mtq_bdot(CANPacket *input, cmd_mtq_bdot *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_mtq_bdot_z = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> cmd_mtq_bdot_y = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> cmd_mtq_bdot_x = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_mtq_bdot(cmd_mtq_bdot *input, CANPacket *output){
    output -> id = 304545825;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_x))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodebdot_tumble_status(CANPacket *input, bdot_tumble_status *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> bdot_tumble_status_status = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodebdot_tumble_status(bdot_tumble_status *input, CANPacket *output){
    output -> id = 34013216;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> bdot_tumble_status_status))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesync_2(CANPacket *input, sync_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodesync_2(sync_2 *input, CANPacket *output){
    output -> id = 65554;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesync_1(CANPacket *input, sync_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodesync_1(sync_1 *input, CANPacket *output){
    output -> id = 65553;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodemsp_temp(CANPacket *input, msp_temp *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> msp_temp_temp = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodemsp_temp(msp_temp *input, CANPacket *output){
    output -> id = 36241427;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> msp_temp_temp))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegrnd_epoch(CANPacket *input, grnd_epoch *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> grnd_epoch_val_overflow = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> grnd_epoch_val = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 32)) >> 32));
}

void encodegrnd_epoch(grnd_epoch *input, CANPacket *output){
    output -> id = 302449337;
    output -> length = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> grnd_epoch_val_overflow))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> grnd_epoch_val))) & 0xffffffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

