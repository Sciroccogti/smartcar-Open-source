#include "stdbool.h"



extern volatile uint32 SysTicks;
bool ESP8266_Init(void);
bool WriteATCommond(const char *str);
bool ESP8266_SendData(void);
void RxMain(void);
float GetSysTickTime(void);
