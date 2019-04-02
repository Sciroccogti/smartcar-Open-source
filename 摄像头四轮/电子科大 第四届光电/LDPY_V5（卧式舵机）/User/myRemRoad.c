
#include "myRemRoad.h"

RemSpeedUpDown_Info RemSpeedUpDown_info = {0};

/*************************************************************************
*					��		��		ƫ		��   
*  ��������: myRemRoad_DataInit
*  ����˵��: �����㷨���ݳ�ʼ������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-07-20
*  ��	ע:
*************************************************************************/
void myRemRoad_DataInit(RemSpeedUpDown_Info *RemSpeedUpDown_info)
{	
	uint8_t ii = 0;
	
	/*---------------------�µ�-----------------------*/
	RemSpeedUpDown_info->Ramp_Counter = 0;							//��¼�µ�����
	RemSpeedUpDown_info->RampSpeedUp[0][0] = 0;						//��¼ʶ�𵽵�һ���µ�ʱ��·��
	RemSpeedUpDown_info->RampSpeedUp[0][1] = 0;						//�µ��󣬸�����ʻǰ����������
	RemSpeedUpDown_info->RampSpeedUp[0][2] = 0;						//��¼��һ���µ���ĸ��پ���
	RemSpeedUpDown_info->RampSpeedUp[1][0] = 0;						//��¼ʶ�𵽵ڶ����µ�ʱ��·��
	RemSpeedUpDown_info->RampSpeedUp[1][1] = 0;						//�µ��󣬸�����ʻǰ����������
	RemSpeedUpDown_info->RampSpeedUp[1][2] = 0;						//��¼�ڶ����µ���ļ��پ���
	
	//�µ���ų�ʼ��
	RemSpeedUpDown_info->Ramp_Num[0] = 1;
	RemSpeedUpDown_info->Ramp_Num[1] = 1;
	
	//�µ������ʼ��
	RemSpeedUpDown_info->RampRemDistance[0] = 0;
	RemSpeedUpDown_info->RampRemDistance[1] = 0;
	RemSpeedUpDown_info->RampRemDistance[2] = 0;
	RemSpeedUpDown_info->RampRemDistance[3] = 0;

	/*---------------------ʮ�ֵ�--------------------*/
	RemSpeedUpDown_info->Cross_Counter = 0;							//��¼ʮ�ֵ�����
	for(ii = 0; ii < 16; ii++)
	{
		RemSpeedUpDown_info->CrossSpeedUp[ii][0] = 0;
		RemSpeedUpDown_info->CrossSpeedUp[ii][1] = 0;
		RemSpeedUpDown_info->CrossSpeedUp[ii][2] = 0;
	}
	
	//�趨ʮ��·�ں�ļ��ٶ�
	RemSpeedUpDown_info->CrossSpeedUp[1][1] = 0;//�ڶ���ʮ�ֺ����ǰ����
	RemSpeedUpDown_info->CrossSpeedUp[1][2] = 0;//�ڶ���ʮ�ֵ���
	RemSpeedUpDown_info->CrossSpeedUp[3][1] = 0;//���ĸ�ʮ�ֺ����ǰ����
	RemSpeedUpDown_info->CrossSpeedUp[3][2] = 0;//���ĸ�ʮ�ֵ���
	
	//ʮ�ֵ���ų�ʼ��
	RemSpeedUpDown_info->Cross_Num[0] = 1;
	RemSpeedUpDown_info->Cross_Num[1] = 1;
	
	//ʮ�ֵ������ʼ��
	RemSpeedUpDown_info->CrossRemDistance[0] = 250;
	RemSpeedUpDown_info->CrossRemDistance[1] = 400;
	RemSpeedUpDown_info->CrossRemDistance[2] = 150;
	RemSpeedUpDown_info->CrossRemDistance[3] = 400;
	
	/*---------------------���ֵ�--------------------*/
	RemSpeedUpDown_info->TurnRound_Counter = 0;						//��¼���ֵ�����
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][0] = 0;				//��¼ʶ�𵽵�һ�����ֵ�ʱ��·��
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][1] = 0;				//���ֵ���,����ǰ����������
	RemSpeedUpDown_info->TurnRoundSpeedUp[0][2] = 0;				//��¼��һ�����ֵ���ĸ��پ���
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][0] = 0;				//��¼ʶ�𵽵ڶ������ֵ�ʱ��·��
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][1] = 0;				//���ֵ���,����ǰ����������
	RemSpeedUpDown_info->TurnRoundSpeedUp[1][2] = 0;				//��¼�ڶ������ֵ���ĸ��پ���
	
	//���ֵ���ų�ʼ��
	RemSpeedUpDown_info->TurnRound_Num[0] = 1;
	RemSpeedUpDown_info->TurnRound_Num[1] = 1;
	
	//���ֵ������ʼ��
	RemSpeedUpDown_info->TurnRoundRemDistance[0] = 300;
	RemSpeedUpDown_info->TurnRoundRemDistance[1] = 400;
	RemSpeedUpDown_info->TurnRoundRemDistance[2] = 300;
	RemSpeedUpDown_info->TurnRoundRemDistance[3] = 400;
	
	RemSpeedUpDown_info->RemRoad_Enable = TRUE;						//Ĭ�Ͽ��������㷨
}	

/*************************************************************************
*					��		��		ƫ		��   
*  ��������: myRemRoad_DataSetting
*  ����˵��: �����㷨���ݸ�ֵ����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-08-18
*  ��	ע:
*************************************************************************/
void myRemRoad_DataSetting(RemSpeedUpDown_Info *RemSpeedUpDown_info)
{	
	/*---------------------�µ�-----------------------*/
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[0] - 1][1] = RemSpeedUpDown_info->RampRemDistance[0];			
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[0] - 1][2] = RemSpeedUpDown_info->RampRemDistance[1];				
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[1] - 1][1] = RemSpeedUpDown_info->RampRemDistance[2];						
	RemSpeedUpDown_info->RampSpeedUp[RemSpeedUpDown_info->Ramp_Num[1] - 1][2] = RemSpeedUpDown_info->RampRemDistance[3];						

	/*---------------------ʮ�ֵ�--------------------*/
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[0] - 1][1] = RemSpeedUpDown_info->CrossRemDistance[0];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[0] - 1][2] = RemSpeedUpDown_info->CrossRemDistance[1];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[1] - 1][1] = RemSpeedUpDown_info->CrossRemDistance[2];
	RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Num[1] - 1][2] = RemSpeedUpDown_info->CrossRemDistance[3];

	/*---------------------���ֵ�--------------------*/			
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[0] - 1][1] = RemSpeedUpDown_info->TurnRoundRemDistance[0];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[0] - 1][2] = RemSpeedUpDown_info->TurnRoundRemDistance[1];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[1] - 1][1] = RemSpeedUpDown_info->TurnRoundRemDistance[2];				
	RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Num[1] - 1][2] = RemSpeedUpDown_info->TurnRoundRemDistance[3];				
}	

/*************************************************************************
*					��		��		ƫ		��   
*  ��������: myRemRoad_ControlSpeed
*  ����˵��: �����㷨���ƺ���
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-07-10
*  ��	ע:
*************************************************************************/
uint8_t myRemRoad_ControlSpeed(RemSpeedUpDown_Info *RemSpeedUpDown_info,Parameter_Info *Parameter_info)
{
	//ʮ�ֵ�����
	if((RemSpeedUpDown_info->Cross_Counter > 0)
		&& (RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][2] > 0))//ʮ�ֵ�������Ϊ��
	{
		if(RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0]
			+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][1]> Parameter_info->RoadDistance)
		{
			//ʮ�ֵ�����Լ200cm�Ĺ��ɶ�
			return FALSE;
		}
		else if((RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0] 
				+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][1]
				+ RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][2]) > Parameter_info->RoadDistance)
		{
			//�˴���������Ϊʮ�ֵ�����ĸ���ģʽ
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}
	
	//���ֵ�����
	if((RemSpeedUpDown_info->TurnRound_Counter >= 1)
		&& (RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][2] > 0))//ʮ�ֵ�������Ϊ��
	{
		if(RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0] 
			+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][1] > Parameter_info->RoadDistance)
		{
			return FALSE;
		}
		else if((RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0]
				+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][1]
				+ RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][2]) > Parameter_info->RoadDistance)
		{
			//���ֵ�����ٶ�����
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}

	//��������ȫ�����������򷵻�FALSE
	return FALSE;
}

/*-----------------------------------end of myRemRoad.c----------------------------------*/
