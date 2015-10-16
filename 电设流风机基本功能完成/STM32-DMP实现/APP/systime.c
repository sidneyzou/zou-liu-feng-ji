#include "systime.h"
#include "led.h"
#include "imu.h"
#include "work.h"
#include "math.h"

volatile u32 time2_tick;//Time5������
u32 time2_s;

void System_Time_Init(void)
{
	TIM2_Config();
}

/*
 * ��������TIM2_Config
 * ����  ��TIM2���� NVIC�ж����� 2ms�ж�һ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����	
 */
void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//����NVIC��TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//ͨ������ΪTIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//��TIM2�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//д������
	
	//����TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��
  //TIM_DeInit(TIM2);//TIM2��ʼ��Ϊȱʡֵ

	TIM_TimeBaseStructure.TIM_Period=2000;//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);//ʱ��Ԥ��Ƶ�� 72M/72
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//������Ƶ TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//����TIM2
    
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//�������жϱ�־
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//��������ж�
	TIM_Cmd(TIM2,ENABLE);//����TIM2����
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);//�ȹر�TIM2ʱ�� �ȴ�ʹ��
		
}

/*
 * ��������TIM2_Start
 * ����  ������TIM2
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����	
 */
void TIM2_Start(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

/*
 * ��������TIM2_Stop
 * ����  ���ر�TIM2
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����	
 */
void TIM2_Stop(void)
{
	TIM_Cmd(TIM2,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}

//*****TIM2�жϺ���2ms*****//
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//���TIM2����ж��Ƿ���
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
		time2_tick++;
		time2_tick = time2_tick%1000;//2HZ
		
		if(gyro[0]>-10&&gyro[0 ]<10) 
		{
			rollmax=Roll;
		}
		
//		if(gyro[0]>-10||gyro[0 ]<10) 
//		{
//			Rollmax=Roll;
//		}
//		if(Rollmax>10) flag=0;
//		if(flag_still)
//		{
//		
//			if(gyro[0]>-10||gyro[0]<10) 
//			{
//				Rollnow=Roll;
//		//	if(Rolllast==0) Rolllast=Rollnow;
//			
//			if(Rollnow>0)
//			Rollerr=Rollnow*3.1415/180;   //ת���ɻ���
//			//Rolllast=Rollerr;
//			else Rollerr=-Rollnow*3.1415/180; 
//			
//			
//			E_highnow=0.6*9.8*0.6*(1-cos(Rollerr));//����600g   ����L0.6m  g=9.8
//		//	if(E_highlast==0) E_highlast=E_highnow;
//			E_higherr=E_highlast-E_highnow;
//		//	E_highlast=E_highnow;;
//			
//			
//			Arc=Rollerr*2*0.8;       //����ڵ�6050�ĳ��ȣ�0.8m
//			Force=E_higherr/Arc*1000/9.8;//�˴��任+-�Ŵﵽ���ٲż�����Ч��
//			if(Force>0)
//			F_PWM1=(Force+95)/0.0936;
//			else F_PWM1=1000;
//			
//			if(Rollnow>0)  {mot1= F_PWM1;mot2=1000;}
//			else {mot1=1000;mot2=F_PWM1;}
//		}
	
//	}
		
		
		
		
		
		
		if(time2_tick%5 == 0)//10ms  --  100HZ����һ��
		{
		
		}		
		if(time2_tick%25 == 0)//50ms	--	20HZ����һ��
		{
	
		}
		if(time2_tick%50 == 0)//100ms  --  10HZ����һ��
		{
			
		}
		if(time2_tick%500 == 0)//1000ms  --  1HZ����һ��
		{
//				time2_s++;
//				time2_s = time2_s%1000;
//				if(time2_s%2==0)
//				{
//					if(Rollmax>10) flag=0;
//					else flag=1;					
//					Rollmax=0;
//				}
		}	
		

	}		
}










