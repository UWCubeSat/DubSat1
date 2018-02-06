/*
 * canwrap.h
 *
 *  Created on: Aug 14, 2017
 *      Author: emoryeng
 */

#include <inttypes.h>

#ifndef DSBASE_INTERFACES_CANWRAP_H_
#define DSBASE_INTERFACES_CANWRAP_H_

#define PARAM_ADCS_STATUS_VELOCITY_RPM 0x4201;


// BEGIN GENERATOR MACROS

#define CAN_ID_SENSORPROC_GPS_TIME 274277376
#define CAN_ID_SENSORPROC_GPS_Z_U 274211840
#define CAN_ID_SENSORPROC_GPS_Y_U 274146304
#define CAN_ID_SENSORPROC_GPS_X_U 274080768
#define CAN_ID_SENSORPROC_GPS_VEL_Z 274015232
#define CAN_ID_SENSORPROC_GPS_VEL_Y 273949696
#define CAN_ID_SENSORPROC_GPS_VEL_X 273884160
#define CAN_ID_SENSORPROC_GPS_POS_Z 273818624
#define CAN_ID_SENSORPROC_GPS_POS_Y 273753088
#define CAN_ID_SENSORPROC_GPS_POS_X 273687552
#define CAN_ID_SENSORPROC_SUN 272696320
#define CAN_ID_SENSORPROC_PHOTODIODE 272630784
#define CAN_ID_BDOT_COMMAND_DIPOLE 270602240
#define CAN_ID_BDOT_TUMBLE_STATUS 270558240
#define CAN_ID_SYNC_2 1245183
#define CAN_ID_SYNC_1 1179647
#define CAN_ID_MSP_TEMP 269705216

#define CAN_ENUM_SENSORPROC_SUN_STATUS_OVER_ILLUMINATION 18
#define CAN_ENUM_SENSORPROC_SUN_STATUS_UNDER_ILLUMINATION 17
#define CAN_ENUM_SENSORPROC_SUN_STATUS_LIGHT_FROM_OUTSIDE_FOV_5 5
#define CAN_ENUM_SENSORPROC_SUN_STATUS_LIGHT_FROM_OUTSIDE_FOV_4 4
#define CAN_ENUM_SENSORPROC_SUN_STATUS_NO_LIGHT 3
#define CAN_ENUM_SENSORPROC_SUN_STATUS_GOOD_DATA 0
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_TRUE 1
#define CAN_ENUM_BDOT_TUMBLE_STATUS_STATUS_FALSE 0

#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_OVER_ILLUMINATION 18
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_UNDER_ILLUMINATION 17
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_LIGHT_FROM_OUTSIDE_FOV_5 5
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_LIGHT_FROM_OUTSIDE_FOV_4 4
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_NO_LIGHT 3
#define CAN_ENUM_SUNPROC_SUN_STATUS_CODES_GOOD_DATA 0
#define CAN_ENUM_BOOL_TRUE 1
#define CAN_ENUM_BOOL_FALSE 0

typedef struct CANPacket {
   uint32_t id; // Actual physical ID of the packet
   uint8_t data[8]; // Data
   uint8_t bufferNum; // Only applicable for Rx, which buffer it landed in
   uint8_t length; // Only applies to sending packets. We don't know how long incoming packets are.
} CANPacket;

void canWrapInit();

// Global function pointer to point to the function
// when a packet is received through CAN
void (*CANPacketReceived)(CANPacket *);

void canSendPacket(CANPacket *packet);

void setCANPacketRxCallback(void (*ReceiveCallbackArg)(CANPacket *packet));
typedef struct sensorproc_gps_time {
    uint32_t sensorproc_gps_time_ms; // ms
    uint16_t sensorproc_gps_time_wk; // week
} sensorproc_gps_time;

typedef struct sensorproc_gps_z_u {
    float sensorproc_gps_x_u_v; //  (No Units)
    float sensorproc_gps_z_u_p; //  (No Units)
} sensorproc_gps_z_u;

typedef struct sensorproc_gps_y_u {
    float sensorproc_gps_y_u_v; //  (No Units)
    float sensorproc_gps_y_u_p; //  (No Units)
} sensorproc_gps_y_u;

typedef struct sensorproc_gps_x_u {
    float sensorproc_gps_x_u_v; //  (No Units)
    float sensorproc_gps_x_u_p; //  (No Units)
} sensorproc_gps_x_u;

typedef struct sensorproc_gps_vel_z {
    double sensorproc_gps_vel_z_val; //  (No Units)
} sensorproc_gps_vel_z;

typedef struct sensorproc_gps_vel_y {
    double sensorproc_gps_vel_y_val; //  (No Units)
} sensorproc_gps_vel_y;

typedef struct sensorproc_gps_vel_x {
    double sensorproc_gps_vel_x_val; //  (No Units)
} sensorproc_gps_vel_x;

typedef struct sensorproc_gps_pos_z {
    double sensorproc_gps_pos_z_val; //  (No Units)
} sensorproc_gps_pos_z;

typedef struct sensorproc_gps_pos_y {
    double sensorproc_gps_pos_y_val; //  (No Units)
} sensorproc_gps_pos_y;

typedef struct sensorproc_gps_pos_x {
    double sensorproc_gps_pos_x_val; //  (No Units)
} sensorproc_gps_pos_x;

typedef struct sensorproc_sun {
    uint8_t sensorproc_sun_status; //  (No Units)
    int16_t sensorproc_sun_reading_beta; // arcminutes
    int16_t sensorproc_sun_reading_alpha; // arcminutes
} sensorproc_sun;

typedef struct sensorproc_photodiode {
    uint8_t sensorproc_photodiode_x_neg; //  (No Units)
    uint8_t sensorproc_photodiode_y_neg; //  (No Units)
    uint8_t sensorproc_photodiode_x_pos; //  (No Units)
} sensorproc_photodiode;

typedef struct bdot_command_dipole {
    uint8_t bdot_command_dipole_z; // mA*m^2
    uint8_t bdot_command_dipole_y; // mA*m^2
    uint8_t bdot_command_dipole_x; // mA*m^2
} bdot_command_dipole;

typedef struct bdot_tumble_status {
    uint8_t bdot_tumble_status_status; // bool
} bdot_tumble_status;

typedef struct sync_2 {
} sync_2;

typedef struct sync_1 {
} sync_1;

typedef struct msp_temp {
    uint16_t msp_temp_temp; // dK
} msp_temp;

void encodesensorproc_gps_time(sensorproc_gps_time *input, CANPacket* output);
void decodesensorproc_gps_time(CANPacket *input, sensorproc_gps_time *output);

void encodesensorproc_gps_z_u(sensorproc_gps_z_u *input, CANPacket* output);
void decodesensorproc_gps_z_u(CANPacket *input, sensorproc_gps_z_u *output);

void encodesensorproc_gps_y_u(sensorproc_gps_y_u *input, CANPacket* output);
void decodesensorproc_gps_y_u(CANPacket *input, sensorproc_gps_y_u *output);

void encodesensorproc_gps_x_u(sensorproc_gps_x_u *input, CANPacket* output);
void decodesensorproc_gps_x_u(CANPacket *input, sensorproc_gps_x_u *output);

void encodesensorproc_gps_vel_z(sensorproc_gps_vel_z *input, CANPacket* output);
void decodesensorproc_gps_vel_z(CANPacket *input, sensorproc_gps_vel_z *output);

void encodesensorproc_gps_vel_y(sensorproc_gps_vel_y *input, CANPacket* output);
void decodesensorproc_gps_vel_y(CANPacket *input, sensorproc_gps_vel_y *output);

void encodesensorproc_gps_vel_x(sensorproc_gps_vel_x *input, CANPacket* output);
void decodesensorproc_gps_vel_x(CANPacket *input, sensorproc_gps_vel_x *output);

void encodesensorproc_gps_pos_z(sensorproc_gps_pos_z *input, CANPacket* output);
void decodesensorproc_gps_pos_z(CANPacket *input, sensorproc_gps_pos_z *output);

void encodesensorproc_gps_pos_y(sensorproc_gps_pos_y *input, CANPacket* output);
void decodesensorproc_gps_pos_y(CANPacket *input, sensorproc_gps_pos_y *output);

void encodesensorproc_gps_pos_x(sensorproc_gps_pos_x *input, CANPacket* output);
void decodesensorproc_gps_pos_x(CANPacket *input, sensorproc_gps_pos_x *output);

void encodesensorproc_sun(sensorproc_sun *input, CANPacket* output);
void decodesensorproc_sun(CANPacket *input, sensorproc_sun *output);

void encodesensorproc_photodiode(sensorproc_photodiode *input, CANPacket* output);
void decodesensorproc_photodiode(CANPacket *input, sensorproc_photodiode *output);

void encodebdot_command_dipole(bdot_command_dipole *input, CANPacket* output);
void decodebdot_command_dipole(CANPacket *input, bdot_command_dipole *output);

void encodebdot_tumble_status(bdot_tumble_status *input, CANPacket* output);
void decodebdot_tumble_status(CANPacket *input, bdot_tumble_status *output);

void encodesync_2(sync_2 *input, CANPacket* output);
void decodesync_2(CANPacket *input, sync_2 *output);

void encodesync_1(sync_1 *input, CANPacket* output);
void decodesync_1(CANPacket *input, sync_1 *output);

void encodemsp_temp(msp_temp *input, CANPacket* output);
void decodemsp_temp(CANPacket *input, msp_temp *output);

#endif /* DSBASE_INTERFACES_CANWRAP_H_ */
