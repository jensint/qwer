#ifndef _APP_TASK_C_
#define _APP_TASK_C_


#include "app_task.h"

#define 	 TS_KEY_JITTER_TIME    3    //3*10ms
#define 	 TS_KEY_CP_TIME    150    //100*10ms
#define 	 TS_KEY_CPH_TIME    30

uint8_t xdata Mode_Init_Flag = 0;

uint8_t xdata Alarm_Ring_Flag = 0;
uint16_t xdata Alarm_Ring_Time = 0; 
uint8_t xdata LEDW_PWM_Time = 0;
uint8_t xdata LEDW_PWM_Num = 0;
uint8_t xdata LEDW_PWM_Num_DOWM = 0;
//uint16_t code LEDW_PWM_Table[30] = {730,722,715,709,704,700,692,685,677,670,660,650,640,632,624,616,608,600,590,575,560,545,515,485,470,440,410,390,360,0};
uint16_t code LEDW_PWM_Table[36] = {18432,18000,17700,17400,17100,16800,16500,15200,14900,14600,14300,14000,13700,12400,12100,11800,11500,11200,10900,10600,10300,10000,9700,9400,9100,8900,8600,8300,8000,7700,7400,7000,5000,2000,1000,500};
uint16_t code LEDW_PWM_Table_DOWM[36] ={500,1000,1300,1600,1900,2200,2500,3000,3300,3600,4000,4300,4600,5000,5300,5600,6000,6300,6600,7000,7300,7600,8000,8300,8600,9000,9300,9600,10000,10300,10600,17100,17400,17700,18000,18432};
	
uint16_t xdata TS_KEY_JUDGE_TIME;

TASK_TIME xdata app_task_time;
SYS_STATE xdata sys_state;

uint8_t xdata FM_CHANGE_FlAG = 0;
uint8_t xdata FM_CHANGE_COUNT = 0;	

uint8_t xdata FM_INIT_COUNNT = 0;	
uint8_t xdata FM_INIT_STATE = 0;  //FM初始化状态 0:空闲 1:正在初始化 2:初始化完成
	
uint8_t xdata FM_AUTO_SEARCH_FLAG = 0;
uint16_t xdata FM_AUTO_FREQ = 0;	//自动搜台频点
uint16_t xdata FM_FREQ_AREA[32];
uint8_t xdata FM_AREA_NUM = 0;
	
uint16_t xdata TIME_DIV_NUM;	
int xdata LED_B_DutyDat=14000;	
uint16_t xdata LED_CIRCLE_FLAG=1;	

uint8_t xdata msg_ts_key = TS_KEY_NONE;
uint8_t xdata TS_LONG_FLAG=0;
uint8_t xdata TS_LONG_COUNT=0;
/******************************************************************************
函数名：	Gpio_Conf
功能描述：GPIO配置
输入参数: 无
返回值：  无					
******************************************************************************/
void Gpio_Conf(void)
{
	//POWER电源唤醒键
	GPIO_Init(POWER_PIN_TCON, INPUT);
	
	GPIO_Init(ADC_KEY_PIN_TCON, P13_ADC3_SETTING);

  //BTPOWER蓝牙电源控制脚
  GPIO_Init(BT_POWER_PIN_TCON, OUTPUT);
	BT_Power_Ctrl(BT_POWER_OFF);
	
//	GPIO_Init(LED_GREEN_PIN_TCON, PU_EN|OUTPUT);
//	LED_GREEN_PIN_DATA = 0;
	
	//LED接口
	GPIO_Init(LED_COM0, P00_COM0_SETTING);
	GPIO_Init(LED_COM1, P01_COM1_SETTING);
	GPIO_Init(LED_COM2, P02_COM2_SETTING);
	GPIO_Init(LED_COM3, P03_COM3_SETTING);
	GPIO_Init(LED_COM4, P04_COM4_SETTING);
	
	GPIO_Init(LED_SEG1, P17_SEG0_SETTING);
	GPIO_Init(LED_SEG2, P37_SEG1_SETTING);
	GPIO_Init(LED_SEG3, P36_SEG2_SETTING);
	GPIO_Init(LED_SEG4, P35_SEG3_SETTING);
	GPIO_Init(LED_SEG5, P07_SEG5_SETTING);
	GPIO_Init(LED_SEG6, P06_SEG6_SETTING);
	GPIO_Init(LED_SEG7, P05_SEG7_SETTING);

}


/******************************************************************************
函数名：	Sys_Parameter_Init
功能描述：系统参数初始化
输入参数: 无
返回值：  无					
******************************************************************************/
void Sys_Parameter_Init(void)
{
  sys_state.Volume = DEFAULT_VOLUME;
	sys_state.Power = SYS_POWER_IDLE;
	sys_state.Mode = SYS_IDLE;
	sys_state.RecentMode = SYS_IDLE;
	sys_state.SetTime = Unit_Null;
	sys_state.FMFreq = 875;
	
	Alarm1_Num.Hour = ALARM_DISABLE;
	Alarm1_Num.Minute = ALARM_DISABLE;
	Alarm2_Num.Hour = ALARM_DISABLE;
	Alarm2_Num.Minute = ALARM_DISABLE;
	sys_state.Alarm = ALARM_IDLE;
	sys_state.LEDWBrightness=0;
}


/******************************************************************************
函数名：	Set_Alarm_Time
功能描述：设置闹钟时间
输入参数: hour    小时
          minute  分钟
          second  秒
返回值：  无					
******************************************************************************/
void Set_Alarm_Time(uint8_t hour, uint8_t minute, uint8_t second)
{
  RTC_SetAlaramTime(hour, minute, second);
}
/***********************************
红灯渐变
***********************************/
void LED_CIRCLE_SHOW(void)
{
	if(LEDW_PWM_Time != 0) return;	
		if(sys_state.Mode != SYS_IDLE)
		{		 
				if(sys_state.LEDWBrightness == LEVEL_1)
			{
				if(LEDW_PWM_Num < 37)
				{
					INDEX = PWM_CH1;
					PWMDUTH	= (unsigned char)(LEDW_PWM_Table[LEDW_PWM_Num]>>8);
					PWMDUTL	= (unsigned char)(LEDW_PWM_Table[LEDW_PWM_Num]);
//				PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
					PWM_Update(1<<PWM_CH1);
					LEDW_PWM_Num++;
					LEDW_PWM_Time = 100;
				}
					if(LEDW_PWM_Num==37)
					{
						if(LEDW_PWM_Num_DOWM < 37)
						{
							INDEX = PWM_CH1;
							PWMDUTH	= (unsigned char)(LEDW_PWM_Table_DOWM[LEDW_PWM_Num_DOWM]>>8);
							PWMDUTL	= (unsigned char)(LEDW_PWM_Table_DOWM[LEDW_PWM_Num_DOWM]);
//						PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
							PWM_Update(1<<PWM_CH1);
							LEDW_PWM_Num_DOWM++;
							LEDW_PWM_Time = 100;		
						}
							if(LEDW_PWM_Num_DOWM==37)
							{
								LEDW_PWM_Num=0;
								LEDW_PWM_Num_DOWM=0;
							}
					}
					
			}
		}
}


/*********************************************
绿灯显示
*********************************************/
void RGB_GREEN_DIS(uint8_t flag)
{
	uint16_t data value=100;
	if(flag)
	{
		Uart0_Send_nData("green\n", 6);
		INDEX = PWM_CH3;
		PWMDUTH	= (unsigned char)(value>>8);
		PWMDUTL	= (unsigned char)(value);
		PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
		PWM_Update(1<<PWM_CH3);
	}
}
void RGB_RED_DIS_OFF(void)
{
	uint16_t data value=0;
	INDEX = PWM_CH1;
	PWMDUTH	= (unsigned char)(value>>8);
	PWMDUTL	= (unsigned char)(value);
	PWM_Update(1<<PWM_CH1);
}

/******************************************************************************
函数名：	BT_Power_Ctrl
功能描述：蓝牙供电控制
输入参数: bt_power_sw
返回值：  无					
******************************************************************************/
void BT_Power_Ctrl(BT_Power_TypeDef bt_power_sw)
{
  BT_POWER_PIN_DATA = bt_power_sw;
}


/******************************************************************************
函数名：	Power_Det
功能描述：电源开关检测
输入参数: 无
返回值：  power_status  0xFF:无效  0:关  1:开					
******************************************************************************/
uint8_t Power_Det(void)
{
	static uint8_t xdata PowerKeyState = 0;
	static uint8_t xdata PreKeyStatus = 0;
	static uint8_t xdata KeyDetCount = 0; 
	
	if(sys_state.Power == SYS_POWER_NORMAL)
	{
	  if(app_task_time.PowerDet >= 1)
	  {
      app_task_time.PowerDet = 0;
    }
	  else
	  {
      return 0xFF;
    }
	}
	
	switch(PowerKeyState)     //初始化，PowerKeyState=0
	{
    case 0:
			PreKeyStatus = POWER_PIN_DATA;    //检测到的POWER_PIN_DATA值赋给PreKeyStatus
			PowerKeyState = 1;
		  KeyDetCount = 0;
			break;
		
		case 1:
			if(PreKeyStatus != POWER_PIN_DATA)   //第二次检测，判断PreKeyStatus 是否为 POWER_PIN_DATA
			{
				PowerKeyState = 0;    // 如果不是，则无效
			}
			else
			{
				KeyDetCount++;
				if(KeyDetCount >= 5)
				{
//          sys_state.Power = POWER_PIN_DATA;
					PowerKeyState = 2;
					return POWER_PIN_DATA;
//					return sys_state.Power;
//					Uart0_Send_nData("POWER", 5);				
				}				
			}
			break;
		
		case 2:
			if(PreKeyStatus != POWER_PIN_DATA)
			{
        PowerKeyState = 0;
      }
			break;
		
		default:
			PowerKeyState = 0;
			break;
  }
	return 0xFF;
}

/******************************************************************************
函数名：	LEDW_Brightness_Ctrl
功能描述：白色LED灯亮度控制
输入参数: value
返回值：  无					
******************************************************************************/
void LEDW_Brightness_Ctrl(uint8_t level)
{
	uint16_t data value;
	value=level;
	
	switch(level)
	{
    case LEVEL_0:
			value = LEDW_BRIGHTNESS_OFF;
			break;
		
		case LEVEL_1:
			value = LEDW_BRIGHTNESS_1;
			break;
		
		case LEVEL_2:
			value = LEDW_BRIGHTNESS_2;
			break;
		
		case LEVEL_3:
			value = LEDW_BRIGHTNESS_3;
			break;
		
		
		case LEVEL_ALARM:
			
			break;
		
		default:
			break;
  }

//	Uart0_Send_nData("LEDWPWM", 7);
	INDEX = PWM_CH1;
	PWMDUTH	= (unsigned char)(value>>8);
	PWMDUTL	= (unsigned char)(value);
//	PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
	PWM_Update(1<<PWM_CH1);
}
void LEDW_Brightness_Ctrl_circle(uint8_t value)
{
	INDEX = PWM_CH1;
	PWMDUTH	= (unsigned char)(value>>8);
	PWMDUTL	= (unsigned char)(value);
//	PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
	PWM_Update(1<<PWM_CH1);
}

/******************************************************************************
函数名：	TSkey_Scan
功能描述：触摸按键扫描
输入参数: 无
返回值：  按键状态			
******************************************************************************/
uint8_t TSkey_Scan(void)
{
	static uint8_t xdata TSKeyState = TS_KEY_STATE_IDLE;
	uint8_t	xdata KeyState;
	
	KeyState = (uint8_t)(KeysFlagSN & 0x0001);
	
	switch(TSKeyState)
	{
    case TS_KEY_STATE_IDLE:
			if(KeyState)
			{
        TSKeyState = TS_KEY_STATE_JITTER;
				TS_KEY_JUDGE_TIME = TS_KEY_JITTER_TIME;
      }
			break;
		
		case TS_KEY_STATE_JITTER:
			if(KeyState)
			{//按键有效
				if(TS_KEY_JUDGE_TIME == 0)
				{
          TSKeyState = TS_KEY_STATE_PRESS_DOWN;
					TS_KEY_JUDGE_TIME = TS_KEY_CP_TIME;
				}				
      }
			else
			{//抖动信号,按键无效
        TSKeyState = TS_KEY_STATE_IDLE;
      }
			break;
		
		case TS_KEY_STATE_PRESS_DOWN:
			if(KeyState)
			{
        if(TS_KEY_JUDGE_TIME == 0)
				{//长按
          TSKeyState = TS_KEY_STATE_CP;
					TS_KEY_JUDGE_TIME = TS_KEY_CPH_TIME;
					return TS_KEY_LONG;
        }
      }
			else
			{//短按释放
				TSKeyState = TS_KEY_STATE_IDLE;
        return TS_KEY_SHORT;
      }
			break;
		
		case TS_KEY_STATE_CP:
			if(KeyState)
			{
        if(TS_KEY_JUDGE_TIME == 0)
				{//长按保持
					TS_KEY_JUDGE_TIME = TS_KEY_CPH_TIME;
          return TS_KEY_LONG_HOLD;
        }
      }
			else
			{//长按释放
				TSKeyState = TS_KEY_STATE_IDLE;
        return TS_KEY_LONG_RELEASE;
      }
			break;
			
		default:
			break;
  }
	
	return TS_KEY_NONE;
}


/******************************************************************************
函数名：	Key_Scan
功能描述：按键扫描处理
输入参数: 无
返回值：  无					
******************************************************************************/
void Key_Scan(void)
{
  static uint8_t MSG_MODE_SHORT_count=0;
	uint8_t xdata msg_adc_key = MSG_NONE;
//	uint8_t xdata msg_ts_key = TS_KEY_NONE;
	
	if(app_task_time.AdcKeyScan >= 1)   //扫描时间
	{
    app_task_time.AdcKeyScan = 0;  
  }
	else
	{
    return; 
  }
	
	msg_adc_key = Adc_Key_Sacn();
	
	if(msg_adc_key != MSG_NONE)
	{
    if(sys_state.Alarm == ALARM_1)
		{
			Mcu_SendCmdToBT(CMD_RING2);
			LEDW_Brightness_Ctrl(LEVEL_0);
      sys_state.Alarm = ALARM_IDLE;
    }
		else if(sys_state.Alarm == ALARM_2)
		{
			Mcu_SendCmdToBT(CMD_RING2);
			LEDW_Brightness_Ctrl(LEVEL_0);
      sys_state.Alarm = ALARM_IDLE;
    }
		
		if(Alarm_Ring_Flag)
		{
			Alarm_Ring_Flag = 0;
			BT_Uart.BTCmd_Temp = CMD_RING2;
		  BT_Uart.BTCmdResendFlag = 1;
//		  Mcu_SendCmdToBT(CMD_RING2);
		}
  }
	//处理按键
	switch(msg_adc_key)
	{
		case MSG_MODE_SHORT://模式切换
//			Uart0_Send_nData("MODE_ST", 7);
		  
		  if(sys_state.Mode == SYS_FM)
			{ 
				MSG_MODE_SHORT_count++;
				if(Mode_Init_Flag)
				{
					Mode_Init_Flag = 0;
          sys_state.Mode = SYS_BLUETOOTH;
				  BT_Uart.BTCmd_Temp = CMD_BT;
		      BT_Uart.BTCmdResendFlag = 1;
				}
      }
			else
			{
				if(sys_state.SetTime != Unit_Null)
				{//确认设置的时间值
					if(sys_state.Mode == SYS_ALARM1)
					{
            Alarm1_Num.Hour = Settime_Num.Hour;
						Alarm1_Num.Minute = Settime_Num.Minute;
						Alarm_LED_Display(SYS_ALARM1);
          }
					else if(sys_state.Mode == SYS_ALARM2)
					{
            Alarm2_Num.Hour = Settime_Num.Hour;
						Alarm2_Num.Minute = Settime_Num.Minute;
						Alarm_LED_Display(SYS_ALARM2);
          }
					else
					{	
						RTC_WriteSecond(0);
						RTC_WriteMinute(Settime_Num.Minute);
						RTC_WriteHour(Settime_Num.Hour);
          }
          sys_state.SetTime = Unit_Null;
        }
			  else if(sys_state.Mode == SYS_CLOCK)
			  {//切换到FM
          sys_state.Mode = SYS_FM;
				  BT_Uart.BTCmd_Temp = CMD_FM;
		      BT_Uart.BTCmdResendFlag = 1;
					FM_Freq_LED_Display(sys_state.FMFreq);
					to_RDA5807_Init(MSG_MODE_SHORT_count);
					//RDA5807_Init();			
        }
			  else if(sys_state.Mode == SYS_BLUETOOTH)
			  {
          sys_state.Mode = SYS_AUX;
				  BT_Uart.BTCmd_Temp = CMD_AUX;
		      BT_Uart.BTCmdResendFlag = 1;
        }
			  else if(sys_state.Mode == SYS_AUX)
			  {
          sys_state.Mode = SYS_ALARM1;
					BT_Uart.BTCmd_Temp = CMD_CLOCK;
		      BT_Uart.BTCmdResendFlag = 1;
				  Alarm_LED_Display(SYS_ALARM1);		
//					Mcu_SendCmdToBT(CMD_RING1);
//				  BT_Uart.BTCmd_Temp = CMD_RING1;
//		      BT_Uart.BTCmdResendFlag = 1;
        }
			  else if(sys_state.Mode == SYS_ALARM1)
			  {
          sys_state.Mode = SYS_ALARM2;
					BT_Uart.BTCmd_Temp = CMD_CLOCK;
		      BT_Uart.BTCmdResendFlag = 1;
				  Alarm_LED_Display(SYS_ALARM2);
//					Mcu_SendCmdToBT(CMD_RING2);
//					BT_Uart.BTCmd_Temp = CMD_RING2;
//		      BT_Uart.BTCmdResendFlag = 1;
        }
        else if(sys_state.Mode == SYS_ALARM2)
        {
          sys_state.Mode = SYS_CLOCK;
					BT_Uart.BTCmd_Temp = CMD_CLOCK;
		      BT_Uart.BTCmdResendFlag = 1;
        }
		  }			
			break;
		
		case MSG_MODE_LONG://设置时间
			if(sys_state.Mode == SYS_BLUETOOTH || sys_state.Mode == SYS_AUX || sys_state.Mode == SYS_CLOCK)
			{
				Settime_Num.Hour = RTC_Num.Hour;
				Settime_Num.Minute = RTC_Num.Minute;
			}
			else if(sys_state.Mode == SYS_ALARM1)
			{//设置闹钟1
        Settime_Num.Hour = Alarm1_Num.Hour;
				Settime_Num.Minute = Alarm1_Num.Minute;
      }
			else if(sys_state.Mode == SYS_ALARM2)
			{//设置闹钟2
        Settime_Num.Hour = Alarm2_Num.Hour;
				Settime_Num.Minute = Alarm2_Num.Minute;
      }
			
			if(sys_state.Mode != SYS_FM && sys_state.SetTime == Unit_Null)
			{
				Settime_Keeptime_Count = KEEPTIME_COUNT;  //5秒
        sys_state.SetTime = Unit_Hour;
      }
//			Uart0_Send_nData("MODE_LG", 7);
			break;
		
		case MSG_MODE_HOLD:
			
			Uart0_Send_nData("MODE_HD", 7);
			break;
		
		case MSG_VOL_DOWN_SHORT://
			if(sys_state.SetTime != Unit_Null)
			{
				Set_Time_Down(sys_state.SetTime);
      }
			else if(sys_state.Mode == SYS_BLUETOOTH)
			{//上一曲
				BT_Uart.BTCmd_Temp = CMD_PREV;
		    BT_Uart.BTCmdResendFlag = 1;
//				Mcu_SendCmdToBT(CMD_PREV);
//        Uart0_Send_nData("KEY_PRE", 7);
      }
			else if(sys_state.Mode == SYS_FM)
			{
        sys_state.FMFreq--;
				if(sys_state.FMFreq < 875)
				{
          sys_state.FMFreq = 1080;
        }
				FM_Freq_LED_Display(sys_state.FMFreq);
				RDA5807_FreqSet(sys_state.FMFreq);
//				FM_AUTO_SEARCH_FLAG = 1;
//				app_task_time.FMAutoSearch = 10;
      }
      else if(sys_state.Mode == SYS_CLOCK || sys_state.Mode == SYS_ALARM1)
			{//时间下调
//        Uart0_Send_nData("TIME_DN", 7);  
      }
			break;
		
		case MSG_VOL_DOWN_LONG:	
		case MSG_VOL_DOWN_HOLD:
			if(sys_state.SetTime != Unit_Null)
			{
				Set_Time_Down(sys_state.SetTime);
      }
			else if(sys_state.Mode == SYS_BLUETOOTH || sys_state.Mode == SYS_AUX || sys_state.Mode == SYS_FM)
			{//音量-
				if(Set_Volume(SET_VOLUME_DOWN))
				{
//          Mcu_SendCmdToBT(CMD_VOLUME);					
				}
				BT_Uart.BTCmd_Temp = CMD_VOLUME;
		    BT_Uart.BTCmdResendFlag = 1;
//				Mcu_SendCmdToBT(CMD_VOLUME);	
				sys_state.SetVolumeFlag = 1;
				SetVolume_Keeptime_Count = KEEPVOLUME_COUNT;
				Volume_LED_Display(sys_state.Volume);
      }
			break;
		
		case MSG_VOL_UP_SHORT:
			if(sys_state.SetTime != Unit_Null)
			{
				Set_Time_Up(sys_state.SetTime);
      }
			else if(sys_state.Mode == SYS_BLUETOOTH)
			{//下一曲
				BT_Uart.BTCmd_Temp = CMD_NEXT;
		    BT_Uart.BTCmdResendFlag = 1;
//				Mcu_SendCmdToBT(CMD_NEXT);
//        Uart0_Send_nData("KEY_NEXT", 8);  
      }
			else if(sys_state.Mode == SYS_FM)
			{
        sys_state.FMFreq++;
				if(sys_state.FMFreq > 1080)
				{
          sys_state.FMFreq = 875;
        }
				FM_Freq_LED_Display(sys_state.FMFreq);
				RDA5807_FreqSet(sys_state.FMFreq);
//				FM_AUTO_SEARCH_FLAG = 1;
//				app_task_time.FMAutoSearch = 10;
      }
      else if(sys_state.Mode == SYS_CLOCK || sys_state.Mode == SYS_ALARM1)
			{//时间上调
//        Uart0_Send_nData("TIME_UP", 7);  
      }
			break;
		
		case MSG_VOL_UP_LONG:
		case MSG_VOL_UP_HOLD:
			if(sys_state.SetTime != Unit_Null)
			{
				Set_Time_Up(sys_state.SetTime);
      }
			else if(sys_state.Mode == SYS_BLUETOOTH || sys_state.Mode == SYS_AUX || sys_state.Mode == SYS_FM)
			{//音量+
        if(Set_Volume(SET_VOLUME_UP))
				{
//          Mcu_SendCmdToBT(CMD_VOLUME);
				}
				BT_Uart.BTCmd_Temp = CMD_VOLUME;
		    BT_Uart.BTCmdResendFlag = 1;
//				Mcu_SendCmdToBT(CMD_VOLUME);
				sys_state.SetVolumeFlag = 1;
				SetVolume_Keeptime_Count = KEEPVOLUME_COUNT;
				Volume_LED_Display(sys_state.Volume);
      }
			break;
		
		case MSG_MFD_SHORT:
			if(sys_state.SetTime == Unit_Hour)
			{
				Settime_Keeptime_Count = KEEPTIME_COUNT;
        sys_state.SetTime = Unit_Minute;
      }
			else if(sys_state.SetTime == Unit_Minute)
			{
				Settime_Keeptime_Count = KEEPTIME_COUNT;
        sys_state.SetTime = Unit_Hour;
      }
			else if(sys_state.Mode == SYS_BLUETOOTH)
			{
				BT_Uart.BTCmd_Temp = CMD_PLAYPUSE;
		    BT_Uart.BTCmdResendFlag = 1;
//        Mcu_SendCmdToBT(CMD_PLAYPUSE);
      }
			else if(sys_state.Mode == SYS_FM)
			{
        if(FM_AUTO_SEARCH_FLAG)
				{//正在搜台,停止搜台
					FM_AUTO_SEARCH_FLAG = 0;
          if(sys_state.FM_CH_Num > 0)
					{
            sys_state.FMFreq = FM_FREQ_AREA[0];
						sys_state.FM_CH = 0;
						
						FM_Freq_LED_Display(sys_state.FMFreq);
			      RDA5807_FreqSet(sys_state.FMFreq);
          }
					else
					{
            sys_state.FM_CH_Num = 0;
          }
        }
				else
				{//切换保存的台
          FM_Channel_Up();
        }
      }
//			Uart0_Send_nData("MFD_SH", 6);
			break;
		
		case MSG_MFD_LONG://设置闹钟
//			if(sys_state.SetTime != Unit_Null)
//			{//取消时间设置
//        sys_state.SetTime = Unit_Null;
//				if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
//			  {
//			    Alarm_LED_Display(sys_state.Mode);
//			  }
//      }
		  if(sys_state.Mode == SYS_BLUETOOTH)
			{
        Mcu_SendCmdToBT(CMD_RECALL);
      }
			else if(sys_state.Mode == SYS_FM)    
			{      
				FM_AREA_NUM = 0;
				sys_state.FM_CH_Num = 0;
				sys_state.FMFreqTemp = sys_state.FMFreq;
				sys_state.FMFreq = 875;
//				FM_AUTO_FREQ = 875;
				RDA5807_FreqSet(sys_state.FMFreq);
				FM_Freq_LED_Display(sys_state.FMFreq);
				FM_AUTO_SEARCH_FLAG = 1;
				app_task_time.FMAutoSearch = 10;
      }
//			Uart0_Send_nData("MFD_LG", 6);
			break;
		
		case MSG_MFD_HOLD:
//			Uart0_Send_nData("MFD_HD", 6);
			break;
		
		default:
//			Uart0_Send_nData("KEY_NONE", 8);
			break;
  }
	
	msg_ts_key = TSkey_Scan();
	
	if(msg_ts_key != TS_KEY_NONE)
	{  		
    if(sys_state.Alarm == ALARM_1)
		{
//			Mcu_SendCmdToBT(CMD_RING2);
			LEDW_Brightness_Ctrl(LEVEL_0);
      sys_state.Alarm = ALARM_IDLE;
    }
		else if(sys_state.Alarm == ALARM_2)
		{			
//			Mcu_SendCmdToBT(CMD_RING2);
			LEDW_Brightness_Ctrl(LEVEL_0);
      sys_state.Alarm = ALARM_IDLE;
    }
		
		if(Alarm_Ring_Flag)
		{
			Alarm_Ring_Flag = 0;
			BT_Uart.BTCmd_Temp = CMD_RING2;
		  BT_Uart.BTCmdResendFlag = 1;
//		  Mcu_SendCmdToBT(CMD_RING2);
		}
  
	
	
	
	if(msg_ts_key == TS_KEY_SHORT)
	{//设置灯光亮度
//    Uart0_Send_nData("TS_KEY_SHORT\n", 13);
		if(sys_state.Mode != SYS_IDLE)
		{
			sys_state.LEDWBrightness++;
			Uart0_PutChar(sys_state.LEDWBrightness);	
			
				if(sys_state.LEDWBrightness > LEVEL_3)
				{
					sys_state.LEDWBrightness = LEVEL_0;
				}
				Mcu_SendCmdToBT(CMD_RING2);
//				Uart0_PutChar(sys_state.LEDWBrightness);
				if(sys_state.LEDWBrightness != LEVEL_1)
				{
					LEDW_Brightness_Ctrl(sys_state.LEDWBrightness);		
				}					
		}
	}
	else if(msg_ts_key == TS_KEY_LONG)
	{//蓝牙TWS功能
		TS_LONG_FLAG=1;
		TS_LONG_COUNT++;
		Uart0_PutChar(TS_LONG_COUNT);
		if(TS_LONG_COUNT==1)
		{
			Uart0_PutChar(TS_LONG_COUNT);
			Uart0_PutChar(TS_LONG_FLAG);
			RGB_RED_DIS_OFF();
//			RGB_GREEN_DIS(TS_LONG_FLAG);
		}
		else
		{
			TS_LONG_FLAG=0;
			TS_LONG_COUNT=0;
		}
		if(sys_state.Mode == SYS_BLUETOOTH)
	  {
			BT_Uart.BTCmd_Temp = CMD_TWS;
		  BT_Uart.BTCmdResendFlag = 1;
//      Mcu_SendCmdToBT(CMD_TWS);
    }
		
		
//		Mcu_SendCmdToBT(CMD_RING1);
//    Uart0_Send_nData("TS_KEY_LONG", 11);
  }

}
}

/******************************************************************************
函数名：	Uart0_Send_nData
功能描述：串口发送数据
输入参数: databuf      数据包
          data_length  数据长度
返回值：  无					
******************************************************************************/
void Uart0_Send_nData(uint8_t *databuf, uint8_t data_length)
{
  uint8_t n;
	
  for(n = 0; n < data_length; n++)
  {
    Uart0_PutChar(*databuf);
    databuf++;
  }
}



/******************************************************************************
函数名：	Set_Time_Up
功能描述：向上设置时间
输入参数: mode  设置小时/分钟
返回值：  无					
******************************************************************************/
void Set_Time_Up(CLOCK_UNIT mode)
{
  if(mode == Unit_Hour)
	{
		if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
		{
      if(Settime_Num.Hour == ALARM_DISABLE)
			{
        Settime_Num.Hour = 0;
				Settime_Num.Minute = 0;
      }
			else if(Settime_Num.Hour == 23)
			{
        Settime_Num.Hour = ALARM_DISABLE;
				Settime_Num.Minute = ALARM_DISABLE;
      }
			else
			{
				Settime_Num.Hour++;
      }
    }
		else
		{
      Settime_Num.Hour++;
			if(Settime_Num.Hour == 24)
			{
        Settime_Num.Hour = 0;
      }
    }
	}
	else if(mode == Unit_Minute)
	{
    Settime_Num.Minute++;
		if(Settime_Num.Minute >= 60)
		{
      Settime_Num.Minute = 0;
    }
  }

	Settime_Keeptime_Count = KEEPTIME_COUNT;  //5秒
	Settime_Dis_Count = DIS_COUNT;  //1.5秒
  Settime_LED_Display(sys_state.Mode, sys_state.SetTime, 1);
}


/******************************************************************************
函数名：	Set_Time_Down
功能描述：向下设置时间
输入参数: mode  设置小时/分钟
返回值：  无					
******************************************************************************/
void Set_Time_Down(CLOCK_UNIT mode)
{
  if(mode == Unit_Hour)
	{
		if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
		{
      if(Settime_Num.Hour == 0)
			{
        Settime_Num.Hour = ALARM_DISABLE;
				Settime_Num.Minute = ALARM_DISABLE;
      }
			else if(Settime_Num.Hour == ALARM_DISABLE)
			{
        Settime_Num.Hour = 23;
				Settime_Num.Minute = 0;
      }
			else
			{
				Settime_Num.Hour--;
      }
    }
		else
		{
			if(Settime_Num.Hour == 0)
			{
        Settime_Num.Hour = 23;
      }
			else
			{
        Settime_Num.Hour--;
      }
    }
	}
	else if(mode == Unit_Minute)
	{       
		if(Settime_Num.Minute == 0)
		{
      Settime_Num.Minute = 60;
    }
    Settime_Num.Minute--;
  }
	
	Settime_Keeptime_Count = KEEPTIME_COUNT;  //5秒
	Settime_Dis_Count = DIS_COUNT;  //1.5秒
	Settime_LED_Display(sys_state.Mode, sys_state.SetTime, 1);
}



/******************************************************************************
函数名：	Set_Volume
功能描述：设置音量
输入参数: set_mode  上调/下调
返回值：  0 已经是最大或者最小音量    1:音量调节成功
******************************************************************************/
uint8_t Set_Volume(_SET_VOLUME set_mode)
{
  if(set_mode == SET_VOLUME_DOWN)
	{
    if(sys_state.Volume > 0)
		{
      sys_state.Volume--;
			return 1;
    }
  }
	else if(set_mode == SET_VOLUME_UP)
	{
    if(sys_state.Volume < MAX_VOLUME)
		{
      sys_state.Volume++;
			return 1;
    }
  }
	return 0;
}


/******************************************************************************
函数名：	
功能描述：闹钟
输入参数: 
返回值：  
******************************************************************************/
void Set_Alarm(void)
{
	if(Alarm1_Num.Hour == ALARM_DISABLE && Alarm2_Num.Hour == ALARM_DISABLE) return;
	
	if(sys_state.Alarm != ALARM_IDLE) return;
	
	if(RTC_Num.Second != 0) return;
	

	if(Alarm1_Num.Hour != ALARM_DISABLE)
	{//设置闹钟1
    if((Alarm1_Num.Hour == RTC_Num.Hour) && (Alarm1_Num.Minute == RTC_Num.Minute))
		{//闹钟1
//      Mcu_SendCmdToBT(CMD_RING1);
			Uart0_Send_nData("IS_RING1", 8);
			sys_state.Alarm = ALARM_1;
			LEDW_PWM_Num = 0;
			Alarm_Ring_Time = 6000;
    }
  }
	
	if(Alarm2_Num.Hour != ALARM_DISABLE)
	{//设置闹钟2
    if((Alarm2_Num.Hour == RTC_Num.Hour) && (Alarm2_Num.Minute == RTC_Num.Minute))
		{//闹钟2
//      Mcu_SendCmdToBT(CMD_RING1);
			Uart0_Send_nData("IS_RING2", 8);
			sys_state.Alarm = ALARM_2;
			LEDW_PWM_Num = 0;
			Alarm_Ring_Time = 6000;
    }
  }
}


/******************************************************************************
函数名：	
功能描述：闹钟灯光控制
输入参数: 
返回值：  
******************************************************************************/
void Alarm_LED_Ctrl(void)
{
	if(LEDW_PWM_Time != 0) return;
	
  if(sys_state.Alarm != ALARM_IDLE)
	{
		if(LEDW_PWM_Num < 30)
		{
//		Uart0_Send_nData("ALARM", 5);
			INDEX = PWM_CH1;
	    PWMDUTH	= (unsigned char)(LEDW_PWM_Table[LEDW_PWM_Num]>>8);
	    PWMDUTL	= (unsigned char)(LEDW_PWM_Table[LEDW_PWM_Num]);
//	PWM_CfgDivDuty(PWM_CH1,LEDW_PWM_FREQ,value);
	    PWM_Update(1<<PWM_CH1);
			LEDW_PWM_Num++;
			LEDW_PWM_Time = 10;
    }   
  }
}

/******************************************************************************
函数名：	FM_Init
功能描述：FM初始化
输入参数: 无
返回值：  无
******************************************************************************/
void FM_Init(void)
{
	if((FM_INIT_STATE == 1)&&(FM_INIT_COUNNT == 0))
	{
		FM_Freq_LED_Display(sys_state.FMFreq);
		RDA5807_Init();
		FM_INIT_STATE = 2;
	}

}


/******************************************************************************
函数名：	FM_Auto_Scan
功能描述：FM搜台
输入参数: 无
返回值：  无
******************************************************************************/
void FM_Auto_Scan(void)
{
	static uint8_t fm_keep_flag = 0;
  if(sys_state.Mode != SYS_FM)
	{
		fm_keep_flag = 0;
		FM_AUTO_SEARCH_FLAG = 0;
    return;
  }
	
	if(FM_AUTO_SEARCH_FLAG)  //长按时MFD键：FM_AUTO_SEARCH_FLAG=1
	{
    if(app_task_time.FMAutoSearch == 0)		//长按时：app_task_time.FMAutoSearch=10;
		{
//				FM_AUTO_SEARCH_FLAG = 0;
			if(fm_keep_flag)
	    {
				fm_keep_flag = 0;
				
				sys_state.FMFreq++;
//        FM_AUTO_FREQ++;
//		    if(FM_AUTO_FREQ > 1080)
				if(sys_state.FMFreq > 1080)
        {
//          Uart0_Send_nData("FM_OVER", 7);
          FM_AUTO_SEARCH_FLAG = 0;
			    if(FM_AREA_NUM > 0)
			    {
				    sys_state.FMFreq = FM_FREQ_AREA[0];
            sys_state.FM_CH = 0;						
          }
					else
					{
            sys_state.FMFreq = sys_state.FMFreqTemp;
          }
			    FM_Freq_LED_Display(sys_state.FMFreq);
			    RDA5807_FreqSet(sys_state.FMFreq);
        }
		    else
		    {
          RDA5807_FreqSet(FM_AUTO_FREQ);
					FM_Freq_LED_Display(FM_AUTO_FREQ);
					app_task_time.FMAutoSearch = 10;
        }
      }
			else if(RDA5807_Auto_Search())
			{
//				Uart0_Send_nData("FM_SUCC", 7);
///*
//				FM_FREQ_AREA[FM_AREA_NUM] = FM_AUTO_FREQ;			
				FM_FREQ_AREA[FM_AREA_NUM] = sys_state.FMFreq;    //把此电台频率放到	FM_FREQ_AREA[FM_AREA_NUM]中
        FM_AREA_NUM++;																		//电台数+1
				sys_state.FM_CH_Num = FM_AREA_NUM;
				FM_Channel_LED_Display(FM_AREA_NUM);							//显示电台的个数
				fm_keep_flag = 1;																
				app_task_time.FMAutoSearch = 100;
//*/
      }
			else
			{		
//				Uart0_Send_nData("FM_FAIL", 7);
///*					
				sys_state.FMFreq++;																	//如果没有搜索到台，频率增加
//				FM_AUTO_FREQ++;
//				if(FM_AUTO_FREQ > 1080)
				if(sys_state.FMFreq > 1080)												
				{
//          Uart0_Send_nData("FM_OVER", 7);
          FM_AUTO_SEARCH_FLAG = 0;													//如果电台频率>1080时，停止自动搜台
				  if(FM_AREA_NUM > 0)
					{																								//如果搜到的电台数大于0，则把第一个频率赋给sys_state.FMFfrep
						sys_state.FMFreq = FM_FREQ_AREA[0];							//FM_FREQ_AREA存储搜索存到的电台
            sys_state.FM_CH = 0;						
          }
					else
					{
            sys_state.FMFreq = sys_state.FMFreqTemp;    //如果没有台，则把870赋给sys_state.FMFrep
          }
					
					FM_Freq_LED_Display(sys_state.FMFreq);								
			    RDA5807_FreqSet(sys_state.FMFreq);
        }
				else
				{
//					RDA5807_FreqSet(FM_AUTO_FREQ);
//					FM_Freq_LED_Display(FM_AUTO_FREQ);
					RDA5807_FreqSet(sys_state.FMFreq);										//设置该频率下的电台
					FM_Freq_LED_Display(sys_state.FMFreq);
          app_task_time.FMAutoSearch = 10;
				}
//*/
      }
    }
  }
}


void FM_Channel_Up(void)
{
  if(sys_state.FM_CH_Num > 0)
	{
    if(sys_state.FM_CH_Num > (sys_state.FM_CH + 1))
		{
      sys_state.FM_CH++;
    }
		else 
		{
      sys_state.FM_CH = 0;
    }
		sys_state.FMFreq = FM_FREQ_AREA[sys_state.FM_CH];
		FM_Channel_LED_Display(sys_state.FM_CH + 1);
		FM_CHANGE_FlAG = 1;
		FM_CHANGE_COUNT = 100;
//		FM_Freq_LED_Display(sys_state.FMFreq);
//		RDA5807_FreqSet(sys_state.FMFreq);
  }
}


void FM_CH_Change(void)
{
  if(FM_CHANGE_FlAG && (FM_CHANGE_COUNT == 0))
	{
    FM_CHANGE_FlAG = 0;
    FM_Freq_LED_Display(sys_state.FMFreq);
  	RDA5807_FreqSet(sys_state.FMFreq);		
  }
}


void Enter_Sleep_Conf(void)
{
//  WDCON |= (7<<5); //关闭看门狗 

//	INT8EN = 0;  //RTC中断关闭
	
	GPIO_Init(P31F,OUTPUT);
	GPIO_Init(P30F,OUTPUT);
	P31 = 0;
	P30 = 0;
	
	SetOutputSCL();
	SetOutputSDA();
	PLL_SCL(0);
	PLL_SDA(0);
	
	GPIO_Init(P16F, OUTPUT);  //关闭PWM功能
	P16 = 0;
	
	Sys_Clk_Set_XOSCL();  //切换主时钟到XOSCL
	
	//LED接口
	GPIO_Init(LED_COM0, INPUT);
	GPIO_Init(LED_COM1, INPUT);
	GPIO_Init(LED_COM2, INPUT);
	GPIO_Init(LED_COM3, INPUT);
	GPIO_Init(LED_COM4, INPUT);
	
	GPIO_Init(LED_SEG1, INPUT);
	GPIO_Init(LED_SEG2, INPUT);
	GPIO_Init(LED_SEG3, INPUT);
	GPIO_Init(LED_SEG4, INPUT);
	GPIO_Init(LED_SEG5, INPUT);
	GPIO_Init(LED_SEG6, INPUT);
	GPIO_Init(LED_SEG7, INPUT);
	
//	P0 = 0x00;
//	P17 = 0;
//	P35 = 0;
//	P36 = 0;
//	P37 = 0;
	
	LED_init(LEN_DISABLE, COM_H, SEG_L, LDRV_7, 2);  //关闭LED模块
	PWMEN &= ~(1<<PWM_CH1);  //关闭PWM通道
	
	Timer0_Init(TIMER_Mode0, TIMER_Func_Timing, Enable, TIMER_Gate_Mode_Tr, CL_TIMER_DIV);
	TIME_DIV_NUM = CL_TIMER_DIV;
	
	CKCON = 0;  //关闭除主时钟外的所有时钟
	PWCON = (PWCON&0xF0)|0x04; //设置LDO进入低功率模式,LDO输出电压为最大
	
	
//	MECON |= (1<<6);
//	while(STPSTH|STPSTL);
//	PCON |= 0x02;
//	_nop_();
//	_nop_();
//	_nop_();
}


void Quit_Sleep_Conf(void)
{
	Sys_Clk_Set_IRCH();  //切换主时钟到IRCH
	
	CKCON |= XLCKE;
	while(!(CKCON & XLSTA));
	PWCON = (PWCON&0xF0)|0x0C;  //LDO高功率模式,输出电压1.58V
	
//	INT8EN = 1;
	
	Timer0_Init(TIMER_Mode0, TIMER_Func_Timing, Enable, TIMER_Gate_Mode_Tr, TIMER_DIV);
	TIME_DIV_NUM = TIMER_DIV;

	//LED接口
	GPIO_Init(LED_COM0, P00_COM0_SETTING);
	GPIO_Init(LED_COM1, P01_COM1_SETTING);
	GPIO_Init(LED_COM2, P02_COM2_SETTING);
	GPIO_Init(LED_COM3, P03_COM3_SETTING);
	GPIO_Init(LED_COM4, P04_COM4_SETTING);
	
	GPIO_Init(LED_SEG1, P17_SEG0_SETTING);
	GPIO_Init(LED_SEG2, P37_SEG1_SETTING);
	GPIO_Init(LED_SEG3, P36_SEG2_SETTING);
	GPIO_Init(LED_SEG4, P35_SEG3_SETTING);
	GPIO_Init(LED_SEG5, P07_SEG5_SETTING);
	GPIO_Init(LED_SEG6, P06_SEG6_SETTING);
	GPIO_Init(LED_SEG7, P05_SEG7_SETTING);
	LED_init(LEN_IRCH, COM_H, SEG_L, LDRV_7, 2);  //LED模块
	
  GPIO_Init(P31F,P31_UART0_RX_SETTING);
	GPIO_Init(P30F,P30_UART0_TX_SETTING);
	
	GPIO_Init(P16F, P16_PWM1_SETTING);
	GPIO_Init(P34F, P34_PWM3_SETTING);
	PWM_EnContrl(1<<PWM_CH1);
//	SetOutputSCL();
//	SetOutputSDA();
//	PLL_SCL(0);
//	PLL_SDA(0);
}

#endif


