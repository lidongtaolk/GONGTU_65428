/*****************************************************
*@file Servo.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SERVO_H__
#define __SERVO_H__
#define SERVO_CENTER    2575        //2520  200hz ¾«¶È10000
#define SERVO_MAX       2950        //2920
#define SERVO_MIN       2250        //2120

extern void Servo_Init();
extern void ServoPWM(uint32 PWM);
extern void Servo_STOP();
extern void Servo_MAX_Change();



#endif