#ifndef __I2C_H__
#define __I2C_H__

#define BMX055_DEVICE I2C1
#define BMX055_ADDRESS 0x18
#define LOWPOWER2       0x40
#define SLEEPTIMER_EN   0X20
#define SCL_TIME        8
#include "common.h"

typedef enum
{
  BSCL, //BMX055
  BSDA,
  
  JSCL, //¼¤¹â
  JSDA,
  I2C_MAX,
}I2C_e;



extern void IIC_Start(I2C_e SCL_PIN,I2C_e SDA_PIN);

extern void IIC_Stop(I2C_e SCL_PIN,I2C_e SDA_PIN);

extern uint8 IIC_RecvACK(I2C_e SCL_PIN,I2C_e SDA_PIN);

extern void IIC_SendNACK(I2C_e SCL_PIN,I2C_e SDA_PIN);

extern void IIC_write_Byte(uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN);

extern uint8 IIC_read_Byte(I2C_e SCL_PIN,I2C_e SDA_PIN);


extern void IIC_write_reg(uint8 address,uint8 reg,uint8 data,I2C_e SCL_PIN,I2C_e SDA_PIN);

extern uint8 IIC_read_reg(uint8 address,uint8 reg,I2C_e SCL_PIN,I2C_e SDA_PIN);

#endif