#ifndef __MYMATH_H__
#define __MYMATH_H__
#include "stdint.h"
#ifdef __cplusplus
	extern "C" {
#endif
		
float mult_r(float get_data,float mult_get);
int16_t Math_Abs(int16_t value);//�����ֵ		
float Math_Sign(float value,float range);//��λб�º���		
float Math_Limit(float value, int limit);//���Ʒ���		
float Math_Max(float value1, float value2);//�����ֵ		
float Math_Min(float value1, float value2);//����Сֵ		
float Math_Hysteresis(float value,float Hys_range);//���ͺ���		
float FIFO(double data_in,double* data_out,int DATACNT);//�����ȳ��Ĵ���		
float Kalman_Buffer(float *Data,int DataCnt);//�����˲�		
int kalman_ACC1(int data);		
int kalman_ACC2(int data);
int kalman_GYRO1(int data);
int kalman_GYRO2(int data);		
int kalman_OUT(int data);		
float LegendreP(int n, float Value);
double Math_Sqrt(double Value,int cnt);
double Math_cos(double Value);
double Math_sin(double Value);
double Math_In(double Value,int cnt);
double LaguerreL(int N, double alpha, double Value);
float Buffer(float data);
float kalman_Steer(float data);
#ifdef __cplusplus
}
#endif

#endif		