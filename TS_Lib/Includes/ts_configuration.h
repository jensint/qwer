#ifndef _TS_CFG_H
#define _TS_CFG_H
/******************************************************************************************************
ע�����
1. ������ҵ���(��TK_CAP�������ӵ���)����ֵ��20nF����203��
2. �������Ŵ������轨��ֵ��1Kŷķ
3. �˴�����ֱ�Ӵ������ɻ��������ʱ��ֻ֧�ֵ�������������Ҫ֧�ָ��ϰ���������ϴ���������
4. ������Ӧ���У��������������ݽӿ�ΪKeysFlagSN��KeysFlagSN��Чλ���Ϊ24��ÿλ��ʾһ���������Ƿ񱻴���
5. �궨��DEBUGΪԤ�����ܣ�Ŀǰ����ʹ��
********************************************************************************************************/

#define DEBUG 				0 	//1:�����ߵ��Թ���;	0:�ر����ߵ��Թ���

#if DEBUG
#undef UART0_EN
#endif 

#define SUPPORT_KEY							1				//1:֧�ִ�������;	0:��֧�ִ�������
#define SUPPORT_WHEEL_SLIDER		0				//1:֧��תȦ���� 0����֧��   ע�⣺תȦ�ͻ����ڳ�������������

#define SUPPORT_WATER_DETECT		0				//1:֧�ִ������ˮλ���ܣ� 0����֧��

#define ANTI_SPEAKPHONE				0					//1���򿪿��Խ������Ź��ܣ� 0���رտ��Խ������Ź��ܣ� 
/*******************************************************************************************************************/
#define SUPPORT_STOP_MODE_AUTO_SCAN				0	  //1��֧��  0����֧��  
//��ģʽ��STOPģʽ���Զ�ɨ�败��ͨ����ɨ�������оƬһֱ����STOP״̬�����������ݳ�����������ֵʱ����������жϲ�����CPU
// ע�⣺ 1 ��ģʽֻ֧��6������ͨ������CPU��������CPU�Ĵ���ͨ������6��ʱ����ģʽ������
//				2 ��ģʽʹ�ܺ���ͬʱ����Ҫ����CPU�Ĵ���ͨ�������л���WAKEUP(|WAKEUP)
//				3 ��ģʽ�¹����뿪��ͨ�����������Զ�ɨ����ʱ�䡢������ҵ��ݶ��й�ϵ
/*******************************************************************************************************************/
#define SUPPORT_STOP_MODE_MANUAL_SCAN			0		//1��֧��  0����֧��  
//��ģʽ��STOPģʽ���ֶ�����ɨ�败��ͨ������ɨ����һ�����������жϲ�����CPU������жϴ��������Ƿ񳬳���ֵ
//����������˳�STOPģʽ�������л�����һ��ͨ��������STOPģʽ��
// ע�⣺ 1 ��ģʽ�����ƻ���CPU��ͨ�����������ǹ��Ļ��ǰһ�ִ��ڲ�����6��ͨ��ʱ������ʹ��ǰһ��ģʽ
//				2 ��ģʽʹ�ܺ���ͬʱ����Ҫ����CPU�Ĵ���ͨ�������л���WAKEUP(|WAKEUP)
//				3 ��ģʽ�¹����뿪��ͨ����������ɨ����ʱ�䡢������ҵ��ݶ��й�ϵ
/*******************************************************************************************************************/
#if (SUPPORT_STOP_MODE_MANUAL_SCAN || SUPPORT_STOP_MODE_AUTO_SCAN)
	#define WAKEUP	0x80
#else
	#define WAKEUP	0x00
#endif
/*********************************************************************************************************************/
#if SUPPORT_KEY

/*******��ӡ���ض��壬���ڴ�����������**********************************************************/
	#define PRINT_TS_KEY_EN			0			//��ӡ��������ֵ����
	#define PRINT_TS_KEY_DATA_EN	0		//��ӡ�������ݿ���
	#if PRINT_TS_KEY_DATA_EN
		#define PRINT_CH			0				//��ӡͨ������,�������Ϊ0xFF�����ӡ���д�����ͨ��
	#endif
/***********************************************************************************************/

/********��������ͨ������������ֵ����***********************************************************/
	#define KEY_CH_COUNT			1			//��������������������ȷ��д
	#define KEY_SEQ						\
	{TS_CH2|WAKEUP,0x50},\
//	{TS_CH3|WAKEUP,0x48},			\
//	{TS_CH4|WAKEUP,0x44},    	\
//	{TS_CH5|WAKEUP,0x48},    	\
//	{TS_CH6|WAKEUP,0x48},    	\
//	{TS_CH7|WAKEUP,0x50},    	\
//	{TS_CH8|WAKEUP,0x68},    	\
//	{TS_CH9|WAKEUP,0x88},    	\
///*	|							|
//		|							|	
//		|							------> ��Ӧ����ͨ����ָ��������ʱ���ݱ仯����������д��ע�⣺������������ֵ����������ֵ�ȶ����ڴ���ֵ������ȡһ������
//		--------------------> �������ܴ���ͨ����ţ�������д�����ʹ����ʡ��ģʽ�Ҹô���ͨ��������Ҫ����MCU������ͨ����Ż����ϻ���WAKEUP(|WAKEUP).
//*/
/***********************************************************************************************/

/***********���������������������ʱ�䶨��************************************************************************************/	
#define FINGER_TOUCH_CONFIRM_TIME						3 				//������������ȷ��ʱ�䣬��λΪ15.625ms
#define FINGER_TOUCH_RELEASE_CONFIRM_TIME		3 				//���������ͷ�ȷ��ʱ�䣬��λΪ15.625ms
#define FINGER_LONG_TOUCH_TIME_LIMIT				30    		//���ư������ʱ�䣬��λΪ0.5s��������Ϊ0����رմ˹���	
/***********************************************************************************************/	
#else
	#define KEY_CH_COUNT			0
#endif
/*********************************************************************************************************************/
#if SUPPORT_WHEEL_SLIDER
//-----------------------------------------------------------------------------
	#define PRINT_WHEEL_SLIDER_POSITION_EN			1		//��ӡ����ת�̻���λ�ÿ���

	#define PRINT_WHEEL_SLIDER_DATA_EN	0						//��ӡ����ת�̻����������ݿ���
	#define PRINT_WHEEL_SLIDER_MAX_RATE_EN		0			//��ӡ������ֵ
	#if (PRINT_WHEEL_SLIDER_DATA_EN || PRINT_WHEEL_SLIDER_MAX_RATE_EN)
		#define PRINT_WHEEL_SLIDER_NUM			0					//��ӡ�ĸ�ת�̻���
	#endif
//-----------------------------------------------------------------------------
	#define WHEEL_SLIDER_COUNT					4			//ת�̻���������������ȷ��д
	
	#define WHEEL_SLIDER_TOUCH_CONFIRM_TIME						2 			//ת�̻�������ȷ��ʱ�䣬��λΪ15.625ms
	#define WHEEL_SLIDER_TOUCH_RELEASE_CONFIRM_TIME		2 			//ת�̻����ͷ�ȷ��ʱ�䣬��λΪ15.625ms	
	#define WHEEL_SLIDER_LONG_TOUCH_TIME_LIMIT				30    	//����ת�̺ͻ�������ʱ�䣬��λΪ0.5s��������Ϊ0����رմ˹���			
//------------------------------------------------------------------------------------	
	#if (WHEEL_SLIDER_COUNT > 0)
		#define WHEEL_SLIDER0_CH_COUNT			4		//��һ��ת�̻���ͨ����Ŀ��������ȷ��д
		
		#define WHEEL_SLIDER0_SEQ								\
		{TS_CH6|WAKEUP,0x30},		\
		{TS_CH7|WAKEUP,0x30},		\
		{TS_CH8|WAKEUP,0x28},		\		
		{TS_CH9|WAKEUP,0x30},		\		
/*		|								|
			|								|	
			|								------>��Ӧ����ͨ����ָ��������ʱ���ݱ仯����������д��ע�⣺������������ֵ����������ֵ�ȶ����ڴ���ֵ������ȡһ������
			-------------------->��һ��ת�̻�������ͨ����ţ������ת�̣���ת��˳ʱ�뷽��������д������ǻ����������������һ���µ��Ϸ���������д�� ���ʹ����ʡ��ģʽ�Ҹô���ͨ��������Ҫ����MCU������ͨ����Ż����ϻ���WAKEUP(|WAKEUP).
*/
		
		#define WHEEL_SLIDER0_CH_MAX_RATE		80,80,75,75		//��ָ����ʱ��ǰͨ�����ݱ仯��ռ�仯�������ͨ���仯��֮�͵������ʣ�˳��������ͨ������һһ��Ӧ��
		#define WHEEL_SLIDER0_POSITION_CORRECTION_FACTOR	96	//ת�̻���λ��У��ֵ
	#endif

		
//���µڶ������������ĸ�ת�̻�����˵���ο���һ��ת�̻���������������׸��
		
//------------------------------------------------------------------------------------		
	#if (WHEEL_SLIDER_COUNT > 1)
		#define WHEEL_SLIDER1_CH_COUNT			3		//�ڶ���ת�̻���ͨ����Ŀ��������ȷ��д
		
		#define WHEEL_SLIDER1_SEQ								\
		{TS_CH2|WAKEUP,0x48},		\
		{TS_CH1|WAKEUP,0x50},		\
		{TS_CH0|WAKEUP,0x20},		\		

		
		#define WHEEL_SLIDER1_CH_MAX_RATE		65,65,65
		#define WHEEL_SLIDER1_POSITION_CORRECTION_FACTOR	42
	#else
		#define WHEEL_SLIDER1_CH_COUNT			0
	#endif
//------------------------------------------------------------------------------------		
	#if (WHEEL_SLIDER_COUNT > 2)
		#define WHEEL_SLIDER2_CH_COUNT			3		//������ת�̻���ͨ����Ŀ��������ȷ��д
		
		#define WHEEL_SLIDER2_SEQ								\
		{TS_CH10|WAKEUP,0x50},		\
		{TS_CH11|WAKEUP,0x50},		\
		{TS_CH12|WAKEUP,0x30},		\		
	
		
		#define WHEEL_SLIDER2_CH_MAX_RATE		65,65,65
		#define WHEEL_SLIDER2_POSITION_CORRECTION_FACTOR	42
	#else
		#define WHEEL_SLIDER2_CH_COUNT			0
	#endif
//------------------------------------------------------------------------------------	
	#if (WHEEL_SLIDER_COUNT > 3)
		#define WHEEL_SLIDER3_CH_COUNT			3		//���ĸ�ת�̻���ͨ����Ŀ��������ȷ��д
		
		#define WHEEL_SLIDER3_SEQ								\
		{TS_CH15|WAKEUP,0x50},		\
		{TS_CH14|WAKEUP,0x50},		\
		{TS_CH13|WAKEUP,0x30},		\		

		
		#define WHEEL_SLIDER3_CH_MAX_RATE		65,65,70
		#define WHEEL_SLIDER3_POSITION_CORRECTION_FACTOR	-42
	#else
		#define WHEEL_SLIDER3_CH_COUNT			0
	#endif
//------------------------------------------------------------------------------------
#else
		#define WHEEL_SLIDER0_CH_COUNT	0
		#define WHEEL_SLIDER1_CH_COUNT	0
		#define WHEEL_SLIDER2_CH_COUNT	0
		#define WHEEL_SLIDER3_CH_COUNT	0
#endif

#if SUPPORT_WATER_DETECT
/************************************************************************************************
Ҫʵ��ˮλ��⹦�ܣ���Ҫѡ��һ�������ο�ͨ�����ڲο�ͨ�����ſ���оƬ�����Լ10pF���ݣ�����Ϊ+-5%���ڡ�
ˮλ����ԭ���ǣ�����ˮʱ�����ˮλ�Ĵ���ͨ������ֵ�����ӣ���Ӧ�Ĵ�����ֵ���С�����ˮλ��ͨ��������ֵ
�Ͳο�ͨ��������ֵ�ı��ʻ��½������˱��ʳ���ĳ��ֵʱ����Ϊ����ˮ���ο�ͨ���������ǿ����ų�оƬһ���Լ�����
���ص�Ӱ�졣		
*************************************************************************************************/
		#define PRINT_WATER_DET_CAP_RATE_EN				1	//��ӡ��ˮ��������

		#define WATER_REF_CAP_CH		TS_CH0	//�ο�����ͨ������
		#define WATER_DET_CH				TS_CH1  //�������ˮͨ������
		
//����˫����ֵ��Ŀ���Ǳ������ٽ�ֵ���������ݶ������״̬Ƶ���䶯
		#define WATER_RATE_THD				80			//����ˮ����ˮ������ֵ����
		#define NO_WATER_RATE_THD			85			//����ˮ����ˮ������ֵ����
		
		#define WATER_DET_FILTER_CNT	20			//�˲���������
		
		
//--------------------------------------------------------------------------------------		
		#define WATER_DETECT_CH_CNT		2
#else
		#define WATER_DETECT_CH_CNT		0
#endif
		
		
#define OPENED_TS_COUNT				(KEY_CH_COUNT+WHEEL_SLIDER0_CH_COUNT+WHEEL_SLIDER1_CH_COUNT+WHEEL_SLIDER2_CH_COUNT+WHEEL_SLIDER3_CH_COUNT+WATER_DETECT_CH_CNT)

//==============================================================================


#define FINGER_TOUCH_THRESHOLD_RATE  	70				//��ָ���ޱ�������ǰ����Ϊ70%

#if (SUPPORT_STOP_MODE_AUTO_SCAN || SUPPORT_STOP_MODE_MANUAL_SCAN)
	#define WAKEUP_THRESHOLD_RATE  	10					//��ֵ���ޱ���,�綨��Ϊ10����Ϊ��ָ�����������ݱ仯����10%
	#define ENTER_STOP_MODE_TIME		10						//���봥��ʡ��ģʽ����ʱ�䣬��λΪ0.5S
	#define SAVE_MODE_SCAN_INTERVAL_TIME	80		//ʡ��ģʽɨ����ʱ�䣬��λԼΪ1ms  
#endif

#define NOISE_THRESHOLD_RATE  	15		//��������ֵ���ʣ������ݱ仯���ڴ˷�Χ�ڣ���Ϊ����



#define AREA_PNOISE_PLUS_CONFIRM_TIME			90 		//��λΪ15.625ms
#define AREA_PNOISE_CONFIRM_TIME					50 		//��λΪ15.625ms
#define AREA_NNOISE_CONFIRM_TIME					50 		//��λΪ15.625ms
#define AREA_OVERLOW_CONFIRM_TIME					30 		//��λΪ15.625ms


#define AREA_PNOISE_PLUS_PERMITTED_DEVIATION_RATE			20 
#define AREA_PNOISE_PERMITTED_DEVIATION_RATE				15
#define AREA_NNOISE_PERMITTED_DEVIATION_RATE				15	
#define AREA_OVERLOW_PERMITTED_DEVIATION_RATE				15	


#define FILTER_COUNT								5		

//�����Ĵ�����������
#define TKDIV_VAL			0			//ʱ�ӷ�Ƶϵ��
#define TKTMS_VAL			15		//�ŵ�ʱ��
#define VRS_VAL				0			//�Ƚ�����ֵ

//ʡ��ģʽ��������
#define STOP_TKDIV_VAL		0
#define STOP_TKTMS_VAL			2		//�ŵ�ʱ��
#define STOP_VRS_VAL				5			//�Ƚ�����ֵ
//==============================================================================
/*******************************************************************************************************/
#endif



						 

