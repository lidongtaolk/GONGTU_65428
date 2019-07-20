/*****************************************************
*@file Servo.h
*
*@core   K60Fx
*@date   2019
******************************************************/
#ifndef __SERVO_H__
#define __SERVO_H__
#define SERVO_CENTER    6730//7500//2575        //2520  200hz ¾«¶È10000
#define SERVO_MAX       7700//10000//2950        //2920
#define SERVO_MIN       5600//5000//2250        //2120
#define SERVO_FREQ      50


extern void Servo_Init();
extern void ServoPWM(uint32 PWM);
extern void Servo_STOP();
extern void Servo_MAX_Change();



#endif