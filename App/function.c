/*****************************************************
*@file function.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

//可以考虑加入坡道判据，若出现空间不够的问题，可以考虑状态压缩
int Miss_Flag = 0;      //丢线判据
int Straight_Flag = 0;  //直道判据
int Stop_Flag = 0;      //停车判据 0:无效 1:丢线停车 2:终点停车
int Common_Flag = 0;    //一般判据
int Decelerate_Flag = 0;        //减速判据
int Accelerate_Flag = 0;        //加速判据
int Beep_Flag = 0;       //蜂鸣器
int zhijiao_Flag = 0;
int zhijiao_Count = 0;
int zhizuo_Flag = 0;
int zhizuo_Count = 0;

int Final_Flag = 0;     //状态压缩判据
//Final_Flag = 0;     //状态压缩判据

double my_abs(double data){
    return(data>0?data:-1*data);
}

int L_Variance(void){
    int L_Ave_X = 0,L_Ave_Y = 0; //一字电感平均值和竖直电感平均值
    int ans = 0,X = 0,Y = 0;
    L_Ave_X = (sensor_p[1]+sensor_p[2]+sensor_p[0]+sensor_p[3]+sensor_p[4])/5;
    L_Ave_Y = (sensor_v[1]+sensor_v[0]+sensor_v[2]);
    for(int i = 0;i<5;i++)X +=(sensor_p[i] - L_Ave_X)*(sensor_p[i] - L_Ave_X);
    X/=5;
    for(int i = 0;i<3;i++)Y +=(sensor_v[i] - L_Ave_Y)*(sensor_v[i] - L_Ave_Y);
    Y/=3;
    return ans = (X+Y)/2;
}