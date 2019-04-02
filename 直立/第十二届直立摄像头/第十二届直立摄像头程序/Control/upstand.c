/*!
 *		��������ѧԺֱ������ͷ
 * @file       upstand.c
 * @brief      ƽ�ⲿ�ֳ���
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 *
 *		�����˲��ͷǾ��󿨶����˲����ܺ���
 *		���󿨶������Թ�������
 *		����ʹ�ü�Ȩ��ƽ��ƽ���˲���������ͺ�
 */

#include  "common.h"
#include  "include.h"
#include  "math.h"



/**********ȫ�ֱ�������********/
//���ٶȼ�
int32 g_nValueOfAcce;	
float g_fAngleOfAcce;
int16 Offset_Acce = 680;//���ٶȼ�ƫ��ֵ��700��Сǰ�㡣
float Ratio_Acce = 0.105;
//������
int32 g_nValueOfGyro[2];
float g_fAngleSpeed[2];
float Offset_Gyro[2] = {0,0};
float Raito_Gyro[2] = {0.24, 0.24};
float angle_Difference;
//�����˲�
float g_fAngleOfGyro[2];
float Const_Time = 3;
uint8 Freq_Inte = 200;
float g_BlanceControlOut;

float g_upstandControl_P = 7.5;		//ֱ��P
float g_upstandControl_D = 0.3;		//ֱ��D
//�Ǿ��󿨶����˲�����Щ�������ø�
float Peried = 1/150.0;				//��������������
float KalmanGain = 1.0;//����������
float Q = 2.0;//��������2.0		ԽС����Խ�������ټ��ٶȼ�Խ��Խƽ��
float R = 5000.0;//��������5000.0	ԽС���ټ��ٶȼ�Խ��
float g_AngleOfCar;					//����Ƕ�ֵ

/**
 * @note		��ʼ��I2C�����ٶȼƺ�������
 * @author		��������ѧԺ
 * @date		2017
 */
void UpstandInit(void)
{
 	gpio_init(PTC10,GPO,1);		//��ʼ��SCL�ܽ�
 	gpio_init(PTC11,GPO,1);		//��ʼ��SDA�ܽ�
 	MPU3050_Init();
 	MMA8451_Init();
	OffsetGyroY();				//����������ƫ��ֵ������ʱ��������

}

/**
 * @file		��ȡ���ٶȼƵ�ֵ������ɽǶ�
 * @file		��ȡ�����ǵ�ֵ��ת��Ϊ���ٶ�
 * @note      	�õ�ȫ�ֱ���g_nValueOfAcce��g_fAngleOfAcce
 * @note      	�õ�ȫ�ֱ���g_nValueOfGyro[2], g_fAngleSpeed
 * @author		��������ѧԺ
 * @date		2017
 */
void GalAcceAndGyroY(void)
{
  	int16 nValueOfAcce = 0;
	int16 nValueOfGyro = 0;
	int8 i = 0;
	for(i=0;i<5;i++)	//������μ��ٶȼ�������ƽ��
	{
		nValueOfAcce += Get_Z_Acc()/5;
		nValueOfGyro += Get_Y_Gyro()/5;
	}
	
	g_nValueOfAcce = (int32)(nValueOfAcce - Offset_Acce);
	g_fAngleOfAcce = Ratio_Acce * g_nValueOfAcce;

	g_nValueOfGyro[GyroY] = (int32)(nValueOfGyro - Offset_Gyro[GyroY]);
	g_fAngleSpeed[GyroY] = Raito_Gyro[GyroY]*g_nValueOfGyro[GyroY];
		  
}


/**
 * @file		�����˲�
 * @file		�����ǻ��ֵõ��Ƕ�
 * @note      	�õ�ȫ�ֱ���g_fAngleOfGyro[2]
 * @author		��������ѧԺ���ο��廪������
 * @date		2017
 */
void CalAngleOfGyro(void)
{
	float fDeltaValue;
	static float fAngleOfgyro0;
	fAngleOfgyro0 = g_fAngleOfGyro[0];
	fDeltaValue = (g_fAngleOfAcce - fAngleOfgyro0)/Const_Time;
	g_fAngleOfGyro[0] += (g_fAngleSpeed[0] + fDeltaValue)/Freq_Inte;
	
}


/**
 * @file		�Ǿ��󿨶����˲�
 * @note      	������ٶ�g_fAngleSpeed[0]���Ƕ�g_fAngleOfAcce
 * @note      	�õ�ȫ�ֱ���g_AngleOfCar
 * @date		  
 */
void KalmanFilter(void)
{
	//�������˲��ֲ�����
    static float Priori_Estimation = 0;//�������
    static float Posterior_Estimation = 0;//�������
    static float Priori_Convariance = 0;//���鷽��
    static float Posterior_Convariance = 0;//���鷽��
		
	//�������˲�
    //1.ʱ�����(Ԥ��) : X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k) 
    Priori_Estimation = Posterior_Estimation + g_fAngleSpeed[0]*Peried;		//������ƣ����ֻ�ýǶ�
    //2.��������Э����  : P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k) 
    Priori_Convariance = (float)sqrt( Posterior_Convariance * Posterior_Convariance + Q * Q );
	
    //  ������������ƣ���������  
    // 1.���㿨��������  : K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k)) /
    KalmanGain = (float)sqrt( Priori_Convariance * Priori_Convariance / ( Priori_Convariance * Priori_Convariance + R * R ) );
    //2.��������(У��): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1)) 
    Posterior_Estimation  = Priori_Estimation + KalmanGain * (g_fAngleOfAcce - Priori_Estimation );
    // 3.���º���Э����  : P(k|k) =��I-K(k)*H(k)��*P(k|k-1) 
    Posterior_Convariance = (float)sqrt(( 1 - KalmanGain ) * Priori_Convariance * Priori_Convariance );
    //�õ����սǶ� 
    g_AngleOfCar = Posterior_Estimation;
  
}

/**
 * @file		ƽ����Ƶ�PD����
 * @note      	�õ�ȫ�ֱ���g_BlanceControlOut
 * @author		��������ѧԺ
 * @date		2017
 */
void BalanceControl(void)
{
	static float g_upstandControl_PWMTemp[5];
	float fPWM;
	
//	if(g_AngleOfCar<-55||g_AngleOfCar>0)Flag_Stop = 1;//��������
	
	fPWM = (( - g_AngleOfCar*g_upstandControl_P)+( - g_fAngleSpeed[0]*g_upstandControl_D));
//	fPWM = (( - g_AngleOfCar*float01)+( - g_fAngleSpeed[0]*float02));
	
	g_upstandControl_PWMTemp[4]=g_upstandControl_PWMTemp[3];
  	g_upstandControl_PWMTemp[3]=g_upstandControl_PWMTemp[2];
  	g_upstandControl_PWMTemp[2]=g_upstandControl_PWMTemp[1];
  	g_upstandControl_PWMTemp[1]=g_upstandControl_PWMTemp[0];
  	g_upstandControl_PWMTemp[0]=fPWM;
	
	g_BlanceControlOut = 1*g_upstandControl_PWMTemp[0] + 0*g_upstandControl_PWMTemp[1];
}

/**
 * @file		ֱ�������ǵĿ�������ʼ��
 * @note      	�ɼ�1000��ƽ��
 * @author		��������ѧԺ
 * @date		2017
 */
void OffsetGyroY()
{
	int16 i;
	float Offset_Gyros = 0;
	for(i=0; i<1000; i++ )
	{
		Offset_Gyros += (Get_Y_Gyro()/1000.0);
	}
 	Offset_Gyro[0] = Offset_Gyros;
}






