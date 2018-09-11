#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#define ALARM_DISABLE    0xFF
#define DIS_COUNT    3
#define KEEPTIME_COUNT    16
#define KEEPVOLUME_COUNT    3

enum{
	IDEL = 0,
	BT,
	AUX,
	FM,
	TIME,
};

typedef enum {
	Unset = 0,
	Set,
}Set_Enable;	


typedef enum{ 
	Unit_Null = 0,
	Unit_Hour,
  Unit_Minute,
	
}CLOCK_UNIT;

typedef struct _LED_DIS_VALUE
{
	uint8_t TenDigitsHour;
  uint8_t UnitsDigitHour;
	uint8_t TenDigitsMinute;
  uint8_t UnitsDigitMinute;
	
}LED_DIS_VALUE;


typedef struct _CLOCK_NUM
{
	uint8_t Hour;
  uint8_t Minute;
	uint8_t Second;
}CLOCK_NUM;


typedef struct _LED_DIS_STATE
{
	uint8_t PlayPuse;
  uint8_t Mode;
	
}LED_DIS_STATE;


extern CLOCK_NUM xdata RTC_Num;  //实时时间值
extern CLOCK_NUM xdata Alarm1_Num;  //闹钟1时间
extern CLOCK_NUM xdata Alarm2_Num;  //闹钟2时间
extern CLOCK_NUM xdata Settime_Num;
extern uint8_t xdata Settime_Dis_Count;
extern uint8_t xdata Settime_Keeptime_Count;
extern uint8_t xdata SetVolume_Keeptime_Count; 
extern uint8_t xdata count;
extern uint8_t xdata ge;

void Led_Seg_Display(uint8_t index, uint8_t dat);
void Led_Display(uint8_t hour, uint8_t minute, uint8_t mode);
void Alarm_LED_Display(uint8_t alarm_style);
void Settime_LED_Display(uint8_t mode, CLOCK_UNIT unit, uint8_t flag);
void Volume_LED_Display(uint8_t volume);
void FM_Freq_LED_Display(uint16_t freq);
void FM_Channel_LED_Display(uint8_t channel);
void LED_Display_Off();
void LED_COUNT(uint8_t mode);
#endif

