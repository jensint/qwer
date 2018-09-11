#ifndef IRC_H
#define IRC_H
/**************************************************************************************************************/
#define USER_CODE1		0x00
#define USER_CODE2		0xFF


#define SYN_WIDTH1		0x103b		   //ͬ���ź�������
#define DAT_1_WIDTH		0x02b1		   //λ1��������
#define DAT_0_WIDTH		0x015c		   //λ0��������
#define WIDTH			(DAT_0_WIDTH/6)

#define 	IRC_LONG_START_TIME		100		//ң�ذ���������ʼʱ��
#define 	IRC_LONG_TIME					30		//����ʱ��

void IRC_MainLoop(void);
/*********************************************************************************************************************/
#endif										
