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
说明：
在本应用示例中，划分FLASH的最后一块（256字节）为数据空间,NeedMemoryData是掉电需要保存的10字节数据，初始值为1~10，每次
NeedMemoryData数组数据改变时，同时更新到数据空间。下次重上电时从数据空间加载数据到NeedMemoryData数组，从面实现掉电记忆功能。

*******************************************************************************************************************/
unsigned char NeedMemoryData[10];
void MemoryDataWrite(void)
{
	unsigned char DataAreaBuf[12];
	DataAreaBuf[0] = 0xAA;			//填写标志字，当数据空间地址0、1的数据为0xAA55时， 表示数据空间的数据是用户写入的，数据有效。
	DataAreaBuf[1] = 0x55;			

	DataAreaBuf[2] = NeedMemoryData[0];			//填写掉电需要保存的数据
	DataAreaBuf[3] = NeedMemoryData[1];
	DataAreaBuf[4] = NeedMemoryData[2];
	DataAreaBuf[5] = NeedMemoryData[3];
	DataAreaBuf[6] = NeedMemoryData[4];
	DataAreaBuf[7] = NeedMemoryData[5];
	DataAreaBuf[8] = NeedMemoryData[6];
	DataAreaBuf[9] = NeedMemoryData[7];
	DataAreaBuf[10] = NeedMemoryData[8];
	DataAreaBuf[11] = NeedMemoryData[9];

	Data_Area_Sector_Erase(0);							//擦除数据空间第一个扇区，注意：扇区号对应的是数据空间的扇区号，不是整个FLASH空间的扇区号
	Data_Area_Mass_Write(0,DataAreaBuf,12);	//写入数据
}
void MemoryDataRead(void)
{
	unsigned char  DataAreaBuf[12];

	Data_Area_Mass_Read(0,DataAreaBuf,12);	//读取保存的数据

	NeedMemoryData[0] = DataAreaBuf[2];			//把读取的数据装载到变量
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
	PADRD = FLASH_SIZE*4 - 1;								//划分数据空间，本例程划分FLASH最后一块（256字节）为数据空间
	Data_Area_Mass_Read(0,DataAreaBuf,2);		//读取状态字
	if((DataAreaBuf[0] != 0xAA)||(DataAreaBuf[1] != 0x55))//如果状态字不是0xAA55,表示是初次上电，需要初始化变量并存入数据空间
	{
		NeedMemoryData[0] = 1;								//初始化需要掉电保存的变量
		NeedMemoryData[1] = 2;
		NeedMemoryData[2] = 3;
		NeedMemoryData[3] = 4;
		NeedMemoryData[4] = 5;
		NeedMemoryData[5] = 6;
		NeedMemoryData[6] = 7;
		NeedMemoryData[7] = 8;
		NeedMemoryData[8] = 9;
		NeedMemoryData[9] = 10;	

		MemoryDataWrite();				//写入状态字和需掉电存储的变量到数据空间
	}
	else
	{
		MemoryDataRead();					//如果不是初次上电，从数据空间直接读取数据并赋值给需掉电存储的变量
	}
}
void PoweroffMemory_MainLoop(void)
{
	unsigned char i;
	PoweroffMemory_init();			//初始化掉电存储功能
	
	
/************************************************************************************************************************
当需要掉电存储的变量数值改变时，需要同时更新到数据空间，在本例程中，每次上电需要掉电存储的数据加一递增。	
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
