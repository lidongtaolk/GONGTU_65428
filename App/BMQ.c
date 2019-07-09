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


int16 Bmq_Get(void){
    int16 val=0;
    val = ftm_quad_get(FTM2);
    ftm_quad_clean(FTM2);
    return val;
}