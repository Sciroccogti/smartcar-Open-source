#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include  "common.h"

/**********ȫ�ֱ����ⲿ����********/
extern float Direction_P;
extern float Direction_D;
extern float DirectionErr[2];
extern float DirectionErr_dot[2];
extern float DirectionOut;
extern int16 sensor[5];
extern int16 sensorFilter[5];

/***********��������***********/
void DirectionControl(void);
void Read_ADC(void);

#endif