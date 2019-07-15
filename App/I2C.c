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

/********���I2C������Stop״̬���⣬����״̬ĩβʱ�����߶�Ϊ�͵�ƽ**********/

void IIC_Start(I2C_e SCL_PIN,I2C_e SDA_PIN)                 
{
    IIC_gpio_set(SDA_PIN,1);                    //����������
    IIC_gpio_set(SCL_PIN,1);                   //����ʱ����
    DELAY_US(SCL_TIME>>1);                 //��ʱ
    IIC_gpio_set(SDA_PIN,0);                    //�����½���
    DELAY_US(SCL_TIME>>1);                 //��ʱ
    IIC_gpio_set(SCL_PIN,0);                    //����ʱ����     ׼����������
    DELAY_US(SCL_TIME>>1);
}

void IIC_Stop(I2C_e SCL_PIN,I2C_e SDA_PIN){
    IIC_gpio_set(SDA_PIN,0);
      
      //����������     ����stop
    DELAY_US(SCL_TIME>>1);
    IIC_gpio_set(SCL_PIN,1);                    //����ʱ����
    DELAY_US(SCL_TIME>>1);                 //��ʱ
    IIC_gpio_set(SDA_PIN,1);                     //����������
    DELAY_US(SCL_TIME>>1);                 //��ʱ         ����ʱ���ߺ������߶�Ϊ�ߵ�ƽ������״̬��
}  

uint8 IIC_RecvACK(I2C_e SCL_PIN,I2C_e SDA_PIN){
    //gpio_set(SDA_PIN,1);        //�ͷ�����        ���ڵ�9����������֮ǰ
    //DELAY_US(SCL_TIME>>1);
    IIC_gpio_set(SCL_PIN,1);                    //����ʱ���� ������
    DELAY_US(SCL_TIME>>1);                 //��ʱ   
    IIC_gpio_init (SDA_PIN, GPI,0);                //*******************************����SDA**************//
    uint8 ack_times = 0;
    while(gpio_get(SDA_PIN)){                   //��Ӧ���ź�
        ack_times++;
        if(ack_times>=10){
            IIC_gpio_init (SDA_PIN, GPO,1);
            IIC_gpio_set(SCL_PIN,0);
            DELAY_US(SCL_TIME>>1);
            return 1;
        }
    }
    IIC_gpio_set(SCL_PIN, 0);                    //����ʱ����
    IIC_gpio_init (SDA_PIN, GPO,1);         //�������߿���               //***************************����SDA**********//
    DELAY_US(SCL_TIME>>1);                 //��ʱ       ��ͨ��ʱ �������  
    return 0;
}

void IIC_SendNACK(I2C_e SCL_PIN,I2C_e SDA_PIN){
    //�����ACK�ź���Ϊ���ֽڶ�ȡ
    IIC_gpio_init(SDA_PIN,GPO,1);
    IIC_gpio_set(SCL_PIN,1);
    DELAY_US(SCL_TIME<<1);     //��9�����ڿ�ʼ
    IIC_gpio_set(SCL_PIN,0);
    IIC_gpio_init(SDA_PIN,GPI,0);
    DELAY_US(SCL_TIME>>1);
}

void IIC_write_Byte(uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN){          //����һ�ֽڵ�����    ǰ���������ߴ����������̬
      uint8 temp=0;
      for(uint8 i=0;i<8;i++){
          temp = data&0x80;
          data = data<<1;
          IIC_gpio_set(SDA_PIN,temp);
          //DELAY_US(SCL_TIME>>1);
          IIC_gpio_set(SCL_PIN,1);
          DELAY_US(SCL_TIME>>1);
          IIC_gpio_set(SCL_PIN,0); 
          if(i == 7)IIC_gpio_set(SDA_PIN,1);        //�ڵ�8�����ڵ�ĩβ�����������ͷ�
          DELAY_US(SCL_PIN>>1);
      }
      
}

uint8 IIC_read_Byte(I2C_e SCL_PIN,I2C_e SDA_PIN){                  //����һ�ֽ����ݣ�ǰ����֮ǰ�����߿�����֮���ڶ���״̬
      uint8 temp = 0;
      //DELAY_US(SCL_TIME>>1);
      for(uint8 i=0;i<8;i++){
          IIC_gpio_set(SCL_PIN,1);
          DELAY_US(SCL_TIME>>1);
          temp = temp<<1;       //���һ���ֽڲ���Ҫ��λ
          temp |= gpio_get(SDA_PIN);
          IIC_gpio_set(SCL_PIN,0);
          DELAY_US(SCL_TIME>>1);   //�����㹻��ʱ����ӻ�
      }
      return temp;
}


void IIC_write_reg(uint8 address,uint8 reg,uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN){
     //IIC_gpio_init(SCL_PIN,GPO,1);      //��ʼ��Ϊ����̬
     //IIC_gpio_init(SDA_PIN,GPO,1);
     IIC_Start(SCL_PIN,SDA_PIN);                       //��ʼ��������
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
     //IIC_gpio_init(SCL_PIN,GPO,1);      //��ʼ��Ϊ����̬
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
     IIC_gpio_set(SDA_PIN,1);   //�ͷ�����
     IIC_gpio_init(SDA_PIN,GPI,0);             //�˴�������ʱ
     ret = IIC_read_Byte(SCL_PIN,SDA_PIN);
     IIC_SendNACK(SCL_PIN,SDA_PIN);
     IIC_Stop(SCL_PIN,SDA_PIN);
     return ret;
}