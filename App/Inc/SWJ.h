/*****************************************************
*@file SWJ.h
*
*@core   K60FX
*@date   2019
******************************************************/
#ifndef __SWJ_H__
#define __SWJ_H__
#define CMD_WARE  3 //´®¿ÚÊ¾²¨Æ÷ÃüÁî
#define SBQ_FLAG 0x1
#define COMMON_FLAG 0x0
extern uint8 SWJ_Flag;
extern uint8 Encode_Switch_Value;
void SWJ_sendData(uint8 *addr,uint32 size);
void SWJ_Init(void);
uint8 Encode_Switch(void);
#endif