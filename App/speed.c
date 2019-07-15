/*****************************************************
*@file SpeedControl.h
*
*@core   K60FX
*@date   2019
******************************************************/

#include "include.h"

int32 MotorPWM = 0;
int Setted_Speed = 0;
int Stop_Count = 0;
int GetSpeed[3]={0};
int Speed_Error[3]={0};

double Speed_Kp = 0.2,Speed_Ki = 0.02,Speed_Kd = 0;              //�ٶ�pidϵ��


void Motor_Init(void){
    ftm_pwm_init(FTM3,Motor_ftm3_pwm3,500,0);//pwm��ʼ��  Ƶ��Ϊ500HZ������Ϊ2us,�ߵ�ƽ����ʱ��Ϊ2us*duty*0.01
    ftm_pwm_init(FTM3,Motor_ftm3_pwm4,500,0);
   
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
  Speed_Kp = 0.2;
  Speed_Ki = 0.02;
  Speed_Kd = 0;
  //**************�޷�
  if(GetSpeed[0]<0)GetSpeed[0] = 0;
  else if(GetSpeed[0]>250)GetSpeed[0] = 250;
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
    MotorPWM+= (Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2;
    //double d = (Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2;
   // if(((Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2)<1&&((Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2)>0)MotorPWM+=1;
    //else if(((Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2)>-1&&((Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2)<0)MotorPWM-=1;
    //else MotorPWM+=(Speed_Kp*Speed_Error[0]+Speed_Ki*(Speed_Error[0]+Speed_Error[1]+Speed_Error[2])+Speed_Kd*(Speed_Error[1]-Speed_Error[0]))*0.2;
    if(MotorPWM<23)MotorPWM = 23;
    else if(MotorPWM>100)MotorPWM=100;
  
    //΢������
    //����ȫ΢��
    return;
}