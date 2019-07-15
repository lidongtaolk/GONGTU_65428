/*****************************************************
*@file CSB.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __CSB_H__
#define __CSB_H__
#define CSB_TRIG_TIME   10


extern uint16 CSB_Time;

extern void CSB_Init();

extern void CSB_Send();

extern uint16 CSB_Get(void);
#endif