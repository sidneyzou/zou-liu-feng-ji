#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stm32f10x.h"


extern u16 moto1,moto2,moto3,moto4;
#define Gyro_Gain 		2000/32767;		//���������̣�+-2000��ÿ��
#define Gyro_GainR 		0.0010653;		//���������̣�+-2000����ÿ��
#define Acc_Gain 			2/32767;			//���ٶȼ����̣�+-2g


//����PID
typedef struct
{
	float P;
	float pout;
	
	float I;
	float IMAX;
	float iout;
	
	float D;
	float dout;
	
	float OUT;
}PID;

extern u8 ARMED;
extern PID PID_ROL,PID_PIT,PID_YAW;


void PID_Init(void);

void CONTROL(float rol_now, float pit_now, float yaw_now, u16 throttle, float rol_tar, float pit_tar, s16 yaw_gyro_tar);


u8 Is_Armed(u16 CH3,u16 CH4);//������С CH4��С
u8 Is_DisArmed(u16 CH3,u16 CH4);//������С CH4���

void PID_WriteFlash(void);//д��PIDֵ
void PID_ReadFlash(void);//��ȡFlash��ĵ�PIDֵ

#endif


