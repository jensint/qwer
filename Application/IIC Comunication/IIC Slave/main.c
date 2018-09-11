#ifndef _MAIN_C_
#define _MAIN_C_

/*********************************************************************************************************************/
#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "../../../includes/ca51f2sfr.h"
#include "../../../includes/ca51f2xsfr.h"
#include "../../../includes/gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "../../../includes/ca51f3sfr.h"
#include "../../../includes/ca51f3xsfr.h"
#include "../../../includes/gpiodef_f3.h"
#endif
#include "../../../includes/system.h"
#include "../../../Library/includes/uart.h"
#include "../../../Library/includes/system_clock.h"
#include "iic_slave.h"
#include <intrins.h>

void main(void)
{
#ifdef UART0_EN
	Uart0_Initial(UART0_BAUTRATE);
#endif
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);
#endif
#ifdef UART2_EN
	Uart2_Initial(UART2_BAUTRATE);
#endif

	EA = 1;

#ifdef PRINT_EN
 	uart_printf("i2c slave example!\n");
#endif
	
	IIC_Slave_MainLoop();
	while(1);
}
#endif
