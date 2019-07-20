/*****************************************************
*@file Servo.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

void Servo_Init(){
    //Servo_Center = 450;//74; //舵机摆到正中间
    ftm_pwm_init(FTM0,Servo_ftm0_pwm,SERVO_FREQ,SERVO_CENTER);
} 

void ServoPWM(uint32 PWM){
    ftm_pwm_duty(FTM0,Servo_ftm0_pwm,PWM);
}

void Servo_STOP(){
    ftm_pwm_duty(FTM0,Servo_ftm0_pwm,SERVO_CENTER);
}