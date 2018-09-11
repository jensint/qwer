#ifndef _POWEROFF_MEMORY_C_
#define _POWEROFF_MEMORY_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "../../includes/ca51f2sfr.h"
#include "../../includes/ca51f2xsfr.h"
#include "../../includes/gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "../../includes/ca51f3sfr.h"
#include "../../includes/ca51f3xsfr.h"
#include "../../includes/gpiodef_f3.h"
#endif
#include "../../includes/system.h"
#include "../../Library/includes/uart.h"
#include "../../Library/includes/flash.h"
#include "poweroff_memory.h"
#include <intrins.h>
/*********************************************************************************************************************/
/******************************************************************************************************************
˵����
�ڱ�Ӧ��ʾ���У�����FLASH�����һ�飨256�ֽڣ�Ϊ���ݿռ�,NeedMemoryData�ǵ�����Ҫ�����10�ֽ����ݣ���ʼֵΪ1~10��ÿ��
NeedMemoryData�������ݸı�ʱ��ͬʱ���µ����ݿռ䡣�´����ϵ�ʱ�����ݿռ�������ݵ�NeedMemoryData���飬����ʵ�ֵ�����书�ܡ�

*******************************************************************************************************************/
unsigned char NeedMemoryData[10];
void MemoryDataWrite(void)
{
	unsigned char DataAreaBuf[12];
	DataAreaBuf[0] = 0xAA;			//��д��־�֣������ݿռ��ַ0��1������Ϊ0xAA55ʱ�� ��ʾ���ݿռ���������û�д��ģ�������Ч��
	DataAreaBuf[1] = 0x55;			

	DataAreaBuf[2] = NeedMemoryData[0];			//��д������Ҫ���������
	DataAreaBuf[3] = NeedMemoryData[1];
	DataAreaBuf[4] = NeedMemoryData[2];
	DataAreaBuf[5] = NeedMemoryData[3];
	DataAreaBuf[6] = NeedMemoryData[4];
	DataAreaBuf[7] = NeedMemoryData[5];
	DataAreaBuf[8] = NeedMemoryData[6];
	DataAreaBuf[9] = NeedMemoryData[7];
	DataAreaBuf[10] = NeedMemoryData[8];
	DataAreaBuf[11] = NeedMemoryData[9];

	Data_Area_Sector_Erase(0);							//�������ݿռ��һ��������ע�⣺�����Ŷ�Ӧ�������ݿռ�������ţ���������FLASH�ռ��������
	Data_Area_Mass_Write(0,DataAreaBuf,12);	//д������
}
void MemoryDataRead(void)
{
	unsigned char  DataAreaBuf[12];

	Data_Area_Mass_Read(0,DataAreaBuf,12);	//��ȡ���������

	NeedMemoryData[0] = DataAreaBuf[2];			//�Ѷ�ȡ������װ�ص�����
	NeedMemoryData[1] = DataAreaBuf[3];
	NeedMemoryData[2] = DataAreaBuf[4];
	NeedMemoryData[3] = DataAreaBuf[5];
	NeedMemoryData[4] = DataAreaBuf[6];
	NeedMemoryData[5] = DataAreaBuf[7];
	NeedMemoryData[6] = DataAreaBuf[8];
	NeedMemoryData[7] = DataAreaBuf[9];
	NeedMemoryData[8] = DataAreaBuf[10];
	NeedMemoryData[9] = DataAreaBuf[11];
}

void PoweroffMemory_init(void)
{
	unsigned char  DataAreaBuf[2];
	PADRD = FLASH_SIZE*4 - 1;								//�������ݿռ䣬�����̻���FLASH���һ�飨256�ֽڣ�Ϊ���ݿռ�
	Data_Area_Mass_Read(0,DataAreaBuf,2);		//��ȡ״̬��
	if((DataAreaBuf[0] != 0xAA)||(DataAreaBuf[1] != 0x55))//���״̬�ֲ���0xAA55,��ʾ�ǳ����ϵ磬��Ҫ��ʼ���������������ݿռ�
	{
		NeedMemoryData[0] = 1;								//��ʼ����Ҫ���籣��ı���
		NeedMemoryData[1] = 2;
		NeedMemoryData[2] = 3;
		NeedMemoryData[3] = 4;
		NeedMemoryData[4] = 5;
		NeedMemoryData[5] = 6;
		NeedMemoryData[6] = 7;
		NeedMemoryData[7] = 8;
		NeedMemoryData[8] = 9;
		NeedMemoryData[9] = 10;	

		MemoryDataWrite();				//д��״̬�ֺ������洢�ı��������ݿռ�
	}
	else
	{
		MemoryDataRead();					//������ǳ����ϵ磬�����ݿռ�ֱ�Ӷ�ȡ���ݲ���ֵ�������洢�ı���
	}
}
void PoweroffMemory_MainLoop(void)
{
	unsigned char i;
	PoweroffMemory_init();			//��ʼ������洢����
	
	
/************************************************************************************************************************
����Ҫ����洢�ı�����ֵ�ı�ʱ����Ҫͬʱ���µ����ݿռ䣬�ڱ������У�ÿ���ϵ���Ҫ����洢�����ݼ�һ������	
**************************************************************************************************************************/
	for(i = 0; i < 10; i++)
	{
		NeedMemoryData[i]++;
	}
	MemoryDataWrite();	
/************************************************************************************************************************/
	
#ifdef PRINT_EN
	uart_printf("NeedMemoryData[0] = 0x%x\n",(unsigned int)NeedMemoryData[0]);
	uart_printf("NeedMemoryData[1] = 0x%x\n",(unsigned int)NeedMemoryData[1]);
	uart_printf("NeedMemoryData[2] = 0x%x\n",(unsigned int)NeedMemoryData[2]);
	uart_printf("NeedMemoryData[3] = 0x%x\n",(unsigned int)NeedMemoryData[3]);
	uart_printf("NeedMemoryData[4] = 0x%x\n",(unsigned int)NeedMemoryData[4]);
	uart_printf("NeedMemoryData[5] = 0x%x\n",(unsigned int)NeedMemoryData[5]);
	uart_printf("NeedMemoryData[6] = 0x%x\n",(unsigned int)NeedMemoryData[6]);
	uart_printf("NeedMemoryData[7] = 0x%x\n",(unsigned int)NeedMemoryData[7]);
	uart_printf("NeedMemoryData[8] = 0x%x\n",(unsigned int)NeedMemoryData[8]);
	uart_printf("NeedMemoryData[9] = 0x%x\n",(unsigned int)NeedMemoryData[9]);
#endif	
		
	while(1)
	{

	}
}
#endif
