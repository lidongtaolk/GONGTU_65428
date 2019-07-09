#include "common.h"
#include "menu.h"
#include "SEEKFREE_18TFT.h"
#include "carImage.h"

const uint8 *__TEAM_NAME__ = "GONGTU";

MENU_e   page_status = MAIN;
int option_key_status=0;
OPTION_e  old_option_status =1;         //�ɵ�ѡ�����ȷ��ˢ��һ��
OPTION_e  option_status = 1;            //�µ�ѡ��
uint8 *(opt_name[MENU_MAX][OPT_SIZE]) = {
                                          {"1.test","2.Servo","3.Motor"},        //���˵�
                                          {"1.test","2.test","3.test"},
                                          {"1.test","2.test","3.test"}};

void menu_init(){
  menu_showtitle();     //����
  for(int i = 0;i<3;i++){
    menu_showoption(opt_name[page_status][i],0,i+1,RED,WHITE);  //����ÿҳ��ѡ���������ͱ�����ѡ��ʱ��������
  }
}

void menu_update(){
  if(option_key_status == -1)return;
  old_option_status = option_status;
  if(((~KEY_ALL)&(1<<KEY_U))&&(option_key_status != KEY_U)){
    if(option_status == 0)option_status = OPT_SIZE-1;
    else option_status --;
    option_key_status = KEY_U;
  }else if(((~KEY_ALL)&(1<<KEY_D))&&(option_key_status != KEY_D)){
    if(option_status == OPT_SIZE-1)option_status = 0;
    else option_status ++;
    option_key_status = KEY_D;
  }else if(!((~KEY_ALL)&(1<<KEY_D))&&!((~KEY_ALL)&(1<<KEY_U))){
    option_key_status = KEY_MAX; //��ʾ���Ը���
  }
  if(old_option_status!=option_status){ //ֱ���������Ĳ�ˢ��
    lcd_showuint8(0,4,option_status);
    menu_showoption(opt_name[page_status][old_option_status],0,old_option_status+1,RED,WHITE);
    menu_showoption(opt_name[page_status][option_status],0,option_status+1,RED,BLUE);
  }
}

void menu_showtitle(){
    lcd_changestyle(BLUE,GREEN);
    showimage(gImage_car,0,0,1);
    lcd_showstr(38,0,__TEAM_NAME__);
    showimage(gImage_car,100,0,0);
}
void menu_showoption(uint8 *c,uint16 x,uint16 y,uint16 p,uint16 b){
        lcd_changestyle(p,b);   //��������ͱ���
        uint8 i,j;
	for(i=0; i<16; i++)     //���ñ���ɫ
	{
		Lcd_SetRegion(x,y*16+i,x+124,y*16+i);   //����д��
		for(j=0; j<125; j++)
		{
			LCD_WriteData_16Bit(BGCOLOR);
		}
	}
        lcd_showstr(x,y,c);     //д����
        lcd_changestyle(RED,WHITE);     //��������ͱ���       //����ͱ���Ϊȫ�ֱ���
}