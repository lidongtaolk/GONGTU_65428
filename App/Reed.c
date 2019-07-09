/*****************************************************
*@file ADC.h
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

void Reed_Init(){
    gpio_init(Reed_port,GPI,1);
}

void Reed_Detect(){
    if(gpio_get(Reed_port)==0){
        Stop_Flag = 2; //�յ�ͣ����־
        Beep_Flag = 1;
    }
}