#include "OLED_I2C.h"
#include "delay.h"
#include "OLED_display.h"
#include "bsp_key.h" 
#include "work.h"


void Oled_Displayinit()
{
	OLED_Fill(0);
	OLED_ShowStr(16,0,"Select mode",2);
	OLED_ShowStr(0,2,"  Fixed line  ",2);
	OLED_ShowStr(0,7,"OK             NEST",1);
}
void Oled_Display()
{
	switch(mode)
	{
		case 0:OLED_ShowStr(0,2,"   Fixed line   ",2);break;      //第一步任务：不短于50cm直线段
		case 1:OLED_ShowStr(0,2," change Length  ",2);break;			 //第二部任务：固定方向30-60cm直线
		case 2:OLED_ShowStr(0,2,"change direction",2);break;			 //第三部任务：任意方向，大于20cm直线
		case 3:OLED_ShowStr(0,2,"   Standstill   ",2);break;			 //第四步任务：拉起角度，可以达到静止
		case 4:OLED_ShowStr(0,2,"  Draw a circle ",2);break;			 //第五步任务: 重复三次可设置半径15-35的圆环内
		case 5:OLED_ShowStr(0,2,"  Draw a circle2",2);break;			 //
		case 6:OLED_ShowStr(0,2,"     Other      ",2);break;
		
	}
	
}


void mode2_Display(u8 lenth)
{
	OLED_ShowStr(0,4,"                ",2);
	OLED_ShowStr(0,4,"lenth:       cm",2);
	OLED_ShowNum(lenth);
}

void mode3_Display(u8 angle)
{
	OLED_ShowStr(0,4,"                ",2);
	OLED_ShowStr(0,4,"angle:       du",2);
	OLED_ShowNum(angle);
	
}

void mode5_Display(u8 r)
{
	OLED_ShowStr(0,4,"             cm",2);
	OLED_ShowStr(0,4,"R:",2);
	OLED_ShowNum(r);
}
void clear()
{
	OLED_ShowStr(0,4,"                ",2);
}
