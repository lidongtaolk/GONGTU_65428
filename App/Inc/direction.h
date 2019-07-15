/*****************************************************
*@file DirectionControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "common.h"
#include "adc.h"
#define SENSOR_SIZE     10       //传感器存放历史值的长度

/****外部全局变量****/
extern float Direction_P;       //比例系数
extern float Direction_D;       //微分系数
extern float Direction_PZJ;
extern float Direction_DZJ;
extern float Direction_PHD;
extern float Direction_DHD;
extern float DirectionErr[(AD_MAX>>1)-1][SENSOR_SIZE];   //误差
extern float DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern uint16 sensor[10][SENSOR_SIZE];         //传感器



/****舵机控制****/
extern uint32 Servo_PWM_now,Servo_PWM_old;

/****函数声明****/
extern void Direction_Data_Update(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif