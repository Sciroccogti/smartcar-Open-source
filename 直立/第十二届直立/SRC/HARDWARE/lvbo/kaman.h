#include "sys.h"
//#include "jifen.h"
#include <math.h>
#include "oled.h"
#define TYPEKAMN int16_t
/*________���ٶ��˲���____________*/
/*________���ٶ��˲���____________*/
/*________���ٶ��˲���____________*/



/*________���ٶ��˲���____________*/


/*________���ٶ��˲���____________*/
/*________���ٶ��˲���____________*/
/*________���ٶ��˲���____________*/
#define Accelerometer_R (240)//���ٶȼƵ�R
#define Accelerometer_Q (25)//���ٶȼƵ�Q
#define Gyroscope_R (320)//�����ǵ�R
#define Gyroscope_Q (40)//�����ǵ�Q
#define Gyroscope_R_Z (320)
#define Gyroscope_Q_Z (40)





#define Diroct_R (160)//�����ǵ�R
#define Diroct_Q (80)//�����ǵ�Q
TYPEKAMN Kaman_Accelerometer(TYPEKAMN signal);
TYPEKAMN Kaman_Gyroscope(TYPEKAMN signal);
TYPEKAMN Kaman_Gyroscope_Z(TYPEKAMN signal);
float Kalman_LvBo(int16_t angle, int16_t Gyro_X);
double Kaman_Angle(double signal);
int Kaman_Diroct_Out(int data);

//void Kaman_Init(void);
/*
  500-3
	100-0.8
	300-0.8
	      Q:��������,Q����,��̬��Ӧ��죬�����ȶ��Ա仵
        R:��������,R���󣬶�̬��Ӧ�����������ȶ��Ա��
*/
//extern uint16_t pianzhi ;

