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
        filter_four = 0x04;
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
        filter_four = 0x04;
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
        filter_four = 0x04;
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
  	setTheFilter(CAN_MASK_0, (uint32_t) 0x10000000);
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
    canSend(0,tech, packet->data);
}

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet)) {
    CANPacketReceived = ReceiveCallbackArg;
}

// AUTOGEN STUFF HERE

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

void decodecmd_ppt_fire(CANPacket *input, cmd_ppt_fire *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> cmd_ppt_fire_fire = (uint8_t) (((fullData & ((uint64_t) 0x1 << 63)) >> 63));
}

void encodecmd_ppt_fire(cmd_ppt_fire *input, CANPacket *output){
    output -> id = 34865408;
    output -> length = 1;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> cmd_ppt_fire_fire))) & 0x1) << 63;
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
    output -> sensorproc_sun_status = (uint8_t) (((fullData & ((uint64_t) 0xff << 24)) >> 24));
    output -> sensorproc_sun_reading_beta = (int16_t) (((fullData & ((uint64_t) 0xffff << 32)) >> 32));
    output -> sensorproc_sun_reading_alpha = (int16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodesensorproc_sun(sensorproc_sun *input, CANPacket *output){
    output -> id = 33882177;
    output -> length = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_status))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_reading_beta))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_reading_alpha))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodebdot_command_dipole(CANPacket *input, bdot_command_dipole *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> bdot_command_dipole_z = (int8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> bdot_command_dipole_y = (int8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> bdot_command_dipole_x = (int8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodebdot_command_dipole(bdot_command_dipole *input, CANPacket *output){
    output -> id = 33882145;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> bdot_command_dipole_z))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> bdot_command_dipole_y))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> bdot_command_dipole_x))) & 0xff) << 56;
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

