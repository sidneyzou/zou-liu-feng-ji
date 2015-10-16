#include "imu.h"
#include "work.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"

	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;//陀螺仪存放数组，加速度存放数组，返回状态量
	unsigned char more;
	long quat[4];//四元数存放数组
	float Yaw=0.00,Roll,Pitch;//欧拉角
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//计算姿态过程用到的变量
	
	#define q30  1073741824.0f
	
 #define BYTE0(dwTemp)       (*(char *)(&dwTemp))
 #define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
 #define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
 #define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


/*函数功能：根据匿名最新上位机协议写的显示姿态的程序
 *具体原理看匿名的讲解视频
 */

void Data_Send_Status(float Pitch,float Roll,float Yaw)
{
	unsigned char i=0;
	unsigned char _cnt=0,sum = 0;
	unsigned int _temp;
	u8 data_to_send[50];

	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(Roll*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0-(int)(Pitch*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	//和校验
	for(i=0;i<_cnt;i++)
		sum+= data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	//串口发送数据
	for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}

void Send_Data(int16_t *Gyro,int16_t *Accel)
{
	unsigned char i=0;
	unsigned char _cnt=0,sum = 0;
//	unsigned int _temp;
	u8 data_to_send[50];

	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	

	data_to_send[_cnt++]=BYTE1(Accel[0]);
	data_to_send[_cnt++]=BYTE0(Accel[0]);
	data_to_send[_cnt++]=BYTE1(Accel[1]);
	data_to_send[_cnt++]=BYTE0(Accel[1]);
	data_to_send[_cnt++]=BYTE1(Accel[2]);
	data_to_send[_cnt++]=BYTE0(Accel[2]);
	
	data_to_send[_cnt++]=BYTE1(Gyro[0]);
	data_to_send[_cnt++]=BYTE0(Gyro[0]);
	data_to_send[_cnt++]=BYTE1(Gyro[1]);
	data_to_send[_cnt++]=BYTE0(Gyro[1]);
	data_to_send[_cnt++]=BYTE1(Gyro[2]);
	data_to_send[_cnt++]=BYTE0(Gyro[2]);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	//和校验
	for(i=0;i<_cnt;i++)
		sum+= data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	//串口发送数据
	for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}


void ANO_DT_Send_MotoPWM()
{
	unsigned char i=0;
	unsigned char _cnt=0,sum = 0;
//	unsigned int _temp;
	u8 data_to_send[50];
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(mot1);
	data_to_send[_cnt++]=BYTE0(mot1);
	data_to_send[_cnt++]=BYTE1(mot2);
	data_to_send[_cnt++]=BYTE0(mot2);
	data_to_send[_cnt++]=BYTE1(mot3);
	data_to_send[_cnt++]=BYTE0(mot3);
	data_to_send[_cnt++]=BYTE1(mot4);
	data_to_send[_cnt++]=BYTE0(mot4);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	
	sum = 0;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	//串口发送数据
	for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}

	
void Read_mpu()
{
	 dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);		
		 if(sensors&INV_WXYZ_QUAT)
		 {
			 q0=quat[0] / q30;
			 q1=quat[1] / q30;
			 q2=quat[2] / q30;
			 q3=quat[3] / q30;
			 Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
			 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
			 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		//感觉没有价值，注掉
			 Data_Send_Status(Pitch,Roll,Yaw);
			 Send_Data(gyro,accel);
			 ANO_DT_Send_MotoPWM();
		 }	
}

