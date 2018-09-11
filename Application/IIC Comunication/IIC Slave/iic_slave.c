#ifndef _WATCH_DOG_C_
#define _WATCH_DOG_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "../../../includes/ca51f2sfr.h"
#include "../../../includes/ca51f2xsfr.h"
#include "../../../includes/gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "../../../includes/ca51f3sfr.h"
#include "../../../includes/ca51f3xsfr.h"
#include "../../../includes/gpiodef_f3.h"
#endif
#include "../../../includes/system.h"
#include "../../../Library/includes/uart.h"
#include "../../../Library/includes/i2c.h"
#include "iic_slave.h"
#include <intrins.h>
/*********************************************************************************************************************/
unsigned char I2CDataIndex;
unsigned char regAddr;
bit iicReadMode;
bit iicEnd;
unsigned char xdata rxBuf[20];
unsigned char *pI2C_Data;

void INT6_ISR(void) interrupt 11 
{
	unsigned char Sta_Temp;

	if(I2CFLG & I2CF)					 //IIC  interrupt
	{						
		Sta_Temp = I2CSTA;			
		if(Sta_Temp == 0x60)			    //接收到从机地址+写位
		{			
			I2CDataIndex = 0xFF;
			iicReadMode = 0;
			I2CCON |= AAK(1);			
		}
		else if(Sta_Temp == 0x80)			   
		{					
			if(iicReadMode)	 	//发送一字节数据
			{
				I2CDataIndex++;
				I2CDAT = *(pI2C_Data + I2CDataIndex + regAddr);			
			}
			else						//接收到一字节数据
			{				
				if(I2CDataIndex == 0xFF) 	//地址
				{
					regAddr=I2CDAT;	 
			   	I2CDataIndex=0;
					I2CCON |= AAK(1);
				}
				else				   	//数据
				{
					*(pI2C_Data + I2CDataIndex + regAddr) = I2CDAT;
					I2CDataIndex++;	
					I2CCON |= AAK(1);		
				}
			}	
		}
		else if(Sta_Temp==0xA8)				//接收到从机地址+读位，发送ACK信号
		{								
			I2CDAT = *(pI2C_Data + I2CDataIndex + regAddr);
			iicReadMode = 1;
		}
		else if(Sta_Temp == 0x88)	
		{
		}
		I2CFLG  |= I2CF;
	}	
}											
void IIC_Slave_MainLoop(void)
{
	unsigned char i;
	
#if (IC_TYPE == IC_TYPE_CA51F2X)
	I2C_SelectComPin(I2C_SEL_P36_P37);
#elif (IC_TYPE == IC_TYPE_CA51F3X)
  I2C_SelectComPin(I2C_SEL_P30_P31);
#endif
	
	I2C_init(I2CE_enable,I2CIE_enable,STFE_disable,I2C_ADDR,0);
	INT6EN = 1;	 
	
	for(i = 0; i < 20; i++)
	{
		rxBuf[i] = 0x10+i;
	}
	
	pI2C_Data = rxBuf;
	
	while(1)
	{
		Uart0_PutChar(0xaa);
		for(i=0;i<20;i++)
		{
			Uart0_PutChar(rxBuf[i]);
		}
	}
}
#endif
