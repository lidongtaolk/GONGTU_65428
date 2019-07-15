#include "common.h"
#include "menu.h"
#include "SEEKFREE_18TFT.h"
#include "carImage.h"

const uint8 *__TEAM_NAME__ = "GONGTU";

MENU_e   page_status = M_MAIN;
int option_key_status=0;                //过去的按键状态，用作关闭使能
OPTION_e  old_option_status =1;         //旧的选项，用来确保刷新一次
OPTION_e  option_status = 1;            //新的选项
uint8 *(opt_name[MENU_MAX][OPT_SIZE]) = {
                                          {"1.adc","2.Servo","3.Motor","4.I2C","5.CSB",""},        //主菜单
                                          
                                          {"1.value","2.L_V","3.sensor","4.","",""},         //子菜单
                                          {"1.pwm","2.ERROR","3.START","4.DataUpdate","5.HDcanshu",""},
                                          {"1.PWM","2.START","3.test3","4.","",""},
                                          {"1.START","2.STOP","","","",""},
                                          {"1.START","2.STOP","","","",""},
                                          {"1.START","2.STOP","","","",""},
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                         
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
                                          {"","","","","",""},    //子子菜单
};

/*
*菜单初始化
*用来更新主菜单和子菜单
*/
void menu_init(){
  menu_showtitle();     //标题
  for(OPTION_e i = OPT_0;i<OPT_SIZE;i++){
    menu_showoption(opt_name[page_status][i],0,i+1,RED,WHITE);  //设置每页的选项，包括字体和背景（选中时的蓝条）
  }
}

/*
*菜单刷新
*@brief 若是关键KEY都弹起，则不操作（但是仍要开总使能）
*/
void menu_update(){
  if(option_key_status == -1)return;
  /*if(((~KEY_ALL)&(1<<KEY_OK))&&(page_status == M_MAIN)&&(option_key_status != KEY_OK)){        //首先确认是否要更改PAGE
    page_status = option_status+1;
    menu_init();        //更改后刷新页面
    return;
  }*/
  if(((~KEY_ALL)&(1<<KEY_OK))&&(option_key_status != KEY_OK)){        //首先确认是否要更改PAGE //子菜单级联和子子菜单级联使用一个算法
    uint32 tem = page_status*6+option_status+1;
    if(tem<MENU_MAX){
      page_status = tem;
      menu_init();        //更改后刷新页面
    }
    return;
  }
  if(((~KEY_ALL)&(1<<KEY_B))&&(page_status != M_MAIN)&&(option_key_status != KEY_OK)){        //回退
    page_status = (page_status-1)/6;
    menu_init();        //更改后刷新页面
    return;
  }
  old_option_status = option_status;                            //其次再更改选项选中状态
  if(((~KEY_ALL)&(1<<KEY_U))&&(option_key_status != KEY_U)){    //向上选中  // 执行后关闭使能，避免其值快速变化
    if(option_status == 0)option_status = OPT_SIZE-1;
    else option_status --;
    option_key_status = KEY_U;
  }else if(((~KEY_ALL)&(1<<KEY_D))&&(option_key_status != KEY_D)){      //向下选中
    if(option_status == OPT_SIZE-1)option_status = 0;
    else option_status ++;
    option_key_status = KEY_D;
  }else if(!((~KEY_ALL)&(1<<KEY_D))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_OK))){         //总使能
    option_key_status = KEY_MAX; //表示可以更改
  }
  if(old_option_status!=option_status){ //直到有所更改才刷新
    lcd_showuint8(0,OPT_SIZE+1,option_status);   //显示选项状态
    menu_showoption(opt_name[page_status][old_option_status],0,old_option_status+1,RED,WHITE);  //将原来的选中状态复位
    menu_showoption(opt_name[page_status][option_status],0,option_status+1,RED,BLUE);   //新的选中状态置位
  }
}

/*
*标题显示
*
*/
void menu_showtitle(){
    lcd_changestyle(BLUE,GREEN);
    showimage(gImage_car,0,0,1);
    lcd_showstr(38,0,__TEAM_NAME__);
    showimage(gImage_car,100,0,0);
}
/*
*选项显示
*@brief 可更改字体和当前行的背景
*/
void menu_showoption(uint8 *c,uint16 x,uint16 y,uint16 p,uint16 b){
        lcd_changestyle(p,b);   //设置字体和背景
        uint8 i,j;
	for(i=0; i<16; i++)     //设置背景色
	{
		Lcd_SetRegion(x,y*16+i,x+124,y*16+i);   //逐行写入
		for(j=0; j<125; j++)
		{
			LCD_WriteData_16Bit(BGCOLOR);
		}
	}
        lcd_showstr(x,y,c);     //写入字
        lcd_changestyle(RED,WHITE);     //重置字体和背景       //字体和背景为全局变量
}