#include "systime.h"
#include "led.h"
#include "imu.h"
#include "work.h"
#include "math.h"

volatile u32 time2_tick;//Time5计数器
u32 time2_s;

void System_Time_Init(void)
{
	TIM2_Config();
}

/*
 * 函数名：TIM2_Config
 * 描述  ：TIM2配置 NVIC中断配置 2ms中断一次
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用	
 */
void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//设置NVIC中TIM2中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//通道设置为TIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//打开TIM2中断通道
	NVIC_Init(&NVIC_InitStructure);//写入配置
	
	//设置TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM2时钟
  //TIM_DeInit(TIM2);//TIM2初始化为缺省值

	TIM_TimeBaseStructure.TIM_Period=2000;//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);//时钟预分频数 72M/72
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//采样分频 TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//配置TIM2
    
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除溢出中断标志
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启溢出中断
	TIM_Cmd(TIM2,ENABLE);//开启TIM2外设
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);//先关闭TIM2时钟 等待使用
		
}

/*
 * 函数名：TIM2_Start
 * 描述  ：开启TIM2
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用	
 */
void TIM2_Start(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

/*
 * 函数名：TIM2_Stop
 * 描述  ：关闭TIM2
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用	
 */
void TIM2_Stop(void)
{
	TIM_Cmd(TIM2,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}

//*****TIM2中断函数2ms*****//
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//检测TIM2溢出中断是否发生
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
//			Rollerr=Rollnow*3.1415/180;   //转换成弧度
//			//Rolllast=Rollerr;
//			else Rollerr=-Rollnow*3.1415/180; 
//			
//			
//			E_highnow=0.6*9.8*0.6*(1-cos(Rollerr));//质量600g   重心L0.6m  g=9.8
//		//	if(E_highlast==0) E_highlast=E_highnow;
//			E_higherr=E_highlast-E_highnow;
//		//	E_highlast=E_highnow;;
//			
//			
//			Arc=Rollerr*2*0.8;       //万向节到6050的长度：0.8m
//			Force=E_higherr/Arc*1000/9.8;//此处变换+-号达到减速才加力的效果
//			if(Force>0)
//			F_PWM1=(Force+95)/0.0936;
//			else F_PWM1=1000;
//			
//			if(Rollnow>0)  {mot1= F_PWM1;mot2=1000;}
//			else {mot1=1000;mot2=F_PWM1;}
//		}
	
//	}
		
		
		
		
		
		
		if(time2_tick%5 == 0)//10ms  --  100HZ进行一次
		{
		
		}		
		if(time2_tick%25 == 0)//50ms	--	20HZ进行一次
		{
	
		}
		if(time2_tick%50 == 0)//100ms  --  10HZ进行一次
		{
			
		}
		if(time2_tick%500 == 0)//1000ms  --  1HZ进行一次
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










