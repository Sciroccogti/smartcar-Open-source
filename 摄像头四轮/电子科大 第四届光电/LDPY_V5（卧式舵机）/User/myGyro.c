
#include "myGyro.h"

Gyro_Info Gyro_info = {0};

extern CCD_Info CCD2_info;
extern CCD_Info CCD3_info;

extern Speed_Info Speed_info;
extern Parameter_Info Parameter_info;

extern RemSpeedUpDown_Info RemSpeedUpDown_info;

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myGyro_DataInit
*  ����˵��:������/���ٶȼ����ݳ�ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-06-26
*  ��    ע��
*************************************************************************/
void myGyro_DataInit(Gyro_Info *Gyro_info)
{
	uint8_t ii = 0;
	
	Gyro_info->GyroscopeZero = 2208;				//����������ѹ
	
	for(ii = 0; ii < GYRO_LENGTH; ii++)
	{
		Gyro_info->Gyroscope_Fifo[ii] = 2208;
	}
	
	Gyro_info->Gyro_Sum = 0;
	Gyro_info->RampThresholdValue = 250;
	
	Gyro_info->RampUpDown_Num = 0;
	
}

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myGyro_GetRamp
*  ����˵��:�ɼ�������/���ٶȼ�
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-07-11
*  ��    ע��
*************************************************************************/	
Road_Condition myGyro_GetRamp(CCD_Info *CCD1_info,Gyro_Info *Gyro_info,Speed_Info *Speed_info,Road_Condition *Road_condition)
{
	uint8_t ii = 0;

	Gyro_info->Gyro_Sum = 0;
	
	//�ɼ������Ƕ˿ڵ�AD��ѹֵ������CCD�ɼ��ж�����
	//���Һ����ʾ̫�ණ���Ļ��������ڴ˴��ɼ�������ADֵ��������CCD�ɼ��ж��г�ͻ
	
	//�ƶ�������ADֵ����
	for(ii = GYRO_LENGTH - 1; ii > 0; ii--)
	{
		Gyro_info->Gyroscope_Fifo[ii] = Gyro_info->Gyroscope_Fifo[ii - 1];
	}
	
	//�����µ�5��ADֵ֮��
	for(ii = 0; ii < 5; ii++)
	{
		Gyro_info->Gyro_Sum += Gyro_info->Gyroscope_Fifo[ii];
	}
	
	Gyro_info->Gyro_Sum = Gyro_info->Gyro_Sum / 5;
	
	//�������
	if((Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition)
		&& (0 == CCD1_info->LeftLoseLine_Flag) && (0 == CCD1_info->RightLoseLine_Flag)
		&& ((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) < -Gyro_info->RampThresholdValue))
	{
		if((Gyro_info->RampUpDown_Num < 20) && (1 == Speed_info->DistanceOf1Cm_Flag))//������⵽����
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if((Gyro_info->RampUpDown_Num >= 15) 
			|| ((1 == Parameter_info.RampReady_FLag) && (Gyro_info->RampUpDown_Num >= 5)))//�����µ�Ԥ�е�ʱ�������������
		{
			Gyro_info->RampUpDown_Num = 0;//�����µ��жϴ���
			Parameter_info.RampReady_FLag = 0;//�����µ�Ԥ�б��
			
			Parameter_info.GetRamp_Enable = FALSE;//�Ѿ������µ�����һ�������ڲ��ټ���µ�
			Parameter_info.GetTurnRound_Enable = FALSE;//�µ���������ֵ�
			Parameter_info.GetStartEndLine_Enable = FALSE;//�µ������������

			//�����ڴ˴���¼�����㷨�µ������ģʽ�ĳ�ʼ����
			if(RemSpeedUpDown_info.Ramp_Counter < 2)
			{
				RemSpeedUpDown_info.Ramp_Counter++;
			}
			
			RemSpeedUpDown_info.RampSpeedUp[RemSpeedUpDown_info.Ramp_Counter - 1][0] = Parameter_info.RoadDistance;//��¼��ʱ����·��
			
			
			return Ramp_Up;
		}
		else 
		{
			return NeedJudge;
		}
	}
	//�������
	else if((Ramp_Up == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info.Ramp_Counter - 1] - 20)
			&& (((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) > 200) || (TRUE == myCCD_JudgeRampDown(CCD1_info,&CCD2_info,&CCD3_info,Road_condition))))
	{
		if((Gyro_info->RampUpDown_Num < 6) && (1 == Speed_info->DistanceOf1Cm_Flag))//������⵽����
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if(Gyro_info->RampUpDown_Num >= 5)
		{
			Gyro_info->RampUpDown_Num = 0;//�����µ��жϼ���
		
			return Ramp_Down;
		}
		else //û�м�⵽������һֱ������
		{
			return Ramp_Up;
		}
	}
	//���µ�ʱ�����µ�
	else if((Ramp_Down == *Road_condition) 
			&& (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info.Ramp_Counter - 1] + 30)
			&& ((int16_t)(Gyro_info->Gyro_Sum - Gyro_info->GyroscopeZero) < -Gyro_info->RampThresholdValue))
	{
		if((Gyro_info->RampUpDown_Num < 6) && (1 == Speed_info->DistanceOf1Cm_Flag))//������⵽�µ�
		{
			Gyro_info->RampUpDown_Num++;
		}
		
		if(Gyro_info->RampUpDown_Num >= 5)
		{
			Gyro_info->RampUpDown_Num = 0;//�����µ��жϼ���
		
			Speed_info->DistanceOfRampUp_Counter = Parameter_info.RoadDistance;//��¼��ʱ�ܵ���ʻ����
			
			return NeedJudge;
  		}
		else //û�м�⵽�µ���һֱ������
		{
			return *Road_condition;
		}
	}
	else
	{
		Gyro_info->RampUpDown_Num = 0;//�����µ��жϼ���
		
		return *Road_condition;//�����������ԭ����·������
	}

}

/*-------------------------------end of myGyro.c-------------------------------*/
