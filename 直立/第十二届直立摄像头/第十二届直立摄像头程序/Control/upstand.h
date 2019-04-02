#ifndef __UPSTAND_H__
#define __UPSTAND_H__

#include  "common.h"

/**********全局变量外部申明********/
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


/**********函数申明********/
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
