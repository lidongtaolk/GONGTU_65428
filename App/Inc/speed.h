/*****************************************************
*@file SpeedControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SPEED_H__
#define __SPEED_H__

#include "common.h"

/****��������****/
extern int32 MotorPWM;
extern int Stop_Count; //����ʹͣ���Ŀ��ܸ���ȷ

extern int GetSpeed[3];
extern int Speed_Error[3];
extern int Setted_Speed; //�ٶ��趨ֵ

extern double Speed_Kp,Speed_Ki,Speed_Kd;              //�ٶ�pidϵ��



/****��������****/
extern void Motor_Init(void);
extern void PWM_Out(void);
extern void Speed_StopCar(void);     //ͣ��
//extern void Speed_Control(void);
extern void Speed_Control(void);
extern void Speed_Get_Data(void);
extern void Speed_Data_Save(void);

#endif