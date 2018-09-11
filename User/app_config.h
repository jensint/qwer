
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__


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


#include "delay.h"
#include "system_clock.h"
#include "ts_configuration.h"
#include "ts_def.h"
#include "ts_api.h"
#include "ts_service.h"
#include "key.h"
#include "wdt.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "exint.h"
#include "stop_idle.h"
#include "timer.h"
#include "rtc.h"
#include "lcd_led.h"
#include "adc_key.h"
#include "led_control.h"
#include "app_task.h"
#include "bluetooth_control.h" 
#include "pll_i2c.h"
#include "RDA5807.h"


#endif

