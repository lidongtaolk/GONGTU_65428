#include "common.h"
#include "include.h"

//各状态判据在function.h中定义
extern uint8 var[3]={0};


int main(){
   // lcd初始化
   //Servo_Init();
   //CSB_Init();
   //Setted_Speed = 70;
   lcd_init();
   //Motor_Init();
   //Bmq_Init();
   My_adc_init();
   SWJ_Init();
   //MotorPWM = 30;
   uart_init(UART0,9600);
   menu_init();
   Beep_On();
   DELAY_MS(10);
   Beep_Off();
   pit_init_ms(PIT0,1);
   set_vector_handler(PORTA_VECTORn,PORTA_IRQHandler);
   set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);
   //set_vector_handler(PIT1_VECTORn,PIT1_IRQHandler);
   NVIC_SetPriorityGrouping((uint32)0x3);
   NVIC_SetPriority(PIT0_IRQn,2); 
   //NVIC_SetPriority(PIT1_IRQn,0);
   NVIC_SetPriority(PORTA_IRQn,1);
   disable_irq(PORTA_IRQn);
   enable_irq(PIT0_IRQn);
   //enable_irq(PIT1_IRQn);
   while(1){
   }
}
