#ifndef IRC_H
#define IRC_H
/**************************************************************************************************************/
#define USER_CODE1		0x00
#define USER_CODE2		0xFF


#define SYN_WIDTH1		0x103b		   //同步信号脉冲宽度
#define DAT_1_WIDTH		0x02b1		   //位1的脉冲宽度
#define DAT_0_WIDTH		0x015c		   //位0的脉冲宽度
#define WIDTH			(DAT_0_WIDTH/6)

#define 	IRC_LONG_START_TIME		100		//遥控按键长按开始时间
#define 	IRC_LONG_TIME					30		//长按时间

void IRC_MainLoop(void);
/*********************************************************************************************************************/
#endif										
