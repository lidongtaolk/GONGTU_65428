#include "include.h"

uint64 reset[FLASH_MAX] = { 10000, 10000,30000,10000,30000,10000,200, 0, 20};

void my_flash_reset1(){         //Direction控制参数reset
  flash_erase_sector(SECTOR_NUM);
  ASSERT(flash_write(SECTOR_NUM,DP*8,reset[DP])==1);
  ASSERT(flash_write(SECTOR_NUM,DD*8,reset[DD])==1);

  ASSERT(flash_write(SECTOR_NUM,DPZJ*8,reset[DPZJ])==1);
  ASSERT(flash_write(SECTOR_NUM,DDZJ*8,reset[DDZJ])==1);

  ASSERT(flash_write(SECTOR_NUM,DPHD*8,reset[DPHD])==1);
  ASSERT(flash_write(SECTOR_NUM,DDHD*8,reset[DDHD])==1);
}

void my_flash_write1(){         //Direction控制参==1数烧写
  flash_erase_sector(SECTOR_NUM);
  ASSERT(flash_write(SECTOR_NUM,DP*8,(uint64)Direction_P*1000));
  ASSERT(flash_write(SECTOR_NUM,DD*8,(uint64)Direction_D*1000));

  ASSERT(flash_write(SECTOR_NUM,DPZJ*8,(uint64)Direction_PZJ*1000));
  ASSERT(flash_write(SECTOR_NUM,DDZJ*8,(uint64)Direction_DZJ*1000));

  ASSERT(flash_write(SECTOR_NUM,DPHD*8,(uint64)Direction_PHD*1000));
  ASSERT(flash_write(SECTOR_NUM,DDHD*8,(uint64)Direction_DHD*1000));
}

void my_flash_read1(){           //Direction控制参数读取
 
  Direction_P = (float)(flash_read(SECTOR_NUM,DP*8,uint64))/1000.0;
  Direction_D = (float)(flash_read(SECTOR_NUM,DD*8,uint64))/1000.0;
  
  Direction_PZJ = (float)(flash_read(SECTOR_NUM,DPZJ*8,uint64))/1000.0;
  Direction_DZJ = (float)(flash_read(SECTOR_NUM,DDZJ*8,uint64))/1000.0;
  
  Direction_PHD = (float)(flash_read(SECTOR_NUM,DPHD*8,uint64))/1000.0;
  Direction_DHD = (float)(flash_read(SECTOR_NUM,DDHD*8,uint64))/1000.0;
}