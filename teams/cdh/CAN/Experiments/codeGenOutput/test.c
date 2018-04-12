
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
void canBlast() { 
	__delay_cycles(10000);	CANPacket gen_panel_temp_packet = {0};
	gen_panel_temp gen_panel_temp_info = {0};
	encodegen_panel_temp(&gen_panel_temp_info, &gen_panel_temp_packet);
	canSendPacket(&gen_panel_temp_packet);

	__delay_cycles(10000);	CANPacket cmd_gen_rst_packet = {0};
	cmd_gen_rst cmd_gen_rst_info = {0};
	encodecmd_gen_rst(&cmd_gen_rst_info, &cmd_gen_rst_packet);
	canSendPacket(&cmd_gen_rst_packet);

	__delay_cycles(10000);	CANPacket cmd_ppt_fire_packet = {0};
	cmd_ppt_fire cmd_ppt_fire_info = {0};
	encodecmd_ppt_fire(&cmd_ppt_fire_info, &cmd_ppt_fire_packet);
	canSendPacket(&cmd_ppt_fire_packet);

	__delay_cycles(10000);	CANPacket gen_panel_pwr_packet = {0};
	gen_panel_pwr gen_panel_pwr_info = {0};
	encodegen_panel_pwr(&gen_panel_pwr_info, &gen_panel_pwr_packet);
	canSendPacket(&gen_panel_pwr_packet);

	__delay_cycles(10000);	CANPacket gen_panel_current_packet = {0};
	gen_panel_current gen_panel_current_info = {0};
	encodegen_panel_current(&gen_panel_current_info, &gen_panel_current_packet);
	canSendPacket(&gen_panel_current_packet);

	__delay_cycles(10000);	CANPacket gen_panel_voltage_packet = {0};
	gen_panel_voltage gen_panel_voltage_info = {0};
	encodegen_panel_voltage(&gen_panel_voltage_info, &gen_panel_voltage_packet);
	canSendPacket(&gen_panel_voltage_packet);

	__delay_cycles(10000);	CANPacket mpc_vp_packet = {0};
	mpc_vp mpc_vp_info = {0};
	encodempc_vp(&mpc_vp_info, &mpc_vp_packet);
	canSendPacket(&mpc_vp_packet);

	__delay_cycles(10000);	CANPacket eps_batt_state_packet = {0};
	eps_batt_state eps_batt_state_info = {0};
	encodeeps_batt_state(&eps_batt_state_info, &eps_batt_state_packet);
	canSendPacket(&eps_batt_state_packet);

	__delay_cycles(10000);	CANPacket eps_batt_current_packet = {0};
	eps_batt_current eps_batt_current_info = {0};
	encodeeps_batt_current(&eps_batt_current_info, &eps_batt_current_packet);
	canSendPacket(&eps_batt_current_packet);

	__delay_cycles(10000);	CANPacket eps_batt_voltage_packet = {0};
	eps_batt_voltage eps_batt_voltage_info = {0};
	encodeeps_batt_voltage(&eps_batt_voltage_info, &eps_batt_voltage_packet);
	canSendPacket(&eps_batt_voltage_packet);

	__delay_cycles(10000);	CANPacket sensorproc_sun_packet = {0};
	sensorproc_sun sensorproc_sun_info = {0};
	encodesensorproc_sun(&sensorproc_sun_info, &sensorproc_sun_packet);
	canSendPacket(&sensorproc_sun_packet);

	__delay_cycles(10000);	CANPacket bdot_command_dipole_packet = {0};
	bdot_command_dipole bdot_command_dipole_info = {0};
	encodebdot_command_dipole(&bdot_command_dipole_info, &bdot_command_dipole_packet);
	canSendPacket(&bdot_command_dipole_packet);

	__delay_cycles(10000);	CANPacket bdot_tumble_status_packet = {0};
	bdot_tumble_status bdot_tumble_status_info = {0};
	encodebdot_tumble_status(&bdot_tumble_status_info, &bdot_tumble_status_packet);
	canSendPacket(&bdot_tumble_status_packet);

	__delay_cycles(10000);	CANPacket sync_2_packet = {0};
	sync_2 sync_2_info = {0};
	encodesync_2(&sync_2_info, &sync_2_packet);
	canSendPacket(&sync_2_packet);

	__delay_cycles(10000);	CANPacket sync_1_packet = {0};
	sync_1 sync_1_info = {0};
	encodesync_1(&sync_1_info, &sync_1_packet);
	canSendPacket(&sync_1_packet);

	__delay_cycles(10000);	CANPacket msp_temp_packet = {0};
	msp_temp msp_temp_info = {0};
	encodemsp_temp(&msp_temp_info, &msp_temp_packet);
	canSendPacket(&msp_temp_packet);

} 
while (1) { 
	canBlast(); 
	int i=0; 
	for(int i=0;i<100000;i++){} 
}