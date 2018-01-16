/*
 * LIN_signals.h
 *
 *  Created on: Mar 22, 2017
 *      Author: emc2
 */

#ifndef LIN_SIGNALS_H_
#define LIN_SIGNALS_H_

#include "sys/types.h"



typedef struct SM1_to_HMIIOM_lin_frame
{
	uint8_t DiagInfoSM1 : 7;
	uint8_t ResponseErrorSM1 : 1;
	uint8_t LIN_BrakeProgramButtonStatus :1;
	uint8_t LIN_DirInd_StalkStatus_1 : 3;
	uint8_t LIN_MainBeam_StalkStatus_1 : 2;
	uint8_t LIN_Rainsensor_ButtonStatus : 1;
	uint8_t LIN_Washing_Status_1 : 1;
	uint8_t LIN_RetarderStalkPosition_1 :3;
	uint8_t LIN_TrailerBrakeInputStatus : 3;
	uint8_t LIN_WiperAdjustStatus_1 :2;
	uint8_t LIN_WiperStalkStat_1 : 3;

}lindata_SM1_HMIIOM;



typedef struct Lin_Frame_Comp_to_Rains
{
	int16_t LIN_WheelBasedVehicleSpdLowRes : 8;
	int16_t LIN_RS_Activation_rqst : 2;
	int16_t LIN_RS_Sensitivity_rqst: 4;
	int16_t LIN_RS_WasherStatus: 2;
	int16_t LIN_RS_WiperParkStatus: 2;
	int16_t LIN_RS_Mode_rqst: 2;
	int16_t LIN_RS_VehicleType: 4;
	int16_t LIN_RS_WindScreenType :4;

}lindata_to_CRains;



typedef struct Lin_Frame_Rain_to_Comp
{
	int16_t DiagInfoRAINS: 7;
	int16_t ResponseErrorRAINS : 1;
	int16_t LIN_RS_Wiping_rqst  : 3;
	int16_t LIN_AutoHeadlightSwitch_rqst :2;
	int16_t LIN_RS_Mode: 2;
	int16_t LIN_AmbientLight_stat : 16;

}lindata_to_RComp;


extern lindata_SM1_HMIIOM hmi_input;
extern lindata_to_RComp rains_to_comp;
extern lindata_to_CRains comp_to_rains;

#endif /* LIN_SIGNALS_H_ */
