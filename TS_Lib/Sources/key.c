#ifndef _KEY_C_
#define _KEY_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "ca51f2sfr.h"
#include "ca51f2xsfr.h"
#include "gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"
#endif
#include "rtc.h"		
#include "system_clock.h"		
#include "uart.h"
#include "key.h"
/*********************************************************************************************************************/
#include "ts_configuration.h"
#include "ts_def.h"
#include "ts_api.h"
#include "ts_service.h"		   
/*********************************************************************************************************************/
#if SUPPORT_KEY
/*************************************************************************************************
������
��K1Ϊ����K1��������������������ʾ��

						--> K1|KEY_BREAK(�̰��ɿ�)
					-
K1(����)--
					-
						--> K1|KEY_LONG_START(������Լ1��)---> K1|KEY_LONG(һֱ������Լÿ300ms����������)---->K1|KEY_LONG_BREAK(�����ɿ�)
																							 ^																							 |
																							 |																							 |
																							 |-----------------------------------------------|

���ϼ���
��K1(K1=0x0001),K2(K2=0x0002)Ϊ���� ��K1��K2ͬʱ����ʱ����������Ϊ(K1<<5)|K2����0x22, ��������������������ʾ��


										--> ((K1<<5)|K2)|KEY_BREAK(�̰��ɿ�)
									-
(K1<<5)|K2(����)--
									-
										--> ((K1<<5)|K2)|KEY_LONG_START(������Լ1��)---> ((K1<<5)|K2)|KEY_LONG(һֱ������Լÿ300ms����������)---->((K1<<5)|K2)|KEY_LONG_BREAK(�����ɿ�)
																																 ^																							           |
																																 |																							           |
																																 |---------------------------------------------------------|
ע�⣺���ڸ��ϼ�����ʱ�������һ��ͬʱ��⵽˫���������ȼ�⵽�������£������ȼ�⵽K1����ʱ���Ȳ���K1�����ȵ���⵽K2Ҳ��
��ʱ���Ż��������(K1<<5)|K2����⵽˫�������˫��һֱ���£������((K1<<5)|K2)|KEY_LONG_START��((K1<<5)|K2)|KEY_LONG�����
��һ���������֣���ֹͣ���������� �ȵ�˫�����ɿ�ʱ���Ż����((K1<<5)|K2)|KEY_BREAK(�̰��ɿ�)��((K1<<5)|K2)|KEY_LONG_BREAK(�����ɿ�)
*************************************************************************************************/
typedef struct 
{	
	unsigned char 	TogKeyNum;
	unsigned char 	TogKeyList[2];
}
T_TogKeyInfo;
code unsigned char TS_KEY_TAB[]=
{
	K1,K2,K3,K4,K5,K6,K7,K8,K9,K10,K11,K12,K13,K14,K15,K16,K17,K18,K19,K20,
#if (IC_TYPE == IC_TYPE_CA51F2X)
	K21,K22,K23,K24
#endif
};
void TSKey_init(void)
{
	TK_State = TK_STATE_RELEASE;
	TS_Key = 0;
}
void TS_GetTogKeyInfor(T_TogKeyInfo *pKeyInfo)
{
	unsigned char i,index;
	pKeyInfo->TogKeyNum = 0;
	index = 0;
	for(i = 0; i < KEY_CH_COUNT; i++)
	{
		if(KeysFlagSN & MASK_TAB[i])
		{
			pKeyInfo->TogKeyNum++;
			
			if(pKeyInfo->TogKeyNum <= 2)
			{
				pKeyInfo->TogKeyList[index++] = TS_KEY_TAB[i];
			}
		}
	}
}
#if PRINT_TS_KEY_EN
void Hex2CharPrint(unsigned int integar)
{
	unsigned char CharBuf[4];
	unsigned char i, temp;
	for(i = 0; i < 4; i++)
	{
		temp = (unsigned char)(integar&0x0F);
		if(temp >= 0x0A)
		{
			CharBuf[i] = (temp - 0x0A) + 'A';
		}
		else
		{
			CharBuf[i] = temp + '0';
		}
		integar >>= 4;
	}
	Uart0_PutChar('0');	
	Uart0_PutChar('x');	
	Uart0_PutChar(CharBuf[2]);
	Uart0_PutChar(CharBuf[1]);
	Uart0_PutChar(CharBuf[0]);
}
#endif
void TS_GetKey(void)
{
	static unsigned int KeyBak;	
	static bit LongFlag;
	T_TogKeyInfo KeyInfo;
	TS_Key = 0;
	TS_GetTogKeyInfor(&KeyInfo);
		
	if(TK_State == TK_STATE_RELEASE)
	{
		if(KeyInfo.TogKeyNum != 0)
		{			
			if(KeyInfo.TogKeyNum == 1)
			{
				KeyBak = (unsigned int)KeyInfo.TogKeyList[0];
				TK_State = TK_STATE_SINGLE_KEY_PRESS;				
			}
			else if(KeyInfo.TogKeyNum == 2)
			{
				KeyBak = ((unsigned int)KeyInfo.TogKeyList[0]<<5)|(unsigned int)KeyInfo.TogKeyList[1];
				TK_State = TK_STATE_DOUBLE_KEY_PRESS;
			}
			
			TS_Key = KeyBak;
			LongFlag = 0;
			TS_LongKeyTimer = TS_LONG_START_TIME;
		}
	}
	else if(TK_State == TK_STATE_SINGLE_KEY_PRESS)
	{
		if(KeyInfo.TogKeyNum == 1)
		{
			if(KeyBak == (unsigned int)KeyInfo.TogKeyList[0])
			{
				if(!TS_LongKeyTimer)
				{
					if(!LongFlag)
					{
						LongFlag = 1;
						TS_Key = KeyBak | KEY_LONG_START;
					}
					else
					{
						TS_Key = KeyBak | KEY_LONG;
					}
					TS_LongKeyTimer = TS_LONG_TIME;
				}				
			}
			else 
			{
				if(!LongFlag)
				{
					TS_Key = KeyBak | KEY_BREAK;
				}
				else
				{
					TS_Key = KeyBak | KEY_LONG_BREAK;
				}
				TK_State = TK_STATE_RELEASE;	
			}
		}
		else if(KeyInfo.TogKeyNum == 2)
		{
			KeyBak = ((unsigned int)KeyInfo.TogKeyList[0]<<5) | (unsigned int)KeyInfo.TogKeyList[1];
			TK_State = TK_STATE_DOUBLE_KEY_PRESS;
			TS_Key = KeyBak;
			LongFlag = 0;
			TS_LongKeyTimer = TS_LONG_START_TIME;
		}
		else if(KeyInfo.TogKeyNum == 0)
		{
			if(!LongFlag)
			{
				TS_Key = KeyBak | KEY_BREAK;
			}
			else
			{
				TS_Key = KeyBak | KEY_LONG_BREAK;
			}
			TK_State = TK_STATE_RELEASE;				
		}
	}
	else if(TK_State == TK_STATE_DOUBLE_KEY_PRESS)
	{
		if(KeyInfo.TogKeyNum == 2)
		{
			if(KeyBak == ((unsigned int)KeyInfo.TogKeyList[0]<<5)|(unsigned int)KeyInfo.TogKeyList[1])
			{
				if(!TS_LongKeyTimer)
				{
					if(!LongFlag)
					{
						LongFlag = 1;
						TS_Key = KeyBak | KEY_LONG_START;
					}
					else
					{
						TS_Key = KeyBak | KEY_LONG;
					}
					TS_LongKeyTimer = TS_LONG_TIME;
				}					
			}
		}
		else if(KeyInfo.TogKeyNum == 0)
		{
			if(!LongFlag)
			{
				TS_Key = KeyBak | KEY_BREAK;
			}
			else
			{
				TS_Key = KeyBak | KEY_LONG_BREAK;
			}
			TK_State = TK_STATE_RELEASE;				
		}
	}
#if PRINT_TS_KEY_EN
	if(TS_Key != 0)
	{
		if((TS_Key & 0xFF00) == 0)
		{
			if(TS_Key > 0x1F)
			{
				Uart0_PutChar('d');	
				Uart0_PutChar('o');	
				Uart0_PutChar('u');	
				Uart0_PutChar('b');	
				Uart0_PutChar('l');	
				Uart0_PutChar('e');			
			}
			else
			{
				Uart0_PutChar('s');	
				Uart0_PutChar('i');	
				Uart0_PutChar('n');	
				Uart0_PutChar('g');	
				Uart0_PutChar('l');	
				Uart0_PutChar('e');		
			}
			Uart0_PutChar(' ');	
			Uart0_PutChar('k');	
			Uart0_PutChar('e');	
			Uart0_PutChar('y');	
			Uart0_PutChar(' ');	
			Uart0_PutChar('=');	
			Uart0_PutChar(' ');	
			Hex2CharPrint(TS_Key);
			Uart0_PutChar('\r');	
			Uart0_PutChar('\n');	
		}
		else if(TS_Key & KEY_BREAK)
		{
			Uart0_PutChar('k');	
			Uart0_PutChar('e');	
			Uart0_PutChar('y');	
			Uart0_PutChar(' ');	
			Uart0_PutChar('u');	
			Uart0_PutChar('p');	
			Uart0_PutChar('\r');	
			Uart0_PutChar('\n');				
		}
		else if(TS_Key & KEY_LONG_START)
		{
			Uart0_PutChar('l');	
			Uart0_PutChar('o');	
			Uart0_PutChar('n');	
			Uart0_PutChar('g');	
			Uart0_PutChar(' ');	
			Uart0_PutChar('s');	
			Uart0_PutChar('t');	
			Uart0_PutChar('a');	
			Uart0_PutChar('r');	
			Uart0_PutChar('t');	
			Uart0_PutChar('\r');	
			Uart0_PutChar('\n');			
		}
		else if(TS_Key & KEY_LONG)
		{
			Uart0_PutChar('l');	
			Uart0_PutChar('o');	
			Uart0_PutChar('n');	
			Uart0_PutChar('g');	
			Uart0_PutChar('\r');	
			Uart0_PutChar('\n');			
		}
	}
#endif
}
#endif

/*********************************************************************************************************************/
#endif