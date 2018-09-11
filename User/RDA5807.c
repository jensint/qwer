

#include "RDA5807.h"
#include "pll_i2c.h"
#include "app_config.h"

unsigned int xdata Rda5807Reg[4]={0xC001,0x0042,0x0c00,0x86a8};

code unsigned int RDA5807_FM_INIT_REG[] = 
{
	0xc001,  				//v10
	0x0042,  
	0x0c00,  //0x0400     
	0x16a8, 
	0x8000,  
	0x3ac6,//07H  0x46  修改立体声分离度
	0x5ec6,  //0x5ec6
	0x0000,  
	0x406e, 
	0x2d80,  
	0x5803,  
	0x5804,//0dh  
	0x5804,  
	0x5804,  
	0x0047, //10h
	0x9000,  
	0xF587,  
	0x0009,  
	0x00F1,  
	0x41c0, //15h
	0x41e0,  
	0x506f,  
	0x5592,  
	0x007d,  
	0x10E0, //1AH 
	0x0780,  
	0x3000,
	0x4006,  
	0x1f9B,  
	0x4c2b,
	0x8110, 
	0x4580,//21H
	0x153f,  
	0xaf40, 
	0x0481,  
	0x1b2a, 
	0x2D04,  
	0x802F,  
	0x178A,  
	0xD349,  
	0x1142, 
};


void _RDA_Write_REG(uint16_t *buf, uint8_t len)
{
	uint8_t valueL;
	uint8_t valueH;
	uint8_t i;
	
  PLL_I2C_Start();
	PLL_I2C_Write_Byte(0x20);
	for(i =0; i<len; i++)
	{
    valueL = (*(buf+i))&0xff;
		valueH = ((*(buf+i)>>8)&0xff);
		PLL_I2C_Write_Byte(valueH);
		PLL_I2C_Write_Byte(valueL);
  }
	PLL_I2C_Stop();
}



void _RDA_Read_Reg(uint16_t *buf, uint8_t len)
{
	uint8_t valueL;
	uint8_t valueH;
	uint8_t i;

	PLL_I2C_Start();
	PLL_I2C_Write_Byte(0x21);
	
	for(i =0; i<len; i++)
	{
		valueH = PLL_I2C_Read_Byte();	
		PLL_I2C_Ack();
		valueL = PLL_I2C_Read_Byte();
		
		if(i!= (len-1))
    {
			PLL_I2C_Ack();
		}
		else
		{
			PLL_I2C_NoAck();
		}
		*buf++ = (((unsigned int)valueH) << 8) | valueL;
	}
	PLL_I2C_Stop();
}

uint8_t RDA5807_Write_REG(uint8_t addr, uint16_t dat)
{
	
  PLL_I2C_Start();
	
	if(PLL_I2C_Write_Byte(RDA5807_CHIP_ADDR|WRITE))
	{
    PLL_I2C_Write_Byte(addr);
		PLL_I2C_Write_Byte(dat>>8);
		PLL_I2C_Write_Byte(dat&0xff);
		PLL_I2C_Stop();
		return 1;
  }
	PLL_I2C_Stop();
	return 0;
}



uint16_t RDA5807_Read_REG(uint8_t addr)
{
	uint16_t dat;
	
  PLL_I2C_Start();
	
	PLL_I2C_Write_Byte(RDA5807_CHIP_ADDR|WRITE);
	PLL_I2C_Write_Byte(addr);
	
	PLL_I2C_Start();
	PLL_I2C_Write_Byte(RDA5807_CHIP_ADDR|READ);
	dat |= PLL_I2C_Read_Byte();
	PLL_I2C_Ack();
	dat <<= 8;
  dat |= PLL_I2C_Read_Byte();
	PLL_I2C_NoAck();
  PLL_I2C_Stop();

  return dat;
}




uint8_t RDA5807_ReadID(void)
{
	RDA5807_Write_REG(RDA_REG_02, 0x0002);  //软复位
	Delay_ms(200);
	
  if(RDA5807_Read_REG(RDA_REG_00) == CHIP_ID)
	{
    return 1;
  }
	return 0;
}

void to_RDA5807_Init(uint8_t mode)
{
		if(mode==1)
		{
			return ;
		}
		else
		{	
			RDA5807_Init();
		}
			
}

void RDA5807_Init(void)
{
	uint16_t RDA_REG[4];
	
	RDA_REG[0] = 0x0002;   //先进行软复位
	_RDA_Write_REG(RDA_REG, 1);			
	Delay_ms(100);				
	RDA_REG[0] = 0xc001;			//往02H中写入0XC001：使用32.768K频率
	_RDA_Write_REG(RDA_REG, 1);
	Delay_ms(100);
//	RDA_REG[0] = 0xc001;
//	RDA_REG[1] = 0x0042;
//	RDA_REG[2] = 0x0c00;
//	RDA_REG[3] = 0x86a8;
//	_RDA_Write_REG(RDA_REG, 4);
	_RDA_Write_REG(RDA5807_FM_INIT_REG,sizeof(RDA5807_FM_INIT_REG)/2);
//	_RDA_Write_REG(Rda5807Reg, 4);
	Delay_ms(100);
//  RDA5807_Write_REG(RDA_REG_02, 0x0002);  //软复位
//	Delay_ms(100);
//	RDA5807_Write_REG(RDA_REG_02, 0xc001);
//	RDA5807_Write_REG(RDA_REG_03, 0x0000);
//  RDA5807_Write_REG(RDA_REG_04, 0x0400);
//	RDA5807_Write_REG(RDA_REG_05, 0x86af);
//	
	RDA5807_FreqSet(sys_state.FMFreq);
//	Delay_ms(50);
	
}



void RDA5807_FreqSet(uint16_t freq)
{
	unsigned int reg[2] = {0x0000,0x0000};
  uint16_t chan;
	
	if((freq < 875) || (freq > 1080))
  {
    freq = 875;
  }
	
  chan = sys_state.FMFreq - 870;
	Rda5807Reg[1]= (Rda5807Reg[1]&0x003f)|(chan<<6);
//	temp = ((sys_state.FMFreq - 870)<<6)|0x0010;
	reg[0]=Rda5807Reg[0];
	reg[1]=(Rda5807Reg[1])|0x0010;   //波段：87~108.频率间距：50K Hz
//	RDA5807_Write_REG(RDA_REG_03, temp);
	_RDA_Write_REG(reg, 2);
	
//	Delay_ms(100);
//	
//	if((RDA5807_Read_REG(RDA_REG_0B)&(1<<8)))
//	{
//    Uart0_Send_nData("FM_SUCC", 7);
//  }
//	else
//	{
//    Uart0_Send_nData("FM_FAIL", 7);
//  }
	
}



uint16_t RDA5807_Seek_Channel(void)
{
  uint16_t temp;
	unsigned int reg[4] = {0x0000,0x0000};

	uint8_t reg_data[4] = {0};

//  temp = RDA5807_Read_REG(RDA_REG_02);
//  temp |=  (1<<8);

//	reg[0]= temp;
  reg[0] = Rda5807Reg[0]|(3<<8);		//向上搜索、开启搜索控制
  reg[1] = Rda5807Reg[1]&(1<<4);		//启用调谐

  _RDA_Write_REG(reg, 2);

  while((RDA5807_Read_REG(RDA_REG_0A)&(1<<14)) == 0) //等待STC完成
	{
    Delay_ms(30);
  }
	
	temp = RDA5807_Read_REG(RDA_REG_0A)&0x03FF + 870;    //读低9位的频率
	return temp;
}



uint8_t RDA5807_Auto_Search(void)
{
	unsigned int rd_reg[2];
	
	do
	{
//		Uart0_Send_nData("WAITSTC", 7);
    _RDA_Read_Reg(rd_reg, 1);
  }
	while(!(rd_reg[0]&0x4000));//等待STC完成
	
	_RDA_Read_Reg(rd_reg, 2);
	
	if(rd_reg[1]&0x0100) //电台
	{
//		Uart0_Send_nData("ISFM", 4);
    if((rd_reg[1]>>9)>9)//&&(fm_current_freq!=9600)) 
		{
			return 1;	//rda5830_reg.value>>9:rssi max:0x7f
		}
  }
//	freq = RDA5807_Seek_Channel();
//	
//	if((RDA5807_Read_REG(RDA_REG_0B)&(1<<8)))
//	{//当前频道是电台
//		return 1;
//  }
//	
////	if((RDA5807_Read_REG(RDA_REG_0B)&(1<<8)))
//	
	return 0;
	
}
