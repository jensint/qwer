
#ifndef __RDA5807_H__
#define __RDA5807_H__

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

#define CHIP_ID   0x5800


#define RDA5807_CHIP_ADDR    0x20
#define READ     1
#define WRITE    0


#define RDA_REG_00    0x00  //¶Á³ö16Î»ID =0X5800
#define RDA_REG_02    0x02
#define RDA_REG_03    0x03
#define RDA_REG_04    0x04
#define RDA_REG_05    0x05
#define RDA_REG_0A    0x0A
#define RDA_REG_0B    0x0B



void _RDA_Write_REG(uint16_t *buf, uint8_t len);
void _RDA_Read_Reg(uint16_t *buf, uint8_t len);
uint8_t RDA5807_Write_REG(uint8_t addr, uint16_t dat);
uint16_t RDA5807_Read_REG(uint8_t addr);
uint8_t RDA5807_ReadID(void);
void RDA5807_Init(void);
void to_RDA5807_Init(uint8_t mode);
void RDA5807_FreqSet(uint16_t freq);
uint16_t RDA5807_Seek_Channel(void);
uint8_t RDA5807_Auto_Search(void);

#endif

