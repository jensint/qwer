#ifndef _WATCH_DOG_C_
#define _WATCH_DOG_C_

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
#include "../../Library/includes/stop_idle.h"
#include "../../Library/includes/system_clock.h"
#include "power_saving.h"
#include <intrins.h>
/*********************************************************************************************************************/

void PowerSaving_MainLoop(void)
{
#if (POWER_SAVING_MODE == STOP_MODE)
	bit IE_EA;
	I2CCON = 0;						//关闭I2C，因为I2C上电默认是打开的，必须关闭，否则主时钟无法关闭
	CKCON = 0;						//关闭所有时钟
	MECON |= BIT6;				//设置FLASH进入深度睡眠模式
	PWCON = (PWCON&0xF0) | 0x04;		//设置内部LDO为低功率模式
	
	while(!CheckStopCondition());
	IE_EA = EA;				//保存当前总中断开关状态
	EA = 0;					//关总中断
	StopEnter();	//进入STOP模式
	
	PWCON = (PWCON&0xF0) | 0x0B;	//退出STOP模式后必须设置LDO为高功率模式
	EA = IE_EA;				//恢复之前的总中断状态
	while(1)
	{
		
	}
#elif (POWER_SAVING_MODE == IDLE_MODE)
	bit IE_EA;
	I2CCON = 0;						//关闭I2C，因为I2C上电默认是打开的，必须关闭，否则主时钟无法关闭
	CKCON = 0;						//关闭所有时钟
	MECON |= BIT6;				//设置FLASH进入深度睡眠模式
	PWCON = (PWCON&0xF0) | 0x04;		//设置内部LDO为低功率模式	
	
	while(!CheckIdleCondition());
	Sys_Clk_Set_XOSCL();	//切换主时钟为低速时钟，因为IDLE模式主时钟是打开的，切换主时钟为低速时钟可有效降低功耗
//	Sys_Clk_Set_IRCL();
	IE_EA = EA;				//保存当前总中断开关状态
	EA = 0;					//关总中断
	IdleEnter();					//进入IDLE模式
	PWCON = (PWCON&0xF0) | 0x0B;	//退出IDLE模式后必须设置LDO为高功率模式
	EA = IE_EA;				//恢复之前的总中断状态
	while(1)
	{
		
	}
#elif (POWER_SAVING_MODE == LOW_SPEED_MODE)
	I2CCON = 0;						//关闭I2C，因为I2C上电默认是打开的，必须关闭，否则主时钟无法关闭
	CKCON = 0;						//关闭所有时钟
	MECON |= BIT6;				//设置FLASH进入深度睡眠模式
	PWCON = (PWCON&0xF0) | 0x04;		//设置内部LDO为低功率模式	
	
	Sys_Clk_Set_XOSCL();	//切换主时钟为低速时钟，注意：当主时钟从低速时钟切换回高速时钟时，必须把内部LDO设置回高功率模式
//	Sys_Clk_Set_IRCL();
	while(1)
	{
		
	}
#endif
	
}
#endif
