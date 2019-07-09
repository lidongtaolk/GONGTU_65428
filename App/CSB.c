/*****************************************************
*@file CSB.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

uint16 CSB_Time = 0;

void CSB_Init(){
  pit_init_ms(CSB_pit,1);
  gpio_init(CSB_Trig,GPO,0);
  gpio_init(CSB_Echo,GPI,0);
  
}
void CSB_Send(){
  gpio_set(CSB_Trig,1);
  DELAY_US(20);
  gpio_set(CSB_Trig,0);
}

uint16 CSB_Get(){
  return 0;
}