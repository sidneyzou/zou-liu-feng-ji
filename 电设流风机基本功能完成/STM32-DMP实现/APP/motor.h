#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f10x.h"

void TIM3_PWM_Init(u16 times);
void Motor_Init(u16 rate);//���PWM��ʼ�������õ����ˢ������50-499
void Set_Motor(u16 M1,u16 M2,u16 M3,u16 M4);//���Ƶ��ת��//1000-2000
void Set_Servo(u16 S1,u16 S2,u16 S3,u16 S4);//���ƶ���Ƕ�//0-180

#endif


