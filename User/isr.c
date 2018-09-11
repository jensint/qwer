#ifndef _ISR_C_
#define _ISR_C_



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
#include <intrins.h>

#include "app_config.h"
#include "app_task.h"


extern uint8_t RDA_Time_Count;


void INT0_ISR(void) interrupt 0
{


}


void TIMER0_ISR(void) interrupt 1
{
//  TH0 = (unsigned char)((TIMER_DIV>>5)&0x00ff);
//	TL0 = (unsigned char)TIMER_DIV&0x1f;
	TH0 = (unsigned char)((TIME_DIV_NUM>>5)&0x00ff);
	TL0 = (unsigned char)TIME_DIV_NUM&0x1f;
	
	app_task_time.AdcKeyScan++;
	
	app_task_time.PowerDet++;
	
	if(app_task_time.FMAutoSearch > 0)
	{
    app_task_time.FMAutoSearch--;
  }
	
	if(FM_INIT_COUNNT > 0)
	{
    FM_INIT_COUNNT--;
  }
	
	if(FM_CHANGE_COUNT > 0)
	{
    FM_CHANGE_COUNT--;
  }
	
	if(ADC_KEY_JUDGE_TIME > 0)
	{
		ADC_KEY_JUDGE_TIME--;
	}
	
	if(TS_KEY_JUDGE_TIME > 0)
	{
    TS_KEY_JUDGE_TIME--;
  }
	
	if(BT_Uart.BT_Init_Count < 0xff)
	{
    BT_Uart.BT_Init_Count++;
  }
	
	if(BT_Uart.BTCmdResendTime_count < 0xff)
	{
    BT_Uart.BTCmdResendTime_count++;
  }
	
	if(LEDW_PWM_Time > 0)
	{
    LEDW_PWM_Time--;
  }
	
	if(sys_state.Alarm != ALARM_IDLE)
	{
    if(Alarm_Ring_Time > 0)
		{
      Alarm_Ring_Time--;
			if(Alarm_Ring_Time == 0)
			{
        BT_Uart.BTCmd_Temp = CMD_RING1;
		    BT_Uart.BTCmdResendFlag = 1;
				Alarm_Ring_Flag = 1;
      }
    }

  }
}


void UART0_ISR(void) interrupt 4
{
  uint8_t rx_dat;	
	if(RI0)
	{
		RI0 = 0;
		rx_dat = S0BUF;	
		if(Mcu_RcvBTCmd(rx_dat))
		{
      BT_Uart.BTCmdRcvFinishFlag = 1;
    }
	}
	if(TI0)
	{	
		TI0 = 0;	
		uart0_tx_flag = 0;
	}
}


void INT3_ISR (void) interrupt 8
{	
	if(TKIF != 0)
	{
//		LED_GREEN_PIN_DATA = ~LED_GREEN_PIN_DATA;
		TS_ISR();
	}
}


void RTC_ISR(void) interrupt 13
{
	static uint8_t display_flag = 0; 	
	
  if(RTCIF & RTC_MF)
	{//毫秒中断
    RTCIF = RTC_MF;
		TS_MS_ISR ();
  }
	
	if(RTCIF & RTC_HF)
	{//半秒中断
    RTCIF = RTC_HF;
		RTC_Num.Hour = RTCH & 0x1F;
		RTC_Num.Minute = RTCM & 0x3F;	
		RTC_Num.Second = RTCS & 0x3F;
		
		TS_HS_ISR ();
		
		if(RDA_Time_Count > 0)
		{
      RDA_Time_Count--;
    }
	
		if(Settime_Dis_Count > 0)
		{
      Settime_Dis_Count--;
    }
		
		if(count>0)
		{
			count--;
		}
		else
		{
			ge+=1;
			count=2;
		}
		
		if(sys_state.SetVolumeFlag)
		{
		  if(SetVolume_Keeptime_Count > 0)
		  {
        SetVolume_Keeptime_Count--;
      }
			else
			{
        sys_state.SetVolumeFlag = 0;
				if(sys_state.Mode == SYS_FM)
				{
          FM_Freq_LED_Display(sys_state.FMFreq);
        }
      }
		}
		else
		{
		
		  if(sys_state.SetTime != Unit_Null)
		  {
        if(Settime_Keeptime_Count> 0)
		    {
          Settime_Keeptime_Count--;
        }	
			
			  if(Settime_Keeptime_Count == 0)
			  {
          sys_state.SetTime = Unit_Null;
					if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
			    {
			      Alarm_LED_Display(sys_state.Mode);
			    }
        }
      }
		  else
		  {
			  Settime_Keeptime_Count = 0;
      }
		
		  if(sys_state.SetTime != Unit_Null)    //如果设置的位数不为空  ，有设置的位数
		  {
				
			  if(Settime_Dis_Count == 0)       //如果按了设置时间键，则1.5后进入闪烁
			  {
				  Settime_LED_Display(sys_state.Mode, sys_state.SetTime, display_flag);
			    if(display_flag)
	        {
            display_flag = 0;
          }
	        else
	        {
		        display_flag = 1;
	        }   
			  }
			  else
			  {
          display_flag = 0;
        }
      }
		  else if(sys_state.Mode == SYS_AUX || sys_state.Mode == SYS_CLOCK)   //如果没有设置位数，则正常现实
		  {
			  Led_Display(RTC_Num.Hour, RTC_Num.Minute, sys_state.Mode);
		  }

			 if(sys_state.Mode == SYS_BLUETOOTH)
			{
				LED_COUNT(sys_state.Mode);
			}	
//			else if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
//			{
//			  Alarm_LED_Display(sys_state.Mode);
//			}
	  }
  }
	
	if(RTCIF & RTC_AF)
	{//闹钟中断
    RTCIF = RTC_AF;
//		LED_GREEN_PIN_DATA = 0;
  }
}


#endif

