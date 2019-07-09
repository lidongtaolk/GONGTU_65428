/*****************************************************
*@file DirectionControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "common.h"

/****外部全局变量****/
extern double Direction_P;       //比例系数
extern double Direction_D;       //微分系数
extern double DirectionErr[2][5];   //误差
extern double DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern int16 sensor_p[5];         //传感器
extern int16 sensor_v[3];



/****舵机控制****/
extern int Servo_PWM_now,Servo_PWM_old;

/****函数声明****/
extern void Direction_Data_Init(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif