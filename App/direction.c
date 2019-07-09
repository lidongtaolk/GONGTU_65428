/*****************************************************
*@file DirectionControl.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

/**********全局变量定义********/ 
double Direction_P = 61;	//方向控制P  提高P偏差响应变快 如果发现转弯及时但是过不了就应增大P
double Direction_D = 22;	//方向控制D
double DirectionErr[2][5] = {0};	//方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）               误差大于0则处于铜线左侧！！！
				//（g_fDirectionError[1]为一对垂直电感的差比和偏差）
double DirectionErr_dot[8] = {0};//方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）
                           //（g_fDirectionError_dot[1]为一对垂直电感的差比和偏差微分)

int16 sensor_p[5]={0};       //获取的电感值
int16 sensor_v[3]={0};


int Servo_PWM_now,Servo_PWM_old;

/****舵机控制****/
int Servo_PWM = 0,old_Servo_PWM = 0;



/***方向数据初始化***/
void Direction_Data_Init(void){
    for(int i=4;i>=1;i--){
        DirectionErr[0][i] = DirectionErr[0][i-1];//前一次的误差记录  动态滤波
       // DirectionErr[1][i] = DirectionErr[1][i-1];
    }
    for(int i = 7;i>=1;i--)DirectionErr_dot[i] =DirectionErr_dot[i-1]; 
}


/****控制函数****/
/**
*当误差小于0时,车向右偏,舵机需要向左打方向
*当误差大于0时,车向左偏,舵机需要向右打方向
*/
void Direction_Control(void){
    Read_ADC(); //电磁信号采集
    
    /****停车保护****/
    if(sensor_p[0]<10&&sensor_v[0]<10){ //计数使结果更精确
        Stop_Count++;
        if(Stop_Count>=7){
            Stop_Flag = 1; //停车
        }
    }
    else{
        Stop_Count = 0;
    }
    
    for(int i = 0; i < 5; i++)sensor_p[i] = (sensor_p[i]<10?10:sensor_p[i]);//电感限幅  考虑加限幅滤波
    for(int i = 0; i<3; i++)sensor_v[i] = (sensor_v[i]<10?10:sensor_v[i]);
    if(sensor_p[2]==10&&sensor_p[4]==10)Miss_Flag = 1;
    else Miss_Flag = 0;
    //方向误差计算
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
    
  
    
    //DirectionErr[0][0] = (sqrt((double)(sensor_p[2]))-sqrt((double)(sensor_p[4])))/(sensor_p[2] + sensor_p[4]);//差比和
     DirectionErr[0][0] = (((double)(sensor_p[0]))-((double)(sensor_p[2])))/(sensor_p[2] + sensor_p[0]);
     DirectionErr[0][0] = ((((double)(sensor_p[0]))-((double)(sensor_p[4])))/(sensor_p[0] + sensor_p[4])-DirectionErr[0][0])/((((double)(sensor_p[0]))-((double)(sensor_p[4])))/(sensor_p[0] + sensor_p[4])+DirectionErr[0][0]);
     DirectionErr[0][0]-= 0.421053;
      //DirectionErr[0][0] += (sqrt((double)(sensor_p[2]))-sqrt((double)(sensor_p[3])))/(sensor_p[2] + sensor_p[3]);
    //DirectionErr[0][0] = (DirectionErr[0][0]/2);                                                                   //是否考虑加权
    //DirectionErr[1][0] = (sqrt((double)(sensor_v[1]))-sqrt((double)(sensor_v[2])))/(sensor_v[1] + sensor_v[2]);//差比和
    
    
    //误差差分计算
    DirectionErr_dot[0] = DirectionErr[0][0] - DirectionErr[0][1];
    //DirectionErr_dot[1] = DirectionErr[1][0] - DirectionErr[1][1];
    
    
    /********************路径判断**********************/
    //优先级判断     1.丢线 2.长直道 3.一般情况（包括弯道和环岛）
    
    
    /* * * * * * * * * * 长直道判断* * * * * * * * * /
    
              /* * * * * * * * * * 丢线判断* * * * * * * * * * /*/
    
     if(Miss_Flag == 1&&zhijiao_Flag!=1){
        int i=0;
        while(DirectionErr_dot[i]==0)i++;
        if(DirectionErr_dot[i]>0)Servo_PWM_now = Servo_MIN;
        else Servo_PWM_now = Servo_MAX;
        ServoPWM(Servo_PWM_now);
        Setted_Speed = 30;
        printf("丢线\n");
        return;
     }
     if(Miss_Flag==1&&zhijiao_Flag == 1){
        if(zhizuo_Flag == 1)Servo_PWM_now = Servo_MAX;
        else Servo_PWM_now = Servo_MIN;
        ServoPWM(Servo_PWM_now);
        Setted_Speed = 30;
        //if()zhijiao_Flag = 0;
        if(sensor_p[4]==10&&zhijiao_Flag == 0)zhizuo_Flag = 0;
        printf("直角\n");
        return;
     } 
    
    
    
    /**************************************************/
    //舵机打脚输出
    Servo_PWM_now = Servo_Center - Direction_P*DirectionErr[0][0] - DirectionErr_dot[0]*Direction_D;       //暂先通过水平电感做PD
    if(Servo_PWM_now>81)Servo_PWM_now = Servo_MAX;
    else if(Servo_PWM_now<50)Servo_PWM_now = Servo_MIN;
    Setted_Speed = 30;
    zhijiao_Flag = 0;
    ServoPWM(Servo_PWM_now);
    //zhizuo_Flag = 0;
}

void Read_ADC(){
    My_adc_once(ADC_8bit);//采样&&排序(每个电感采集5次)
    //=================中值滤波============================
    for(int i = 0;i < AD_MAX;   i++){//一字电感
        ad_val[i][0] = (ad_val[i][2]+ad_val[i][3]+ad_val[i][4])/3;      //均值滤波
        sensor_p[i] = (int16)(ad_val[i][0]/10*10);  //暂时不做降精度处理.ps:以后或许会用到(出自13届代码)
        //之所以要做降精度处理是因为硬件传感器精度的限制，在给定值附近系统会频繁的振动
        //所以要加入死区
        //sensor_p[i] = ad_p[i][0];
    }
    
}