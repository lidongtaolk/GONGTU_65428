/*****************************************************
*@file SpeedControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SPEED_H__
#define __SPEED_H__

#include "common.h"

/****变量声明****/
extern int32 MotorPWM;
extern int Stop_Count; //计数使停车的可能更精确

extern int GetSpeed[3];
extern int Speed_Error[3];
extern int Setted_Speed; //速度设定值

extern double Speed_Kp,Speed_Ki,Speed_Kd;              //速度pid系数



/****函数声明****/
extern void Motor_Init(void);
extern void PWM_Out(void);
extern void Speed_StopCar(void);     //停车
//extern void Speed_Control(void);
extern void Speed_Control(void);
extern void Speed_Get_Data(void);
extern void Speed_Data_Save(void);

#endif