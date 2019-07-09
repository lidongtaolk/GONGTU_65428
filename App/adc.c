/*****************************************************
*@file ADC.h
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

//ADC管脚遍历
PTXn_e ADC_PTxn[AD_MAX] = {AD_1_P, AD_2_P, AD_3_P, AD_R_P, AD_4_P, AD_5_P, AD_6_P,
                                            AD_1_V, AD_R_V, AD_2_V
                            };

uint16 ad_val[10][6]={0};//一字电感   ad_p[i][0]用存放哨站以及中值滤波后的结果
/****adc处理函数****/
void My_adc_init(){
  for(int i = 0;i <AD_MAX; i++){
      adc_init(ADC_PTxn[i]);
  }
}

void My_adc_once(ADC_nbit bit){
    int j=0;
    for(int i=1; i <= 5; i++){  //五次采样
      
      for(int a = 0;a < AD_MAX; a++){
          ad_val[a][i] = adc_once(ADC_PTxn[a],bit);
          ad_val[a][0] = ad_val[a][i];//哨站
          j = i-1;
          for(; j>=1 && ad_val[a][j]>ad_val[a][0]; j--)ad_val[a][j+1] = ad_val[a][j];//插入排序
          ad_val[a][j+1] = ad_val[a][0];
      }
    }
}