#ifndef __MENU_H__
#define __MENU_H__


typedef enum
{
  OPT_0,
  OPT_1,
  OPT_2,
  OPT_SIZE,
}OPTION_e;

typedef enum
{
  MAIN,
  SUB1,
  SUB2,
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