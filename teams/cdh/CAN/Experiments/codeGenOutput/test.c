
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
void canBlast() { 
	__delay_cycles(10000);	CANPacket rc_eps_batt_6_packet = {0};
	rc_eps_batt_6 rc_eps_batt_6_info = {0};
	encoderc_eps_batt_6(&rc_eps_batt_6_info, &rc_eps_batt_6_packet);
	canSendPacket(&rc_eps_batt_6_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_5_packet = {0};
	rc_eps_batt_5 rc_eps_batt_5_info = {0};
	encoderc_eps_batt_5(&rc_eps_batt_5_info, &rc_eps_batt_5_packet);
	canSendPacket(&rc_eps_batt_5_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_4_packet = {0};
	rc_eps_batt_4 rc_eps_batt_4_info = {0};
	encoderc_eps_batt_4(&rc_eps_batt_4_info, &rc_eps_batt_4_packet);
	canSendPacket(&rc_eps_batt_4_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_3_packet = {0};
	rc_eps_batt_3 rc_eps_batt_3_info = {0};
	encoderc_eps_batt_3(&rc_eps_batt_3_info, &rc_eps_batt_3_packet);
	canSendPacket(&rc_eps_batt_3_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_2_packet = {0};
	rc_eps_batt_2 rc_eps_batt_2_info = {0};
	encoderc_eps_batt_2(&rc_eps_batt_2_info, &rc_eps_batt_2_packet);
	canSendPacket(&rc_eps_batt_2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_1_packet = {0};
	rc_eps_batt_1 rc_eps_batt_1_info = {0};
	encoderc_eps_batt_1(&rc_eps_batt_1_info, &rc_eps_batt_1_packet);
	canSendPacket(&rc_eps_batt_1_packet);

	__delay_cycles(10000);	CANPacket cmd_ppt_set_count_packet = {0};
	cmd_ppt_set_count cmd_ppt_set_count_info = {0};
	encodecmd_ppt_set_count(&cmd_ppt_set_count_info, &cmd_ppt_set_count_packet);
	canSendPacket(&cmd_ppt_set_count_packet);

	__delay_cycles(10000);	CANPacket cmd_ppt_time_upd_packet = {0};
	cmd_ppt_time_upd cmd_ppt_time_upd_info = {0};
	encodecmd_ppt_time_upd(&cmd_ppt_time_upd_info, &cmd_ppt_time_upd_packet);
	canSendPacket(&cmd_ppt_time_upd_packet);

	__delay_cycles(10000);	CANPacket cmd_ppt_halt_packet = {0};
	cmd_ppt_halt cmd_ppt_halt_info = {0};
	encodecmd_ppt_halt(&cmd_ppt_halt_info, &cmd_ppt_halt_packet);
	canSendPacket(&cmd_ppt_halt_packet);

	__delay_cycles(10000);	CANPacket ppt_firing_result_packet = {0};
	ppt_firing_result ppt_firing_result_info = {0};
	encodeppt_firing_result(&ppt_firing_result_info, &ppt_firing_result_packet);
	canSendPacket(&ppt_firing_result_packet);

	__delay_cycles(10000);	CANPacket tle_6_packet = {0};
	tle_6 tle_6_info = {0};
	encodetle_6(&tle_6_info, &tle_6_packet);
	canSendPacket(&tle_6_packet);

	__delay_cycles(10000);	CANPacket tle_5_packet = {0};
	tle_5 tle_5_info = {0};
	encodetle_5(&tle_5_info, &tle_5_packet);
	canSendPacket(&tle_5_packet);

	__delay_cycles(10000);	CANPacket tle_4_packet = {0};
	tle_4 tle_4_info = {0};
	encodetle_4(&tle_4_info, &tle_4_packet);
	canSendPacket(&tle_4_packet);

	__delay_cycles(10000);	CANPacket tle_3_packet = {0};
	tle_3 tle_3_info = {0};
	encodetle_3(&tle_3_info, &tle_3_packet);
	canSendPacket(&tle_3_packet);

	__delay_cycles(10000);	CANPacket tle_2_packet = {0};
	tle_2 tle_2_info = {0};
	encodetle_2(&tle_2_info, &tle_2_packet);
	canSendPacket(&tle_2_packet);

	__delay_cycles(10000);	CANPacket tle_1_packet = {0};
	tle_1 tle_1_info = {0};
	encodetle_1(&tle_1_info, &tle_1_packet);
	canSendPacket(&tle_1_packet);

	__delay_cycles(10000);	CANPacket eps_pd_undervoltage_packet = {0};
	eps_pd_undervoltage eps_pd_undervoltage_info = {0};
	encodeeps_pd_undervoltage(&eps_pd_undervoltage_info, &eps_pd_undervoltage_packet);
	canSendPacket(&eps_pd_undervoltage_packet);

	__delay_cycles(10000);	CANPacket eps_pd_batt_voltage_packet = {0};
	eps_pd_batt_voltage eps_pd_batt_voltage_info = {0};
	encodeeps_pd_batt_voltage(&eps_pd_batt_voltage_info, &eps_pd_batt_voltage_packet);
	canSendPacket(&eps_pd_batt_voltage_packet);

	__delay_cycles(10000);	CANPacket cmd_pd_rst_packet = {0};
	cmd_pd_rst cmd_pd_rst_info = {0};
	encodecmd_pd_rst(&cmd_pd_rst_info, &cmd_pd_rst_packet);
	canSendPacket(&cmd_pd_rst_packet);

	__delay_cycles(10000);	CANPacket cmd_com2_run_packet = {0};
	cmd_com2_run cmd_com2_run_info = {0};
	encodecmd_com2_run(&cmd_com2_run_info, &cmd_com2_run_packet);
	canSendPacket(&cmd_com2_run_packet);

	__delay_cycles(10000);	CANPacket com2_state_packet = {0};
	com2_state com2_state_info = {0};
	encodecom2_state(&com2_state_info, &com2_state_packet);
	canSendPacket(&com2_state_packet);

	__delay_cycles(10000);	CANPacket cmd_pd_enable_packet = {0};
	cmd_pd_enable cmd_pd_enable_info = {0};
	encodecmd_pd_enable(&cmd_pd_enable_info, &cmd_pd_enable_packet);
	canSendPacket(&cmd_pd_enable_packet);

	__delay_cycles(10000);	CANPacket cmd_batt_rst_packet = {0};
	cmd_batt_rst cmd_batt_rst_info = {0};
	encodecmd_batt_rst(&cmd_batt_rst_info, &cmd_batt_rst_packet);
	canSendPacket(&cmd_batt_rst_packet);

	__delay_cycles(10000);	CANPacket cmd_batt_bal_enable_packet = {0};
	cmd_batt_bal_enable cmd_batt_bal_enable_info = {0};
	encodecmd_batt_bal_enable(&cmd_batt_bal_enable_info, &cmd_batt_bal_enable_packet);
	canSendPacket(&cmd_batt_bal_enable_packet);

	__delay_cycles(10000);	CANPacket cmd_reboot_request_packet = {0};
	cmd_reboot_request cmd_reboot_request_info = {0};
	encodecmd_reboot_request(&cmd_reboot_request_info, &cmd_reboot_request_packet);
	canSendPacket(&cmd_reboot_request_packet);

	__delay_cycles(10000);	CANPacket cmd_ignore_fsw_packet = {0};
	cmd_ignore_fsw cmd_ignore_fsw_info = {0};
	encodecmd_ignore_fsw(&cmd_ignore_fsw_info, &cmd_ignore_fsw_packet);
	canSendPacket(&cmd_ignore_fsw_packet);

	__delay_cycles(10000);	CANPacket cmd_mtq_fsw_packet = {0};
	cmd_mtq_fsw cmd_mtq_fsw_info = {0};
	encodecmd_mtq_fsw(&cmd_mtq_fsw_info, &cmd_mtq_fsw_packet);
	canSendPacket(&cmd_mtq_fsw_packet);

	__delay_cycles(10000);	CANPacket eps_domain_output_packet = {0};
	eps_domain_output eps_domain_output_info = {0};
	encodeeps_domain_output(&eps_domain_output_info, &eps_domain_output_packet);
	canSendPacket(&eps_domain_output_packet);

	__delay_cycles(10000);	CANPacket sensorproc_imu_packet = {0};
	sensorproc_imu sensorproc_imu_info = {0};
	encodesensorproc_imu(&sensorproc_imu_info, &sensorproc_imu_packet);
	canSendPacket(&sensorproc_imu_packet);

	__delay_cycles(10000);	CANPacket sensorproc_mag_packet = {0};
	sensorproc_mag sensorproc_mag_info = {0};
	encodesensorproc_mag(&sensorproc_mag_info, &sensorproc_mag_packet);
	canSendPacket(&sensorproc_mag_packet);

	__delay_cycles(10000);	CANPacket cmd_rollcall_packet = {0};
	cmd_rollcall cmd_rollcall_info = {0};
	encodecmd_rollcall(&cmd_rollcall_info, &cmd_rollcall_packet);
	canSendPacket(&cmd_rollcall_packet);

	__delay_cycles(10000);	CANPacket mtq_ack_packet = {0};
	mtq_ack mtq_ack_info = {0};
	encodemtq_ack(&mtq_ack_info, &mtq_ack_packet);
	canSendPacket(&mtq_ack_packet);

	__delay_cycles(10000);	CANPacket gen_panel_temp_packet = {0};
	gen_panel_temp gen_panel_temp_info = {0};
	encodegen_panel_temp(&gen_panel_temp_info, &gen_panel_temp_packet);
	canSendPacket(&gen_panel_temp_packet);

	__delay_cycles(10000);	CANPacket cmd_gen_rst_packet = {0};
	cmd_gen_rst cmd_gen_rst_info = {0};
	encodecmd_gen_rst(&cmd_gen_rst_info, &cmd_gen_rst_packet);
	canSendPacket(&cmd_gen_rst_packet);

	__delay_cycles(10000);	CANPacket cmd_ppt_single_fire_packet = {0};
	cmd_ppt_single_fire cmd_ppt_single_fire_info = {0};
	encodecmd_ppt_single_fire(&cmd_ppt_single_fire_info, &cmd_ppt_single_fire_packet);
	canSendPacket(&cmd_ppt_single_fire_packet);

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

	__delay_cycles(10000);	CANPacket cmd_mtq_bdot_packet = {0};
	cmd_mtq_bdot cmd_mtq_bdot_info = {0};
	encodecmd_mtq_bdot(&cmd_mtq_bdot_info, &cmd_mtq_bdot_packet);
	canSendPacket(&cmd_mtq_bdot_packet);

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