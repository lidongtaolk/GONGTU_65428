/*****************************************************
*@file DirectionControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "common.h"

/****�ⲿȫ�ֱ���****/
extern double Direction_P;       //����ϵ��
extern double Direction_D;       //΢��ϵ��
extern double DirectionErr[2][5];   //���
extern double DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern int16 sensor_p[5];         //������
extern int16 sensor_v[3];



/****�������****/
extern int Servo_PWM_now,Servo_PWM_old;

/****��������****/
extern void Direction_Data_Init(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif