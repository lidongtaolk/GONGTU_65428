/*****************************************************
*@file DirectionControl.c
*
*@core   K60FX
*@date   2019
******************************************************/
#include "include.h"

/**********全局变量定义********/ 
float Direction_P = 10;	//方向控制P  提高P偏差响应变快 如果发现转弯及时但是过不了就应增大P
float Direction_D = 10;	//方向控制D
float Direction_PZJ = 30;
float Direction_DZJ = 10;
float Direction_PHD = 30;
float Direction_DHD = 10;
float DirectionErr[(AD_MAX>>1)-1][SENSOR_SIZE] = {0};	//方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）               误差大于0则处于铜线左侧！！！ 
                                 //前三个为一字和八字电感的差比和，且按着从两边到中间 ，最后一个是竖直的差比和
				//（g_fDirectionError[1]为一对垂直电感的差比和偏差）
float DirectionErr_dot[8] = {0};//方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）
                           //（g_fDirectionError_dot[1]为一对垂直电感的差比和偏差微分)

uint16 sensor[10][SENSOR_SIZE]={0};       //获取的电感值


uint32 Servo_PWM_now = SERVO_CENTER,Servo_PWM_old = SERVO_CENTER;

/****舵机控制****/
int Servo_PWM = 0,old_Servo_PWM = 0;

int HD_HOLD = 0;

int HD_FLAG = 0;

int ZJ_HOLD = 0;
/***方向数据初始化***/
void Direction_Data_Update(void){

    for(int i=SENSOR_SIZE-1;i>=1;i--){                  //将上次的值后移存储
      for(int j = 1;j<=((AD_MAX>>1)-1);j++){    
        DirectionErr[j-1][i] = DirectionErr[j-1][i-1];
      }
    }
    
    for(int i = SENSOR_SIZE;i>=1;i--)DirectionErr_dot[i] =DirectionErr_dot[i-1]; 
    
    for(ADC_e  i = ADP1;i<AD_MAX;i++){                  //将上次的值后移存储
      for(int j = SENSOR_SIZE-1;j>=1;j--){
        sensor[i][j] = sensor[i][j-1];
      }
    }
    
    Read_ADC();         //获取新的值
    
    for(ADC_e i = ADP1; i < AD_MAX; i++)sensor[i][0] = (sensor[i][0]<10?10:sensor[i][0]);//电感限幅  考虑加限幅滤波
    
    for(int i = 0;i<((AD_MAX>>1)-2);i++){ //水平加八字的差比和
        DirectionErr[i][0] = (my_sqrt((float)(sensor[i][0]),0)-my_sqrt((float)(sensor[ADP6-i][0]),0))/(sensor[i][0] + sensor[ADP6-i][0]) * 1000;
    }
    DirectionErr[(AD_MAX>>1)-2][0] = ((my_sqrt((float)(sensor[ADVR-1][0]),0)-my_sqrt((float)(sensor[ADVR+1][0]),0))/(sensor[ADVR-1][0] + sensor[ADVR+1][0])) * 1000;
}


/****控制函数****/
/**
*当误差小于0时,车向左偏,舵机需要向右打方向
*当误差大于0时,车向右偏,舵机需要向左打方向
*/
void Direction_Control2(void){
     /****停车保护****/
    if((sensor[ADPR][0]<40&&my_abs(DirectionErr[2][0])<40)&&sensor[ADVR][0]<20){ //计数使结果更精确 //一字差比和为0的时候若中心电感值极小则为丢线   sensor[ADPR][0]<100&&DirectionErr[2][0]<40
        Stop_Count++;
        if(Stop_Count>=100){
            Stop_Flag = 1; //停车
            SAIDAO_FLAG = STOP;
         }
        lcd_showuint8(30,9,9);
    }
    else{
        if(HD_HOLD!=0)SAIDAO_FLAG = HD;         //若是HD关闭总使能则锁死为HD
        else SAIDAO_FLAG = SAIDAO_MAX;               //开头刷新，后面判断时若不更新判断则维持上一次的pwm 
        lcd_showuint8(30,9,22);
        Stop_Count = 0;
    }
    /***十字>环岛>直角**/
    if(SAIDAO_FLAG != STOP&&HD_HOLD == 0){    //关闭总使能  STOP   进环岛时
        /***赛道判断**/
       if(sensor[ADPR][0]<40&&DirectionErr[2][0]<40&&sensor[ADVR][0]<20){   //MISS
          SAIDAO_FLAG = MISS;
          //Stop_Count = 0;       //Stop标志清零
       }else if(sensor[ADV1][0]>140&&sensor[ADV2][0]>140){                //十字
          SAIDAO_FLAG = SZ;
          lcd_showuint8(30,9,44);
       }else if(((sensor[ADP2][0]>=120&&sensor[ADP5][0]>=120))&&(sensor[ADPR][0]>=240))     //判环岛 //加入参考电感判环                      //sensor[ADP2][0]>=240&&sensor[ADP5][0]>=160)||(sensor[ADP5][0]>=240&&sensor[ADP2][0]>=160)
      {
          SAIDAO_FLAG = HD;
          if(HD_FLAG == 0){     //只处理一次，后面由HD_HOLD关闭使能
                      //入环信号  信号在common处重置 但是在进环岛的情况下也有可能被清
            if(sensor[ADV1][0]>sensor[ADV2][0]){
              HD_FLAG = 1;
              HD_HOLD = 1;
              lcd_showuint8(30,9,12);
            }
            else if(sensor[ADV1][0]<sensor[ADV2][0]){
              HD_FLAG = 1;
              HD_HOLD = -1;
              lcd_showuint8(30,9,22);
            }
            else if(sensor[ADP2][0]>sensor[ADP5][0]){
              HD_FLAG = 1;
              HD_HOLD = 1;
              lcd_showuint8(30,9,12);
            }
            else if(sensor[ADP2][0]<sensor[ADP5][0]){
              HD_FLAG = 1;
              HD_HOLD = -1;
              lcd_showuint8(30,9,22);
            }
            else{
              SAIDAO_FLAG = COMMON;      //若竖直电感无反应则水平循迹
              lcd_showuint8(30,9,42);
            }
          }else{        //出环不处理
            SAIDAO_FLAG = COMMON;
            lcd_showuint8(30,9,32);
          }
       }else if(((sensor[ADV1][0]>140&&sensor[ADV2][0]<20)||(sensor[ADV1][0]<20&&sensor[ADV2][0]>140))&&sensor[ADPR][0]<100){     //直角
          SAIDAO_FLAG = ZJ;
          if(sensor[ADV1][0]>30)ZJ_HOLD = 1;
          else if(sensor[ADV2][0]>30)ZJ_HOLD = -1;
          lcd_showuint8(30,9,33);
       }else if(sensor[ADPR][0]>100&&(sensor[ADP3][0]>20||sensor[ADP4][0]>20)){ //COMMON        //sensor[ADV1][0]<60&&sensor[ADV2][0]<60&&
          //if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = HD;
          if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = ZJ;         //等直角处理自己释放或时除直角外其他状态更新
          else{
            if((sensor[ADP2][1]>=240||sensor[ADP5][1]>=160)&&HD_FLAG == 1){        //当最终出环岛的时候，重置入环信号
                HD_FLAG = 0;
            }
            SAIDAO_FLAG = COMMON;
            ZJ_HOLD = 0;
          }
          lcd_showuint8(30,9,0);
       }
    }
    /**************************************************/
     //舵机打脚输出
    Servo_PWM_old = Servo_PWM_now;
    if(SAIDAO_FLAG == STOP){
      // = 0;
      //PWM_Out();
      lcd_showuint8(0,9,101);
      Servo_PWM_now = Servo_PWM_old;
    }
    else if(SAIDAO_FLAG == SAIDAO_MAX){
      if( Servo_PWM_old>SERVO_CENTER){
           Servo_PWM_now = SERVO_MAX;
      }
      else if( Servo_PWM_old<SERVO_CENTER){
           Servo_PWM_now = SERVO_MIN;
      }
    }
    else if(SAIDAO_FLAG == COMMON){
      //float servo_pwm_delta = 0;
      //servo_pwm_delta = Direction_P*(DirectionErr[3][0] - DirectionErr[3][1]) + (DirectionErr[3][2]-2*DirectionErr[3][1]+DirectionErr[3][0])*Direction_D;
       Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;       //位置式  //暂先通过水平电感做PD
      //Servo_PWM_now = Servo_PWM_n ow - servo_pwm_delta;
      //Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0];
    }
    else if(SAIDAO_FLAG == HD){
      if(sensor[ADP2][0]>=120&&sensor[ADP5][0]>=120){//观察是否判为HD//if(sensor[ADVR][0]>10){      //竖直电感入环
        if(HD_HOLD>0){
            //DirectionErr[1][0]=(mysqrt((float)(sensor[ADVR][0]),0)-my_sqrt((float)(sensor[ADV1][0]),0))/(sensor[ADVR][0] + sensor[ADV1][0]) * 1000;  //竖直减左一
            Servo_PWM_now = 5900 - Direction_PHD*DirectionErr[2][0] - (DirectionErr[2][0] - DirectionErr[2][1])*Direction_DHD;
        }
        else if(HD_HOLD<0){
            //DirectionErr[1][0]=(my_sqrt((float)(sensor[ADV2][0]),0)-my_sqrt((float)(sensor[ADVR][0]),0))/(sensor[ADV2][0] + sensor[ADVR][0]) * 1000;    //竖直减左二 
            Servo_PWM_now = 7400 - Direction_PHD*DirectionErr[2][0] - (DirectionErr[2][0] - DirectionErr[2][1])*Direction_DHD;
        }
      }else{
        HD_HOLD = 0 ;//清HD关闭总使能标志
      }
    
    }
    else if(SAIDAO_FLAG == MISS){
      if(ZJ_HOLD!=0){   //清保护
        Stop_Count = 0;
        if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else if(HD_HOLD!=0){      //环岛保护
        if(HD_HOLD > 0)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else{     //其余情况
        if( Servo_PWM_old>SERVO_CENTER){
           Servo_PWM_now = SERVO_MAX;
        }
        else if( Servo_PWM_old<SERVO_CENTER){
           Servo_PWM_now = SERVO_MIN;
        }
      }
    }
    else if(SAIDAO_FLAG == ZJ){
      if(ZJ_HOLD == -1)Servo_PWM_now = SERVO_MAX- Direction_PZJ*DirectionErr[3][0] - (DirectionErr[3][0]-DirectionErr[3][1])*Direction_DZJ;
      if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN- Direction_PZJ*DirectionErr[3][0] - (DirectionErr[3][0]-DirectionErr[3][1])*Direction_DZJ;
    }else if(SAIDAO_FLAG == SZ){
          Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;
    }
    if(Servo_PWM_now>SERVO_MAX)Servo_PWM_now = SERVO_MAX;
    else if(Servo_PWM_now<SERVO_MIN)Servo_PWM_now = SERVO_MIN;
    //Setted_Speed = 340;
    ServoPWM(Servo_PWM_now);
}
/****无lcd显示*****/
void Direction_Control(void){
     /****停车保护****/
    if((sensor[ADPR][0]<40&&my_abs(DirectionErr[2][0])<40)&&sensor[ADVR][0]<20){ //计数使结果更精确 //一字差比和为0的时候若中心电感值极小则为丢线   sensor[ADPR][0]<100&&DirectionErr[2][0]<40
        Stop_Count++;
        if(Stop_Count>=10){
            Stop_Flag = 1; //停车
            SAIDAO_FLAG = STOP;
         }
        //lcd_showuint8(30,9,9);
    }
    else{
        if(HD_HOLD!=0)SAIDAO_FLAG = HD;         //若是HD关闭总使能则锁死为HD
        else SAIDAO_FLAG = SAIDAO_MAX;               //开头刷新，后面判断时若不更新判断则维持上一次的pwm 
        //lcd_showuint8(30,9,22);
        Stop_Count = 0;
    }
    /***十字>环岛>直角**/
    if(SAIDAO_FLAG != STOP&&HD_HOLD == 0){    //关闭总使能  STOP   进环岛时
        /***赛道判断**/
       if(sensor[ADPR][0]<40&&DirectionErr[2][0]<40&&sensor[ADVR][0]<20){   //MISS
          SAIDAO_FLAG = MISS;
          //Stop_Count = 0;       //Stop标志清零
       }else if(sensor[ADV1][0]>140&&sensor[ADV2][0]>140){                //十字
          SAIDAO_FLAG = SZ;
          //lcd_showuint8(30,9,44);
       }else if(((sensor[ADP2][0]>=120&&sensor[ADP5][0]>=120))&&(sensor[ADPR][0]>=240))     //判环岛 //加入参考电感判环                      //sensor[ADP2][0]>=240&&sensor[ADP5][0]>=160)||(sensor[ADP5][0]>=240&&sensor[ADP2][0]>=160)
      {
          SAIDAO_FLAG = HD;
          if(HD_FLAG == 0){     //只处理一次，后面由HD_HOLD关闭使能
                      //入环信号  信号在common处重置 但是在进环岛的情况下也有可能被清
            if(sensor[ADV1][0]>sensor[ADV2][0]){
              HD_FLAG = 1;
              HD_HOLD = 1;
              //lcd_showuint8(30,9,12);
            }
            else if(sensor[ADV1][0]<sensor[ADV2][0]){
              HD_FLAG = 1;
              HD_HOLD = -1;
              //lcd_showuint8(30,9,22);
            }
            else if(sensor[ADP2][0]>sensor[ADP5][0]){
              HD_FLAG = 1;
              HD_HOLD = 1;
              //lcd_showuint8(30,9,12);
            }
            else if(sensor[ADP2][0]<sensor[ADP5][0]){
              HD_FLAG = 1;
              HD_HOLD = -1;
              //lcd_showuint8(30,9,22);
            }
            else{
              SAIDAO_FLAG = COMMON;      //若竖直电感无反应则水平循迹
              //lcd_showuint8(30,9,42);
            }
          }else{        //出环不处理
            SAIDAO_FLAG = COMMON;
            //lcd_showuint8(30,9,32);
          }
       }else if(((sensor[ADV1][0]>140&&sensor[ADV2][0]<20)||(sensor[ADV1][0]<20&&sensor[ADV2][0]>140))&&sensor[ADPR][0]<100){     //直角
          SAIDAO_FLAG = ZJ;
          if(sensor[ADV1][0]>30)ZJ_HOLD = 1;
          else if(sensor[ADV2][0]>30)ZJ_HOLD = -1;
          //lcd_showuint8(30,9,33);
       }else if(sensor[ADPR][0]>100&&(sensor[ADP3][0]>20||sensor[ADP4][0]>20)){ //COMMON        //sensor[ADV1][0]<60&&sensor[ADV2][0]<60&&
          //if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = HD;
          if(SAIDAO_FLAG ==ZJ)SAIDAO_FLAG = ZJ;         //等直角处理自己释放或时除直角外其他状态更新
          else{
            if((sensor[ADP2][1]>=240||sensor[ADP5][1]>=160)&&HD_FLAG == 1){        //当最终出环岛的时候，重置入环信号
                HD_FLAG = 0;
            }
            SAIDAO_FLAG = COMMON;
            ZJ_HOLD = 0;
          }
          //lcd_showuint8(30,9,0);
       }
    }
    /**************************************************/
     //舵机打脚输出
    Servo_PWM_old = Servo_PWM_now;
    if(SAIDAO_FLAG == STOP){
      // = 0;
      //PWM_Out();
      //lcd_showuint8(0,9,101);
      Servo_PWM_now = Servo_PWM_old;
    }
    else if(SAIDAO_FLAG == SAIDAO_MAX){
      if( Servo_PWM_old>SERVO_CENTER){
           Servo_PWM_now = SERVO_MAX;
      }
      else if( Servo_PWM_old<SERVO_CENTER){
           Servo_PWM_now = SERVO_MIN;
      }
    }
    else if(SAIDAO_FLAG == COMMON){
      //float servo_pwm_delta = 0;
      //servo_pwm_delta = Direction_P*(DirectionErr[3][0] - DirectionErr[3][1]) + (DirectionErr[3][2]-2*DirectionErr[3][1]+DirectionErr[3][0])*Direction_D;
       Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;       //位置式  //暂先通过水平电感做PD
      //Servo_PWM_now = Servo_PWM_n ow - servo_pwm_delta;
      //Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0];
    }
    else if(SAIDAO_FLAG == HD){
      if(sensor[ADP2][0]>=120&&sensor[ADP5][0]>=120){//观察是否判为HD//if(sensor[ADVR][0]>10){      //竖直电感入环
        if(HD_HOLD>0){
            //DirectionErr[1][0]=(mysqrt((float)(sensor[ADVR][0]),0)-my_sqrt((float)(sensor[ADV1][0]),0))/(sensor[ADVR][0] + sensor[ADV1][0]) * 1000;  //竖直减左一
            Servo_PWM_now = 5900 - Direction_PHD*DirectionErr[2][0] - (DirectionErr[2][0] - DirectionErr[2][1])*Direction_DHD;
        }
        else if(HD_HOLD<0){
            //DirectionErr[1][0]=(my_sqrt((float)(sensor[ADV2][0]),0)-my_sqrt((float)(sensor[ADVR][0]),0))/(sensor[ADV2][0] + sensor[ADVR][0]) * 1000;    //竖直减左二 
            Servo_PWM_now = SERVO_MAX - Direction_PHD*DirectionErr[2][0] - (DirectionErr[2][0] - DirectionErr[2][1])*Direction_DHD;
        }
      }else{
        HD_HOLD = 0 ;//清HD关闭总使能标志
      }
    
    }
    else if(SAIDAO_FLAG == MISS){
      if(ZJ_HOLD!=0){   //清保护
        Stop_Count = 0;
        if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else if(HD_HOLD!=0){      //环岛保护
        if(HD_HOLD > 0)Servo_PWM_now = SERVO_MIN;
        else Servo_PWM_now = SERVO_MAX;
      }
      else{     //其余情况
        if( Servo_PWM_old>SERVO_CENTER){
           Servo_PWM_now = SERVO_MAX;
        }
        else if( Servo_PWM_old<SERVO_CENTER){
           Servo_PWM_now = SERVO_MIN;
        }
      }
    }
    else if(SAIDAO_FLAG == ZJ){
      if(ZJ_HOLD == -1)Servo_PWM_now = SERVO_MAX- Direction_PZJ*DirectionErr[3][0] - (DirectionErr[3][0]-DirectionErr[3][1])*Direction_DZJ;
      if(ZJ_HOLD == 1)Servo_PWM_now = SERVO_MIN- Direction_PZJ*DirectionErr[3][0] - (DirectionErr[3][0]-DirectionErr[3][1])*Direction_DZJ;
    }else if(SAIDAO_FLAG == SZ){
          Servo_PWM_now = SERVO_CENTER - Direction_P*DirectionErr[2][0] - (DirectionErr[2][0]-DirectionErr[2][1])*Direction_D;
    }
    if(Servo_PWM_now>SERVO_MAX)Servo_PWM_now = SERVO_MAX;
    else if(Servo_PWM_now<SERVO_MIN)Servo_PWM_now = SERVO_MIN;
    //Setted_Speed = 340;
    ServoPWM(Servo_PWM_now);
}

void Read_ADC(){ 
    My_adc_once(ADC_8bit);//采样&&排序(每个电感采集5次)
    //=================中值滤波============================
    for(ADC_e i = ADP1;i < AD_MAX;   i++){//一字电感
        ad_val[i][0] = (ad_val[i][2]+ad_val[i][3]+ad_val[i][4])/3;      //均值滤波
        sensor[i][0] = (uint16)(ad_val[i][0]/10*10);  //暂时不做降精度处理.ps:以后或许会用到(出自13届代码)
        //之所以要做降精度处理是因为硬件传感器精度的限制，在给定值附近系统会频繁的振动
        //所以要加入死区
        //sensor_p[i] = ad_p[i][0];
    }
}