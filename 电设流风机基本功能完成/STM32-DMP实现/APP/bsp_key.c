/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h" 

/// ����ȷ����ʱ
static void Key_Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڣ�PB����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
	//����PB3Ϊ��ͨIO��
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
  * @brief   ����Ƿ��а�������
  * @param   ����Ķ˿ںͶ˿�λ
  *		@arg GPIOx: x�����ǣ�A...G�� 
  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��
  *   @arg Down_state:��������ʱ�ĵ�ƽ��1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,uint8_t Down_state)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Down_state ) 
	{	   
		/*��ʱ����*/
		Key_Delay(10000);		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == Down_state )  
		{	 
			/*�ȴ������ͷ� */
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
