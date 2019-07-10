/*****************************************************
*@file function.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

//���Կ��Ǽ����µ��оݣ������ֿռ䲻�������⣬���Կ���״̬ѹ��
extern int Miss_Flag;      //�����о�
extern int Straight_Flag;  //ֱ���о�
extern int Stop_Flag;      //ͣ���о� 0:��Ч 1:����ͣ�� 2:�յ�ͣ��
extern int Common_Flag;    //һ���о�
extern int Decelerate_Flag;        //�����о�
extern int Accelerate_Flag;        //�����о�
extern int Beep_Flag;       //������
extern int zhijiao_Flag;
extern int zhijiao_Count;
extern int zhizuo_Flag;
extern int zhizuo_Count;

extern int Final_Flag;     //״̬ѹ���о�

//����ֵ
double my_abs(double data);
float my_sqrt(float number,int greater);
float L_Variance1(uint16 *sensor);
int L_Variance2(void);   //���㹤�ֵ�еķ���

#endif