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
    uint32_t filter_one = 0x0;
    switch(ss) {
      case Module_Test :
        filter_one = 0x5;
        break;
      case Module_ADCS_BDot :
        filter_one = 0x2;
        break;
      case Module_ADCS_RWX :
        filter_one = 0x0;
        break;
      case Module_ADCS_RWY :
        filter_one = 0x0;
        break;
      case Module_ADCS_RWZ :
        filter_one = 0x0;
        break;
      case Module_PPT :
        filter_one = 0x0;
        break;
      case Module_EPS_Dist :
        filter_one = 0x0;
        break;
      case Module_EPS_Gen :
        filter_one = 0x0;
        break;
      case Module_EPS_Batt :
        filter_one = 0x0;
        break;
      case Module_ADCS_Estim :
        filter_one = 0x0;
        break;
      case Module_ADCS_MPC :
        filter_one = 0x0;
        break;
      case Module_ADCS_SensorProc :
        filter_one = 0x0;
        break;
      case Module_ADCS_MTQ :
        filter_one = 0x1000;
        break;
      case Module_COM1 :
        filter_one = 0x0;
        break;
      case Module_COM2 :
        filter_one = 0x0;
        break;
      case Module_RAHS :
        filter_one = 0x0;
        break;
      default :
        filter_one = 0x0;
  	}
  	setTheFilter(CAN_MASK_0, (uint32_t) 0x10000000);
   	setTheFilter(CAN_FILTER_0, (uint32_t) 0x00);
   	setTheFilter(CAN_FILTER_1, (uint32_t) 0x00);

   	setTheFilter(CAN_MASK_1, filter_one);
   	setTheFilter(CAN_FILTER_2, (uint32_t) filter_one);
    setTheFilter(CAN_FILTER_3, (uint32_t) filter_one);
    setTheFilter(CAN_FILTER_4, (uint32_t) filter_one);
    setTheFilter(CAN_FILTER_5, (uint32_t) filter_one);

    int8_t floop;
    uint8_t filterNum = 0;
    for(floop = 28;floop >= 0; floop--){
        if((filter_one >> floop) & 0x01){
            PJDIR |= 0x04;
            PJOUT |= 0x04;
            setTheFilter((filterNum*4+12)-(!filterNum * 4), 0x1 << floop);
            filterNum++;
        }
    }


//    uint8_t debugPollReg = CAN_FILTER_5;
//
//    CANPacket p = {0};
//    p.length = 5;
//    bitModify(MCP_CANCTRL, 0xE0, 0x80);
//    readRegister(debugPollReg + 1, &(p.data[2]));
//    // This line has to be duplicated for it to work.
//    // Because of some weird delay between moving to config mode from normal mode.
//    readRegister(debugPollReg + 1, &(p.data[2]));
//    readRegister(debugPollReg + 2, &(p.data[3]));
//    readRegister(debugPollReg + 3, &(p.data[4]));
//    readRegister(debugPollReg, &(p.data[1]));
//    bitModify(MCP_CANCTRL, 0xE0, 0x00);
//    canSendPacket(&p);
//
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

void decodesensorproc_gps_time(CANPacket *input, sensorproc_gps_time *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_gps_time_ms = (uint32_t) (((fullData & ((uint64_t) 0xffffffff << 16)) >> 16));
    output -> sensorproc_gps_time_wk = (uint16_t) (((fullData & ((uint64_t) 0xffff << 48)) >> 48));
}

void encodesensorproc_gps_time(sensorproc_gps_time *input, CANPacket *output){
    output -> id = 274277376;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_gps_time_ms))) & 0xffffffff) << 16;
    fullPacketData |= (((uint64_t)((input -> sensorproc_gps_time_wk))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_z_u(CANPacket *input, sensorproc_gps_z_u *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempsensorproc_gps_x_u_v = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> sensorproc_gps_x_u_v = (*((float *)(&(tempsensorproc_gps_x_u_v))));
    uint32_t tempsensorproc_gps_z_u_p = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> sensorproc_gps_z_u_p = (*((float *)(&(tempsensorproc_gps_z_u_p))));
}

void encodesensorproc_gps_z_u(sensorproc_gps_z_u *input, CANPacket *output){
    output -> id = 274211840;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempsensorproc_gps_x_u_v = ((input -> sensorproc_gps_x_u_v));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_x_u_v)))));
    const float tempsensorproc_gps_z_u_p = ((input -> sensorproc_gps_z_u_p));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_z_u_p))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_y_u(CANPacket *input, sensorproc_gps_y_u *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempsensorproc_gps_y_u_v = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> sensorproc_gps_y_u_v = (*((float *)(&(tempsensorproc_gps_y_u_v))));
    uint32_t tempsensorproc_gps_y_u_p = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> sensorproc_gps_y_u_p = (*((float *)(&(tempsensorproc_gps_y_u_p))));
}

void encodesensorproc_gps_y_u(sensorproc_gps_y_u *input, CANPacket *output){
    output -> id = 274146304;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempsensorproc_gps_y_u_v = ((input -> sensorproc_gps_y_u_v));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_y_u_v)))));
    const float tempsensorproc_gps_y_u_p = ((input -> sensorproc_gps_y_u_p));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_y_u_p))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_x_u(CANPacket *input, sensorproc_gps_x_u *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint32_t tempsensorproc_gps_x_u_v = (uint32_t) ((fullData & ((uint64_t) 0xffffffff)));
    output -> sensorproc_gps_x_u_v = (*((float *)(&(tempsensorproc_gps_x_u_v))));
    uint32_t tempsensorproc_gps_x_u_p = (uint32_t) ((fullData & ((uint64_t) 0xffffffff << 32)) >> 32);
    output -> sensorproc_gps_x_u_p = (*((float *)(&(tempsensorproc_gps_x_u_p))));
}

void encodesensorproc_gps_x_u(sensorproc_gps_x_u *input, CANPacket *output){
    output -> id = 274080768;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const float tempsensorproc_gps_x_u_v = ((input -> sensorproc_gps_x_u_v));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_x_u_v)))));
    const float tempsensorproc_gps_x_u_p = ((input -> sensorproc_gps_x_u_p));
    fullPacketData |= ((uint64_t)(*((uint32_t *)(&(tempsensorproc_gps_x_u_p))))) << 32;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_vel_z(CANPacket *input, sensorproc_gps_vel_z *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_vel_z_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_vel_z_val = (*((double *)(&(tempsensorproc_gps_vel_z_val))));
}

void encodesensorproc_gps_vel_z(sensorproc_gps_vel_z *input, CANPacket *output){
    output -> id = 274015232;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_vel_z_val = ((input -> sensorproc_gps_vel_z_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_vel_z_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_vel_y(CANPacket *input, sensorproc_gps_vel_y *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_vel_y_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_vel_y_val = (*((double *)(&(tempsensorproc_gps_vel_y_val))));
}

void encodesensorproc_gps_vel_y(sensorproc_gps_vel_y *input, CANPacket *output){
    output -> id = 273949696;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_vel_y_val = ((input -> sensorproc_gps_vel_y_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_vel_y_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_vel_x(CANPacket *input, sensorproc_gps_vel_x *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_vel_x_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_vel_x_val = (*((double *)(&(tempsensorproc_gps_vel_x_val))));
}

void encodesensorproc_gps_vel_x(sensorproc_gps_vel_x *input, CANPacket *output){
    output -> id = 273884160;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_vel_x_val = ((input -> sensorproc_gps_vel_x_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_vel_x_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_pos_z(CANPacket *input, sensorproc_gps_pos_z *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_pos_z_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_pos_z_val = (*((double *)(&(tempsensorproc_gps_pos_z_val))));
}

void encodesensorproc_gps_pos_z(sensorproc_gps_pos_z *input, CANPacket *output){
    output -> id = 273818624;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_pos_z_val = ((input -> sensorproc_gps_pos_z_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_pos_z_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_pos_y(CANPacket *input, sensorproc_gps_pos_y *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_pos_y_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_pos_y_val = (*((double *)(&(tempsensorproc_gps_pos_y_val))));
}

void encodesensorproc_gps_pos_y(sensorproc_gps_pos_y *input, CANPacket *output){
    output -> id = 273753088;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_pos_y_val = ((input -> sensorproc_gps_pos_y_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_pos_y_val)))));
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_gps_pos_x(CANPacket *input, sensorproc_gps_pos_x *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    uint64_t tempsensorproc_gps_pos_x_val = (uint64_t) ((fullData & ((uint64_t) 0xffffffffffffffff)));
output -> sensorproc_gps_pos_x_val = (*((double *)(&(tempsensorproc_gps_pos_x_val))));
}

void encodesensorproc_gps_pos_x(sensorproc_gps_pos_x *input, CANPacket *output){
    output -> id = 273687552;
    output -> length = 8;
    uint64_t fullPacketData = 0x0000000000000000;
    const double tempsensorproc_gps_pos_x_val = ((input -> sensorproc_gps_pos_x_val));
    fullPacketData |= ((uint64_t)(*((uint64_t *)(&(tempsensorproc_gps_pos_x_val)))));
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
    output -> id = 272696320;
    output -> length = 5;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_status))) & 0xff) << 24;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_reading_beta))) & 0xffff) << 32;
    fullPacketData |= (((uint64_t)((input -> sensorproc_sun_reading_alpha))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

void decodesensorproc_photodiode(CANPacket *input, sensorproc_photodiode *output){
    uint64_t *thePointer = (uint64_t *) input -> data;
    reverseArray(input -> data, 0, 7);
    const uint64_t fullData = *thePointer;
    output -> sensorproc_photodiode_x_neg = (uint8_t) (((fullData & ((uint64_t) 0xff << 40)) >> 40));
    output -> sensorproc_photodiode_y_neg = (uint8_t) (((fullData & ((uint64_t) 0xff << 48)) >> 48));
    output -> sensorproc_photodiode_x_pos = (uint8_t) (((fullData & ((uint64_t) 0xff << 56)) >> 56));
}

void encodesensorproc_photodiode(sensorproc_photodiode *input, CANPacket *output){
    output -> id = 272630784;
    output -> length = 3;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> sensorproc_photodiode_x_neg))) & 0xff) << 40;
    fullPacketData |= (((uint64_t)((input -> sensorproc_photodiode_y_neg))) & 0xff) << 48;
    fullPacketData |= (((uint64_t)((input -> sensorproc_photodiode_x_pos))) & 0xff) << 56;
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
    output -> id = 270602240;
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
    output -> id = 270558240;
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
    output -> id = 1245183;
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
    output -> id = 1179647;
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
    output -> id = 269705216;
    output -> length = 2;
    uint64_t fullPacketData = 0x0000000000000000;
    fullPacketData |= (((uint64_t)((input -> msp_temp_temp))) & 0xffff) << 48;
    uint64_t *thePointer = (uint64_t *) (&(output -> data));
    *thePointer = fullPacketData;
    reverseArray((output->data), 0, 7);
}

