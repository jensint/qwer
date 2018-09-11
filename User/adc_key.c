#ifndef _ADC_KEY_C_
#define _ADC_KEY_C_


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
#include "../../Library/includes/delay.h"
#include "../../Library/includes/adc.h"
#include <intrins.h>

#include "app_config.h"
#include "adc_key.h"


#define 	 ADC_KEY_JITTER_TIME    5    //3*10ms
#define 	 ADC_KEY_CP_TIME    150    //100*10ms
#define 	 ADC_KEY_CPH_TIME    30


#define    ADC_VALUE_K1    0x10
#define    ADC_VALUE_K2    0x21
#define    ADC_VALUE_K3    0x61
#define    ADC_VALUE_K4    0x87

#define    DEBOUNCE    7 

uint16_t xdata ADC_KEY_JUDGE_TIME;

static const uint8_t AdcKeyEvent[][5] =
{//PDS(按键开始)  SPR(短按松开)  CPS(长按开始)  CPH(长按保持)  CPR(长按松开)
//  {MSG_NONE,    MSG_MODE,    MSG_SET_TIME,    MSG_NONE,    MSG_NONE},  //K1 -- MODE
//  {MSG_NONE,    MSG_PRE,     MSG_VOL_DW,    MSG_VOL_DW,    MSG_NONE},  //K2 -- V-
//  {MSG_NONE,    MSG_NEXT,    MSG_VOL_UP,    MSG_VOL_UP,    MSG_NONE},  //K3 -- V+
//  {MSG_NONE,    MSG_PLAY_PAUSE,    MSG_SET_CLOCK,    MSG_NONE,    MSG_NONE},  //K4 -- MFD
	{MSG_NONE,  MSG_MODE_SHORT,  MSG_MODE_LONG,  MSG_MODE_HOLD,  MSG_NONE},  //K1 -- MODE
  {MSG_NONE,  MSG_VOL_DOWN_SHORT,  MSG_VOL_DOWN_LONG,  MSG_VOL_DOWN_HOLD,  MSG_NONE},  //K2 -- V-
  {MSG_NONE,  MSG_VOL_UP_SHORT,  MSG_VOL_UP_LONG,  MSG_VOL_UP_HOLD,  MSG_NONE},  //K3 -- V+
  {MSG_NONE,  MSG_MFD_SHORT,  MSG_MFD_LONG,  MSG_MFD_HOLD,  MSG_NONE},  //K4 -- MFD
};


static uint8_t Get_Adc_Value(void)
{
	uint16_t xdata adc_value = 0;
	uint8_t xdata n;
	uint8_t xdata max_value =0;
	uint8_t xdata min_value =255;
	
	for(n = 0; n < 18; n++)
	{
	  ADC_StartConversion();
		while(!(ADCON & ADIF));   //等待转换完成
	  ADCON |= ADIF;						//清除中断标志位
		
		if(max_value < ADCDH)
		{
      max_value = ADCDH;
    }
		
		if(min_value > ADCDH)
		{
      min_value = ADCDH;
    }
		adc_value += ADCDH;
	}
	
	adc_value = adc_value - (max_value + min_value);
	adc_value >>= 4; 
	return (uint8_t)adc_value;
}


static uint8_t Get_Adc_Key_Index(void)
{
  uint8_t	xdata value;
//	uint8_t	KeyIndex = 0xff;
	
	value = Get_Adc_Value();
//	Uart0_PutChar(value);
	
	if((ADC_VALUE_K1 + DEBOUNCE) >= value)
  {
    return ADC_KEY_1;  //按键1
  }
  else if((value >= (ADC_VALUE_K2 - DEBOUNCE)) && ((ADC_VALUE_K2 + DEBOUNCE) >= value))	
  {
    return ADC_KEY_2;  //按键2
  }
  else if((value >= (ADC_VALUE_K3 - DEBOUNCE)) && ((ADC_VALUE_K3 + DEBOUNCE) >= value))	
  {
    return ADC_KEY_3;  //按键3
  }
  else if((value >= (ADC_VALUE_K4 - DEBOUNCE)) && ((ADC_VALUE_K4 + DEBOUNCE) >= value))	
  {
    return ADC_KEY_4;  //按键4
  }
  else
  {
    return ADC_KEY_ERROR;  //没按键
  }	
}


uint8_t Adc_Key_Sacn(void)
{
  static uint8_t xdata PreKeyIndex = ADC_KEY_ERROR;
	static uint8_t xdata AdcKeyState = ADC_KEY_STATE_IDLE;
	uint8_t	xdata KeyIndex;
	
  ADC_ConfigChannel(ADC_CH3);
	Delay_50us(10);

	KeyIndex = Get_Adc_Key_Index();  	
//	Uart0_PutChar(KeyIndex);
	
	switch(AdcKeyState)
  {
    case ADC_KEY_STATE_IDLE:
			  PreKeyIndex = KeyIndex;
				if(KeyIndex != ADC_KEY_ERROR)
				{
				  ADC_KEY_JUDGE_TIME = ADC_KEY_JITTER_TIME;
          AdcKeyState = ADC_KEY_STATE_JITTER;
        }
			break;
				
		case ADC_KEY_STATE_JITTER:
			if(PreKeyIndex != KeyIndex)
			{//抖动，按键无效
				AdcKeyState = ADC_KEY_STATE_IDLE;
      }
			else
			{//按键有效
			  if(ADC_KEY_JUDGE_TIME > 0)
			  {
          return MSG_NONE;
        }
				ADC_KEY_JUDGE_TIME = ADC_KEY_CP_TIME;
        AdcKeyState = ADC_KEY_STATE_PRESS_DOWN;
				return AdcKeyEvent[PreKeyIndex][0];
      }					
			break;
				
		case ADC_KEY_STATE_PRESS_DOWN:
			if(PreKeyIndex != KeyIndex)
			{//短按松开
				AdcKeyState = ADC_KEY_STATE_IDLE;
				return AdcKeyEvent[PreKeyIndex][1];
      }
			else
			{//长按开始
				if(ADC_KEY_JUDGE_TIME > 0)
				{
          return MSG_NONE;
        }
				ADC_KEY_JUDGE_TIME = ADC_KEY_CPH_TIME;
				AdcKeyState = ADC_KEY_STATE_CP;
        return AdcKeyEvent[PreKeyIndex][2];   
      }
			break;
				
		case ADC_KEY_STATE_CP:      //长按保持
			if(PreKeyIndex != KeyIndex)
			{//长按松开
				AdcKeyState = ADC_KEY_STATE_IDLE;
        return AdcKeyEvent[PreKeyIndex][4];
      }
			else
			{//长按保持
				if(ADC_KEY_JUDGE_TIME > 0)
				{
          return MSG_NONE;
        }
				ADC_KEY_JUDGE_TIME = ADC_KEY_CPH_TIME;
        return AdcKeyEvent[PreKeyIndex][3];
      }
			break;
				
		default:
			AdcKeyState = ADC_KEY_STATE_IDLE;
			break;		

  }
	
	return MSG_NONE;
}







#endif


