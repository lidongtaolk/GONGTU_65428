#ifndef __MENU_H__
#define __MENU_H__


typedef enum
{
  OPT_0,
  OPT_1,
  OPT_2,
  OPT_3,
  OPT_4,
  OPT_5,
  OPT_SIZE,
}OPTION_e;

typedef enum
{
  M_MAIN,
  
  M_SUB1,
  M_SUB2,
  M_SUB3,
  M_SUB4,
  M_SUB5,
  M_SUB6,
  
  M_SUB11,
  M_SUB12,
  M_SUB13,
  M_SUB14,
  M_SUB15,
  M_SUB16,
  
  M_SUB21,
  M_SUB22,
  M_SUB23,
  M_SUB24,
  M_SUB25, 
  M_SUB26,

  M_SUB31,
  M_SUB32,
  M_SUB33,
  M_SUB34,
  M_SUB35, 
  M_SUB36,
  
  M_SUB41,
  M_SUB42,
  M_SUB43,
  M_SUB44,
  M_SUB45, 
  M_SUB46,
  
  M_SUB51,
  M_SUB52,
  M_SUB53,
  M_SUB54,
  M_SUB55, 
  M_SUB56,
  
  M_SUB61,
  M_SUB62,
  M_SUB63,
  M_SUB64,
  M_SUB65, 
  M_SUB66,
  
  MENU_MAX,
}MENU_e;

extern MENU_e   page_status;
extern OPTION_e  old_option_status;         //旧的选项，用来确保刷新一次
extern OPTION_e  option_status;            //新的选项
extern int option_key_status;

void menu_init();

void menu_showtitle(void);

void menu_showoption(uint8 *c,uint16 x,uint16 y,uint16 p,uint16 b);

void menu_update(void);
#endif