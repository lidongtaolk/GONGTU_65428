#include "include.h"

uint64 reset[FLASH_MAX] = { 10000, 10000,30000,10000,30000,10000,2000, 0, 0,    200000,150000,150000,150000,200000};



void my_flash_reset1(){         //Direction控制参数reset
  flash_erase_sector(SECTOR_NUM);
  ASSERT(flash_write(SECTOR_NUM,DP*8,reset[DP])==1);
  ASSERT(flash_write(SECTOR_NUM,DD*8,reset[DD])==1);

  ASSERT(flash_write(SECTOR_NUM,DPZJ*8,reset[DPZJ])==1);
  ASSERT(flash_write(SECTOR_NUM,DDZJ*8,reset[DDZJ])==1);

  ASSERT(flash_write(SECTOR_NUM,DPHD*8,reset[DPHD])==1);
  ASSERT(flash_write(SECTOR_NUM,DDHD*8,reset[DDHD])==1);
}

void my_flash_reset2(){
  flash_erase_sector(SECTOR_NUM2);
  ASSERT(flash_write(SECTOR_NUM2,DP*8,reset[SP])==1);
  ASSERT(flash_write(SECTOR_NUM2,DD*8,reset[SD])==1);
  ASSERT(flash_write(SECTOR_NUM2,DPZJ*8,reset[SI])==1);
}

void my_flash_write1(){         //Direction控制参数烧写
  flash_erase_sector(SECTOR_NUM);
  ASSERT(flash_write(SECTOR_NUM,DP*8,(uint64)Direction_P*1000));
  ASSERT(flash_write(SECTOR_NUM,DD*8,(uint64)Direction_D*1000));

  ASSERT(flash_write(SECTOR_NUM,DPZJ*8,(uint64)Direction_PZJ*1000));
  ASSERT(flash_write(SECTOR_NUM,DDZJ*8,(uint64)Direction_DZJ*1000));

  ASSERT(flash_write(SECTOR_NUM,DPHD*8,(uint64)Direction_PHD*1000));
  ASSERT(flash_write(SECTOR_NUM,DDHD*8,(uint64)Direction_DHD*1000));
}

void my_flash_write2(){         //Direction控制参数烧写
  flash_erase_sector(SECTOR_NUM2); 
  ASSERT(flash_write(SECTOR_NUM2,DP*8,(uint64)(Speed_Kp*1000)));
  ASSERT(flash_write(SECTOR_NUM2,DD*8,(uint64)(Speed_Kd*1000)));
  ASSERT(flash_write(SECTOR_NUM2,DPZJ*8,(uint64)(Speed_Ki*1000)));
}

void my_flash_read1(){           //Direction控制参数读取
 
  Direction_P = (float)(flash_read(SECTOR_NUM,DP*8,uint64))/1000.0;
  Direction_D = (float)(flash_read(SECTOR_NUM,DD*8,uint64))/1000.0;
  
  Direction_PZJ = (float)(flash_read(SECTOR_NUM,DPZJ*8,uint64))/1000.0;
  Direction_DZJ = (float)(flash_read(SECTOR_NUM,DDZJ*8,uint64))/1000.0;
  
  Direction_PHD = (float)(flash_read(SECTOR_NUM,DPHD*8,uint64))/1000.0;
  Direction_DHD = (float)(flash_read(SECTOR_NUM,DDHD*8,uint64))/1000.0;
}

void my_flash_read2(){           //Direction控制参数读取
 
  Speed_Kp = (float)(flash_read(SECTOR_NUM2,DP*8,uint64))/1000.0;
  Speed_Kd = (float)(flash_read(SECTOR_NUM2,DD*8,uint64))/1000.0;
  Speed_Ki = (float)(flash_read(SECTOR_NUM2,DPZJ*8,uint64))/1000.0;
}


void my_flash_reset3(){
  flash_erase_sector(SECTOR_NUM3);
  ASSERT(flash_write(SECTOR_NUM3,DP*8,reset[COMMON_S])==1);
  ASSERT(flash_write(SECTOR_NUM3,DD*8,reset[HD_S])==1);
  ASSERT(flash_write(SECTOR_NUM3,DPZJ*8,reset[ZJ_S])==1);
  ASSERT(flash_write(SECTOR_NUM3,DDZJ*8,reset[BZ_S])==1);
  ASSERT(flash_write(SECTOR_NUM3,DPHD*8,reset[SZ_S])==1);
  
}

extern void my_flash_write3(){
  flash_erase_sector(SECTOR_NUM3);
  ASSERT(flash_write(SECTOR_NUM3,DP*8,(uint64)COMMON_SPEED*1000));
  ASSERT(flash_write(SECTOR_NUM3,DD*8,(uint64)HD_SPEED*1000));

  ASSERT(flash_write(SECTOR_NUM3,DPZJ*8,(uint64)ZJ_SPEED*1000));
  ASSERT(flash_write(SECTOR_NUM3,DDZJ*8,(uint64)BZ_SPEED*1000));

  ASSERT(flash_write(SECTOR_NUM3,DPHD*8,(uint64)SZ_SPEED*1000));

}

extern void my_flash_read3(){
  
  COMMON_SPEED= (float)(flash_read(SECTOR_NUM3,DP*8,uint64))/1000.0;
  HD_SPEED = (float)(flash_read(SECTOR_NUM3,DD*8,uint64))/1000.0;
  
  ZJ_SPEED = (float)(flash_read(SECTOR_NUM3,DPZJ*8,uint64))/1000.0;
  BZ_SPEED = (float)(flash_read(SECTOR_NUM3,DDZJ*8,uint64))/1000.0;
  
  SZ_SPEED = (float)(flash_read(SECTOR_NUM3,DPHD*8,uint64))/1000.0;

}












