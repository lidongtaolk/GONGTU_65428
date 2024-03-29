/*****************************************************
*@file function.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

//可以考虑加入坡道判据，若出现空间不够的问题，可以考虑状态压缩
extern int Miss_Flag;      //丢线判据
extern int Straight_Flag;  //直道判据
extern int Stop_Flag;      //停车判据 0:无效 1:丢线停车 2:终点停车
extern int Common_Flag;    //一般判据
extern int Decelerate_Flag;        //减速判据
extern int Accelerate_Flag;        //加速判据
extern int Beep_Flag;       //蜂鸣器
extern int zhijiao_Flag;
extern int zhijiao_Count;
extern int zhizuo_Flag;
extern int zhizuo_Count;

extern int Final_Flag;     //状态压缩判据

typedef enum{
  COMMON,       //初始不做特殊判断
  CZ,           //长直
  HD,           //环岛
  ZJ,           //直角
  SZ,           //十字
  MISS,         //丢线
  STOP,           //停车
  BZ,              //避障
  SAIDAO_MAX,  //状态个数  
}SAIDAO_e;

extern SAIDAO_e SAIDAO_FLAG;

//绝对值
float my_abs(float data);
float my_err(float *data,uint8 size);
float my_sqrt(float number,int greater);
float L_Variance1(uint16 *sensor);
int L_Variance2(void);   //计算工字电感的方差

#endif