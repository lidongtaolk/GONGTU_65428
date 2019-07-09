/*****************************************************
*@file DirectionControl.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

/**********ȫ�ֱ�������********/ 
double Direction_P = 61;	//�������P  ���Pƫ����Ӧ��� �������ת�估ʱ���ǹ����˾�Ӧ����P
double Direction_D = 22;	//�������D
double DirectionErr[2][5] = {0};	//����ƫ�g_fDirectionError[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ�               ������0����ͭ����࣡����
				//��g_fDirectionError[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ�
double DirectionErr_dot[8] = {0};//����ƫ��΢�֣�g_fDirectionError_dot[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ��΢�֣�
                           //��g_fDirectionError_dot[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ��΢��)

int16 sensor_p[5]={0};       //��ȡ�ĵ��ֵ
int16 sensor_v[3]={0};


int Servo_PWM_now,Servo_PWM_old;

/****�������****/
int Servo_PWM = 0,old_Servo_PWM = 0;



/***�������ݳ�ʼ��***/
void Direction_Data_Init(void){
    for(int i=4;i>=1;i--){
        DirectionErr[0][i] = DirectionErr[0][i-1];//ǰһ�ε�����¼  ��̬�˲�
       // DirectionErr[1][i] = DirectionErr[1][i-1];
    }
    for(int i = 7;i>=1;i--)DirectionErr_dot[i] =DirectionErr_dot[i-1]; 
}


/****���ƺ���****/
/**
*�����С��0ʱ,������ƫ,�����Ҫ�������
*��������0ʱ,������ƫ,�����Ҫ���Ҵ���
*/
void Direction_Control(void){
    Read_ADC(); //����źŲɼ�
    
    /****ͣ������****/
    if(sensor_p[0]<10&&sensor_v[0]<10){ //����ʹ�������ȷ
        Stop_Count++;
        if(Stop_Count>=7){
            Stop_Flag = 1; //ͣ��
        }
    }
    else{
        Stop_Count = 0;
    }
    
    for(int i = 0; i < 5; i++)sensor_p[i] = (sensor_p[i]<10?10:sensor_p[i]);//����޷�  ���Ǽ��޷��˲�
    for(int i = 0; i<3; i++)sensor_v[i] = (sensor_v[i]<10?10:sensor_v[i]);
    if(sensor_p[2]==10&&sensor_p[4]==10)Miss_Flag = 1;
    else Miss_Flag = 0;
    //����������
    if(sensor_v[0]>10)zhijiao_Count += 1;
    else{
      zhijiao_Count = 0;
    }
    if(sensor_p[4]>10)zhizuo_Count+=1;
    else{
      zhizuo_Count = 0;
    }
    if(zhijiao_Count>=5)zhijiao_Flag = 1;
    if(zhizuo_Count>=5)zhizuo_Flag = 1;
    
  
    
    //DirectionErr[0][0] = (sqrt((double)(sensor_p[2]))-sqrt((double)(sensor_p[4])))/(sensor_p[2] + sensor_p[4]);//��Ⱥ�
     DirectionErr[0][0] = (((double)(sensor_p[0]))-((double)(sensor_p[2])))/(sensor_p[2] + sensor_p[0]);
     DirectionErr[0][0] = ((((double)(sensor_p[0]))-((double)(sensor_p[4])))/(sensor_p[0] + sensor_p[4])-DirectionErr[0][0])/((((double)(sensor_p[0]))-((double)(sensor_p[4])))/(sensor_p[0] + sensor_p[4])+DirectionErr[0][0]);
     DirectionErr[0][0]-= 0.421053;
      //DirectionErr[0][0] += (sqrt((double)(sensor_p[2]))-sqrt((double)(sensor_p[3])))/(sensor_p[2] + sensor_p[3]);
    //DirectionErr[0][0] = (DirectionErr[0][0]/2);                                                                   //�Ƿ��Ǽ�Ȩ
    //DirectionErr[1][0] = (sqrt((double)(sensor_v[1]))-sqrt((double)(sensor_v[2])))/(sensor_v[1] + sensor_v[2]);//��Ⱥ�
    
    
    //����ּ���
    DirectionErr_dot[0] = DirectionErr[0][0] - DirectionErr[0][1];
    //DirectionErr_dot[1] = DirectionErr[1][0] - DirectionErr[1][1];
    
    
    /********************·���ж�**********************/
    //���ȼ��ж�     1.���� 2.��ֱ�� 3.һ���������������ͻ�����
    
    
    /* * * * * * * * * * ��ֱ���ж�* * * * * * * * * /
    
              /* * * * * * * * * * �����ж�* * * * * * * * * * /*/
    
     if(Miss_Flag == 1&&zhijiao_Flag!=1){
        int i=0;
        while(DirectionErr_dot[i]==0)i++;
        if(DirectionErr_dot[i]>0)Servo_PWM_now = Servo_MIN;
        else Servo_PWM_now = Servo_MAX;
        ServoPWM(Servo_PWM_now);
        Setted_Speed = 30;
        printf("����\n");
        return;
     }
     if(Miss_Flag==1&&zhijiao_Flag == 1){
        if(zhizuo_Flag == 1)Servo_PWM_now = Servo_MAX;
        else Servo_PWM_now = Servo_MIN;
        ServoPWM(Servo_PWM_now);
        Setted_Speed = 30;
        //if()zhijiao_Flag = 0;
        if(sensor_p[4]==10&&zhijiao_Flag == 0)zhizuo_Flag = 0;
        printf("ֱ��\n");
        return;
     } 
    
    
    
    /**************************************************/
    //���������
    Servo_PWM_now = Servo_Center - Direction_P*DirectionErr[0][0] - DirectionErr_dot[0]*Direction_D;       //����ͨ��ˮƽ�����PD
    if(Servo_PWM_now>81)Servo_PWM_now = Servo_MAX;
    else if(Servo_PWM_now<50)Servo_PWM_now = Servo_MIN;
    Setted_Speed = 30;
    zhijiao_Flag = 0;
    ServoPWM(Servo_PWM_now);
    //zhizuo_Flag = 0;
}

void Read_ADC(){
    My_adc_once(ADC_8bit);//����&&����(ÿ����вɼ�5��)
    //=================��ֵ�˲�============================
    for(int i = 0;i < AD_MAX;   i++){//һ�ֵ��
        ad_val[i][0] = (ad_val[i][2]+ad_val[i][3]+ad_val[i][4])/3;      //��ֵ�˲�
        sensor_p[i] = (int16)(ad_val[i][0]/10*10);  //��ʱ���������ȴ���.ps:�Ժ������õ�(����13�����)
        //֮����Ҫ�������ȴ�������ΪӲ�����������ȵ����ƣ��ڸ���ֵ����ϵͳ��Ƶ������
        //����Ҫ��������
        //sensor_p[i] = ad_p[i][0];
    }
    
}