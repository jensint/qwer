#ifndef _MAIN_C_
#define _MAIN_C_

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



#include <intrins.h>
#include "app_config.h"
#include "app_task.h"


uint8_t xdata Power_Det_State;
uint8_t POWER_STATE;
/***
void PWM3_init(void)
{
	P34F=6;
	INDEX=PWM_CH3;
	PWMCON=TIE(0)|ZIE(0)|PIE(0)|NIE(0)|MS(0)|CKS_IH;
	PWMCFG=TOG(1)|0;
	PWMDIVH=0;
	PWMDIVL=123;
	PWMDUTH=0;
	PWMDUTL=37;
	PWMUPD |= (1<<PWM_CH3);
	while(PWMUPD);
	PWMEN |= (1<<PWM_CH3);
}
**/

void main(void)
{
  Sys_Clk_Set_IRCH();  //系统时钟为IRCH
//	Sys_Clk_Set_XOSCL();
	//打开XOSCL
#if (IC_TYPE == IC_TYPE_CA51F3X)
	GPIO_Init(P32F,P32_XOSCL_IN_SETTING);
	GPIO_Init(P33F,P33_XOSCL_OUT_SETTING);
#elif  (IC_TYPE == IC_TYPE_CA51F2X)
	GPIO_Init(P71F,P71_XOSCL_OUT_SETTING);
	GPIO_Init(P72F,P72_XOSCL_IN_SETTING);
#endif
	CKCON |= XLCKE;
	while(!(CKCON & XLSTA));    //等待时钟稳定
	
	I2CCON = 0;
	
//	WDT_init(WDTS_IRCH, WDRE_reset, 0x0708); //看门狗时钟为IRCH,复位模式,看门狗时间为1秒
	WDT_init(WDTS_XOSCL, WDRE_reset, 132);  //看门狗时钟为XOSCL(32.768KHz),复位模式,看门狗时间为1秒
	WDT_FeedDog();  //刷新看门狗
	
	Gpio_Conf();  //IO口配置
	
	Uart0_Initial(19200);  //串口0 38400
	
	GPIO_Init(P34F, P34_PWM3_SETTING);
	GPIO_Init(P16F, P16_PWM1_SETTING);
		
	PWM_init(PWM_CH1, Edge_Align, CKS_IH, Toggle_On, 0, Complementary_Off);   //输出反向开，不进入互补输出
	PWM_init(PWM_CH3, Edge_Align, CKS_IH, Toggle_On, 0, Complementary_Off);   //输出反向开，不进入互补输出
	
	PWM_CfgDivDuty(PWM_CH1, LEDW_PWM_FREQ, LEDW_PWM_FREQ);  //频率为5K,占空比100%
	PWM_CfgDivDuty(PWM_CH3, LEDW_PWM_FREQ, LEDW_PWM_FREQ);  //频率为5K,占空比100
	
	PWM_Update((1<<PWM_CH1)|(1<<PWM_CH3));			//当改写了PWMDIV、PWMDUT、PWMCKD寄存器时,寄存器PWMUPD必须置位才能更新数据，数据更新后，PWMUPD相应位自动清零	
//	PWM_Update(1<<PWM_CH3);
	
	WaitPWMUpdateComplete();     		//等待更新完成

	PWM_EnContrl((1<<PWM_CH1)|(1<<PWM_CH3));				//PWM1使能
//	PWM_EnContrl(1<<PWM_CH3);				//PWM3使能
/***************************************************
PWM3配置
***************************************************/


	ADC_init(ADC_REF_VDD, Disable, Disable, NO_AMP);
	
	Timer0_Init(TIMER_Mode0, TIMER_Func_Timing, Enable, TIMER_Gate_Mode_Tr, TIMER_DIV);//TIME0定时器模式0、定时器关
	TIME_DIV_NUM = TIMER_DIV;				//  10ms时钟频率3,6864M,1s=1/3.6863M=307200个机器周期,即10MS执行3072个机器周期,故定时器初值为2^13-3072=5120
	GPIO_Init(P20F,INPUT);						//检测电源电压
	INT0_Init(P20_INDEX, INT_Trigger_Rising, 0);  //设置为最高中断优先级
	
//	LED_init(LEN_IRCH, COM_H, SEG_L, LDRV_7, 2);
  LED_init(LEN_XOSCL, COM_H, SEG_L, LDRV_7, 0);
	
	RTC_init(RTCE_On, MSE_On, HSE_On);
	RTC_SetRTMSS(0);
	RTC_SetAlaramTime(5, 0, 0);  //闹钟早上5点
	RTC_AlarmCfg(HCE_On, MCE_On, SCE_On);
	INT8EN = 1;  //允许RTC中断;
	
	EA = 1;  //开总中断
	
	TS_init();
//#if SUPPORT_KEY
//	TSKey_init();
//#endif

  Sys_Parameter_Init();    //	sys_state.Power = SYS_POWER_IDLE;

  Uart0_Send_nData("MCUSTAR", 7);
	
  while(1)
  {
    WDT_FeedDog();
		
//		TS_Action();
//#if SUPPORT_KEY
//		TS_GetKey();
//#endif
		
		Power_Det_State = Power_Det();
		if(Power_Det_State != 0xff)
		{
      if(Power_Det_State == 0)
			{//关机
				LED_Display_Off();
	      Power_Det_State = 0xff;
	      BT_Power_Ctrl(BT_POWER_OFF);
				Enter_Sleep_Conf();
				sys_state.Power = SYS_POWER_DOWN;
				sys_state.Mode = SYS_IDLE;
				sys_state.Alarm = ALARM_IDLE;

      }
			else if(Power_Det_State == 1)
			{//开机
       
				BT_Power_Ctrl(BT_POWER_ON);
				Quit_Sleep_Conf();
        sys_state.Power = SYS_POWER_NORMAL;
				sys_state.Mode = SYS_CLOCK;                 //一开始SYS_STATE.MODE=SYS_CLOCK
      }
    }
		
		if(sys_state.Power == SYS_POWER_NORMAL)
		{
//      BT_Power_Ctrl(BT_POWER_OFF);
      TS_Action();
			
			Key_Scan();	

      BTCmd_Resend();
		  BT_CmdProcess();
		
		  FM_Init();
		  FM_Auto_Scan();
      FM_CH_Change();	
      Set_Alarm();	
      Alarm_LED_Ctrl();
			LED_CIRCLE_SHOW();
			RGB_GREEN_DIS(1);
//			PWM3_init();
    }
		else if(sys_state.Power == SYS_POWER_DOWN)
		{
      if(POWER_PIN_DATA == 1)
			{
        
      }
    }
		else if(sys_state.Power == SYS_POWER_SLEEP)
		{
			if(POWER_PIN_DATA == 0)
			{
				LED_Display_Off();
//	      Power_Det_State = 0xff;
	      BT_Power_Ctrl(BT_POWER_OFF);
        Enter_Sleep_Conf();
				sys_state.Power = SYS_POWER_DOWN;
				sys_state.Mode = SYS_IDLE;
				sys_state.Alarm = ALARM_IDLE;
//				while(POWER_PIN_DATA == 0)
//				{
//          WDT_FeedDog();
//        }
//				Quit_Sleep_Conf();
      }
			else
			{
//        if(KeysFlagSN & 0x0001)
//			  {
//          BT_Power_Ctrl(BT_POWER_ON);
//				  Quit_Sleep_Conf();
//          sys_state.Power = SYS_POWER_NORMAL;
//        }
			}
    }
		
  }

	
}

#endif
