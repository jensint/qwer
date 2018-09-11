
#ifndef __APP_TASK_H__
#define __APP_TASK_H__



#include <intrins.h>
#include "app_config.h"

#define POWER_PIN_TCON       P20F
#define POWER_PIN_DATA       P20

#define BT_POWER_PIN_TCON    P11F
#define BT_POWER_PIN_DATA    P11

#define ADC_KEY_PIN_TCON    P13F
#define ADC_KEY_PIN_DATA    P13

//#define LED_GREEN_PIN_TCON    P34F
//#define LED_GREEN_PIN_DATA    P34


#define LED_SEG1    P17F
#define LED_SEG2    P37F
#define LED_SEG3    P36F
#define LED_SEG4    P35F
#define LED_SEG5    P07F
#define LED_SEG6    P06F
#define LED_SEG7    P05F

#define LED_COM0    P00F
#define LED_COM1    P01F
#define LED_COM2    P02F
#define LED_COM3    P03F
#define LED_COM4    P04F

#define CL_TIMER_DIV    8190//8164  //10ms
#define TIMER_DIV    5120  //10ms
#define LEDW_PWM_FREQ    18432  //200Hz      //738//5KHz

#define LEDW_BRIGHTNESS_OFF    LEDW_PWM_FREQ//738
#define LEDW_BRIGHTNESS_1      14000//700
#define LEDW_BRIGHTNESS_2      10000
#define LEDW_BRIGHTNESS_3      0
#define MAX_VOLUME    31
#define DEFAULT_VOLUME    15

typedef enum {
	SYS_POWER_IDLE = 0,
	SYS_POWER_NORMAL = 1,
	
	SYS_POWER_SLEEP = 0xFE,
	SYS_POWER_DOWN = 0xFF,
}SYS_Power_TypeDef;

typedef enum {
	BT_POWER_OFF = 0,
	BT_POWER_ON = 1,
}BT_Power_TypeDef;


enum {
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_ALARM = 0x10,
};

enum {
	ALARM_IDLE = 0,
	ALARM_1,
	ALARM_2,
};

typedef enum {
	SET_VOLUME_DOWN,
	SET_VOLUME_UP,
}_SET_VOLUME;

typedef enum {
	SYS_IDLE = 0,
	SYS_BLUETOOTH,
	SYS_AUX,
	SYS_FM,
	SYS_CLOCK,
	SYS_ALARM1,
	SYS_ALARM2,
	
}Mode_Style;


enum
{
  TS_KEY_NONE = 0x00,
	  
	TS_KEY_SHORT,
	TS_KEY_LONG,
	TS_KEY_LONG_HOLD,
  TS_KEY_LONG_RELEASE,
  TS_KEY_ERROR = 0xFF,
};

typedef enum 
{
  TS_KEY_STATE_IDLE,
  TS_KEY_STATE_JITTER,
  TS_KEY_STATE_PRESS_DOWN,
  TS_KEY_STATE_CP,
	
} TS_KEY_STATE;

typedef struct _TASK_TIME
{
	uint8_t PowerDet;
  uint8_t AdcKeyScan;
	uint8_t AudioDet;
	uint8_t FMAutoSearch;
}TASK_TIME;


typedef struct _SYS_STATE
{
	uint8_t Volume;
	uint8_t Power;
  uint8_t Mode;
	uint8_t RecentMode;
	uint8_t SetTime;
	uint8_t SetVolumeFlag;
	uint8_t Alarm;
	uint8_t LEDWBrightness;
	uint16_t FMFreq; 
	uint16_t FMFreqTemp;
	uint8_t FM_CH_Num;
	uint8_t FM_CH;
}SYS_STATE;


extern uint8_t xdata Mode_Init_Flag;
extern uint8_t xdata Alarm_Ring_Flag;
extern uint16_t xdata Alarm_Ring_Time; 
extern uint8_t xdata LEDW_PWM_Time;

extern uint16_t xdata TS_KEY_JUDGE_TIME;

extern uint8_t xdata FM_INIT_COUNNT;	
extern uint8_t xdata FM_INIT_STATE;

extern uint8_t xdata FM_CHANGE_FlAG;
extern uint8_t xdata FM_CHANGE_COUNT;	

extern uint8_t xdata FM_AUTO_SEARCH_FLAG;
extern uint16_t xdata FM_AUTO_FREQ;	
extern uint16_t xdata FM_FREQ_AREA[32];
extern uint8_t xdata FM_AREA_NUM;

extern TASK_TIME xdata app_task_time;
extern SYS_STATE xdata sys_state;

extern uint16_t xdata TIME_DIV_NUM;
extern uint16_t xdata MSG_MODE_SHORT_count;

void Gpio_Conf(void);
void Sys_Parameter_Init(void);
void Set_RTM_Time(uint8_t hour, uint8_t minute, uint8_t second);
void BT_Power_Ctrl(BT_Power_TypeDef bt_power_sw);
uint8_t Power_Det(void);
void LEDW_Brightness_Ctrl(uint8_t level);
void LEDW_Brightness_Ctrl_circle(uint8_t value);
uint8_t TSkey_Scan(void);
void Key_Scan(void);
void Uart0_Send_nData(uint8_t *databuf, uint8_t data_length);
void Set_Time_Up(CLOCK_UNIT mode);
void Set_Time_Down(CLOCK_UNIT mode);
uint8_t Set_Volume(_SET_VOLUME set_mode);
void Set_Alarm(void);
void Alarm_LED_Ctrl(void);
void FM_Init(void);
void FM_Auto_Scan(void);
void FM_Channel_Up(void);
void FM_CH_Change(void);
void Enter_Sleep_Conf(void);
void Quit_Sleep_Conf(void);
void LED_CIRCLE_SHOW(void);
void RGB_GREEN_DIS(uint8_t flag);
void RGB_RED_DIS_OFF(void);

#endif

