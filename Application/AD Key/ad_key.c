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
#include "../../Library/includes/adc.h"
#include "key.h"
#include "ad_key.h"
#include <intrins.h>
/*********************************************************************************************************************/
unsigned char ADKey_LongKeyTimer;
unsigned char ADKey_ScanTimer;
bit ADKey_MonitorFlag;
bit ADKey_LongStartFlag;
unsigned int ADKey_Msg;
void Timer0_init(void)
{
	TMOD=(TMOD&0xFC)|0x01;
	TL0 = 0x00;
	TH0 = 0xF4;
	TR0 = 1;
	ET0 = 1;	
}
void TIMER0_ISR (void) interrupt 1 		 //10ms 
{
	TL0 = 0x00;
	TH0 = 0xF4;

	if(ADKey_LongKeyTimer) ADKey_LongKeyTimer--;
	if(ADKey_ScanTimer) ADKey_ScanTimer--;

}
unsigned char ADC_GetKey(unsigned int ADC_Value)	//根据AD值获取按键值
{
	code unsigned int KEY_AD_TAB[][2]=
	{
		{K1,0x0000},
		{K2,0x0200},
		{K3,0x0400},
		{0,0x0FFF},
	};
	unsigned char i;
	for(i = 0; i < sizeof(KEY_AD_TAB)/4-1; i++)
	{
		if(ADC_Value < KEY_AD_TAB[i][1]/2 + KEY_AD_TAB[i+1][1]/2)
		{
			return KEY_AD_TAB[i][0];
		}
	}
	return 0;
}
unsigned int ADCKey_GetMsg(void)
{
	static unsigned int KeyBak = 0;
	static unsigned char DebounceCnt = 0;
	unsigned int ADC_Value,Msg = KEY_INVALID;
	unsigned char KeyVal;
	if(!ADKey_ScanTimer)
	{
		ADKey_ScanTimer = 2;
		ADC_ConfigChannel(ADC_CH0);	//配置ADC通道
		ADC_StartConversion();			//启动ADC转换
		ADC_Value = GetAdcValue();	//获取ADC数值
#ifdef PRINT_EN
		if(ADC_Value < 0xFF0)
		{
// 			uart_printf("ADC_Value = 0x%x\n",ADC_Value);	
		}
#endif
		KeyVal = ADC_GetKey(ADC_Value);
		if(!ADKey_MonitorFlag)
		{
			if(KeyVal != 0)
			{
				if(DebounceCnt == 0)
				{
					KeyBak = KeyVal;
					DebounceCnt++;
				}
				else if(KeyBak == KeyVal)
				{
					DebounceCnt++;
					if(DebounceCnt >= 1)
					{
						Msg = KeyVal;				//按键按下
						ADKey_MonitorFlag = 1;
						ADKey_LongStartFlag = 0;
						ADKey_LongKeyTimer = KEY_LONG_START_TIME;
						DebounceCnt = 0;
					}
				}
				else
				{
					DebounceCnt = 0;
				}
			}
		}
		else
		{
			if(KeyBak == KeyVal)
			{
				DebounceCnt = 0;
				if(!ADKey_LongKeyTimer)
				{
					if(!ADKey_LongStartFlag)
					{
						ADKey_LongStartFlag = 1;
						Msg = KeyBak | KEY_LONG_START;	//长按键开始
					}
					else
					{	
						Msg = KeyBak | KEY_LONG;				//长按键
					}
					ADKey_LongKeyTimer = KEY_LONG_TIME;
				}		
			}
			else
			{
				DebounceCnt++;
				if(DebounceCnt >= 2)
				{
					if(!ADKey_LongStartFlag)
					{
						Msg = KeyBak | KEY_BREAK;			//短按松手
					}
					else
					{
						Msg = KeyBak | KEY_LONG_BREAK;	//长按松手
					}
					ADKey_MonitorFlag = 0;		
				}
			}
		}
	}
	return Msg;
}
void ADKey_init(void)
{
#if (IC_TYPE == IC_TYPE_CA51F2X)
	GPIO_Init(P40F,P40_ADC0_SETTING);					//初始化ADC引脚
#elif (IC_TYPE == IC_TYPE_CA51F3X) 
	GPIO_Init(P10F,P10_ADC0_SETTING);					//初始化ADC引脚
#endif
	ADC_init(ADC_REF_VDD,DISABLE,DISABLE,0);	//ADC初始化
	ADKey_MonitorFlag = 0;
}
void ADKey_MainLoop(void)
{
	Timer0_init();
	ADKey_init();
	while(1)
	{
		ADKey_Msg = ADCKey_GetMsg();
		if(ADKey_Msg != KEY_INVALID)
		{
#ifdef PRINT_EN
			uart_printf("ADKey_Msg = 0x%x\n",ADKey_Msg);	
#endif		
		}
	}
}
#endif
