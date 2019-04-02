#include "Balance.h"

S_FLOAT_XYZ 
	GYRO_Real,		// ������ת���������
	ACC_Real,		// ���ٶȼ�ת���������
	Attitude_Angle,	// ��ǰ�Ƕ�
	Last_Angle,		// �ϴνǶ�
	Target_Angle;	// Ŀ��Ƕ�
	

S_INT16_XYZ
	GYRO,			// ������ԭʼ����
	GYRO_Offset,	// ��������Ʈ
	GYRO_Last,		// �������ϴ�����
	ACC, 			// ���ٶȼ�����
	ACC_Offset,		// ���ٶȼ���Ʈ
	ACC_Last;		// ���ٶȼ��ϴ�����
S_INT32_XYZ
	Tar_Ang_Vel,	// Ŀ����ٶ�
	Tar_Ang_Vel_Last;	// �ϴ�Ŀ����ٶ�

int32 
	Speed_Now = 0,	// ��ǰʵ���ٶ�
	Speed_Min = 0,	// ������С�ٶ�
	Speed_Set = 0, 	// Ŀ���趨�ٶ�
	Theory_Duty = 0,// ����ֱ��ռ�ձ�
	Vel_Set = 0,	// Ŀ��ת����ٶ�
	Direct_Parameter = 0,// ת��ϵ��
	Direct_Last = 0,
	Radius = 0;		// Ŀ��ת��뾶����

uchar Point = 80;
int32 Difference = 0;
	  
	  			/* ���ֱ�־λ���Ŷ�ʱ���н���ʱ����� */
char Speed_Flag, Angle_Flag, Ang_Velocity_Flag, DMP_Flag;
/********************************************************/

/********************* ����ƽ����� *********************/
// Ƶ�ʿ����ڶ�ʱ��������
void Balance_Control(void)
{
	if (Ang_Velocity_Flag)	// ֱ�����ٶȻ�	2ms
	{
		Ang_Velocity_Flag = 0;
		
		MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
		Data_Filter();					// ��ԭʼ���ݻ����˲�												
												/* ���ٶȻ���Ϊ���ڻ�����ֱ�� */
		Theory_Duty += PID_Increase(&Ang_Vel_PID, Ang_Vel, (int32)(GYRO_Real.Y*10), (int32)(Tar_Ang_Vel.Y));	// ����ֱ��PWM
		Theory_Duty = range_protect(Theory_Duty, -950, 950);
			
		if (System_OK)
		{						/* ���ٶȻ���Ϊ���ڻ�����ת�� */									//Speed_Min
			Direct_Parameter = PID_Realize(&Direct_PID, Direct, (int32)(GYRO_Real.Z*100), Radius*Speed_Min);	// ת�������Ҹ�
			Direct_Parameter = range_protect(Direct_Parameter, -1200, 1200);
		}
		
		Direct_Last = Direct_Last*0.3 + Direct_Parameter*0.7;	// �����ϴν��ٶȻ����
		
		MOTOR_Duty_Left  = Theory_Duty - Direct_Last;	// ���ҵ������ת��ϵ����������
		MOTOR_Duty_Right = Theory_Duty + Direct_Last;	

		if (Run_Flag)
		{
			MOTOR_Control(MOTOR_Duty_Left, MOTOR_Duty_Right);	// �������ҵ��
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
		
		mpu_dmp_get_data(&Attitude_Angle.Y, &Attitude_Angle.X, &Attitude_Angle.Z);	// ʹ��DMPֱ�Ӷ�ȡŷ����
	}
	
	if (Angle_Flag)		// ֱ���ǶȻ�	10ms
	{
		Angle_Flag = 0;
		
		Speed_Measure();// ��ȡ��ǰ�ٶ�
											/* �ǶȻ��ӵ����ٶȻ��ϴ������� */
		Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), (int32)Target_Angle.Y);	// ���Ϊ�Ŵ�10����Ŀ����ٶ�	
		Tar_Ang_Vel.Y = range_protect(Tar_Ang_Vel.Y, -1500, 1500);	// ע��������
	}
	if (Speed_Flag)		// �ٶȻ�	100ms
	{
		Speed_Flag = 0;
												/* �ٶȻ��ӵ��ǶȻ��ϴ������� */
		Target_Angle.Y = -PID_Realize(&MOTOR_PID, MOTOR, Speed_Now, Speed_Set);	// ���Ϊ�Ŵ�100����Ŀ��Ƕ�
		Target_Angle.Y += Zero_Angle*100;	// Ŀ��Ƕȵ����������
		Target_Angle.Y = range_protect((int32)Target_Angle.Y, -1700, 800);	// -44 22

		Speed_Min = Speed_Min * 0.1 + Speed_Now * 0.9;
		if (Speed_Min < 40)
		{
			Speed_Min = 40;
		}
	}
}

/* ��ʼ���õ���һЩ���� */
void Balance_Init(void)
{
	Attitude_Angle.Y = 0;
	Target_Angle.Y = 0;
	Tar_Ang_Vel.Y = 0;
	Tar_Ang_Vel.Z = 0;
}

