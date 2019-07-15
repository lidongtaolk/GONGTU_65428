/*****************************************************
*@file CSB.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

uint16 CSB_Time = 0;

void CSB_Init(){
  gpio_init(CSB_Trig,GPO,0);
  gpio_init(CSB_Echo,GPI,0);
 
  port_init_NoALT(CSB_Echo,IRQ_RISING);
}

void CSB_Send(){
  gpio_set(CSB_Trig,1);
  DELAY_US(CSB_TRIG_TIME);
  gpio_set(CSB_Trig,0);
}

void CSB_IRQHandler(float *distance){
  lptmr_time_start_us();
  while(gpio_get(CSB_Echo));
  uint32 time = lptmr_time_get_us(); //单位是一个微秒 //若是启用pit计时则单位是一个bus周期
  if(time != ~0){   //未溢出
    //time = time*1000/bus_clk_khz; //单位为微秒；
    *distance = 0.034*time/2;
    printf("距离为：%f cm\n",*distance);
  }else{
    *distance = 0xFFFF;
  }
  //return distance;
}