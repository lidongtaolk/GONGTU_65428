/*****************************************************
*@file SpeedControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SPEED_H__
#define __SPEED_H__

#include "common.h"

#define BMQ_MIN -2000
#define BMQ_MAX  2000
#define PWM_MAX 400
#define PWM_MIN 0


/****��������****/
extern int32 MotorPWM;
extern int32 old_MotorPWM;
extern uint8 Stop_Count; //����ʹͣ���Ŀ��ܸ���ȷ

extern int16 GetSpeed[3];
extern float Speed_Error[3];
extern float Setted_Speed; //�ٶ��趨ֵ

extern float Speed_Kp,Speed_Ki,Speed_Kd;              //�ٶ�pidϵ��

extern float COMMON_SPEED,HD_SPEED,ZJ_SPEED,BZ_SPEED,SZ_SPEED;

/****��������****/
extern void Motor_Init(void);
extern void PWM_Out(void);
extern void Speed_StopCar(void);     //ͣ��
//extern void Speed_Control(void);
extern void Speed_Control(void);
extern void Speed_Get_Data(void);
extern void Speed_Data_Save(void);

#endif