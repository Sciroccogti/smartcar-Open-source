#include "PID.h"

PID MOTOR_PID, Angle_PID, Ang_Vel_PID, Direct_PID, Turn_PID, Distance_PID;	//�������͵����PID�����ṹ��
float MOTOR[4]   = {50, 0, 5, 1000};		// �ٶȻ�PID	���һ��Ϊ�����޷�
float Angle[4]   = {0.3, 0, 0.1, 500};		// �ǶȻ�PID
float Ang_Vel[4] = {0.4, 0.03, 0, 1000};		// ���ٶȻ�PID
float Direct[4]  = {0.017, 0.001, 0.023, 70};	// ת��PID λ��	0.017	0.02
float Turn[5][4] = {{100, 7, 7, 400},	// 130	// ת���⻷��̬PID	���߷�
					{100, 8, 8, 400},	// 140
					{100, 8, 8, 400},	// 150
					{100, 9, 9, 400},	// 160
					{100, 10, 10, 400}};	// 170

/*
float Turn[5][4] = {{200, 13, 5, 400},	// 130	// ת���⻷��̬PID	�����
					{200, 14, 8, 400},	// 140
					{200, 15.5, 8, 400},	// 150
					{200, 16.5, 8, 400},	// 160
					{200, 18, 8, 400}};// 170
*/


// PID������ʼ��
void PID_Parameter_Init(PID *sptr)
{
	sptr->SumError  = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastData  = 0;
}


// λ��ʽ��̬PID����
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		  Actual;	//���ó���ʵ�����ֵ
	float Kp;		//��̬P
	
	iError = SetPoint - NowPiont;	//���㵱ǰ���
	sprt->SumError += iError*0.01;
	if (sprt->SumError >= PID[KT])
	{
		sprt->SumError = PID[KT];
	}
	else if (sprt->SumError <= PID[KT])
	{
		sprt->SumError = -PID[KT];
	}
	Kp = 1.0 * (iError*iError) / PID[KP] + PID[KI];	//Pֵ���ֵ�ɶ��κ�����ϵ���˴�P��I����PID���������Ƕ�̬PID������Ҫע�⣡����
	
	Actual = Kp * iError
		   + PID[KD] * ((0.8*iError + 0.2*sprt->LastError) - sprt->LastError);//ֻ��PD
	sprt->LastError = iError;		//�����ϴ����

//	Actual += sprt->SumError*0.1;
	Actual = range_protect(Actual, -260, 260);

	return Actual;
}

// λ��ʽPID����
int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	// ��ǰ���
		 Realize;	// ���ó���ʵ������

	iError = Point - NowData;	// ���㵱ǰ���
	sptr->SumError += PID[KI] * iError;	// ������
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
	sptr->PrevError = sptr->LastError;	// ����ǰ�����
	sptr->LastError = iError;		  	// �����ϴ����
	sptr->LastData  = NowData;			// �����ϴ�����

	return Realize;	// ����ʵ��ֵ
}

// ����ʽPID�������
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point)
{
	//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		Increase;	//���ó���ʵ������

	iError = Point - NowData;	// ���㵱ǰ���

	Increase =  PID[KP] * (iError - sptr->LastError)
			  + PID[KI] * iError
			  + PID[KD] * (iError - 2 * sptr->LastError + sptr->PrevError);
	
	sptr->PrevError = sptr->LastError;	// ����ǰ�����
	sptr->LastError = iError;		  	// �����ϴ����
	sptr->LastData  = NowData;			// �����ϴ�����
	
	return Increase;	// ��������
}
