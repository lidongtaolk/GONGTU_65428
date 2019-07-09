/*****************************************************
*@file Servo.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SERVO_H__
#define __SERVO_H__

extern int Servo_Center; //´ýµ÷Õû
extern int Servo_MAX;
extern int Servo_MIN;

extern void Servo_Init();
extern void ServoPWM(int PWM);
extern void Servo_STOP();
extern void Servo_MAX_Change();



#endif