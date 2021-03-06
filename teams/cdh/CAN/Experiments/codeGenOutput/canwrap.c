/*
 * canwrap.c
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include "canwrap.h"
#include "../bsp/bsp.h"

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
        filter_two = 0x04;
        filter_three = 0x05;
        filter_four = 0x07;
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

   	setTheFilter(CAN_MASK_1, (uint32_t) 0xf0000);
   	setTheFilter(CAN_FILTER_2, (uint32_t) filter_one << 16);
    setTheFilter(CAN_FILTER_3, (uint32_t) filter_two << 16);
    setTheFilter(CAN_FILTER_4, (uint32_t) filter_three << 16);
    setTheFilter(CAN_FILTER_5, (uint32_t) filter_four << 16);
}


void reverseArray(uint8_t arr[], uint8_t start, uint8_t end) {
    uint8_t temp;
    if (start >= end)
        return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    reverseArray(arr, start+1, end-1);
}

uint8_t canSendPacket(CANPacket *packet){
    uint8_t tech[5] = {
       (uint8_t) (packet->id >> 21),
       (uint8_t) (packet->id >> 16) & 0x03 | (uint8_t) (packet->id >> 13) & 0xE0 | 0x08,
       (uint8_t) (packet->id >> 8),
       (uint8_t) packet->id,
       packet->length
    };

    const uint8_t bufferAvailability = canTxCheck();
    if (!(canTxCheck() & 0x01)){
        canSend(0, tech, packet->data);
        return 0;
    }
    if (!(canTxCheck() & 0x02)){
        canSend(1, tech, packet->data);
        return 0;
    }
    if (!(canTxCheck() & 0x04)){
        canSend(2, tech, packet->data);
        return 0;
    }
    return 1;
}

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet)) {
    CANPacketReceived = ReceiveCallbackArg;
}

// AUTOGEN STUFF HERE

void decodegcmd_reset_i2c(CANPacket *input, gcmd_reset_i2c *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_reset_i2c(gcmd_reset_i2c *input, CANPacket *output){
    output -> id = 302252768;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_11(CANPacket *input, rc_adcs_bdot_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_11_mag_z_var = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_11_mag_y_var = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_11_mag_x_var = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_11(rc_adcs_bdot_11 *input, CANPacket *output){
    output -> id = 308871796;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_11_mag_z_var))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_11_mag_y_var))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_11_mag_x_var))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodecom1_mode(CANPacket *input, com1_mode *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> com1_mode_mode = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecom1_mode(com1_mode *input, CANPacket *output){
    output -> id = 302252858;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> com1_mode_mode))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_mode_camera(CANPacket *input, gcmd_com1_mode_camera *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_com1_mode_camera_time = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_com1_mode_camera(gcmd_com1_mode_camera *input, CANPacket *output){
    output -> id = 302252856;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_com1_mode_camera_time))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_mode_realtime(CANPacket *input, gcmd_com1_mode_realtime *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_com1_mode_realtime_time = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_com1_mode_realtime(gcmd_com1_mode_realtime *input, CANPacket *output){
    output -> id = 302252855;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_com1_mode_realtime_time))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_mode_health(CANPacket *input, gcmd_com1_mode_health *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_com1_mode_health(gcmd_com1_mode_health *input, CANPacket *output){
    output -> id = 302252854;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_mode_safe(CANPacket *input, gcmd_com1_mode_safe *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_com1_mode_safe(gcmd_com1_mode_safe *input, CANPacket *output){
    output -> id = 302252853;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_transponder_off(CANPacket *input, gcmd_com1_transponder_off *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_com1_transponder_off(gcmd_com1_transponder_off *input, CANPacket *output){
    output -> id = 302252852;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com1_transponder_on(CANPacket *input, gcmd_com1_transponder_on *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_com1_transponder_on(gcmd_com1_transponder_on *input, CANPacket *output){
    output -> id = 302252851;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderahs_camera(CANPacket *input, rahs_camera *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rahs_camera_data = (uint64_t) (((fullData & ((uint64_t) 0xffffffffffffffff))));
}

void encoderahs_camera(rahs_camera *input, CANPacket *output){
    output -> id = 303563552;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rahs_camera_data))) & 0xffffffffffffffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_10(CANPacket *input, rc_eps_gen_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_10_pnl_3_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_10_pnl_2_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_10_pnl_1_power_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_gen_10(rc_eps_gen_10 *input, CANPacket *output){
    output -> id = 309920562;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_10_pnl_3_power_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_10_pnl_2_power_avg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_10_pnl_1_power_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_enable(CANPacket *input, gcmd_autoseq_enable *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_enable_enable = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_autoseq_enable(gcmd_autoseq_enable *input, CANPacket *output){
    output -> id = 302252849;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_enable_enable))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_autoshutoff(CANPacket *input, gcmd_dist_autoshutoff *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_dist_autoshutoff_ppt = (uint8_t) (((fullData & ((uint64_t) 0x3 << 52)) >> 52));
    output -> gcmd_dist_autoshutoff_eps = (uint8_t) (((fullData & ((uint64_t) 0x3 << 54)) >> 54));
    output -> gcmd_dist_autoshutoff_estim = (uint8_t) (((fullData & ((uint64_t) 0x3 << 56)) >> 56));
    output -> gcmd_dist_autoshutoff_bdot = (uint8_t) (((fullData & ((uint64_t) 0x3 << 58)) >> 58));
    output -> gcmd_dist_autoshutoff_rahs = (uint8_t) (((fullData & ((uint64_t) 0x3 << 60)) >> 60));
    output -> gcmd_dist_autoshutoff_com2 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_dist_autoshutoff(gcmd_dist_autoshutoff *input, CANPacket *output){
    output -> id = 302252847;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_ppt))) & 0x3) << 52;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_eps))) & 0x3) << 54;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_estim))) & 0x3) << 56;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_bdot))) & 0x3) << 58;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_rahs))) & 0x3) << 60;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_autoshutoff_com2))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_ppt_halt(CANPacket *input, gcmd_ppt_halt *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_ppt_halt_confirm = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodegcmd_ppt_halt(gcmd_ppt_halt *input, CANPacket *output){
    output -> id = 285475076;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_ppt_halt_confirm))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_reset_mission(CANPacket *input, gcmd_dist_reset_mission *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_dist_reset_mission(gcmd_dist_reset_mission *input, CANPacket *output){
    output -> id = 302252767;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_self_restart(CANPacket *input, gcmd_dist_self_restart *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encodegcmd_dist_self_restart(gcmd_dist_self_restart *input, CANPacket *output){
    output -> id = 302252766;
    output -> length = 0;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_18(CANPacket *input, rc_eps_dist_18 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_18_rahs_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 10)) >> 10));
    output -> rc_eps_dist_18_ppt_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 19)) >> 19));
    output -> rc_eps_dist_18_estim_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 28)) >> 28));
    output -> rc_eps_dist_18_eps_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 37)) >> 37));
    output -> rc_eps_dist_18_com2_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 46)) >> 46));
    output -> rc_eps_dist_18_bdot_ocp_thresh = (uint16_t) (((fullData & ((uint64_t) 0x1ff << 55)) >> 55));
}

void encoderc_eps_dist_18(rc_eps_dist_18 *input, CANPacket *output){
    output -> id = 307823323;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_rahs_ocp_thresh))) & 0x1ff) << 10;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_ppt_ocp_thresh))) & 0x1ff) << 19;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_estim_ocp_thresh))) & 0x1ff) << 28;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_eps_ocp_thresh))) & 0x1ff) << 37;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_com2_ocp_thresh))) & 0x1ff) << 46;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_18_bdot_ocp_thresh))) & 0x1ff) << 55;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_10(CANPacket *input, rc_adcs_bdot_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_10_dipole_var_z = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_10_dipole_var_y = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_10_dipole_var_x = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_10(rc_adcs_bdot_10 *input, CANPacket *output){
    output -> id = 308871901;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_10_dipole_var_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_10_dipole_var_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_10_dipole_var_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_9(CANPacket *input, rc_adcs_bdot_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_9_dipole_gain_z = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_bdot_9_dipole_gain_y = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_bdot_9_spam_off_z_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_9_spam_off_z_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_9_spam_off_z_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_9(rc_adcs_bdot_9 *input, CANPacket *output){
    output -> id = 308871795;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_9_dipole_gain_z))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_9_dipole_gain_y))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_9_spam_off_z_mtq_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_9_spam_off_z_mtq_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_9_spam_off_z_mtq_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_8(CANPacket *input, rc_adcs_bdot_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_8_spam_off_y_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_bdot_8_spam_off_y_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_8_spam_off_y_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_8_spam_off_x_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_8(rc_adcs_bdot_8 *input, CANPacket *output){
    output -> id = 308871794;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_8_spam_off_y_mtq_z))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_8_spam_off_y_mtq_y))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_8_spam_off_y_mtq_x))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_8_spam_off_x_mtq_z))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_batt_set_heater_check(CANPacket *input, gcmd_batt_set_heater_check *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_batt_set_heater_check_state = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_batt_set_heater_check(gcmd_batt_set_heater_check *input, CANPacket *output){
    output -> id = 301990618;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_batt_set_heater_check_state))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_dist_autoseq_get_met_rsp(CANPacket *input, eps_dist_autoseq_get_met_rsp *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_dist_autoseq_get_met_rsp_met = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 32)) >> 32));
}

void encodeeps_dist_autoseq_get_met_rsp(eps_dist_autoseq_get_met_rsp *input, CANPacket *output){
    output -> id = 307233497;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_dist_autoseq_get_met_rsp_met))) & 0xffffffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodeeps_dist_autoseq_get_ind_rsp(CANPacket *input, eps_dist_autoseq_get_ind_rsp *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> eps_dist_autoseq_get_ind_rsp_ind = (uint64_t) (((fullData & ((uint64_t) 0xffffffffffffffff))));
}

void encodeeps_dist_autoseq_get_ind_rsp(eps_dist_autoseq_get_ind_rsp *input, CANPacket *output){
    output -> id = 307233496;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> eps_dist_autoseq_get_ind_rsp_ind))) & 0xffffffffffffffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_get_met(CANPacket *input, gcmd_autoseq_get_met *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_get_met_index = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_autoseq_get_met(gcmd_autoseq_get_met *input, CANPacket *output){
    output -> id = 302252759;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_get_met_index))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_get_indices(CANPacket *input, gcmd_autoseq_get_indices *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_get_indices_id = (uint32_t) (((fullData & ((uint64_t) 0x1fffffff << 35)) >> 35));
}

void encodegcmd_autoseq_get_indices(gcmd_autoseq_get_indices *input, CANPacket *output){
    output -> id = 302252758;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_get_indices_id))) & 0x1fffffff) << 35;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_remove_can_id(CANPacket *input, gcmd_autoseq_remove_can_id *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_remove_can_id_id = (uint32_t) (((fullData & ((uint64_t) 0x1fffffff << 35)) >> 35));
}

void encodegcmd_autoseq_remove_can_id(gcmd_autoseq_remove_can_id *input, CANPacket *output){
    output -> id = 302252757;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_remove_can_id_id))) & 0x1fffffff) << 35;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_rm_at_index(CANPacket *input, gcmd_autoseq_rm_at_index *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_rm_at_index_index = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_autoseq_rm_at_index(gcmd_autoseq_rm_at_index *input, CANPacket *output){
    output -> id = 302252756;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_rm_at_index_index))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_add_2(CANPacket *input, gcmd_autoseq_add_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_add_2_data = (uint64_t) (((fullData & ((uint64_t) 0xffffffffffffffff))));
}

void encodegcmd_autoseq_add_2(gcmd_autoseq_add_2 *input, CANPacket *output){
    output -> id = 302252755;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_add_2_data))) & 0xffffffffffffffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_autoseq_add_1(CANPacket *input, gcmd_autoseq_add_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_autoseq_add_1_sendflg = (uint8_t) (((fullData & ((uint64_t) 0x1 << 2)) >> 2));
    output -> gcmd_autoseq_add_1_met = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 3)) >> 3));
    output -> gcmd_autoseq_add_1_can_id = (uint32_t) (((fullData & ((uint64_t) 0x1fffffff << 35)) >> 35));
}

void encodegcmd_autoseq_add_1(gcmd_autoseq_add_1 *input, CANPacket *output){
    output -> id = 302252754;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_add_1_sendflg))) & 0x1) << 2;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_add_1_met))) & 0xffffffff) << 3;
    fullPacketData |= (((uint64_t)((input -> gcmd_autoseq_add_1_can_id))) & 0x1fffffff) << 35;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_7(CANPacket *input, rc_adcs_bdot_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_7_dipole_gain_x = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_bdot_7_spam_off_x_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
    output -> rc_adcs_bdot_7_spam_off_x_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_adcs_bdot_7_spam_magnitude_z = (int16_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_adcs_bdot_7_spam_magnitude_y = (int16_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_bdot_7_spam_magnitude_x = (int16_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_bdot_7(rc_adcs_bdot_7 *input, CANPacket *output){
    output -> id = 308871793;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_dipole_gain_x))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_spam_off_x_mtq_y))) & 0xffff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_spam_off_x_mtq_x))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_spam_magnitude_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_spam_magnitude_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_7_spam_magnitude_x))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_6(CANPacket *input, rc_adcs_bdot_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_6_spam_on_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_6_spam_off_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_6_spam_control = (uint8_t) (((fullData & ((uint64_t) 0x1 << 31)) >> 31));
    output -> rc_adcs_bdot_6_pop_status_z = (uint8_t) (((fullData & ((uint64_t) 0x1 << 10)) >> 10));
    output -> rc_adcs_bdot_6_pop_status_y = (uint8_t) (((fullData & ((uint64_t) 0x1 << 11)) >> 11));
    output -> rc_adcs_bdot_6_pop_status_x = (uint8_t) (((fullData & ((uint64_t) 0x1 << 12)) >> 12));
    output -> rc_adcs_bdot_6_max_tumble_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 15)) >> 15));
    output -> rc_adcs_bdot_6_mag_control = (uint8_t) (((fullData & ((uint64_t) 0x3 << 5)) >> 5));
    output -> rc_adcs_bdot_6_gain_ovr_status_z = (uint8_t) (((fullData & ((uint64_t) 0x1 << 7)) >> 7));
    output -> rc_adcs_bdot_6_gain_ovr_status_y = (uint8_t) (((fullData & ((uint64_t) 0x1 << 8)) >> 8));
    output -> rc_adcs_bdot_6_gain_ovr_status_x = (uint8_t) (((fullData & ((uint64_t) 0x1 << 9)) >> 9));
    output -> rc_adcs_bdot_6_current_state = (uint8_t) (((fullData & ((uint64_t) 0x3 << 13)) >> 13));
}

void encoderc_adcs_bdot_6(rc_adcs_bdot_6 *input, CANPacket *output){
    output -> id = 307823216;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_spam_on_time))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_spam_off_time))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_spam_control))) & 0x1) << 31;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_pop_status_z))) & 0x1) << 10;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_pop_status_y))) & 0x1) << 11;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_pop_status_x))) & 0x1) << 12;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_max_tumble_time))) & 0xffff) << 15;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_mag_control))) & 0x3) << 5;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_gain_ovr_status_z))) & 0x1) << 7;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_gain_ovr_status_y))) & 0x1) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_gain_ovr_status_x))) & 0x1) << 9;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_6_current_state))) & 0x3) << 13;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_ppt_multiple_fire(CANPacket *input, gcmd_ppt_multiple_fire *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_ppt_multiple_fire_override = (uint8_t) (((fullData & ((uint64_t) 0x1 << 55)) >> 55));
    output -> gcmd_ppt_multiple_fire_count = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_ppt_multiple_fire(gcmd_ppt_multiple_fire *input, CANPacket *output){
    output -> id = 302252753;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_ppt_multiple_fire_override))) & 0x1) << 55;
    fullPacketData |= (((uint64_t)((input -> gcmd_ppt_multiple_fire_count))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_mtq_pwm_time(CANPacket *input, gcmd_mtq_pwm_time *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_mtq_pwm_time_measurement = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> gcmd_mtq_pwm_time_actuation = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_mtq_pwm_time(gcmd_mtq_pwm_time *input, CANPacket *output){
    output -> id = 302252752;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pwm_time_measurement))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pwm_time_actuation))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_eps_batt_fulldef(CANPacket *input, gcmd_eps_batt_fulldef *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_eps_batt_fulldef_const_volt = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_eps_batt_fulldef_const_volt = (*((float *)(&(tempgcmd_eps_batt_fulldef_const_volt))));
    uint32_t tempgcmd_eps_batt_fulldef_chg_curr = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> gcmd_eps_batt_fulldef_chg_curr = (*((float *)(&(tempgcmd_eps_batt_fulldef_chg_curr))));
}

void encodegcmd_eps_batt_fulldef(gcmd_eps_batt_fulldef *input, CANPacket *output){
    output -> id = 302252751;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_eps_batt_fulldef_const_volt = ((input -> gcmd_eps_batt_fulldef_const_volt));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_eps_batt_fulldef_const_volt))))) << 32;
    const float tempgcmd_eps_batt_fulldef_chg_curr = ((input -> gcmd_eps_batt_fulldef_chg_curr));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_eps_batt_fulldef_chg_curr)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_5(CANPacket *input, rc_adcs_bdot_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_5_spam_on_z_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_5_spam_on_z_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_5_spam_on_z_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_5(rc_adcs_bdot_5 *input, CANPacket *output){
    output -> id = 307823196;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_5_spam_on_z_mtq_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_5_spam_on_z_mtq_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_5_spam_on_z_mtq_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_reset_minmax(CANPacket *input, gcmd_reset_minmax *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_reset_minmax_mtq = (uint8_t) (((fullData & ((uint64_t) 0x1 << 55)) >> 55));
    output -> gcmd_reset_minmax_sensorproc = (uint8_t) (((fullData & ((uint64_t) 0x1 << 56)) >> 56));
    output -> gcmd_reset_minmax_mpc = (uint8_t) (((fullData & ((uint64_t) 0x1 << 57)) >> 57));
    output -> gcmd_reset_minmax_estim = (uint8_t) (((fullData & ((uint64_t) 0x1 << 58)) >> 58));
    output -> gcmd_reset_minmax_batt = (uint8_t) (((fullData & ((uint64_t) 0x1 << 59)) >> 59));
    output -> gcmd_reset_minmax_gen = (uint8_t) (((fullData & ((uint64_t) 0x1 << 60)) >> 60));
    output -> gcmd_reset_minmax_dist = (uint8_t) (((fullData & ((uint64_t) 0x1 << 61)) >> 61));
    output -> gcmd_reset_minmax_ppt = (uint8_t) (((fullData & ((uint64_t) 0x1 << 62)) >> 62));
    output -> gcmd_reset_minmax_bdot = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodegcmd_reset_minmax(gcmd_reset_minmax *input, CANPacket *output){
    output -> id = 302252750;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_mtq))) & 0x1) << 55;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_sensorproc))) & 0x1) << 56;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_mpc))) & 0x1) << 57;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_estim))) & 0x1) << 58;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_batt))) & 0x1) << 59;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_gen))) & 0x1) << 60;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_dist))) & 0x1) << 61;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_ppt))) & 0x1) << 62;
    fullPacketData |= (((uint64_t)((input -> gcmd_reset_minmax_bdot))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_h2(CANPacket *input, rc_eps_batt_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_h2_last_i2c_res = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_eps_batt_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_eps_batt_h2(rc_eps_batt_h2 *input, CANPacket *output){
    output -> id = 308871790;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h2_last_i2c_res))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_h2(CANPacket *input, rc_adcs_bdot_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_bdot_h2(rc_adcs_bdot_h2 *input, CANPacket *output){
    output -> id = 308871789;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_h2(CANPacket *input, rc_eps_dist_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_eps_dist_h2(rc_eps_dist_h2 *input, CANPacket *output){
    output -> id = 308871788;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_h2(CANPacket *input, rc_adcs_mpc_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mpc_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mpc_h2(rc_adcs_mpc_h2 *input, CANPacket *output){
    output -> id = 308871787;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_h2(CANPacket *input, rc_adcs_estim_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_estim_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_estim_h2(rc_adcs_estim_h2 *input, CANPacket *output){
    output -> id = 307823210;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_h2(CANPacket *input, rc_eps_gen_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_eps_gen_h2(rc_eps_gen_h2 *input, CANPacket *output){
    output -> id = 308871784;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_h2(CANPacket *input, rc_adcs_mtq_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_h2(rc_adcs_mtq_h2 *input, CANPacket *output){
    output -> id = 308871783;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_h2(CANPacket *input, rc_ppt_h2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_h2_last_fire_result = (uint8_t) (((fullData & ((uint64_t) 0x3 << 54)) >> 54));
    output -> rc_ppt_h2_canrxerror = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_ppt_h2(rc_ppt_h2 *input, CANPacket *output){
    output -> id = 308871782;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h2_last_fire_result))) & 0x3) << 54;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h2_canrxerror))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_h1(CANPacket *input, rc_eps_batt_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_eps_batt_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_eps_batt_h1(rc_eps_batt_h1 *input, CANPacket *output){
    output -> id = 308871781;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_h1(CANPacket *input, rc_adcs_bdot_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_bdot_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_adcs_bdot_h1(rc_adcs_bdot_h1 *input, CANPacket *output){
    output -> id = 308871780;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_h1(CANPacket *input, rc_eps_dist_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_dist_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_eps_dist_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_eps_dist_h1(rc_eps_dist_h1 *input, CANPacket *output){
    output -> id = 308871779;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_h1(CANPacket *input, rc_adcs_mpc_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mpc_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_mpc_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_mpc_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_mpc_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mpc_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_adcs_mpc_h1(rc_adcs_mpc_h1 *input, CANPacket *output){
    output -> id = 308871778;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_h1(CANPacket *input, rc_adcs_estim_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_estim_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_estim_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_estim_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_estim_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_estim_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_adcs_estim_h1(rc_adcs_estim_h1 *input, CANPacket *output){
    output -> id = 307823201;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_h1(CANPacket *input, rc_adcs_sp_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_sp_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_adcs_sp_h1(rc_adcs_sp_h1 *input, CANPacket *output){
    output -> id = 308871776;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_gen_h1(CANPacket *input, rc_eps_gen_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_gen_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_eps_gen_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_eps_gen_h1(rc_eps_gen_h1 *input, CANPacket *output){
    output -> id = 308871775;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mtq_h1(CANPacket *input, rc_adcs_mtq_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_mtq_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_mtq_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_mtq_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_adcs_mtq_h1(rc_adcs_mtq_h1 *input, CANPacket *output){
    output -> id = 308871774;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_h1(CANPacket *input, rc_ppt_h1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_h1_temp_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_ppt_h1_temp_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_h1_temp_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_ppt_h1_sysrstiv = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_ppt_h1_reset_count = (uint8_t) (((fullData & ((uint64_t) 0xff))));
}

void encoderc_ppt_h1(rc_ppt_h1 *input, CANPacket *output){
    output -> id = 308871773;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h1_temp_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h1_temp_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h1_sysrstiv))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_h1_reset_count))) & 0xff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_mtq_pms(CANPacket *input, gcmd_mtq_pms *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_mtq_pms_z = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> gcmd_mtq_pms_y = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> gcmd_mtq_pms_x = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> gcmd_mtq_pms_enable = (uint8_t) (((fullData & ((uint64_t) 0x3 << 38)) >> 38));
}

void encodegcmd_mtq_pms(gcmd_mtq_pms *input, CANPacket *output){
    output -> id = 302252749;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pms_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pms_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pms_x))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pms_enable))) & 0x3) << 38;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_bdot_max_tumble(CANPacket *input, gcmd_bdot_max_tumble *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_bdot_max_tumble_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodegcmd_bdot_max_tumble(gcmd_bdot_max_tumble *input, CANPacket *output){
    output -> id = 302252748;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_max_tumble_time))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_bdot_spam(CANPacket *input, gcmd_bdot_spam *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_bdot_spam_magnitude_z = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> gcmd_bdot_spam_magnitude_y = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> gcmd_bdot_spam_magnitude_x = (int8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> gcmd_bdot_spam_time_on = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> gcmd_bdot_spam_time_off = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> gcmd_bdot_spam_control = (uint8_t) (((fullData & ((uint64_t) 0x3 << 6)) >> 6));
}

void encodegcmd_bdot_spam(gcmd_bdot_spam *input, CANPacket *output){
    output -> id = 302252747;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_magnitude_z))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_magnitude_y))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_magnitude_x))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_time_on))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_time_off))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_spam_control))) & 0x3) << 6;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_bdot_control(CANPacket *input, gcmd_bdot_control *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_bdot_control_mode = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_bdot_control(gcmd_bdot_control *input, CANPacket *output){
    output -> id = 302252746;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_control_mode))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_bdot_mag_control(CANPacket *input, gcmd_bdot_mag_control *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_bdot_mag_control_mode = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_bdot_mag_control(gcmd_bdot_mag_control *input, CANPacket *output){
    output -> id = 302252745;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_mag_control_mode))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_bdot_pole_override(CANPacket *input, gcmd_bdot_pole_override *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_bdot_pole_override_gain_z = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> gcmd_bdot_pole_override_gain_y = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> gcmd_bdot_pole_override_gain_x = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> gcmd_bdot_pole_override_z = (uint8_t) (((fullData & ((uint64_t) 0x3 << 34)) >> 34));
    output -> gcmd_bdot_pole_override_y = (uint8_t) (((fullData & ((uint64_t) 0x3 << 36)) >> 36));
    output -> gcmd_bdot_pole_override_x = (uint8_t) (((fullData & ((uint64_t) 0x3 << 38)) >> 38));
}

void encodegcmd_bdot_pole_override(gcmd_bdot_pole_override *input, CANPacket *output){
    output -> id = 302252744;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_gain_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_gain_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_gain_x))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_z))) & 0x3) << 34;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_y))) & 0x3) << 36;
    fullPacketData |= (((uint64_t)((input -> gcmd_bdot_pole_override_x))) & 0x3) << 38;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_gen_set_pt_state(CANPacket *input, gcmd_gen_set_pt_state *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_gen_set_pt_state_3 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 58)) >> 58));
    output -> gcmd_gen_set_pt_state_2 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 60)) >> 60));
    output -> gcmd_gen_set_pt_state_1 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_gen_set_pt_state(gcmd_gen_set_pt_state *input, CANPacket *output){
    output -> id = 302252742;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_gen_set_pt_state_3))) & 0x3) << 58;
    fullPacketData |= (((uint64_t)((input -> gcmd_gen_set_pt_state_2))) & 0x3) << 60;
    fullPacketData |= (((uint64_t)((input -> gcmd_gen_set_pt_state_1))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_ppt(CANPacket *input, gcmd_dist_set_pd_ovc_ppt *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_ppt_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_ppt_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_ppt_ovc))));
}

void encodegcmd_dist_set_pd_ovc_ppt(gcmd_dist_set_pd_ovc_ppt *input, CANPacket *output){
    output -> id = 302252741;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_ppt_ovc = ((input -> gcmd_dist_set_pd_ovc_ppt_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_ppt_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_eps(CANPacket *input, gcmd_dist_set_pd_ovc_eps *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_eps_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_eps_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_eps_ovc))));
}

void encodegcmd_dist_set_pd_ovc_eps(gcmd_dist_set_pd_ovc_eps *input, CANPacket *output){
    output -> id = 302252740;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_eps_ovc = ((input -> gcmd_dist_set_pd_ovc_eps_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_eps_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_estim(CANPacket *input, gcmd_dist_set_pd_ovc_estim *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_estim_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_estim_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_estim_ovc))));
}

void encodegcmd_dist_set_pd_ovc_estim(gcmd_dist_set_pd_ovc_estim *input, CANPacket *output){
    output -> id = 302252738;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_estim_ovc = ((input -> gcmd_dist_set_pd_ovc_estim_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_estim_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_bdot(CANPacket *input, gcmd_dist_set_pd_ovc_bdot *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_bdot_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_bdot_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_bdot_ovc))));
}

void encodegcmd_dist_set_pd_ovc_bdot(gcmd_dist_set_pd_ovc_bdot *input, CANPacket *output){
    output -> id = 302252737;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_bdot_ovc = ((input -> gcmd_dist_set_pd_ovc_bdot_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_bdot_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_rahs(CANPacket *input, gcmd_dist_set_pd_ovc_rahs *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_rahs_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_rahs_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_rahs_ovc))));
}

void encodegcmd_dist_set_pd_ovc_rahs(gcmd_dist_set_pd_ovc_rahs *input, CANPacket *output){
    output -> id = 302252736;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_rahs_ovc = ((input -> gcmd_dist_set_pd_ovc_rahs_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_rahs_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_ovc_com2(CANPacket *input, gcmd_dist_set_pd_ovc_com2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempgcmd_dist_set_pd_ovc_com2_ovc = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> gcmd_dist_set_pd_ovc_com2_ovc = (*((float *)(&(tempgcmd_dist_set_pd_ovc_com2_ovc))));
}

void encodegcmd_dist_set_pd_ovc_com2(gcmd_dist_set_pd_ovc_com2 *input, CANPacket *output){
    output -> id = 302252735;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempgcmd_dist_set_pd_ovc_com2_ovc = ((input -> gcmd_dist_set_pd_ovc_com2_ovc));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempgcmd_dist_set_pd_ovc_com2_ovc))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_dist_set_pd_state(CANPacket *input, gcmd_dist_set_pd_state *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_dist_set_pd_state_wheels = (uint8_t) (((fullData & ((uint64_t) 0x3 << 52)) >> 52));
    output -> gcmd_dist_set_pd_state_rahs = (uint8_t) (((fullData & ((uint64_t) 0x3 << 58)) >> 58));
    output -> gcmd_dist_set_pd_state_ppt = (uint8_t) (((fullData & ((uint64_t) 0x3 << 48)) >> 48));
    output -> gcmd_dist_set_pd_state_estim = (uint8_t) (((fullData & ((uint64_t) 0x3 << 54)) >> 54));
    output -> gcmd_dist_set_pd_state_eps = (uint8_t) (((fullData & ((uint64_t) 0x3 << 50)) >> 50));
    output -> gcmd_dist_set_pd_state_com2 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 60)) >> 60));
    output -> gcmd_dist_set_pd_state_com1 = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
    output -> gcmd_dist_set_pd_state_bdot = (uint8_t) (((fullData & ((uint64_t) 0x3 << 56)) >> 56));
}

void encodegcmd_dist_set_pd_state(gcmd_dist_set_pd_state *input, CANPacket *output){
    output -> id = 302252733;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_wheels))) & 0x3) << 52;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_rahs))) & 0x3) << 58;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_ppt))) & 0x3) << 48;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_estim))) & 0x3) << 54;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_eps))) & 0x3) << 50;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_com2))) & 0x3) << 60;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_com1))) & 0x3) << 62;
    fullPacketData |= (((uint64_t)((input -> gcmd_dist_set_pd_state_bdot))) & 0x3) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_mtq_pop(CANPacket *input, gcmd_mtq_pop *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_mtq_pop_z = (uint8_t) (((fullData & ((uint64_t) 0x3 << 58)) >> 58));
    output -> gcmd_mtq_pop_y = (uint8_t) (((fullData & ((uint64_t) 0x3 << 60)) >> 60));
    output -> gcmd_mtq_pop_x = (uint8_t) (((fullData & ((uint64_t) 0x3 << 62)) >> 62));
}

void encodegcmd_mtq_pop(gcmd_mtq_pop *input, CANPacket *output){
    output -> id = 302449340;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pop_z))) & 0x3) << 58;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pop_y))) & 0x3) << 60;
    fullPacketData |= (((uint64_t)((input -> gcmd_mtq_pop_x))) & 0x3) << 62;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_7(CANPacket *input, rc_eps_batt_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_7_acc_charge_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_7_acc_charge_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_7_acc_charge_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_batt_7(rc_eps_batt_7 *input, CANPacket *output){
    output -> id = 307823194;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_7_acc_charge_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_7_acc_charge_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_7_acc_charge_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_mag2(CANPacket *input, sensorproc_mag2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_mag2_bdot_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 14)) >> 14));
    output -> sensorproc_mag2_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_mag2_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_mag2_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> sensorproc_mag2_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encodesensorproc_mag2(sensorproc_mag2 *input, CANPacket *output){
    output -> id = 335872068;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag2_bdot_valid))) & 0x1) << 14;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag2_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag2_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag2_x))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag2_valid))) & 0x1) << 15;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_14(CANPacket *input, rc_adcs_estim_14 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_14_mag_z = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_14_mag_z = (*((double *)(&(temprc_adcs_estim_14_mag_z))));
}

void encoderc_adcs_estim_14(rc_adcs_estim_14 *input, CANPacket *output){
    output -> id = 307823193;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_14_mag_z = ((input -> rc_adcs_estim_14_mag_z));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_14_mag_z)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_13(CANPacket *input, rc_adcs_estim_13 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_13_mag_y = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_13_mag_y = (*((double *)(&(temprc_adcs_estim_13_mag_y))));
}

void encoderc_adcs_estim_13(rc_adcs_estim_13 *input, CANPacket *output){
    output -> id = 307823192;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_13_mag_y = ((input -> rc_adcs_estim_13_mag_y));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_13_mag_y)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_12(CANPacket *input, rc_adcs_estim_12 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_12_mag_x = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_12_mag_x = (*((double *)(&(temprc_adcs_estim_12_mag_x))));
}

void encoderc_adcs_estim_12(rc_adcs_estim_12 *input, CANPacket *output){
    output -> id = 307823191;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_12_mag_x = ((input -> rc_adcs_estim_12_mag_x));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_12_mag_x)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_11(CANPacket *input, rc_adcs_estim_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_11_sun_z = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_11_sun_z = (*((double *)(&(temprc_adcs_estim_11_sun_z))));
}

void encoderc_adcs_estim_11(rc_adcs_estim_11 *input, CANPacket *output){
    output -> id = 307823190;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_11_sun_z = ((input -> rc_adcs_estim_11_sun_z));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_11_sun_z)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_10(CANPacket *input, rc_adcs_estim_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_10_sun_y = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_10_sun_y = (*((double *)(&(temprc_adcs_estim_10_sun_y))));
}

void encoderc_adcs_estim_10(rc_adcs_estim_10 *input, CANPacket *output){
    output -> id = 307823189;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_10_sun_y = ((input -> rc_adcs_estim_10_sun_y));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_10_sun_y)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_9(CANPacket *input, rc_adcs_estim_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_9_sun_x = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_9_sun_x = (*((double *)(&(temprc_adcs_estim_9_sun_x))));
}

void encoderc_adcs_estim_9(rc_adcs_estim_9 *input, CANPacket *output){
    output -> id = 307823188;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_9_sun_x = ((input -> rc_adcs_estim_9_sun_x));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_9_sun_x)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_16(CANPacket *input, rc_eps_dist_16 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_16_ppt_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_16_ppt_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_16_ppt_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_16_ppt_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_16(rc_eps_dist_16 *input, CANPacket *output){
    output -> id = 307823186;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_16_ppt_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_16_ppt_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_16_ppt_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_16_ppt_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_9(CANPacket *input, rc_eps_dist_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_9_rahs_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_9_rahs_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_9_rahs_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_9(rc_eps_dist_9 *input, CANPacket *output){
    output -> id = 307823179;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_9_rahs_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_9_rahs_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_9_rahs_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_7(CANPacket *input, rc_eps_dist_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_7_com2_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_7_com2_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_7_com2_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_7(rc_eps_dist_7 *input, CANPacket *output){
    output -> id = 307823177;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_7_com2_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_7_com2_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_7_com2_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_6(CANPacket *input, rc_eps_dist_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_6_com2_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_6_com2_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_6_com2_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_6_com2_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_6(rc_eps_dist_6 *input, CANPacket *output){
    output -> id = 307823176;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_6_com2_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_6_com2_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_6_com2_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_6_com2_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_5(CANPacket *input, rc_eps_dist_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_5_com1_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_5_com1_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_5_com1_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_5(rc_eps_dist_5 *input, CANPacket *output){
    output -> id = 307823175;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_5_com1_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_5_com1_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_5_com1_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_3(CANPacket *input, rc_eps_dist_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_3_batt_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_dist_3_batt_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_3_batt_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_dist_3(rc_eps_dist_3 *input, CANPacket *output){
    output -> id = 307823173;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_3_batt_v_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_3_batt_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_3_batt_v_min))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_2(CANPacket *input, rc_eps_dist_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_2_uv_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_2_met_overflow = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_eps_dist_2_met = (uint64_t) (((fullData & ((uint64_t) 0xffffffff << 16)) >> 16));
}

void encoderc_eps_dist_2(rc_eps_dist_2 *input, CANPacket *output){
    output -> id = 307823172;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_2_uv_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_2_met_overflow))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_2_met))) & 0xffffffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_17(CANPacket *input, rc_eps_dist_17 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_17_ppt_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_17_ppt_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_17_ppt_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_17(rc_eps_dist_17 *input, CANPacket *output){
    output -> id = 307823187;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_17_ppt_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_17_ppt_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_17_ppt_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_14(CANPacket *input, rc_eps_dist_14 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_14_eps_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_14_eps_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_14_eps_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_14_eps_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_14(rc_eps_dist_14 *input, CANPacket *output){
    output -> id = 307823184;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_14_eps_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_14_eps_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_14_eps_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_14_eps_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_13(CANPacket *input, rc_eps_dist_13 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_13_estim_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_13_estim_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_13_estim_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_13(rc_eps_dist_13 *input, CANPacket *output){
    output -> id = 307823183;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_13_estim_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_13_estim_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_13_estim_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_11(CANPacket *input, rc_eps_dist_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_11_bdot_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_11_bdot_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_11_bdot_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_11(rc_eps_dist_11 *input, CANPacket *output){
    output -> id = 307823181;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_11_bdot_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_11_bdot_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_11_bdot_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_12(CANPacket *input, rc_eps_dist_12 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_12_estim_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_12_estim_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_12_estim_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_12_estim_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_12(rc_eps_dist_12 *input, CANPacket *output){
    output -> id = 307823182;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_12_estim_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_12_estim_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_12_estim_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_12_estim_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_15(CANPacket *input, rc_eps_dist_15 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_15_eps_v_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_dist_15_eps_v_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_15_eps_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_dist_15(rc_eps_dist_15 *input, CANPacket *output){
    output -> id = 307823185;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_15_eps_v_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_15_eps_v_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_15_eps_v_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_10(CANPacket *input, rc_eps_dist_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_10_bdot_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_10_bdot_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_10_bdot_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_10_bdot_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_10(rc_eps_dist_10 *input, CANPacket *output){
    output -> id = 307823180;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_10_bdot_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_10_bdot_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_10_bdot_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_10_bdot_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_8(CANPacket *input, rc_eps_dist_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_8_rahs_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_8_rahs_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_8_rahs_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_8_rahs_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_8(rc_eps_dist_8 *input, CANPacket *output){
    output -> id = 307823178;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_8_rahs_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_8_rahs_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_8_rahs_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_8_rahs_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_4(CANPacket *input, rc_eps_dist_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_4_com1_state = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_dist_4_com1_c_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_eps_dist_4_com1_c_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_eps_dist_4_com1_c_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
}

void encoderc_eps_dist_4(rc_eps_dist_4 *input, CANPacket *output){
    output -> id = 308871750;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_4_com1_state))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_4_com1_c_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_4_com1_c_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_4_com1_c_avg))) & 0xffff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_dist_1(CANPacket *input, rc_eps_dist_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_dist_1_temp_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_dist_1_com1_c_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_dist_1_batt_v_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_dist_1(rc_eps_dist_1 *input, CANPacket *output){
    output -> id = 309330499;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_1_temp_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_1_com1_c_avg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_dist_1_batt_v_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_11(CANPacket *input, rc_adcs_mpc_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_11_omega_z = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_11_omega_z = (*((double *)(&(temprc_adcs_mpc_11_omega_z))));
}

void encoderc_adcs_mpc_11(rc_adcs_mpc_11 *input, CANPacket *output){
    output -> id = 307823166;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_11_omega_z = ((input -> rc_adcs_mpc_11_omega_z));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_11_omega_z)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_7(CANPacket *input, rc_adcs_mpc_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_7_omega_max = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_7_omega_max = (*((double *)(&(temprc_adcs_mpc_7_omega_max))));
}

void encoderc_adcs_mpc_7(rc_adcs_mpc_7 *input, CANPacket *output){
    output -> id = 307823162;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_7_omega_max = ((input -> rc_adcs_mpc_7_omega_max));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_7_omega_max)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_8(CANPacket *input, rc_adcs_mpc_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_8_omega_avg = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_8_omega_avg = (*((double *)(&(temprc_adcs_mpc_8_omega_avg))));
}

void encoderc_adcs_mpc_8(rc_adcs_mpc_8 *input, CANPacket *output){
    output -> id = 307823163;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_8_omega_avg = ((input -> rc_adcs_mpc_8_omega_avg));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_8_omega_avg)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_9(CANPacket *input, rc_adcs_mpc_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_9_omega_x = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_9_omega_x = (*((double *)(&(temprc_adcs_mpc_9_omega_x))));
}

void encoderc_adcs_mpc_9(rc_adcs_mpc_9 *input, CANPacket *output){
    output -> id = 307823164;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_9_omega_x = ((input -> rc_adcs_mpc_9_omega_x));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_9_omega_x)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_5(CANPacket *input, rc_adcs_mpc_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_5_sc_quat_4 = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_5_sc_quat_4 = (*((double *)(&(temprc_adcs_mpc_5_sc_quat_4))));
}

void encoderc_adcs_mpc_5(rc_adcs_mpc_5 *input, CANPacket *output){
    output -> id = 307823160;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_5_sc_quat_4 = ((input -> rc_adcs_mpc_5_sc_quat_4));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_5_sc_quat_4)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_6(CANPacket *input, rc_adcs_mpc_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_6_omega_min = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_6_omega_min = (*((double *)(&(temprc_adcs_mpc_6_omega_min))));
}

void encoderc_adcs_mpc_6(rc_adcs_mpc_6 *input, CANPacket *output){
    output -> id = 307823161;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_6_omega_min = ((input -> rc_adcs_mpc_6_omega_min));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_6_omega_min)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_4(CANPacket *input, rc_adcs_mpc_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_4_sc_quat_3 = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_4_sc_quat_3 = (*((double *)(&(temprc_adcs_mpc_4_sc_quat_3))));
}

void encoderc_adcs_mpc_4(rc_adcs_mpc_4 *input, CANPacket *output){
    output -> id = 307823159;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_4_sc_quat_3 = ((input -> rc_adcs_mpc_4_sc_quat_3));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_4_sc_quat_3)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_3(CANPacket *input, rc_adcs_mpc_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_3_sc_quat_2 = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_3_sc_quat_2 = (*((double *)(&(temprc_adcs_mpc_3_sc_quat_2))));
}

void encoderc_adcs_mpc_3(rc_adcs_mpc_3 *input, CANPacket *output){
    output -> id = 307823158;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_3_sc_quat_2 = ((input -> rc_adcs_mpc_3_sc_quat_2));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_3_sc_quat_2)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_15(CANPacket *input, rc_adcs_mpc_15 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mpc_15_sc_mode = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_adcs_mpc_15_point_true = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
}

void encoderc_adcs_mpc_15(rc_adcs_mpc_15 *input, CANPacket *output){
    output -> id = 307823170;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_15_sc_mode))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mpc_15_point_true))) & 0xff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_12(CANPacket *input, rc_adcs_mpc_12 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_mpc_12(rc_adcs_mpc_12 *input, CANPacket *output){
    output -> id = 307823167;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_13(CANPacket *input, rc_adcs_mpc_13 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_mpc_13(rc_adcs_mpc_13 *input, CANPacket *output){
    output -> id = 307823168;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_14(CANPacket *input, rc_adcs_mpc_14 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_mpc_14(rc_adcs_mpc_14 *input, CANPacket *output){
    output -> id = 307823169;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_10(CANPacket *input, rc_adcs_mpc_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_10_omega_y = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_10_omega_y = (*((double *)(&(temprc_adcs_mpc_10_omega_y))));
}

void encoderc_adcs_mpc_10(rc_adcs_mpc_10 *input, CANPacket *output){
    output -> id = 307823165;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_10_omega_y = ((input -> rc_adcs_mpc_10_omega_y));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_10_omega_y)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_2(CANPacket *input, rc_adcs_mpc_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_mpc_2_sc_quat_1 = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_mpc_2_sc_quat_1 = (*((double *)(&(temprc_adcs_mpc_2_sc_quat_1))));
}

void encoderc_adcs_mpc_2(rc_adcs_mpc_2 *input, CANPacket *output){
    output -> id = 307823157;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_mpc_2_sc_quat_1 = ((input -> rc_adcs_mpc_2_sc_quat_1));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_mpc_2_sc_quat_1)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_mpc_1(CANPacket *input, rc_adcs_mpc_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_mpc_1(rc_adcs_mpc_1 *input, CANPacket *output){
    output -> id = 307823156;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_8(CANPacket *input, rc_adcs_estim_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_estim_8_tle_id = (uint8_t) (((fullData & ((uint64_t) 0x1 << 13)) >> 13));
    output -> rc_adcs_estim_8_sgp4_flag = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_estim_8_sc_in_sun = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
    output -> rc_adcs_estim_8_sc_above_gs = (uint8_t) (((fullData & ((uint64_t) 0x1 << 14)) >> 14));
    output -> rc_adcs_estim_8_epoch_overflow = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_estim_8_epoch = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 32)) >> 32));
}

void encoderc_adcs_estim_8(rc_adcs_estim_8 *input, CANPacket *output){
    output -> id = 307823155;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_tle_id))) & 0x1) << 13;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_sgp4_flag))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_sc_in_sun))) & 0x1) << 15;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_sc_above_gs))) & 0x1) << 14;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_epoch_overflow))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_estim_8_epoch))) & 0xffffffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_7(CANPacket *input, rc_adcs_estim_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_7_vel_z = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_7_vel_z = (*((double *)(&(temprc_adcs_estim_7_vel_z))));
}

void encoderc_adcs_estim_7(rc_adcs_estim_7 *input, CANPacket *output){
    output -> id = 307823154;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_7_vel_z = ((input -> rc_adcs_estim_7_vel_z));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_7_vel_z)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_6(CANPacket *input, rc_adcs_estim_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_6_vel_y = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_6_vel_y = (*((double *)(&(temprc_adcs_estim_6_vel_y))));
}

void encoderc_adcs_estim_6(rc_adcs_estim_6 *input, CANPacket *output){
    output -> id = 307823153;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_6_vel_y = ((input -> rc_adcs_estim_6_vel_y));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_6_vel_y)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_5(CANPacket *input, rc_adcs_estim_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_5_vel_x = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_5_vel_x = (*((double *)(&(temprc_adcs_estim_5_vel_x))));
}

void encoderc_adcs_estim_5(rc_adcs_estim_5 *input, CANPacket *output){
    output -> id = 307823152;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_5_vel_x = ((input -> rc_adcs_estim_5_vel_x));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_5_vel_x)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_4(CANPacket *input, rc_adcs_estim_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_4_pos_z = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_4_pos_z = (*((double *)(&(temprc_adcs_estim_4_pos_z))));
}

void encoderc_adcs_estim_4(rc_adcs_estim_4 *input, CANPacket *output){
    output -> id = 307823151;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_4_pos_z = ((input -> rc_adcs_estim_4_pos_z));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_4_pos_z)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_2(CANPacket *input, rc_adcs_estim_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_2_pos_x = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_2_pos_x = (*((double *)(&(temprc_adcs_estim_2_pos_x))));
}

void encoderc_adcs_estim_2(rc_adcs_estim_2 *input, CANPacket *output){
    output -> id = 307823149;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_2_pos_x = ((input -> rc_adcs_estim_2_pos_x));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_2_pos_x)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_3(CANPacket *input, rc_adcs_estim_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temprc_adcs_estim_3_pos_y = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> rc_adcs_estim_3_pos_y = (*((double *)(&(temprc_adcs_estim_3_pos_y))));
}

void encoderc_adcs_estim_3(rc_adcs_estim_3 *input, CANPacket *output){
    output -> id = 307823150;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temprc_adcs_estim_3_pos_y = ((input -> rc_adcs_estim_3_pos_y));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temprc_adcs_estim_3_pos_y)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_estim_1(CANPacket *input, rc_adcs_estim_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_estim_1(rc_adcs_estim_1 *input, CANPacket *output){
    output -> id = 307823148;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_12(CANPacket *input, rc_adcs_sp_12 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_12_mag2_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_12_mag2_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_12_mag2_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_12_mag2_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_sp_12(rc_adcs_sp_12 *input, CANPacket *output){
    output -> id = 307823142;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_12_mag2_z_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_12_mag2_z_max))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_12_mag2_y_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_12_mag2_y_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_11(CANPacket *input, rc_adcs_sp_11 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_11_mag2_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_11_mag2_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_11_mag2_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_11_mag2_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
}

void encoderc_adcs_sp_11(rc_adcs_sp_11 *input, CANPacket *output){
    output -> id = 307823141;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_11_mag2_x_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_11_mag2_x_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_11_mag2_x_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_11_mag2_y_min))) & 0xffff);
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_7(CANPacket *input, rc_adcs_sp_7 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_7_mag1_var_z = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_7_mag1_var_y = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_7_mag1_var_x = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_sp_7(rc_adcs_sp_7 *input, CANPacket *output){
    output -> id = 307823137;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_7_mag1_var_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_7_mag1_var_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_7_mag1_var_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_6(CANPacket *input, rc_adcs_sp_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_6_mag2_valid = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_sp_6_mag1_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_6_mag1_valid = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_sp_6_sun_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_6_sun_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_sp_6(rc_adcs_sp_6 *input, CANPacket *output){
    output -> id = 307823136;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_6_mag2_valid))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_6_mag1_x_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_6_mag1_valid))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_6_sun_z_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_6_sun_z_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_17(CANPacket *input, rc_adcs_sp_17 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_17_i2c_result_sun = (uint8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_sp_17_i2c_result_imu = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_sp_17_i2c_result_mag_2 = (uint8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_sp_17_i2c_result_mag_1 = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_sp_17_imu_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_17_imu_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_sp_17(rc_adcs_sp_17 *input, CANPacket *output){
    output -> id = 307823147;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_i2c_result_sun))) & 0xff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_i2c_result_imu))) & 0xff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_i2c_result_mag_2))) & 0xff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_i2c_result_mag_1))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_imu_z_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_17_imu_z_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_16(CANPacket *input, rc_adcs_sp_16 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_16_imu_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_16_imu_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_16_imu_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_16_imu_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_adcs_sp_16(rc_adcs_sp_16 *input, CANPacket *output){
    output -> id = 307823146;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_16_imu_z_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_16_imu_y_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_16_imu_y_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_16_imu_y_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_10(CANPacket *input, rc_adcs_sp_10 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_10_mag1_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_10_mag1_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_10_mag1_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_10_mag1_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_sp_10(rc_adcs_sp_10 *input, CANPacket *output){
    output -> id = 307823140;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_10_mag1_z_min))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_10_mag1_z_max))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_10_mag1_z_avg))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_10_mag1_y_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_8(CANPacket *input, rc_adcs_sp_8 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_8_mag2_var_z = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_8_mag2_var_y = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_8_mag2_var_x = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_sp_8(rc_adcs_sp_8 *input, CANPacket *output){
    output -> id = 307823138;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_8_mag2_var_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_8_mag2_var_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_8_mag2_var_x))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_9(CANPacket *input, rc_adcs_sp_9 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_9_mag1_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_9_mag1_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_9_mag1_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_9_mag1_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_sp_9(rc_adcs_sp_9 *input, CANPacket *output){
    output -> id = 307823139;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_9_mag1_y_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_9_mag1_y_max))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_9_mag1_x_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_9_mag1_x_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_13(CANPacket *input, rc_adcs_sp_13 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_13_suna_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_13_suna_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_13_suna_avg = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_13_mag2_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_sp_13(rc_adcs_sp_13 *input, CANPacket *output){
    output -> id = 307823143;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_13_suna_min))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_13_suna_max))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_13_suna_avg))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_13_mag2_z_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_15(CANPacket *input, rc_adcs_sp_15 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_15_imu_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 40)) >> 40));
    output -> rc_adcs_sp_15_imu_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 24)) >> 24));
    output -> rc_adcs_sp_15_imu_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 8)) >> 8));
    output -> rc_adcs_sp_15_imu_valid = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_sp_15(rc_adcs_sp_15 *input, CANPacket *output){
    output -> id = 307823145;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_15_imu_x_min))) & 0xffff) << 40;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_15_imu_x_max))) & 0xffff) << 24;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_15_imu_x_avg))) & 0xffff) << 8;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_15_imu_valid))) & 0xff) << 56;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_14(CANPacket *input, rc_adcs_sp_14 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_14_sunb_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_14_sunb_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_14_sunb_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_14_sun_valid = (uint8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_adcs_sp_14(rc_adcs_sp_14 *input, CANPacket *output){
    output -> id = 307823144;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_14_sunb_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_14_sunb_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_14_sunb_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_14_sun_valid))) & 0xff) << 8;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_5(CANPacket *input, rc_adcs_sp_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_5_sun_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_5_sun_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_5_sun_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_5_sun_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_adcs_sp_5(rc_adcs_sp_5 *input, CANPacket *output){
    output -> id = 307823135;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_5_sun_z_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_5_sun_y_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_5_sun_y_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_5_sun_y_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_4(CANPacket *input, rc_adcs_sp_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_4_sun_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_4_sun_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_4_sun_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_4_imup_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_sp_4(rc_adcs_sp_4 *input, CANPacket *output){
    output -> id = 307823134;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_4_sun_x_min))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_4_sun_x_max))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_4_sun_x_avg))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_4_imup_z_avg))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_3(CANPacket *input, rc_adcs_sp_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_3_imup_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_sp_3_imup_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_3_imup_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_3_imup_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_sp_3(rc_adcs_sp_3 *input, CANPacket *output){
    output -> id = 307823133;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_3_imup_z_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_3_imup_z_max))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_3_imup_y_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_3_imup_y_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_2(CANPacket *input, rc_adcs_sp_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_sp_2_imup_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_sp_2_imup_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_sp_2_imup_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_sp_2_imup_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_adcs_sp_2(rc_adcs_sp_2 *input, CANPacket *output){
    output -> id = 307823132;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_2_imup_y_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_2_imup_x_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_2_imup_x_max))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_sp_2_imup_x_avg))) & 0xffff) << 16;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_sp_1(CANPacket *input, rc_adcs_sp_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
}

void encoderc_adcs_sp_1(rc_adcs_sp_1 *input, CANPacket *output){
    output -> id = 307823131;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_4(CANPacket *input, rc_adcs_bdot_4 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_4_spam_on_y_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_4_spam_on_y_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_4_mag_z_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_4_tumble = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encoderc_adcs_bdot_4(rc_adcs_bdot_4 *input, CANPacket *output){
    output -> id = 307823130;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_4_spam_on_y_mtq_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_4_spam_on_y_mtq_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_4_mag_z_avg))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_4_tumble))) & 0x1) << 15;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_3(CANPacket *input, rc_adcs_bdot_3 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_3_mag_z_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_3_mag_z_max = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_bdot_3_mag_y_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_3_mag_y_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_bdot_3(rc_adcs_bdot_3 *input, CANPacket *output){
    output -> id = 307823129;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_mag_z_min))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_mag_z_max))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_mag_y_max))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_3_mag_y_avg))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_2(CANPacket *input, rc_adcs_bdot_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_2_mag_y_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_bdot_2_mag_x_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_2_mag_x_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_adcs_bdot_2_mag_x_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
}

void encoderc_adcs_bdot_2(rc_adcs_bdot_2 *input, CANPacket *output){
    output -> id = 307823111;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_y_min))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_x_avg))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_x_min))) & 0xffff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_2_mag_x_max))) & 0xffff) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_adcs_bdot_1(CANPacket *input, rc_adcs_bdot_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_bdot_1_spam_on_y_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_bdot_1_spam_on_x_mtq_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_bdot_1_spam_on_x_mtq_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_bdot_1_spam_on_x_mtq_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_adcs_bdot_1(rc_adcs_bdot_1 *input, CANPacket *output){
    output -> id = 307823110;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_spam_on_y_mtq_x))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_spam_on_x_mtq_z))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_spam_on_x_mtq_y))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_bdot_1_spam_on_x_mtq_x))) & 0xffff) << 48;
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

void decoderc_adcs_mtq_5(CANPacket *input, rc_adcs_mtq_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_adcs_mtq_5_cmds_z_var = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_adcs_mtq_5_cmds_y_var = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_adcs_mtq_5_cmds_x_var = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_adcs_mtq_5_reset_counts = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_5_fsw_ignore = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_5(rc_adcs_mtq_5 *input, CANPacket *output){
    output -> id = 308871695;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_5_cmds_z_var))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_5_cmds_y_var))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_adcs_mtq_5_cmds_x_var))) & 0xffff) << 32;
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
    output -> id = 308871694;
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
    output -> rc_adcs_mtq_3_fsw_z_avg = (int8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_mtq_3_fsw_y_min = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_3_fsw_y_max = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_mtq_3_fsw_y_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_mtq_3_fsw_x_min = (int8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_adcs_mtq_3_fsw_x_max = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_adcs_mtq_3_fsw_x_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_3_bdot_z_min = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_3(rc_adcs_mtq_3 *input, CANPacket *output){
    output -> id = 308871693;
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
    output -> rc_adcs_mtq_2_bdot_z_max = (int8_t) (((fullData & ((uint64_t) 0xff))));
    output -> rc_adcs_mtq_2_bdot_z_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
    output -> rc_adcs_mtq_2_bdot_y_min = (int8_t) (((fullData & ((uint64_t) 0xff << 16)) >> 16));
    output -> rc_adcs_mtq_2_bdot_y_max = (int8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> rc_adcs_mtq_2_bdot_y_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 32)) >> 32));
    output -> rc_adcs_mtq_2_bdot_x_min = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> rc_adcs_mtq_2_bdot_x_max = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_adcs_mtq_2_bdot_x_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encoderc_adcs_mtq_2(rc_adcs_mtq_2 *input, CANPacket *output){
    output -> id = 308871692;
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

void decoderc_ppt_2(CANPacket *input, rc_ppt_2 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_2_cooldown_time = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_ppt_2_ign_charge_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_ppt_2_main_ign_delay = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_2_main_charge_time = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_ppt_2(rc_ppt_2 *input, CANPacket *output){
    output -> id = 307823113;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_cooldown_time))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_ign_charge_time))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_main_ign_delay))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_2_main_charge_time))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_ppt_1(CANPacket *input, rc_ppt_1 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_ppt_1_smt_wait_time = (uint16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_ppt_1_last_main_charge = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_ppt_1_fault_count = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_ppt_1_fire_count = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_ppt_1(rc_ppt_1 *input, CANPacket *output){
    output -> id = 307823112;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_smt_wait_time))) & 0xffff);
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_last_main_charge))) & 0xffff) << 16;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_fault_count))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_ppt_1_fire_count))) & 0xffff) << 48;
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
    output -> id = 307823128;
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
    output -> rc_eps_gen_8_pnl_3_power_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_8_pnl_3_power_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_8_pnl_3_power_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_8_pnl_1_temp_min = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_gen_8(rc_eps_gen_8 *input, CANPacket *output){
    output -> id = 307823127;
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
    output -> rc_eps_gen_7_pnl_2_power_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_7_pnl_2_power_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_7_pnl_2_power_avg = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_7_pnl_1_power_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_gen_7(rc_eps_gen_7 *input, CANPacket *output){
    output -> id = 307823126;
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
    output -> rc_eps_gen_6_pnl_3_current_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_6_pnl_3_current_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_6_pnl_1_power_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_6_pnl_1_power_max = (int16_t) (((fullData & ((uint64_t) 0xffff))));
}

void encoderc_eps_gen_6(rc_eps_gen_6 *input, CANPacket *output){
    output -> id = 307823125;
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
    output -> rc_eps_gen_5_pnl_3_current_min = (int16_t) (((fullData & ((uint64_t) 0xffff))));
    output -> rc_eps_gen_5_pnl_2_current_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_gen_5_pnl_2_current_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_5_pnl_2_current_avg = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
}

void encoderc_eps_gen_5(rc_eps_gen_5 *input, CANPacket *output){
    output -> id = 307823124;
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
    output -> rc_eps_gen_4_pnl_1_current_min = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_gen_4_pnl_1_current_max = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_gen_4_pnl_1_current_avg = (int16_t) (((fullData & ((uint64_t) 0xffff))));
}

void encoderc_eps_gen_4(rc_eps_gen_4 *input, CANPacket *output){
    output -> id = 307823123;
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
    output -> id = 307823122;
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
    output -> id = 307823121;
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
    output -> rc_eps_gen_1_pnl_3_charging = (uint8_t) (((fullData & ((uint64_t) 0x1 << 58)) >> 58));
    output -> rc_eps_gen_1_pnl_2_charging = (uint8_t) (((fullData & ((uint64_t) 0x1 << 59)) >> 59));
    output -> rc_eps_gen_1_pnl_1_charging = (uint8_t) (((fullData & ((uint64_t) 0x1 << 60)) >> 60));
    output -> rc_eps_gen_1_pnl_3_enabled = (uint8_t) (((fullData & ((uint64_t) 0x1 << 61)) >> 61));
    output -> rc_eps_gen_1_pnl_2_enabled = (uint8_t) (((fullData & ((uint64_t) 0x1 << 62)) >> 62));
    output -> rc_eps_gen_1_pnl_1_enabled = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encoderc_eps_gen_1(rc_eps_gen_1 *input, CANPacket *output){
    output -> id = 307823120;
    output -> length = 6;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_3_charging))) & 0x1) << 58;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_2_charging))) & 0x1) << 59;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_1_charging))) & 0x1) << 60;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_3_enabled))) & 0x1) << 61;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_2_enabled))) & 0x1) << 62;
    fullPacketData |= (((uint64_t)((input -> rc_eps_gen_1_pnl_1_enabled))) & 0x1) << 63;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decoderc_eps_batt_6(CANPacket *input, rc_eps_batt_6 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> rc_eps_batt_6_status = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
    output -> rc_eps_batt_6_ctrl = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> rc_eps_batt_6_last_charge = (uint64_t) (((fullData & ((uint64_t) 0xffffffffff << 8)) >> 8));
}

void encoderc_eps_batt_6(rc_eps_batt_6 *input, CANPacket *output){
    output -> id = 308871685;
    output -> length = 7;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_status))) & 0xff) << 56;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_ctrl))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_6_last_charge))) & 0xffffffffff) << 8;
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
    output -> id = 308871684;
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
    output -> rc_eps_batt_4_bal_auto_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 7)) >> 7));
    output -> rc_eps_batt_4_heater_auto_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 13)) >> 13));
    output -> rc_eps_batt_4_voltage_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_4_voltage_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_4_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_4_heater_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 14)) >> 14));
    output -> rc_eps_batt_4_balancer_state = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encoderc_eps_batt_4(rc_eps_batt_4 *input, CANPacket *output){
    output -> id = 307823107;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_bal_auto_state))) & 0x1) << 7;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_4_heater_auto_state))) & 0x1) << 13;
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
    output -> rc_eps_batt_3_current_min = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> rc_eps_batt_3_current_max = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_3_current_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> rc_eps_batt_3_batt_temp_avg = (int8_t) (((fullData & ((uint64_t) 0xff << 8)) >> 8));
}

void encoderc_eps_batt_3(rc_eps_batt_3 *input, CANPacket *output){
    output -> id = 308871682;
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
    output -> id = 308871681;
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
    output -> rc_eps_batt_1_voltage_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> rc_eps_batt_1_acc_charge_avg = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encoderc_eps_batt_1(rc_eps_batt_1 *input, CANPacket *output){
    output -> id = 309920256;
    output -> length = 4;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_voltage_avg))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> rc_eps_batt_1_acc_charge_avg))) & 0xffff) << 48;
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

void decodetle_5(CANPacket *input, tle_5 *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t temptle_5_mnm = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> tle_5_mnm = (*((double *)(&(temptle_5_mnm))));
}

void encodetle_5(tle_5 *input, CANPacket *output){
    output -> id = 302448712;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double temptle_5_mnm = ((input -> tle_5_mnm));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(temptle_5_mnm)))));
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
    output -> id = 302448711;
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
    output -> id = 302448710;
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
    output -> id = 302448709;
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
    uint32_t temptle_1_mna = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> tle_1_mna = (*((float *)(&(temptle_1_mna))));
    uint32_t temptle_1_bstar = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> tle_1_bstar = (*((float *)(&(temptle_1_bstar))));
}

void encodetle_1(tle_1 *input, CANPacket *output){
    output -> id = 302448708;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float temptle_1_mna = ((input -> tle_1_mna));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_1_mna)))));
    const float temptle_1_bstar = ((input -> tle_1_bstar));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(temptle_1_bstar))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodegcmd_com2_run(CANPacket *input, gcmd_com2_run *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> gcmd_com2_run_clear = (uint8_t) (((fullData & ((uint64_t) 0x1 << 54)) >> 54));
    output -> gcmd_com2_run_jump = (uint8_t) (((fullData & ((uint64_t) 0x1 << 55)) >> 55));
    output -> gcmd_com2_run_fileno = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodegcmd_com2_run(gcmd_com2_run *input, CANPacket *output){
    output -> id = 302514673;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> gcmd_com2_run_clear))) & 0x1) << 54;
    fullPacketData |= (((uint64_t)((input -> gcmd_com2_run_jump))) & 0x1) << 55;
    fullPacketData |= (((uint64_t)((input -> gcmd_com2_run_fileno))) & 0xff) << 56;
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
    output -> id = 307757552;
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

void decodecmd_reboot_request(CANPacket *input, cmd_reboot_request *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_reboot_request_domain = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodecmd_reboot_request(cmd_reboot_request *input, CANPacket *output){
    output -> id = 262152;
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
    output -> id = 302252068;
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
    output -> id = 302252067;
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

void decodesensorproc_imu(CANPacket *input, sensorproc_imu *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_imu_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_imu_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_imu_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
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
    output -> sensorproc_mag_bdot_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 14)) >> 14));
    output -> sensorproc_mag_z = (int16_t) (((fullData & ((uint64_t) 0xffff << 16)) >> 16));
    output -> sensorproc_mag_y = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_mag_x = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
    output -> sensorproc_mag_valid = (uint8_t) (((fullData & ((uint64_t) 0x1 << 15)) >> 15));
}

void encodesensorproc_mag(sensorproc_mag *input, CANPacket *output){
    output -> id = 335872067;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_mag_bdot_valid))) & 0x1) << 14;
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
    output -> id = 5308436;
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
    output -> id = 307691568;
    output -> length = 8;
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

void decodecmd_ppt_single_fire(CANPacket *input, cmd_ppt_single_fire *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_single_fire_override_smt = (uint8_t) (((fullData & ((uint64_t) 0x1 << 61)) >> 61));
    output -> cmd_ppt_single_fire_override = (uint8_t) (((fullData & ((uint64_t) 0x1 << 62)) >> 62));
    output -> cmd_ppt_single_fire_with_pulse = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_ppt_single_fire(cmd_ppt_single_fire *input, CANPacket *output){
    output -> id = 302252288;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_single_fire_override_smt))) & 0x1) << 61;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_single_fire_override))) & 0x1) << 62;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_single_fire_with_pulse))) & 0x1) << 63;
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
    output -> id = 302448674;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> mpc_vp_status))) & 0x1) << 63;
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
    output -> id = 307691553;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> cmd_mtq_bdot_x))) & 0xff) << 56;
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

