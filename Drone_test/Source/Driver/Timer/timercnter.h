#ifndef _TIMERCNT_H_
#define _TIMERCNT_H_

#include "Common_Datatype.h"

void init_Timer_Counter(void);
void Start_Timer_counter(void);
void Stop_Timer_counter(void);
void Pause_Timer_counter(void);
void Refresh_Timer_counter(void);
unsigned int millis(void);
uint64_t micros(void);
void Delay_ms_(unsigned int delay_ms);
void Delay_us(uint32_t delay);
#endif 
