#include "PID.h"

PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;	//定义舵机和电机的PID参数结构体
float MOTOR[4]   = {50, 0, 5, 1000};		// 速度环PID	最后一项为积分限幅
float Angle[4]   = {0.3, 0, 0.1, 500};		// 角度环PID
float Ang_Vel[4] = {0.4, 0.03, 0, 1000};		// 角速度环PID
float Direct[4]  = {0.017, 0.001, 0.023, 70};	// 转向环PID 位置	0.017	0.02
float Turn[5][4] = {{100, 7, 7, 400},	// 130	// 转向外环动态PID	中线法
					{100, 8, 8, 400},	// 140
					{100, 8, 8, 400},	// 150
					{100, 9, 9, 400},	// 160
					{100, 10, 10, 400}};	// 170

/*
float Turn[5][4] = {{200, 13, 5, 400},	// 130	// 转向外环动态PID	面积法
					{200, 14, 8, 400},	// 140
					{200, 15.5, 8, 400},	// 150
					{200, 16.5, 8, 400},	// 160
					{200, 18, 8, 400}};// 170
*/


// PID参数初始化
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0;
}


// 位置式动态PID控制
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
	//定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		  Actual;	//最后得出的实际输出值
	float Kp;		//动态P
	
	iError = SetPoint - NowPiont;	//计算当前误差
	sprt->SumError += iError*0.01;
	if (sprt->SumError >= PID[KT])
	{
		sprt->SumError = PID[KT];
	}
	else if (sprt->SumError <= PID[KT])
	{
		sprt->SumError = -PID[KT];
	}
	Kp = 1.0 * (iError*iError) / PID[KP] + PID[KI];	//P值与差值成二次函数关系，此处P和I不是PID参数，而是动态PID参数，要注意！！！
	
	Actual = Kp * iError
		   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError);//只用PD
	sprt->LastError = iError;		//更新上次误差

//	Actual += sprt->SumError*0.1;
	Actual = range_protect(Actual, -260, 260);

	return Actual;
}

// 位置式PID控制
int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	// 当前误差
		 Realize;	// 最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差
	sptr->SumError += PID[KI] * iError;	// 误差积分
	if (sptr->SumError >= PID[KT])
	{
		sptr->SumError = PID[KT];
	}
	else if (sptr->SumError <= -PID[KT])
	{
		sptr->SumError = -PID[KT];
	}

	Realize = PID[KP] * iError
			+ sptr->SumError
			+ PID[KD] * (iError - sptr->LastError);
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据

	return Realize;	// 返回实际值
}

// 增量式PID电机控制
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
	int32 iError,	//当前误差
		Increase;	//最后得出的实际增量

	iError = Point - NowData;	// 计算当前误差

	Increase =  PID[KP] * (iError - sptr->LastError)
			  + PID[KI] * iError
			  + PID[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);
	
	sptr->PrevError = sptr->LastError;	// 更新前次误差
	sptr->LastError = iError;		  	// 更新上次误差
	sptr->LastData  = NowData;			// 更新上次数据
	
	return Increase;	// 返回增量
}
