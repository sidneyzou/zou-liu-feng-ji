#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
#include "OLED_I2C.h"
#include "led.h"
#include "OLED_display.h"
#include "work.h"
#include "imu.h"
#include "motor.h"
#include "systime.h"





void Data_Send_Status(float Pitch,float Roll,float Yaw);
void Send_Data(int16_t *Gyro,int16_t *Accel);

int main(void)
{		
	//u16 a=1050;
	
	NVIC_Configuration();//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	//����Բ�㲩ʿ��I2C����                           ���������ƽ��û��ʲô����
	ANBT_I2C_Configuration();		//IIC��ʼ��
	AnBT_Uart1_Send_String("I2C��ʼ���ɹ�",13);
//	delay_ms(30);
//AnBT_Uart1_Send_String("I2C��ʼ��",13);	
	AnBT_DMP_MPU6050_Init();			//6050DMP��ʼ�
	delay_init(72);
	LED_GPIO_Config();
	LED0(0);
	TIM3_PWM_Init(20000);
	delay_ms(1000);
	delay_ms(1000);
	TIM2_Config();
	
////	Set_Motor(1000,1000,1000,1000);
//	delay_ms(1000);
	

	I2C_Configuration();
	OLED_Init();
	Oled_Displayinit();
	//OLED_CLS();
	
	
//	Set_Motor(1000,1000,1000,1000);
//	while(1);
	Get_mode();
	while(1)                                
	{
		 RUN();
		 Read_mpu();
		
	}
}
