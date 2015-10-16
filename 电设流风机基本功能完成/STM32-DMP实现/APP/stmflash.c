#include "stmflash.h"


//��һ����������
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

//�������������
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//д������ݵ�һ��ҳ
u8 STMFLASH_Write(u32 PageAddr,u16 *pBuffer,u16 NumToWrite)	
{
	
	FLASH_Status temp_status;//״̬
	u16 i;
	
	FLASH_Unlock();//����
	
	temp_status = FLASH_ErasePage(PageAddr);//����һ��ҳ
	if(temp_status != FLASH_COMPLETE)
	{
		FLASH_Lock();
		return 0;
	}
	
	for(i=0;i<NumToWrite;i++)//����д��
	{
		temp_status = FLASH_ProgramHalfWord(PageAddr+2*i,pBuffer[i]);
		if(temp_status != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 0;
		}
	}
	
	FLASH_Lock();//����
	return 1;
}






