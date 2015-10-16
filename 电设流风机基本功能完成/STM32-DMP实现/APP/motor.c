#include "motor.h"

//TIM3  PWM  ���ͨ����
//channel1  channel2  channel3  channel4
//PA6       PA7       PB0       PB1

/*
 * ��������TIM3_PWM_Init
 * ����  ��TIM3_PWM�������
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM3_PWM_Init(u16 times)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	//TIM3_PWM IO������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//GPIOA GPIOB ʱ�ӿ���

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//����������� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOA 6 7������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIOB 0 1 ������ 


	//TIM3_PWM����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3 ʱ�ӿ���

	TIM_TimeBaseStructure.TIM_Period = times-1;//20000��Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//ʱ��Ԥ��Ƶ�� 72M/72(1M/20000) = 50HZ
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ(0->?)
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//����TIM3

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����ΪPWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
 	TIM_OCInitStructure.TIM_Pulse = 0;//��������ֵ
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

 	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
 	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��ͨ��1

	/* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��ͨ��2

	/* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��ͨ��3

	/* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ��ͨ��4
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);//ʹ��TIM3���ؼĴ���ARR
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);//ʹ�ܶ�ʱ��3
	
	Set_Motor(1000,1000,1000,1000);
//	Set_Motor(2000,2000,2000,2000);    �����г�У��
}

//�����ˢ������50-499
void Motor_Init(u16 rate)
{
	u16 temp = rate;
	
	if(temp>499)//��Ƶ�ʽ�������
	{
		temp = 499;
	}
	if(temp<50)
	{
		temp =50;
	}
	
	TIM3_PWM_Init(1000000/temp);
}


//���Ƶ��ת��
//1000-2000
void Set_Motor(u16 M1,u16 M2,u16 M3,u16 M4)
{
		TIM_SetCompare1(TIM3,M1);//1000-2000,1ms-2ms
		TIM_SetCompare2(TIM3,M2);
		TIM_SetCompare3(TIM3,M3);
		TIM_SetCompare4(TIM3,M4);
}

//���ƶ���Ƕ�
//0-180
void Set_Servo(u16 S1,u16 S2,u16 S3,u16 S4)
{
	if(S1>180 || S2>180 || S3>180 || S4>180) return;
	else
	{
		TIM_SetCompare1(TIM3,(float)S1*11.111+500);//500-2500,0.5ms-2.5ms
		TIM_SetCompare2(TIM3,(float)S2*11.111+500);
		TIM_SetCompare3(TIM3,(float)S3*11.111+500);
		TIM_SetCompare4(TIM3,(float)S4*11.111+500);
	}
}


