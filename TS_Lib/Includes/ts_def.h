#ifndef _TS_DEF_H_
#define _TS_DEF_H_


//TKCON����
#define TKST(N)		(N<<7)	 //N=0-1
#define TKIE(N)		(N<<6)	 //N=0-1
#define TMEN(N)		(N<<5)	 //N=0-1	//RTC����TKST
#define TWKE(N)		(N<<4)	 //N=0-1	//ʡ���Զ�����	  0��TKST=0������TKIF, 1: TKST=0������Ƚ�������Ų���TKIF 

#define VRS(N)		(N<<0)	 //N=0-7	//�Ƚ����ο���ѹѡ��

//TKCFG����
#define TKDIV(N)	(N<<5)	 //N=0-7	//ʱ�ӷ�Ƶ		��Ƶϵ�� ��TKDIV+1����0������Ƶ
#define TKTMS(N)	(N<<0)	 //N=0-31	//�ŵ�ʱ������   �ŵ�ʱ�䣺 (TKTMS)*32*Ttk  , TKTMS�������0�����Ϊ0���ŵ� 

//TKCHS����				   		//�������� INDEX=0-5
#define POL(N)      (N<<7) 	 	//N=0-1	   //0- TKMS <  ATKC; 1-  TKMS >=  ATKC
#define NPOL(N)      (N<<6) 	 	//N=0-1	   //0- TKMS <  ATKN; 1-  TKMS >=  ATKN
#define TKCHS(N)    (N<<0) 		//N=0-25   0����ѡ    1-24��ӦTK0-TK23; 25��ӦTKCAP

//TKIF����					  //TKST=0��һ����λ
#define TKIF5		(1<<5)
#define TKIF4		(1<<4)
#define TKIF3		(1<<3)
#define TKIF2		(1<<2)
#define TKIF1		(1<<1)
#define TKIF0		(1<<0)

typedef union 
{	
	unsigned long int 	dwVal;
	unsigned int 		wVal[2];
	unsigned char 		bVal[4];
}
DWORD_UNION;

typedef union 
{	
	unsigned int 	wVal;
	unsigned char 	bVal[2];
}
WORD_UNION;

typedef enum
{
	 TS_INIT,
	 TS_DEAL
} TSState_T;

enum 
{
	AREA_OVER_HIGH,
	AREA_FINGER_TOUCH,
	AREA_PNOISE_PLUS,
	AREA_PNOISE,
	AREA_NNOISE,
	AREA_OVER_LOW		
};

enum 
{
	WHEEL_SLIDER_NO_TOUCH,
	WHEEL_SLIDER_TOUCH,	
};


typedef struct
{
	unsigned char  MinChIdx;
	unsigned char  SecMinChIdx;
	unsigned int Delta1;
	unsigned int Delta2;	
}TouchToggleInfo_TypeDef;


enum
{
	TS_CH0,
	TS_CH1,
	TS_CH2,
	TS_CH3,
	TS_CH4,
	TS_CH5,
	TS_CH6,
	TS_CH7,
	TS_CH8,
	TS_CH9,
	TS_CH10,
	TS_CH11,
	TS_CH12,
	TS_CH13,
	TS_CH14,
	TS_CH15,
	TS_CH16,
	TS_CH17,
	TS_CH18,
	TS_CH19,
	TS_CH20,
	TS_CH21,
	TS_CH22,
	TS_CH23,
};


#endif
