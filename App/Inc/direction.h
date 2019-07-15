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
#define SENSOR_SIZE     10       //�����������ʷֵ�ĳ���

/****�ⲿȫ�ֱ���****/
extern float Direction_P;       //����ϵ��
extern float Direction_D;       //΢��ϵ��
extern float Direction_PZJ;
extern float Direction_DZJ;
extern float Direction_PHD;
extern float Direction_DHD;
extern float DirectionErr[(AD_MAX>>1)-1][SENSOR_SIZE];   //���
extern float DirectionErr_dot[8];
//extern double pre_DirectionErr[2];
extern uint16 sensor[10][SENSOR_SIZE];         //������



/****�������****/
extern uint32 Servo_PWM_now,Servo_PWM_old;

/****��������****/
extern void Direction_Data_Update(void);
extern void Direction_Control(void);
extern void Read_ADC(void);

#endif