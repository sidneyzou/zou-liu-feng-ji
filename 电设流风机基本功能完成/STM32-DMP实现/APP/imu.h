#ifndef __IMU_H
#define	__IMU_H

#include "stm32f10x.h"


	extern short gyro[3], accel[3], sensors;//陀螺仪存放数组，加速度存放数组，返回状态量
	extern float Yaw,Roll,Pitch;//欧拉角
	
void Read_mpu(void);

#endif
