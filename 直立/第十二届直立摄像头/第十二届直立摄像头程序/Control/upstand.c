/*!
 *		湖北工程学院直立摄像头
 * @file       upstand.c
 * @brief      平衡部分程序
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 *
 *		互补滤波和非矩阵卡尔曼滤波都很好用
 *		矩阵卡尔曼尝试过不会用
 *		不必使用加权或平均平滑滤波容易造成滞后
 */

#include  "common.h"
#include  "include.h"
#include  "math.h"



/**********全局变量定义********/
//加速度计
int32 g_nValueOfAcce;	
float g_fAngleOfAcce;
int16 Offset_Acce = 680;//加速度计偏移值，700减小前倾。
float Ratio_Acce = 0.105;
//陀螺仪
int32 g_nValueOfGyro[2];
float g_fAngleSpeed[2];
float Offset_Gyro[2] = {0,0};
float Raito_Gyro[2] = {0.24, 0.24};
float angle_Difference;
//互补滤波
float g_fAngleOfGyro[2];
float Const_Time = 3;
uint8 Freq_Inte = 200;
float g_BlanceControlOut;

float g_upstandControl_P = 7.5;		//直立P
float g_upstandControl_D = 0.3;		//直立D
//非矩阵卡尔曼滤波，这些参数不用改
float Peried = 1/150.0;				//卡尔曼积分周期
float KalmanGain = 1.0;//卡尔曼增益
float Q = 2.0;//过程噪声2.0		越小积分越慢，跟踪加速度计越慢越平滑
float R = 5000.0;//测量噪声5000.0	越小跟踪加速度计越快
float g_AngleOfCar;					//输出角度值

/**
 * @note		初始化I2C、加速度计和陀螺仪
 * @author		湖北工程学院
 * @date		2017
 */
void UpstandInit(void)
{
 	gpio_init(PTC10,GPO,1);		//初始化SCL管脚
 	gpio_init(PTC11,GPO,1);		//初始化SDA管脚
 	MPU3050_Init();
 	MMA8451_Init();
	OffsetGyroY();				//设置陀螺仪偏移值，开机时不能碰车

}

/**
 * @file		读取加速度计的值并计算成角度
 * @file		读取陀螺仪的值并转化为角速度
 * @note      	得到全局变量g_nValueOfAcce和g_fAngleOfAcce
 * @note      	得到全局变量g_nValueOfGyro[2], g_fAngleSpeed
 * @author		湖北工程学院
 * @date		2017
 */
void GalAcceAndGyroY(void)
{
  	int16 nValueOfAcce = 0;
	int16 nValueOfGyro = 0;
	int8 i = 0;
	for(i=0;i<5;i++)	//连读五次加速度计陀螺仪平均
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
 * @file		互补滤波
 * @file		陀螺仪积分得到角度
 * @note      	得到全局变量g_fAngleOfGyro[2]
 * @author		湖北工程学院（参考清华方案）
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
 * @file		非矩阵卡尔曼滤波
 * @note      	输入角速度g_fAngleSpeed[0]，角度g_fAngleOfAcce
 * @note      	得到全局变量g_AngleOfCar
 * @date		  
 */
void KalmanFilter(void)
{
	//卡尔曼滤波局部参量
    static float Priori_Estimation = 0;//先验估计
    static float Posterior_Estimation = 0;//后验估计
    static float Priori_Convariance = 0;//先验方差
    static float Posterior_Convariance = 0;//后验方差
		
	//卡尔曼滤波
    //1.时间更新(预测) : X(k|k-1) = A(k,k-1)*X(k-1|k-1) + B(k)*u(k) 
    Priori_Estimation = Posterior_Estimation + g_fAngleSpeed[0]*Peried;		//先验估计，积分获得角度
    //2.更新先验协方差  : P(k|k-1) = A(k,k-1)*P(k-1|k-1)*A(k,k-1)'+Q(k) 
    Priori_Convariance = (float)sqrt( Posterior_Convariance * Posterior_Convariance + Q * Q );
	
    //  卡尔曼后验估计：测量更新  
    // 1.计算卡尔曼增益  : K(k) = P(k|k-1)*H(k)' / (H(k)*P(k|k-1)*H(k)' + R(k)) /
    KalmanGain = (float)sqrt( Priori_Convariance * Priori_Convariance / ( Priori_Convariance * Priori_Convariance + R * R ) );
    //2.测量更新(校正): X(k|k) = X(k|k-1)+K(k)*(Z(k)-H(k)*X(k|k-1)) 
    Posterior_Estimation  = Priori_Estimation + KalmanGain * (g_fAngleOfAcce - Priori_Estimation );
    // 3.更新后验协方差  : P(k|k) =（I-K(k)*H(k)）*P(k|k-1) 
    Posterior_Convariance = (float)sqrt(( 1 - KalmanGain ) * Priori_Convariance * Priori_Convariance );
    //得到最终角度 
    g_AngleOfCar = Posterior_Estimation;
  
}

/**
 * @file		平衡控制的PD计算
 * @note      	得到全局变量g_BlanceControlOut
 * @author		湖北工程学院
 * @date		2017
 */
void BalanceControl(void)
{
	static float g_upstandControl_PWMTemp[5];
	float fPWM;
	
//	if(g_AngleOfCar<-55||g_AngleOfCar>0)Flag_Stop = 1;//车倒保护
	
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
 * @file		直立陀螺仪的开机零点初始化
 * @note      	采集1000次平均
 * @author		湖北工程学院
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






