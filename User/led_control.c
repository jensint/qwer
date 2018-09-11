
#ifndef _LED_CONTROL_C_
#define _LED_CONTROL_C_

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

#include "led_control.h"
#include "app_task.h"
/********************************0****1****2****3****4***5*****6****7***8****9*****A***B*****C***D****E******F*************************************************/
uint8_t code Number_Table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};  //0-9,A-F
uint8_t code Number_VOL = 0x3E;  //LED屏显示"V"
uint8_t code Number_FMChannel = 0x76;  //LED屏显示"P""0x73"|"H""0x76"
uint8_t xdata Led_Display_Table[7] = {0};

CLOCK_NUM xdata RTC_Num;  //实时时间值
CLOCK_NUM xdata Alarm1_Num;  //闹钟1时间
CLOCK_NUM xdata Alarm2_Num;  //闹钟2时间
CLOCK_NUM xdata Settime_Num;  //设置的时间

uint8_t xdata Settime_Dis_Count = 0;  //显示设置时间的常亮时间  单位0.5s
uint8_t xdata Settime_Keeptime_Count = 0;  //设置时间到取消的时限  单位0.5s
uint8_t xdata SetVolume_Keeptime_Count = 0;  //设置音量显示保持的时限  单位0.5s
uint8_t ge=0,shi=0,bai=0,qian=0;
LED_DIS_VALUE xdata Led_Number;  //led显示值
LED_DIS_STATE xdata Led_Dis_State;  //显示的状态, BT/FM/AUX/MHZ/TIME/PLAY/PUSE

/******************************************************************************
函数名：	Led_Seg_Display
功能描述：设置seg上的值
输入参数: index  显示寄存器编号
          dat    显示缓存值
返回值：  无					
******************************************************************************/
void Led_Seg_Display(uint8_t index, uint8_t dat)
{
  INDEX = index;
	LXDAT = dat;
}

/******************************************************************************
函数名：	Led_Display
功能描述：LED屏显示
输入参数: hour 小时
          minute 分钟
          mode 模式
返回值：  无					
******************************************************************************/
void LED_COUNT(uint8_t mode)
{
	uint8_t count=2;
	uint8_t i;
	if(ge>9)
	{
			ge=0;
		shi+=1;
		if(shi>9)
			{
					shi=0;
					bai+=1;
			}
		if(bai>9)
			{
					bai=0;
					qian+=1;
			}
			if(qian>9)
			{
				ge=0;
				shi=0;
				bai=0;
				qian=0;			
			}
	}

	
		Led_Number.TenDigitsHour = qian;
		Led_Number.UnitsDigitHour = bai;
		Led_Number.TenDigitsMinute = shi;
		Led_Number.UnitsDigitMinute = ge;
	
	  for(i=0; i<7; i++)
		{
			Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;  //Number_Table[Led_Number.TenDigitsHour] >> i,显示每一段
			Led_Display_Table[i] <<= 1;
			Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
			Led_Display_Table[i] <<= 1;
			Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
			Led_Display_Table[i] <<= 1;
			Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
			Led_Display_Table[i] <<= 1; 
		}
		
	if(mode == SYS_BLUETOOTH)
	{
    if(BT_Operating_State == BT_IDEL || BT_Operating_State == BT_DISCONNECT)
		{
      Led_Display_Table[3] |= 1;              //BT图标闪烁
    }
		else
		{
      Led_Display_Table[3] |= 1;
    }		
	}	
		
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
}

void Led_Display(uint8_t hour, uint8_t minute, uint8_t mode)
{
	static uint8_t point = 0;
  uint8_t i = 0;
	Led_Number.TenDigitsHour = hour/10;
	Led_Number.UnitsDigitHour = hour%10;
	Led_Number.TenDigitsMinute = minute/10;
	Led_Number.UnitsDigitMinute = minute%10;
	
  for(i=0; i<7; i++)
	{
    Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;  //Number_Table[Led_Number.TenDigitsHour] >> i,显示每一段
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;  
  }

	if(point)
	{
    point = 0;
  }
	else
	{
		point = 1;
	}

	Led_Display_Table[4] |= point;  //Led_Display_Table[0-4]:1A,2A,3A,4A,K0
	
	if(mode == SYS_AUX)
	{
    Led_Display_Table[5] |= 1;		//显示AUX图标      
  }
/***
	else if(mode == SYS_BLUETOOTH)
	{
    if(BT_Operating_State == BT_IDEL || BT_Operating_State == BT_DISCONNECT)
		{
      Led_Display_Table[3] |= point;              //BT图标闪烁
    }
		else
		{
      Led_Display_Table[3] |= 1;
    }
  }
	
****/
	if(Alarm1_Num.Hour != ALARM_DISABLE || Alarm1_Num.Minute != ALARM_DISABLE)
	{	
		Led_Display_Table[0] |= 1;         //闹钟图标
  }

	if(Alarm2_Num.Hour != ALARM_DISABLE || Alarm2_Num.Minute != ALARM_DISABLE)
  {
		Led_Display_Table[1] |= 1;					//	闹钟图标
  }
	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
	
}


/******************************************************************************
函数名：	Alarm_LED_Display
功能描述：显示闹钟的时间
输入参数: alarm_style 闹钟1/闹钟2
返回值：  无					
******************************************************************************/

void Alarm_LED_Display(uint8_t alarm_style)
{
	uint8_t i = 0;
	uint8_t xdata hour_num = 0;
	uint8_t xdata minute_num = 0;
	 
	if(alarm_style == SYS_ALARM1)
	{	
		if(Alarm1_Num.Hour == ALARM_DISABLE || Alarm1_Num.Minute == ALARM_DISABLE)
		{
      Led_Number.TenDigitsHour = 0x10;   //清屏，不显示
	    Led_Number.UnitsDigitHour = 0;   //显示0
	    Led_Number.TenDigitsMinute = 0x0f;   //显示F
	    Led_Number.UnitsDigitMinute = 0x0f;		//显示F	
		}
		else
		{
			hour_num = Alarm1_Num.Hour;
	  	minute_num = Alarm1_Num.Minute;
			
      Led_Number.TenDigitsHour = hour_num/10;
	    Led_Number.UnitsDigitHour = hour_num%10;
	    Led_Number.TenDigitsMinute = minute_num/10;
	    Led_Number.UnitsDigitMinute = minute_num%10;
    }
  }
  else if(alarm_style == SYS_ALARM2)
	{
    if(Alarm2_Num.Hour == ALARM_DISABLE || Alarm2_Num.Minute == ALARM_DISABLE)
		{
      Led_Number.TenDigitsHour = 0x10;
	    Led_Number.UnitsDigitHour = 0;
	    Led_Number.TenDigitsMinute = 0x0f;
	    Led_Number.UnitsDigitMinute = 0x0f;
    }
		else
		{
			hour_num = Alarm2_Num.Hour;
	  	minute_num = Alarm2_Num.Minute;
			
      Led_Number.TenDigitsHour = hour_num/10;
	    Led_Number.UnitsDigitHour = hour_num%10;
	    Led_Number.TenDigitsMinute = minute_num/10;
	    Led_Number.UnitsDigitMinute = minute_num%10;
    }
  
	}
	
	for(i=0; i<7; i++)
	{
    Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
  
  }
	
	if(alarm_style == SYS_ALARM1)
	{
		if(Alarm1_Num.Hour != ALARM_DISABLE && Alarm1_Num.Minute != ALARM_DISABLE)
		{
      Led_Display_Table[4] |= 1;				//显示两点				
		}	
			Led_Display_Table[0] |= 1;					//点亮闹钟图标
  }
  else if(alarm_style == SYS_ALARM2)
	{
		if(Alarm2_Num.Hour != ALARM_DISABLE && Alarm2_Num.Minute != ALARM_DISABLE)
    {
			Led_Display_Table[4] |= 1;
		}
		Led_Display_Table[1] |= 1;
  }
//	Led_Display_Table[4] |= 1;

	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
}

/******************************************************************************
函数名：	Settime_LED_Display
功能描述：显示设置时的时间
输入参数: mode 时间/闹钟1/闹钟2
          unit 设置位数
          flag 闪烁 亮/灭
返回值：  无					
******************************************************************************/
void Settime_LED_Display(uint8_t mode, CLOCK_UNIT unit, uint8_t flag)
{
//	static uint8_t point = 0;
	uint8_t i = 0;
	
	if((mode == SYS_ALARM1 || mode == SYS_ALARM2) && (Settime_Num.Hour == ALARM_DISABLE || Settime_Num.Minute == ALARM_DISABLE))       //闪烁OFF图标
	{
    Led_Number.TenDigitsHour = 0x10;
	  Led_Number.UnitsDigitHour = 0;
	  Led_Number.TenDigitsMinute = 0x0f;
	  Led_Number.UnitsDigitMinute = 0x0f;
		
//		if(flag)
//		{
      if(flag)
		  {
        for(i=0; i<7; i++)
	      {
          Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
        }

			
				if(mode == SYS_ALARM1)
				{
					Led_Display_Table[0]|=1;
				}
					if(mode == SYS_ALARM2)
				{
					Led_Display_Table[1]|=1;
				}


				
      }
			else
			{
				for(i=0; i<7; i++)
				{
          Led_Display_Table[i] <<= 4;
				}
      }
//    }
  }
	else
	{
	  Led_Number.TenDigitsHour = Settime_Num.Hour/10;
	  Led_Number.UnitsDigitHour = Settime_Num.Hour%10;
	  Led_Number.TenDigitsMinute = Settime_Num.Minute/10;
	  Led_Number.UnitsDigitMinute = Settime_Num.Minute%10;
	
	  if(unit == Unit_Hour)
	  {
      if(flag)
		  {
        for(i=0; i<7; i++)
	      {
          Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
        }
				
			if(mode == SYS_ALARM1)       //			
			{
				Led_Display_Table[0]|=1;
			}
				if(mode == SYS_ALARM2)       //			
			{
				Led_Display_Table[1]|=1;
			}	
				
				
      }
		  else
		  {
				for(i=0; i<7; i++)
	      {
//        Led_Display_Table[i] |= 0;
//		    Led_Display_Table[i] <<= 1;
//		    Led_Display_Table[i] |= 0;
		      Led_Display_Table[i] <<= 2;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
				}
      }	
    }
	  else if(unit ==Unit_Minute)
	  {
      if(flag)
		  {
        for(i=0; i<7; i++)
	      {
          Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
        }
				
				
					if(mode == SYS_ALARM1)       //			
			{
				Led_Display_Table[0]|=1;
			}
				if(mode == SYS_ALARM2)       //			
			{
				Led_Display_Table[1]|=1;
			}	
															
      }
		  else
		  {
				for(i=0; i<7; i++)
				{
          Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		      Led_Display_Table[i] <<= 1;
		      Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
//		      Led_Display_Table[i] <<= 1;
//		      Led_Display_Table[i] |= 0;
//		      Led_Display_Table[i] <<= 1;
//		      Led_Display_Table[i] |= 0;
		      Led_Display_Table[i] <<= 3;
				}
      }
    }
	  Led_Display_Table[4] |= 1;
  }
	

	
	if(mode == SYS_BLUETOOTH)
	{
    if(BT_Operating_State == BT_IDEL || BT_Operating_State == BT_DISCONNECT)
		{
			if(flag)
			{
        Led_Display_Table[3] |= 1;
			}
			else
			{
        Led_Display_Table[3] |= 0;
      }
    }
		else
		{
      Led_Display_Table[3] |= 1;
    }
  }
	else if(mode == SYS_AUX)
	{
    Led_Display_Table[5] |= 1;
  }
/********************delece***************************
	********************************************/
  else if(mode == SYS_ALARM1)
	{
		if((unit == Unit_Null)||(unit ==Unit_Null))
		{	
			if((Settime_Num.Hour != ALARM_DISABLE) || (Settime_Num.Minute != ALARM_DISABLE))	  //delece
			{
				Led_Display_Table[0] |= 1;
			}
		}
	}

	else if(mode == SYS_ALARM2)
	{
   	if((unit == Unit_Null)||(unit ==Unit_Null))
		{	
			if((Settime_Num.Hour != ALARM_DISABLE) || (Settime_Num.Minute != ALARM_DISABLE))	  //delece
			Led_Display_Table[1] |= 1;
		}
  }

  for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
}



/******************************************************************************
函数名：	Volume_LED_Display
功能描述：显示音量
输入参数: volume 音量值
返回值：  无					
******************************************************************************/
void Volume_LED_Display(uint8_t volume)
{
  uint8_t i = 0;
	
	Led_Number.TenDigitsMinute = volume/10;
	Led_Number.UnitsDigitMinute = volume%10;

	for(i=0; i<7; i++)
	{
    Led_Display_Table[i] |= 0;                //第一位不显示
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_VOL >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
  }
	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
}



/******************************************************************************
函数名：	Volume_LED_Display
功能描述：显示FM当前频率
输入参数: freq 频率值
返回值：  无					
******************************************************************************/
void FM_Freq_LED_Display(uint16_t freq)
{
  uint8_t i = 0;
	
	Led_Number.TenDigitsHour = freq/1000;
	Led_Number.UnitsDigitHour = (freq/100)%10;
	Led_Number.TenDigitsMinute = (freq%100)/10;
	Led_Number.UnitsDigitMinute = freq%10;
	
	if(Led_Number.TenDigitsHour == 0)
	{
    Led_Number.TenDigitsHour = 0x10;
  }
	
	for(i=0; i<7; i++)
	{
    Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsHour] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitHour] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
  }
	
	Led_Display_Table[2] |= 1;
	Led_Display_Table[6] |= 1;
	
	if(Alarm1_Num.Hour != ALARM_DISABLE || Alarm1_Num.Minute != ALARM_DISABLE)
	{
		Led_Display_Table[0] |= 1;
  }

	if(Alarm2_Num.Hour != ALARM_DISABLE || Alarm2_Num.Minute != ALARM_DISABLE)
  {
		Led_Display_Table[1] |= 1;
  }
	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
	
}



/******************************************************************************
函数名：	Volume_LED_Display
功能描述：显示FM当前频道
输入参数: channel 频道值
返回值：  无					
******************************************************************************/
void FM_Channel_LED_Display(uint8_t channel)
{
  uint8_t i = 0;
	
	Led_Number.TenDigitsHour = 0x01;
	if(channel>99)
	{
    channel = 99;
  }
	Led_Number.TenDigitsMinute = channel/10;
	Led_Number.UnitsDigitMinute = channel%10;
	
	for(i=0; i<7; i++)
	{
    Led_Display_Table[i] |= (Number_Table[0x0C] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_FMChannel >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.TenDigitsMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
		Led_Display_Table[i] |= (Number_Table[Led_Number.UnitsDigitMinute] >> i)&0x01;
		Led_Display_Table[i] <<= 1;
  }
	
	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
}


/******************************************************************************
函数名：	LED_Display_Off
功能描述：关闭LED显示
输入参数: 无
返回值：  无					
******************************************************************************/
void LED_Display_Off(void)
{
  uint8_t i = 0;
	
	for(i=0; i<7; i++)
	{   
		Led_Display_Table[i] <<= 4;
  }
	
	for(i=0; i<7; i++)
	{
    INDEX = i;
	  LXDAT = Led_Display_Table[i];
		Led_Display_Table[i] = 0;
  }
	
}


#endif

