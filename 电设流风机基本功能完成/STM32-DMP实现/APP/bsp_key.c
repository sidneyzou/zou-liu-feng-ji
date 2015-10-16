/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h" 

/// 不精确的延时
static void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口（PB）的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
	//设置PB3为普通IO口
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

/**
  * @brief   检测是否有按键按下
  * @param   具体的端口和端口位
  *		@arg GPIOx: x可以是（A...G） 
  *		@arg GPIO_PIN 可以是GPIO_PIN_x（x可以是1...16）
  *   @arg Down_state:按键按下时的电平，1为高电平，0为低电平
  * @retval  按键的状态
  *		@arg KEY_ON:按键按下
  *		@arg KEY_OFF:按键没按下
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,uint8_t Down_state)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Down_state ) 
	{	   
		/*延时消抖*/
		Key_Delay(10000);		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Down_state )  
		{	 
			/*等待按键释放 */
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Down_state);   
			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}

uint8_t Read_Key()
{
	u8 key;
	if(KEY_ON==Key_Scan(GPIOA,GPIO_Pin_5,0))  key=1;
	else if(KEY_ON==Key_Scan(GPIOB,GPIO_Pin_12,0)) key=2;
	else if(KEY_ON==Key_Scan(GPIOC,GPIO_Pin_4,0))  key=3;
	else if(KEY_ON==Key_Scan(GPIOC,GPIO_Pin_5,0))  key=4;
	else if(KEY_ON==Key_Scan(GPIOC,GPIO_Pin_6,0))  key=5;
	else if(KEY_ON==Key_Scan(GPIOC,GPIO_Pin_7,0))  key=6;
	else key=0;
	
	return key;
	
}
/*********************************************END OF FILE**********************/
