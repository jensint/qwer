
#ifndef __BLUETOOTH_CONTROL_H__
#define __BLUETOOTH_CONTROL_H__



#include <intrins.h>
#include "app_config.h"

#define RX_TIMEOUT_TIME    10  //100ms

typedef enum{
	CMD_IDEL = 0,
	CMD_BT,
	CMD_AUX,
	CMD_FM,
	CMD_CLOCK,
	CMD_ALARM,
	CMD_PREV,
	CMD_NEXT,
	CMD_VOLUME,
	CMD_PLAYPUSE,
	CMD_SIRI,
	CMD_ACCEPTCALL,
	CMD_REJECTCALL,
	CMD_RECALL,
	CMD_DISCONNECT,
	CMD_TWS,
	CMD_RING1,
	CMD_RING2,
}BT_CMD;


enum{
	BT_IDEL = 0,
	BT_DISCONNECT,
	BT_CONNECT,
	BT_PLAY,
	BT_PUSE,
	BT_CALL,
};


typedef struct _BT_UART_STATE
{
	uint8_t BTCmdRcvFinishFlag;
  uint8_t Rx_Timeout;
	uint8_t BTCmdResendFlag;
	uint8_t BTCmdResendTime_count;
	uint8_t BTCmd_Temp;
	uint8_t gBTCmd[8];
	uint8_t BT_Init_Count;
}BT_UART_STATE;


extern BT_UART_STATE xdata BT_Uart;
extern uint8_t BT_Operating_State;

void Mcu_SendCmdToBT(uint8_t Cmd);
uint8_t Mcu_RcvBTCmd(uint8_t Rx_Data);
void BT_CmdProcess(void);
void BTCmd_Resend(void);


#endif

