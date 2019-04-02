#include "kaman.h"
#include "extern.h"

#define T 0.085
int16_t Finaldata_Accelerometer=0;//�����˲���ʼֵ���ٶ�
int16_t Finaldata_Gyroscope=0;//�����˲���ʼֵ������X��
int16_t Finaldata_Gyroscope_Z=0;//�����˲���ʼֵZ��
int Finaldata_Diroct = 0;
////////////


float A[2][2] = {{1, -T}, {0, 1}};    //????
float B[2] = {T, 0}, H[2] = {1, 0};   //????
float Last_result;                    //???????
float Bias_angle;                     //????????????????????
float Now_Groy_palstance;             //?????????????????
float Gyro_measure_noise = 0;         //????????
float Acceleration_measure_noise = 0; //?????????
float Measure;                        //??????
float Q[2][2] = {{0.01, 0}, {0, 0.005}}, R = 0.6;
//??????????Q????????????R
float Now_measure_result[2]; //??????
float Now_best_result[2];    //????????
float Now_covariance[2][2];  //???Now_measure_result????
float last_covariance[2][2] = {{0.005, 0.005}, {0.005, 0.005}};
//???Last_best_result????
float Kalman_Gain[2];             //?????
float I[2][2] = {{1, 0}, {0, 1}}; //I????
float Real_angle;

float Kalman_LvBo(int16_t angle, int16_t Gyro_X)
{

    float P[2][2], K[2] ,D[2][2], S[2][2]; //????
    Now_Groy_palstance = (Gyro_X);          //??????
    Last_result = Now_best_result[0];       //?
    Bias_angle = Now_best_result[1];        //b
                                            //????         Z(K)
    Measure = angle;                        //?????

    //??(1)    ??????       X(K|K-1)
    Now_measure_result[0] = A[0][0] * Last_result + A[0][1] * Bias_angle + B[0] * Now_Groy_palstance + Gyro_measure_noise * T;
    Now_measure_result[1] = A[1][0] * Last_result + A[1][1] * Bias_angle;
    //??(2)    ???????          P(K/K-1)
    P[0][0] = A[0][0] * last_covariance[0][0] + A[0][1] * last_covariance[1][0];
    P[0][1] = A[0][0] * last_covariance[0][1] + A[0][1] * last_covariance[1][1];
    P[1][0] = A[1][0] * last_covariance[0][0] + A[1][1] * last_covariance[1][0];
    P[1][1] = A[1][0] * last_covariance[0][1] + A[1][1] * last_covariance[1][1];
    Now_covariance[0][0] = P[0][0] * A[0][0] + P[0][1] * A[0][1] + Q[0][0];
    Now_covariance[0][1] = P[0][0] * A[1][0] + P[0][1] * A[1][1] + Q[0][1];
    Now_covariance[1][0] = P[1][0] * A[0][0] + P[1][1] * A[0][1] + Q[1][0];
    Now_covariance[1][1] = P[1][0] * A[1][0] + P[1][1] * A[1][1] + Q[1][1];
    //??(4)    ?????
    K[0] = H[0] * Now_covariance[0][0] + H[1] * Now_covariance[1][0];
    K[1] = H[0] * Now_covariance[0][1] + H[1] * Now_covariance[1][1];
    Kalman_Gain[0] = (Now_covariance[0][0] * H[0] + Now_covariance[0][1] * H[1]) / (H[0] * K[0] + H[1] * K[1] + R);
    Kalman_Gain[1] = (Now_covariance[1][0] * H[0] + Now_covariance[1][1] * H[1]) / (H[0] * K[0] + H[1] * K[1] + R);
    //??(3)   ??????              X(K|K)
    Now_best_result[0] = Now_measure_result[0] + Kalman_Gain[0] * (Measure - (H[0] * Now_measure_result[0] + H[1] * Now_measure_result[1]));
    Now_best_result[1] = Now_measure_result[1] + Kalman_Gain[1] * (Measure - (H[0] * Now_measure_result[0] + H[1] * Now_measure_result[1]));
    //??(5)   ??????????      P(K|K)
    D[0][0] = I[0][0] - Kalman_Gain[0] * H[0];
    D[0][1] = I[0][1] - Kalman_Gain[0] * H[1];
    D[1][0] = I[1][0] - Kalman_Gain[1] * H[0];
    D[1][1] = I[1][1] - Kalman_Gain[1] * H[1];
    S[0][0] = D[0][0] * Now_covariance[0][0] + D[0][1] * Now_covariance[1][0];
    S[0][1] = D[0][0] * Now_covariance[0][1] + D[0][1] * Now_covariance[1][1];
    S[1][0] = D[1][0] * Now_covariance[0][0] + D[1][1] * Now_covariance[1][0];
    S[1][1] = D[1][0] * Now_covariance[0][1] + D[1][1] * Now_covariance[1][1];
    last_covariance[0][0] = S[0][0] * D[0][0] + S[0][1] * D[0][1] + Kalman_Gain[0] * Kalman_Gain[0] * R;
    last_covariance[0][1] = S[0][0] * D[1][0] + S[0][1] * D[1][1] + Kalman_Gain[0] * Kalman_Gain[1] * R;
    last_covariance[1][0] = S[1][0] * D[0][0] + S[1][1] * D[0][1] + Kalman_Gain[1] * Kalman_Gain[0] * R;
    last_covariance[1][1] = S[1][0] * D[1][0] + S[1][1] * D[1][1] + Kalman_Gain[1] * Kalman_Gain[1] * R;
    Real_angle = Now_best_result[0]; //??????
    return Real_angle;
}


























//////////////////
//double Finaldata_Angle=0;//�����˲��Ƕ�ֵ
//void Kaman_Init(void)
//{
//   	//������ʼֵ�ĸ�ֵ
//	Finaldata_Gyroscope  = Gyroscope_Zero;// =temp;
//	
//	Finaldata_Accelerometer  =Accelerometer_Zero; //=temp;

//  Finaldata_Gyroscope_X = Gyroscope_Zero_X;
//}

/***********************************************
* �ṹ��: Kaman
* ��  ��: �ı����
* ���ú���: ��
* ���ò���: 
* ����ֵ: 
* ʹ�÷���:�������˲��������˲����ֵ
* ��дʱ��:2014-10-25
* �޸�ʱ��:2014-10-25
*******************************************/
TYPEKAMN Kaman_Accelerometer(TYPEKAMN signal)//���ٶȼƵĿ�����
{
    static float nowdata_p=20;
	  int16_t nowdata;
	  float kg =0.1;
	  /*
	      ����������������Ż�������һ������
        Q:��������,Q����,��̬��Ӧ��죬�����ȶ��Ա仵
        R:��������,R���󣬶�̬��Ӧ�����������ȶ��Ա��
	      
	      �޸�1.0
	      ���������ź�����Ľ��
	  */
	  /*Ԥ��*/
	  nowdata = Finaldata_Accelerometer;
	  /*Э����*/
	  nowdata_p = nowdata_p+Accelerometer_Q;
	  /*����������*/
	  kg =nowdata_p/(nowdata_p+Accelerometer_R);
	  /*���Ž�*/
    Finaldata_Accelerometer = nowdata+kg*(signal - nowdata);
		/*����Э����*/
		nowdata_p=(1-kg)*nowdata_p;		
		return Finaldata_Accelerometer;
}
TYPEKAMN Kaman_Gyroscope(TYPEKAMN signal)
{
    static float nowdata_p=20;
	  int16_t nowdata;
	  
	  float kg =0.1;
	  /*
	      ����������������Ż�������һ������
        Q:��������,Q����,��̬��Ӧ��죬�����ȶ��Ա仵
        R:��������,R���󣬶�̬��Ӧ�����������ȶ��Ա��
	      
	      �޸�1.0
	      ���������ź�����Ľ��
	  */
	  /*Ԥ��*/
	  nowdata = Finaldata_Gyroscope;
	  /*Э����*/
	  nowdata_p = nowdata_p+Gyroscope_Q;
	  /*����������*/
	  kg =nowdata_p/(nowdata_p+Gyroscope_R);
	  /*���Ž�*/
    Finaldata_Gyroscope = nowdata+kg*(signal - nowdata);
		/*����Э����*/
		nowdata_p=(1-kg)*nowdata_p;
		return Finaldata_Gyroscope;
}

TYPEKAMN Kaman_Gyroscope_Z(TYPEKAMN signal)
{
    static float nowdata_p=20;
	  int16_t nowdata;
	  
	  float kg =0.1;
	  /*
	      ����������������Ż�������һ������
        Q:��������,Q����,��̬��Ӧ��죬�����ȶ��Ա仵
        R:��������,R���󣬶�̬��Ӧ�����������ȶ��Ա��
	      
	      �޸�1.0
	      ���������ź�����Ľ��
	  */
	  /*Ԥ��*/
	  nowdata = Finaldata_Gyroscope_Z;
	  /*Э����*/
	  nowdata_p = nowdata_p+Gyroscope_Q_Z;
	  /*����������*/
	  kg =nowdata_p/(nowdata_p+Gyroscope_R_Z);
	  /*���Ž�*/
    Finaldata_Gyroscope_Z = nowdata+kg*(signal - nowdata);
		/*����Э����*/
		nowdata_p=(1-kg)*nowdata_p;
		return Finaldata_Gyroscope_Z;
}

/***********************************************
* �ṹ��: Kaman
* ��  ��: �ı����
* ���ú���: ��
* ���ò���: 
* ����ֵ: 
* ʹ�÷���:�������˲��������˲����ֵ
* ��дʱ��:2014-10-25
* �޸�ʱ��:2014-10-25
*******************************************/
int Kaman_Diroct_Out(int data)
{
    static float nowdata_p=20;
	  int nowdata;
	  float kg =0.1;
	  /*
	      ����������������Ż�������һ������
        Q:��������,Q����,��̬��Ӧ��죬�����ȶ��Ա仵
        R:��������,R���󣬶�̬��Ӧ�����������ȶ��Ա��
	      
	      �޸�1.0
	      ���������ź�����Ľ��
	  */
	  /*Ԥ��*/
	  nowdata = Finaldata_Diroct;
	  /*Э����*/
	  nowdata_p = nowdata_p+Diroct_Q;
	  /*����������*/
	  kg =nowdata_p/(nowdata_p+Diroct_R);
	  /*���Ž�*/
    Finaldata_Diroct = nowdata+kg*(data - nowdata);
		/*����Э����*/
		nowdata_p=(1-kg)*nowdata_p;
		
		return Finaldata_Diroct;
}

//���׿������˲������뺯��
float dt=0.001;//ע�⣺dt��ȡֵΪkalman�˲�������ʱ��
float angle, angle_dot;//�ǶȺͽ��ٶ�

float P[2][2] = {{ 1, 0 },

                 { 0, 1 }};

float Pdot[4] ={ 0,0,0,0};

float Q_angle=0.001, Q_gyro=0.005; //�Ƕ��������Ŷ�,���ٶ��������Ŷ�

float R_angle=0.5 ,C_0 = 1;

float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

//�������˲�

TYPEKAMN Kalman_Filter(TYPEKAMN angle_m, TYPEKAMN gyro_m)//angleAx �� gyroGy
{
				dt=0.0001;
        angle+=(gyro_m-q_bias) * dt;

        angle_err = angle_m - angle;

        Pdot[0]=Q_angle - P[0][1] - P[1][0];

        Pdot[1]= - P[1][1];

        Pdot[2]= - P[1][1];

        Pdot[3]=Q_gyro;

        P[0][0] += Pdot[0] * dt;

        P[0][1] += Pdot[1] * dt;

        P[1][0] += Pdot[2] * dt;

        P[1][1] += Pdot[3] * dt;

        PCt_0 = C_0 * P[0][0];

        PCt_1 = C_0 * P[1][0];

        E = R_angle + C_0 * PCt_0;

        K_0 = PCt_0 / E;

        K_1 = PCt_1 / E;

        t_0 = PCt_0;

        t_1 = C_0 * P[0][1];

        P[0][0] -= K_0 * t_0;

        P[0][1] -= K_0 * t_1;

        P[1][0] -= K_1 * t_0;

        P[1][1] -= K_1 * t_1;

        angle += K_0 * angle_err; //���ŽǶ�

        q_bias += K_1 * angle_err;

        angle_dot = gyro_m-q_bias;//���Ž��ٶ�

        return angle;

}