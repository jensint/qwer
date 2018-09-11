#ifndef _IRC_C_
#define _IRC_C_

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
#include "../../Library/includes/sample.h"
#include "../../Library/includes/exint.h"
#include "irc.h"
#include "key.h"
#include <intrins.h>
/*********************************************************************************************************************/
unsigned char IR_Code[4];			   	//ң����ֵ
bit IRC_SyncFlag;					   			//ͬ���źű�־�� Ϊ1��ʾ�ѽ��յ�ͬ���ź�
bit IR_RxEndFlag;
unsigned char IRC_BreakTimer;
unsigned char IRC_LongKeyTimer;
bit IRC_MonitorFlag;
bit IRC_LongStartFlag;
unsigned int IRC_Msg;
unsigned char IR_BitCount;			  //ң����ֵλָʾ
#if (IR_MODE == SAMPLE_MOD)
unsigned char OverFlowCount;		  //�������������
#elif (IR_MODE == INT_MODE)
unsigned char IRC_JudgeValidPulseTimer;
unsigned int LastTimVal;
#endif
#if (IR_MODE == SAMPLE_MOD)
void INT9_ISR (void) interrupt 14 	
{
	unsigned int PulseWidth;
	if(SMSTA & SMEF)
	{
		SMSTA |= SMEF;
	
		if(OverFlowCount == 0)
		{
			PulseWidth = (SMDATH&0x7F)*256 + SMDATL;	 //��ȡ�������
			if(!IRC_SyncFlag)			
			{
				if((PulseWidth > (SYN_WIDTH1-WIDTH*6)) && (PulseWidth < (SYN_WIDTH1+WIDTH*6))) //���δͬ���� �жϵ�ǰ�����Ƿ�ͬ���ź�
				{
					IRC_SyncFlag = 1;
					IR_BitCount = 0;
				}
			}
			else
			{
				if((PulseWidth > (DAT_1_WIDTH-WIDTH*2)) && (PulseWidth < (DAT_1_WIDTH+WIDTH*2)))	  //�жϵ�ǰ�����Ƿ�λ1
				{
					IR_Code[IR_BitCount/8] |= (1<<(7-(IR_BitCount%8)));	
					IR_BitCount++;	
				}
				else if((PulseWidth > (DAT_0_WIDTH-WIDTH)) && (PulseWidth < (DAT_0_WIDTH+WIDTH)))	  //�жϵ�ǰ�����Ƿ�λ0
				{
					IR_Code[IR_BitCount/8] &= ~(1<<(7-(IR_BitCount%8)));	
					IR_BitCount++;	
				}
				else
				{
					IRC_SyncFlag = 0;		  //�������λ1��0�Ŀ��ȣ� ��ȴ�������һ��ͬ���ź�
				}
				if((IR_BitCount == 32) && IRC_SyncFlag)	   //32λң����������
				{
					IRC_SyncFlag = 0;
					IR_RxEndFlag = 1;

				}
			}
		}
		OverFlowCount=0;				 //��λ���������
		IRC_BreakTimer = 13;
	}
	if(SMSTA & SMOF)
	{
		SMSTA |= SMOF;
		if(OverFlowCount < 0xFF) 
		{
			OverFlowCount++;			//��������ۼ�
		}
	}	
}

void Sample_init(void)
{										     	 	 
	GPIO_Init(P70F,P70_SAMPLE_SETTING);			  		//����P70Ϊ�����ܽ�

	SMCON = SMEN_SYS_CLK | SMOE(1) | SMMD(1);	 		//��ʼ��SAMPLE����
	SMDIV = 6;								 	 					//����SAMPLEʱ�ӷ�Ƶ
	SMVTHL = (SYN_WIDTH1*2)%256;			 	 	//�������ʱ�ӿ���
	SMVTHH = (SYN_WIDTH1*2)/256;
	SMCON |= SMIE(1);	
	INT9EN = 1; 				 				//ʹ��SAMPLE�ж�		 
}
#elif (IR_MODE == INT_MODE)
void ExInt2_init(void)
{
	INDEX = 0;										
	EPCON = EPPL(1) | P00_INDEX;	//����P00ΪINT2���ж����ţ��½����ж�
	GPIO_Init(P00F,INPUT);				//����P00Ϊ��������
	INT2EN = 1; 									//INT2�ж�ʹ��
	EPIE |= BIT0;	
	EXIP |= BIT0;									//����INT2Ϊ�����ȼ��ж�
}
void Timer1_init(void)
{
	TMOD=(TMOD&0xCF)|0x10;				//��ʼ��TIMER1�� ʹ��TIMER1������ң������
	TR1 = 1;
}
void INT2_ISR (void) interrupt 7
{
	unsigned int CrtTimVal,PulseWidth;	
	if(EPIF & BIT0)
	{
		EPIF = BIT0;
		TR1=0;
		CrtTimVal=((unsigned int)TH1)<<8|(unsigned int)TL1;			//��ȡ��ǰTIMER1����ֵ
		TR1 = 1;		

		if(IRC_JudgeValidPulseTimer)
		{
			PulseWidth = CrtTimVal - LastTimVal;			//��ȥ�ϴμ���ֵ�� �õ�����
			
			if(!IRC_SyncFlag)
			{
				if((PulseWidth > (SYN_WIDTH1-WIDTH*6)) && (PulseWidth < (SYN_WIDTH1+WIDTH*6))) //���δͬ���� �жϵ�ǰ�����Ƿ�ͬ���ź�
				{
					IRC_SyncFlag = 1;
					IR_BitCount = 0;
				}
			}
			else
			{
				if((PulseWidth > (DAT_1_WIDTH-WIDTH*2)) && (PulseWidth < (DAT_1_WIDTH+WIDTH*2)))	  //�жϵ�ǰ�����Ƿ�λ1
				{
					IR_Code[IR_BitCount/8] |= (1<<(7-(IR_BitCount%8)));	
					IR_BitCount++;	
				}
				else if((PulseWidth > (DAT_0_WIDTH-WIDTH)) && (PulseWidth < (DAT_0_WIDTH+WIDTH)))	  //�жϵ�ǰ�����Ƿ�λ0
				{
					IR_Code[IR_BitCount/8] &= ~(1<<(7-(IR_BitCount%8)));	
					IR_BitCount++;	
				}
				else
				{
					IRC_SyncFlag = 0;		  //�������λ1��0�Ŀ��ȣ� ��ȴ�������һ��ͬ���ź�
				}
				if((IR_BitCount == 32) && IRC_SyncFlag)	   //32λң����������
				{
					IRC_SyncFlag = 0;
					IR_RxEndFlag = 1;
				}			
			}
		}	
		LastTimVal = CrtTimVal;
		IRC_JudgeValidPulseTimer = 3;
		IRC_BreakTimer = 13;				
	}
}
#endif
void Timer0_init(void)
{
	TMOD=(TMOD&0xFC)|0x01;
	TL0 = 0x00;
	TH0 = 0xF4;
	TR0 = 1;
	ET0 = 1;	
}
void TIMER0_ISR (void) interrupt 1 		 //10ms 
{
	TL0 = 0x00;
	TH0 = 0xF4;

	if(IRC_BreakTimer) IRC_BreakTimer--;
	if(IRC_LongKeyTimer) IRC_LongKeyTimer--;
#if (IR_MODE == INT_MODE)
	if(IRC_JudgeValidPulseTimer) IRC_JudgeValidPulseTimer--;
#endif
}
void IRC_init(void)
{
#if (IR_MODE == SAMPLE_MOD)
	Sample_init();
	
	IR_RxEndFlag = 0;
	OverFlowCount=0;	
#elif (IR_MODE == INT_MODE)
	ExInt2_init();
	Timer1_init();
	IRC_JudgeValidPulseTimer = 0;
#endif
	IRC_SyncFlag = 0;
	IRC_BreakTimer = 0;
	IRC_MonitorFlag = 0;
	IRC_LongStartFlag = 0;
	IRC_Msg = KEY_INVALID;
}
unsigned char IRC_GetKeyCode(unsigned char irc_code)		
{
	unsigned char i;
	code unsigned char IRKEY_TABLE[][2]=
	{
		{K1,162},	
		{K2,98},
		{K3,226},
		{K4,34},
		{K5,2},
		{K6,194},
		{K7,224},
		{K8,168},
		{K9,144},	 
		{K10,152},
	};
	for(i=0;i<sizeof(IRKEY_TABLE)/2;i++)
	{
		if(IRKEY_TABLE[i][1] == irc_code)
		{
			return IRKEY_TABLE[i][0];
		}
	}
	return 0;
}
unsigned int IRC_GetMsg(void)
{
	static unsigned int KeyBak;
	unsigned int Msg = KEY_INVALID;
	if(!IRC_MonitorFlag)
	{
		if(IR_RxEndFlag)
		{
			IR_RxEndFlag = 0;
#ifdef PRINT_EN
			uart_printf("code1 = 0x%x,code2 = 0x%x,code3 = 0x%x,code4 = 0x%x \n",(unsigned int)IR_Code[0],(unsigned int)IR_Code[1],(unsigned int)IR_Code[2],(unsigned int)IR_Code[3]);	
#endif			
			if((IR_Code[0] == USER_CODE1) && (IR_Code[1] == USER_CODE2) && (IR_Code[2] == ~IR_Code[3]))
			{
				Msg = (unsigned int)IRC_GetKeyCode(IR_Code[2]);		//�õ�ң�ذ������µ���Ϣ
				KeyBak = Msg;						
				IRC_MonitorFlag = 1;
				IRC_LongStartFlag = 0;
				IRC_LongKeyTimer = IRC_LONG_START_TIME;			
			}
		}				
	}	
	else
	{
		if(!IRC_BreakTimer)
		{
			if(!IRC_LongStartFlag)
			{
				Msg = KeyBak | KEY_BREAK;		//�õ�ң�ذ����̰��ͷ���Ϣ
			}
			else
			{
				Msg = KeyBak | KEY_LONG_BREAK;	//�õ�ң�ذ��������ͷ���Ϣ
			}
			IRC_MonitorFlag = 0;			
		}
		else
		{
			if(!IRC_LongKeyTimer)
			{
				if(!IRC_LongStartFlag)
				{
					IRC_LongStartFlag = 1;
					Msg = KeyBak | KEY_LONG_START;			//�õ�ң�ذ���������ʼ����Ϣ�� �������º���1������������Ϣ
				}
				else
				{
					Msg = KeyBak | KEY_LONG;		//�õ�������Ϣ�� ������ʼ�󣬴��ÿ300ms����һ�γ�����Ϣ
				}
				IRC_LongKeyTimer = IRC_LONG_TIME;
			}			
		}
	}
	return Msg;
}
void IRC_MainLoop(void)
{
		IRC_init();
		Timer0_init();
		while(1)
		{
			IRC_Msg = IRC_GetMsg();
			if(IRC_Msg != KEY_INVALID)
			{
#ifdef PRINT_EN
				uart_printf("IRC_Msg = 0x%x\n",IRC_Msg);	
#endif		
			}
		}
}
#endif