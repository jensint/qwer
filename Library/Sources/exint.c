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
  //ȷ���ж���������Ϊ��������
  IT0CON = int_index;      // ѡ��P20��Ϊ�ж�0���ţ�16ΪP20��Ӧ���������
	EX0 = 1;							//INT0�ж�0ʹ��
	IE0 = 1;							//�ⲿ�ж�0ʹ��
	IT0 = trigger;				//�����жϷ�ʽ
	PX0 = priority;				//�����ж����ȼ�
}






#endif
