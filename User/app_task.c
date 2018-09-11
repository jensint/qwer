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
uint8_t xdata FM_INIT_STATE = 0;  //FM��ʼ��״̬ 0:���� 1:���ڳ�ʼ�� 2:��ʼ�����
	
uint8_t xdata FM_AUTO_SEARCH_FLAG = 0;
uint16_t xdata FM_AUTO_FREQ = 0;	//�Զ���̨Ƶ��
uint16_t xdata FM_FREQ_AREA[32];
uint8_t xdata FM_AREA_NUM = 0;
	
uint16_t xdata TIME_DIV_NUM;	
int xdata LED_B_DutyDat=14000;	
uint16_t xdata LED_CIRCLE_FLAG=1;	

uint8_t xdata msg_ts_key = TS_KEY_NONE;
uint8_t xdata TS_LONG_FLAG=0;
uint8_t xdata TS_LONG_COUNT=0;
/******************************************************************************
��������	Gpio_Conf
����������GPIO����
�������: ��
����ֵ��  ��					
******************************************************************************/
void Gpio_Conf(void)
{
	//POWER��Դ���Ѽ�
	GPIO_Init(POWER_PIN_TCON, INPUT);
	
	GPIO_Init(ADC_KEY_PIN_TCON, P13_ADC3_SETTING);

  //BTPOWER������Դ���ƽ�
  GPIO_Init(BT_POWER_PIN_TCON, OUTPUT);
	BT_Power_Ctrl(BT_POWER_OFF);
	
//	GPIO_Init(LED_GREEN_PIN_TCON, PU_EN|OUTPUT);
//	LED_GREEN_PIN_DATA = 0;
	
	//LED�ӿ�
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
��������	Sys_Parameter_Init
����������ϵͳ������ʼ��
�������: ��
����ֵ��  ��					
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
��������	Set_Alarm_Time
������������������ʱ��
�������: hour    Сʱ
          minute  ����
          second  ��
����ֵ��  ��					
******************************************************************************/
void Set_Alarm_Time(uint8_t hour, uint8_t minute, uint8_t second)
{
  RTC_SetAlaramTime(hour, minute, second);
}
/***********************************
��ƽ���
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
�̵���ʾ
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
��������	BT_Power_Ctrl
���������������������
�������: bt_power_sw
����ֵ��  ��					
******************************************************************************/
void BT_Power_Ctrl(BT_Power_TypeDef bt_power_sw)
{
  BT_POWER_PIN_DATA = bt_power_sw;
}


/******************************************************************************
��������	Power_Det
������������Դ���ؼ��
�������: ��
����ֵ��  power_status  0xFF:��Ч  0:��  1:��					
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
	
	switch(PowerKeyState)     //��ʼ����PowerKeyState=0
	{
    case 0:
			PreKeyStatus = POWER_PIN_DATA;    //��⵽��POWER_PIN_DATAֵ����PreKeyStatus
			PowerKeyState = 1;
		  KeyDetCount = 0;
			break;
		
		case 1:
			if(PreKeyStatus != POWER_PIN_DATA)   //�ڶ��μ�⣬�ж�PreKeyStatus �Ƿ�Ϊ POWER_PIN_DATA
			{
				PowerKeyState = 0;    // ������ǣ�����Ч
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
��������	LEDW_Brightness_Ctrl
������������ɫLED�����ȿ���
�������: value
����ֵ��  ��					
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
��������	TSkey_Scan
������������������ɨ��
�������: ��
����ֵ��  ����״̬			
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
			{//������Ч
				if(TS_KEY_JUDGE_TIME == 0)
				{
          TSKeyState = TS_KEY_STATE_PRESS_DOWN;
					TS_KEY_JUDGE_TIME = TS_KEY_CP_TIME;
				}				
      }
			else
			{//�����ź�,������Ч
        TSKeyState = TS_KEY_STATE_IDLE;
      }
			break;
		
		case TS_KEY_STATE_PRESS_DOWN:
			if(KeyState)
			{
        if(TS_KEY_JUDGE_TIME == 0)
				{//����
          TSKeyState = TS_KEY_STATE_CP;
					TS_KEY_JUDGE_TIME = TS_KEY_CPH_TIME;
					return TS_KEY_LONG;
        }
      }
			else
			{//�̰��ͷ�
				TSKeyState = TS_KEY_STATE_IDLE;
        return TS_KEY_SHORT;
      }
			break;
		
		case TS_KEY_STATE_CP:
			if(KeyState)
			{
        if(TS_KEY_JUDGE_TIME == 0)
				{//��������
					TS_KEY_JUDGE_TIME = TS_KEY_CPH_TIME;
          return TS_KEY_LONG_HOLD;
        }
      }
			else
			{//�����ͷ�
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
��������	Key_Scan
��������������ɨ�账��
�������: ��
����ֵ��  ��					
******************************************************************************/
void Key_Scan(void)
{
  static uint8_t MSG_MODE_SHORT_count=0;
	uint8_t xdata msg_adc_key = MSG_NONE;
//	uint8_t xdata msg_ts_key = TS_KEY_NONE;
	
	if(app_task_time.AdcKeyScan >= 1)   //ɨ��ʱ��
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
	//������
	switch(msg_adc_key)
	{
		case MSG_MODE_SHORT://ģʽ�л�
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
				{//ȷ�����õ�ʱ��ֵ
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
			  {//�л���FM
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
		
		case MSG_MODE_LONG://����ʱ��
			if(sys_state.Mode == SYS_BLUETOOTH || sys_state.Mode == SYS_AUX || sys_state.Mode == SYS_CLOCK)
			{
				Settime_Num.Hour = RTC_Num.Hour;
				Settime_Num.Minute = RTC_Num.Minute;
			}
			else if(sys_state.Mode == SYS_ALARM1)
			{//��������1
        Settime_Num.Hour = Alarm1_Num.Hour;
				Settime_Num.Minute = Alarm1_Num.Minute;
      }
			else if(sys_state.Mode == SYS_ALARM2)
			{//��������2
        Settime_Num.Hour = Alarm2_Num.Hour;
				Settime_Num.Minute = Alarm2_Num.Minute;
      }
			
			if(sys_state.Mode != SYS_FM && sys_state.SetTime == Unit_Null)
			{
				Settime_Keeptime_Count = KEEPTIME_COUNT;  //5��
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
			{//��һ��
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
			{//ʱ���µ�
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
			{//����-
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
			{//��һ��
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
			{//ʱ���ϵ�
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
			{//����+
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
				{//������̨,ֹͣ��̨
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
				{//�л������̨
          FM_Channel_Up();
        }
      }
//			Uart0_Send_nData("MFD_SH", 6);
			break;
		
		case MSG_MFD_LONG://��������
//			if(sys_state.SetTime != Unit_Null)
//			{//ȡ��ʱ������
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
	{//���õƹ�����
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
	{//����TWS����
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
��������	Uart0_Send_nData
�������������ڷ�������
�������: databuf      ���ݰ�
          data_length  ���ݳ���
����ֵ��  ��					
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
��������	Set_Time_Up
������������������ʱ��
�������: mode  ����Сʱ/����
����ֵ��  ��					
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

	Settime_Keeptime_Count = KEEPTIME_COUNT;  //5��
	Settime_Dis_Count = DIS_COUNT;  //1.5��
  Settime_LED_Display(sys_state.Mode, sys_state.SetTime, 1);
}


/******************************************************************************
��������	Set_Time_Down
������������������ʱ��
�������: mode  ����Сʱ/����
����ֵ��  ��					
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
	
	Settime_Keeptime_Count = KEEPTIME_COUNT;  //5��
	Settime_Dis_Count = DIS_COUNT;  //1.5��
	Settime_LED_Display(sys_state.Mode, sys_state.SetTime, 1);
}



/******************************************************************************
��������	Set_Volume
������������������
�������: set_mode  �ϵ�/�µ�
����ֵ��  0 �Ѿ�����������С����    1:�������ڳɹ�
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
��������	
��������������
�������: 
����ֵ��  
******************************************************************************/
void Set_Alarm(void)
{
	if(Alarm1_Num.Hour == ALARM_DISABLE && Alarm2_Num.Hour == ALARM_DISABLE) return;
	
	if(sys_state.Alarm != ALARM_IDLE) return;
	
	if(RTC_Num.Second != 0) return;
	

	if(Alarm1_Num.Hour != ALARM_DISABLE)
	{//��������1
    if((Alarm1_Num.Hour == RTC_Num.Hour) && (Alarm1_Num.Minute == RTC_Num.Minute))
		{//����1
//      Mcu_SendCmdToBT(CMD_RING1);
			Uart0_Send_nData("IS_RING1", 8);
			sys_state.Alarm = ALARM_1;
			LEDW_PWM_Num = 0;
			Alarm_Ring_Time = 6000;
    }
  }
	
	if(Alarm2_Num.Hour != ALARM_DISABLE)
	{//��������2
    if((Alarm2_Num.Hour == RTC_Num.Hour) && (Alarm2_Num.Minute == RTC_Num.Minute))
		{//����2
//      Mcu_SendCmdToBT(CMD_RING1);
			Uart0_Send_nData("IS_RING2", 8);
			sys_state.Alarm = ALARM_2;
			LEDW_PWM_Num = 0;
			Alarm_Ring_Time = 6000;
    }
  }
}


/******************************************************************************
��������	
�������������ӵƹ����
�������: 
����ֵ��  
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
��������	FM_Init
����������FM��ʼ��
�������: ��
����ֵ��  ��
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
��������	FM_Auto_Scan
����������FM��̨
�������: ��
����ֵ��  ��
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
	
	if(FM_AUTO_SEARCH_FLAG)  //����ʱMFD����FM_AUTO_SEARCH_FLAG=1
	{
    if(app_task_time.FMAutoSearch == 0)		//����ʱ��app_task_time.FMAutoSearch=10;
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
				FM_FREQ_AREA[FM_AREA_NUM] = sys_state.FMFreq;    //�Ѵ˵�̨Ƶ�ʷŵ�	FM_FREQ_AREA[FM_AREA_NUM]��
        FM_AREA_NUM++;																		//��̨��+1
				sys_state.FM_CH_Num = FM_AREA_NUM;
				FM_Channel_LED_Display(FM_AREA_NUM);							//��ʾ��̨�ĸ���
				fm_keep_flag = 1;																
				app_task_time.FMAutoSearch = 100;
//*/
      }
			else
			{		
//				Uart0_Send_nData("FM_FAIL", 7);
///*					
				sys_state.FMFreq++;																	//���û��������̨��Ƶ������
//				FM_AUTO_FREQ++;
//				if(FM_AUTO_FREQ > 1080)
				if(sys_state.FMFreq > 1080)												
				{
//          Uart0_Send_nData("FM_OVER", 7);
          FM_AUTO_SEARCH_FLAG = 0;													//�����̨Ƶ��>1080ʱ��ֹͣ�Զ���̨
				  if(FM_AREA_NUM > 0)
					{																								//����ѵ��ĵ�̨������0����ѵ�һ��Ƶ�ʸ���sys_state.FMFfrep
						sys_state.FMFreq = FM_FREQ_AREA[0];							//FM_FREQ_AREA�洢�����浽�ĵ�̨
            sys_state.FM_CH = 0;						
          }
					else
					{
            sys_state.FMFreq = sys_state.FMFreqTemp;    //���û��̨�����870����sys_state.FMFrep
          }
					
					FM_Freq_LED_Display(sys_state.FMFreq);								
			    RDA5807_FreqSet(sys_state.FMFreq);
        }
				else
				{
//					RDA5807_FreqSet(FM_AUTO_FREQ);
//					FM_Freq_LED_Display(FM_AUTO_FREQ);
					RDA5807_FreqSet(sys_state.FMFreq);										//���ø�Ƶ���µĵ�̨
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
//  WDCON |= (7<<5); //�رտ��Ź� 

//	INT8EN = 0;  //RTC�жϹر�
	
	GPIO_Init(P31F,OUTPUT);
	GPIO_Init(P30F,OUTPUT);
	P31 = 0;
	P30 = 0;
	
	SetOutputSCL();
	SetOutputSDA();
	PLL_SCL(0);
	PLL_SDA(0);
	
	GPIO_Init(P16F, OUTPUT);  //�ر�PWM����
	P16 = 0;
	
	Sys_Clk_Set_XOSCL();  //�л���ʱ�ӵ�XOSCL
	
	//LED�ӿ�
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
	
	LED_init(LEN_DISABLE, COM_H, SEG_L, LDRV_7, 2);  //�ر�LEDģ��
	PWMEN &= ~(1<<PWM_CH1);  //�ر�PWMͨ��
	
	Timer0_Init(TIMER_Mode0, TIMER_Func_Timing, Enable, TIMER_Gate_Mode_Tr, CL_TIMER_DIV);
	TIME_DIV_NUM = CL_TIMER_DIV;
	
	CKCON = 0;  //�رճ���ʱ���������ʱ��
	PWCON = (PWCON&0xF0)|0x04; //����LDO����͹���ģʽ,LDO�����ѹΪ���
	
	
//	MECON |= (1<<6);
//	while(STPSTH|STPSTL);
//	PCON |= 0x02;
//	_nop_();
//	_nop_();
//	_nop_();
}


void Quit_Sleep_Conf(void)
{
	Sys_Clk_Set_IRCH();  //�л���ʱ�ӵ�IRCH
	
	CKCON |= XLCKE;
	while(!(CKCON & XLSTA));
	PWCON = (PWCON&0xF0)|0x0C;  //LDO�߹���ģʽ,�����ѹ1.58V
	
//	INT8EN = 1;
	
	Timer0_Init(TIMER_Mode0, TIMER_Func_Timing, Enable, TIMER_Gate_Mode_Tr, TIMER_DIV);
	TIME_DIV_NUM = TIMER_DIV;

	//LED�ӿ�
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
	LED_init(LEN_IRCH, COM_H, SEG_L, LDRV_7, 2);  //LEDģ��
	
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


