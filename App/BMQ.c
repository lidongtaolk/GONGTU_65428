/*****************************************************
*@file BMQ.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

void Bmq_Init(void){
    ftm_quad_init(FTM2);
}

int16 Bmq_Speed(float speed){
  float tem = (speed*1000/60)/(3.14*16.8)*500;
  int16 ret = tem;
  return ret;
}

float Bmq_Bmq(int16 bmq){
  float tem = bmq/500.0*3.14*16.8*60.0/1000.0;
  return tem;
}

int16 Bmq_Get(void){
    int16 val=0;
    val = ftm_quad_get(FTM2);
    ftm_quad_clean(FTM2);
    return val;
}