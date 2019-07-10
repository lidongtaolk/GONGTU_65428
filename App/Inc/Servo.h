/*****************************************************
*@file Servo.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SERVO_H__
#define __SERVO_H__
#define SERVO_CENTER    750
#define SERVO_MAX       1250
#define SERVO_MIN       250

extern void Servo_Init();
extern void ServoPWM(uint32 PWM);
extern void Servo_STOP();
extern void Servo_MAX_Change();



#endif