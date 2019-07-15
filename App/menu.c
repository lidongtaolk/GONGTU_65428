#include "common.h"
#include "menu.h"
#include "SEEKFREE_18TFT.h"
#include "carImage.h"

const uint8 *__TEAM_NAME__ = "GONGTU";

MENU_e   page_status = M_MAIN;
int option_key_status=0;                //��ȥ�İ���״̬�������ر�ʹ��
OPTION_e  old_option_status =1;         //�ɵ�ѡ�����ȷ��ˢ��һ��
OPTION_e  option_status = 1;            //�µ�ѡ��
uint8 *(opt_name[MENU_MAX][OPT_SIZE]) = {
                                          {"1.adc","2.Servo","3.Motor","4.I2C","5.CSB",""},        //���˵�
                                          
                                          {"1.value","2.L_V","3.sensor","4.","",""},         //�Ӳ˵�
                                          {"1.pwm","2.ERROR","3.START","4.DataUpdate","5.HDcanshu",""},
                                          {"1.PWM","2.START","3.test3","4.","",""},
                                          {"1.START","2.STOP","","","",""},
                                          {"1.START","2.STOP","","","",""},
                                          {"1.START","2.STOP","","","",""},
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                         
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
                                          {"","","","","",""},    //���Ӳ˵�
};

/*
*�˵���ʼ��
*�����������˵����Ӳ˵�
*/
void menu_init(){
  menu_showtitle();     //����
  for(OPTION_e i = OPT_0;i<OPT_SIZE;i++){
    menu_showoption(opt_name[page_status][i],0,i+1,RED,WHITE);  //����ÿҳ��ѡ���������ͱ�����ѡ��ʱ��������
  }
}

/*
*�˵�ˢ��
*@brief ���ǹؼ�KEY�������򲻲�����������Ҫ����ʹ�ܣ�
*/
void menu_update(){
  if(option_key_status == -1)return;
  /*if(((~KEY_ALL)&(1<<KEY_OK))&&(page_status == M_MAIN)&&(option_key_status != KEY_OK)){        //����ȷ���Ƿ�Ҫ����PAGE
    page_status = option_status+1;
    menu_init();        //���ĺ�ˢ��ҳ��
    return;
  }*/
  if(((~KEY_ALL)&(1<<KEY_OK))&&(option_key_status != KEY_OK)){        //����ȷ���Ƿ�Ҫ����PAGE //�Ӳ˵����������Ӳ˵�����ʹ��һ���㷨
    uint32 tem = page_status*6+option_status+1;
    if(tem<MENU_MAX){
      page_status = tem;
      menu_init();        //���ĺ�ˢ��ҳ��
    }
    return;
  }
  if(((~KEY_ALL)&(1<<KEY_B))&&(page_status != M_MAIN)&&(option_key_status != KEY_OK)){        //����
    page_status = (page_status-1)/6;
    menu_init();        //���ĺ�ˢ��ҳ��
    return;
  }
  old_option_status = option_status;                            //����ٸ���ѡ��ѡ��״̬
  if(((~KEY_ALL)&(1<<KEY_U))&&(option_key_status != KEY_U)){    //����ѡ��  // ִ�к�ر�ʹ�ܣ�������ֵ���ٱ仯
    if(option_status == 0)option_status = OPT_SIZE-1;
    else option_status --;
    option_key_status = KEY_U;
  }else if(((~KEY_ALL)&(1<<KEY_D))&&(option_key_status != KEY_D)){      //����ѡ��
    if(option_status == OPT_SIZE-1)option_status = 0;
    else option_status ++;
    option_key_status = KEY_D;
  }else if(!((~KEY_ALL)&(1<<KEY_D))&&!((~KEY_ALL)&(1<<KEY_U))&&!((~KEY_ALL)&(1<<KEY_L))&&!((~KEY_ALL)&(1<<KEY_R))&&!((~KEY_ALL)&(1<<KEY_OK))){         //��ʹ��
    option_key_status = KEY_MAX; //��ʾ���Ը���
  }
  if(old_option_status!=option_status){ //ֱ���������Ĳ�ˢ��
    lcd_showuint8(0,OPT_SIZE+1,option_status);   //��ʾѡ��״̬
    menu_showoption(opt_name[page_status][old_option_status],0,old_option_status+1,RED,WHITE);  //��ԭ����ѡ��״̬��λ
    menu_showoption(opt_name[page_status][option_status],0,option_status+1,RED,BLUE);   //�µ�ѡ��״̬��λ
  }
}

/*
*������ʾ
*
*/
void menu_showtitle(){
    lcd_changestyle(BLUE,GREEN);
    showimage(gImage_car,0,0,1);
    lcd_showstr(38,0,__TEAM_NAME__);
    showimage(gImage_car,100,0,0);
}
/*
*ѡ����ʾ
*@brief �ɸ�������͵�ǰ�еı���
*/
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