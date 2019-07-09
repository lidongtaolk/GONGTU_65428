/*****************************************************
*@file function.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

//���Կ��Ǽ����µ��оݣ������ֿռ䲻�������⣬���Կ���״̬ѹ��
int Miss_Flag = 0;      //�����о�
int Straight_Flag = 0;  //ֱ���о�
int Stop_Flag = 0;      //ͣ���о� 0:��Ч 1:����ͣ�� 2:�յ�ͣ��
int Common_Flag = 0;    //һ���о�
int Decelerate_Flag = 0;        //�����о�
int Accelerate_Flag = 0;        //�����о�
int Beep_Flag = 0;       //������
int zhijiao_Flag = 0;
int zhijiao_Count = 0;
int zhizuo_Flag = 0;
int zhizuo_Count = 0;

int Final_Flag = 0;     //״̬ѹ���о�
//Final_Flag = 0;     //״̬ѹ���о�

double my_abs(double data){
    return(data>0?data:-1*data);
}

int L_Variance(void){
    int L_Ave_X = 0,L_Ave_Y = 0; //һ�ֵ��ƽ��ֵ����ֱ���ƽ��ֵ
    int ans = 0,X = 0,Y = 0;
    L_Ave_X = (sensor_p[1]+sensor_p[2]+sensor_p[0]+sensor_p[3]+sensor_p[4])/5;
    L_Ave_Y = (sensor_v[1]+sensor_v[0]+sensor_v[2]);
    for(int i = 0;i<5;i++)X +=(sensor_p[i] - L_Ave_X)*(sensor_p[i] - L_Ave_X);
    X/=5;
    for(int i = 0;i<3;i++)Y +=(sensor_v[i] - L_Ave_Y)*(sensor_v[i] - L_Ave_Y);
    Y/=3;
    return ans = (X+Y)/2;
}