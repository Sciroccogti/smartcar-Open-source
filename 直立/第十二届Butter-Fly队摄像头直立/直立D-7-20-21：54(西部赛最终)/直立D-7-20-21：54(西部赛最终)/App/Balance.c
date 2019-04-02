#include "Balance.h"

S_FLOAT_XYZ 
	GYRO_Real,		// 陀螺仪转化后的数据
	ACC_Real,		// 加速度计转化后的数据
	Attitude_Angle,	// 当前角度
	Last_Angle,		// 上次角度
	Target_Angle;	// 目标角度
	

S_INT16_XYZ
	GYRO,			// 陀螺仪原始数据
	GYRO_Offset,	// 陀螺仪温飘
	GYRO_Last,		// 陀螺仪上次数据
	ACC, 			// 加速度计数据
	ACC_Offset,		// 加速度计温飘
	ACC_Last;		// 加速度计上次数据
S_INT32_XYZ
	Tar_Ang_Vel,	// 目标角速度
	Tar_Ang_Vel_Last;	// 上次目标角速度

int32 
	Speed_Now = 0,	// 当前实际速度
	Speed_Min = 0,	// 左右最小速度
	Speed_Set = 0, 	// 目标设定速度
	Theory_Duty = 0,// 理论直立占空比
	Vel_Set = 0,	// 目标转向角速度
	Direct_Parameter = 0,// 转向系数
	Direct_Last = 0,
	Radius = 0;		// 目标转向半径倒数

uchar Point = 80;
int32 Difference = 0;
	  
	  			/* 各种标志位，放定时器中进行时序控制 */
char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;
/********************************************************/

/********************* 串级平衡控制 *********************/
// 频率控制在定时器中设置
void Balance_Control(void)
{
	if (Ang_Velocity_Flag)	// 直立角速度环	2ms
	{
		Ang_Velocity_Flag = 0;
		
		MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
		Data_Filter();					// 对原始数据滑动滤波												
												/* 角速度环作为最内环控制直立 */
		Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// 计算直立PWM
		Theory_Duty = range_protect(Theory_Duty, -950, 950);
			
		if (System_OK)
		{						/* 角速度环作为最内环控制转向 */									//Speed_Min
			Direct_Parameter = PID_Realize(&Direct_PID, Direct, (int32)(GYRO_Real.Z*100), Radius*Speed_Min);	// 转向环左正右负
			Direct_Parameter = range_protect(Direct_Parameter, -1200, 1200);
		}
		
		Direct_Last = Direct_Last*0.3 + Direct_Parameter*0.7;	// 更新上次角速度环结果
		
		MOTOR_Duty_Left  = Theory_Duty - Direct_Last;	// 左右电机根据转向系数调整差速
		MOTOR_Duty_Right = Theory_Duty + Direct_Last;	

		if (Run_Flag)
		{
			MOTOR_Control(MOTOR_Duty_Left, MOTOR_Duty_Right);	// 控制左右电机
		}
		else
		{
			if (Stop_Flag)
			{
				if (Speed_Now > 20)
				{
					MOTOR_Control(-350, -350);
				}
				else
				{
					MOTOR_Control(0, 0);
				}
			}
			else
			{
				MOTOR_Control(0, 0);
			}
		}
		
		mpu_dmp_get_data(&Attitude_Angle.Y, &Attitude_Angle.X, &Attitude_Angle.Z);	// 使用DMP直接读取欧拉角
	}
	
	if (Angle_Flag)		// 直立角度环	10ms
	{
		Angle_Flag = 0;
		
		Speed_Measure();// 获取当前速度
											/* 角度环加到角速度环上串级控制 */
		Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y);	// 结果为放大10倍的目标角速度	
		Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	// 注意正负号
	}
	if (Speed_Flag)		// 速度环	100ms
	{
		Speed_Flag = 0;
												/* 速度环加到角度环上串级控制 */
		Target_Angle.Y = -PID_Realize(&MOTOR_PID, MOTOR, Speed_Now, Speed_Set);	// 结果为放大100倍的目标角度
		Target_Angle.Y += Zero_Angle*100;	// 目标角度叠加在零点上
		Target_Angle.Y = range_protect((int32)Target_Angle.Y, -1700, 800);	// -44 22

		Speed_Min = Speed_Min * 0.1 + Speed_Now * 0.9;
		if (Speed_Min < 40)
		{
			Speed_Min = 40;
		}
	}
}

/* 初始化用到的一些变量 */
void Balance_Init(void)
{
	Attitude_Angle.Y = 0;
	Target_Angle.Y = 0;
	Tar_Ang_Vel.Y = 0;
	Tar_Ang_Vel.Z = 0;
}

