#ifndef __IMU_H
#define	__IMU_H

#include "stm32f10x.h"


	extern short gyro[3], accel[3], sensors;//�����Ǵ�����飬���ٶȴ�����飬����״̬��
	extern float Yaw,Roll,Pitch;//ŷ����
	
void Read_mpu(void);

#endif
