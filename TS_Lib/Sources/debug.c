#ifndef _I2C_C_
#define _I2C_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "includes\ca51f2sfr.h"
#include "includes\ca51f2xsfr.h"
#include "includes\gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "includes\ca51f3sfr.h"
#include "includes\ca51f3xsfr.h"
#include "includes\gpiodef_f3.h"
#endif
#include "Library\Includes\rtc.h"		
#include "Library\Includes\system_clock.h"		
/*********************************************************************************************************************/
#include "TS_Lib\Includes\ts_configuration.h"
#include "TS_Lib\Includes\ts_def.h"
#include "TS_Lib\Includes\ts_api.h"
#include "TS_Lib\Includes\ts_service.h"		   
/*********************************************************************************************************************/
#if DEBUG
unsigned char xdata TouchDebugID = 'D';
unsigned char rxOffset;
unsigned char regAddr;
bit iicReadMode;
bit iicEnd;
code unsigned char *TS_INFO[]=
{
	&TouchDebugID,
	&TS_State,
	&TS_Cnt,
	&TS_RefChBaseLineData,
	&TS_RefChRawData,
	&KeysFlagSN,
	&TSKey_BaseLineData[0],
	&TSKey_PostData[0],
	&TSKey_FingerThd[0],
	&TS_CH[0],
};
unsigned char *pTS_Info;
void I2C_ISR(void) interrupt 11 // using 1
{
	unsigned char Sta_Temp;
	if(I2CFLG & I2CF)					 
	{						
		Sta_Temp = I2CSTA;			
		if(Sta_Temp == 0x60)			    //接收到从机地址+写位
		{			
			rxOffset = 0xff;
			iicReadMode = 0;
			I2CCON |= AAK(1);			
		}
		else if(Sta_Temp == 0x80)			   
		{					
			if(iicReadMode)	 				//发送一字节数据
			{
				rxOffset++;
				I2CDAT = *(pTS_Info+rxOffset);			
			}
			else							//接收到一字节数据
			{				
				if(rxOffset == 0xff) 		//地址
				{
					regAddr=I2CDAT;	 
					pTS_Info = TS_INFO[regAddr];
			   		rxOffset=0;
					I2CCON |= AAK(1);
				}
				else				   		//数据
				{
					*(pTS_Info+rxOffset) = I2CDAT;
					rxOffset++;	
					I2CCON |= AAK(1);		
				}
			}	
		}
		else if(Sta_Temp==0xA8)				//接收到从机地址+读位，发送ACK信号
		{								
			I2CDAT = *(pTS_Info+rxOffset);
			iicReadMode = 1;
		}
		else if(Sta_Temp == 0x88)	
		{
		}
		I2CFLG  |= I2CF;
	}	
	if(I2CFLG & I2CSTP)
	{
		I2CFLG  |= I2CSTP;	
		iicEnd = 1;	
	}
}
void I2C_init(void)
{
#if (IC_TYPE == IC_TYPE_CA51F3X)
	I2CIOS = 0;
	GPIO_Init(P30F,P30_I2C_SDA_SETTING | PU_EN);
	GPIO_Init(P31F,P31_I2C_SCL_SETTING | PU_EN);

// 	I2CIOS = 1;
// 	GPIO_Init(P11F,P11_I2C_SDA_SETTING | PU_EN);
// 	GPIO_Init(P12F,P12_I2C_SCL_SETTING | PU_EN);
#elif  (IC_TYPE == IC_TYPE_CA51F2X)
//	I2CIOS = 0;
//	GPIO_Init(P36F,P36_I2C_SDA_SETTING | PU_EN);
//	GPIO_Init(P37F,P37_I2C_SCL_SETTING | PU_EN);

	I2CIOS = 1;
	GPIO_Init(P30F,P30_I2C_SDA_SETTING | PU_EN);
	GPIO_Init(P31F,P31_I2C_SCL_SETTING | PU_EN);
//
//
//	I2CIOS = 2;
//	GPIO_Init(P66F,P66_I2C_SDA_SETTING | PU_EN);
//	GPIO_Init(P67F,P67_I2C_SCL_SETTING | PU_EN);
//
//	I2CIOS = 3;
//	GPIO_Init(P61F,P61_I2C_SDA_SETTING | PU_EN);
//	GPIO_Init(P60F,P60_I2C_SCL_SETTING | PU_EN);
#endif


	I2CCON = I2CE(1) | I2CIE(1) | STA(0) | STP(0)| CKHD(0) | AAK(1)| CBSE(0) | STFE(0);		
	I2CADR = (I2C_ADDR>>1);

	INT6EN = 1;
	EXIP |= (1<<4);			//优先级设置为最高
	iicEnd = 0;
}
#endif
#endif