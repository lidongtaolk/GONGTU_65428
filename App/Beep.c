/*****************************************************
*@file Beep.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"


int Beep_Count = 0;

void Beep_Init(){
    gpio_init(Beep_port,GPO,0);
}

void Beep_On(){
    gpio_set(Beep_port,1);
}

void Beep_Off(){
    gpio_set(Beep_port,0);
}

void Beep_Work(){
  if(Beep_Flag == 1&&Beep_Count <= 100){
      Beep_Count++;
      Beep_On();
  }
  else {
      Beep_Count = 0;
      Beep_Flag = 0;
      Beep_Off();
  }
}