/*****************************************************
*@file Servo.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"
int Servo_Center = 0; //待调整
int Servo_MAX = 80;
int Servo_MIN = 52;


void Servo_Init(){
    Servo_Center = 450;//74; //舵机摆到正中间
    ftm_pwm_init(FTM0,Servo_ftm0_pwm,300,Servo_Center);
} 

void ServoPWM(int PWM){
    ftm_pwm_duty(FTM0,Servo_ftm0_pwm,PWM);
}

void Servo_STOP(){
    ftm_pwm_duty(FTM0,Servo_ftm0_pwm,Servo_Center);
}