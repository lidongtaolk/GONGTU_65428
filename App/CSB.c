/*****************************************************
*@file CSB.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

float OLD_CSB_DISTANCE1 = 0,OLD_CSB_DISTANCE2 = 0;
float CSB_DISTANCE1 = 0,CSB_DISTANCE2 = 0;
float CSB_ERR[CSB_NUM] = {0};
uint8 CSB_HOLD =  0;

CSB_e CSB_FLAG = CSB1;

void CSB_DataUpdate(){
  for(uint8 i = CSB_NUM-1;i>0;i--){
    CSB_ERR[i] = CSB_ERR[i-1];  //数据后移
  }
  CSB_ERR[0] = CSB_DISTANCE2 - CSB_DISTANCE1;
}

void CSB_Control(){
   //static uint8 CSB_TIME = 0;
   
}

void CSB_Init1(){
  gpio_init(CSB_Trig,GPO,0);
  gpio_init(CSB_Echo,GPI,0);
 
  port_init_NoALT(CSB_Echo,IRQ_RISING);
}

void CSB_Init2(){
  gpio_init(CSB_Trig2,GPO,0);
  gpio_init(CSB_Echo2,GPI,0);
 
  port_init_NoALT(CSB_Echo2,IRQ_RISING);
}

void CSB_Send1(){
  gpio_set(CSB_Trig,1);
  DELAY_US(CSB_TRIG_TIME);
  gpio_set(CSB_Trig,0);
}

void CSB_Send2(){
  gpio_set(CSB_Trig2,1);
  DELAY_US(CSB_TRIG_TIME);
  gpio_set(CSB_Trig2,0);
}

void CSB_IRQHandler1(float *distance){
  lptmr_time_start_us();
  uint32 i = 0;
  while(gpio_get(CSB_Echo)){
     if(i==25000)break;
     i++;
  }
  uint32 time = lptmr_time_get_us(); //单位是一个微秒 //若是启用pit计时则单位是一个bus周期
  if(time != ~0){   //未溢出
    //time = time*1000/bus_clk_khz; //单位为微秒；
    *distance = 0.034*time/2;
    //printf("距离为：%f cm\n",*distance);
  }else{
    *distance = 0xFFFF;
  }
  //return distance;
}

void CSB_IRQHandler2(float *distance){
  lptmr_time_start_us();
  uint32 i = 0;
  while(gpio_get(CSB_Echo2)){
     if(i==25000)break;
     i++;
  }
  uint32 time = lptmr_time_get_us(); //单位是一个微秒 //若是启用pit计时则单位是一个bus周期
  if(time != ~0){   //未溢出
    //time = time*1000/bus_clk_khz; //单位为微秒；
    *distance = 0.034*time/2;
    //printf("距离为：%f cm\n",*distance);
  }else{
    *distance = 0xFFFF;
  }
  //return distance;
}