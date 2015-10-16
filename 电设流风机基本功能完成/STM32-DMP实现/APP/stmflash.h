#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f10x.h"

#define PAGE1 0x0801F800	//255ҳ

#define ADJUST_FLAG 0xF00F 


void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 STMFLASH_Write(u32 PageAddr,u16 *pBuffer,u16 NumToWrite);//��ָ����ַ��ʼд��ָ�����ȵ�����



#endif





