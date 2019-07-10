/*****************************************************
*@file DirectionControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "common.h"
#define SENSOR_SIZE     5       //传感器存放历史值的长度

/****外部全局变量****/
extern float Direction_P;       //比例系数
extern float Direction_D;       //微分系数
extern float DirectionErr[2][SENSOR_SIZE];   //误差
extern float DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern int16 sensor[10][5];         //传感器



/****舵机控制****/
extern int Servo_PWM_now,Servo_PWM_old;

/****函数声明****/
extern void Direction_Data_Update(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif