#ifndef __FLASH_H__
#define __FLASH_H__
#define SECTOR_NUM      (FLASH_SECTOR_NUM)

typedef enum{
  DP,
  DD,
  DPZJ,
  DDZJ,
  DPHD,
  DDHD,
  SP,
  SD,
  SI,
  FLASH_MAX,
}FLASH_e;

extern void my_flash_reset1();

extern void my_flash_write1();

extern void my_flash_read1();

#endif