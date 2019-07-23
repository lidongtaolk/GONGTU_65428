#ifndef __FLASH_H__
#define __FLASH_H__
#define SECTOR_NUM      (FLASH_SECTOR_NUM)
#define SECTOR_NUM2      (FLASH_SECTOR_NUM-1)
#define SECTOR_NUM3      (FLASH_SECTOR_NUM-2)

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
  COMMON_S,
  HD_S,
  ZJ_S,
  BZ_S,
  SZ_S,
  FLASH_MAX,
}FLASH_e;

extern void my_flash_reset1();

extern void my_flash_write1();

extern void my_flash_read1();

extern void my_flash_reset2();

extern void my_flash_write2();

extern void my_flash_read2();
#endif