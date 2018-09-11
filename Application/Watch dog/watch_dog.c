#ifndef _WATCH_DOG_C_
#define _WATCH_DOG_C_

/*********************************************************************************************************************/
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
#include "../../Library/includes/uart.h"
#include "../../Library/includes/delay.h"
#include "../../Library/includes/system_clock.h"
#include "../../Library/includes/wdt.h"
#include "watch_dog.h"
#include <intrins.h>
/*********************************************************************************************************************/
#if (WDT_MODE == WDT_INTERRUPT)
bit WdtIntEvFlag;
void WDT_ISR (void) interrupt 12 
{
	static unsigned char i;
	if(WDFLG & WDIF)
	{
		WDT_FeedDog();
		WdtIntEvFlag = 1;
	}	
}
#endif												
void WatchDog_MainLoop(void)
{
#if (WDT_MODE == WDT_RESET)	
#ifdef PRINT_EN	
	if(WDFLG & WDRF)
	{
		uart_printf("Watch dog reset event happen!\n");	
	}
#endif	
#endif	

#if (WDT_MODE == WDT_INTERRUPT)	
	#if (WDT_CLK_SOURCE == IRCH)
		WDT_init(WDTS_IRCH,WDRE_int,0x708);
	#elif (WDT_CLK_SOURCE == XOSCL)
		CKCON |= XLCKE;
		while(!(CKCON & XLSTA));
		WDT_init(WDTS_XOSCL,WDRE_int,0x0F);
	#elif (WDT_CLK_SOURCE == IRCL)
		CKCON |= ILCKE;
		WDT_init(WDTS_IRCL,WDRE_int,0x0F);
	#endif
	
	INT7EN = 1; 
#elif (WDT_MODE == WDT_RESET)	
	#if (WDT_CLK_SOURCE == IRCH)
		WDT_init(WDTS_IRCH,WDRE_reset,0x708);
	#elif (WDT_CLK_SOURCE == XOSCL)
		CKCON |= XLCKE;
		while(!(CKCON & XLSTA));
		WDT_init(WDTS_XOSCL,WDRE_reset,0x0F);
	#elif (WDT_CLK_SOURCE == IRCL)
		CKCON |= ILCKE;
		WDT_init(WDTS_IRCL,WDRE_reset,0x0F);
	#endif	
#endif
	WDT_FeedDog();

	while(1)
	{
// 		WDT_FeedDog();
#if (WDT_MODE == WDT_INTERRUPT)	
		if(WdtIntEvFlag)
		{
			WdtIntEvFlag = 0;
			uart_printf("Watch dog interrupt event happen!\n");	
		}
#endif
	}
}
#endif
