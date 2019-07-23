/*****************************************************
*@file CSB.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __CSB_H__
#define __CSB_H__
#define CSB_TRIG_TIME   10
#define CSB_TIME 10
#define CSB_NUM 10
#define CSB_COUNT 5

extern float OLD_CSB_DISTANCE1,OLD_CSB_DISTANCE2;
extern float CSB_DISTANCE1,CSB_DISTANCE2;
extern float CSB_ERR[CSB_NUM];
extern uint8 CSB_HOLD;


typedef enum{
  CSB1,
  CSB2,
  CSB_BZ,
  CSB_MAX,
}CSB_e;

extern CSB_e CSB_FLAG;

extern void CSB_DataUpdate();

extern void CSB_Control();

extern void CSB_Init1();

extern void CSB_Init2();

extern void CSB_Send1();

extern void CSB_Send2(); 
#endif