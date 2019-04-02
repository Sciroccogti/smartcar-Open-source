

#include "myMotor.h"

  Speed_Info	Speed_info = {0};
  Motor_Info    Motor_info = {0};
  
  extern CCD_Info CCD3_info;
  extern HeadSteer_Info HeadSteer_info;
  
  extern Gyro_Info Gyro_info;
  
  extern Car_State Car_state;
  extern Car_State Car_state_Pre;
  
  extern Parameter_Info Parameter_info;
  extern Road_Type Road_type;
  
  extern Road_Condition Road_condition;
  
 #ifdef DMA_CH2_SendLabVIEW_Info_Enable

	extern LabVIEW_Info LabVIEW_info;
	
#endif

/*************************************************************************
*				��	��	ƫ	��  
*
*  �������ƣ�myMotor_DataInit
*  ����˵���������ز����ĳ�ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myMotor_DataInit(Motor_Info *Motor_info)
{
	Motor_info->KPT = 900;
	Motor_info->KP = 300.0;
	Motor_info->KI = 100;
	Motor_info->KD = 8.0;
	
	Motor_info->KP_Mult = 0;
	Motor_info->KI_Mult = 0;
	Motor_info->KD_Mult = 0;
	
	Motor_info->PWMOutputDuty = 0;		
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_SpeedDataInit
*  ����˵�����ٶ���ز����ĳ�ʼ��,��λΪ������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myMotor_SpeedDataInit(Speed_Info *Speed_info)
{
	uint8_t  ii = 0;
	
  	Speed_info->Straight_Speed = 74;				//ֱ���ٶ�
	Speed_info->Cur_Speed = 68;				 		//����ٶ�
	Speed_info->Snake_Speed = 72;					//СS�ٶ�
	Speed_info->TurnRound_Speed = 50;				//��ͷ�ٶ�
	Speed_info->Obstacle_Speed = 70;				//·���ٶ�
	Speed_info->RampUp_Speed = 30;					//���µ��ٶ�
	Speed_info->RampDown_Speed = 60;				//���µ��ٶ�
	Speed_info->Angle60_Speed = 50;					//60�ȼ���ٶ�
	
	Speed_info->Speed_Now = 0;
	Speed_info->TargetSpeed[0] = 0;
	Speed_info->TargetSpeed[1] = 0;
	Speed_info->SpeedError = 0;
	Speed_info->LastSpeedError = 0;
	Speed_info->SpeedRAcc = 0;
	
	Speed_info->Speed_Direction = 0;
	Speed_info->EncoderData = 0;
	
	for(ii = 0; ii < SPEED_FIFO_LENGTH; ii++)
	{
		Speed_info->SpeedFifo[ii] = 0;
	}
	
	Speed_info->SpeedMode = 1;						//Ĭ��Ϊ1��
	Speed_info->LastSpeedMode = 1;
	
	Speed_info->SpeedAdd_3 = 20;					//3��ֱ������ٶ�����ֵ
	Speed_info->SpeedMode3Add = 0;					//3���ٶ�����ֵ
	Speed_info->SpeedMode3Acc_Counter = 0;
	Speed_info->CurSpeedAcc_Flag = 0;				//��������ٱ������
	Speed_info->CurSpeedAcc = 0;					//���������ֵ
	
	Speed_info->Error_K = 18;
	Speed_info->Error_D_K = 18;
	Speed_info->HeadSteerPWMError_K = 750;
	Speed_info->HeadSteerPWMError_D_K = 300;

	Speed_info->DistanceTo1CmCount = 0;				//1cm�������
	Speed_info->DistanceOf1Cm_Flag = 0;				//1cm���˱�־
	Speed_info->DistanceOfCross_Counter = 0;
	Speed_info->DistanceOfTurnRound_Counter = 0;
	Speed_info->DiatanceToTurnRound = 0;			//ʶ�����ֵ�ʱ�����ǵľ���
	Speed_info->DistanceBeginTurn = 0;				
	Speed_info->DistanceToTurn_MIN = 0;
	Speed_info->DistanceToTurn_MAX = 20;
	Speed_info->DistanceOfRampUp[0] = 100;
	Speed_info->DistanceOfRampUp[1] = 100;
	Speed_info->DistanceOfRampDown = 60;
	Speed_info->DistanceOfRampUp_Counter = 0;
	Speed_info->DistanceOfObstacle_Counter = 0;	
	Speed_info->DistanceOfSpeedMode3_Counter = 0;	
	Speed_info->DistanceOfSpeedMode3_Max = 800;		//3�����پ����Ϊ8m
	Speed_info->DistanceOfRoadInvalid_Counter = 0;
	Speed_info->DistanceOfCCD1GetSL_Counter = 0;
	Speed_info->DistanceOf60Angle_Counter = 0;
	
	Speed_info->SpeedCalculate_Enable = TRUE;		//Ĭ�ϱջ�
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_UpdateEncoder
*  ����˵������������ص����ݸ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myMotor_UpdateEncoder(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							RemSpeedUpDown_Info *RemSpeedUpDown_info,
							Road_Condition *Road_condition,
							Car_State *Car_state,
							Car_State *Car_state_Pre
						  )
{
	uint8_t ii = 0;
	
	//����������
	Speed_info->SpeedFifo[0] = 0 - Speed_info->EncoderData;
	
	//�������޷�
	if(fabs(Speed_info->SpeedFifo[0]) > 165)//3ms�������ڶ�Ӧ��ʵ���ٶ�Ϊ 8m/s
	{
		Speed_info->SpeedFifo[0] = Speed_info->SpeedFifo[1];
	}
	
	//1cm�����ۼ�
	Speed_info->DistanceTo1CmCount += Speed_info->SpeedFifo[0];
	
	//------------------------�������----------------------------//
	if(Speed_info->DistanceTo1CmCount >= 68)//1cm����
	{
		Speed_info->DistanceTo1CmCount -= 68;
		Speed_info->DistanceOf1Cm_Flag = 1;		//���1cm����
		
		//��ʻ�����Լ�
		Parameter_info->RoadDistance++;
		
		if(Speed_info->DistanceTo1CmCount >= 68)//�����ٶȺܿ죬������ܼ��㲻׼�ˣ���Ӧ���ٴμ������
		{
			Speed_info->DistanceTo1CmCount -= 68;
			
			//��ʻ�����Լ�
			Parameter_info->RoadDistance++;
			
			//����ģʽ�����Լ�
			if(Parameter_info->Mode_HighSpeedDistance > 0)
			{
				Parameter_info->Mode_HighSpeedDistance--;
			}
			
			//3�����پ����¼
			if((3 == Speed_info->SpeedMode) && (Speed_info->DistanceOfSpeedMode3_Counter < Speed_info->DistanceOfSpeedMode3_Max))
			{
				Speed_info->DistanceOfSpeedMode3_Counter++;//3�����پ����Լ�
			}
			
			//CCD1Ԥ�е������߾����Լ�
			if(1 == Parameter_info->CCD1_GetedStartEndLine_Flag)
			{
				Speed_info->DistanceOfCCD1GetSL_Counter++;
			}
			
			//���ֵ�60�ȼ��Ԥ�гɹ�
			if(Speed_info->DistanceOf60Angle_Counter > 0)
			{
				Speed_info->DistanceOf60Angle_Counter--;
			}
		}
		
		//����ģʽ�����Լ�
		if(Parameter_info->Mode_HighSpeedDistance > 0)
		{
			Parameter_info->Mode_HighSpeedDistance--;
		}
		
		//������Ч�����Լ�
		if(1 == CCD1_info->RoadInvalid_Flag)
		{
			Speed_info->DistanceOfRoadInvalid_Counter++;
			
			if(Speed_info->DistanceOfRoadInvalid_Counter >= 5)//����5cm��ǿ����Ч
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//�����·��Ч����
			}
		}
		
		//CCD1Ԥ�е������߾����Լ�
		if(1 == Parameter_info->CCD1_GetedStartEndLine_Flag)
		{
			Speed_info->DistanceOfCCD1GetSL_Counter++;
			
			if(Speed_info->DistanceOfCCD1GetSL_Counter >= 50)//50cm��û�м�⵽�������������Ԥ��
			{
				Speed_info->DistanceOfCCD1GetSL_Counter = 0;
				Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
			}
		}
		
		//���ֵ�60�ȼ��Ԥ�гɹ�
		if(Speed_info->DistanceOf60Angle_Counter > 0)
		{
			Speed_info->DistanceOf60Angle_Counter--;
			
			if((Speed_info->DistanceOf60Angle_Counter <= 0) 
				|| (1 == CCD3_info.Cross_Flag)
				|| (1 == CCD2_info->Cross_Flag)
				|| (1 == CCD2_info->Cross_Flag))
			{
				Parameter_info->Left60Angle_Flag = 0;
				Parameter_info->Right60Angle_Flag = 0;
			}
			
			Bell_ON;
		}
		
		//3�����پ����¼
		if((3 == Speed_info->SpeedMode) && (Speed_info->DistanceOfSpeedMode3_Counter < Speed_info->DistanceOfSpeedMode3_Max))
		{
			Speed_info->DistanceOfSpeedMode3_Counter++;//3�����پ����Լ�
		}
		else if((2 == Speed_info->SpeedMode))//�ٶȼ�������ʱ��������3�����پ���
		{
			Speed_info->DistanceOfSpeedMode3_Counter = 0;
		}
		
		/*-------------------------------------------------------------------------------------------*/
		/*---------------------------------����·�εľ������-------------------------------------*/
		/*-------------------------------------------------------------------------------------------*/
		//ʮ�ֵ��������
		if((1 == CCD2_info->Cross_Flag) 
			&& (Speed_info->DistanceOfCross_Counter <= 0) 
			&& (TRUE == RemSpeedUpDown_info->RemRoad_Enable))//�����㷨�Ѿ�����)
		{
			Speed_info->DistanceOfCross_Counter++;
		}
		else if(Speed_info->DistanceOfCross_Counter > 0)
		{
			Speed_info->DistanceOfCross_Counter++;
			
			if(Speed_info->DistanceOfCross_Counter >= 60)//60cmһ��ʮ�ֵ�����
			{
				if(RemSpeedUpDown_info->Cross_Counter < 16)
				{
					RemSpeedUpDown_info->Cross_Counter++;
					RemSpeedUpDown_info->CrossSpeedUp[RemSpeedUpDown_info->Cross_Counter - 1][0] = Parameter_info->RoadDistance;//��¼��ʱ����������
					
					Speed_info->DistanceOfCross_Counter = 0;//����ʮ�ֵ�����
				}
			}
		}
		
		//------------------------·�Ͼ������----------------------------//
		if(Speed_info->DistanceOfObstacle_Counter > 0)//����·�϶�
		{
			Speed_info->DistanceOfObstacle_Counter--;//·�Ͼ����Լ�
			
			if(Speed_info->DistanceOfObstacle_Counter <= 0 )
			{
				if(Obstacle == *Road_condition)
				{
					*Road_condition = NeedJudge;//�ص�����״̬
				}
				
				CCD2_info->CCD_PhotoCenter = 63;
				Parameter_info->LeftOrRightObstacle_Flag = 0;
				Parameter_info->GetObstacle_Enable = TRUE;//·�Ͼ��������·�ϵ��ٴ�ʶ��
			}
		}
		
		//------------------------�µ��������-----------------------------//
		//����µ�Ԥ�гɹ�
		if((1 == Parameter_info->RampReady_FLag) && (Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition))
		{
			if(Parameter_info->RampReady_Distance < 120)//120cm��û���жϵ��µ�������ñ��
			{
				Parameter_info->RampReady_Distance++;
			}
			else
			{
				Parameter_info->RampReady_Distance = 0;
				Parameter_info->RampReady_FLag = 0;//�����µ�Ԥ�б��
			}
		}
		
		//�����µ���ʱ�ľ������
		if((Ramp_Up == *Road_condition) || (Ramp_Down == *Road_condition))
		{
			Speed_info->DistanceOfRampUp_Counter++;//�µ������Լ�
			
			if((Ramp_Up == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= Speed_info->DistanceOfRampUp[RemSpeedUpDown_info->Ramp_Counter - 1]))//�������¾�����Զ��ص�����״̬
			{
				*Road_condition = Ramp_Down;
				
				Gyro_info.RampUpDown_Num = 0;//�����µ��жϼ���
			}
			else if((Ramp_Down == *Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= (Speed_info->DistanceOfRampUp[RemSpeedUpDown_info->Ramp_Counter - 1] + Speed_info->DistanceOfRampDown)))
			{
				*Road_condition = NeedJudge;
				
				Speed_info->DistanceOfRampUp_Counter = Parameter_info->RoadDistance;//��¼��ʱ�ܵ���ʻ����
			}
		}
		else if((FALSE == Parameter_info->GetRamp_Enable) 
			&& ((int32_t)(Parameter_info->RoadDistance - Speed_info->DistanceOfRampUp_Counter) > 50))//50cm�Ժ�ʶ���µ��µ������ֵ�
		{
			if((All == Road_type) && (Turn_Round != *Car_state))//ֻ�е�����ȫ�����������ʱ��ſ������ֵ�ʶ�𣬷���ʶ�����ֵ�
			{
				Parameter_info->GetTurnRound_Enable = TRUE;
			}
			
			//1.5m�Ժ�ʶ���µ��µ�
			if((int32_t)(Parameter_info->RoadDistance - Speed_info->DistanceOfRampUp_Counter) > 150)
			{
				Parameter_info->RampReady_Distance = 0;		//�����µ�Ԥ�о������
				Parameter_info->RampReady_FLag = 0;			//�����µ�Ԥ�б��
				
				Speed_info->DistanceOfRampUp_Counter = 0;
				
				Parameter_info->GetStartEndLine_Enable = TRUE;//���Լ��������
				
				if((All == Road_type) || (Ramp_Only == Road_type))//��ֹ������ʱ���Զ������µ���ʶ��
				{
					Parameter_info->GetRamp_Enable = TRUE;
				}
			}
		}
		
		//-----------------------------------���ֵ��������------------------------------//
		if (Speed_info->DiatanceToTurnRound > 0)//�Ѿ���¼�˾������ֵ���ǵľ���
		{
			Speed_info->DiatanceToTurnRound--;//�����ֵ���ǵľ����Լ�
		}
		
		if((Turn_Round == *Car_state) || (Turn_Round == *Car_state_Pre))//�������ֵ�
		{
			Speed_info->DistanceOfTurnRound_Counter++;//���ֵ������Լ�
			
			#ifdef RemRoad_Control_Enable
				if(Speed_info->DistanceOfTurnRound_Counter >= 150)
				{
					//�����ڴ˴���¼�����㷨���ֵ������ģʽ�ĳ�ʼ����
					if(TRUE == RemSpeedUpDown_info->RemRoad_Enable)//�����㷨�Ѿ�����
					{
						if(RemSpeedUpDown_info->TurnRound_Counter < 2)
						{
							RemSpeedUpDown_info->TurnRound_Counter++;
						}
							
						RemSpeedUpDown_info->TurnRoundSpeedUp[RemSpeedUpDown_info->TurnRound_Counter - 1][0] = Parameter_info->RoadDistance;//��¼��ʱ����·��
					}
				}
			#endif
			
			if((0 == CCD1_info->LeftLoseLine_Flag) && (0 == CCD1_info->RightLoseLine_Flag)
				&& (0 == CCD2_info->LeftLoseLine_Flag) && (0 == CCD2_info->RightLoseLine_Flag)
				&& (Speed_info->DistanceOfTurnRound_Counter > 100))//CCD1�������߼���ʶ���µ�
			{
				Parameter_info->GetRamp_Enable = TRUE;		//���Լ���µ�
			}
			
			if(Speed_info->DistanceOfTurnRound_Counter > 200)//��ʶ�����ֵ���ʼ200cm������ٴμ�����ֵ�
			{
				*Car_state_Pre = Go_Forward;
				
				//�������ֵ����
				Speed_info->DiatanceToTurnRound = 0;
				Speed_info->DistanceOfTurnRound_Counter = 0;
				Speed_info->DistanceBeginTurn = 0;
				
				Parameter_info->GetTurnRound_Enable = TRUE;	//�����ٴμ�����ֵ�
				
				if((Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition))
				{
					Parameter_info->GetRamp_Enable = TRUE;		//�����µ�����Լ���µ�
				}
				
			}
			else if((Turn_Round == *Car_state) && (Speed_info->DistanceOfTurnRound_Counter > 100))//���ֵ����볬��100cm��δѰ������ǿ�ƻ���
			{
				*Car_state = Go_Forward;
				*Car_state_Pre = Turn_Round;
				
				//�������ֵ����
				Parameter_info->LeftIntoTurnRound_Flag = 0;
				Parameter_info->RightIntoTurnRound_Flag = 0;
				
				//����ʶ�����ֵ���CCD���
				Parameter_info->CCD2OrCCD3_GetTurnRound = 0;
				
				//ҡͷ������
				HeadSteer_info.Steer_PWMOutputDuty = HeadSteer_info.Steer_Center;
				FTM2->CONTROLS[1].CnV = HeadSteer_info.Steer_Center;
			}
		}
	}
	
	//�õ�����ǰ��ʵ���ٶ�(������)
	Speed_info->Speed_Now = ( 	  Speed_info->SpeedFifo[0]
								+ Speed_info->SpeedFifo[1]
								+ Speed_info->SpeedFifo[2]
							) / 3;
	
	//�����ٶȶ���(������),Ԫ��0�����µ��ٶȣ���ǰ���ٶȲɼ���ʱ�����
	for(ii = SPEED_FIFO_LENGTH - 1; ii > 0; ii--)
	{
		Speed_info->SpeedFifo[ii] = Speed_info->SpeedFifo[ii - 1];
	}
	
	//�ƶ�Ŀ���ٶȶ���
	Speed_info->TargetSpeed[1] = Speed_info->TargetSpeed[0];
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_CalculateTargetSpeed
*  ����˵����С��Ŀ���ٶȼ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע������ҡͷ����Ĵ�Ǻʹ�Ǻ��CCD��������������ٶȼ���
*************************************************************************/
void myMotor_CalculateTargetSpeed(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info,
									Steer_Info *Steer_info,
									Speed_Info *Speed_info
								 )
{
	int16_t ii = 0, LineError_MAX = 0,LineError_D_MAX = 0;
	int32_t HeadSteerPWMError_MAX = 0, HeadSteerPWMError_D_MAX = 0,SteerPWMError_MAX = 0;
	uint8_t SpeedMode = Speed_info->SpeedMode; 
	
	switch(SpeedMode)
	{
		case 1://CCD1
			break;
		case 2://CCD2
			for(ii = 0; ii < 20; ii++)
			{
				//�ҳ�CCD������ƫ��
				if(LineError_MAX < fabs(CCD2_info->LineError[ii]))
				{
					LineError_MAX = fabs(CCD2_info->LineError[ii]);
				}
				
				//�ҳ�CCD������ƫ��仯��
				if(LineError_D_MAX < fabs(CCD2_info->LineError_D[ii]))
				{
					LineError_D_MAX = fabs(CCD2_info->LineError_D[ii]);
				}
			}
			break;
		case 3://CCD3
			break;
		default:
			break;
	}
	
	//------------����CCDƫ������ֵ--------------------//
	if(LineError_MAX > Speed_info->Error_K)
	{
		LineError_MAX = Speed_info->Error_K;
	}
	
	//------------����CCDƫ��仯�ʵ����ֵ--------------------//
	if(LineError_D_MAX > Speed_info->Error_D_K)
	{
		LineError_D_MAX = Speed_info->Error_D_K;
	}
	
	//------------�ҳ�ҡͷ������ƫ���ƫ�������ֵ----------------//
	for(ii = 0; ii < 10; ii++)
	{
		//ҡͷ������ƫ�����ֵ
		if(HeadSteerPWMError_MAX < fabs(HeadSteer_info->SteerPWM_Error[ii]))
		{
			HeadSteerPWMError_MAX = fabs(HeadSteer_info->SteerPWM_Error[ii]);
		}
		
		//ҡͷ������ƫ�������ֵ
		if(HeadSteerPWMError_D_MAX < fabs(HeadSteer_info->SteerPWM_Error_D[ii]))
		{
			HeadSteerPWMError_D_MAX = fabs(HeadSteer_info->SteerPWM_Error_D[ii]);
		}
		
		//ת�������ƫ�����ֵ
		if(SteerPWMError_MAX < fabs(Steer_info->SteerPWM_Error[ii]))
		{
			SteerPWMError_MAX = fabs(Steer_info->SteerPWM_Error[ii]);
		}
	}
	
	//------------����ҡͷ���ƫ�����ֵ------------------//
	if(HeadSteerPWMError_MAX > Speed_info->HeadSteerPWMError_K)
	{
		HeadSteerPWMError_MAX = Speed_info->HeadSteerPWMError_K;
	}
	
	//-------------����ҡͷ���ƫ�������ֵ--------------//
	if(HeadSteerPWMError_D_MAX > Speed_info->HeadSteerPWMError_D_K)
	{
		HeadSteerPWMError_D_MAX = Speed_info->HeadSteerPWMError_D_K;
	}
	
	//------------����ת����ƫ�����ֵ------------------//
	if(SteerPWMError_MAX > Speed_info->HeadSteerPWMError_K)
	{
		SteerPWMError_MAX = Speed_info->HeadSteerPWMError_K;
	}
	
	//ֱ�����߻������ʮ�ֵ�����ʹ��3��
	if(	   (2 == SpeedMode) 
		&& (1 == CCD3_info->Cross_Flag) 
		&& (0 == CCD1_info->LeftLoseLine_Flag) 
		&& (0 == CCD1_info->RightLoseLine_Flag))
	{
		SpeedMode = 3;
	}
	else if(   (2 == SpeedMode)
			&& (CCD1_info->CCD_Ready_Num >= CCD1_DataReady_Num) 
			&& (CCD2_info->CCD_Ready_Num >= CCD2_DataReady_Num)
			&& (0 == CCD1_info->Cross_Flag)
			&& (0 == CCD2_info->Cross_Flag)
			&& (0 == Parameter_info.RampReady_FLag)
			&& ((0 == CCD3_info->LeftLoseLine_Flag) || (0 == CCD3_info->RightLoseLine_Flag)))//�����ֳ�������ʱ����ʹ��3�����ٶ�
	{
	 
		Speed_info->SpeedMode3Acc_Counter++;
		
		if(Speed_info->SpeedMode3Acc_Counter >= 50)//������Ч50�βŻ���3������ֹ����
		{
			SpeedMode = 3;
		}
	}
	else if((2 == SpeedMode) || (1 == SpeedMode))
	{
		Speed_info->SpeedMode3Acc_Counter = 0;
	}
	
	//�����������趨
	if((( 1== SpeedMode) || (2 == SpeedMode))
		&& (1 != Speed_info->CurSpeedAcc_Flag) && (Speed_info->Speed_Now <= Speed_info->Cur_Speed)
		&& (fabs(Steer_info->SteerPWM_Error[0]) >= 900))//ת�����򵽼���֮��ʼ����
	{
		Speed_info->CurSpeedAcc_Flag = 1;
	}
	else if(fabs(Steer_info->SteerPWM_Error[0]) < 100)//���������ֹͣ����
	{
		Speed_info->CurSpeedAcc_Flag = 0;
		
		Speed_info->CurSpeedAcc = 0;//�����������ֵ
	}
	
	switch(SpeedMode)//���ݵ�λ�����ٶ�,һ�������趨�����ֵ�ǰΪ����ģʽ�����
	{
		case 1://ʹ������ٶ�����
			{
				if((Turn_Round != Car_state) && (Turn_Round != Car_state_Pre))//����������ת�Ǵ�
				{
					Speed_info->TargetSpeed[0] = Speed_info->Cur_Speed - (Speed_info->Straight_Speed - Speed_info->Snake_Speed);
				}
				else
				{
					Speed_info->TargetSpeed[0] = Speed_info->TurnRound_Speed;
				}
				
				Speed_info->SpeedMode3Add = 0;//����3���ٶ�����ֵ
			}
			break;
		case 2://��Ҫ����ƫ�����
			{
				if(0 == Parameter_info.RampReady_FLag)
				{
				
				Speed_info->TargetSpeed[0] =  Speed_info->Straight_Speed
										   - (Speed_info->Straight_Speed - Speed_info->Cur_Speed)*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K
										   - (Speed_info->Straight_Speed - Speed_info->Snake_Speed)*HeadSteerPWMError_D_MAX/Speed_info->HeadSteerPWMError_D_K;
				}
				else
				{
					Speed_info->TargetSpeed[0] = Speed_info->RampUp_Speed;
				}
				
				Speed_info->SpeedMode3Add = 0;//����3���ٶ�����ֵ
			}
			break;
		case 3://����ǰ��������������,��ֹ�������ٵ����޷���������
			{
				if(Speed_info->DistanceOfSpeedMode3_Counter >= Speed_info->DistanceOfSpeedMode3_Max)//�ﵽ��3��ʹ�������,���ȥ3������ٶȼ���
				{
					//��ʱӦ��ȥ��ֱ����߼���
					Speed_info->TargetSpeed[0] =  Speed_info->Straight_Speed;
					
					Speed_info->SpeedMode3Add = 0;//����3���ٶ�����ֵ
				}
				else
				{
					if(Speed_info->SpeedMode3Add < Speed_info->SpeedAdd_3)
					{
						Speed_info->SpeedMode3Add += 1;//ÿ����������1������,�����ӵ����ֵ����ֹ�ٶ�ͻ�����𶶶�
					}
					
					//û�н������ֵ����ʱ��
					if(Parameter_info.Time < Parameter_info.GetTurnRoundTime)
					{
						Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedMode3Add
												   - Speed_info->SpeedMode3Add*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K;
					}
					else//�������ֵ����ʱ�䣬��ȥ��3������ٶȼ���ֵ
					{
						Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + (0 == Parameter_info.SlowBeforeTurnRound_Flag)*Speed_info->SpeedMode3Add
												   - ((0 == Parameter_info.SlowBeforeTurnRound_Flag)*Speed_info->SpeedMode3Add)*HeadSteerPWMError_MAX/Speed_info->HeadSteerPWMError_K;
					}
				}
			}
			break;
		default:
			break;
	}//end of switch(Speed_info->SpeedMode)
	
	if(Parameter_info.Mode_HighSpeedDistance > 0)
	{
		Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedAdd_3;
	}
	
	//���ֵ�����жϳɹ�,�趨Ϊ���ֵ�����ٶ�
	if(((1 == Parameter_info.Left60Angle_Flag) || (1 == Parameter_info.Right60Angle_Flag))
		&& (Speed_info->TargetSpeed[0] > Speed_info->Angle60_Speed))
	{
		Speed_info->TargetSpeed[0] = Speed_info->Angle60_Speed;
	}
	
	//Ŀ���ٶ��޷�����ֹ�ٶ�����������
	if(Speed_info->TargetSpeed[0] > Speed_info->Straight_Speed + Speed_info->SpeedAdd_3)
	{
		Speed_info->TargetSpeed[0] = Speed_info->Straight_Speed + Speed_info->SpeedAdd_3;
	}
	else if(Speed_info->TargetSpeed[0] < 10)
	{
		Speed_info->TargetSpeed[0] = 10;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_PIDCalculate
*  ����˵�������PID����
*  ����˵����
*  �������أ�	��ǰӦ�������PWMռ�ձȣ�����ʽPID����Ľ����
*  				����Ϊ��ʱ�����ת������Ϊ��ʱ�����ת
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
int32 myMotor_PIDCalculate(	Speed_Info *Speed_info,
							Motor_Info *Motor_info,
							Parameter_Info * Parameter_info)
{
  	int32 MotorPWMDuty = 0;				//���յõ���PWMռ�ձ�
	int32 SpeedDelta = 0;				//�����ƫ��(������)
	uint8 ii = 0;
	
	//��⵽������
	if(1 == Parameter_info->StartEndLine_Flag)
	{
		Speed_info->TargetSpeed[0] = 0;
	}
	
	//�����ٶ�ƫ��(������)
	Speed_info->LastSpeedError = Speed_info->SpeedError;
	Speed_info->SpeedError = Speed_info->TargetSpeed[0] - Speed_info->Speed_Now;
	
	//�����ٶ�ƫ��仯��(���ٶ�)
	Speed_info->SpeedRAcc = 0;
	
	for(ii = 0; ii < 5; ii++)
	{
		Speed_info->SpeedRAcc += Speed_info->SpeedFifo[ii];
	}
	
	for(ii = 5; ii < 10; ii++)
	{
		Speed_info->SpeedRAcc -= Speed_info->SpeedFifo[ii];
	}
	
	SpeedDelta = (int32_t)(    Motor_info->KPT*(Speed_info->TargetSpeed[0] - Speed_info->TargetSpeed[1])
							 + Motor_info->KP*(Speed_info->SpeedError - Speed_info->LastSpeedError)
							 + Motor_info->KI*(Speed_info->SpeedError)
							 - Motor_info->KD*(Speed_info->SpeedRAcc)
						  );
		
	//������ʱ��������λ��
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
	
		#ifndef DMA_CH2_SendImage_Enable
	
		LabVIEW_info.Motor_KP_Item = Motor_info->KP*(Speed_info->SpeedError - Speed_info->LastSpeedError);
		LabVIEW_info.Motor_KI_Item = Motor_info->KI*(Speed_info->SpeedError);
		LabVIEW_info.Motor_KD_Item = Motor_info->KD*(Speed_info->SpeedRAcc);
	
		#endif
	
	#endif
		
	/*-------------------------���������Ϳ����ֱ��ʹ���----------------------*/
	if((SpeedDelta > 0) && (Motor_info->PWMOutputDuty >= MOTOR_PWM_OUTPUT_MAX))//��ǰ��������PWMռ�ձ��Ѿ������������Ϊ��ʱ��������PWMռ�ձ�
	{
		SpeedDelta = 0;
	}
	else if((SpeedDelta < 0) && (Motor_info->PWMOutputDuty <= MOTOR_PWM_OUTPUT_MIN))//��ǰ��������PWMռ�ձ��Ѿ���С��������Ϊ��ʱ���ټ�СPWMռ�ձ�(������ֵ)
	{
		SpeedDelta = 0;
	}
	
	//���µ����PWM���
	MotorPWMDuty = Motor_info->PWMOutputDuty + SpeedDelta;
	
	//--------------------------------Bang-Bang�㷨-----------------------------------------//
	//�ж��Ƿ���Ҫ���Ӽ��٣������Ҫ��ȥ��PID����ֱ��ȫ���Ӽ���
	if(Speed_info->SpeedError > 12)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MAX;//ֱ��ȫ��ǰ��
	}
	else if(Speed_info->SpeedError < -12)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MIN;//ֱ��ȫ�ٷ�תɲ��
	}
		
	//--------------------------PWM���ռ�ձ�����ܵ��޷�--------------------------------//
	if(MotorPWMDuty > MOTOR_PWM_OUTPUT_MAX)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MAX;
	}
	else if(MotorPWMDuty < MOTOR_PWM_OUTPUT_MIN)
	{
		MotorPWMDuty = MOTOR_PWM_OUTPUT_MIN;
	}
	
	return MotorPWMDuty;
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_Control
*  ����˵�������PWM�������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myMotor_Control(	Motor_Info *Motor_info,
						Speed_Info *Speed_info,
						Parameter_Info * Parameter_info
					)
										
{
	if(TRUE == Speed_info->SpeedCalculate_Enable)//����ջ�
	{
		Motor_info->PWMOutputDuty = myMotor_PIDCalculate(Speed_info,Motor_info,Parameter_info);
	}
	else//�������
	{
		if(1 == Parameter_info->StartEndLine_Flag)
		{
			Motor_info->PWMOutputDuty = 0;
		}
		else
		{
			Motor_info->PWMOutputDuty = MOTOR_PWM_OUTPUT_DUTY;
		}
	}
	
	//������˳���3m�ľ��룬���Զ�ֹͣ���,��ֹ���������ײ��
	if((Speed_info->Speed_Now < 0)&& (Speed_info->DistanceTo1CmCount < -20400) && (Motor_info->PWMOutputDuty < -1000))//����ʱ���������Ϊ��
	{
		Motor_info->PWMOutputDuty = 0;
	}
	
	//�Ե��������޷�
	if(Motor_info->PWMOutputDuty > 9990)
	{
		Motor_info->PWMOutputDuty = 9990;
	}
	else if(Motor_info->PWMOutputDuty < -9990)
	{
		Motor_info->PWMOutputDuty  = -9990;
	}

	//������
	if(Motor_info->PWMOutputDuty > 0)//�����ת
	{
		FTM0->CONTROLS[6].CnV = Motor_info->PWMOutputDuty;
		FTM0->CONTROLS[7].CnV = 0;
	}
	else//�����ת
	{
		FTM0->CONTROLS[6].CnV = 0;
		FTM0->CONTROLS[7].CnV = -Motor_info->PWMOutputDuty;
	}
}

/*************************************************************************
*					��		��		ƫ		��   
*
*  �������ƣ�myMotor_Init
*  ����˵��������ĳ�ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myMotor_Init(void)
{
	PORTD->PCR[7]=PORT_PCR_MUX(4);			//D7:FTM0_CH7
	PORTD->PCR[6]=PORT_PCR_MUX(4);			//D6:FTM0_CH7
			
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;			//ʹ��ʱ��
	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;			//д������ֹ
			
	//----------------------CnSCͨ��״̬����---------------------------//
	//----------------------���ض���PWM�ȸߺ��  		
	FTM0->CONTROLS[7].CnSC |= FTM_CnSC_ELSB_MASK;		//����룬�ȸߺ��
	FTM0->CONTROLS[7].CnSC &=~FTM_CnSC_ELSA_MASK; 		//
	FTM0->CONTROLS[7].CnSC |= FTM_CnSC_MSB_MASK;		//ģʽѡ�ű��ض���ģʽ
		
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_ELSB_MASK;		//����룬�ȸߺ��
	FTM0->CONTROLS[6].CnSC &=~FTM_CnSC_ELSA_MASK; 		//
	FTM0->CONTROLS[6].CnSC |= FTM_CnSC_MSB_MASK;		//ģʽѡ�ű��ض���ģʽ

	//---------------------FTM0״̬����--------------------------------
	//[7]TOF��ʱ�������־
	//[6]TOFIE��ʱ������ж�ʹ��
	//[5]CPWMS=0 �������ӷ�����
	//[4:3]CLKS=01 system clock
	//[2:0]PS Ԥ��Ƶ��=2^PS			
	FTM0->SC=0x08;		//�жϽ�ֹ,ʱ��Դ����Ƶ16����������

	//---------------------����ģʽѡ��-----------------------------------
  	FTM0->MODE &=~ 1;					//ʹ��FTM0
			
	//---------------------������μĴ���---------------------------------
	FTM0->OUTMASK =~ (FTM_OUTINIT_CH7OI_MASK | FTM_OUTINIT_CH6OI_MASK);	//ͨ��7 6�����������ֹ
	FTM0->OUTINIT |=0xC0;
	
	//---------------------��������״̬����-------------------------------
	FTM0->QDCTRL &=~ FTM_QDCTRL_QUADEN_MASK;	//��ֹ��������ģʽ
		
	//---------------------ͨ�����Ϲ���-----------------------------------
	FTM0->COMBINE = 0 ;	//˫���ز�׽��ֹ��������
		
	FTM0->CNTIN = 0 ;					//��������ʼֵ 0
	FTM0->MOD = 10000;					//��������ֵֹ
	
	//----------------------����ֵ------------------------------------------
	FTM0->CONTROLS[7].CnV = 0;			//�ߵ�ƽʱ��
	FTM0->CONTROLS[6].CnV = 0;			//�ߵ�ƽʱ��
	
	FTM0->CNT = 0;						//��������0
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myMotor_EncoderInit
*  ����˵������������ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-05-09
*  ��    ע��
*************************************************************************/
void myMotor_EncoderInit(void)
{
  
	PORTA->PCR[8]=PORT_PCR_MUX(6);			// A8ΪFTM_PH1A
	PORTA->PCR[9]=PORT_PCR_MUX(6);			// A9ΪFTM_PH1B
	
	SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;			//ʹ��FTM1ʱ��
	
	FTM1->MODE |= FTM_MODE_WPDIS_MASK;			//д������ֹ
	
	FTM1->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;		//���ü����ͷ������ģʽ
	
	FTM1->CNTIN = 0;						//��������ʼֵ
	FTM1->MOD = 65535;  					//����������ֵ
	
	FTM1->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;		//����FTM1��������ģʽ
	FTM1->MODE |= FTM_MODE_FTMEN_MASK;			//ʹ��FTM1
	FTM1->CNT = 0;						//������ֵ����
	
}

/*--------------------------end of myMotor.c--------------------------------*/


