#include "led.h"


/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����IO������ �ṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��GPIOCʱ��


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//ѡ������13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO����ٶ�50Hz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//����GPIOC_13����
	

	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11);//PC13��λ

}






