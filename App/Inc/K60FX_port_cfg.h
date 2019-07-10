/*****************************************************
*@file K60FX_port_cfg.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __K60FX_PORT_CFG_H__
#define __K60FX_PORT_CFG_H__


//-------------Basic Define---------------------------
#define ON      1
#define OFF     0

//------------�������ܽ�------------------------------
#define Coder_dir_left          PTB18
#define Coder_dir_right         PTB19
//------------������----------------------------------
#define Beep_port               PTB17

//------------ADC-------------------------------------
#define AD_1_P             ADC1_DM0//ADC0_DP0//ADC1_DM0
#define AD_2_P             ADC0_DM0
#define AD_3_P             ADC0_DP0//ADC1_DP0//ADC0_DM0
#define AD_4_P             ADC0_DM1//ADC0_DM1
#define AD_5_P             ADC0_DP1//ADC1_DM1
#define AD_6_P             ADC1_SE5a  
#define AD_1_V             ADC1_DP0
#define AD_2_V             ADC1_SE4a//ADC1_DP1     //ADC1_DP1
#define AD_R_P             ADC1_DM1//ADC1_DM0//ADC0_DP0     //һ�ֲο����
#define AD_R_V             ADC1_DP1//ADC0_DP1     //��ֱ�ο����

#define AD(n,X)            AD_##n##_##X
//------------������--------------------------------
#define CSB_Trig           PTB9
#define CSB_Echo           PTB8
#define CSB_pit            PIT1


//-----------IRQ-------------------------------------//�Ƿ��ȥ

//-----------�����--------------------------------
#define NaviLeft           PTD11
#define NaviRight          PTD14
#define NaviUp             PTD10//PTD13
#define NaviDown           PTD13//PTD10
#define NaviOK             PTD12
//-------------BlueTooth-----------------------------
#define My_uart_port       UART0

//-----------Motor-----------------------------------//
#define Motor_ftm3_pwm1              FTM_CH0       //�����ź�
#define Motor_ftm3_pwm2              FTM_CH1
#define Motor_ftm3_pwm3              FTM_CH2
#define Motor_ftm3_pwm4              FTM_CH3
#define Motor_EN                     PTD7               //���ʹ���źţ�ir2104sʹ�ܣ�

//----------���-------------------------------------
#define Servo_ftm0_pwm                    FTM_CH2

//---------�ɻɹ�------------------------------------
#define Reed_port                  PTA16

#endif
