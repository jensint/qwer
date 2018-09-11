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
#include "../../../Library/includes/delay.h"
#include "iic_master.h"
#include <intrins.h>
/*********************************************************************************************************************/
unsigned char *pI2CData;
unsigned char RegAddr;
unsigned char I2CDataLengh;
unsigned char I2CDataIndex;
bit iicEnd;
#if (MASTER_MODE == MASTER_READ)
bit rdSession;


unsigned char idata rxBuf[20];
#elif (MASTER_MODE == MASTER_WRITE)
unsigned char idata txBuf[20];
#endif

void INT6_ISR(void) interrupt 11 
{
	unsigned char Sta_Temp;

#if (MASTER_MODE == MASTER_WRITE)	
	if(I2CFLG & I2CF)					 //IIC  interrupt
	{			
		Sta_Temp = I2CSTA;	
		if(Sta_Temp == 0x08)			    //START condition detected
		{
			I2CDAT = I2C_ADDR;											
		}
		else if(Sta_Temp == 0x18)		    //Address + Write bit transmitted, ACK received
		{			
			I2CDAT = RegAddr;
			I2CDataIndex = 0;			
		}
		else if(Sta_Temp == 0x28)		   //Data byte transmitted/received in master mode, ACK received
		{
			if(I2CDataIndex >= I2CDataLengh)
			{
				I2CCON |= STP(1);	
				iicEnd = 1;	
			}
			else
			{
				I2CDAT = *(pI2CData +I2CDataIndex);	
				I2CDataIndex++;
			}								
		}
		I2CFLG  |= I2CF;
	}	
	
#elif (MASTER_MODE == MASTER_READ)		
	if(I2CFLG & I2CF)					 			//IIC中断
	{			
		Sta_Temp = I2CSTA;	
		if(Sta_Temp == 0x08)			    //检测到START信号
		{
			if(rdSession)
			{
				I2CDAT = I2C_ADDR|1;			//主机读阶段， 准备发送从机地址+读位
			}
			else
			{
				I2CDAT = I2C_ADDR;			 //主机写数据阶段，准备发送从机地址+写位
			}												
		}
		else if(Sta_Temp == 0x18)		    //Address + Write bit transmitted, ACK received
		{			
			I2CDAT	= RegAddr;					//发送完从机地址+写位后， 准备发送数据寄存器地址
		}
		else if(Sta_Temp == 0x28)		   //Data byte transmitted/received in master mode, ACK received
		{
			if(rdSession)
			{				
				if(I2CDataIndex >= I2CDataLengh)
				{
					I2CCON |= STP(1);				//所有数据读完， 准备发送STOP信号
					iicEnd = 1;
				}
				else
				{
					*(pI2CData+I2CDataIndex) = I2CDAT;
					I2CDataIndex++;
					if(I2CDataIndex <= (I2CDataLengh-1))
					{
						I2CCON |= AAK(1);								//如果不是最后一字节，预设ACK状态
					}
					else
					{
						I2CCON &= ~AAK(1);				//如果是最后一字节，不发送ACK
					}
				}										
			}
			else
			{
				I2CCON |= STA(1);	
				rdSession = 1;			
			}
		}
		else if(Sta_Temp == 0x30)		   //Data byte transmitted/received in master mode, ACK not received
		{
			if(rdSession)
			{				
				if(I2CDataIndex >= I2CDataLengh)
				{
					I2CCON |= STP(1);	
					iicEnd = 1;
				}										
			}
		}
		else if(Sta_Temp == 0x40)		    //Address + Read bit transmitted, ACK received
		{		
			I2CCON |= AAK(1);	
			I2CDataIndex=0;					
		}
		else if(Sta_Temp == 0x48)		    //Address + Read bit transmitted, no ACK received
		{		
			I2CCON |= AAK(1);	
			I2CDataIndex=0;		
		}	
		I2CFLG  |= I2CF;
	}
#endif

}					
#if (MASTER_MODE == MASTER_READ)
void IIC_MasterReadStart(unsigned char *pData,unsigned char regaddr,unsigned char lengh)
{
		iicEnd = 0;
		rdSession = 0;
		pI2CData 	= pData;
		RegAddr 	= regaddr;
		I2CDataLengh = lengh;
		I2CCON |= STA(1);	
}

#elif (MASTER_MODE == MASTER_WRITE)
void IIC_MasterWriteStart(unsigned char *pData,unsigned char regaddr,unsigned char lengh)
{
		iicEnd = 0;
		pI2CData 	= pData;
		RegAddr 	= regaddr;
		I2CDataLengh = lengh;
		I2CCON |= STA(1);	
}
#endif
void WaitI2CEnd(void)
{
	while(!iicEnd);
	iicEnd = 0;
}
void IIC_Master_MainLoop(void)
{
#if (IC_TYPE == IC_TYPE_CA51F2X)
	I2C_SelectComPin(I2C_SEL_P36_P37);
#elif (IC_TYPE == IC_TYPE_CA51F3X)
  I2C_SelectComPin(I2C_SEL_P30_P31);
#endif
	
	I2C_init(I2CE_enable,I2CIE_enable,STFE_enable,I2C_ADDR,20);
	
	INT6EN = 1;	 
	while(1)
	{
		unsigned char i;
#if (MASTER_MODE == MASTER_READ)
		
		IIC_MasterReadStart(rxBuf,0,20);
		WaitI2CEnd();
		
		Uart0_PutChar(0xaa);
		for(i=0;i<I2CDataLengh;i++)
		{
			Uart0_PutChar(rxBuf[i]);
		}
		Delay_ms(10);
#elif (MASTER_MODE == MASTER_WRITE)
		for(i = 0; i < 20; i++)
		{
			txBuf[i] = i;
		}
		IIC_MasterWriteStart(txBuf,0,20);
		WaitI2CEnd();
		Delay_ms(10);
#endif

	}
}
#endif
