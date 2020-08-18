#ifndef __UPSTAND_H__
#define __UPSTAND_H__

#include  "common.h"

/**********ȫ�ֱ����ⲿ����********/
extern int16 Offset_Acce;
extern int32 g_nValueOfAcce;
extern float g_fAngleOfAcce;
extern int32 g_nValueOfGyro[2];
extern float Raito_Gyro[2];
extern float Offset_Gyro[2];
extern float g_fAngleSpeed[2];
extern float g_fAngleOfGyro[2];
extern float g_AngleOfCar;
extern float g_BlanceControlOut;
extern float g_upstandControl_P;
extern float g_upstandControl_D;


/**********��������********/
void UpstandInit(void);
void GalAcceAndGyroY(void);
void CalAngleOfGyro(void);
void BalanceControl(void);
void OffsetGyroY();
void KalmanFilter(void);

typedef enum
{
  GyroY,
  GyroX
}Gyron;


#endif
