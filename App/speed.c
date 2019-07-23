/*****************************************************
*@file SpeedControl.h
*
*@core   K60FX
*@date   2019
******************************************************/

#include "include.h"

int32 MotorPWM = 0;
int32 old_MotorPWM = 0;
float Setted_Speed = 0;
uint8 Stop_Count = 0;
int16 GetSpeed[3]={0};
float Speed_Error[3]={0};
float COMMON_SPEED = 200,HD_SPEED = 150,ZJ_SPEED = 150,BZ_SPEED = 150,SZ_SPEED = 200;

float Speed_Kp = 2,Speed_Ki = 0.2,Speed_Kd = 0;              //�ٶ�pidϵ��
//float ud = 0;//΢������΢����

void Motor_Init(void){
    ftm_pwm_init(FTM3,Motor_ftm3_pwm3,14*1000,0);//pwm��ʼ��  Ƶ��Ϊ500HZ������Ϊ2us,�ߵ�ƽ����ʱ��Ϊ2us*duty*0.01
    ftm_pwm_init(FTM3,Motor_ftm3_pwm4,14*1000,0);
   
    gpio_init(Motor_EN,GPO,1);//ʹ�ܶ˳�ʼ��
}

void PWM_Out(){
    if(MotorPWM>=0){     //��ת
       ftm_pwm_duty(FTM3,Motor_ftm3_pwm3,MotorPWM);
       ftm_pwm_duty(FTM3,Motor_ftm3_pwm4,0);
    }
    else{//��ת
       ftm_pwm_duty(FTM3,Motor_ftm3_pwm3,0);
       ftm_pwm_duty(FTM3,Motor_ftm3_pwm4,-1*MotorPWM);
    }
}

void Speed_StopCar(void){
  if(GetSpeed[0]>10){
      MotorPWM = -100;
      PWM_Out();
  }
  else{
      Stop_Count++;
      if(Stop_Count>10){
          Stop_Flag = 2;        //�յ�ͣ��
          Stop_Count = 11;  
      }
      MotorPWM = 0;
      PWM_Out();
  }
}

void Speed_Get_Data(){
  
  GetSpeed[0] = Bmq_Get();
  //**************�޷�
  if(GetSpeed[0]<BMQ_MIN)GetSpeed[0] = BMQ_MIN;
  else if(GetSpeed[0]>BMQ_MAX)GetSpeed[0] = BMQ_MAX;
  //**************�˲�
  GetSpeed[0] = (GetSpeed[0]+GetSpeed[1]+GetSpeed[2])/3;
  //Speed_d[0] = GetSpeed[0]-GetSpeed[1];
}

void Speed_Data_Save(){
  for(int i=2;i>=1;i--){
    GetSpeed[i] = GetSpeed[i-1];
  }
  for(int i=2;i>=1;i--){
    Speed_Error[i] = Speed_Error[i-1];
  }
}

void Speed_Control(){
    Speed_Error[0] = Setted_Speed - GetSpeed[0]; 
    //һ��pid�㷨
    MotorPWM+= (Speed_Kp*(Speed_Error[0]-Speed_Error[1])+Speed_Ki*(Speed_Error[0])+Speed_Kd*(Speed_Error[2]-2*Speed_Error[1]+Speed_Error[0]));//����ʽ
    if(MotorPWM<PWM_MIN)MotorPWM = PWM_MIN;
    else if(MotorPWM>PWM_MAX)MotorPWM = PWM_MAX;
    
    //΢������
    /*MotorPWM+= (Speed_Kp*(Speed_Error[0]-Speed_Error[1]) + Speed_Ki*(Speed_Error[0]) - Speed_Kd*(GetSpeed[0]-2*GetSpeed[1]+GetSpeed[2]) - Speed_Kd*(GetSpeed[0]-GetSpeed[1])); //����ʽ
    if(MotorPWM<PWM_MIN)MotorPWM = PWM_MIN;
    else if(MotorPWM>PWM_MAX)MotorPWM = PWM_MAX;*/
    //����ȫ΢��
    return;
}