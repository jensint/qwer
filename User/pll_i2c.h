#ifndef __PLL_I2C_H__
#define __PLL_I2C_H__

#include "ca51f_config.h"
#if (IC_TYPE == IC_TYPE_CA51F2X)
#include "ca51f2sfr.h"
#include "ca51f2xsfr.h"
#include "gpiodef_f2.h"
#elif (IC_TYPE == IC_TYPE_CA51F3X)
#include "ca51f3sfr.h"
#include "ca51f3xsfr.h"
#include "gpiodef_f3.h"
#endif



#define PLL_I2C_SCL_TCON    P14F
#define PLL_I2C_SCL_DATA    P14

#define PLL_I2C_SDA_TCON    P15F
#define PLL_I2C_SDA_DATA    P15


#define SetOutputSCL()    GPIO_Init(PLL_I2C_SCL_TCON, OUTPUT)
#define PLL_SCL(n)        {if(n) PLL_I2C_SCL_DATA = 1; else PLL_I2C_SCL_DATA = 0;}

#define SetInputSDA()     GPIO_Init(PLL_I2C_SDA_TCON, INPUT)
#define SetOutputSDA()    GPIO_Init(PLL_I2C_SDA_TCON, OUTPUT)
#define PLL_SDA(n)        {if(n) PLL_I2C_SDA_DATA = 1; else PLL_I2C_SDA_DATA = 0;} 
#define GetSDA()          PLL_I2C_SDA_DATA



void PLL_I2C_Start(void);
void PLL_I2C_Stop(void);
void PLL_I2C_Ack(void);
void PLL_I2C_NoAck(void);
uint8_t PLL_I2C_CheckAck(void);
uint8_t PLL_I2C_Write_Byte(uint8_t dat);
uint8_t PLL_I2C_Write_NByte(uint8_t *datbuff, uint8_t len);
uint8_t PLL_I2C_Read_Byte(void);
uint8_t PLL_I2C_Read_NByte(uint8_t *datbuff, uint8_t len);


#endif

