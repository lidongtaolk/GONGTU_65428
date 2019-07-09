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
    key_init(KEY_MAX);  //普通管脚初始化
    port_init_NoALT(PTA6,IRQ_FALLING);   //A6上升沿中断       中断使能在irq模块中完成
    
    Beep_Init();
}

uint8 Encode_Switch(void){
    return get_bmkey_value();
}

void SWJ_sendData(uint8* addr,uint32 size){
    uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令
    uart_putbuff(UART0, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(UART0, addr, size);    //发送数据
    uart_putbuff(UART0, cmdr, sizeof(cmdr));    //发送后命令
}