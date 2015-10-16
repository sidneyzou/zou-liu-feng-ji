#ifndef __USART_H
#define __USART_H
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 
extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA;         //接收状态标记	

#define AnBT_COM_Buf_Length	64

void uart_init(u32 bound);
void AnBT_Uart1_Send_Char(unsigned char ascii_code);
void AnBT_Uart1_Send_String(unsigned char* str_buf , unsigned char str_len);
void AnBT_Uart1_Send_Num(unsigned char number);
void AnBT_Uart1_Send_Nums(unsigned char* nums_buf , unsigned char nums_len);
#endif
