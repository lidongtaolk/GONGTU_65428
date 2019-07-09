/*****************************************************
*@file function.c
*
*@core   K60FX
*@date   2019
******************************************************/

#include "include.h"
extern uint8 var[3];
extern uint8 Encode_Switch_Value;
extern uint16 ad_p[7][6];//一字电感   ad_p[i][0]用存放哨站以及中值滤波后的结果
extern uint16 ad_v[3][6];//竖直电感   ad_v[i][0]用存放哨站以及中值滤波后的结果
extern int PITx_Flag = 1;
uint32 distance = 0;
void PIT0_IRQHandler(void){
  key_IRQHandler();     //定时扫描按键状态
  menu_update();      //菜单更新
  
  //var[0]=key_get(KEY_A);//MotorPWM;
  //var[1]=Encode_Switch();//ad_p[0][3];//Bmq_Get();
  //SWJ_sendData(var,sizeof(var));
  /*Speed_Get_Data();
  Speed_Control();
  Speed_Data_Save();*/
  //My_adc_once();
 /* Direction_Data_Init();
  Direction_Control();
  Speed_Get_Data();
  Speed_Control();
  Speed_Data_Save();
  PWM_Out();*/
  /*printf("水平电感值：%d,%d,%d,%d,%d\n",sensor_p[1],sensor_p[2],sensor_p[0],sensor_p[3],sensor_p[4]);
  printf("垂直电感值：%d,%d,%d\n",sensor_v[1],sensor_v[0],sensor_v[2]);
  printf("差比和结果：%f\n",DirectionErr[0][0]);*/
  My_adc_once(ADC_8bit);
  printf("水平电感值：%d,%d,%d,%d,%d,%d,%d\n",ad_val[0][3],ad_val[1][3],ad_val[2][3],ad_val[3][3],ad_val[4][3],ad_val[5][3],ad_val[6][3]);
  printf("垂直电感值：%d,%d,%d\n",ad_val[7][3],ad_val[8][3],ad_val[9][3]);
  //printf("水平电感值：%d,%d,%d,%d,%d,%d,%d\n",adc_once(AD_1_P,ADC_8bit),adc_once(AD_2_P,ADC_8bit),adc_once(AD_3_P,ADC_8bit),var[1]=adc_once(AD_R_P,ADC_8bit),adc_once(AD_4_P,ADC_8bit),adc_once(AD_5_P,ADC_8bit),adc_once(AD_6_P,ADC_8bit));
  //printf("垂直电感值：%d,%d,%d\n",adc_once(AD_1_V,AD C_8bit),adc_once(AD_R_V,ADC_8bit),adc_once(AD_2_V,ADC_8bit));
  //printf("PIT0 start\n");
  lcd_showuint8(0,5,ad_val[3][3]);
  lcd_showuint8(60,5,ad_val[8][3]);
  lcd_changestyle(GREEN,WHITE);
  lcd_showuint8(0,6,key_check(KEY_A));
  lcd_showuint8(0,7,key_check(KEY_B));
  lcd_changestyle(RED,WHITE);
  lcd_showuint8(0,8,Encode_Switch());
  lcd_showuint16(0,9,KEY_ALL);
  lcd_showuint16(60,9,KEY_ALL_HOLD);
  /*if(PITx_Flag){
    ServoPWM(700);
    PITx_Flag^=1;
    printf("PWM1\n");
  }else{
    ServoPWM(200);
    PITx_Flag^=1;
    
    printf("PWM3\n");
  }*/
  PIT_Flag_Clear(PIT0);
}
void PIT1_IRQHandler(void){
  
        //pit_time_start(PIT2);
        printf("进入超声波中断\n");
        gpio_set(CSB_Trig,1);
        DELAY_US(10);
        gpio_set(CSB_Trig,0);
        while(gpio_get(CSB_Echo)==0);
        pit_time_start(PIT2);
        while(gpio_get(CSB_Echo));
        uint32 time = pit_time_get(PIT2); //单位是一个bus周期
        if(time != ~0){   //未溢出
            time = time*1000/bus_clk_khz; //单位为微秒；
            distance = 0.34*time/20;
            printf("距离为：%f cm\n",distance);
        }
        //uint32 T=pit_time_get(PIT3);
        //T = time*1000/bus_clk_khz;
        //printf("采样计时时间为：%d us\n",T);
        pit_close(PIT2);
        //pit_close(PIT3);
  
  PIT_Flag_Clear(PIT1);
}
void PORTA_IRQHandler(void){
  if(PORTA_ISFR&(1<<6)){        
    uint8 *c = "start";
    lcd_showstr(0,1,c);
    if(key_check(KEY_A) == KEY_DOWN){
        Beep_On();      //进入DEBUG模式；
        DELAY_MS(10);
        Beep_Off();
    }
    PORTA_ISFR = (1<<6);
  }
}
  