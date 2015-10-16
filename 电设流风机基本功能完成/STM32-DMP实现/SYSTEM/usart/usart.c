#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
////////////////////////////////////////////////////////////////////////////////// 	  


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
//����״̬
//bit7��������ɱ�־
//bit6�����յ�0x0d
//bit5~0�����յ�����Ч�ֽ���Ŀ
u8 USART_RX_STA=0;       //����״̬���

void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
  
   //USART ��ʼ������
   
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
   

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
   
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x80)==0)//����δ���
			{
			if(USART_RX_STA&0x40)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x80;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x40;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>63)USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
} 

void AnBT_Uart1_Send_Char(unsigned char ascii_code) 		//Բ�㲩ʿ:����һ���ַ�
{
	USART_SendData(USART1,ascii_code);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								//Բ�㲩ʿ:�ȴ�ֱ���������
}

void AnBT_Uart1_Send_String(unsigned char* str_buf , unsigned char str_len)		//Բ�㲩ʿ:����һ��ָ�����ȵ��ַ���
{
	unsigned char i;
	if(str_len>AnBT_COM_Buf_Length) str_len=AnBT_COM_Buf_Length;
	AnBT_Uart1_Send_Char(13);																	//Բ�㲩ʿ:���ͻس��ַ�
// 	AnBT_Uart1_Send_Char(':');  															//Բ�㲩ʿ:�����ַ�:
  for(i=0;i<str_len;i++) AnBT_Uart1_Send_Char(str_buf[i]); 	//Բ�㲩ʿ:�����ַ�:
// 	AnBT_Uart1_Send_Char('/');																//Բ�㲩ʿ:�����ַ�/
	AnBT_Uart1_Send_Char(13);																	//Բ�㲩ʿ:���ͻس��ַ�
}

void AnBT_Uart1_Send_Num(unsigned char number) 	//Բ�㲩ʿ:����һ���ַ�
{
	unsigned char num_low,num_high;
	num_low=number&0x0f;													//Բ�㲩ʿ:ȡ���ݵ�λ
	num_high=(number&0xf0)>>4;										//Բ�㲩ʿ:ȡ���ݸ�λ
	if(num_high<10)USART_SendData(USART1,num_high+48);
	else USART_SendData(USART1,num_high+55);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								//Բ�㲩ʿ:�ȴ�ֱ���������
	if(num_low<10)USART_SendData(USART1,num_low+48);
	else USART_SendData(USART1,num_low+55);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}								//Բ�㲩ʿ:�ȴ�ֱ���������
}

void AnBT_Uart1_Send_Nums(unsigned char* nums_buf , unsigned char nums_len)		//Բ�㲩ʿ:����һ��ָ�����ȵ��ַ���
{
	unsigned char i;
	if(nums_len>AnBT_COM_Buf_Length) nums_len=AnBT_COM_Buf_Length;
	AnBT_Uart1_Send_Char(13);																										//Բ�㲩ʿ:���ͻس��ַ�
	AnBT_Uart1_Send_Char(':');  																								//Բ�㲩ʿ:�����ַ�:
  for(i=0;i<nums_len;i++) AnBT_Uart1_Send_Num(nums_buf[nums_len-i-1]); 				//Բ�㲩ʿ:��������
	AnBT_Uart1_Send_Char('/');																									//Բ�㲩ʿ:�����ַ�/
}
