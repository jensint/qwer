#ifndef _RTC_C_
#define _RTC_C_
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
#include "../../Library/includes/rtc.h"
#include "../../Library/includes/delay.h"
#include <intrins.h>
/*********************************************************************************************************************/
/***********************************************************************************
��������		RTC_init
����˵���� ��ʼ��RTC���ƼĴ���
��������� rtce		RTC����ģ��ʹ�ܿ���
						mse		RTC�����ж�ʹ�ܿ���
						hse		RTC�����ж�ʹ�ܿ���
����ֵ��	��
***********************************************************************************/																												
void RTC_init(RTCE_TypeDef rtce,MSE_TypeDef mse,HSE_TypeDef hse)
{	
	RTCON = (RTCON & 0x1F) | RTCE(rtce) | MSE(mse) | HSE(hse);
}

/***********************************************************************************
��������		RTC_WriteSecond
����˵���� RTCд����ֵ
��������� second		��ֵ
����ֵ��		��
***********************************************************************************/	
void RTC_WriteSecond(unsigned char second) //second = 0~59
{
	RTCON |= RTCWE(1);
	RTCS = second;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}

/***********************************************************************************
��������		RTC_WriteMinute
����˵���� RTCд���ֵ
��������� minute		��ֵ
����ֵ��		��
***********************************************************************************/	
void RTC_WriteMinute(unsigned char minute) //minute = 0~59
{
	RTCON |= RTCWE(1);
	RTCM = minute;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);
}


/***********************************************************************************
��������		RTC_WriteHour
����˵���� RTCд��Сʱ
��������� hour		Сʱֵ
����ֵ��		��
***********************************************************************************/
void RTC_WriteHour(unsigned char hour)	//hour = 0~23
{
	RTCON |= RTCWE(1);
	RTCH = (RTCH&0xE0)|hour;
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}


/***********************************************************************************
��������		RTC_WriteWeek
����˵���� RTCд������
��������� week		����ֵ
����ֵ��		��
***********************************************************************************/
void RTC_WriteWeek(unsigned char week) //hour = 1~7��������һ~������
{
	RTCON |= RTCWE(1);
	RTCH = (RTCH&0x1F)|(week<<5);
	Delay_50us(1);
	RTCON &= ~RTCWE(1);	 	
}


/***********************************************************************************
��������		RTC_AlarmCfg
����˵���� ����RTC ALARM���ܼĴ���λ
��������� hce		Сʱ�Ƚ�ʹ�ܿ���
						mce		���ӱȽ�ʹ�ܿ���
						sce		��Ƚ�ʹ�ܿ���
����ֵ��		��
***********************************************************************************/
void RTC_AlarmCfg(HCE_TypeDef hce,MCE_TypeDef mce,SCE_TypeDef sce)
{	
	RTCON = (RTCON & 0xE3) | HCE(hce) | MCE(mce) | SCE(sce);
}

/***********************************************************************************
��������		RTC_SetAlaramTime
����˵����  ����RTC ALARMʱ��
���������  al_hr		����Сʱֵ
						al_min		���ӷ���ֵ
						al_sec		������ֵ
����ֵ��		��
***********************************************************************************/
void RTC_SetAlaramTime(unsigned char al_hr,unsigned char al_min,unsigned char al_sec)
{
	RTAH	=	al_hr;
	RTAM	=	al_min;
	RTAS	=	al_sec;
}

/***********************************************************************************
��������		RTC_SetRTMSS
����˵����  ���ú����жϼ��
���������  rtmss		
����ֵ��		��
***********************************************************************************/
void RTC_SetRTMSS(unsigned char rtmss)
{
	RTMSS = rtmss;
}

/*********************************************************************************************************************/
#endif