#ifndef _BLUETOOTH_CONTROL_C_
#define _BLUETOOTH_CONTROL_C_

#include <string.h>
#include "bluetooth_control.h"

#define AT_CMD_SIZE    7


BT_UART_STATE xdata BT_Uart;
uint8_t BT_Operating_State = BT_IDEL;
uint8_t BT_Inti_Flag = 0;

void Mcu_SendCmdToBT(BT_CMD Cmd)
{
	uint8_t xdata btcmdbuf[8] = {0};
  uint8_t xdata btcmdlen = 0;

	switch(Cmd)
	{
//    case 0:
//			memcpy((void*)&btcmdbuf[0], (void*)"at+moda", 7);
//			btcmdlen = 7;
//		break;
//			
//		case 1:
//			memcpy((void*)&btcmdbuf[0], (void*)"at+modb", 7);
//			btcmdlen = 7;
//			break;
			
		case CMD_BT:
			memcpy((void*)&btcmdbuf[0], (void*)"at+modc", 7);
			btcmdlen = 7;
			break;
			
		case CMD_AUX:
			memcpy((void*)&btcmdbuf[0], (void*)"at+modd", 7);
			btcmdlen = 7;
			break;
		
		case CMD_FM:
			memcpy((void*)&btcmdbuf[0], (void*)"at+mode", 7);
			btcmdlen = 7;
			break;
		
		case CMD_CLOCK:
			memcpy((void*)&btcmdbuf[0], (void*)"at+modf", 7);
			btcmdlen = 7;
			break;
			
		case CMD_PREV:
			memcpy((void*)&btcmdbuf[0], (void*)"at+prev", 7);
			btcmdlen = 7;
			break;
		
		case CMD_VOLUME:
			memcpy((void*)&btcmdbuf[0], (void*)"at+vol", 6);
		  btcmdbuf[6] = sys_state.Volume;
		  btcmdlen = 7;
			break;
			
		case CMD_NEXT:
			memcpy((void*)&btcmdbuf[0], (void*)"at+next", 7);
			btcmdlen = 7;
			break;
			
	  case CMD_PLAYPUSE:
			memcpy((void*)&btcmdbuf[0], (void*)"at+play", 7);
			btcmdlen = 7;
			break;
		
		case CMD_SIRI:
			memcpy((void*)&btcmdbuf[0], (void*)"at+siri", 7);
			btcmdlen = 7;
			break;
			
	  case CMD_ACCEPTCALL:
			memcpy((void*)&btcmdbuf[0], (void*)"at+acce", 7);
			btcmdlen = 7;
			break;
		
		case CMD_REJECTCALL:
			memcpy((void*)&btcmdbuf[0], (void*)"at+reje", 7);
			btcmdlen = 7;
			break;
			
	  case CMD_RECALL:
			memcpy((void*)&btcmdbuf[0], (void*)"at+call", 7);
			btcmdlen = 7;
			break;
			
	  case CMD_DISCONNECT:
			memcpy((void*)&btcmdbuf[0], (void*)"at+disc", 7);
			btcmdlen = 7;
			break;
		
		case CMD_TWS:
			memcpy((void*)&btcmdbuf[0], (void*)"at+ttws", 7);
			btcmdlen = 7;
			break;
			
		case CMD_RING1:
			memcpy((void*)&btcmdbuf[0], (void*)"at+rin1", 7);
			btcmdlen = 7;
		  break;
		
		case CMD_RING2:
			memcpy((void*)&btcmdbuf[0], (void*)"at+rin2", 7);
			btcmdlen = 7;
		  break;
		
		default:
			break;
  }
	
//	Uart0_Send_nData(btcmdbuf, btcmdlen);
				
}


uint8_t Mcu_RcvBTCmd(uint8_t Rx_Data)
{
    static uint8_t xdata Rx_Index = 0;
	  static uint8_t xdata Cmd_Len =0;
	
	  if(BT_Uart.Rx_Timeout == 0)
		{
        Rx_Index = 0;
			  Cmd_Len = 0;
    }

    BT_Uart.Rx_Timeout = RX_TIMEOUT_TIME;
		
	  switch(Rx_Index)
	  {
        case 0:
					if(Rx_Data == 'a')
						Rx_Index = 1;
					break;
					
				case 1:
					if(Rx_Data == 't')
						Rx_Index = 2;
					else
						Rx_Index = 0;
					break;
					
				case 2:
					if(Rx_Data == '+')
					{
						Rx_Index = 3;
						Cmd_Len = 0;
					}
					else
						Rx_Index = 0;
					break;
					
				case 3:
					BT_Uart.gBTCmd[Cmd_Len] = Rx_Data;
				  Cmd_Len++;
				  if(Cmd_Len >= 4)
					{						   
              Rx_Index = 0;
						  return 1;
          }
					break;
				
				default:
					Rx_Index = 0;
					break;
    }
		return 0;
}


void BT_CmdProcess(void)
{
  if(BT_Uart.BTCmdRcvFinishFlag == 0)
	{
    return;
  }
	BT_Uart.BTCmdRcvFinishFlag = 0;
	
	if(memcmp(BT_Uart.gBTCmd, "moda", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "modb", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "modc", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
		
  }
	else if(memcmp(BT_Uart.gBTCmd, "modd", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }	
	else if(memcmp(BT_Uart.gBTCmd, "mode", 4) == 0)
	{//FM
    BT_Uart.BTCmdResendFlag = 0;
		FM_Freq_LED_Display(sys_state.FMFreq);
		FM_INIT_COUNNT = 100;
		FM_INIT_STATE = 1;
  }
	else if(memcmp(BT_Uart.gBTCmd, "modf", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }	
	else if(memcmp(BT_Uart.gBTCmd, "prev", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "next", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "disc", 4) == 0)
	{
    BT_Operating_State = BT_DISCONNECT;
  }
	else if(memcmp(BT_Uart.gBTCmd, "conn", 4) == 0)
	{
    BT_Operating_State = BT_CONNECT;
  }
	else if(memcmp(BT_Uart.gBTCmd, "play", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "paus", 4) == 0)
	{

  }
	else if(memcmp(BT_Uart.gBTCmd, "siri", 4) == 0)
	{

  }
	else if(memcmp(BT_Uart.gBTCmd, "call", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "tele", 4) == 0)
	{

  }
	else if(memcmp(BT_Uart.gBTCmd, "star", 4) == 0)
	{
		BT_Inti_Flag = 1;
		if(sys_state.Mode == SYS_BLUETOOTH)
    {
      BT_Uart.BTCmd_Temp = CMD_BT;
    }	
    else if(sys_state.Mode == SYS_AUX)
		{
      BT_Uart.BTCmd_Temp = CMD_AUX;
    }
		else if(sys_state.Mode == SYS_FM)
		{
      BT_Uart.BTCmd_Temp = CMD_FM;  
    }
		else if(sys_state.Mode == SYS_CLOCK)
		{
      BT_Uart.BTCmd_Temp = CMD_CLOCK;
    }
		else if(sys_state.Mode == SYS_ALARM1 || sys_state.Mode == SYS_ALARM2)
		{
			BT_Uart.BTCmd_Temp = CMD_CLOCK;
		}	  
		BT_Uart.BTCmdResendFlag = 1;
		BT_Uart.BT_Init_Count = 0;	
  }
	else if(memcmp(BT_Uart.gBTCmd, "mute", 4) == 0)
	{
		if(sys_state.Mode == SYS_FM)
		{
      Mode_Init_Flag = 1;
    }
		if(BT_Inti_Flag)
//		if((CMD_VOLUME < MAX_VOLUME) && (CMD_VOLUME > 0))
    {
			BT_Inti_Flag = 0;
//			Mcu_SendCmdToBT(CMD_VOLUME);
			BT_Uart.BTCmd_Temp = CMD_VOLUME;
			BT_Uart.BTCmdResendFlag = 1;
//		  BT_Uart.BT_Init_Count = 0;
    }
		
    if(Alarm_Ring_Flag)
		{
			BT_Uart.BTCmd_Temp = CMD_RING1;
		  BT_Uart.BTCmdResendFlag = 1;
		}	
		
  }	
	else if(memcmp(BT_Uart.gBTCmd, "batt", 4) == 0)
	{//µÍµçÁ¿
    sys_state.Power = SYS_POWER_SLEEP;
		LED_Display_Off();
		BT_Power_Ctrl(BT_POWER_OFF);
    Enter_Sleep_Conf();
  }
	else if(memcmp(BT_Uart.gBTCmd, "vol", 3) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "rin1", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "rin2", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
	else if(memcmp(BT_Uart.gBTCmd, "ttws", 4) == 0)
	{
    BT_Uart.BTCmdResendFlag = 0;
  }
}


void BTCmd_Resend(void)
{
	if(BT_Uart.BT_Init_Count >= 80 && BT_Uart.BTCmdResendFlag && BT_Uart.BTCmdResendTime_count >= 5)
	{  
    Mcu_SendCmdToBT(BT_Uart.BTCmd_Temp);
		BT_Uart.BTCmdResendTime_count = 0;
	}
}


#endif

