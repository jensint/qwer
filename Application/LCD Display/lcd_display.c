#ifndef _LCD_DISPLAY_C_
#define _LCD_DISPLAY_C_

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
#include "../../Library/includes/lcd_led.h"
#include "lcd_display.h"
#include <intrins.h>
/*********************************************************************************************************************/
code unsigned char NUM_TAB[]=
{
	0x5F,		// 0
	0x06,		// 1
	0x3D,		// 2
	0x2F,		// 3
	0x66,		// 4
	0x6B,		// 5
	0x7B,		// 6
	0x0E,		// 7
	0x7F,		// 8
	0x6F,		// 9
	0x7E,		// A
	0x73,		// B
	0x59,		// C
	0x37,		// D
	0x79,		// E
	0x78		// F
};
void LCD_ShowChar(unsigned char laddr,unsigned char lnum)
{
	unsigned char temp1,temp2,temp3;
 	if((laddr > LOCATION_MAX)||(lnum > NUM_MAX)) return;
	if(laddr == 0)
	{
		temp3 = LCD_LED_ReadLram(5);
		temp3 =	temp3&0x01;

		if(lnum > 2) lnum = 2;

		if(lnum == 1) 
		{
			temp3 |= 0x06;		
		}
		else if(lnum == 2)
		{
			temp3 |= 0x0c;		
		}  
		LCD_LED_WriteLram(5,temp3);	
		return;
	}
	else if(laddr == 1)
	{
		temp1 = 6;
	}
	else if(laddr == 2)
	{
		temp1 = 8;
	}
	else if(laddr == 3)
	{
		temp1 = 2;
	}
	temp2 = NUM_TAB[lnum];

	temp3 = LCD_LED_ReadLram(temp1);
	temp3 = (temp3&0x08)|(temp2>>4);
	LCD_LED_WriteLram(temp1,temp3);	
	if(laddr == 1)
	{
		temp1++;
	}
	else if(laddr == 2)
	{
		temp1 = 3;
	}
	else if(laddr == 3)
	{
		temp1 = 1;
	}
	LCD_LED_WriteLram(temp1,temp2&0x0F);	
}
void LCD_RamClear(void)
{
	unsigned char i;
	for(i = 0; i < LCD_RAM_SIZE; i++)
	{
		LCD_LED_WriteLram(i,0x00);
	}
}
void LCD_Display_MainLoop(void)
{
#if (IC_TYPE == IC_TYPE_CA51F3X)
	GPIO_Init(P00F,P00_COM0_SETTING);
	GPIO_Init(P01F,P01_COM1_SETTING);
	GPIO_Init(P02F,P02_COM2_SETTING);
	GPIO_Init(P03F,P03_COM3_SETTING);	 
// 	GPIO_Init(P04F,P04_COM4_SETTING);

	GPIO_Init(P17F,P17_SEG0_SETTING);
	GPIO_Init(P37F,P37_SEG1_SETTING); 
	GPIO_Init(P36F,P36_SEG2_SETTING);  
	GPIO_Init(P35F,P35_SEG3_SETTING); 
	GPIO_Init(P34F,P34_SEG4_SETTING);
	GPIO_Init(P07F,P07_SEG5_SETTING);
	GPIO_Init(P06F,P06_SEG6_SETTING); 
	GPIO_Init(P05F,P05_SEG7_SETTING);
	GPIO_Init(P04F,P04_SEG8_SETTING);
//	GPIO_Init(P03F,P03_SEG9_SETTING);
#elif  (IC_TYPE == IC_TYPE_CA51F2X)
	GPIO_Init(P00F,P00_COM0_SETTING);
	GPIO_Init(P01F,P01_COM1_SETTING);
	GPIO_Init(P02F,P02_COM2_SETTING);
	GPIO_Init(P03F,P03_COM3_SETTING);
// 	GPIO_Init(P04F,P04_COM4_SETTING);
// 	GPIO_Init(P05F,P05_COM5_SETTING);
// 	GPIO_Init(P06F,P06_COM6_SETTING);
// 	GPIO_Init(P07F,P07_COM7_SETTING);

	GPIO_Init(P57F,P57_SEG0_SETTING);	  
	GPIO_Init(P34F,P34_SEG1_SETTING);	
	GPIO_Init(P35F,P35_SEG2_SETTING);	 
	GPIO_Init(P56F,P56_SEG3_SETTING);
	GPIO_Init(P50F,P50_SEG4_SETTING);	
	GPIO_Init(P51F,P51_SEG5_SETTING);
	GPIO_Init(P52F,P52_SEG6_SETTING);
	GPIO_Init(P53F,P53_SEG7_SETTING);
	GPIO_Init(P54F,P54_SEG8_SETTING);  
// 	GPIO_Init(P55F,P55_SEG9_SETTING);

// 	GPIO_Init(P60F,P60_SEG10_SETTING);
// 	GPIO_Init(P61F,P61_SEG11_SETTING);
// 	GPIO_Init(P62F,P62_SEG12_SETTING);
// 	GPIO_Init(P63F,P63_SEG13_SETTING);
// 	GPIO_Init(P64F,P64_SEG14_SETTING);		
// 	GPIO_Init(P65F,P65_SEG15_SETTING);	
// 	GPIO_Init(P10F,P10_SEG16_SETTING);
// 	GPIO_Init(P11F,P11_SEG17_SETTING);
// 	GPIO_Init(P12F,P12_SEG18_SETTING);
// 	GPIO_Init(P13F,P13_SEG19_SETTING);

// 	GPIO_Init(P14F,P14_SEG20_SETTING);
// 	GPIO_Init(P15F,P15_SEG21_SETTING);
// 	GPIO_Init(P16F,P16_SEG22_SETTING);
// 	GPIO_Init(P17F,P17_SEG23_SETTING);
// 	GPIO_Init(P27F,P27_SEG24_SETTING);
// 	GPIO_Init(P26F,P26_SEG25_SETTING);
// 	GPIO_Init(P25F,P25_SEG26_SETTING);
// 	GPIO_Init(P24F,P24_SEG27_SETTING);
// 	GPIO_Init(P23F,P23_SEG28_SETTING);
// 	GPIO_Init(P22F,P22_SEG29_SETTING);

// 	GPIO_Init(P21F,P21_SEG30_SETTING);
// 	GPIO_Init(P20F,P20_SEG31_SETTING);
//	GPIO_Init(P07F,P07_LCD_SEG32_SETTING);
//	GPIO_Init(P06F,P06_LCD_SEG33_SETTING);
//	GPIO_Init(P05F,P05_LCD_SEG34_SETTING);
//	GPIO_Init(P04F,P04_LCD_SEG35_SETTING); 
#endif

	LCD_RamClear();

#if (LCD_CLK_SOURCE == IRCH)
	LCD_init(LEN_IRCH,DMOD_5ua,BIAS_1_3,LDRV_7,112);
#elif (LCD_CLK_SOURCE == XOSCL)
	CKCON |= XLCKE;
	while(!(CKCON & XLSTA));
	LCD_init(LEN_XOSCL,DMOD_5ua,BIAS_1_3,LDRV_7,0);
#elif (LCD_CLK_SOURCE == IRCL)
	CKCON |= ILCKE;
	LCD_init(LEN_IRCL,DMOD_5ua,BIAS_1_3,LDRV_7,0);
#endif
	LCD_ShowChar(0,1);
	LCD_ShowChar(1,2);
	LCD_ShowChar(2,3);
	LCD_ShowChar(3,4);
	while(1)
	{
	}
}
#endif
