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
		case 0:OLED_ShowStr(0,2,"   Fixed line   ",2);break;      //��һ�����񣺲�����50cmֱ�߶�
		case 1:OLED_ShowStr(0,2," change Length  ",2);break;			 //�ڶ������񣺹̶�����30-60cmֱ��
		case 2:OLED_ShowStr(0,2,"change direction",2);break;			 //�������������ⷽ�򣬴���20cmֱ��
		case 3:OLED_ShowStr(0,2,"   Standstill   ",2);break;			 //���Ĳ���������Ƕȣ����Դﵽ��ֹ
		case 4:OLED_ShowStr(0,2,"  Draw a circle ",2);break;			 //���岽����: �ظ����ο����ð뾶15-35��Բ����
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
