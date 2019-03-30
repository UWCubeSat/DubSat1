
#include <msp430.h>
#include <stddef.h>
#include "interfaces/canwrap.h"
int rcFlag=0;void canBlast() { 
	 while(rcFlag>0 && (canTxCheck() != CAN_TX_BUSY)){
		if(rcFlag == 0){
			CANPacket rc_adcs_sp_18_packet = {0};
			rc_adcs_sp_18 rc_adcs_sp_18_info = {0};
			encoderc_adcs_sp_18(&rc_adcs_sp_18_info, &rc_adcs_sp_18_packet);
			canSendPacket(&rc_adcs_sp_18_packet);
		}
		if(rcFlag == 1){
			CANPacket rc_adcs_bdot_11_packet = {0};
			rc_adcs_bdot_11 rc_adcs_bdot_11_info = {0};
			encoderc_adcs_bdot_11(&rc_adcs_bdot_11_info, &rc_adcs_bdot_11_packet);
			canSendPacket(&rc_adcs_bdot_11_packet);
		}
		if(rcFlag == 2){
			CANPacket rc_eps_gen_10_packet = {0};
			rc_eps_gen_10 rc_eps_gen_10_info = {0};
			encoderc_eps_gen_10(&rc_eps_gen_10_info, &rc_eps_gen_10_packet);
			canSendPacket(&rc_eps_gen_10_packet);
		}
		if(rcFlag == 3){
			CANPacket rc_eps_dist_18_packet = {0};
			rc_eps_dist_18 rc_eps_dist_18_info = {0};
			encoderc_eps_dist_18(&rc_eps_dist_18_info, &rc_eps_dist_18_packet);
			canSendPacket(&rc_eps_dist_18_packet);
		}
		if(rcFlag == 4){
			CANPacket rc_adcs_bdot_10_packet = {0};
			rc_adcs_bdot_10 rc_adcs_bdot_10_info = {0};
			encoderc_adcs_bdot_10(&rc_adcs_bdot_10_info, &rc_adcs_bdot_10_packet);
			canSendPacket(&rc_adcs_bdot_10_packet);
		}
		if(rcFlag == 5){
			CANPacket rc_adcs_bdot_9_packet = {0};
			rc_adcs_bdot_9 rc_adcs_bdot_9_info = {0};
			encoderc_adcs_bdot_9(&rc_adcs_bdot_9_info, &rc_adcs_bdot_9_packet);
			canSendPacket(&rc_adcs_bdot_9_packet);
		}
		if(rcFlag == 6){
			CANPacket rc_adcs_bdot_8_packet = {0};
			rc_adcs_bdot_8 rc_adcs_bdot_8_info = {0};
			encoderc_adcs_bdot_8(&rc_adcs_bdot_8_info, &rc_adcs_bdot_8_packet);
			canSendPacket(&rc_adcs_bdot_8_packet);
		}
		if(rcFlag == 7){
			CANPacket rc_adcs_bdot_7_packet = {0};
			rc_adcs_bdot_7 rc_adcs_bdot_7_info = {0};
			encoderc_adcs_bdot_7(&rc_adcs_bdot_7_info, &rc_adcs_bdot_7_packet);
			canSendPacket(&rc_adcs_bdot_7_packet);
		}
		if(rcFlag == 8){
			CANPacket rc_adcs_bdot_6_packet = {0};
			rc_adcs_bdot_6 rc_adcs_bdot_6_info = {0};
			encoderc_adcs_bdot_6(&rc_adcs_bdot_6_info, &rc_adcs_bdot_6_packet);
			canSendPacket(&rc_adcs_bdot_6_packet);
		}
		if(rcFlag == 9){
			CANPacket rc_adcs_bdot_5_packet = {0};
			rc_adcs_bdot_5 rc_adcs_bdot_5_info = {0};
			encoderc_adcs_bdot_5(&rc_adcs_bdot_5_info, &rc_adcs_bdot_5_packet);
			canSendPacket(&rc_adcs_bdot_5_packet);
		}
		if(rcFlag == 10){
			CANPacket rc_eps_batt_h2_packet = {0};
			rc_eps_batt_h2 rc_eps_batt_h2_info = {0};
			encoderc_eps_batt_h2(&rc_eps_batt_h2_info, &rc_eps_batt_h2_packet);
			canSendPacket(&rc_eps_batt_h2_packet);
		}
		if(rcFlag == 11){
			CANPacket rc_adcs_bdot_h2_packet = {0};
			rc_adcs_bdot_h2 rc_adcs_bdot_h2_info = {0};
			encoderc_adcs_bdot_h2(&rc_adcs_bdot_h2_info, &rc_adcs_bdot_h2_packet);
			canSendPacket(&rc_adcs_bdot_h2_packet);
		}
		if(rcFlag == 12){
			CANPacket rc_eps_dist_h2_packet = {0};
			rc_eps_dist_h2 rc_eps_dist_h2_info = {0};
			encoderc_eps_dist_h2(&rc_eps_dist_h2_info, &rc_eps_dist_h2_packet);
			canSendPacket(&rc_eps_dist_h2_packet);
		}
		if(rcFlag == 13){
			CANPacket rc_adcs_mpc_h2_packet = {0};
			rc_adcs_mpc_h2 rc_adcs_mpc_h2_info = {0};
			encoderc_adcs_mpc_h2(&rc_adcs_mpc_h2_info, &rc_adcs_mpc_h2_packet);
			canSendPacket(&rc_adcs_mpc_h2_packet);
		}
		if(rcFlag == 14){
			CANPacket rc_adcs_estim_h2_packet = {0};
			rc_adcs_estim_h2 rc_adcs_estim_h2_info = {0};
			encoderc_adcs_estim_h2(&rc_adcs_estim_h2_info, &rc_adcs_estim_h2_packet);
			canSendPacket(&rc_adcs_estim_h2_packet);
		}
		if(rcFlag == 15){
			CANPacket rc_eps_gen_h2_packet = {0};
			rc_eps_gen_h2 rc_eps_gen_h2_info = {0};
			encoderc_eps_gen_h2(&rc_eps_gen_h2_info, &rc_eps_gen_h2_packet);
			canSendPacket(&rc_eps_gen_h2_packet);
		}
		if(rcFlag == 16){
			CANPacket rc_adcs_mtq_h2_packet = {0};
			rc_adcs_mtq_h2 rc_adcs_mtq_h2_info = {0};
			encoderc_adcs_mtq_h2(&rc_adcs_mtq_h2_info, &rc_adcs_mtq_h2_packet);
			canSendPacket(&rc_adcs_mtq_h2_packet);
		}
		if(rcFlag == 17){
			CANPacket rc_ppt_h2_packet = {0};
			rc_ppt_h2 rc_ppt_h2_info = {0};
			encoderc_ppt_h2(&rc_ppt_h2_info, &rc_ppt_h2_packet);
			canSendPacket(&rc_ppt_h2_packet);
		}
		if(rcFlag == 18){
			CANPacket rc_eps_batt_h1_packet = {0};
			rc_eps_batt_h1 rc_eps_batt_h1_info = {0};
			encoderc_eps_batt_h1(&rc_eps_batt_h1_info, &rc_eps_batt_h1_packet);
			canSendPacket(&rc_eps_batt_h1_packet);
		}
		if(rcFlag == 19){
			CANPacket rc_adcs_bdot_h1_packet = {0};
			rc_adcs_bdot_h1 rc_adcs_bdot_h1_info = {0};
			encoderc_adcs_bdot_h1(&rc_adcs_bdot_h1_info, &rc_adcs_bdot_h1_packet);
			canSendPacket(&rc_adcs_bdot_h1_packet);
		}
		if(rcFlag == 20){
			CANPacket rc_eps_dist_h1_packet = {0};
			rc_eps_dist_h1 rc_eps_dist_h1_info = {0};
			encoderc_eps_dist_h1(&rc_eps_dist_h1_info, &rc_eps_dist_h1_packet);
			canSendPacket(&rc_eps_dist_h1_packet);
		}
		if(rcFlag == 21){
			CANPacket rc_adcs_mpc_h1_packet = {0};
			rc_adcs_mpc_h1 rc_adcs_mpc_h1_info = {0};
			encoderc_adcs_mpc_h1(&rc_adcs_mpc_h1_info, &rc_adcs_mpc_h1_packet);
			canSendPacket(&rc_adcs_mpc_h1_packet);
		}
		if(rcFlag == 22){
			CANPacket rc_adcs_estim_h1_packet = {0};
			rc_adcs_estim_h1 rc_adcs_estim_h1_info = {0};
			encoderc_adcs_estim_h1(&rc_adcs_estim_h1_info, &rc_adcs_estim_h1_packet);
			canSendPacket(&rc_adcs_estim_h1_packet);
		}
		if(rcFlag == 23){
			CANPacket rc_adcs_sp_h1_packet = {0};
			rc_adcs_sp_h1 rc_adcs_sp_h1_info = {0};
			encoderc_adcs_sp_h1(&rc_adcs_sp_h1_info, &rc_adcs_sp_h1_packet);
			canSendPacket(&rc_adcs_sp_h1_packet);
		}
		if(rcFlag == 24){
			CANPacket rc_eps_gen_h1_packet = {0};
			rc_eps_gen_h1 rc_eps_gen_h1_info = {0};
			encoderc_eps_gen_h1(&rc_eps_gen_h1_info, &rc_eps_gen_h1_packet);
			canSendPacket(&rc_eps_gen_h1_packet);
		}
		if(rcFlag == 25){
			CANPacket rc_adcs_mtq_h1_packet = {0};
			rc_adcs_mtq_h1 rc_adcs_mtq_h1_info = {0};
			encoderc_adcs_mtq_h1(&rc_adcs_mtq_h1_info, &rc_adcs_mtq_h1_packet);
			canSendPacket(&rc_adcs_mtq_h1_packet);
		}
		if(rcFlag == 26){
			CANPacket rc_ppt_h1_packet = {0};
			rc_ppt_h1 rc_ppt_h1_info = {0};
			encoderc_ppt_h1(&rc_ppt_h1_info, &rc_ppt_h1_packet);
			canSendPacket(&rc_ppt_h1_packet);
		}
		if(rcFlag == 27){
			CANPacket rc_eps_batt_7_packet = {0};
			rc_eps_batt_7 rc_eps_batt_7_info = {0};
			encoderc_eps_batt_7(&rc_eps_batt_7_info, &rc_eps_batt_7_packet);
			canSendPacket(&rc_eps_batt_7_packet);
		}
		if(rcFlag == 28){
			CANPacket rc_adcs_estim_14_packet = {0};
			rc_adcs_estim_14 rc_adcs_estim_14_info = {0};
			encoderc_adcs_estim_14(&rc_adcs_estim_14_info, &rc_adcs_estim_14_packet);
			canSendPacket(&rc_adcs_estim_14_packet);
		}
		if(rcFlag == 29){
			CANPacket rc_adcs_estim_13_packet = {0};
			rc_adcs_estim_13 rc_adcs_estim_13_info = {0};
			encoderc_adcs_estim_13(&rc_adcs_estim_13_info, &rc_adcs_estim_13_packet);
			canSendPacket(&rc_adcs_estim_13_packet);
		}
		if(rcFlag == 30){
			CANPacket rc_adcs_estim_12_packet = {0};
			rc_adcs_estim_12 rc_adcs_estim_12_info = {0};
			encoderc_adcs_estim_12(&rc_adcs_estim_12_info, &rc_adcs_estim_12_packet);
			canSendPacket(&rc_adcs_estim_12_packet);
		}
		if(rcFlag == 31){
			CANPacket rc_adcs_estim_11_packet = {0};
			rc_adcs_estim_11 rc_adcs_estim_11_info = {0};
			encoderc_adcs_estim_11(&rc_adcs_estim_11_info, &rc_adcs_estim_11_packet);
			canSendPacket(&rc_adcs_estim_11_packet);
		}
		if(rcFlag == 32){
			CANPacket rc_adcs_estim_10_packet = {0};
			rc_adcs_estim_10 rc_adcs_estim_10_info = {0};
			encoderc_adcs_estim_10(&rc_adcs_estim_10_info, &rc_adcs_estim_10_packet);
			canSendPacket(&rc_adcs_estim_10_packet);
		}
		if(rcFlag == 33){
			CANPacket rc_adcs_estim_9_packet = {0};
			rc_adcs_estim_9 rc_adcs_estim_9_info = {0};
			encoderc_adcs_estim_9(&rc_adcs_estim_9_info, &rc_adcs_estim_9_packet);
			canSendPacket(&rc_adcs_estim_9_packet);
		}
		if(rcFlag == 34){
			CANPacket rc_eps_dist_16_packet = {0};
			rc_eps_dist_16 rc_eps_dist_16_info = {0};
			encoderc_eps_dist_16(&rc_eps_dist_16_info, &rc_eps_dist_16_packet);
			canSendPacket(&rc_eps_dist_16_packet);
		}
		if(rcFlag == 35){
			CANPacket rc_eps_dist_9_packet = {0};
			rc_eps_dist_9 rc_eps_dist_9_info = {0};
			encoderc_eps_dist_9(&rc_eps_dist_9_info, &rc_eps_dist_9_packet);
			canSendPacket(&rc_eps_dist_9_packet);
		}
		if(rcFlag == 36){
			CANPacket rc_eps_dist_7_packet = {0};
			rc_eps_dist_7 rc_eps_dist_7_info = {0};
			encoderc_eps_dist_7(&rc_eps_dist_7_info, &rc_eps_dist_7_packet);
			canSendPacket(&rc_eps_dist_7_packet);
		}
		if(rcFlag == 37){
			CANPacket rc_eps_dist_6_packet = {0};
			rc_eps_dist_6 rc_eps_dist_6_info = {0};
			encoderc_eps_dist_6(&rc_eps_dist_6_info, &rc_eps_dist_6_packet);
			canSendPacket(&rc_eps_dist_6_packet);
		}
		if(rcFlag == 38){
			CANPacket rc_eps_dist_5_packet = {0};
			rc_eps_dist_5 rc_eps_dist_5_info = {0};
			encoderc_eps_dist_5(&rc_eps_dist_5_info, &rc_eps_dist_5_packet);
			canSendPacket(&rc_eps_dist_5_packet);
		}
		if(rcFlag == 39){
			CANPacket rc_eps_dist_3_packet = {0};
			rc_eps_dist_3 rc_eps_dist_3_info = {0};
			encoderc_eps_dist_3(&rc_eps_dist_3_info, &rc_eps_dist_3_packet);
			canSendPacket(&rc_eps_dist_3_packet);
		}
		if(rcFlag == 40){
			CANPacket rc_eps_dist_2_packet = {0};
			rc_eps_dist_2 rc_eps_dist_2_info = {0};
			encoderc_eps_dist_2(&rc_eps_dist_2_info, &rc_eps_dist_2_packet);
			canSendPacket(&rc_eps_dist_2_packet);
		}
		if(rcFlag == 41){
			CANPacket rc_eps_dist_17_packet = {0};
			rc_eps_dist_17 rc_eps_dist_17_info = {0};
			encoderc_eps_dist_17(&rc_eps_dist_17_info, &rc_eps_dist_17_packet);
			canSendPacket(&rc_eps_dist_17_packet);
		}
		if(rcFlag == 42){
			CANPacket rc_eps_dist_14_packet = {0};
			rc_eps_dist_14 rc_eps_dist_14_info = {0};
			encoderc_eps_dist_14(&rc_eps_dist_14_info, &rc_eps_dist_14_packet);
			canSendPacket(&rc_eps_dist_14_packet);
		}
		if(rcFlag == 43){
			CANPacket rc_eps_dist_13_packet = {0};
			rc_eps_dist_13 rc_eps_dist_13_info = {0};
			encoderc_eps_dist_13(&rc_eps_dist_13_info, &rc_eps_dist_13_packet);
			canSendPacket(&rc_eps_dist_13_packet);
		}
		if(rcFlag == 44){
			CANPacket rc_eps_dist_11_packet = {0};
			rc_eps_dist_11 rc_eps_dist_11_info = {0};
			encoderc_eps_dist_11(&rc_eps_dist_11_info, &rc_eps_dist_11_packet);
			canSendPacket(&rc_eps_dist_11_packet);
		}
		if(rcFlag == 45){
			CANPacket rc_eps_dist_12_packet = {0};
			rc_eps_dist_12 rc_eps_dist_12_info = {0};
			encoderc_eps_dist_12(&rc_eps_dist_12_info, &rc_eps_dist_12_packet);
			canSendPacket(&rc_eps_dist_12_packet);
		}
		if(rcFlag == 46){
			CANPacket rc_eps_dist_15_packet = {0};
			rc_eps_dist_15 rc_eps_dist_15_info = {0};
			encoderc_eps_dist_15(&rc_eps_dist_15_info, &rc_eps_dist_15_packet);
			canSendPacket(&rc_eps_dist_15_packet);
		}
		if(rcFlag == 47){
			CANPacket rc_eps_dist_10_packet = {0};
			rc_eps_dist_10 rc_eps_dist_10_info = {0};
			encoderc_eps_dist_10(&rc_eps_dist_10_info, &rc_eps_dist_10_packet);
			canSendPacket(&rc_eps_dist_10_packet);
		}
		if(rcFlag == 48){
			CANPacket rc_eps_dist_8_packet = {0};
			rc_eps_dist_8 rc_eps_dist_8_info = {0};
			encoderc_eps_dist_8(&rc_eps_dist_8_info, &rc_eps_dist_8_packet);
			canSendPacket(&rc_eps_dist_8_packet);
		}
		if(rcFlag == 49){
			CANPacket rc_eps_dist_4_packet = {0};
			rc_eps_dist_4 rc_eps_dist_4_info = {0};
			encoderc_eps_dist_4(&rc_eps_dist_4_info, &rc_eps_dist_4_packet);
			canSendPacket(&rc_eps_dist_4_packet);
		}
		if(rcFlag == 50){
			CANPacket rc_eps_dist_1_packet = {0};
			rc_eps_dist_1 rc_eps_dist_1_info = {0};
			encoderc_eps_dist_1(&rc_eps_dist_1_info, &rc_eps_dist_1_packet);
			canSendPacket(&rc_eps_dist_1_packet);
		}
		if(rcFlag == 51){
			CANPacket rc_adcs_mpc_11_packet = {0};
			rc_adcs_mpc_11 rc_adcs_mpc_11_info = {0};
			encoderc_adcs_mpc_11(&rc_adcs_mpc_11_info, &rc_adcs_mpc_11_packet);
			canSendPacket(&rc_adcs_mpc_11_packet);
		}
		if(rcFlag == 52){
			CANPacket rc_adcs_mpc_7_packet = {0};
			rc_adcs_mpc_7 rc_adcs_mpc_7_info = {0};
			encoderc_adcs_mpc_7(&rc_adcs_mpc_7_info, &rc_adcs_mpc_7_packet);
			canSendPacket(&rc_adcs_mpc_7_packet);
		}
		if(rcFlag == 53){
			CANPacket rc_adcs_mpc_8_packet = {0};
			rc_adcs_mpc_8 rc_adcs_mpc_8_info = {0};
			encoderc_adcs_mpc_8(&rc_adcs_mpc_8_info, &rc_adcs_mpc_8_packet);
			canSendPacket(&rc_adcs_mpc_8_packet);
		}
		if(rcFlag == 54){
			CANPacket rc_adcs_mpc_9_packet = {0};
			rc_adcs_mpc_9 rc_adcs_mpc_9_info = {0};
			encoderc_adcs_mpc_9(&rc_adcs_mpc_9_info, &rc_adcs_mpc_9_packet);
			canSendPacket(&rc_adcs_mpc_9_packet);
		}
		if(rcFlag == 55){
			CANPacket rc_adcs_mpc_5_packet = {0};
			rc_adcs_mpc_5 rc_adcs_mpc_5_info = {0};
			encoderc_adcs_mpc_5(&rc_adcs_mpc_5_info, &rc_adcs_mpc_5_packet);
			canSendPacket(&rc_adcs_mpc_5_packet);
		}
		if(rcFlag == 56){
			CANPacket rc_adcs_mpc_6_packet = {0};
			rc_adcs_mpc_6 rc_adcs_mpc_6_info = {0};
			encoderc_adcs_mpc_6(&rc_adcs_mpc_6_info, &rc_adcs_mpc_6_packet);
			canSendPacket(&rc_adcs_mpc_6_packet);
		}
		if(rcFlag == 57){
			CANPacket rc_adcs_mpc_4_packet = {0};
			rc_adcs_mpc_4 rc_adcs_mpc_4_info = {0};
			encoderc_adcs_mpc_4(&rc_adcs_mpc_4_info, &rc_adcs_mpc_4_packet);
			canSendPacket(&rc_adcs_mpc_4_packet);
		}
		if(rcFlag == 58){
			CANPacket rc_adcs_mpc_3_packet = {0};
			rc_adcs_mpc_3 rc_adcs_mpc_3_info = {0};
			encoderc_adcs_mpc_3(&rc_adcs_mpc_3_info, &rc_adcs_mpc_3_packet);
			canSendPacket(&rc_adcs_mpc_3_packet);
		}
		if(rcFlag == 59){
			CANPacket rc_adcs_mpc_15_packet = {0};
			rc_adcs_mpc_15 rc_adcs_mpc_15_info = {0};
			encoderc_adcs_mpc_15(&rc_adcs_mpc_15_info, &rc_adcs_mpc_15_packet);
			canSendPacket(&rc_adcs_mpc_15_packet);
		}
		if(rcFlag == 60){
			CANPacket rc_adcs_mpc_12_packet = {0};
			rc_adcs_mpc_12 rc_adcs_mpc_12_info = {0};
			encoderc_adcs_mpc_12(&rc_adcs_mpc_12_info, &rc_adcs_mpc_12_packet);
			canSendPacket(&rc_adcs_mpc_12_packet);
		}
		if(rcFlag == 61){
			CANPacket rc_adcs_mpc_13_packet = {0};
			rc_adcs_mpc_13 rc_adcs_mpc_13_info = {0};
			encoderc_adcs_mpc_13(&rc_adcs_mpc_13_info, &rc_adcs_mpc_13_packet);
			canSendPacket(&rc_adcs_mpc_13_packet);
		}
		if(rcFlag == 62){
			CANPacket rc_adcs_mpc_14_packet = {0};
			rc_adcs_mpc_14 rc_adcs_mpc_14_info = {0};
			encoderc_adcs_mpc_14(&rc_adcs_mpc_14_info, &rc_adcs_mpc_14_packet);
			canSendPacket(&rc_adcs_mpc_14_packet);
		}
		if(rcFlag == 63){
			CANPacket rc_adcs_mpc_10_packet = {0};
			rc_adcs_mpc_10 rc_adcs_mpc_10_info = {0};
			encoderc_adcs_mpc_10(&rc_adcs_mpc_10_info, &rc_adcs_mpc_10_packet);
			canSendPacket(&rc_adcs_mpc_10_packet);
		}
		if(rcFlag == 64){
			CANPacket rc_adcs_mpc_2_packet = {0};
			rc_adcs_mpc_2 rc_adcs_mpc_2_info = {0};
			encoderc_adcs_mpc_2(&rc_adcs_mpc_2_info, &rc_adcs_mpc_2_packet);
			canSendPacket(&rc_adcs_mpc_2_packet);
		}
		if(rcFlag == 65){
			CANPacket rc_adcs_mpc_1_packet = {0};
			rc_adcs_mpc_1 rc_adcs_mpc_1_info = {0};
			encoderc_adcs_mpc_1(&rc_adcs_mpc_1_info, &rc_adcs_mpc_1_packet);
			canSendPacket(&rc_adcs_mpc_1_packet);
		}
		if(rcFlag == 66){
			CANPacket rc_adcs_estim_8_packet = {0};
			rc_adcs_estim_8 rc_adcs_estim_8_info = {0};
			encoderc_adcs_estim_8(&rc_adcs_estim_8_info, &rc_adcs_estim_8_packet);
			canSendPacket(&rc_adcs_estim_8_packet);
		}
		if(rcFlag == 67){
			CANPacket rc_adcs_estim_7_packet = {0};
			rc_adcs_estim_7 rc_adcs_estim_7_info = {0};
			encoderc_adcs_estim_7(&rc_adcs_estim_7_info, &rc_adcs_estim_7_packet);
			canSendPacket(&rc_adcs_estim_7_packet);
		}
		if(rcFlag == 68){
			CANPacket rc_adcs_estim_6_packet = {0};
			rc_adcs_estim_6 rc_adcs_estim_6_info = {0};
			encoderc_adcs_estim_6(&rc_adcs_estim_6_info, &rc_adcs_estim_6_packet);
			canSendPacket(&rc_adcs_estim_6_packet);
		}
		if(rcFlag == 69){
			CANPacket rc_adcs_estim_5_packet = {0};
			rc_adcs_estim_5 rc_adcs_estim_5_info = {0};
			encoderc_adcs_estim_5(&rc_adcs_estim_5_info, &rc_adcs_estim_5_packet);
			canSendPacket(&rc_adcs_estim_5_packet);
		}
		if(rcFlag == 70){
			CANPacket rc_adcs_estim_4_packet = {0};
			rc_adcs_estim_4 rc_adcs_estim_4_info = {0};
			encoderc_adcs_estim_4(&rc_adcs_estim_4_info, &rc_adcs_estim_4_packet);
			canSendPacket(&rc_adcs_estim_4_packet);
		}
		if(rcFlag == 71){
			CANPacket rc_adcs_estim_2_packet = {0};
			rc_adcs_estim_2 rc_adcs_estim_2_info = {0};
			encoderc_adcs_estim_2(&rc_adcs_estim_2_info, &rc_adcs_estim_2_packet);
			canSendPacket(&rc_adcs_estim_2_packet);
		}
		if(rcFlag == 72){
			CANPacket rc_adcs_estim_3_packet = {0};
			rc_adcs_estim_3 rc_adcs_estim_3_info = {0};
			encoderc_adcs_estim_3(&rc_adcs_estim_3_info, &rc_adcs_estim_3_packet);
			canSendPacket(&rc_adcs_estim_3_packet);
		}
		if(rcFlag == 73){
			CANPacket rc_adcs_estim_1_packet = {0};
			rc_adcs_estim_1 rc_adcs_estim_1_info = {0};
			encoderc_adcs_estim_1(&rc_adcs_estim_1_info, &rc_adcs_estim_1_packet);
			canSendPacket(&rc_adcs_estim_1_packet);
		}
		if(rcFlag == 74){
			CANPacket rc_adcs_sp_12_packet = {0};
			rc_adcs_sp_12 rc_adcs_sp_12_info = {0};
			encoderc_adcs_sp_12(&rc_adcs_sp_12_info, &rc_adcs_sp_12_packet);
			canSendPacket(&rc_adcs_sp_12_packet);
		}
		if(rcFlag == 75){
			CANPacket rc_adcs_sp_11_packet = {0};
			rc_adcs_sp_11 rc_adcs_sp_11_info = {0};
			encoderc_adcs_sp_11(&rc_adcs_sp_11_info, &rc_adcs_sp_11_packet);
			canSendPacket(&rc_adcs_sp_11_packet);
		}
		if(rcFlag == 76){
			CANPacket rc_adcs_sp_7_packet = {0};
			rc_adcs_sp_7 rc_adcs_sp_7_info = {0};
			encoderc_adcs_sp_7(&rc_adcs_sp_7_info, &rc_adcs_sp_7_packet);
			canSendPacket(&rc_adcs_sp_7_packet);
		}
		if(rcFlag == 77){
			CANPacket rc_adcs_sp_6_packet = {0};
			rc_adcs_sp_6 rc_adcs_sp_6_info = {0};
			encoderc_adcs_sp_6(&rc_adcs_sp_6_info, &rc_adcs_sp_6_packet);
			canSendPacket(&rc_adcs_sp_6_packet);
		}
		if(rcFlag == 78){
			CANPacket rc_adcs_sp_17_packet = {0};
			rc_adcs_sp_17 rc_adcs_sp_17_info = {0};
			encoderc_adcs_sp_17(&rc_adcs_sp_17_info, &rc_adcs_sp_17_packet);
			canSendPacket(&rc_adcs_sp_17_packet);
		}
		if(rcFlag == 79){
			CANPacket rc_adcs_sp_16_packet = {0};
			rc_adcs_sp_16 rc_adcs_sp_16_info = {0};
			encoderc_adcs_sp_16(&rc_adcs_sp_16_info, &rc_adcs_sp_16_packet);
			canSendPacket(&rc_adcs_sp_16_packet);
		}
		if(rcFlag == 80){
			CANPacket rc_adcs_sp_10_packet = {0};
			rc_adcs_sp_10 rc_adcs_sp_10_info = {0};
			encoderc_adcs_sp_10(&rc_adcs_sp_10_info, &rc_adcs_sp_10_packet);
			canSendPacket(&rc_adcs_sp_10_packet);
		}
		if(rcFlag == 81){
			CANPacket rc_adcs_sp_8_packet = {0};
			rc_adcs_sp_8 rc_adcs_sp_8_info = {0};
			encoderc_adcs_sp_8(&rc_adcs_sp_8_info, &rc_adcs_sp_8_packet);
			canSendPacket(&rc_adcs_sp_8_packet);
		}
		if(rcFlag == 82){
			CANPacket rc_adcs_sp_9_packet = {0};
			rc_adcs_sp_9 rc_adcs_sp_9_info = {0};
			encoderc_adcs_sp_9(&rc_adcs_sp_9_info, &rc_adcs_sp_9_packet);
			canSendPacket(&rc_adcs_sp_9_packet);
		}
		if(rcFlag == 83){
			CANPacket rc_adcs_sp_13_packet = {0};
			rc_adcs_sp_13 rc_adcs_sp_13_info = {0};
			encoderc_adcs_sp_13(&rc_adcs_sp_13_info, &rc_adcs_sp_13_packet);
			canSendPacket(&rc_adcs_sp_13_packet);
		}
		if(rcFlag == 84){
			CANPacket rc_adcs_sp_15_packet = {0};
			rc_adcs_sp_15 rc_adcs_sp_15_info = {0};
			encoderc_adcs_sp_15(&rc_adcs_sp_15_info, &rc_adcs_sp_15_packet);
			canSendPacket(&rc_adcs_sp_15_packet);
		}
		if(rcFlag == 85){
			CANPacket rc_adcs_sp_14_packet = {0};
			rc_adcs_sp_14 rc_adcs_sp_14_info = {0};
			encoderc_adcs_sp_14(&rc_adcs_sp_14_info, &rc_adcs_sp_14_packet);
			canSendPacket(&rc_adcs_sp_14_packet);
		}
		if(rcFlag == 86){
			CANPacket rc_adcs_sp_5_packet = {0};
			rc_adcs_sp_5 rc_adcs_sp_5_info = {0};
			encoderc_adcs_sp_5(&rc_adcs_sp_5_info, &rc_adcs_sp_5_packet);
			canSendPacket(&rc_adcs_sp_5_packet);
		}
		if(rcFlag == 87){
			CANPacket rc_adcs_sp_4_packet = {0};
			rc_adcs_sp_4 rc_adcs_sp_4_info = {0};
			encoderc_adcs_sp_4(&rc_adcs_sp_4_info, &rc_adcs_sp_4_packet);
			canSendPacket(&rc_adcs_sp_4_packet);
		}
		if(rcFlag == 88){
			CANPacket rc_adcs_sp_3_packet = {0};
			rc_adcs_sp_3 rc_adcs_sp_3_info = {0};
			encoderc_adcs_sp_3(&rc_adcs_sp_3_info, &rc_adcs_sp_3_packet);
			canSendPacket(&rc_adcs_sp_3_packet);
		}
		if(rcFlag == 89){
			CANPacket rc_adcs_sp_2_packet = {0};
			rc_adcs_sp_2 rc_adcs_sp_2_info = {0};
			encoderc_adcs_sp_2(&rc_adcs_sp_2_info, &rc_adcs_sp_2_packet);
			canSendPacket(&rc_adcs_sp_2_packet);
		}
		if(rcFlag == 90){
			CANPacket rc_adcs_sp_1_packet = {0};
			rc_adcs_sp_1 rc_adcs_sp_1_info = {0};
			encoderc_adcs_sp_1(&rc_adcs_sp_1_info, &rc_adcs_sp_1_packet);
			canSendPacket(&rc_adcs_sp_1_packet);
		}
		if(rcFlag == 91){
			CANPacket rc_adcs_bdot_4_packet = {0};
			rc_adcs_bdot_4 rc_adcs_bdot_4_info = {0};
			encoderc_adcs_bdot_4(&rc_adcs_bdot_4_info, &rc_adcs_bdot_4_packet);
			canSendPacket(&rc_adcs_bdot_4_packet);
		}
		if(rcFlag == 92){
			CANPacket rc_adcs_bdot_3_packet = {0};
			rc_adcs_bdot_3 rc_adcs_bdot_3_info = {0};
			encoderc_adcs_bdot_3(&rc_adcs_bdot_3_info, &rc_adcs_bdot_3_packet);
			canSendPacket(&rc_adcs_bdot_3_packet);
		}
		if(rcFlag == 93){
			CANPacket rc_adcs_bdot_2_packet = {0};
			rc_adcs_bdot_2 rc_adcs_bdot_2_info = {0};
			encoderc_adcs_bdot_2(&rc_adcs_bdot_2_info, &rc_adcs_bdot_2_packet);
			canSendPacket(&rc_adcs_bdot_2_packet);
		}
		if(rcFlag == 94){
			CANPacket rc_adcs_bdot_1_packet = {0};
			rc_adcs_bdot_1 rc_adcs_bdot_1_info = {0};
			encoderc_adcs_bdot_1(&rc_adcs_bdot_1_info, &rc_adcs_bdot_1_packet);
			canSendPacket(&rc_adcs_bdot_1_packet);
		}
		if(rcFlag == 95){
			CANPacket rc_adcs_mtq_5_packet = {0};
			rc_adcs_mtq_5 rc_adcs_mtq_5_info = {0};
			encoderc_adcs_mtq_5(&rc_adcs_mtq_5_info, &rc_adcs_mtq_5_packet);
			canSendPacket(&rc_adcs_mtq_5_packet);
		}
		if(rcFlag == 96){
			CANPacket rc_adcs_mtq_4_packet = {0};
			rc_adcs_mtq_4 rc_adcs_mtq_4_info = {0};
			encoderc_adcs_mtq_4(&rc_adcs_mtq_4_info, &rc_adcs_mtq_4_packet);
			canSendPacket(&rc_adcs_mtq_4_packet);
		}
		if(rcFlag == 97){
			CANPacket rc_adcs_mtq_3_packet = {0};
			rc_adcs_mtq_3 rc_adcs_mtq_3_info = {0};
			encoderc_adcs_mtq_3(&rc_adcs_mtq_3_info, &rc_adcs_mtq_3_packet);
			canSendPacket(&rc_adcs_mtq_3_packet);
		}
		if(rcFlag == 98){
			CANPacket rc_adcs_mtq_2_packet = {0};
			rc_adcs_mtq_2 rc_adcs_mtq_2_info = {0};
			encoderc_adcs_mtq_2(&rc_adcs_mtq_2_info, &rc_adcs_mtq_2_packet);
			canSendPacket(&rc_adcs_mtq_2_packet);
		}
		if(rcFlag == 99){
			CANPacket rc_ppt_2_packet = {0};
			rc_ppt_2 rc_ppt_2_info = {0};
			encoderc_ppt_2(&rc_ppt_2_info, &rc_ppt_2_packet);
			canSendPacket(&rc_ppt_2_packet);
		}
		if(rcFlag == 100){
			CANPacket rc_ppt_1_packet = {0};
			rc_ppt_1 rc_ppt_1_info = {0};
			encoderc_ppt_1(&rc_ppt_1_info, &rc_ppt_1_packet);
			canSendPacket(&rc_ppt_1_packet);
		}
		if(rcFlag == 101){
			CANPacket rc_eps_gen_9_packet = {0};
			rc_eps_gen_9 rc_eps_gen_9_info = {0};
			encoderc_eps_gen_9(&rc_eps_gen_9_info, &rc_eps_gen_9_packet);
			canSendPacket(&rc_eps_gen_9_packet);
		}
		if(rcFlag == 102){
			CANPacket rc_eps_gen_8_packet = {0};
			rc_eps_gen_8 rc_eps_gen_8_info = {0};
			encoderc_eps_gen_8(&rc_eps_gen_8_info, &rc_eps_gen_8_packet);
			canSendPacket(&rc_eps_gen_8_packet);
		}
		if(rcFlag == 103){
			CANPacket rc_eps_gen_7_packet = {0};
			rc_eps_gen_7 rc_eps_gen_7_info = {0};
			encoderc_eps_gen_7(&rc_eps_gen_7_info, &rc_eps_gen_7_packet);
			canSendPacket(&rc_eps_gen_7_packet);
		}
		if(rcFlag == 104){
			CANPacket rc_eps_gen_6_packet = {0};
			rc_eps_gen_6 rc_eps_gen_6_info = {0};
			encoderc_eps_gen_6(&rc_eps_gen_6_info, &rc_eps_gen_6_packet);
			canSendPacket(&rc_eps_gen_6_packet);
		}
		if(rcFlag == 105){
			CANPacket rc_eps_gen_5_packet = {0};
			rc_eps_gen_5 rc_eps_gen_5_info = {0};
			encoderc_eps_gen_5(&rc_eps_gen_5_info, &rc_eps_gen_5_packet);
			canSendPacket(&rc_eps_gen_5_packet);
		}
		if(rcFlag == 106){
			CANPacket rc_eps_gen_4_packet = {0};
			rc_eps_gen_4 rc_eps_gen_4_info = {0};
			encoderc_eps_gen_4(&rc_eps_gen_4_info, &rc_eps_gen_4_packet);
			canSendPacket(&rc_eps_gen_4_packet);
		}
		if(rcFlag == 107){
			CANPacket rc_eps_gen_3_packet = {0};
			rc_eps_gen_3 rc_eps_gen_3_info = {0};
			encoderc_eps_gen_3(&rc_eps_gen_3_info, &rc_eps_gen_3_packet);
			canSendPacket(&rc_eps_gen_3_packet);
		}
		if(rcFlag == 108){
			CANPacket rc_eps_gen_2_packet = {0};
			rc_eps_gen_2 rc_eps_gen_2_info = {0};
			encoderc_eps_gen_2(&rc_eps_gen_2_info, &rc_eps_gen_2_packet);
			canSendPacket(&rc_eps_gen_2_packet);
		}
		if(rcFlag == 109){
			CANPacket rc_eps_gen_1_packet = {0};
			rc_eps_gen_1 rc_eps_gen_1_info = {0};
			encoderc_eps_gen_1(&rc_eps_gen_1_info, &rc_eps_gen_1_packet);
			canSendPacket(&rc_eps_gen_1_packet);
		}
		if(rcFlag == 110){
			CANPacket rc_eps_batt_6_packet = {0};
			rc_eps_batt_6 rc_eps_batt_6_info = {0};
			encoderc_eps_batt_6(&rc_eps_batt_6_info, &rc_eps_batt_6_packet);
			canSendPacket(&rc_eps_batt_6_packet);
		}
		if(rcFlag == 111){
			CANPacket rc_eps_batt_5_packet = {0};
			rc_eps_batt_5 rc_eps_batt_5_info = {0};
			encoderc_eps_batt_5(&rc_eps_batt_5_info, &rc_eps_batt_5_packet);
			canSendPacket(&rc_eps_batt_5_packet);
		}
		if(rcFlag == 112){
			CANPacket rc_eps_batt_4_packet = {0};
			rc_eps_batt_4 rc_eps_batt_4_info = {0};
			encoderc_eps_batt_4(&rc_eps_batt_4_info, &rc_eps_batt_4_packet);
			canSendPacket(&rc_eps_batt_4_packet);
		}
		if(rcFlag == 113){
			CANPacket rc_eps_batt_3_packet = {0};
			rc_eps_batt_3 rc_eps_batt_3_info = {0};
			encoderc_eps_batt_3(&rc_eps_batt_3_info, &rc_eps_batt_3_packet);
			canSendPacket(&rc_eps_batt_3_packet);
		}
		if(rcFlag == 114){
			CANPacket rc_eps_batt_2_packet = {0};
			rc_eps_batt_2 rc_eps_batt_2_info = {0};
			encoderc_eps_batt_2(&rc_eps_batt_2_info, &rc_eps_batt_2_packet);
			canSendPacket(&rc_eps_batt_2_packet);
		}
		if(rcFlag == 115){
			CANPacket rc_eps_batt_1_packet = {0};
			rc_eps_batt_1 rc_eps_batt_1_info = {0};
			encoderc_eps_batt_1(&rc_eps_batt_1_info, &rc_eps_batt_1_packet);
			canSendPacket(&rc_eps_batt_1_packet);
		}
	rcFlag--;
	}} 
while (1) { 
	canBlast(); 
	int i=0; 
	for(int i=0;i<100000;i++){} 
}
