#include "include.h"

PTXn_e  I2C_PTxn[I2C_MAX] = {BMX_SCL_PIN,BMX_SDA_PIN,JG_SCL_PIN,JG_SDA_PIN};

void IIC_gpio_init (PTXn_e ptxn, GPIO_CFG cfg, uint8 data)
{
    gpio_init(ptxn,cfg,data);
    port_init_NoALT(ptxn,ODO|PULLUP); 
}

void IIC_gpio_set(PTXn_e ptxn,uint8 data){
    IIC_gpio_init(ptxn,GPO,data);
}

/********软件I2C，除了Stop状态以外，其余状态末尾时钟总线都为低电平**********/

void IIC_Start(I2C_e SCL_PIN,I2C_e SDA_PIN)                 
{
    IIC_gpio_set(SDA_PIN,1);                    //拉高数据线
    IIC_gpio_set(SCL_PIN,1);                   //拉高时钟线
    DELAY_US(SCL_TIME>>1);                 //延时
    IIC_gpio_set(SDA_PIN,0);                    //产生下降沿
    DELAY_US(SCL_TIME>>1);                 //延时
    IIC_gpio_set(SCL_PIN,0);                    //拉低时钟线     准备数据输入
    DELAY_US(SCL_TIME>>1);
}

void IIC_Stop(I2C_e SCL_PIN,I2C_e SDA_PIN){
    IIC_gpio_set(SDA_PIN,0);
      
      //拉低数据线     正常stop
    DELAY_US(SCL_TIME>>1);
    IIC_gpio_set(SCL_PIN,1);                    //拉高时钟线
    DELAY_US(SCL_TIME>>1);                 //延时
    IIC_gpio_set(SDA_PIN,1);                     //产生上升沿
    DELAY_US(SCL_TIME>>1);                 //延时         最终时钟线和数据线都为高电平（空闲状态）
}  

uint8 IIC_RecvACK(I2C_e SCL_PIN,I2C_e SDA_PIN){
    //gpio_set(SDA_PIN,1);        //释放总线        放在第9个数据周期之前
    //DELAY_US(SCL_TIME>>1);
    IIC_gpio_set(SCL_PIN,1);                    //拉高时钟线 上升沿
    DELAY_US(SCL_TIME>>1);                 //延时   
    IIC_gpio_init (SDA_PIN, GPI,0);                //*******************************这里SDA**************//
    uint8 ack_times = 0;
    while(gpio_get(SDA_PIN)){                   //读应答信号
        ack_times++;
        if(ack_times>=10){
            IIC_gpio_init (SDA_PIN, GPO,1);
            IIC_gpio_set(SCL_PIN,0);
            DELAY_US(SCL_TIME>>1);
            return 1;
        }
    }
    IIC_gpio_set(SCL_PIN, 0);                    //拉低时钟线
    IIC_gpio_init (SDA_PIN, GPO,1);         //数据总线空闲               //***************************这里SDA**********//
    DELAY_US(SCL_TIME>>1);                 //延时       普通延时 半个周期  
    return 0;
}

void IIC_SendNACK(I2C_e SCL_PIN,I2C_e SDA_PIN){
    //若输出ACK信号则为多字节读取
    IIC_gpio_init(SDA_PIN,GPO,1);
    IIC_gpio_set(SCL_PIN,1);
    DELAY_US(SCL_TIME<<1);     //第9个周期开始
    IIC_gpio_set(SCL_PIN,0);
    IIC_gpio_init(SDA_PIN,GPI,0);
    DELAY_US(SCL_TIME>>1);
}

void IIC_write_Byte(uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN){          //发送一字节的数据    前提是数据线处于数据输出态
      uint8 temp=0;
      for(uint8 i=0;i<8;i++){
          temp = data&0x80;
          data = data<<1;
          IIC_gpio_set(SDA_PIN,temp);
          //DELAY_US(SCL_TIME>>1);
          IIC_gpio_set(SCL_PIN,1);
          DELAY_US(SCL_TIME>>1);
          IIC_gpio_set(SCL_PIN,0); 
          if(i == 7)IIC_gpio_set(SDA_PIN,1);        //在第8个周期的末尾将数据总线释放
          DELAY_US(SCL_PIN>>1);
      }
      
}

uint8 IIC_read_Byte(I2C_e SCL_PIN,I2C_e SDA_PIN){                  //接受一字节数据，前提是之前数据线空闲且之后处于读入状态
      uint8 temp = 0;
      //DELAY_US(SCL_TIME>>1);
      for(uint8 i=0;i<8;i++){
          IIC_gpio_set(SCL_PIN,1);
          DELAY_US(SCL_TIME>>1);
          temp = temp<<1;       //最后一个字节不需要移位
          temp |= gpio_get(SDA_PIN);
          IIC_gpio_set(SCL_PIN,0);
          DELAY_US(SCL_TIME>>1);   //留出足够的时间给从机
      }
      return temp;
}


void IIC_write_reg(uint8 address,uint8 reg,uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN){
     //IIC_gpio_init(SCL_PIN,GPO,1);      //初始化为空闲态
     //IIC_gpio_init(SDA_PIN,GPO,1);
     IIC_Start(SCL_PIN,SDA_PIN);                       //开始总线周期
     IIC_write_Byte(address<<1|0x0,SCL_PIN,SDA_PIN);
     //ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0);
     IIC_RecvACK(SCL_PIN,SDA_PIN);
     IIC_write_Byte(reg,SCL_PIN,SDA_PIN);
     //ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0);
     IIC_RecvACK(SCL_PIN,SDA_PIN);
     IIC_write_Byte(data,SCL_PIN,SDA_PIN);
     // ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0);
     IIC_RecvACK(SCL_PIN,SDA_PIN );
     IIC_Stop(SCL_PIN,SDA_PIN);
}

uint8 IIC_read_reg(uint8 address,uint8 reg,I2C_e SCL_PIN,I2C_e SDA_PIN){
     uint8 ret;
     //IIC_gpio_init(SCL_PIN,GPO,1);      //初始化为空闲态
     //IIC_gpio_init(SDA_PIN,GPO,1);
     IIC_Start(SCL_PIN,SDA_PIN);
     IIC_write_Byte(address<<1|(0x0),SCL_PIN,SDA_PIN);
     //ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0);
     //if(IIC_RecvACK(SCL_PIN,SDA_PIN))lcd_showuint8(0,7,1);
     //else lcd_showuint8(0,7,0);
     IIC_RecvACK(SCL_PIN,SDA_PIN);
     IIC_write_Byte(reg,SCL_PIN,SDA_PIN);
     //ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0); 
     //if(IIC_RecvACK(SCL_PIN,SDA_PIN))lcd_showuint8(0,8,1);
     //else lcd_showuint8(0,8,0);
     IIC_RecvACK(SCL_PIN,SDA_PIN);
     IIC_Start(SCL_PIN,SDA_PIN);
     IIC_write_Byte(address<<1|(0x1),SCL_PIN,SDA_PIN);
     //ASSERT((IIC_RecvACK(SCL_PIN,SDA_PIN))==0);
     //if(IIC_RecvACK(SCL_PIN,SDA_PIN))lcd_showuint8(0,9,1);
     //else lcd_showuint8(0,9,0);
     IIC_RecvACK(SCL_PIN,SDA_PIN);
     IIC_gpio_set(SDA_PIN,1);   //释放总线
     IIC_gpio_init(SDA_PIN,GPI,0);             //此处考虑延时
     ret = IIC_read_Byte(SCL_PIN,SDA_PIN);
     IIC_SendNACK(SCL_PIN,SDA_PIN);
     IIC_Stop(SCL_PIN,SDA_PIN);
     return ret;
}