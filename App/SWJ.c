/*****************************************************
*@file SWJ.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"
uint8 SWJ_Flag = 0x0;
uint8 Encode_Switch_Value = 0;

void SWJ_Init(void){
    key_init(KEY_MAX);  //��ͨ�ܽų�ʼ��
    port_init_NoALT(PTA6,IRQ_FALLING);   //A6�������ж�       �ж�ʹ����irqģ�������
    
    Beep_Init();
}

uint8 Encode_Switch(void){
    return get_bmkey_value();
}

void SWJ_sendData(uint8* addr,uint32 size){
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����
    uart_putbuff(UART0, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(UART0, addr, size);    //��������
    uart_putbuff(UART0, cmdr, sizeof(cmdr));    //���ͺ�����
}