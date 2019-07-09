/*****************************************************
*@file ADC.h
*
*@brief         adc管脚初始化以及adc值获取
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __ADC_H__
#define __ADC_H__

//ADC端口枚举
typedef enum
{
  
  ADP1,       //一字和八字
  ADP2,
  ADP3,
  ADPR,
  ADP4,
  ADP5,
  ADP6,
  
  ADV1,       //竖直
  ADVR,
  ADV2,
  
  AD_MAX,
    
}ADC_e;

/****ad变量****/
extern uint16 ad_val[10][6];//电感值   ad_val[i][0]用存放哨站以及中值滤波后的结果

/****adc初始化函数****/
extern void My_adc_init(void);
extern void My_adc_once(ADC_nbit bit);


#endif