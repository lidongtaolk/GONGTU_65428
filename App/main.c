#include "common.h"
#include "include.h"

//各状态判据在function.h中定义
 
int main(){
   // lcd初始化
   Servo_Init();
   CSB_Init1();CSB_Init2();
   //Setted_Speed = 70; 
   lcd_init();
   Motor_Init();
   Bmq_Init();
   My_adc_init();
   SWJ_Init();
   IIC_gpio_init(BMX_SCL_PIN,GPO,1);
   IIC_gpio_init(BMX_SDA_PIN,GPO,1);
   flash_init();        //初始化flash
   MotorPWM = 340;
   Setted_Speed = 200;
   uart_init(UART0,9600);
   menu_init();
   Beep_On();
   DELAY_MS(10);
   Beep_Off();
   pit_init_ms(PIT0,1);
   set_vector_handler(PORTC_VECTORn,PORTC_IRQHandler);
   //set_vector_handler(PORTA_VECTORn,PORTA_IRQHandler);
   set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);
   //set_vector_handler(PIT1_VECTORn,PIT1_IRQHandler);
   set_vector_handler(PORTB_VECTORn,PORTB_IRQHandler);
   NVIC_SetPriorityGrouping((uint32)0x3);
   NVIC_SetPriority(PIT0_IRQn,2); 
   NVIC_SetPriority(PORTB_IRQn,1);
   NVIC_SetPriority(PORTC_IRQn,1);
   //NVIC_SetPriority(PORTA_IRQn,2);
   //disable_irq(PORTA_IRQn);
   enable_irq(PIT0_IRQn);
   enable_irq(PORTB_IRQn);
   enable_irq(PORTC_IRQn);
   my_flash_read1();
   my_flash_read2();
   my_flash_read3();
   while(1){
   }
}
