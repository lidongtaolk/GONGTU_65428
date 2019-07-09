/*****************************************************
*@file ADC.h
*
*@brief         adc�ܽų�ʼ���Լ�adcֵ��ȡ
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __ADC_H__
#define __ADC_H__

//ADC�˿�ö��
typedef enum
{
  
  ADP1,       //һ�ֺͰ���
  ADP2,
  ADP3,
  ADPR,
  ADP4,
  ADP5,
  ADP6,
  
  ADV1,       //��ֱ
  ADVR,
  ADV2,
  
  AD_MAX,
    
}ADC_e;

/****ad����****/
extern uint16 ad_val[10][6];//���ֵ   ad_val[i][0]�ô����վ�Լ���ֵ�˲���Ľ��

/****adc��ʼ������****/
extern void My_adc_init(void);
extern void My_adc_once(ADC_nbit bit);


#endif