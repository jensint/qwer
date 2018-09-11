#ifndef TIMER_H
#define TIMER_H




typedef enum {
	TIMER_Mode0 = 0,
	TIMER_Mode1 = 1,
	TIMER_Mode2 = 2,
	TIMER_Mode3 = 3,
} TIMER_Mode_TypeDef;


typedef enum {
	TIMER_Func_Timing = 0,
	TIMER_Func_Counting = 1,
} TIMER_Fun_TypeDef;


typedef enum {
  TIMER_Gate_Mode_Tr = 0,
  TIMER_Gate_Mode_Int  = 1,
}
TIMER_Gate_Mode_TypeDef;



void Timer0_Init(TIMER_Mode_TypeDef timer_mode,
                 TIMER_Fun_TypeDef timer_func,
								 unsigned char interrupt_sw,
								 TIMER_Gate_Mode_TypeDef timer_gate_mode,
								 unsigned int div);


#endif
