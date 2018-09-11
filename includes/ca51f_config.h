#ifndef CA51F_CONFIG_H
#define CA51F_CONFIG_H
/**************************************************************************************************************/

#include "common.h"

#define IC_TYPE_CA51F2X 0
#define IC_TYPE_CA51F3X 1

#define IRCH		0
#define IRCL		1
#define PLL			2
#define XOSCH		3
#define XOSCL		4

/**************************************************************************************************************/
#define IC_TYPE			IC_TYPE_CA51F3X     //芯片系列选择
#define SYSCLK_SRC	IRCH								//芯片系统时钟选择
/**************************************************************************************************************/

/************系统时钟频率定义，主要用于配置UART波特率**********************************************************/
#if (SYSCLK_SRC == IRCH)
	#define FOSC		(3686400)
#elif (SYSCLK_SRC == PLL)
	#define PLL_Multiple			7			//PLL倍频倍数
	#define FOSC		(3686400*PLL_Multiple)
#else
	#define FOSC		(3686400)
#endif
/***************************************************************************************************************/

#define RTC_CLK_SELECT		XOSCL			//定义RTC时钟源， 在外接32768晶振时定义为XOSCL，否则定义为IRCL
/***************************************************************************************************************/

//#define LVD_RST_ENABLE		//芯片掉电到1.2V左右再重上电有可以导致死机，打开LVD复位功能可解决此问题


/*************************UART功能开关宏定义********************************************************************/
//#define UART0_EN			//如果使用UART0，打开此宏定义
//#define UART1_EN			//如果使用UART1，打开此宏定义
#if (IC_TYPE == IC_TYPE_CA51F2X)
//	#define UART2_EN 		//只有F2系列才有UART2，如果使用UART2，打开此宏定义
#endif

#define PRINT_EN				//使用uart_printf函数打印使能

#ifdef PRINT_EN
	#define UART0_PRINT		//如果使用UART0打印，打开此宏定义
//	#define UART1_PRINT		//如果使用UART1打印，打开此宏定义
	#if (IC_TYPE == IC_TYPE_CA51F2X)
//		#define UART2_PRINT		//如果使用UART2打印，打开此宏定义
	#endif
			
	#ifdef UART0_PRINT
		#define UART0_EN
	#elif defined  UART1_PRINT
		#define UART1_EN
	#elif defined  UART2_PRINT
		#define UART2_EN
	#endif
#endif
#ifdef UART0_EN
	#define UART0_BAUTRATE		115200
#endif
#ifdef UART1_EN
	#define UART1_BAUTRATE		115200
#endif
#ifdef UART2_EN
	#define UART2_BAUTRATE		115200
#endif
/*********************************************************************************************************************/


/***************************红外遥控功能定义**************************************************************************/
#define SAMPLE_MOD			0			//用sample功能实现遥控接收，仅F2系列部分型号支持
#define INT_MODE				1			//用外部中断实现遥控接收

#define IR_MODE					INT_MODE


/*********************************************************************************************************************/
#endif										
