/*****************************************************
*@file DirectionControl.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

/**********ȫ�ֱ�������********/ 
float Direction_P = 10;	//�������P  ���Pƫ����Ӧ��� �������ת�估ʱ���ǹ����˾�Ӧ����P
float Direction_D = 10;	//�������D
float Direction_PZJ = 30;
float Direction_DZJ = 10;
float Direction_PHD = 30;
float Direction_DHD = 10;
float DirectionErr[(AD_MAX>>1)-1][SENSOR_SIZE] = {0};	//����ƫ�g_fDirectionError[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ�               ������0����ͭ����࣡���� 
                                 //ǰ����Ϊһ�ֺͰ��ֵ�еĲ�Ⱥͣ��Ұ��Ŵ����ߵ��м� �����һ������ֱ�Ĳ�Ⱥ�
				//��g_fDirectionError[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ�
float DirectionErr_dot[8] = {0};//����ƫ��΢�֣�g_fDirectionError_dot[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ��΢�֣�
                           //��g_fDirectionError_dot[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ��΢��)

uint16 sensor[10][SENSOR_SIZE]={0};       //��ȡ�ĵ��ֵ


uint32 Servo_PWM_now = SERVO_CENTER,Servo_PWM_old = SERVO_CENTER;

/****�������****/
int Servo_PWM = 0,old_Servo_PWM = 0;

int HD_HOLD = 0;

int HD_FLAG = 0;

int ZJ_HOLD = 0;
/***�������ݳ�ʼ��***/
void Direction_Data_Update(void){

    for(int i=SENSOR_SIZE-1;i>=1;i--){                  //���ϴε�ֵ���ƴ洢
      for(int j = 1;j<=((AD_MAX>>1)-1);j++){    
        DirectionErr[j-1][i] = DirectionErr[j-1][i-1];
      }
    }
    
    for(int i = SENSOR_SIZE;i>=1;i--)DirectionErr_dot[i] =DirectionErr_dot[i-1]; 
    
    for(ADC_e  i = ADP1;i<AD_MAX;i++){                  //���ϴε�ֵ���ƴ洢
      for(int j = SENSOR_SIZE-1;j>=1;j--){
        sensor[i][j] = sensor[i][j-1];
      }
    }
    
    Read_ADC();         //��ȡ�µ�ֵ
    
    for(ADC_e i = ADP1; i < AD_MAX; i++)sensor[i][0] = (sensor[i][0]<10?10:sensor[i][0]);//����޷�  ���Ǽ��޷��˲�
    
    for(int i = 0;i<((AD_MAX>>1)-2);i++){ //ˮƽ�Ӱ��ֵĲ�Ⱥ�
        DirectionErr[i][0] = (my_sqrt((float)(sensor[i][0]),0)-my_sqrt((float)(sensor[ADP6-i][0]),0))/(sensor[i][0] + sensor[ADP6-i][0]) * 1000;
    }
    DirectionErr[(AD_MAX>>1)-2][0] = ((my_sqrt((float)(sensor[ADVR-1][0]),0)-my_sqrt((float)(sensor[ADVR+1][0]),0))/(sensor[ADVR-1][0] + sensor[ADVR+1][0])) * 1000;
}


/****���ƺ���****/
/**
*�����С��0ʱ,������ƫ,�����Ҫ���Ҵ���
*��������0ʱ,������ƫ,�����Ҫ�������
*/
void Direction_Control(void){
    /****ͣ������****/
    /*if(sensor[ADPR][0]<100)lcd_showuint8(0,9,101);
    else if(sensor[ADPR][0]>=100)lcd_showuint8(0,9,0);
    if(DirectionErr[2][0]<40)lcd_showuint8(60,9,202);
    else if(DirectionErr[2][0]>40)lcd_showuint8(60,9,0);*/
      if((sensor[ADPR][0]<40&&DirectionErr[2][0]<40)&&sensor[ADVR][0]<20){ //����ʹ�������ȷ //һ�ֲ�Ⱥ�Ϊ0��ʱ�������ĵ��ֵ��С��Ϊ����   sensor[ADPR][0]<100&&DirectionErr[2][0]<40
        Stop_Count++;
        if(Stop_Count>=7){
            Stop_Flag = 1; //ͣ��
            SAIDAO_FLAG = STOP;
        }
        lcd_showuint8(30,9,0);
    }
    else{
        
        SAIDAO_FLAG = SAIDAO_MAX;               //��ͷˢ�£������ж�ʱ���������ж���ά����һ�ε�pwm 
        //lcd_showuint8(30,9,22);
        Stop_Count = 0;
    }
    if(SAIDAO_FLAG != STOP){
        /***�����ж�**/
      if((sensor[ADP2][0]>200||sensor[ADP5][0]>200))     //�л���
      {
          SAIDAO_FLAG = HD;
          if(HD_HOLD!=0){}//������
          else if(sensor[ADP2][0]){
              HD_HOLD = 10;
          }
          else if(sensor[ADP5][0]){
              HD_HOLD = -10;
          }
          lcd_showuint8(30,9,22);
       }else if(((sensor[ADV1][0]>40&&sensor[ADV2][0]<20)||(sensor[ADV1][0]<20&&sensor[ADV2][0]>40))){     //ֱ��
          SAIDAO_FLAG = ZJ;
          if(sensor[ADV1][0]>40)ZJ_HOLD = 1;
          else if(sensor[ADV2][0]>40)ZJ_HOLD = -1;
          lcd_showuint8(30,9,33);
       }else if(sensor[ADV1][0]>60&&sensor[ADV2][0]>60){                //ʮ��
          SAIDAO_FLAG = SZ;
          lcd_showuint8(30,9,44);
       }else if(sensor[ADPR][0]<40&&DirectionErr[2][0]<40&&sensor[ADVR][0]<20){   //MISS
          SAIDAO_FLAG = MISS;
          //Stop_Count = 0;       //Stop��־����
       }else if(sensor[ADV1][0]<60&&sensor[ADV2][0]<60&&sensor[ADPR][0]>100&&(sensor[ADP3][0]>20||sensor[ADP4][0]>20)){
          if(HD_HOLD!=0)SAIDAO_FLAG = HD;
          //if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = HD;
          else if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = ZJ;         //��ֱ�Ǵ����Լ��ͷŻ�ʱ��ֱ��������״̬����
          else{
            SAIDAO_FLAG = COMMON;
            HD_HOLD = 0;
            ZJ_HOLD = 0;
          }
          lcd_showuint8(30,9,0);
       }
    }
    /**************************************************/
    //���������
    Servo_PWM_old = Servo_PWM_now;
    if(SAIDAO_FLAG == STOP){
      lcd_showuint8(0,9,101);
      Servo_PWM_now = Servo_PWM_old;
    }
    else if(SAIDAO_FLAG == COMMON){
      //float servo_pwm_delta = 0;
      //servo_pwm_delta = Direction_P*(DirectionErr[3][0] - DirectionErr[3][1]) + (DirectionErr[3][2]-2*DirectionErr[3][1]+DirectionErr[3][0])*Direction_D;
       Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;       //λ��ʽ  //����ͨ��ˮƽ�����PD
      //Servo_PWM_now = Servo_PWM_n ow - servo_pwm_delta;
      //Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0];
    }
    else if(SAIDAO_FLAG == HD){
      if(HD_HOLD != 0){
        //Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D-(sensor[ADP2][0]-sensor[ADP2][1])*Direction_D;
        if(HD_HOLD>0){
          Servo_PWM_now = SERVO_CENTER - Direction_PHD*DirectionErr[1][0] - (DirectionErr[1][0]-DirectionErr[1][1])*Direction_DHD;
          HD_HOLD--;
        }else if(HD_HOLD<0){
          Servo_PWM_now = SERVO_CENTER - Direction_PHD*2*DirectionErr[1][0] - (DirectionErr[1][0]-DirectionErr[1][1])*Direction_DHD*2+500; //��ת�����Ƚ�����
          HD_HOLD++;
        }
      }
    
    }
    else if(SAIDAO_FLAG == MISS){
      if(ZJ_HOLD!=0){
        if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else if(HD_HOLD!=0){
        if(HD_HOLD > 0)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else Servo_PWM_now = Servo_PWM_old;
    }
    else if(SAIDAO_FLAG == ZJ){
      //if(ZJ_HOLD == -1)Servo_PWM_now = SERVO_MAX;
      //if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN;
       Servo_PWM_now = SERVO_CENTER - Direction_PZJ*DirectionErr[3][0] - (DirectionErr[3][0]-DirectionErr[3][1])*Direction_DZJ;
      //if(DirectionErr[2][0]>60||DirectionErr[2][0]<-60)ZJ_HOLD = 0;
      //if(sensor[ADPR][0]>200&& )ZJ_HOLD = 0;
      /*if(my_min(DirectionErr[2])>50){
          ZJ_HOLD = 0;
          lcd_showuint8(30,9,1);
      }*/
    }else if(SAIDAO_FLAG == SZ){
          Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;
    }
    if(Servo_PWM_now>SERVO_MAX)Servo_PWM_now = SERVO_MAX;
    else if(Servo_PWM_now<SERVO_MIN)Servo_PWM_now = SERVO_MIN;
    Setted_Speed = 30;
    ServoPWM(Servo_PWM_now);
}

void Read_ADC(){
    My_adc_once(ADC_8bit);//����&&����(ÿ����вɼ�5��)
    //=================��ֵ�˲�============================
    for(ADC_e i = ADP1;i < AD_MAX;   i++){//һ�ֵ��
        ad_val[i][0] = (ad_val[i][2]+ad_val[i][3]+ad_val[i][4])/3;      //��ֵ�˲�
        sensor[i][0] = (uint16)(ad_val[i][0]/10*10);  //��ʱ���������ȴ���.ps:�Ժ������õ�(����13�����)
        //֮����Ҫ�������ȴ�������ΪӲ�����������ȵ����ƣ��ڸ���ֵ����ϵͳ��Ƶ������
        //����Ҫ��������
        //sensor_p[i] = ad_p[i][0];
    }
    
}