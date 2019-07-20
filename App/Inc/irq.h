/*****************************************************
*@file irq.h
*
*@core   K60FX
*@date   2019
******************************************************/

#ifndef __IRQ_H__
#define __IRQ_H__

extern int16 var[3];

void PIT0_IRQHandler(void);
void PIT1_IRQHandler(void);
void PORTA_IRQHandler(void);
void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);

#endif