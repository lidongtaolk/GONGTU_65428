/*****************************************************
*@file DirectionControl.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "common.h"
#define SENSOR_SIZE     5       //�����������ʷֵ�ĳ���

/****�ⲿȫ�ֱ���****/
extern float Direction_P;       //����ϵ��
extern float Direction_D;       //΢��ϵ��
extern float DirectionErr[2][SENSOR_SIZE];   //���
extern float DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern int16 sensor[10][5];         //������



/****�������****/
extern int Servo_PWM_now,Servo_PWM_old;

/****��������****/
extern void Direction_Data_Update(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif