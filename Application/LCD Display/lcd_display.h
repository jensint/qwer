#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H
/**************************************************************************************************************/
#define CHAR_0 		0
#define CHAR_1 		1
#define CHAR_2 		2
#define CHAR_3 		3
#define CHAR_4 		4
#define CHAR_5 		5
#define CHAR_6 		6
#define CHAR_7 		7
#define CHAR_8 		8
#define CHAR_9 		9
#define CHAR_A	     0x0a
#define CHAR_B	     0x0b
#define CHAR_C       0x0c
#define CHAR_D       0x0d
#define CHAR_E	     0x0e
#define CHAR_F	     0x0f
#define CHAR_G	     0x10
#define CHAR_H	     0x11
#define CHAR_I	     0x12
#define CHAR_J	     0x13
#define CHAR_K	     0x14
#define CHAR_L	     0x15
#define CHAR_M	     0x16
#define CHAR_N	     0x17
#define CHAR_O	     0x18
#define CHAR_P	     0x19
#define CHAR_Q	     0x1a
#define CHAR_R	     0x1b
#define CHAR_S	     0x1c
#define CHAR_T       0x1d
#define CHAR_U	     0x1e
#define CHAR_V	     0x1f
#define CHAR_W	     0x20
#define CHAR_X	     0x21
#define CHAR_Y	     0x22
#define CHAR_Z	     0x23
#define CHAR__       0x24
#define CHAR_ 	     0x25

#define LOCATION_MAX		3
#define NUM_MAX 				CHAR_

void LCD_Display_MainLoop(void);
/*********************************************************************************************************************/
#endif										
