#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include  "common.h"



/**********ȫ�ֱ����ⲿ����********/
extern float g_dirControl_P;
extern float g_dirControl_D;
extern float g_fDirectionError[2];
extern float g_fDirectionError_dot[2];
extern float g_fDirectionControlOut;
extern int16 g_ValueOfAD[4];
extern int16 g_ValueOfADFilter[4];




/***********��������***********/

void DirectionControl(void);
void Read_ADC(void);

#endif