#ifndef ADC_H
#define ADC_H

//ADCON����
#define AST(N)		(N<<7)					//ADC���� �� AST=0����
#define ADIE(N)		(N<<6)					//�ж�ʹ��
#define ADIF		(1<<5)					 	//�жϱ�־
#define HTME(N) 	(N<<2)					//N=0-7			 //����ʱ������, ʱ��Ϊ2��HTME�η���ʱ������
#define VSEL(N) 	(N)							//N=0-3			 //ѡ��ο���ѹ 0-�ڲ� 1-VDD 2-�ⲿ

//ADCFGL����													   
#define ACKD(N)			(N<<5)  			//N=0-7			 //ADCʱ�ӷ�Ƶ	  ��Ƶ����=��ACKD+1)
#define ADCALE(N)		(N<<4) 				//ADCУ����	  ѡ���ڲ��ο���ѹ����Ч 
#define ADCHS(N)		(N) 					//N=0-15		 //ADCͨ��ѡ�� �� 1-13��Ӧͨ��0-12

//ADCFGH����
#if (IC_TYPE == IC_TYPE_CA51F2X)	
	#define AOVF		(1<<7)					 //ADC���ݳ����趨��Χ�жϱ�־
	#define AOVE(N)		(N<<6)					 //ADC���ݳ����趨��Χ�ж�ʹ��
#elif (IC_TYPE == IC_TYPE_CA51F3X)
	#define AOPS(N)		(N<<6)					 //N=0~3,ADC�˷�����	
#endif

#define VTRIM(N)	(N)						 //0-63	   У���ڲ��ο���ѹ

#if (IC_TYPE == IC_TYPE_CA51F2X)	
//OPACON����
#define OPAM(N)		(N<<6)	  //n=0~3
#define OPAS(N)		(N<<0)	  //n=0~7
#endif

typedef enum {
  ADC_REF_INNER  	= 0,
  ADC_REF_VDD  		= 1,
  ADC_REF_Outer  	= 2
} ADC_RefVolt_TypeDef;

typedef enum {
  Disable = 0,
  Enable  = 1
}
InterruptSW_TypeDef,AmpSW_TypeDef;

#if (IC_TYPE == IC_TYPE_CA51F2X)	
typedef enum {
  AMP_GAIN_1_4 = 0,
  AMP_GAIN_1_3 = 1,
  AMP_GAIN_1_2 = 2,
  AMP_GAIN_5 	 = 3,
  AMP_GAIN_10  = 4,
  AMP_GAIN_15  = 5,
  AMP_GAIN_20  = 6,
  AMP_GAIN_30  = 7,
}AMP_Gain_TypeDef;
#elif (IC_TYPE == IC_TYPE_CA51F3X)
typedef enum {
  NO_AMP = 0,
  AMP_GAIN_1_4 = 1,
  AMP_GAIN_1_3 = 2,
  AMP_GAIN_1_2 = 3,
}AMP_Gain_TypeDef;
#endif



typedef enum {
  ADC_CH0  	= 1,
  ADC_CH1  	= 2,
  ADC_CH2  	= 3,
  ADC_CH3  	= 4,
  ADC_CH4  	= 5,
  ADC_CH5  	= 6,
  ADC_CH6  	= 7,
  ADC_CH7  	= 8,
  ADC_VDD  	= 9,
} ADC_Channel_TypeDef;

void ADC_StartConversion(void);
void ADC_init(ADC_RefVolt_TypeDef adc_ref,InterruptSW_TypeDef adc_int,AmpSW_TypeDef amp_sw,AMP_Gain_TypeDef gain);
unsigned int GetAdcValue(void);
void ADC_ConfigChannel(ADC_Channel_TypeDef channel);
#endif