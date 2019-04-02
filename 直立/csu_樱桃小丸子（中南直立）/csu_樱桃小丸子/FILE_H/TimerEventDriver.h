#ifndef TIMEREVENTDRIVER_H
#define TIMEREVENTDRIVER_H


#define Event_0Cycle    2       //µ¥Î»MS
#define Event_1Cycle    5

void    systick_timing(uint32 time);
void    EventControlTimer_init(void);
void    TimerEvent_0_IRQHandler(void);
void    TimerEvent_1_IRQHandler(void);
void    SysTick_IRQHandler(void);
uint8    EventTimerDelay(uint16 ms, uint32 Order);
float    GetSysTime(void);
void    CommonDelay(uint32 ms);
float   TimeEnd(void);
void    TimeBegin(void);

#endif