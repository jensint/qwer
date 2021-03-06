#ifndef EXINT_H
#define EXINT_H


//EPCON�Ĵ�������
#define EPPL(N)	(N<<7)

typedef enum 
{
	P00_INDEX = 0,
	P01_INDEX = 1,
	P02_INDEX = 2,
	P03_INDEX = 3,
	P04_INDEX = 4,
	P05_INDEX = 5,
	P06_INDEX = 6,
	P07_INDEX = 7,

	P10_INDEX = 8,
	P11_INDEX = 9,
	P12_INDEX = 10,
	P13_INDEX = 11,
	P14_INDEX = 12,
	P15_INDEX = 13,
	P16_INDEX = 14,
	P17_INDEX = 15,

	P20_INDEX = 16,
	P21_INDEX = 17,
	P22_INDEX = 18,
	P23_INDEX = 19,
	P24_INDEX = 20,
	P25_INDEX = 21,
	P26_INDEX = 22,
	P27_INDEX = 23,

	P30_INDEX = 24,
	P31_INDEX = 25,
	P32_INDEX = 26,
	P33_INDEX = 27,
	P34_INDEX = 28,
	P35_INDEX = 29,
	P36_INDEX = 30,
	P37_INDEX = 31,
	
	P40_INDEX = 32,
	P41_INDEX = 33,
	P42_INDEX = 34,
	P43_INDEX = 35,
	P44_INDEX = 36,
	P45_INDEX = 37,
	P46_INDEX = 38,
	P47_INDEX = 39,
	
	P50_INDEX = 40,
	P51_INDEX = 41,
	P52_INDEX = 42,
	P53_INDEX = 43,
	P54_INDEX = 44,
	P55_INDEX = 45,
	P56_INDEX = 46,
	P57_INDEX = 47,
	
	P60_INDEX = 48,
	P61_INDEX = 49,
	P62_INDEX = 50,
	P63_INDEX = 51,
	P64_INDEX = 52,
	P65_INDEX = 53,
	P66_INDEX = 54,
	P67_INDEX = 55,
	
	P70_INDEX = 56,
	P71_INDEX = 57,
	P72_INDEX = 58,
	P73_INDEX = 59,
	P74_INDEX = 60,
	P75_INDEX = 61,
	
}INT_Index_TypeDef;


typedef enum 
{
	INT_Trigger_Rising = 0,
	INT_Trigger_Failling = 1,
}INT_Trigger_TypeDef;


void INT0_Init(INT_Index_TypeDef int_index, INT_Trigger_TypeDef trigger, unsigned char priority);



#endif

