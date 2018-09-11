
#include "pll_i2c.h"


void delay(uint8_t n)
{
//	_nop_();
//  while(n)
//	{
//    n--;
//  }
}


/******************************************************************************
函数名：	PLL_I2C_Start
功能描述：I2C开始时序
输入参数: 无
返回值：  无
******************************************************************************/
void PLL_I2C_Start(void)
{
  SetOutputSCL();
	SetOutputSDA();
	PLL_SCL(1);
	PLL_SDA(1);
	delay(1);
	PLL_SDA(0);
}



/******************************************************************************
函数名：	PLL_I2C_Stop
功能描述：I2C停止时序
输入参数: 无
返回值：  无
******************************************************************************/
void PLL_I2C_Stop(void)
{
	SetOutputSDA();
	PLL_SCL(0);
	PLL_SDA(0);
	delay(1);
	PLL_SCL(1);
	delay(1);
	PLL_SDA(1);
}


/******************************************************************************
函数名：	PLL_I2C_Ack
功能描述：I2C应答时序
输入参数: 无
返回值：  无
******************************************************************************/
void PLL_I2C_Ack(void)
{
	PLL_SCL(0);
	SetOutputSDA();
	delay(0);
	PLL_SDA(0);
	delay(1);
	PLL_SCL(1);
	delay(1);
	PLL_SCL(0);
}



/******************************************************************************
函数名：	PLL_I2C_NoAck
功能描述：I2C无应答时序
输入参数: 无
返回值：  无
******************************************************************************/
void PLL_I2C_NoAck(void)
{
	PLL_SCL(0);
	SetOutputSDA();
	PLL_SDA(1);
	delay(0);
	PLL_SCL(1);
	delay(1);
	PLL_SCL(0);
}



/******************************************************************************
函数名：	PLL_I2C_CheckAck
功能描述：等待I2C应答信号
输入参数: 无
返回值：  1:ack  0:nack
******************************************************************************/
uint8_t PLL_I2C_CheckAck(void)
{
	uint8_t ack;
	
	PLL_SCL(0);
	SetInputSDA();
	delay(0);
	PLL_SCL(1);
	delay(0);
	ack = !GetSDA();
	PLL_SCL(0);
	return ack;
}



/******************************************************************************
函数名：	PLL_I2C_Write_Byte
功能描述：写一个字节数据
输入参数: dat
返回值：  0:写失败  1:写成功
******************************************************************************/
uint8_t PLL_I2C_Write_Byte(uint8_t dat)
{
	uint8_t i;
	
	SetOutputSDA();
  PLL_SCL(0);
	delay(0);
	for(i = 0; i < 8; i++)
	{
    if(dat & 0x80)
		{
      PLL_SDA(1);
    }
		else
		{
      PLL_SDA(0);
    }
		dat <<= 1;
		PLL_SCL(1);
		delay(1);
		PLL_SCL(0);
  }

	return PLL_I2C_CheckAck();
}


/******************************************************************************
函数名：	PLL_I2C_Write_NByte
功能描述：写多个字节数据
输入参数: datbuff,len
返回值：  0:写失败  1:写成功
******************************************************************************/
uint8_t PLL_I2C_Write_NByte(uint8_t *datbuff, uint8_t len)
{
	while(len)
	{
		len--;
		if(PLL_I2C_Write_Byte(*datbuff))
		{
      datbuff++;
    }
		else
		{
      return 0;
    }
  }
	return 1;
}


/******************************************************************************
函数名：	PLL_I2C_Read_Byte
功能描述：读一个字节数据
输入参数: 无
返回值：  数据值
******************************************************************************/
uint8_t PLL_I2C_Read_Byte(void)
{
  uint8_t i;
	uint8_t dat = 0;
	
	PLL_SCL(0);
	SetInputSDA();
	
	for(i = 0; i<8; i++)
	{
		dat <<= 1;
    PLL_SCL(1);
		if(GetSDA())
		{
		  dat |= 0x01;
		}
		PLL_SCL(0);
  }
	SetOutputSDA();
	
	return dat;
}



/******************************************************************************
函数名：	PLL_I2C_Read_Byte
功能描述：读一个字节数据
输入参数: datbuff,len
返回值：  数据值
******************************************************************************/
uint8_t PLL_I2C_Read_NByte(uint8_t *datbuff, uint8_t len)
{
  while(len)
	{
		len--;
    *datbuff = PLL_I2C_Read_Byte();
		if(len)
		{
      PLL_I2C_NoAck();
    }
		else
		{
      PLL_I2C_NoAck();
    }
  }
	
	return 1;
}

