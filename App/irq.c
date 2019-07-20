/*****************************************************
*@file function.c
*
*@core   K60FX
*@date   2019
******************************************************/

#include "include.h"
extern int16 var[3] = {0};
extern uint8 Encode_Switch_Value;
extern uint16 ad_p[7][6];//һ�ֵ��   ad_p[i][0]�ô����վ�Լ���ֵ�˲���Ľ��
extern uint16 ad_v[3][6];//��ֱ���   ad_v[i][0]�ô����վ�Լ���ֵ�˲���Ľ��
extern int PITx_Flag = 1;
//float distance = 0;
uint8 time = 0;
uint8 Motor_time = 10;
uint16 test_servo = SERVO_CENTER;
uint16 option_key_flash_status = 0;

void PIT0_IRQHandler(void){
  key_IRQHandler();     //��ʱɨ�谴��״̬
  menu_update();      //�˵�����
  Direction_Data_Update();
  //SWJ_sendData((uint8*)var,sizeof(var));
  if(page_status == M_SUB11){    //��һ���Ӳ˵����Ӳ˵�
    for(ADC_e i = ADP1; i <ADV1;i++){           //����ˮƽ�Ҳ����ֱ
      lcd_showuint8(0,i+1,ad_val[i][0]);
    }
    for(ADC_e i = ADV1;i < AD_MAX; i++){
      lcd_showuint8(60,i-ADV1+1,ad_val[i][0]);
    }
  }
  
  if(page_status == M_SUB12){
    for(ADC_e i = ADP1; i <ADV1;i++){           //����ˮƽ�Ҳ����ֱ
      lcd_showuint8(0,i+1,L_Variance1(sensor[i]));
    }
    for(ADC_e i = ADV1;i < AD_MAX; i++){
      lcd_showuint8(60,i-ADV1+1,L_Variance1(sensor[i]));
    }
  }
  
  if(page_status == M_SUB13){
    Direction_Control2();
    for(ADC_e i = ADP1; i <ADV1;i++){           //����ˮƽ�Ҳ����ֱ
      lcd_showuint8(0,i+1,sensor[i][0]);
    }
    for(ADC_e i = ADV1;i < AD_MAX; i++){
      lcd_showuint8(60,i-ADV1+1,sensor[i][0]);
    }
    //SAIDAO_FLAG = STOP;
    lcd_showuint16(0,8,SAIDAO_FLAG);
  }
  
  if(page_status == M_SUB21){    //�ڶ����Ӳ˵����Ӳ˵�
    if(((~KEY_ALL)&(1<<KEY_L))&&(option_key_status!=KEY_L)){    //KEY_L����       // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
       test_servo+=100;
       if(test_servo>SERVO_MAX)test_servo=SERVO_MIN;
       ServoPWM(test_servo);
       option_key_status = KEY_L;
    }
    else if(((~KEY_ALL)&(1<<KEY_R))&&(option_key_status!=KEY_R)){       //KEY_R����       //ͬ��
       test_servo-=100;
       if(test_servo<SERVO_MIN)test_servo=SERVO_MAX;
       ServoPWM(test_servo);
       option_key_status = KEY_R;
    }else if(!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_D))){       //�ܵ�ʹ��
        option_key_status = KEY_MAX; //��ʾ���Ը���
    }
    lcd_showuint16(60,1,test_servo);
  }
  
  if(page_status == M_SUB22){
    Direction_Control2();
    int32 a = 0;
    for(int i = 0;i<((AD_MAX>>1)-2);i++){ //ˮƽ�Ӱ��ֵĲ�Ⱥ�
      a = (int32)((DirectionErr[i][0])*1000);
      //lcd_showfloat(0,i,DirectionErr[i][0]);
      lcd_showfloat(0,i+1,a);
      //printf("%d:%f\n",i,DirectionErr[i][0]);
      //if(i==2)printf("%d\n",a);
    }
    a = (int32)((DirectionErr[(AD_MAX>>1)-2][0])*1000);
    lcd_showfloat(0,(AD_MAX>>1)-1,a);
    lcd_showuint16(0,8,SAIDAO_FLAG);
    //lcd_showint16(0,6,(int32)(DirectionErr[3][0]));
    //printf("%d:%f\n",(AD_MAX>>1)-2,DirectionErr[(AD_MAX>>1)-2][0]);
  }
  
  if(page_status == M_SUB23){
     Direction_Control();
     lcd_showuint16(0,1,Servo_PWM_now);
     lcd_showuint16(0,2,SAIDAO_FLAG);
     int32 a = 0;
     a = my_ave2()*1000;
     printf("ave2:%d\n",a);
     lcd_showfloat(0,3,a);
     a = my_ave3()*1000;
     printf("ave3:%d\n",a);
     lcd_showfloat(0,4,a);
  }
  
  if(page_status == M_SUB24){
    if(((KEY_ALL_HOLD)&(1<<KEY_A))&&((~option_key_flash_status)&(1<<(KEY_A+KEY_MAX))))   //����A          ����
    {                                        //����
      my_flash_reset1();
      my_flash_read1();
      Beep_On();
      DELAY_MS(10);
      Beep_Off();
      option_key_flash_status |= (1<<(KEY_A+KEY_MAX));
    }else if(((KEY_ALL_HOLD)&(1<<KEY_OK))&&((~option_key_flash_status)&(1<<(KEY_OK+KEY_MAX)))) //����ok д��        //��д��������ڳ���������rom�е�ֵ
    {  
      my_flash_write1();               //���ڿհ״���
       Beep_On();
       DELAY_MS(10);
       Beep_Off();
       option_key_flash_status |= (1<<(KEY_OK+KEY_MAX));
    
    }else if((~(KEY_ALL)&(1<<KEY_OK))&&((~option_key_flash_status)&(1<<(KEY_OK))))             //ok ��ȡ���뿪��
    {                                       //���ж��Ƿ�Ϊ���������ǳ�����ִ�����
       if(option_status == DP)Direction_P = Encode_Switch();
       else if(option_status == DD)Direction_D = Encode_Switch();//����
       else if(option_status == DPZJ)Direction_PZJ = Encode_Switch();
       else if(option_status == DDZJ)Direction_DZJ = Encode_Switch();
       option_key_flash_status |= (1<<(KEY_OK));
    }else if((~(KEY_ALL)&(1<<KEY_A))&&((~option_key_flash_status)&(1<<(KEY_A))))               //A ����
    {                                       //���ж��Ƿ�Ϊ���������ǳ�����ִ�����
       my_flash_read1();
       option_key_flash_status |=(1<<(KEY_A));
    
    }else if(!((KEY_ALL_HOLD)&(1<<KEY_A))&&!((KEY_ALL_HOLD)&(1<<KEY_OK))&&!(~(KEY_ALL)&(1<<KEY_OK))&&!(~(KEY_ALL)&(1<<KEY_A)))
    {
      option_key_flash_status = 0;
    }
    /****���ҿ��ظ���ֵ***/
    if(((~KEY_ALL)&(1<<KEY_L))&&(option_key_status!=KEY_L)){    //KEY_L����       // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
       if(option_status == DP)Direction_P += 1;
       else if(option_status == DD)Direction_D += 1;
       else if(option_status == DPZJ)Direction_PZJ +=1;
       else if(option_status == DDZJ)Direction_DZJ +=1;
       option_key_status = KEY_L;
    }
    else if(((~KEY_ALL)&(1<<KEY_R))&&(option_key_status!=KEY_R)){       //KEY_R����       //ͬ��
       if(option_status == DP)Direction_P -= 1;
       else if(option_status == DD)Direction_D -= 1;
       else if(option_status == DPZJ)Direction_PZJ -= 1;
       else if(option_status == DDZJ)Direction_DZJ -= 1;
       option_key_status = KEY_R;
    }else if(!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_D))){       //�ܵ�ʹ��
        option_key_status = KEY_MAX; //��ʾ���Ը���
    }
    
    uint8 *str1 = "DP:"; 
    uint8 *str2 = "DD:";
    uint8 *str3 = "DPZJ:";
    uint8 *str4 = "DDZJ:";
    
    lcd_showstr(0,1,str1);
    int32 a = Direction_P * 1000;
    lcd_showfloat(20,DP+1,a);
    
    lcd_showstr(0,2,str2);
    a = Direction_D * 1000;
    lcd_showfloat(20,DD+1,a);
    
    lcd_showstr(0,3,str3);
    a = Direction_PZJ * 1000;
    lcd_showfloat(40,DPZJ+1,a);
    
    lcd_showstr(0,4,str4);
    a = Direction_DZJ * 1000;
    lcd_showfloat(40,DDZJ+1,a);
  }
  
  if(page_status == M_SUB25){
    if(((KEY_ALL_HOLD)&(1<<KEY_A))&&((~option_key_flash_status)&(1<<(KEY_A+KEY_MAX))))   //����A          ����
    {                                        //����
      my_flash_reset1();
      my_flash_read1();
      Beep_On();
      DELAY_MS(10);
      Beep_Off();
      option_key_flash_status |= (1<<(KEY_A+KEY_MAX));
    }else if(((KEY_ALL_HOLD)&(1<<KEY_OK))&&((~option_key_flash_status)&(1<<(KEY_OK+KEY_MAX)))) //����ok д��        //��д��������ڳ���������rom�е�ֵ
    {  
       my_flash_write1();               //���ڿհ״���
       Beep_On();
       DELAY_MS(10);
       Beep_Off();
       option_key_flash_status |= (1<<(KEY_OK+KEY_MAX));
    
    }else if((~(KEY_ALL)&(1<<KEY_OK))&&((~option_key_flash_status)&(1<<(KEY_OK))))             //ok ��ȡ���뿪��
    {                                       //���ж��Ƿ�Ϊ���������ǳ�����ִ�����
       if(option_status == DPHD)Direction_PHD = Encode_Switch();
       else if(option_status == DDHD)Direction_DHD = Encode_Switch();//����
       option_key_flash_status |= (1<<(KEY_OK));
    }else if((~(KEY_ALL)&(1<<KEY_A))&&((~option_key_flash_status)&(1<<(KEY_A))))               //A ����
    {                                       //���ж��Ƿ�Ϊ���������ǳ�����ִ�����
       my_flash_read1();
       option_key_flash_status |=(1<<(KEY_A));
    
    }else if(!((KEY_ALL_HOLD)&(1<<KEY_A))&&!((KEY_ALL_HOLD)&(1<<KEY_OK))&&!(~(KEY_ALL)&(1<<KEY_OK))&&!(~(KEY_ALL)&(1<<KEY_A)))
    {
      option_key_flash_status = 0;
    }
    /****���ҿ��ظ���ֵ***/
    if(((~KEY_ALL)&(1<<KEY_L))&&(option_key_status!=KEY_L)){    //KEY_L����       // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
       if(option_status == DPHD)Direction_PHD += 1;
       else if(option_status == DDHD)Direction_DHD += 1;
       option_key_status = KEY_L;
    }
    else if(((~KEY_ALL)&(1<<KEY_R))&&(option_key_status!=KEY_R)){       //KEY_R����       //ͬ��
       if(option_status == DPHD)Direction_PHD -= 1;
       else if(option_status == DDHD)Direction_DHD -= 1;
       option_key_status = KEY_R;
    }else if(!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_D))){       //�ܵ�ʹ��
        option_key_status = KEY_MAX; //��ʾ���Ը���
    }
    
    uint8 *str1 = "DPHD:"; 
    uint8 *str2 = "DDHD:";
    
    lcd_showstr(0,DPHD+1,str1);
    int32 a = Direction_PHD * 1000;
    lcd_showfloat(60,DPHD+1,a);
    
    lcd_showstr(0,DDHD+1,str2);
    a = Direction_DHD * 1000;
    lcd_showfloat(60,DDHD+1,a);
  }

  if(page_status == M_SUB31){
    var[0]=0;//MotorPWM;
    var[1]=Bmq_Get();
    SWJ_sendData((uint8*)var,sizeof(var));
    if(((~KEY_ALL)&(1<<KEY_L))&&(option_key_status!=KEY_L)){    //KEY_L����       // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
       MotorPWM+=10;
       if(MotorPWM>900)MotorPWM=900;
       if(Bmq_Get() == 0)MotorPWM =0;
       option_key_status = KEY_L;
    }
    else if(((~KEY_ALL)&(1<<KEY_R))&&(option_key_status!=KEY_R)){       //KEY_R����       //ͬ��
       MotorPWM-=10;
       if(MotorPWM<0)MotorPWM=0;
       if(Bmq_Get() == 0)MotorPWM =0;
       option_key_status = KEY_R;
    }else if(!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_D))){       //�ܵ�ʹ��
        option_key_status = KEY_MAX; //��ʾ���Ը���
    }
    PWM_Out();
    lcd_showint16(0,1,var[1]);
    lcd_showuint16(60,1,MotorPWM);
  }
  
  if(page_status == M_SUB32){
    Direction_Control();
    if(((~KEY_ALL)&(1<<KEY_L))&&(option_key_status!=KEY_L)){    //KEY_L����       // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
       MotorPWM+=10;
       if(MotorPWM>900)MotorPWM=900;
       if(Bmq_Get() == 0)MotorPWM =0;
       option_key_status = KEY_L;
    }
    else if(((~KEY_ALL)&(1<<KEY_R))&&(option_key_status!=KEY_R)){       //KEY_R����       //ͬ��
       MotorPWM-=10;
       if(MotorPWM<0)MotorPWM=0;
       if(Bmq_Get() == 0)MotorPWM =0;
       option_key_status = KEY_R;
    }else if(!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_D))){       //�ܵ�ʹ��
        option_key_status = KEY_MAX; //��ʾ���Ը���
    }
    //if(SAIDAO_FLAG == STOP)MotorPWM = 0;
    Motor_time++;
    if(Motor_time >= 10){
      var[0]=0;//MotorPWM;
      var[1]=Bmq_Get();
      //if(var[1] < 300)MotorPWM = 0;
      //printf("%d\n",var[1]);
      PWM_Out();
      time = 0;
    }
    lcd_showint16(0,1,var[1]);
    lcd_showuint16(60,1,MotorPWM);
    lcd_showuint16(0,2,Servo_PWM_now);
    lcd_showuint16(0,3,SAIDAO_FLAG);
    //lcd_showuint16(0,4,sensor[ADPR][0]);
  }
  
  if(page_status == M_SUB41){
     //uint8 i = gpio_get(BMX_SCL_PIN);
     //lcd_showuint8(0,1,i);
     uint8 i = IIC_read_reg(0x29,0xC2,BMX_SCL_PIN,BMX_SDA_PIN);
     lcd_showuint8(0,1,i);
     i = IIC_read_reg(0x29,0xC0,BMX_SCL_PIN,BMX_SDA_PIN);
     lcd_showuint8(0,2,i);
     i = IIC_read_reg(0x29,0x50,BMX_SCL_PIN,BMX_SDA_PIN);
     lcd_showuint8(0,3,i);lcd_showuint16(30,3,(i+1)<<1);
     i = IIC_read_reg(0x29,0x70,BMX_SCL_PIN,BMX_SDA_PIN);
     lcd_showuint8(0,4,i);lcd_showuint16(30,4,(i+1)<<1);
     
     IIC_write_reg(0x29,0x00,0x01,BMX_SCL_PIN,BMX_SDA_PIN);
     uint8 val = 0;
     uint16 cnt = 0;
     while (cnt < 5) { // 1 second waiting time max
      DELAY_MS(10);
      val = IIC_read_reg(0x29,0x00,BMX_SCL_PIN,BMX_SDA_PIN);
      if (val & 0x01) break;
      cnt++;
     }
     lcd_showuint8(0,5,val);
     uint8 gbuf[12] = {0};
     for(int j=0x14;j<(0x14+12);j++){
      gbuf[j-0x14] = IIC_read_reg(0x29,j,BMX_SCL_PIN,BMX_SDA_PIN);
     }
     uint16 acnt = ((gbuf[6] & 0xFF) << 8) | (gbuf[7] & 0xFF);
     uint16 scnt = ((gbuf[8] & 0xFF) << 8) | (gbuf[9] & 0xFF);
     uint16 dist = ((gbuf[10] & 0xFF) << 8) | (gbuf[11] & 0xFF);
      
     uint8 DeviceRangeStatusInternal = ((gbuf[0]&0x78)>>3);
     
     lcd_showuint16(0,6,acnt);
     lcd_showuint16(0,7,scnt);
     lcd_showuint16(0,8,dist);
     lcd_showuint8(0,9,DeviceRangeStatusInternal);
  }
  
  if(page_status == M_SUB51){
    time++;
    if(time == CSB_TIME){
      if(CSB_FLAG == CSB1){
        CSB_Send1();
        int32 a = 0;
        a = (int32)(CSB_DISTANCE1*10);
        lcd_showfloat(0,1,a);
      }else if(CSB_FLAG == CSB2){
        CSB_Send2();
        int32 a = 0;
        a = (int32)(CSB_DISTANCE2*10);
        lcd_showfloat(0,2,a);
      }
      time = 0;
    }
  }
  /*Speed_Get_Data();
  Speed_Control();
  Speed_Data_Save();*/
  if(page_status == M_MAIN){    //���˵�
    
    lcd_changestyle(GREEN,WHITE);
    lcd_showuint8(25,8,key_check(KEY_A));
    lcd_showuint8(60,8,key_check(KEY_B));
    lcd_changestyle(RED,WHITE);
    lcd_showuint8(0,8,Encode_Switch());
    lcd_showuint16(0,9,KEY_ALL);
    lcd_showuint16(60,9,KEY_ALL_HOLD);
  }
  PIT_Flag_Clear(PIT0);
}

void PORTB_IRQHandler(void){
  if(PORTB_ISFR&(1<<8)){
    CSB_IRQHandler1(&CSB_DISTANCE1);
    CSB_FLAG = CSB2;    //�رձ�������ʹ�ܣ�������һ��������ʹ��
    PORTB_ISFR = (1<<8);
  }
}

void PORTC_IRQHandler(void){
  if(PORTC_ISFR&(1<<10)){
    CSB_IRQHandler2(&CSB_DISTANCE2);
    CSB_FLAG = CSB1;    //�رձ�������ʹ�ܣ�������һ��������ʹ��
    PORTC_ISFR = (1<<10);
  }
}

void PIT1_IRQHandler(void){
  
        /*//pit_time_start(PIT2);
        printf("���볬�����ж�\n");
        gpio_set(CSB_Trig,1);
        DELAY_US(10);
        gpio_set(CSB_Trig,0);
        while(gpio_get(CSB_Echo)==0);
        pit_time_start(PIT2);
        while(gpio_get(CSB_Echo));
        uint32 time = pit_time_get(PIT2); //��λ��һ��bus����
        if(time != ~0){   //δ���
            time = time*1000/bus_clk_khz; //��λΪ΢�룻
            distance = 0.34*time/20;
            printf("����Ϊ��%f cm\n",distance);
        }
        //uint32 T=pit_time_get(PIT3);
        //T = time*1000/bus_clk_khz;
        //printf("������ʱʱ��Ϊ��%d us\n",T);
        pit_close(PIT2);
        //pit_close(PIT3);*/
  if(PITx_Flag){
    ServoPWM(1000);
    PITx_Flag^=1;
    printf("PWM1\n");
  }else{
    ServoPWM(300);
    PITx_Flag^=1;
    
    printf("PWM3\n");
  }
  PIT_Flag_Clear(PIT1);
}
void PORTA_IRQHandler(void){
  if(PORTA_ISFR&(1<<6)){        
    uint8 *c = "start";
    lcd_showstr(0,1,c);
    if(key_check(KEY_A) == KEY_DOWN){
        Beep_On();      //����DEBUGģʽ��
        DELAY_MS(10);
        Beep_Off();
    }
    PORTA_ISFR = (1<<6);
  }
}