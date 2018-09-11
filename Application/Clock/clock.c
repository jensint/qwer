#ifndef _IRC_C_
#define _IRC_C_

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
#include "../../Library/includes/delay.h"
#include "../../Library/includes/system_clock.h"
#include "../../Library/includes/rtc.h"
#include "clock.h"
#include <intrins.h>
/*********************************************************************************************************************/
bit HalfSecFlag;
bit AlarmEvFlag;
void RTC_ISR (void) interrupt 13 	 
{
	if(RTCIF & RTC_MF)			//毫秒中断
	{
		RTCIF = RTC_MF;			
	}
	if(RTCIF & RTC_HF)			//半秒中断
	{
		RTCIF = RTC_HF;
		HalfSecFlag = 1;
	}	
	if(RTCIF & RTC_AF)			//闹钟中断
	{
		RTCIF = RTC_AF;	
		AlarmEvFlag = 1;
	}
}															
void Clock_MainLoop(void)
{
#if (IC_TYPE == IC_TYPE_CA51F3X)
	GPIO_Init(P32F,P32_XOSCL_IN_SETTING);
	GPIO_Init(P33F,P33_XOSCL_OUT_SETTING);
#elif  (IC_TYPE == IC_TYPE_CA51F2X)
	GPIO_Init(P72F,P72_XOSCL_IN_SETTING);
	GPIO_Init(P71F,P71_XOSCL_OUT_SETTING);
#endif	

	CKCON |= XLCKE;
	while(!(CKCON & XLSTA));
	
	RTC_init(RTCE_On,MSE_Off,HSE_On);
	
	Delay_50us(6);		//RTC使能后必须延时300us再写入时间， 否则写入时间可能无效。
	
	RTC_WriteHour(12);
	RTC_WriteMinute(0);
	RTC_WriteSecond(0);
	
	
	
	RTC_SetAlaramTime(12,1,0);
	RTC_AlarmCfg(HCE_On,MCE_On,SCE_On);
	INT8EN = 1;
	
	HalfSecFlag = 0;
	AlarmEvFlag = 0;
	while(1)
	{
		if(HalfSecFlag)
		{
			HalfSecFlag = 0;
#ifdef PRINT_EN
			uart_printf("Hour = %d,Minute = %d,Second = %d\n",(unsigned int)RTCH,(unsigned int)RTCM,(unsigned int)RTCS);	
#endif		
		}
		if(AlarmEvFlag)
		{
			AlarmEvFlag = 0;
#ifdef PRINT_EN
			uart_printf("Alarm event happen!\n");	
#endif		
		}
	}
}
#endif
