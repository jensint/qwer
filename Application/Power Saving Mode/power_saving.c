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
	I2CCON = 0;						//�ر�I2C����ΪI2C�ϵ�Ĭ���Ǵ򿪵ģ�����رգ�������ʱ���޷��ر�
	CKCON = 0;						//�ر�����ʱ��
	MECON |= BIT6;				//����FLASH�������˯��ģʽ
	PWCON = (PWCON&0xF0) | 0x04;		//�����ڲ�LDOΪ�͹���ģʽ
	
	while(!CheckStopCondition());
	IE_EA = EA;				//���浱ǰ���жϿ���״̬
	EA = 0;					//�����ж�
	StopEnter();	//����STOPģʽ
	
	PWCON = (PWCON&0xF0) | 0x0B;	//�˳�STOPģʽ���������LDOΪ�߹���ģʽ
	EA = IE_EA;				//�ָ�֮ǰ�����ж�״̬
	while(1)
	{
		
	}
#elif (POWER_SAVING_MODE == IDLE_MODE)
	bit IE_EA;
	I2CCON = 0;						//�ر�I2C����ΪI2C�ϵ�Ĭ���Ǵ򿪵ģ�����رգ�������ʱ���޷��ر�
	CKCON = 0;						//�ر�����ʱ��
	MECON |= BIT6;				//����FLASH�������˯��ģʽ
	PWCON = (PWCON&0xF0) | 0x04;		//�����ڲ�LDOΪ�͹���ģʽ	
	
	while(!CheckIdleCondition());
	Sys_Clk_Set_XOSCL();	//�л���ʱ��Ϊ����ʱ�ӣ���ΪIDLEģʽ��ʱ���Ǵ򿪵ģ��л���ʱ��Ϊ����ʱ�ӿ���Ч���͹���
//	Sys_Clk_Set_IRCL();
	IE_EA = EA;				//���浱ǰ���жϿ���״̬
	EA = 0;					//�����ж�
	IdleEnter();					//����IDLEģʽ
	PWCON = (PWCON&0xF0) | 0x0B;	//�˳�IDLEģʽ���������LDOΪ�߹���ģʽ
	EA = IE_EA;				//�ָ�֮ǰ�����ж�״̬
	while(1)
	{
		
	}
#elif (POWER_SAVING_MODE == LOW_SPEED_MODE)
	I2CCON = 0;						//�ر�I2C����ΪI2C�ϵ�Ĭ���Ǵ򿪵ģ�����رգ�������ʱ���޷��ر�
	CKCON = 0;						//�ر�����ʱ��
	MECON |= BIT6;				//����FLASH�������˯��ģʽ
	PWCON = (PWCON&0xF0) | 0x04;		//�����ڲ�LDOΪ�͹���ģʽ	
	
	Sys_Clk_Set_XOSCL();	//�л���ʱ��Ϊ����ʱ�ӣ�ע�⣺����ʱ�Ӵӵ���ʱ���л��ظ���ʱ��ʱ��������ڲ�LDO���ûظ߹���ģʽ
//	Sys_Clk_Set_IRCL();
	while(1)
	{
		
	}
#endif
	
}
#endif
