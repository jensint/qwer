
#ifndef __ADC_KEY_H__
#define __ADC_KEY_H__


enum
{
  MSG_NONE = 0x00,
	  
  MSG_PRE,  //��һ��
  MSG_NEXT,  //��һ��
  MSG_VOL_UP,  //����+
  MSG_VOL_DW,  //����-
  MSG_PLAY_PAUSE,  //����/��ͣ
  MSG_MODE,  //ģʽ�л�
  MSG_SET_CLOCK,  //��������
  MSG_SET_TIME,  //����ʱ�� 
	
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

