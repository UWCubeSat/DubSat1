
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
void canBlast() { 
	__delay_cycles(10000);	CANPacket gcmd_dist_autoshutoff_packet = {0};
	gcmd_dist_autoshutoff gcmd_dist_autoshutoff_info = {0};
	encodegcmd_dist_autoshutoff(&gcmd_dist_autoshutoff_info, &gcmd_dist_autoshutoff_packet);
	canSendPacket(&gcmd_dist_autoshutoff_packet);

	__delay_cycles(10000);	CANPacket gcmd_ppt_halt_packet = {0};
	gcmd_ppt_halt gcmd_ppt_halt_info = {0};
	encodegcmd_ppt_halt(&gcmd_ppt_halt_info, &gcmd_ppt_halt_packet);
	canSendPacket(&gcmd_ppt_halt_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_reset_mission_packet = {0};
	gcmd_dist_reset_mission gcmd_dist_reset_mission_info = {0};
	encodegcmd_dist_reset_mission(&gcmd_dist_reset_mission_info, &gcmd_dist_reset_mission_packet);
	canSendPacket(&gcmd_dist_reset_mission_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_self_restart_packet = {0};
	gcmd_dist_self_restart gcmd_dist_self_restart_info = {0};
	encodegcmd_dist_self_restart(&gcmd_dist_self_restart_info, &gcmd_dist_self_restart_packet);
	canSendPacket(&gcmd_dist_self_restart_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_18_packet = {0};
	rc_eps_dist_18 rc_eps_dist_18_info = {0};
	encoderc_eps_dist_18(&rc_eps_dist_18_info, &rc_eps_dist_18_packet);
	canSendPacket(&rc_eps_dist_18_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_10_packet = {0};
	rc_adcs_bdot_10 rc_adcs_bdot_10_info = {0};
	encoderc_adcs_bdot_10(&rc_adcs_bdot_10_info, &rc_adcs_bdot_10_packet);
	canSendPacket(&rc_adcs_bdot_10_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_9_packet = {0};
	rc_adcs_bdot_9 rc_adcs_bdot_9_info = {0};
	encoderc_adcs_bdot_9(&rc_adcs_bdot_9_info, &rc_adcs_bdot_9_packet);
	canSendPacket(&rc_adcs_bdot_9_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_8_packet = {0};
	rc_adcs_bdot_8 rc_adcs_bdot_8_info = {0};
	encoderc_adcs_bdot_8(&rc_adcs_bdot_8_info, &rc_adcs_bdot_8_packet);
	canSendPacket(&rc_adcs_bdot_8_packet);

	__delay_cycles(10000);	CANPacket gcmd_batt_set_heater_check_packet = {0};
	gcmd_batt_set_heater_check gcmd_batt_set_heater_check_info = {0};
	encodegcmd_batt_set_heater_check(&gcmd_batt_set_heater_check_info, &gcmd_batt_set_heater_check_packet);
	canSendPacket(&gcmd_batt_set_heater_check_packet);

	__delay_cycles(10000);	CANPacket eps_dist_autoseq_get_met_rsp_packet = {0};
	eps_dist_autoseq_get_met_rsp eps_dist_autoseq_get_met_rsp_info = {0};
	encodeeps_dist_autoseq_get_met_rsp(&eps_dist_autoseq_get_met_rsp_info, &eps_dist_autoseq_get_met_rsp_packet);
	canSendPacket(&eps_dist_autoseq_get_met_rsp_packet);

	__delay_cycles(10000);	CANPacket eps_dist_autoseq_get_ind_rsp_packet = {0};
	eps_dist_autoseq_get_ind_rsp eps_dist_autoseq_get_ind_rsp_info = {0};
	encodeeps_dist_autoseq_get_ind_rsp(&eps_dist_autoseq_get_ind_rsp_info, &eps_dist_autoseq_get_ind_rsp_packet);
	canSendPacket(&eps_dist_autoseq_get_ind_rsp_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_get_met_packet = {0};
	gcmd_autoseq_get_met gcmd_autoseq_get_met_info = {0};
	encodegcmd_autoseq_get_met(&gcmd_autoseq_get_met_info, &gcmd_autoseq_get_met_packet);
	canSendPacket(&gcmd_autoseq_get_met_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_get_indices_packet = {0};
	gcmd_autoseq_get_indices gcmd_autoseq_get_indices_info = {0};
	encodegcmd_autoseq_get_indices(&gcmd_autoseq_get_indices_info, &gcmd_autoseq_get_indices_packet);
	canSendPacket(&gcmd_autoseq_get_indices_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_remove_can_id_packet = {0};
	gcmd_autoseq_remove_can_id gcmd_autoseq_remove_can_id_info = {0};
	encodegcmd_autoseq_remove_can_id(&gcmd_autoseq_remove_can_id_info, &gcmd_autoseq_remove_can_id_packet);
	canSendPacket(&gcmd_autoseq_remove_can_id_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_rm_at_index_packet = {0};
	gcmd_autoseq_rm_at_index gcmd_autoseq_rm_at_index_info = {0};
	encodegcmd_autoseq_rm_at_index(&gcmd_autoseq_rm_at_index_info, &gcmd_autoseq_rm_at_index_packet);
	canSendPacket(&gcmd_autoseq_rm_at_index_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_add_2_packet = {0};
	gcmd_autoseq_add_2 gcmd_autoseq_add_2_info = {0};
	encodegcmd_autoseq_add_2(&gcmd_autoseq_add_2_info, &gcmd_autoseq_add_2_packet);
	canSendPacket(&gcmd_autoseq_add_2_packet);

	__delay_cycles(10000);	CANPacket gcmd_autoseq_add_1_packet = {0};
	gcmd_autoseq_add_1 gcmd_autoseq_add_1_info = {0};
	encodegcmd_autoseq_add_1(&gcmd_autoseq_add_1_info, &gcmd_autoseq_add_1_packet);
	canSendPacket(&gcmd_autoseq_add_1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_7_packet = {0};
	rc_adcs_bdot_7 rc_adcs_bdot_7_info = {0};
	encoderc_adcs_bdot_7(&rc_adcs_bdot_7_info, &rc_adcs_bdot_7_packet);
	canSendPacket(&rc_adcs_bdot_7_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_6_packet = {0};
	rc_adcs_bdot_6 rc_adcs_bdot_6_info = {0};
	encoderc_adcs_bdot_6(&rc_adcs_bdot_6_info, &rc_adcs_bdot_6_packet);
	canSendPacket(&rc_adcs_bdot_6_packet);

	__delay_cycles(10000);	CANPacket gcmd_ppt_multiple_fire_packet = {0};
	gcmd_ppt_multiple_fire gcmd_ppt_multiple_fire_info = {0};
	encodegcmd_ppt_multiple_fire(&gcmd_ppt_multiple_fire_info, &gcmd_ppt_multiple_fire_packet);
	canSendPacket(&gcmd_ppt_multiple_fire_packet);

	__delay_cycles(10000);	CANPacket gcmd_mtq_pwm_time_packet = {0};
	gcmd_mtq_pwm_time gcmd_mtq_pwm_time_info = {0};
	encodegcmd_mtq_pwm_time(&gcmd_mtq_pwm_time_info, &gcmd_mtq_pwm_time_packet);
	canSendPacket(&gcmd_mtq_pwm_time_packet);

	__delay_cycles(10000);	CANPacket gcmd_eps_batt_fulldef_packet = {0};
	gcmd_eps_batt_fulldef gcmd_eps_batt_fulldef_info = {0};
	encodegcmd_eps_batt_fulldef(&gcmd_eps_batt_fulldef_info, &gcmd_eps_batt_fulldef_packet);
	canSendPacket(&gcmd_eps_batt_fulldef_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_5_packet = {0};
	rc_adcs_bdot_5 rc_adcs_bdot_5_info = {0};
	encoderc_adcs_bdot_5(&rc_adcs_bdot_5_info, &rc_adcs_bdot_5_packet);
	canSendPacket(&rc_adcs_bdot_5_packet);

	__delay_cycles(10000);	CANPacket gcmd_reset_minmax_packet = {0};
	gcmd_reset_minmax gcmd_reset_minmax_info = {0};
	encodegcmd_reset_minmax(&gcmd_reset_minmax_info, &gcmd_reset_minmax_packet);
	canSendPacket(&gcmd_reset_minmax_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_h2_packet = {0};
	rc_eps_batt_h2 rc_eps_batt_h2_info = {0};
	encoderc_eps_batt_h2(&rc_eps_batt_h2_info, &rc_eps_batt_h2_packet);
	canSendPacket(&rc_eps_batt_h2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_h2_packet = {0};
	rc_adcs_bdot_h2 rc_adcs_bdot_h2_info = {0};
	encoderc_adcs_bdot_h2(&rc_adcs_bdot_h2_info, &rc_adcs_bdot_h2_packet);
	canSendPacket(&rc_adcs_bdot_h2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_h2_packet = {0};
	rc_eps_dist_h2 rc_eps_dist_h2_info = {0};
	encoderc_eps_dist_h2(&rc_eps_dist_h2_info, &rc_eps_dist_h2_packet);
	canSendPacket(&rc_eps_dist_h2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_h2_packet = {0};
	rc_adcs_mpc_h2 rc_adcs_mpc_h2_info = {0};
	encoderc_adcs_mpc_h2(&rc_adcs_mpc_h2_info, &rc_adcs_mpc_h2_packet);
	canSendPacket(&rc_adcs_mpc_h2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_h2_packet = {0};
	rc_adcs_estim_h2 rc_adcs_estim_h2_info = {0};
	encoderc_adcs_estim_h2(&rc_adcs_estim_h2_info, &rc_adcs_estim_h2_packet);
	canSendPacket(&rc_adcs_estim_h2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_h2_packet = {0};
	rc_adcs_sp_h2 rc_adcs_sp_h2_info = {0};
	encoderc_adcs_sp_h2(&rc_adcs_sp_h2_info, &rc_adcs_sp_h2_packet);
	canSendPacket(&rc_adcs_sp_h2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_h2_packet = {0};
	rc_eps_gen_h2 rc_eps_gen_h2_info = {0};
	encoderc_eps_gen_h2(&rc_eps_gen_h2_info, &rc_eps_gen_h2_packet);
	canSendPacket(&rc_eps_gen_h2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_h2_packet = {0};
	rc_adcs_mtq_h2 rc_adcs_mtq_h2_info = {0};
	encoderc_adcs_mtq_h2(&rc_adcs_mtq_h2_info, &rc_adcs_mtq_h2_packet);
	canSendPacket(&rc_adcs_mtq_h2_packet);

	__delay_cycles(10000);	CANPacket rc_ppt_h2_packet = {0};
	rc_ppt_h2 rc_ppt_h2_info = {0};
	encoderc_ppt_h2(&rc_ppt_h2_info, &rc_ppt_h2_packet);
	canSendPacket(&rc_ppt_h2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_h1_packet = {0};
	rc_eps_batt_h1 rc_eps_batt_h1_info = {0};
	encoderc_eps_batt_h1(&rc_eps_batt_h1_info, &rc_eps_batt_h1_packet);
	canSendPacket(&rc_eps_batt_h1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_h1_packet = {0};
	rc_adcs_bdot_h1 rc_adcs_bdot_h1_info = {0};
	encoderc_adcs_bdot_h1(&rc_adcs_bdot_h1_info, &rc_adcs_bdot_h1_packet);
	canSendPacket(&rc_adcs_bdot_h1_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_h1_packet = {0};
	rc_eps_dist_h1 rc_eps_dist_h1_info = {0};
	encoderc_eps_dist_h1(&rc_eps_dist_h1_info, &rc_eps_dist_h1_packet);
	canSendPacket(&rc_eps_dist_h1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_h1_packet = {0};
	rc_adcs_mpc_h1 rc_adcs_mpc_h1_info = {0};
	encoderc_adcs_mpc_h1(&rc_adcs_mpc_h1_info, &rc_adcs_mpc_h1_packet);
	canSendPacket(&rc_adcs_mpc_h1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_h1_packet = {0};
	rc_adcs_estim_h1 rc_adcs_estim_h1_info = {0};
	encoderc_adcs_estim_h1(&rc_adcs_estim_h1_info, &rc_adcs_estim_h1_packet);
	canSendPacket(&rc_adcs_estim_h1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_h1_packet = {0};
	rc_adcs_sp_h1 rc_adcs_sp_h1_info = {0};
	encoderc_adcs_sp_h1(&rc_adcs_sp_h1_info, &rc_adcs_sp_h1_packet);
	canSendPacket(&rc_adcs_sp_h1_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_h1_packet = {0};
	rc_eps_gen_h1 rc_eps_gen_h1_info = {0};
	encoderc_eps_gen_h1(&rc_eps_gen_h1_info, &rc_eps_gen_h1_packet);
	canSendPacket(&rc_eps_gen_h1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_h1_packet = {0};
	rc_adcs_mtq_h1 rc_adcs_mtq_h1_info = {0};
	encoderc_adcs_mtq_h1(&rc_adcs_mtq_h1_info, &rc_adcs_mtq_h1_packet);
	canSendPacket(&rc_adcs_mtq_h1_packet);

	__delay_cycles(10000);	CANPacket rc_ppt_h1_packet = {0};
	rc_ppt_h1 rc_ppt_h1_info = {0};
	encoderc_ppt_h1(&rc_ppt_h1_info, &rc_ppt_h1_packet);
	canSendPacket(&rc_ppt_h1_packet);

	__delay_cycles(10000);	CANPacket gcmd_mtq_pms_packet = {0};
	gcmd_mtq_pms gcmd_mtq_pms_info = {0};
	encodegcmd_mtq_pms(&gcmd_mtq_pms_info, &gcmd_mtq_pms_packet);
	canSendPacket(&gcmd_mtq_pms_packet);

	__delay_cycles(10000);	CANPacket gcmd_bdot_max_tumble_packet = {0};
	gcmd_bdot_max_tumble gcmd_bdot_max_tumble_info = {0};
	encodegcmd_bdot_max_tumble(&gcmd_bdot_max_tumble_info, &gcmd_bdot_max_tumble_packet);
	canSendPacket(&gcmd_bdot_max_tumble_packet);

	__delay_cycles(10000);	CANPacket gcmd_bdot_spam_packet = {0};
	gcmd_bdot_spam gcmd_bdot_spam_info = {0};
	encodegcmd_bdot_spam(&gcmd_bdot_spam_info, &gcmd_bdot_spam_packet);
	canSendPacket(&gcmd_bdot_spam_packet);

	__delay_cycles(10000);	CANPacket gcmd_bdot_control_packet = {0};
	gcmd_bdot_control gcmd_bdot_control_info = {0};
	encodegcmd_bdot_control(&gcmd_bdot_control_info, &gcmd_bdot_control_packet);
	canSendPacket(&gcmd_bdot_control_packet);

	__delay_cycles(10000);	CANPacket gcmd_bdot_mag_control_packet = {0};
	gcmd_bdot_mag_control gcmd_bdot_mag_control_info = {0};
	encodegcmd_bdot_mag_control(&gcmd_bdot_mag_control_info, &gcmd_bdot_mag_control_packet);
	canSendPacket(&gcmd_bdot_mag_control_packet);

	__delay_cycles(10000);	CANPacket gcmd_bdot_pole_override_packet = {0};
	gcmd_bdot_pole_override gcmd_bdot_pole_override_info = {0};
	encodegcmd_bdot_pole_override(&gcmd_bdot_pole_override_info, &gcmd_bdot_pole_override_packet);
	canSendPacket(&gcmd_bdot_pole_override_packet);

	__delay_cycles(10000);	CANPacket gcmd_gen_set_pt_state_packet = {0};
	gcmd_gen_set_pt_state gcmd_gen_set_pt_state_info = {0};
	encodegcmd_gen_set_pt_state(&gcmd_gen_set_pt_state_info, &gcmd_gen_set_pt_state_packet);
	canSendPacket(&gcmd_gen_set_pt_state_packet);

	__delay_cycles(10000);	CANPacket gcmd_sp_set_thresh_packet = {0};
	gcmd_sp_set_thresh gcmd_sp_set_thresh_info = {0};
	encodegcmd_sp_set_thresh(&gcmd_sp_set_thresh_info, &gcmd_sp_set_thresh_packet);
	canSendPacket(&gcmd_sp_set_thresh_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_ppt_packet = {0};
	gcmd_dist_set_pd_ovc_ppt gcmd_dist_set_pd_ovc_ppt_info = {0};
	encodegcmd_dist_set_pd_ovc_ppt(&gcmd_dist_set_pd_ovc_ppt_info, &gcmd_dist_set_pd_ovc_ppt_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_ppt_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_eps_packet = {0};
	gcmd_dist_set_pd_ovc_eps gcmd_dist_set_pd_ovc_eps_info = {0};
	encodegcmd_dist_set_pd_ovc_eps(&gcmd_dist_set_pd_ovc_eps_info, &gcmd_dist_set_pd_ovc_eps_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_eps_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_wheels_packet = {0};
	gcmd_dist_set_pd_ovc_wheels gcmd_dist_set_pd_ovc_wheels_info = {0};
	encodegcmd_dist_set_pd_ovc_wheels(&gcmd_dist_set_pd_ovc_wheels_info, &gcmd_dist_set_pd_ovc_wheels_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_wheels_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_estim_packet = {0};
	gcmd_dist_set_pd_ovc_estim gcmd_dist_set_pd_ovc_estim_info = {0};
	encodegcmd_dist_set_pd_ovc_estim(&gcmd_dist_set_pd_ovc_estim_info, &gcmd_dist_set_pd_ovc_estim_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_estim_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_bdot_packet = {0};
	gcmd_dist_set_pd_ovc_bdot gcmd_dist_set_pd_ovc_bdot_info = {0};
	encodegcmd_dist_set_pd_ovc_bdot(&gcmd_dist_set_pd_ovc_bdot_info, &gcmd_dist_set_pd_ovc_bdot_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_bdot_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_rahs_packet = {0};
	gcmd_dist_set_pd_ovc_rahs gcmd_dist_set_pd_ovc_rahs_info = {0};
	encodegcmd_dist_set_pd_ovc_rahs(&gcmd_dist_set_pd_ovc_rahs_info, &gcmd_dist_set_pd_ovc_rahs_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_rahs_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_com2_packet = {0};
	gcmd_dist_set_pd_ovc_com2 gcmd_dist_set_pd_ovc_com2_info = {0};
	encodegcmd_dist_set_pd_ovc_com2(&gcmd_dist_set_pd_ovc_com2_info, &gcmd_dist_set_pd_ovc_com2_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_com2_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_ovc_com1_packet = {0};
	gcmd_dist_set_pd_ovc_com1 gcmd_dist_set_pd_ovc_com1_info = {0};
	encodegcmd_dist_set_pd_ovc_com1(&gcmd_dist_set_pd_ovc_com1_info, &gcmd_dist_set_pd_ovc_com1_packet);
	canSendPacket(&gcmd_dist_set_pd_ovc_com1_packet);

	__delay_cycles(10000);	CANPacket gcmd_dist_set_pd_state_packet = {0};
	gcmd_dist_set_pd_state gcmd_dist_set_pd_state_info = {0};
	encodegcmd_dist_set_pd_state(&gcmd_dist_set_pd_state_info, &gcmd_dist_set_pd_state_packet);
	canSendPacket(&gcmd_dist_set_pd_state_packet);

	__delay_cycles(10000);	CANPacket gcmd_mtq_pop_packet = {0};
	gcmd_mtq_pop gcmd_mtq_pop_info = {0};
	encodegcmd_mtq_pop(&gcmd_mtq_pop_info, &gcmd_mtq_pop_packet);
	canSendPacket(&gcmd_mtq_pop_packet);

	__delay_cycles(10000);	CANPacket rc_eps_batt_7_packet = {0};
	rc_eps_batt_7 rc_eps_batt_7_info = {0};
	encoderc_eps_batt_7(&rc_eps_batt_7_info, &rc_eps_batt_7_packet);
	canSendPacket(&rc_eps_batt_7_packet);

	__delay_cycles(10000);	CANPacket sensorproc_mag2_packet = {0};
	sensorproc_mag2 sensorproc_mag2_info = {0};
	encodesensorproc_mag2(&sensorproc_mag2_info, &sensorproc_mag2_packet);
	canSendPacket(&sensorproc_mag2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_14_packet = {0};
	rc_adcs_estim_14 rc_adcs_estim_14_info = {0};
	encoderc_adcs_estim_14(&rc_adcs_estim_14_info, &rc_adcs_estim_14_packet);
	canSendPacket(&rc_adcs_estim_14_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_13_packet = {0};
	rc_adcs_estim_13 rc_adcs_estim_13_info = {0};
	encoderc_adcs_estim_13(&rc_adcs_estim_13_info, &rc_adcs_estim_13_packet);
	canSendPacket(&rc_adcs_estim_13_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_12_packet = {0};
	rc_adcs_estim_12 rc_adcs_estim_12_info = {0};
	encoderc_adcs_estim_12(&rc_adcs_estim_12_info, &rc_adcs_estim_12_packet);
	canSendPacket(&rc_adcs_estim_12_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_11_packet = {0};
	rc_adcs_estim_11 rc_adcs_estim_11_info = {0};
	encoderc_adcs_estim_11(&rc_adcs_estim_11_info, &rc_adcs_estim_11_packet);
	canSendPacket(&rc_adcs_estim_11_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_10_packet = {0};
	rc_adcs_estim_10 rc_adcs_estim_10_info = {0};
	encoderc_adcs_estim_10(&rc_adcs_estim_10_info, &rc_adcs_estim_10_packet);
	canSendPacket(&rc_adcs_estim_10_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_9_packet = {0};
	rc_adcs_estim_9 rc_adcs_estim_9_info = {0};
	encoderc_adcs_estim_9(&rc_adcs_estim_9_info, &rc_adcs_estim_9_packet);
	canSendPacket(&rc_adcs_estim_9_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_16_packet = {0};
	rc_eps_dist_16 rc_eps_dist_16_info = {0};
	encoderc_eps_dist_16(&rc_eps_dist_16_info, &rc_eps_dist_16_packet);
	canSendPacket(&rc_eps_dist_16_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_9_packet = {0};
	rc_eps_dist_9 rc_eps_dist_9_info = {0};
	encoderc_eps_dist_9(&rc_eps_dist_9_info, &rc_eps_dist_9_packet);
	canSendPacket(&rc_eps_dist_9_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_7_packet = {0};
	rc_eps_dist_7 rc_eps_dist_7_info = {0};
	encoderc_eps_dist_7(&rc_eps_dist_7_info, &rc_eps_dist_7_packet);
	canSendPacket(&rc_eps_dist_7_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_6_packet = {0};
	rc_eps_dist_6 rc_eps_dist_6_info = {0};
	encoderc_eps_dist_6(&rc_eps_dist_6_info, &rc_eps_dist_6_packet);
	canSendPacket(&rc_eps_dist_6_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_5_packet = {0};
	rc_eps_dist_5 rc_eps_dist_5_info = {0};
	encoderc_eps_dist_5(&rc_eps_dist_5_info, &rc_eps_dist_5_packet);
	canSendPacket(&rc_eps_dist_5_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_3_packet = {0};
	rc_eps_dist_3 rc_eps_dist_3_info = {0};
	encoderc_eps_dist_3(&rc_eps_dist_3_info, &rc_eps_dist_3_packet);
	canSendPacket(&rc_eps_dist_3_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_2_packet = {0};
	rc_eps_dist_2 rc_eps_dist_2_info = {0};
	encoderc_eps_dist_2(&rc_eps_dist_2_info, &rc_eps_dist_2_packet);
	canSendPacket(&rc_eps_dist_2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_17_packet = {0};
	rc_eps_dist_17 rc_eps_dist_17_info = {0};
	encoderc_eps_dist_17(&rc_eps_dist_17_info, &rc_eps_dist_17_packet);
	canSendPacket(&rc_eps_dist_17_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_14_packet = {0};
	rc_eps_dist_14 rc_eps_dist_14_info = {0};
	encoderc_eps_dist_14(&rc_eps_dist_14_info, &rc_eps_dist_14_packet);
	canSendPacket(&rc_eps_dist_14_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_13_packet = {0};
	rc_eps_dist_13 rc_eps_dist_13_info = {0};
	encoderc_eps_dist_13(&rc_eps_dist_13_info, &rc_eps_dist_13_packet);
	canSendPacket(&rc_eps_dist_13_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_11_packet = {0};
	rc_eps_dist_11 rc_eps_dist_11_info = {0};
	encoderc_eps_dist_11(&rc_eps_dist_11_info, &rc_eps_dist_11_packet);
	canSendPacket(&rc_eps_dist_11_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_12_packet = {0};
	rc_eps_dist_12 rc_eps_dist_12_info = {0};
	encoderc_eps_dist_12(&rc_eps_dist_12_info, &rc_eps_dist_12_packet);
	canSendPacket(&rc_eps_dist_12_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_15_packet = {0};
	rc_eps_dist_15 rc_eps_dist_15_info = {0};
	encoderc_eps_dist_15(&rc_eps_dist_15_info, &rc_eps_dist_15_packet);
	canSendPacket(&rc_eps_dist_15_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_10_packet = {0};
	rc_eps_dist_10 rc_eps_dist_10_info = {0};
	encoderc_eps_dist_10(&rc_eps_dist_10_info, &rc_eps_dist_10_packet);
	canSendPacket(&rc_eps_dist_10_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_8_packet = {0};
	rc_eps_dist_8 rc_eps_dist_8_info = {0};
	encoderc_eps_dist_8(&rc_eps_dist_8_info, &rc_eps_dist_8_packet);
	canSendPacket(&rc_eps_dist_8_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_4_packet = {0};
	rc_eps_dist_4 rc_eps_dist_4_info = {0};
	encoderc_eps_dist_4(&rc_eps_dist_4_info, &rc_eps_dist_4_packet);
	canSendPacket(&rc_eps_dist_4_packet);

	__delay_cycles(10000);	CANPacket rc_eps_dist_1_packet = {0};
	rc_eps_dist_1 rc_eps_dist_1_info = {0};
	encoderc_eps_dist_1(&rc_eps_dist_1_info, &rc_eps_dist_1_packet);
	canSendPacket(&rc_eps_dist_1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_11_packet = {0};
	rc_adcs_mpc_11 rc_adcs_mpc_11_info = {0};
	encoderc_adcs_mpc_11(&rc_adcs_mpc_11_info, &rc_adcs_mpc_11_packet);
	canSendPacket(&rc_adcs_mpc_11_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_7_packet = {0};
	rc_adcs_mpc_7 rc_adcs_mpc_7_info = {0};
	encoderc_adcs_mpc_7(&rc_adcs_mpc_7_info, &rc_adcs_mpc_7_packet);
	canSendPacket(&rc_adcs_mpc_7_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_8_packet = {0};
	rc_adcs_mpc_8 rc_adcs_mpc_8_info = {0};
	encoderc_adcs_mpc_8(&rc_adcs_mpc_8_info, &rc_adcs_mpc_8_packet);
	canSendPacket(&rc_adcs_mpc_8_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_9_packet = {0};
	rc_adcs_mpc_9 rc_adcs_mpc_9_info = {0};
	encoderc_adcs_mpc_9(&rc_adcs_mpc_9_info, &rc_adcs_mpc_9_packet);
	canSendPacket(&rc_adcs_mpc_9_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_5_packet = {0};
	rc_adcs_mpc_5 rc_adcs_mpc_5_info = {0};
	encoderc_adcs_mpc_5(&rc_adcs_mpc_5_info, &rc_adcs_mpc_5_packet);
	canSendPacket(&rc_adcs_mpc_5_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_6_packet = {0};
	rc_adcs_mpc_6 rc_adcs_mpc_6_info = {0};
	encoderc_adcs_mpc_6(&rc_adcs_mpc_6_info, &rc_adcs_mpc_6_packet);
	canSendPacket(&rc_adcs_mpc_6_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_4_packet = {0};
	rc_adcs_mpc_4 rc_adcs_mpc_4_info = {0};
	encoderc_adcs_mpc_4(&rc_adcs_mpc_4_info, &rc_adcs_mpc_4_packet);
	canSendPacket(&rc_adcs_mpc_4_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_3_packet = {0};
	rc_adcs_mpc_3 rc_adcs_mpc_3_info = {0};
	encoderc_adcs_mpc_3(&rc_adcs_mpc_3_info, &rc_adcs_mpc_3_packet);
	canSendPacket(&rc_adcs_mpc_3_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_15_packet = {0};
	rc_adcs_mpc_15 rc_adcs_mpc_15_info = {0};
	encoderc_adcs_mpc_15(&rc_adcs_mpc_15_info, &rc_adcs_mpc_15_packet);
	canSendPacket(&rc_adcs_mpc_15_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_12_packet = {0};
	rc_adcs_mpc_12 rc_adcs_mpc_12_info = {0};
	encoderc_adcs_mpc_12(&rc_adcs_mpc_12_info, &rc_adcs_mpc_12_packet);
	canSendPacket(&rc_adcs_mpc_12_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_13_packet = {0};
	rc_adcs_mpc_13 rc_adcs_mpc_13_info = {0};
	encoderc_adcs_mpc_13(&rc_adcs_mpc_13_info, &rc_adcs_mpc_13_packet);
	canSendPacket(&rc_adcs_mpc_13_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_14_packet = {0};
	rc_adcs_mpc_14 rc_adcs_mpc_14_info = {0};
	encoderc_adcs_mpc_14(&rc_adcs_mpc_14_info, &rc_adcs_mpc_14_packet);
	canSendPacket(&rc_adcs_mpc_14_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_10_packet = {0};
	rc_adcs_mpc_10 rc_adcs_mpc_10_info = {0};
	encoderc_adcs_mpc_10(&rc_adcs_mpc_10_info, &rc_adcs_mpc_10_packet);
	canSendPacket(&rc_adcs_mpc_10_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_2_packet = {0};
	rc_adcs_mpc_2 rc_adcs_mpc_2_info = {0};
	encoderc_adcs_mpc_2(&rc_adcs_mpc_2_info, &rc_adcs_mpc_2_packet);
	canSendPacket(&rc_adcs_mpc_2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mpc_1_packet = {0};
	rc_adcs_mpc_1 rc_adcs_mpc_1_info = {0};
	encoderc_adcs_mpc_1(&rc_adcs_mpc_1_info, &rc_adcs_mpc_1_packet);
	canSendPacket(&rc_adcs_mpc_1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_8_packet = {0};
	rc_adcs_estim_8 rc_adcs_estim_8_info = {0};
	encoderc_adcs_estim_8(&rc_adcs_estim_8_info, &rc_adcs_estim_8_packet);
	canSendPacket(&rc_adcs_estim_8_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_7_packet = {0};
	rc_adcs_estim_7 rc_adcs_estim_7_info = {0};
	encoderc_adcs_estim_7(&rc_adcs_estim_7_info, &rc_adcs_estim_7_packet);
	canSendPacket(&rc_adcs_estim_7_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_6_packet = {0};
	rc_adcs_estim_6 rc_adcs_estim_6_info = {0};
	encoderc_adcs_estim_6(&rc_adcs_estim_6_info, &rc_adcs_estim_6_packet);
	canSendPacket(&rc_adcs_estim_6_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_5_packet = {0};
	rc_adcs_estim_5 rc_adcs_estim_5_info = {0};
	encoderc_adcs_estim_5(&rc_adcs_estim_5_info, &rc_adcs_estim_5_packet);
	canSendPacket(&rc_adcs_estim_5_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_4_packet = {0};
	rc_adcs_estim_4 rc_adcs_estim_4_info = {0};
	encoderc_adcs_estim_4(&rc_adcs_estim_4_info, &rc_adcs_estim_4_packet);
	canSendPacket(&rc_adcs_estim_4_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_2_packet = {0};
	rc_adcs_estim_2 rc_adcs_estim_2_info = {0};
	encoderc_adcs_estim_2(&rc_adcs_estim_2_info, &rc_adcs_estim_2_packet);
	canSendPacket(&rc_adcs_estim_2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_3_packet = {0};
	rc_adcs_estim_3 rc_adcs_estim_3_info = {0};
	encoderc_adcs_estim_3(&rc_adcs_estim_3_info, &rc_adcs_estim_3_packet);
	canSendPacket(&rc_adcs_estim_3_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_estim_1_packet = {0};
	rc_adcs_estim_1 rc_adcs_estim_1_info = {0};
	encoderc_adcs_estim_1(&rc_adcs_estim_1_info, &rc_adcs_estim_1_packet);
	canSendPacket(&rc_adcs_estim_1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_12_packet = {0};
	rc_adcs_sp_12 rc_adcs_sp_12_info = {0};
	encoderc_adcs_sp_12(&rc_adcs_sp_12_info, &rc_adcs_sp_12_packet);
	canSendPacket(&rc_adcs_sp_12_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_11_packet = {0};
	rc_adcs_sp_11 rc_adcs_sp_11_info = {0};
	encoderc_adcs_sp_11(&rc_adcs_sp_11_info, &rc_adcs_sp_11_packet);
	canSendPacket(&rc_adcs_sp_11_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_7_packet = {0};
	rc_adcs_sp_7 rc_adcs_sp_7_info = {0};
	encoderc_adcs_sp_7(&rc_adcs_sp_7_info, &rc_adcs_sp_7_packet);
	canSendPacket(&rc_adcs_sp_7_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_6_packet = {0};
	rc_adcs_sp_6 rc_adcs_sp_6_info = {0};
	encoderc_adcs_sp_6(&rc_adcs_sp_6_info, &rc_adcs_sp_6_packet);
	canSendPacket(&rc_adcs_sp_6_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_17_packet = {0};
	rc_adcs_sp_17 rc_adcs_sp_17_info = {0};
	encoderc_adcs_sp_17(&rc_adcs_sp_17_info, &rc_adcs_sp_17_packet);
	canSendPacket(&rc_adcs_sp_17_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_16_packet = {0};
	rc_adcs_sp_16 rc_adcs_sp_16_info = {0};
	encoderc_adcs_sp_16(&rc_adcs_sp_16_info, &rc_adcs_sp_16_packet);
	canSendPacket(&rc_adcs_sp_16_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_10_packet = {0};
	rc_adcs_sp_10 rc_adcs_sp_10_info = {0};
	encoderc_adcs_sp_10(&rc_adcs_sp_10_info, &rc_adcs_sp_10_packet);
	canSendPacket(&rc_adcs_sp_10_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_8_packet = {0};
	rc_adcs_sp_8 rc_adcs_sp_8_info = {0};
	encoderc_adcs_sp_8(&rc_adcs_sp_8_info, &rc_adcs_sp_8_packet);
	canSendPacket(&rc_adcs_sp_8_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_9_packet = {0};
	rc_adcs_sp_9 rc_adcs_sp_9_info = {0};
	encoderc_adcs_sp_9(&rc_adcs_sp_9_info, &rc_adcs_sp_9_packet);
	canSendPacket(&rc_adcs_sp_9_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_13_packet = {0};
	rc_adcs_sp_13 rc_adcs_sp_13_info = {0};
	encoderc_adcs_sp_13(&rc_adcs_sp_13_info, &rc_adcs_sp_13_packet);
	canSendPacket(&rc_adcs_sp_13_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_15_packet = {0};
	rc_adcs_sp_15 rc_adcs_sp_15_info = {0};
	encoderc_adcs_sp_15(&rc_adcs_sp_15_info, &rc_adcs_sp_15_packet);
	canSendPacket(&rc_adcs_sp_15_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_14_packet = {0};
	rc_adcs_sp_14 rc_adcs_sp_14_info = {0};
	encoderc_adcs_sp_14(&rc_adcs_sp_14_info, &rc_adcs_sp_14_packet);
	canSendPacket(&rc_adcs_sp_14_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_5_packet = {0};
	rc_adcs_sp_5 rc_adcs_sp_5_info = {0};
	encoderc_adcs_sp_5(&rc_adcs_sp_5_info, &rc_adcs_sp_5_packet);
	canSendPacket(&rc_adcs_sp_5_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_4_packet = {0};
	rc_adcs_sp_4 rc_adcs_sp_4_info = {0};
	encoderc_adcs_sp_4(&rc_adcs_sp_4_info, &rc_adcs_sp_4_packet);
	canSendPacket(&rc_adcs_sp_4_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_3_packet = {0};
	rc_adcs_sp_3 rc_adcs_sp_3_info = {0};
	encoderc_adcs_sp_3(&rc_adcs_sp_3_info, &rc_adcs_sp_3_packet);
	canSendPacket(&rc_adcs_sp_3_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_2_packet = {0};
	rc_adcs_sp_2 rc_adcs_sp_2_info = {0};
	encoderc_adcs_sp_2(&rc_adcs_sp_2_info, &rc_adcs_sp_2_packet);
	canSendPacket(&rc_adcs_sp_2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_sp_1_packet = {0};
	rc_adcs_sp_1 rc_adcs_sp_1_info = {0};
	encoderc_adcs_sp_1(&rc_adcs_sp_1_info, &rc_adcs_sp_1_packet);
	canSendPacket(&rc_adcs_sp_1_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_4_packet = {0};
	rc_adcs_bdot_4 rc_adcs_bdot_4_info = {0};
	encoderc_adcs_bdot_4(&rc_adcs_bdot_4_info, &rc_adcs_bdot_4_packet);
	canSendPacket(&rc_adcs_bdot_4_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_3_packet = {0};
	rc_adcs_bdot_3 rc_adcs_bdot_3_info = {0};
	encoderc_adcs_bdot_3(&rc_adcs_bdot_3_info, &rc_adcs_bdot_3_packet);
	canSendPacket(&rc_adcs_bdot_3_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_2_packet = {0};
	rc_adcs_bdot_2 rc_adcs_bdot_2_info = {0};
	encoderc_adcs_bdot_2(&rc_adcs_bdot_2_info, &rc_adcs_bdot_2_packet);
	canSendPacket(&rc_adcs_bdot_2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_bdot_1_packet = {0};
	rc_adcs_bdot_1 rc_adcs_bdot_1_info = {0};
	encoderc_adcs_bdot_1(&rc_adcs_bdot_1_info, &rc_adcs_bdot_1_packet);
	canSendPacket(&rc_adcs_bdot_1_packet);

	__delay_cycles(10000);	CANPacket estim_sun_unit_z_packet = {0};
	estim_sun_unit_z estim_sun_unit_z_info = {0};
	encodeestim_sun_unit_z(&estim_sun_unit_z_info, &estim_sun_unit_z_packet);
	canSendPacket(&estim_sun_unit_z_packet);

	__delay_cycles(10000);	CANPacket estim_sun_unit_y_packet = {0};
	estim_sun_unit_y estim_sun_unit_y_info = {0};
	encodeestim_sun_unit_y(&estim_sun_unit_y_info, &estim_sun_unit_y_packet);
	canSendPacket(&estim_sun_unit_y_packet);

	__delay_cycles(10000);	CANPacket estim_sun_unit_x_packet = {0};
	estim_sun_unit_x estim_sun_unit_x_info = {0};
	encodeestim_sun_unit_x(&estim_sun_unit_x_info, &estim_sun_unit_x_packet);
	canSendPacket(&estim_sun_unit_x_packet);

	__delay_cycles(10000);	CANPacket estim_mag_unit_z_packet = {0};
	estim_mag_unit_z estim_mag_unit_z_info = {0};
	encodeestim_mag_unit_z(&estim_mag_unit_z_info, &estim_mag_unit_z_packet);
	canSendPacket(&estim_mag_unit_z_packet);

	__delay_cycles(10000);	CANPacket estim_mag_unit_y_packet = {0};
	estim_mag_unit_y estim_mag_unit_y_info = {0};
	encodeestim_mag_unit_y(&estim_mag_unit_y_info, &estim_mag_unit_y_packet);
	canSendPacket(&estim_mag_unit_y_packet);

	__delay_cycles(10000);	CANPacket estim_mag_unit_x_packet = {0};
	estim_mag_unit_x estim_mag_unit_x_info = {0};
	encodeestim_mag_unit_x(&estim_mag_unit_x_info, &estim_mag_unit_x_packet);
	canSendPacket(&estim_mag_unit_x_packet);

	__delay_cycles(10000);	CANPacket estim_state_packet = {0};
	estim_state estim_state_info = {0};
	encodeestim_state(&estim_state_info, &estim_state_packet);
	canSendPacket(&estim_state_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_5_packet = {0};
	rc_adcs_mtq_5 rc_adcs_mtq_5_info = {0};
	encoderc_adcs_mtq_5(&rc_adcs_mtq_5_info, &rc_adcs_mtq_5_packet);
	canSendPacket(&rc_adcs_mtq_5_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_4_packet = {0};
	rc_adcs_mtq_4 rc_adcs_mtq_4_info = {0};
	encoderc_adcs_mtq_4(&rc_adcs_mtq_4_info, &rc_adcs_mtq_4_packet);
	canSendPacket(&rc_adcs_mtq_4_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_3_packet = {0};
	rc_adcs_mtq_3 rc_adcs_mtq_3_info = {0};
	encoderc_adcs_mtq_3(&rc_adcs_mtq_3_info, &rc_adcs_mtq_3_packet);
	canSendPacket(&rc_adcs_mtq_3_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_2_packet = {0};
	rc_adcs_mtq_2 rc_adcs_mtq_2_info = {0};
	encoderc_adcs_mtq_2(&rc_adcs_mtq_2_info, &rc_adcs_mtq_2_packet);
	canSendPacket(&rc_adcs_mtq_2_packet);

	__delay_cycles(10000);	CANPacket rc_adcs_mtq_1_packet = {0};
	rc_adcs_mtq_1 rc_adcs_mtq_1_info = {0};
	encoderc_adcs_mtq_1(&rc_adcs_mtq_1_info, &rc_adcs_mtq_1_packet);
	canSendPacket(&rc_adcs_mtq_1_packet);

	__delay_cycles(10000);	CANPacket rc_ppt_3_packet = {0};
	rc_ppt_3 rc_ppt_3_info = {0};
	encoderc_ppt_3(&rc_ppt_3_info, &rc_ppt_3_packet);
	canSendPacket(&rc_ppt_3_packet);

	__delay_cycles(10000);	CANPacket rc_ppt_2_packet = {0};
	rc_ppt_2 rc_ppt_2_info = {0};
	encoderc_ppt_2(&rc_ppt_2_info, &rc_ppt_2_packet);
	canSendPacket(&rc_ppt_2_packet);

	__delay_cycles(10000);	CANPacket rc_ppt_1_packet = {0};
	rc_ppt_1 rc_ppt_1_info = {0};
	encoderc_ppt_1(&rc_ppt_1_info, &rc_ppt_1_packet);
	canSendPacket(&rc_ppt_1_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_9_packet = {0};
	rc_eps_gen_9 rc_eps_gen_9_info = {0};
	encoderc_eps_gen_9(&rc_eps_gen_9_info, &rc_eps_gen_9_packet);
	canSendPacket(&rc_eps_gen_9_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_8_packet = {0};
	rc_eps_gen_8 rc_eps_gen_8_info = {0};
	encoderc_eps_gen_8(&rc_eps_gen_8_info, &rc_eps_gen_8_packet);
	canSendPacket(&rc_eps_gen_8_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_7_packet = {0};
	rc_eps_gen_7 rc_eps_gen_7_info = {0};
	encoderc_eps_gen_7(&rc_eps_gen_7_info, &rc_eps_gen_7_packet);
	canSendPacket(&rc_eps_gen_7_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_6_packet = {0};
	rc_eps_gen_6 rc_eps_gen_6_info = {0};
	encoderc_eps_gen_6(&rc_eps_gen_6_info, &rc_eps_gen_6_packet);
	canSendPacket(&rc_eps_gen_6_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_5_packet = {0};
	rc_eps_gen_5 rc_eps_gen_5_info = {0};
	encoderc_eps_gen_5(&rc_eps_gen_5_info, &rc_eps_gen_5_packet);
	canSendPacket(&rc_eps_gen_5_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_4_packet = {0};
	rc_eps_gen_4 rc_eps_gen_4_info = {0};
	encoderc_eps_gen_4(&rc_eps_gen_4_info, &rc_eps_gen_4_packet);
	canSendPacket(&rc_eps_gen_4_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_3_packet = {0};
	rc_eps_gen_3 rc_eps_gen_3_info = {0};
	encoderc_eps_gen_3(&rc_eps_gen_3_info, &rc_eps_gen_3_packet);
	canSendPacket(&rc_eps_gen_3_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_2_packet = {0};
	rc_eps_gen_2 rc_eps_gen_2_info = {0};
	encoderc_eps_gen_2(&rc_eps_gen_2_info, &rc_eps_gen_2_packet);
	canSendPacket(&rc_eps_gen_2_packet);

	__delay_cycles(10000);	CANPacket rc_eps_gen_1_packet = {0};
	rc_eps_gen_1 rc_eps_gen_1_info = {0};
	encoderc_eps_gen_1(&rc_eps_gen_1_info, &rc_eps_gen_1_packet);
	canSendPacket(&rc_eps_gen_1_packet);

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

	__delay_cycles(10000);	CANPacket ppt_firing_result_packet = {0};
	ppt_firing_result ppt_firing_result_info = {0};
	encodeppt_firing_result(&ppt_firing_result_info, &ppt_firing_result_packet);
	canSendPacket(&ppt_firing_result_packet);

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

	__delay_cycles(10000);	CANPacket cmd_pd_rst_packet = {0};
	cmd_pd_rst cmd_pd_rst_info = {0};
	encodecmd_pd_rst(&cmd_pd_rst_info, &cmd_pd_rst_packet);
	canSendPacket(&cmd_pd_rst_packet);

	__delay_cycles(10000);	CANPacket gcmd_com2_run_packet = {0};
	gcmd_com2_run gcmd_com2_run_info = {0};
	encodegcmd_com2_run(&gcmd_com2_run_info, &gcmd_com2_run_packet);
	canSendPacket(&gcmd_com2_run_packet);

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

	__delay_cycles(10000);	CANPacket cmd_ppt_single_fire_packet = {0};
	cmd_ppt_single_fire cmd_ppt_single_fire_info = {0};
	encodecmd_ppt_single_fire(&cmd_ppt_single_fire_info, &cmd_ppt_single_fire_packet);
	canSendPacket(&cmd_ppt_single_fire_packet);

	__delay_cycles(10000);	CANPacket mpc_vp_packet = {0};
	mpc_vp mpc_vp_info = {0};
	encodempc_vp(&mpc_vp_info, &mpc_vp_packet);
	canSendPacket(&mpc_vp_packet);

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

	__delay_cycles(10000);	CANPacket sync_1_packet = {0};
	sync_1 sync_1_info = {0};
	encodesync_1(&sync_1_info, &sync_1_packet);
	canSendPacket(&sync_1_packet);

	__delay_cycles(10000);	CANPacket grnd_epoch_packet = {0};
	grnd_epoch grnd_epoch_info = {0};
	encodegrnd_epoch(&grnd_epoch_info, &grnd_epoch_packet);
	canSendPacket(&grnd_epoch_packet);

} 
while (1) { 
	canBlast(); 
	int i=0; 
	for(int i=0;i<100000;i++){} 
}