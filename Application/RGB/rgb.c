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
#include "../../Library/includes/pwm.h"
#include "../../Library/includes/delay.h"
#include "key.h"
#include "rgb.h"
#include <intrins.h>
/*********************************************************************************************************************/
unsigned int PWM_DivDat;
unsigned int LED_R_DutyDat;
unsigned int LED_G_DutyDat;
unsigned int LED_B_DutyDat;

void RGB_MainLoop(void)
{

#if (IC_TYPE == IC_TYPE_CA51F2X)
	GPIO_Init(P52F,P52_PWM2_SETTING);
	GPIO_Init(P53F,P53_PWM3_SETTING);
	GPIO_Init(P54F,P54_PWM4_SETTING);
#elif (IC_TYPE == IC_TYPE_CA51F3X)
	GPIO_Init(P35F,P35_PWM2_SETTING);
	GPIO_Init(P34F,P34_PWM3_SETTING);
	GPIO_Init(P07F,P07_PWM4_SETTING);
#endif
	
	PWM_DivDat = 3686;
	
	LED_R_DutyDat = 0;
	LED_G_DutyDat = 0;
	LED_B_DutyDat = 0;
	
	PWM_init(PWM_CH2,Edge_Align,CKS_IH,Toggle_Off,0,0);
	PWM_init(PWM_CH3,Edge_Align,0,Toggle_Off,0,Complementary_Off);
	PWM_init(PWM_CH4,Edge_Align,CKS_IH,Toggle_Off,0,0);
	
	PWM_CfgDivDuty(PWM_CH2,PWM_DivDat,LED_B_DutyDat);
	PWM_CfgDivDuty(PWM_CH3,PWM_DivDat,LED_G_DutyDat);
	PWM_CfgDivDuty(PWM_CH4,PWM_DivDat,LED_R_DutyDat);
	
	PWM_Update((1<<PWM_CH2)|(1<<PWM_CH3)|(1<<PWM_CH4));
	WaitPWMUpdateComplete();
	PWM_EnContrl((1<<PWM_CH2)|(1<<PWM_CH3)|(1<<PWM_CH4));
	while(1)
	{
		do
		{
			Delay_ms(1);
			LED_B_DutyDat += 5;
			if(LED_B_DutyDat > PWM_DivDat)
			{
				LED_B_DutyDat = PWM_DivDat;
			}
			PWM_CfgDivDuty(PWM_CH2,PWM_DivDat,LED_B_DutyDat);
			PWM_Update(1<<PWM_CH2);		
		}
		while(LED_B_DutyDat < PWM_DivDat);
		do
		{
			Delay_ms(1);
			if(LED_B_DutyDat >= 5)
			{
				LED_B_DutyDat -= 5;
			}
			else
			{
				LED_B_DutyDat = 0;
			}
			PWM_CfgDivDuty(PWM_CH2,PWM_DivDat,LED_B_DutyDat);
			PWM_Update(1<<PWM_CH2);		
		}
		while(LED_B_DutyDat);
		
		
		do
		{
			Delay_ms(1);
			LED_G_DutyDat += 5;
			if(LED_G_DutyDat > PWM_DivDat)
			{
				LED_G_DutyDat = PWM_DivDat;
			}
			PWM_CfgDivDuty(PWM_CH3,PWM_DivDat,LED_G_DutyDat);
			PWM_Update(1<<PWM_CH3);		
		}
		while(LED_G_DutyDat < PWM_DivDat);
		do
		{
			Delay_ms(1);
			if(LED_G_DutyDat >= 5)
			{
				LED_G_DutyDat -= 5;
			}
			else
			{
				LED_G_DutyDat = 0;
			}
			PWM_CfgDivDuty(PWM_CH3,PWM_DivDat,LED_G_DutyDat);
			PWM_Update(1<<PWM_CH3);		
		}
		while(LED_G_DutyDat);
		
		do
		{
			Delay_ms(1);
			LED_R_DutyDat += 5;
			if(LED_R_DutyDat > PWM_DivDat)
			{
				LED_R_DutyDat = PWM_DivDat;
			}
			PWM_CfgDivDuty(PWM_CH4,PWM_DivDat,LED_R_DutyDat);
			PWM_Update(1<<PWM_CH4);		
		}
		while(LED_R_DutyDat < PWM_DivDat);
		do
		{
			Delay_ms(1);
			if(LED_R_DutyDat >= 5)
			{
				LED_R_DutyDat -= 5;
			}
			else
			{
				LED_R_DutyDat = 0;
			}
			PWM_CfgDivDuty(PWM_CH4,PWM_DivDat,LED_R_DutyDat);
			PWM_Update(1<<PWM_CH4);		
		}
		while(LED_R_DutyDat);

	}
}
#endif
