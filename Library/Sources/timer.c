#ifndef _TIMER_C_
#define _TIMER_C_

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
#include "../../Library/includes/timer.h"
#include <intrins.h>


void Timer0_Init(TIMER_Mode_TypeDef timer_mode,
                 TIMER_Fun_TypeDef timer_func,
								 unsigned char interrupt_sw,
								 TIMER_Gate_Mode_TypeDef timer_gate_mode,
								 unsigned int div)
{
  TMOD &= 0xf0;
	
	TMOD |= (timer_gate_mode<<3 | timer_func<<2 | timer_mode);
	
  switch(timer_mode)
	{
    case 0:
			TH0 = (unsigned char)((div>>5)&0x00ff);
		  TL0 = (unsigned char)div&0x1f;
			break;
		
		case 1:
			TH0 = (unsigned char)(div>>8)&0xff;
		  TL0 = (unsigned char)div&0xff;
			break;
		
		case 2:
			TH0 = (unsigned char)(div>>8);
		  TL0 = (unsigned char)div;
			break;
		
		case 3:
			break;
		
		default:
			break;
  }
	
	IE &= 0xfd;
	IE |= (interrupt_sw<<1);   //外部中断使能位
	
	if(timer_gate_mode == TIMER_Gate_Mode_Tr)
	{
    TR0 = 1;  //开始计时   TR0：运行控制位
  }	
	
}

#if 0
//void TIMER0_ISR (void) interrupt 1
//{
//  
//}
#endif



#endif
