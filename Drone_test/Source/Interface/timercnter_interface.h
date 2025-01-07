#ifndef _TIMERCNT_INTERFACE_H_
#define _TIMERCNT_INTERFACE_H_

extern void init_Timer_Counter(void);
extern void Start_Timer_counter(void);
extern void Stop_Timer_counter(void);
extern void Pause_Timer_counter(void);
extern void Refresh_Timer_counter(void);
extern unsigned int millis(void);
extern uint64_t micros(void);
extern void Delay_ms_(unsigned int delay_ms);
#endif 
