#ifndef _EXINT_C_
#define _EXINT_C_

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
#include "../../Library/includes/exint.h"
#include <intrins.h>


void INT0_Init(INT_Index_TypeDef int_index, INT_Trigger_TypeDef trigger, unsigned char priority)
{
  //确定中断引脚设置为输入引脚
  IT0CON = int_index;      // 选择P20作为中断0引脚，16为P20对应的索引编号
	EX0 = 1;							//INT0中断0使能
	IE0 = 1;							//外部中断0使能
	IT0 = trigger;				//设置中断方式
	PX0 = priority;				//设置中断优先级
}






#endif
