
#ifndef __ADC_KEY_H__
#define __ADC_KEY_H__


enum
{
  MSG_NONE = 0x00,
	  
  MSG_PRE,  //上一曲
  MSG_NEXT,  //下一曲
  MSG_VOL_UP,  //音量+
  MSG_VOL_DW,  //音量-
  MSG_PLAY_PAUSE,  //播放/暂停
  MSG_MODE,  //模式切换
  MSG_SET_CLOCK,  //设置闹钟
  MSG_SET_TIME,  //设置时间 
	
	MSG_MODE_SHORT,
	MSG_MODE_LONG,
	MSG_MODE_HOLD,
	MSG_VOL_DOWN_SHORT,
	MSG_VOL_DOWN_LONG,
	MSG_VOL_DOWN_HOLD,
	MSG_VOL_UP_SHORT,
	MSG_VOL_UP_LONG,
	MSG_VOL_UP_HOLD,
	MSG_MFD_SHORT,
	MSG_MFD_LONG,
	MSG_MFD_HOLD,
	
  MSG_ERROR = 0xFF,
};


enum
{
  ADC_KEY_1 = 0x00,	  
  ADC_KEY_2,
  ADC_KEY_3,
  ADC_KEY_4, 
	
  ADC_KEY_ERROR = 0xFF,
};

typedef enum 
{
  ADC_KEY_STATE_IDLE,
  ADC_KEY_STATE_JITTER,
  ADC_KEY_STATE_PRESS_DOWN,
  ADC_KEY_STATE_CP,
	
} ADC_KEY_STATE;


extern uint16_t xdata ADC_KEY_JUDGE_TIME;


uint8_t Adc_Key_Sacn(void);




#endif

