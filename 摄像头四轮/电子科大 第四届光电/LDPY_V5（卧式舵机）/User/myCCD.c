

#include "myCCD.h"

 CCD_Info CCD1_info = {0};
 CCD_Info CCD2_info = {0};
 CCD_Info CCD3_info = {0};
 
 extern Parameter_Info Parameter_info;
 extern HeadSteer_Info HeadSteer_info;
 extern Steer_Info Steer_info;
 extern Motor_Info Motor_info;
 extern Speed_Info Speed_info;
 extern Road_Condition Road_condition;
 extern Gyro_Info Gyro_info;
 
 extern Car_State Car_state;
 extern Car_State Car_state_Pre;
 
 extern Car_Mode Car_mode;
 
 extern uint16_t StopType;
 
 extern DMA_Required_Variable DMA_Variable_1;
 
/*************************************************************************
*					��		��		ƫ		��                           
*
*  ��������:myCCD_DataInit(CCD_Info * CCD_info)
*  ����˵��:CCD ���ݳ�ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-06-21
*  ��    ע��
*************************************************************************/	
void myCCD_DataInit(CCD_Info * CCD_info)
{
	uint8_t ii = 0, jj = 0;
	 
	for (ii = 0; ii < 2; ii++)
	{
		for (jj = 0; jj < 20; jj++)
		{
			CCD_info->PixelOri[ii][jj] = 0;
			CCD_info->PixelOri[ii][127 - jj] = 0;
		}
			
		for (jj = 20; jj < 107; jj++)
		{
			CCD_info->PixelOri[ii][jj] = 2000;
		}
	}
	
	for (ii = 0; ii < 128; ii++)
	{
		CCD_info->Pixel[ii] = 2000;
		CCD_info->PixelAfterMult[ii] = 2000;
		CCD_info->PixelBinary[ii] = 0;
	}
	
	CCD_info->CCD_PhotoCenter = 63;
	CCD_info->CCD_ObstacleShift = 2;					//Ĭ��ƫ��������
	
	for (ii = 0; ii < Line_SIZE; ii++)
	{
	    CCD_info->LeftLine[ii] = 0;
		CCD_info->RightLine[ii] = 0;
		CCD_info->CentralLine[ii] = Photo_Center;
		CCD_info->LineError[ii] = 0;
		CCD_info->LineError_D[ii] = 0;
	}
	
	CCD_info->LeftLossLinePixel = 0;
	CCD_info->RightLossLinePixel = 0;
	CCD_info->LeftLoseLine_Flag = 0;
	CCD_info->RightLoseLine_Flag = 0;
	
	for (ii = 0; ii < 10; ii++)
	{
		CCD_info->RoadWidth[ii] = 50;
	}
	
	CCD_info->RoadWidthOfStraight = 70;
	
	CCD_info->LeftLine[0] = 28;
	CCD_info->RightLine[0] = 100;
	
	for (ii = 0; ii < 4; ii++)
	{
		CCD_info->AD_MAX[ii] = 1200;
		CCD_info->AD_MIN[ii] = 0;
	}
	
	for (ii = 0; ii < 16; ii++)
	{
		CCD_info->CCD_PhotoValue[ii] = 0;
	}
	
	for(ii = 0; ii < ANGLE_LENGTH; ii++)
	{
		CCD_info->Angle_LeftFifo[ii] = 20;
		CCD_info->Angle_RightFifo[ii] = 20;
	}

	CCD_info->InvalidPixel_Num = 15;	//ͼ����Ч������ʼ��Ϊ15
	CCD_info->CCD_Ready_Num = 0;
	
	CCD_info->AddLine_Flag = 0;			//���߱��
	CCD_info->LossLine_Flag = 0;		//���߱��
	CCD_info->Cross_Flag = 0;			//ʮ�ֵ����
	CCD_info->RoadInvalid_Flag = 0;
}
 
/*************************************************************************
*					��		��		ƫ		��                                
*
*  ��������:myCCD_DataHandle
*  ����˵��:CCD���ݴ���
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-26
*  ��    ע: 
*************************************************************************/	
void myCCD_DataHandle(	CCD_Info *CCD1_info,
						CCD_Info *CCD2_info,
						CCD_Info *CCD3_info,
						HeadSteer_Info *HeadSteer_info,
						Speed_Info *Speed_info,
						Parameter_Info *Parameter_info,
						Road_Condition *Road_condition,
						Car_State *Car_state,
						Car_State *Car_state_Pre
				   )
{
	uint8_t ii = 0;
	
	//�����˲��Ͷ�ֵ��
	myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);

	//CCD�ѱ��ߺ�ƫ�����
	myCCD_CCD1_GetLineError(CCD1_info,Speed_info);
	myCCD_CCD2_GetLineError(CCD1_info,CCD2_info,Speed_info);
	myCCD_CCD3_GetLineError(CCD3_info,Speed_info);
	
	//�ƶ�60�ȼ�Ƕ���
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		//�ƶ�����
		for(ii = ANGLE_LENGTH - 1; ii > 0; ii--)
		{
			CCD3_info->Angle_LeftFifo[ii] = CCD3_info->Angle_LeftFifo[ii - 1];
			CCD3_info->Angle_RightFifo[ii] = CCD3_info->Angle_RightFifo[ii - 1];
		}
	}
		
	//ʶ�����ֵ�60�ȼ��
	if(	   (TRUE == Parameter_info->GetTurnRound_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetTurnRoundTime)
		&& (1 != CCD1_info->Cross_Flag)
		&& (1 != CCD2_info->Cross_Flag))//ʮ�ֵ���ʶ��60�ȼ��
	{
		//�ж�60�ȼ��
		myCCD_Get_60_Angle(CCD1_info,CCD2_info,CCD3_info,Speed_info,Parameter_info,Car_state);
	}

	//ѡ��λ(ѡ��ʹ��CCD1��CCD2����CCD3)
	myCCD_ChooseSpeedMode(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info);
	
	//�ж�ֱ�������ڼ��������
	if(TRUE == Parameter_info->GetStartEndLine_SameLight_Enable)
	{
		for(ii = 0; ii < 10; ii++)
		{
			if((fabs(HeadSteer_info->SteerPWM_Error[ii]) > 500)
				|| (fabs(CCD2_info->LineError[ii]) > 16)
				|| (0 == CCD3_info->CCD_Ready_Num)
				|| (1 == CCD1_info->LossLine_Flag))
			{
				break;
			}
		}
		
		if(10 == ii)
		{
			Parameter_info->LongStraight_Flag = 1;
		}
		else 
		{
			Parameter_info->LongStraight_Flag = 0;
		}
	}
	
	#ifdef DMA_CH1_Enable//��DMA������ƫ��Ȳ���

		LDPY_DMA_UpdateFifo_Parameters(CCD1_info,&Steer_info,&Motor_info,Speed_info,Road_condition);
		
		DMA0->ERQ |= (1 << DMA_CH1);
	
	#endif
	//end of DMA_CH1_Enable
	
	#ifdef DMA_CH2_Enable//��DMA,����ѹ��ͼ��
		#ifdef DMA_CH2_SendImage_Enable
			if (1 == Speed_info->DistanceOf1Cm_Flag)//1cm����һ��
			{
				LDPY_DMA_UpdateFifo_PhotoValue(CCD1_info,CCD2_info,CCD3_info);
			
				DMA0->ERQ |= (1 << DMA_CH2);
			}
		#endif
	#endif
	//end of DMA_CH2_Enable
			
	/*---------------------------CCD1ʶ��������-------------------------------------------------------*/
	if(    (Stop != *Car_state)
		&& (Ramp_Up != *Road_condition)
		&& (Ramp_Down != *Road_condition)
		&& (1 != Parameter_info->CCD1_GetedStartEndLine_Flag)
		&& (1 == Parameter_info->GetStartEndLine_Flag)
		&& (TRUE == Parameter_info->GetStartEndLine_Enable)
		&& (TRUE == Parameter_info->CCD1GetSL_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetStartEndLineTime)
		&& (TRUE == myCCD_CCD1_GetStartEndLine(CCD1_info,CCD2_info,CCD3_info,Road_condition,Car_state)))
	{
		Parameter_info->CCD1_GetedStartEndLine_Flag = 1;//CCD1Ԥ�������߳ɹ�
	}
	else if((1 == Parameter_info->CCD1_GetedStartEndLine_Flag))//CCD1������Ԥ�гɹ���Ӧ�ü�ʱ�������ֹ����
	{
		if((1 == CCD2_info->LossLine_Flag)
			|| (1 == CCD2_info->AddLine_Flag)
			|| (1 == CCD3_info->LossLine_Flag)
			|| (fabs(CCD2_info->LineError[0]) > 20))
		{
			Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
			Speed_info->DistanceOfCCD1GetSL_Counter = 0;
		}
	}
	
	/*---------------------------��ҡͷ�����Ǻ�С��ʱ������µ�,·���ж�-----------------------*/
	//�ж��µ�(�������º�����)
	if(	   ((Go_Forward == *Car_state) && (TRUE == Parameter_info->GetRamp_Enable) && (Parameter_info->Time >= Parameter_info->GetRampTime))
		|| ((Ramp_Up == *Road_condition) || (Ramp_Down == *Road_condition)))//������£����������µ�ʱ�������º��µ�
	{
		*Road_condition = myGyro_GetRamp(CCD1_info,&Gyro_info,Speed_info,Road_condition);
	}
	
	//�ж�·��
	if(	(Go_Forward == *Car_state)
		&& (Ramp_Up != *Road_condition) && (Ramp_Down != *Road_condition) && (Obstacle != *Road_condition)
		&& (TRUE == Parameter_info->GetObstacle_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetObstacleTime)
		&& (0 != myCCD_GetObstacle(CCD1_info,CCD2_info,&Steer_info)))//�������µ���ʱ�����·���ж�
	{
		Parameter_info->LeftOrRightObstacle_Flag = myCCD_GetObstacle(CCD1_info,CCD2_info,&Steer_info);

		Parameter_info->GetObstacle_Enable = FALSE;//ʶ��·�Ϻ���ʶ��·��
		
		Speed_info->DistanceOfObstacle_Counter = 80;//80cm�Ĺ�·�Ͼ���
		
		*Road_condition = Obstacle;//·���жϳɹ�
	}
} 

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_ChooseSpeedMode		
*  ����˵��: ѡ��ʹ����һ��CCD
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-07-05
*  ��    ע:
*************************************************************************/	
void myCCD_ChooseSpeedMode(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info
						  )
{	

	uint8_t OldSpeedMode = Speed_info->SpeedMode;//��¼��ǰ���ٶ�ģʽ
	
	Speed_info->SpeedMode = 1;//Ĭ��Ϊ1��
	
	/*------------------------------ѡ��CCD----------------------------------*/
	if(1 == OldSpeedMode)//���ڵ��ٵ�(1��)
	{
		if (   (CCD2_info->CCD_Ready_Num >= 3)
			&& (0 == CCD1_info->LossLine_Flag))//CCD2��Ч
		{
			Speed_info->SpeedMode = 2;//ʹ��2��
			
			if(	   (0 == CCD2_info->LossLine_Flag)
				&& (0 == CCD2_info->AddLine_Flag)
				&& (CCD3_info->CCD_Ready_Num >= 15)
				&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//CCD2�������߲��л���3��
			{
				Speed_info->SpeedMode = 3;//ʹ��3��
			}
		}
	}
	else//���ڸ��ٵ�(2����3��)
	{
		if (CCD2_info->CCD_Ready_Num >= 3)//CCD2��Ч,�Ҳ�����ȷ,CCD1��Ч����ֹ�ܵ�
		{
			Speed_info->SpeedMode = 2;//ʹ��2��
			
			//����2��
			if(    (2 == OldSpeedMode)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (0 == CCD2_info->AddLine_Flag)
				&& (CCD3_info->CCD_Ready_Num >= 15)
				&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//ԭ������2��������2������3������Ҫ�ϸ�
			{
				Speed_info->SpeedMode = 3;//ʹ��3��
			}
			//����3��
			else if(   (CCD3_info->CCD_Ready_Num >= 15)
					&& (FALSE == myCCD_JudgeWheelOutOfWay(CCD1_info,CCD2_info,CCD3_info,Speed_info)))//ԭ������3��,��3��ʹ����������
			{
				Speed_info->SpeedMode = 3;//ʹ��3��
			}
		}
	}
	
	if(Speed_info->SpeedMode != OldSpeedMode)//���ٶ�ģʽ����һ�β�ͬ��ʱ��ı�
	{
		Speed_info->LastSpeedMode = OldSpeedMode;//��¼��һ�ε��ٶ�ģʽ
	}
	
	if((Ramp_Up == Road_condition) && (Speed_info->DistanceOfRampUp_Counter >= 35))//���³���35cm���Զ���Ϊ����
	{
		CCD1_info->LossLine_Flag = 1;//CCD1ǿ�ƶ���
		
		CCD1_info->CCD_Ready_Num = 0;
		CCD2_info->CCD_Ready_Num = 0;
		CCD3_info->CCD_Ready_Num = 0;
	}
	
	//����CCD���г�ֱ�������µ�ʱԤ���µ���������
	if( (Go_Forward == Car_state)
		&& (TRUE == Parameter_info->GetRamp_Enable)
		&& (Parameter_info->Time >= Parameter_info->GetRampTime)
		&& (HeadSteer_info->SteerPWM_Error[0] < 500)
		&& (TRUE == myCCD_JudgeRampInAdvance(CCD1_info,CCD2_info,CCD3_info)))
	{
		
		if((Parameter_info->RampJgeInAdvce_Num < 30) && (1 == Speed_info->DistanceOf1Cm_Flag))
		{
			Parameter_info->RampJgeInAdvce_Num++;
		}
		
		if(Parameter_info->RampJgeInAdvce_Num >= 10)//����10����Ԥ�гɹ�
		{
			Parameter_info->RampReady_FLag = 1;//�µ�Ԥ�гɹ�
			
			if(3 == Speed_info->SpeedMode)
			{
				Speed_info->SpeedMode = 2;
			}
			
			LED3_ON;
		}
	}
	else
	{
		Parameter_info->RampJgeInAdvce_Num = 0;//�����µ�Ԥ�м���
	}
	
}

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_JudgeRampInAdvance		
*  ����˵��:����3��CCD���г�ֱ�����µ����µ�Ԥ��
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-08-13
*  ��    ע: �µ�·����������Ҵ�����ֱ���ϵ�·�������Ϊ�µ�
*************************************************************************/	
uint8_t myCCD_JudgeRampInAdvance(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8_t ii = 0;
	
	int16_t CCD1_Left = 0,CCD1_Right = 0;
	int16_t CCD2_Left = 0,CCD2_Right = 0;
	int16_t CCD3_Left = 0,CCD3_Right = 0;

	ii = 63;
	
	if((1 == CCD1_info->PixelBinary[ii]) && (1 == CCD2_info->PixelBinary[ii]) && (1 == CCD3_info->PixelBinary[ii]))
	{
		//�ж�CCD3·���Ƿ����
		ii = 63;
		
		//�ж���߽�
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Left = ii;
				break;
			}
			
			ii--;
		}
		
		ii = 63;
		
		//�ж��ұ߽�
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Right = ii;
				break;
			}
			
			ii++;
		}
		
		//�ж�CCD1
		if((0 != CCD3_Left) && (0 != CCD3_Right) && (CCD3_Right - CCD3_Left > 50) && (CCD3_Right - CCD3_Left < ROAD_WIDETH_3_MAX))
		{
			//�ж�CCD2·���Ƿ����
			ii = 63;
			
			//�ж���߽�
			while(ii > CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Left = ii;
					break;
				}
				
				ii--;
			}
			
			ii = 63;
			
			//�ж��ұ߽�
			while(ii < 127 - CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Right = ii;
					break;
				}
				
				ii++;
			}
			
			//�ж�CCD2
			if((0 != CCD2_Left) && (0 != CCD2_Right) && (CCD2_Right - CCD2_Left > 66) && (CCD2_Right - CCD2_Left < ROAD_WIDETH_2_MAX))
			{
				//�ж�CCD1·���Ƿ����
				ii = 63;
				
				//�ж���߽�
				while(ii > CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Left = ii;
						break;
					}
					
					ii--;
				}
				
				ii = 63;
				
				//�ж��ұ߽�
				while(ii < 127 - CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Right = ii;
						break;
					}
					
					ii++;
				}
				
				//�ж�CCD1
				if((0 != CCD1_Left) && (0 != CCD1_Right) && (CCD1_Right - CCD1_Left > ROAD_WIDETH_1_MIN) && (CCD1_Right - CCD1_Left < ROAD_WIDETH_1_MAX))
				{	
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else 
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_JudgeRampDown	
*  ����˵��:����3��CCD���������ж�
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-08-14
*  ��    ע: 3��CCD���ص�����״̬�������Ϊ���µı�־
*************************************************************************/	
uint8_t myCCD_JudgeRampDown(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Road_Condition *Road_condition)
{
	uint8_t ii = 0;
	
	int16_t CCD1_Left = 0,CCD1_Right = 0;
	int16_t CCD2_Left = 0,CCD2_Right = 0;
	int16_t CCD3_Left = 0,CCD3_Right = 0;

	ii = 63;
	
	if((Ramp_Up == *Road_condition) && (1 == CCD1_info->PixelBinary[ii]) && (1 == CCD2_info->PixelBinary[ii]) && (1 == CCD3_info->PixelBinary[ii]))
	{
		//�ж�CCD3·���Ƿ����
		ii = 63;
		
		//�ж���߽�
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Left = ii;
				break;
			}
			
			ii--;
		}
		
		ii = 63;
		
		//�ж��ұ߽�
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])
			{
				CCD3_Right = ii;
				break;
			}
			
			ii++;
		}
		
		//�ж�CCD1
		if((0 != CCD3_Left) && (0 != CCD3_Right) && (CCD3_Right - CCD3_Left > ROAD_WIDETH_3_MIN) && (CCD3_Right - CCD3_Left < ROAD_WIDETH_3_MAX))
		{
			//�ж�CCD2·���Ƿ����
			ii = 63;
			
			//�ж���߽�
			while(ii > CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Left = ii;
					break;
				}
				
				ii--;
			}
			
			ii = 63;
			
			//�ж��ұ߽�
			while(ii < 127 - CCD2_info->InvalidPixel_Num)
			{
				if(0 == CCD2_info->PixelBinary[ii])
				{
					CCD2_Right = ii;
					break;
				}
				
				ii++;
			}
			
			//�ж�CCD2
			if((0 != CCD2_Left) && (0 != CCD2_Right) && (CCD2_Right - CCD2_Left > ROAD_WIDETH_2_MIN) && (CCD2_Right - CCD2_Left < ROAD_WIDETH_2_MAX))
			{
				//�ж�CCD1·���Ƿ����
				ii = 63;
				
				//�ж���߽�
				while(ii > CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Left = ii;
						break;
					}
					
					ii--;
				}
				
				ii = 63;
				
				//�ж��ұ߽�
				while(ii < 127 - CCD1_info->InvalidPixel_Num)
				{
					if(0 == CCD1_info->PixelBinary[ii])
					{
						CCD1_Right = ii;
						break;
					}
					
					ii++;
				}
				
				//�ж�CCD3
				if((0 != CCD1_Left) && (0 != CCD1_Right) && (CCD1_Right - CCD1_Left > ROAD_WIDETH_1_MIN) && (CCD1_Right - CCD1_Left < ROAD_WIDETH_1_MAX))
				{	
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else 
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_JudgeStraightIntoCross	
*  ����˵��:����3��CCD���г�ֱ����ʮ�ֵ����ж�
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-07-05
*  ��    ע:
*************************************************************************/	
uint8_t myCCD_JudgeLongStraight(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info
								)
{
	uint8_t ii = 0;
	
	//��ֱ���ж�
	for(ii = 1; ii < 31; ii++)
	{
		if(fabs(CCD3_info->LineError[ii]) >= 10)
		{
			break;
		}
		else if(fabs(CCD2_info->LineError[ii]) >= 10)
		{
			break;
		}
		else if(fabs(CCD1_info->LineError[ii]) >= 10)
		{
			break;
		}
	}
	
	if((31 == ii) && (fabs(HeadSteer_info->SteerPWM_Error[0]) < 200))//�ǳ�ֱ��
	{	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_CCD1_GetLineError		
*  ����˵��:CCD1��Ѱ����
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-21
*  ��    ע: ���ϴε��е������������͵�ƽ
*************************************************************************/	
void myCCD_CCD1_GetLineError(CCD_Info *CCD1_info,Speed_Info *Speed_info)
{	
	int16_t CentralLinePixel_Now = CCD1_info->CentralLine[0];//���ϴε��е㿪ʼ����
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//������
	CCD1_info->LossLine_Flag = 0;
	CCD1_info->Cross_Flag  = 0;
	CCD1_info->AddLine_Flag = 0;
	CCD1_info->LeftLoseLine_Flag = 1;
	CCD1_info->RightLoseLine_Flag = 1;

	if ((CentralLinePixel_Now < 2 + CCD1_info->InvalidPixel_Num) && (CCD1_info->RightLine[0] > CCD1_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD1_info->InvalidPixel_Num;
		
		//�ж������Ƿ���Ч
		if((TRUE == Parameter_info.RoadInvalid_Enable)
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition))//�µ����ж�������Ч
		{
			if(	   (0 == CCD1_info->RoadInvalid_Flag)
				&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 1;
			}
			else if(   (1 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now])
					&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//�����·��Ч����
			}
		}
		else 
		{
			CCD1_info->RoadInvalid_Flag = 0;
		}
	}
	else if ((CentralLinePixel_Now > 125 - CCD1_info->InvalidPixel_Num) && (CCD1_info->RightLine[0] > CCD1_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD1_info->InvalidPixel_Num;
		
		//�ж������Ƿ���Ч
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition))//�µ����ж�������Ч
		{
			if(    (0 == CCD1_info->RoadInvalid_Flag)
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5]) 
				&& (0 == CCD1_info->PixelBinary[CentralLinePixel_Now]) 
				&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			
			{
				CCD1_info->RoadInvalid_Flag = 2;
			}
			else if(   (0 == CCD1_info->PixelBinary[CentralLinePixel_Now - 5])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now])
					&& (1 == CCD1_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD1_info->RoadInvalid_Flag = 0;
				
				Speed_info->DistanceOfRoadInvalid_Counter = 0;//�����·��Ч����
			}
		}
		else 
		{
			CCD1_info->RoadInvalid_Flag = 0;
		}
	}
	else if (CCD1_info->LeftLine[0] >= CCD1_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//���ұ߽�����쳣������һ�ε����ұ߽�ֵ�����
		CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
		CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
	}

	if((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition))//�µ����м俪ʼ����
	{
		LinePixel_Temp = CCD1_info->CCD_PhotoCenter;
	}
	else
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------�����------------------------*/
	if (0 == CCD1_info->RoadInvalid_Flag)//������Чʱ�Ž�������
	{		
		ii = LinePixel_Temp;
		
		while(ii > CCD1_info->InvalidPixel_Num)
		{
			if(1 == CCD1_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
			{
				break;
			}
			
			ii--;
		}
		
		if(ii != CCD1_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
		{
			while (ii > CCD1_info->InvalidPixel_Num)
			{
				 if (0 == CCD1_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
				 {
					 Left_temp = ii;
					 break;
				 }
				 else
				 {
					 ii--;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
				 }
			}
		}
	}
	else
	{
		CCD1_info->LossLine_Flag = 1;
	}

	//��߶���
	if (0 == Left_temp)
	{ 
		Left_temp = CCD1_info->InvalidPixel_Num; 
	}	
			
   /*-------------------�ұ���------------------------*/	
	if(0 == CCD1_info->RoadInvalid_Flag)//������Чʱ�Ž�������
	{
		ii = LinePixel_Temp + 1;
		
		while(ii < 127 - CCD1_info->InvalidPixel_Num)
		{
			if(1 == CCD1_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
			{
				break;
			}
			
			ii++;
		}
		
		if(ii != 127 - CCD1_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
		{
			while (ii < (127 - CCD1_info->InvalidPixel_Num))
			{
				 if (0 == CCD1_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
				 {
					 Right_temp = ii;
					 break;
				 }
				 else
				 {
					 ii++;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
				 }
			}
		}
	}
	else
	{
		CCD1_info->LossLine_Flag = 1;
	}

	//�ұ߶���
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD1_info->InvalidPixel_Num;
	}	
	
	/*------------------------------�ƶ�����---------------------------------*/
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD1_info->LeftLine[ii]    = CCD1_info->LeftLine[ii - 1];
		CCD1_info->RightLine[ii]   = CCD1_info->RightLine[ii - 1];
		CCD1_info->CentralLine[ii] = CCD1_info->CentralLine[ii - 1];
		CCD1_info->LineError[ii]   = CCD1_info->LineError[ii - 1];
		CCD1_info->LineError_D[ii] = CCD1_info->LineError_D[ii - 1];
	}
	
	//�ƶ�AD_MAX,AD_MIN����
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		CCD1_info->AD_MAX[3] = CCD1_info->AD_MAX[2];
		CCD1_info->AD_MAX[2] = CCD1_info->AD_MAX[1];
		CCD1_info->AD_MAX[1] = CCD1_info->AD_MAX[0];
		
		CCD1_info->AD_MIN[3] = CCD1_info->AD_MIN[2];
		CCD1_info->AD_MIN[2] = CCD1_info->AD_MIN[1];
		CCD1_info->AD_MIN[1] = CCD1_info->AD_MIN[0];
	}
	
	/*------------------------------��ͨ���������-------------------------------*/	
	if ((Left_temp <= CCD1_info->InvalidPixel_Num) && (Right_temp < 127 - CCD1_info->InvalidPixel_Num))//�����ұ߽緶Χ
	{
		CCD1_info->AddLine_Flag = 1;
		
		CCD1_info->LeftLine[0]  = Right_temp -  CCD1_info->RoadWidth[0];
		CCD1_info->RightLine[0] = Right_temp;

		if (((CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1) > CCD1_info->CCD_PhotoCenter)//��ֹ�������ʮ�ֵ�ʱ���ߴ���
		{
			CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
			CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
		}
		else if ((1 == Speed_info->DistanceOf1Cm_Flag) 
				&& (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//��������,1cm����,����1cm��־���ڸ���ҡͷ���ƫ�����֮��
		{
			CCD1_info->CCD_Ready_Num++;//CCD1������Ч�����Լ�
		}
		
		//��Ѱ���ֵ�
		if ((TRUE == Parameter_info.GetTurnRound_Enable) 
			&& (Parameter_info.Time >= Parameter_info.GetTurnRoundTime) 
			&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
			&& (TRUE == myCCD_CCD2_GetRightIntoTurnRound(&CCD2_info)))
		{
			if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX + 3)
			{
				Parameter_info.TurnRound_OK_Num++;
			}
			
			if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX + 2)
			{
				Parameter_info.TurnRound_OK_Num = 0;
				
				Parameter_info.GetTurnRound_Enable = FALSE;//�Ѿ��������ֵ����ټ�����ֵ�
					
				Car_state_Pre = Car_state;
				Car_state = Turn_Round;
				
				//���ұ߽������ֵ�
				Parameter_info.LeftIntoTurnRound_Flag = 0;
				Parameter_info.RightIntoTurnRound_Flag = 1;
				
				Parameter_info.CCD2OrCCD3_GetTurnRound = 2;//CCD2ʶ�������ֵ�
			}
		}
		else
		{
			Parameter_info.TurnRound_OK_Num = 0;
		}
	}
	/*------------------------------��ͨ������Ҳ���-------------------------------*/	
	else if ((Right_temp >= 127 - CCD1_info->InvalidPixel_Num) && (Left_temp > CCD1_info->InvalidPixel_Num))//������߽緶Χ
	{	
		CCD1_info->AddLine_Flag = 1;
		
		CCD1_info->RightLine[0] = Left_temp + CCD1_info->RoadWidth[0];
		CCD1_info->LeftLine[0]  = Left_temp;
		
		if (((CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1) < CCD1_info->CCD_PhotoCenter)//��ֹ�������ʮ�ֵ�ʱ���ߴ���
		{
			CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
			CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
		}
		else if ((1 == Speed_info->DistanceOf1Cm_Flag) 
				&& (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//��������,1cm����,����1cm��־���ڸ���ҡͷ���ƫ�����֮��
		{
			CCD1_info->CCD_Ready_Num++;//CCD1������Ч�����Լ�
		}
		
		//��Ѱ���ֵ�
		if ((TRUE == Parameter_info.GetTurnRound_Enable) 
			&& (Parameter_info.Time >= Parameter_info.GetTurnRoundTime)
			&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
			&& (TRUE == myCCD_CCD2_GetLeftIntoTurnRound(&CCD2_info)))
		{
			if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX + 3)
			{
				Parameter_info.TurnRound_OK_Num++;
			}
			
			if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX + 2)
			{
				Parameter_info.TurnRound_OK_Num = 0;
				
				Parameter_info.GetTurnRound_Enable = FALSE;//�Ѿ��������ֵ����ټ�����ֵ�
					
				Car_state_Pre = Car_state;
				Car_state = Turn_Round;
				
				//����߽������ֵ�
				Parameter_info.LeftIntoTurnRound_Flag = 1;
				Parameter_info.RightIntoTurnRound_Flag = 0;
				
				Parameter_info.CCD2OrCCD3_GetTurnRound = 2;//CCD2ʶ�������ֵ�
			}
		}	
		else
		{
			Parameter_info.TurnRound_OK_Num = 0;
		}
	}	
	/*------------------------------ʮ�ֵ�����������-------------------------------*/			 
	else if (0 == CCD1_info->AddLine_Flag)//û�н��в���
	{
		//��������
		if ((CCD1_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD1_info->InvalidPixel_Num))
		{
			CCD1_info->LeftLine[0] = Left_temp;
			CCD1_info->RightLine[0] = Right_temp;
			
			if ((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD1_info->CCD_Ready_Num < CCD1_DataReady_Num))//1cm����,����1cm��־���ڸ���ҡͷ���ƫ�����֮��
			{
				CCD1_info->CCD_Ready_Num++;//CCD1������Ч�����Լ�
			}
			
			CCD1_info->LeftLoseLine_Flag = 0;
			CCD1_info->RightLoseLine_Flag = 0;
		}
		else//������ʮ�ֵ�,�µ���ȫ�ڶ���,ֹͣ�������ұ߽�ֵ
		{
			CCD1_info->LossLine_Flag = 1;
			
			//�ж�ʮ�ֵ���ʮ�ֵ������CCD1����Ч����
			if(	   (0 == CCD1_info->RoadInvalid_Flag) 
				&& ((Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
				&& ((1 == CCD1_info->PixelBinary[LinePixel_Temp]) 
				&& (1 == CCD1_info->PixelBinary[LinePixel_Temp - 1]) 
				&& (1 == CCD1_info->PixelBinary[LinePixel_Temp + 1])))//�����µ���Ϊʮ�ֵ�����ȫ�ڶ���
			{
				CCD1_info->Cross_Flag = 1;
			}
			else
			{
				CCD1_info->CCD_Ready_Num = 0;
			}
		}
	}
	
	/*------------------------------���¶���-----------------------------------------*/
	if ((1 != CCD1_info->LossLine_Flag) && (CCD1_info->RightLine[0] - CCD1_info->LeftLine[0]) >= ROAD_WIDETH_1_MIN)//����·��
	{
		CCD1_info->CentralLine[0] = (CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1;
		CCD1_info->LineError[0] = CCD1_info->CentralLine[0] - CCD1_info->CCD_PhotoCenter;
		CCD1_info->LineError_D[0] = CCD1_info->LineError[0] - CCD1_info->LineError[8]; 
		
		if ((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition))//�µ�ʹ��CCD1�����������кܴ�Ĵ���Ҳ�����ƫ��ͻ��
		{
			if((fabs(CCD1_info->CentralLine[0] - CCD1_info->CentralLine[1]) > 5)
				|| (fabs(CCD1_info->LineError[0]) > 15))
			{
				CCD1_info->LeftLine[0] = CCD1_info->LeftLine[1];
				CCD1_info->RightLine[0] = CCD1_info->RightLine[1];
				
				CCD1_info->CentralLine[0] = (CCD1_info->LeftLine[0] + CCD1_info->RightLine[0]) >> 1;
				CCD1_info->LineError[0] = CCD1_info->CentralLine[0] - Photo_Center;
				CCD1_info->LineError_D[0] = CCD1_info->LineError[0] - CCD1_info->LineError[8]; 
				
				CCD1_info->LossLine_Flag = 1;//ƫ����ͻ��ʱ��ΪCCD1����
			}
		}
		
		if (((Right_temp - Left_temp) <= ROAD_WIDETH_1_MAX) && (1 == Speed_info->DistanceOf1Cm_Flag))//·����Ϲ涨�ٸ���·��1cm����һ��
		{
			CCD1_info->RoadWidth[9] = CCD1_info->RoadWidth[8];
			CCD1_info->RoadWidth[8] = CCD1_info->RoadWidth[7];
			CCD1_info->RoadWidth[7] = CCD1_info->RoadWidth[6];
			CCD1_info->RoadWidth[6] = CCD1_info->RoadWidth[5];
			CCD1_info->RoadWidth[5] = CCD1_info->RoadWidth[4];
			CCD1_info->RoadWidth[4] = CCD1_info->RoadWidth[3];
			CCD1_info->RoadWidth[3] = CCD1_info->RoadWidth[2];
			CCD1_info->RoadWidth[2] = CCD1_info->RoadWidth[1];
			CCD1_info->RoadWidth[1] = CCD1_info->RoadWidth[0];
			
			CCD1_info->RoadWidth[0] = CCD1_info->RightLine[0] - CCD1_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myCCD_CCD2_GetLineError		
*  ����˵��:CCD2��Ѱ����,���ƫ��
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-17
*  ��    ע: ���ϴε��е������������͵�ƽ
*************************************************************************/	
void myCCD_CCD2_GetLineError(CCD_Info *CCD1_info,CCD_Info *CCD2_info,Speed_Info *Speed_info)
{
	int16_t CentralLinePixel_Now = CCD2_info->CentralLine[0];//���ϴε��е㿪ʼ����
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//������ر��
	CCD2_info->Cross_Flag = 0;
	CCD2_info->AddLine_Flag = 0;
	CCD2_info->LeftLoseLine_Flag = 1;
	CCD2_info->RightLoseLine_Flag = 1;
	
	if ((CentralLinePixel_Now < 2 + CCD2_info->InvalidPixel_Num) && (CCD2_info->RightLine[0] > CCD2_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD2_info->InvalidPixel_Num;
		
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition) 
			&& (1 != CCD1_info->Cross_Flag))//�µ���ʮ�ֵ����ж���·��Ч
		{
			if((0 == CCD2_info->RoadInvalid_Flag) 
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 1;
			}
			else if((1 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 0;
			}
		}
		else 
		{
			CCD2_info->RoadInvalid_Flag = 0;
		}
	}
	else if ((CentralLinePixel_Now > 125 - CCD2_info->InvalidPixel_Num) && (CCD2_info->RightLine[0] > CCD2_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD2_info->InvalidPixel_Num;
		
		if((TRUE == Parameter_info.RoadInvalid_Enable) 
			&& (Ramp_Up != Road_condition) 
			&& (Ramp_Down != Road_condition) 
			&& (1 != CCD1_info->Cross_Flag))//�µ���ʮ�ֵ����ж���·��Ч
		{
			if((0 == CCD2_info->RoadInvalid_Flag) 
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (0 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 2;
			}
			else if((0 == CCD2_info->PixelBinary[CentralLinePixel_Now - 5])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now])
				&& (1 == CCD2_info->PixelBinary[CentralLinePixel_Now + 5]))
			{
				CCD2_info->RoadInvalid_Flag = 0;
			}
		}
		else 
		{
			CCD2_info->RoadInvalid_Flag = 0;
		}
	}
	else if (CCD2_info->LeftLine[0] >= CCD2_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//���ұ߽�����쳣������һ�εı߽�ֵ������
		CCD2_info->LeftLine[0] = CCD2_info->LeftLine[1];
		CCD2_info->RightLine[0] = CCD2_info->RightLine[1];
	}

	if((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition) || (1 == CCD1_info->Cross_Flag)
		|| ((CCD2_info->CCD_Ready_Num < 3) && (Turn_Round == Car_state_Pre)))//�µ���ʮ�ֵ��ͳ����ֵ������м�����������
	{
		LinePixel_Temp = 63;
	}
	else 
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------�����------------------------*/
	if(0 == CCD2_info->RoadInvalid_Flag)
	{
		ii = LinePixel_Temp;
		
		while(ii > CCD2_info->InvalidPixel_Num)
		{
			if(1 == CCD2_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
			{
				break;
			}
			
			ii--;
		}
		
		if(ii != CCD2_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
		{
			while (ii > CCD2_info->InvalidPixel_Num)
			{
				 if (0 == CCD2_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
				 {
					 Left_temp = ii;
					 break;
				 }
				 else
				 {
					 ii--;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
				 }
			}
		}
	}
	
	//��߶���
	if (0 == Left_temp) 
	{
		Left_temp = CCD2_info->InvalidPixel_Num;
	}	
			
    /*-------------------�ұ���------------------------*/
	if(0 == CCD2_info->RoadInvalid_Flag)
	{
		ii = LinePixel_Temp + 1;
		
		while(ii < 127 - CCD2_info->InvalidPixel_Num)
		{
			if(1 == CCD2_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
			{
				break;
			}
			
			ii++;
		}
		
		if(ii != 127 - CCD2_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
		{
			while (ii < (127 - CCD2_info->InvalidPixel_Num))
			{
				 if (0 == CCD2_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
				 {
					 Right_temp = ii ;
					 break;
				 }
				 else
				 {
					 ii++;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
				 }
			}
		}
	}
	//�ұ߶���
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD2_info->InvalidPixel_Num;
	}	
	
	//------------------------------�ƶ�����----------------------------------------//
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD2_info->LeftLine[ii] = CCD2_info->LeftLine[ii - 1];
		CCD2_info->RightLine[ii] = CCD2_info->RightLine[ii - 1];
		CCD2_info->CentralLine[ii] = CCD2_info->CentralLine[ii - 1];
		CCD2_info->LineError[ii] = CCD2_info->LineError[ii - 1];
		CCD2_info->LineError_D[ii] = CCD2_info->LineError_D[ii - 1];
	}
	
	//----------------------------------------CCD2������------------------------------------//
	if ((Left_temp <= CCD2_info->InvalidPixel_Num) && (Right_temp < 127 - CCD2_info->InvalidPixel_Num))
	{ 
		CCD2_info->AddLine_Flag = 1;
		
		if(	   (1 == CCD1_info->RoadInvalid_Flag)
			|| (1 == CCD2_info->LossLine_Flag)
		    || ((Right_temp - (CCD2_info->RoadWidth[0] >> 1)) > CCD2_info->CCD_PhotoCenter)
		  )//CCD1���в���ʱ����CCD1ȫ���߻���CCD2��������CCD2ֹͣ����
		{
			CCD2_info->LossLine_Flag = 1;		//��λCCD2���߱��
			
			CCD2_info->CCD_Ready_Num = 0;
		}
		else
		{
			CCD2_info->LeftLine[0] = Right_temp - CCD2_info->RoadWidth[0];
			CCD2_info->RightLine[0] = Right_temp;		

			if((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))
			{
				CCD2_info->CCD_Ready_Num++;		
			}
			
			CCD2_info->LossLine_Flag = 0;			//����CCD2���߱��
		}
		
		
		//����CCD3��Ѱ���ֵ�
		if ((TRUE == Parameter_info.GetTurnRound_Enable) && (Parameter_info.Time >= Parameter_info.GetTurnRoundTime))
		{
			if (   (0 == CCD1_info->LossLine_Flag)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (CCD2_info->LeftLossLinePixel >= 20)
				&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
				)//CCD2��߶���(���ֵ�����ʱLeftLine[0] >= 20)���ұ߲�����,CCD1û�ж��߲���ҡͷ���û�д�����
			{
				if(Speed_info->DiatanceToTurnRound <= 0)//û�м�¼�����ֵ���ǵľ���
				{
					Speed_info->DiatanceToTurnRound = 50;//��¼��ʱ�����ֵ���ǵľ���,CCD2ǰհ50cm
				}

				if (TRUE == myCCD_CCD3_GetRightIntoTurnRound(&CCD3_info))
				{
					if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num++;
					}
					
					if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.GetTurnRound_Enable = FALSE;//�Ѿ��������ֵ����ټ�����ֵ�
						
						Car_state_Pre = Car_state;
						Car_state = Turn_Round;
						
						//���ұ߽������ֵ�
						Parameter_info.LeftIntoTurnRound_Flag = 0;
						Parameter_info.RightIntoTurnRound_Flag = 1;
						
						Parameter_info.CCD2OrCCD3_GetTurnRound = 3;//CCD3ʶ�������ֵ�
					}
				}
				else
				{
					Parameter_info.TurnRound_OK_Num = 0;
				}
			}
		}
	}
	//---------------------------------------CCD2���Ҳ���-------------------------------------//
	else if ((CCD2_info->InvalidPixel_Num < Left_temp) && (Right_temp >= 127 - CCD2_info->InvalidPixel_Num))//��߲����ߣ��ұ߶���(���ֵ�����ʱRightLine[0] <= 90)
	{
		CCD2_info->AddLine_Flag = 1;
		
		if(    (1 == CCD1_info->RoadInvalid_Flag)
			|| (1 == CCD2_info->LossLine_Flag)
			|| ((Left_temp + (CCD2_info->RoadWidth[0] >> 1)) < CCD2_info->CCD_PhotoCenter)
		  )//CCD1���в���ʱ����CCD1ȫ���߻���CCD2��������CCD2ֹͣ����
		{
			CCD2_info->LossLine_Flag = 1;			//��λCCD2���߱��
			
			CCD2_info->CCD_Ready_Num = 0;
		}
		else 
		{
			CCD2_info->RightLine[0] = Left_temp + CCD2_info->RoadWidth[0];
			CCD2_info->LeftLine[0]  = Left_temp;	

			if((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))
			{
				CCD2_info->CCD_Ready_Num++;		
			}			
			
			CCD2_info->LossLine_Flag = 0;			//����CCD2���߱��
		}
		
		//����CCD3��Ѱ���ֵ�
		if ((TRUE == Parameter_info.GetTurnRound_Enable) && (Parameter_info.Time >= Parameter_info.GetTurnRoundTime))
		{	
			if (   (0 == CCD1_info->LossLine_Flag)
				&& (0 == CCD1_info->AddLine_Flag)
				&& (CCD2_info->RightLossLinePixel <= 110)
				&& (fabs(Steer_info.SteerPWM_Error[0]) < TurnRoundSteer_Max)
				)//CCD2�ұ߶���(���ֵ�����ʱ�ұ߽� <= 110)����߲�����,CCD1û�ж��߲���ҡͷ���û�д�����
			{
				if(Speed_info->DiatanceToTurnRound <= 0)//û�м�¼�����ֵ���ǵľ���
				{
					Speed_info->DiatanceToTurnRound = 50;//��¼��ʱ�����ֵ���ǵľ���,CCD2ǰհ50cm
				}
				
				if (TRUE == myCCD_CCD3_GetLeftIntoTurnRound(&CCD3_info))
				{
					if(Parameter_info.TurnRound_OK_Num < Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num++;
					}
					
					if(Parameter_info.TurnRound_OK_Num >= Parameter_info.TurnRound_Num_MAX)
					{
						Parameter_info.TurnRound_OK_Num = 0;
						
						Parameter_info.GetTurnRound_Enable = FALSE;//�Ѿ��������ֵ����ټ�����ֵ�
						
						Car_state_Pre = Car_state;
						Car_state = Turn_Round;
						
						//����߽������ֵ�
						Parameter_info.LeftIntoTurnRound_Flag = 1;
						Parameter_info.RightIntoTurnRound_Flag = 0;
						
						Parameter_info.CCD2OrCCD3_GetTurnRound = 3;//CCD3ʶ�������ֵ�
					}
				}
				else
				{
					Parameter_info.TurnRound_OK_Num = 0;
				}
			}
		}
	}
	else 
	{
		//---------------------------------��ͨ����-----------------------------------------//
		if (   (CCD2_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD2_info->InvalidPixel_Num)
			&& ((Right_temp - Left_temp) >= ROAD_WIDETH_2_MIN))//����·��
		{
			CCD2_info->LeftLine[0] = Left_temp;
			CCD2_info->RightLine[0] = Right_temp;
			
			if (   (1 == Speed_info->DistanceOf1Cm_Flag)
				&& (0 == CCD1_info->RoadInvalid_Flag)
				&& (CCD2_info->CCD_Ready_Num < CCD2_DataReady_Num))//1cm���ˣ�����α�־���ڿ���ҡͷ���֮��
			{
				CCD2_info->CCD_Ready_Num++;//CCD1û�в��߲��ҵ�·��Чʱ��CCD2������Ч�����Լӣ���ֹ�ܵ�
			}
			
			//��¼ÿ�ε����ߵ㣬һ�����ߣ������ͨ���жϴ˵��������Ƿ����ֵ�����
			CCD2_info->LeftLossLinePixel = Left_temp;
			CCD2_info->RightLossLinePixel = Right_temp;
			
			CCD2_info->LeftLoseLine_Flag = 0;
			CCD2_info->RightLoseLine_Flag = 0;
			
			CCD2_info->LossLine_Flag = 0;
		}
		//---------------------------------ʮ�ֵ�-----------------------------------------//
		else if((Left_temp <= CCD2_info->InvalidPixel_Num) && (Right_temp >= 127 - CCD2_info->InvalidPixel_Num))
		{
			if(    (1 == CCD2_info->PixelBinary[LinePixel_Temp]) 
				&& (1 == CCD2_info->PixelBinary[LinePixel_Temp - 1]) 
				&& (1 == CCD2_info->PixelBinary[LinePixel_Temp + 1]))
			{
				CCD2_info->Cross_Flag = 1;
			}
			else
			{
				CCD2_info->CCD_Ready_Num = 0;			//����CCD2������Ч����
			}
			
			CCD2_info->LossLine_Flag = 1;
		}
	}
	
	/*------------------------------���¶���--------------------------------*/
	if ((0 == CCD2_info->LossLine_Flag) && ((CCD2_info->RightLine[0] - CCD2_info->LeftLine[0]) <= ROAD_WIDETH_2_MAX))//CCD2������Ч
	{		
		CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0] ) >> 1;	
		CCD2_info->LineError[0] = CCD2_info->CentralLine[0] - CCD2_info->CCD_PhotoCenter; 
		CCD2_info->LineError_D[0] = CCD2_info->LineError[0] - CCD2_info->LineError[8];  
		
		if (1 == Speed_info->DistanceOf1Cm_Flag)//����CCD2��·��,1cm����һ��
		{
			CCD2_info->RoadWidth[9] = CCD2_info->RoadWidth[8];
			CCD2_info->RoadWidth[8] = CCD2_info->RoadWidth[7];
			CCD2_info->RoadWidth[7] = CCD2_info->RoadWidth[6];
			CCD2_info->RoadWidth[6] = CCD2_info->RoadWidth[5];
			CCD2_info->RoadWidth[5] = CCD2_info->RoadWidth[4];
			CCD2_info->RoadWidth[4] = CCD2_info->RoadWidth[3];
			CCD2_info->RoadWidth[3] = CCD2_info->RoadWidth[2];
			CCD2_info->RoadWidth[2] = CCD2_info->RoadWidth[1];
			CCD2_info->RoadWidth[1] = CCD2_info->RoadWidth[0];
			
			CCD2_info->RoadWidth[0] = CCD2_info->RightLine[0] -  CCD2_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					��		��		ƫ		��                              
*
*  ��������:myCCD_CCD3_GetLineError		
*  ����˵��:CCD3��Ѱ����,���ƫ��,ʶ�����ֵ�
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-17
*  ��    ע: ���ϴε��е������������͵�ƽ
*************************************************************************/	
void myCCD_CCD3_GetLineError(CCD_Info *CCD3_info,Speed_Info *Speed_info)
{
	int16_t CentralLinePixel_Now = CCD3_info->CentralLine[0];//���ϴε��е㿪ʼ����
	int16_t LinePixel_Temp = CentralLinePixel_Now;
	
	int16_t Left_temp=0, Right_temp=0;
	uint8_t ii = 0;
	
	//����ʮ�ֵ����
	CCD3_info->Cross_Flag= 0;
	CCD3_info->LeftLoseLine_Flag = 1;
	CCD3_info->RightLoseLine_Flag = 1;

	if ((CentralLinePixel_Now < 2 + CCD3_info->InvalidPixel_Num) && (CCD3_info->RightLine[0] > CCD3_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 2 + CCD3_info->InvalidPixel_Num;
	}
	else if ((CentralLinePixel_Now > 125 - CCD3_info->InvalidPixel_Num) && (CCD3_info->RightLine[0] > CCD3_info->LeftLine[0]))
	{
		CentralLinePixel_Now = 125 - CCD3_info->InvalidPixel_Num;
	}
	else if (CCD3_info->LeftLine[0] >= CCD3_info->RightLine[0])
	{
		CentralLinePixel_Now = Photo_Center;
		
		//���ұ߽�����쳣������һ�εı߽�ֵ������
		CCD3_info->LeftLine[0] = CCD3_info->LeftLine[1];
		CCD3_info->RightLine[0] = CCD3_info->RightLine[1];
	}
	
	if((1 == CCD1_info.Cross_Flag) || (1 == CCD2_info.Cross_Flag)
		|| (Ramp_Up == Road_condition) || (Ramp_Down == Road_condition)
		|| (Turn_Round == Car_state_Pre))//�µ���ʮ�ֵ�,���м�����������
	{
		LinePixel_Temp = 63;
	}
	else 
	{
		LinePixel_Temp = CentralLinePixel_Now;
	}
	
	/*------------------�����------------------------*/		
	ii = LinePixel_Temp;
	
	while(ii > CCD3_info->InvalidPixel_Num)
	{
		if(1 == CCD3_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
		{
			break;
		}
		
		ii--;
	}
	
	if(ii != CCD3_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
	{
		while (ii > CCD3_info->InvalidPixel_Num)
		{
			 if (0 == CCD3_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
			 {
				 Left_temp = ii ;
				 break;
			 }
			 else
			 {
				 ii--;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
			 }
		} 
	}

	//��߶���
	if (0 == Left_temp) 
	{
		Left_temp = CCD3_info->InvalidPixel_Num; 
	}	
			
    /*-------------------�ұ���------------------------*/	
	ii = LinePixel_Temp + 1;
		
	while(ii < 127 - CCD3_info->InvalidPixel_Num)
	{
		if(1 == CCD3_info->PixelBinary[ii])//�ѵ��ߵ�ƽ��1��
		{
			break;
		}
		
		ii++;
	}
	
	if(ii != 127 - CCD3_info->InvalidPixel_Num)//�Ѿ��������ߵ�ƽ
	{
		while (ii < (127 - CCD3_info->InvalidPixel_Num))
		{
			 if (0 == CCD3_info->PixelBinary[ii])//Ѱ 1 -> 0 ����
			 {
				 Right_temp = ii ;
				 break;
			 }
			 else
			 {
				 ii++;//ֻ�е�ǰ��Ķ�ֵ������Ϊ 1 ʱ�ż���������Ѱ�½��أ�����ֹͣ����
			 }
		}
	}

	//�ұ߶���
	if (0 == Right_temp) 
	{
		Right_temp = 127 - CCD3_info->InvalidPixel_Num;
	}	
	
	//------------------------------�ƶ�����----------------------------------------//
	for (ii = Line_SIZE - 1; ii > 0; ii--)
	{
		CCD3_info->CentralLine[ii] = CCD3_info->CentralLine[ii - 1];
		CCD3_info->LineError[ii] = CCD3_info->LineError[ii - 1];
		CCD3_info->LineError_D[ii] = CCD3_info->LineError_D[ii - 1];
	}
	
	if(1 == Speed_info->DistanceOf1Cm_Flag)
	{
		for (ii = Line_SIZE - 1; ii > 0; ii--)
		{
			CCD3_info->LeftLine[ii] = CCD3_info->LeftLine[ii - 1];
			CCD3_info->RightLine[ii] = CCD3_info->RightLine[ii - 1];
		}
	}
	
	//CCD3���߶������߲���Ϊ������Ч
	if ((CCD3_info->InvalidPixel_Num < Left_temp) && (Right_temp < 127 - CCD3_info->InvalidPixel_Num)
		&& ((Right_temp - Left_temp) >= ROAD_WIDETH_3_MIN) 
		&& ((Right_temp - Left_temp) <= ROAD_WIDETH_3_MAX))//����·��
	{
		CCD3_info->LeftLine[0] = Left_temp;
		CCD3_info->RightLine[0] = Right_temp;
		
		if ((1 == Speed_info->DistanceOf1Cm_Flag) && (CCD3_info->CCD_Ready_Num < CCD3_DataReady_Num))//1cm����,���1cm��־�Ǹ���ҡͷ���ƫ�����֮��
		{
			CCD3_info->CCD_Ready_Num++;//CCD3������Ч�����Լ�
		}
		
		CCD3_info->LeftLossLinePixel = Left_temp;
		CCD3_info->RightLossLinePixel = Right_temp;
		CCD3_info->LeftLoseLine_Flag = 0;
		CCD3_info->RightLoseLine_Flag = 0;
		
		CCD3_info->LossLine_Flag = 0;
	}
	else
	{
		CCD3_info->LossLine_Flag = 1;
		CCD3_info->CCD_Ready_Num = 0;//���CCD3������Ч����
		
		//-------------------------------�����ж����ֵ����----------------------------//
		//��߶���
		if((CCD3_info->InvalidPixel_Num >= Left_temp) && (Right_temp < 127 - CCD3_info->InvalidPixel_Num))
		{
			CCD3_info->RightLossLinePixel = Right_temp;
			CCD3_info->RightLoseLine_Flag = 0;
		}
		//�ұ߶���
		else if((CCD3_info->InvalidPixel_Num < Left_temp) && (Right_temp >= 127 - CCD3_info->InvalidPixel_Num))
		{
			CCD3_info->LeftLossLinePixel = Left_temp;
			CCD3_info->LeftLoseLine_Flag = 0;
		}
		//�ж��Ƿ����ʮ�ֵ�
		else if(((Left_temp <= CCD3_info->InvalidPixel_Num) && (Right_temp >= 127 - CCD3_info->InvalidPixel_Num))
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp])
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp - 1])
			&& (1 == CCD3_info->PixelBinary[LinePixel_Temp + 1]))//CCD3ȫ���߲���ȫ��
		{
			CCD3_info->Cross_Flag = 1;
		}
	}
	
	/*------------------------------���¶���--------------------------------*/
	if (0 == CCD3_info->LossLine_Flag)//CCD3������Ч
	{
		CCD3_info->CentralLine[0] = (CCD3_info->LeftLine[0] + CCD3_info->RightLine[0]) >> 1;	
		CCD3_info->LineError[0]   = CCD3_info->CentralLine[0] - CCD3_info->CCD_PhotoCenter;  
		CCD3_info->LineError_D[0] = CCD3_info->LineError[0] - CCD3_info->LineError[8]; 
		
		if(1 == Speed_info->DistanceOf1Cm_Flag)//1cm����һ��
		{
			CCD3_info->RoadWidth[9] = CCD3_info->RoadWidth[8];
			CCD3_info->RoadWidth[8] = CCD3_info->RoadWidth[7];
			CCD3_info->RoadWidth[7] = CCD3_info->RoadWidth[6];
			CCD3_info->RoadWidth[6] = CCD3_info->RoadWidth[5];
			CCD3_info->RoadWidth[5] = CCD3_info->RoadWidth[4];
			CCD3_info->RoadWidth[4] = CCD3_info->RoadWidth[3];
			CCD3_info->RoadWidth[3] = CCD3_info->RoadWidth[2];
			CCD3_info->RoadWidth[2] = CCD3_info->RoadWidth[1];
			CCD3_info->RoadWidth[1] = CCD3_info->RoadWidth[0];
			
			CCD3_info->RoadWidth[0]   = CCD3_info->RightLine[0] -  CCD3_info->LeftLine[0];
		}
	}
}

/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_CCD1_GetStartEndLine
*  ����˵��:CCD ʶ��ֱ����СS·��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-07-23
*  ��    ע��CCD2,CCD3�����������߲���CCD1����������Ѱ����������Ϊ������
*			��2�������غ�2���½���
*			��3�������غ�3���½���
*************************************************************************/
uint8_t myCCD_CCD1_GetStartEndLine(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Road_Condition *Road_condition,
									Car_State *Car_state
								  )
{
	uint8_t ii = CCD1_info->InvalidPixel_Num;//�������Ч�㿪ʼ���ұ���Ѱ������
	int16_t Up_1 = 0, Down_1 = 0, Up_2 = 0;
	
	//ֻ��CCD2��CCD3�����������߽磬�������ֵ��������µ�ʱ�ż��������
	if(    (0 == CCD2_info->AddLine_Flag)
		&& (0 == CCD2_info->Cross_Flag)
		&& (CCD2_info->CCD_Ready_Num >= 6)
		&& (CCD3_info->CCD_Ready_Num >= 16))
	{
		while(ii < (127 - CCD1_info->InvalidPixel_Num))
		{
			if(0 == CCD1_info->PixelBinary[ii])//��Ѱ���͵�ƽ
			{
				ii += 1;
				
				while(ii < (127 - CCD1_info->InvalidPixel_Num))
				{
					if(1 == CCD1_info->PixelBinary[ii])//��Ѱ��������1
					{
						Up_1 = ii;
						
						ii += 1;
						
						while(ii < (127 - CCD1_info->InvalidPixel_Num))
						{
							if(0 == CCD1_info->PixelBinary[ii])//��Ѱ���½���1
							{
								Down_1 = ii;
								
								ii += 1;
								
								while(ii < (127 - CCD1_info->InvalidPixel_Num))
								{
									if(1 == CCD1_info->PixelBinary[ii])//��Ѱ��������2
									{
										Up_2 = ii;
										
										ii += 1;
										
										while(ii < (127 - CCD1_info->InvalidPixel_Num))
										{
											if(0 == CCD1_info->PixelBinary[ii])//��Ѱ���½���2
											{
												if(((Down_1 - Up_1) >= StartEndLine_Pixel_MIN) && ((Down_1 - Up_1) <= StartEndLine_Pixel_MAX)
													&& ((Up_2 - Down_1) >= StartEndLine_Pixel_MIN) && ((Up_2 - Down_1) <- StartEndLine_Pixel_MAX))
												{
													return TRUE;
												}
												else
												{
													return TRUE;//��⵽������
												}
											}
											
											ii++;
										}
									
										return FALSE;
									}
									
									ii++;
								}
							
								return FALSE;
							}
							
							ii++;
						}
					
						return FALSE;
					}
					
					ii++;
				}
			
				return FALSE;
			}
			
			ii++;
		}
		
		return FALSE;//��⵽�����ߣ�ͣ��
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��                          
*
*  ��������:myCCD_CCD2_GetLeftIntoTurnRound
*  ����˵��:������߽����ֵ�
*  ����˵��:
*  ��������:	TRUE - ���������ֵ�
*				FALSE - δ���������ֵ�
*
*  �޸�ʱ��:2014-06-21
*  ��    ע:������������ұ�����Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD2_GetLeftIntoTurnRound(CCD_Info *CCD2_info)
{
	uint8 Up_1 = 0, Down_1 = 0, Up_2 = 0;
	uint8  LeftLinePixel_Now = 18;
	
	while (LeftLinePixel_Now < 111)
	{
		if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//�������͵�ƽ
		{
			LeftLinePixel_Now += 1;
			
			while (LeftLinePixel_Now < 111)//��������Up_1
			{
				if (1 == CCD2_info->PixelBinary[LeftLinePixel_Now])//������Up_1
				{
					Up_1 = LeftLinePixel_Now;	//�õ�Up_1
					LeftLinePixel_Now += 1;
					
					while (LeftLinePixel_Now < 111)//��������Down_1
					{
						if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//�õ�Down_1
						{
							Down_1 = LeftLinePixel_Now;
							LeftLinePixel_Now += 1;
							
							while (LeftLinePixel_Now < 111)//��������Up_2
							{
								if (1 == CCD2_info->PixelBinary[LeftLinePixel_Now])//�õ�Up_2
								{
									Up_2 = LeftLinePixel_Now;
									LeftLinePixel_Now += 1;
									
									while (LeftLinePixel_Now < 111)//��������Down_2
									{
										if (0 == CCD2_info->PixelBinary[LeftLinePixel_Now])//�õ�Down_2
										{
											//�ж��Ƿ������ֵ�
											if (((int)(Down_1 - Up_1) > TurnRound_Pixel_MIN2) && ((int)(Down_1 - Up_1) < TurnRound_Pixel_MAX2)
												&& ((int)(Up_2 - Down_1) > TurnRound_Pixel_MIN2) && ((int)(Up_2 - Down_1) < TurnRound_Pixel_MAX2))
											{
												return TRUE;//�õ����ֵ�
											}
											else
												return FALSE;//�õ���Up_1,Down_1,Up_2,Down_2,�����������ֵ�����
										}
										
										LeftLinePixel_Now++;
									}
									
									return FALSE;//ֻ�õ���Up_1,Down_1,Up_2
								}

								LeftLinePixel_Now++;
							}
							
							return FALSE;//ֻ�õ���Up_1,Down_1
						}
						
						LeftLinePixel_Now++;
					}
					
					return FALSE;//ֻ�õ���Up_1
				}
				
				LeftLinePixel_Now++;
			}//end of while(LeftLinePixel_Now < 119)
			
			return FALSE;//û��������Up_1,ȫΪ�͵�ƽ
		}//end of if(0 == CCD2_info->PixelBinary[LeftLinePixel_Now])

		LeftLinePixel_Now++;
	}
	
	return FALSE;//û���������͵�ƽ��ȫΪ�ߵ�ƽ
}

/*************************************************************************
*					��		��		ƫ		��                          
*
*  ��������:myCCD_CCD3_GetLeftLineIntoTurnRound
*  ����˵��:������߽����ֵ�
*  ����˵��:
*  ��������:	TRUE - ���������ֵ�
*				FALSE - δ���������ֵ�
*
*  �޸�ʱ��:2014-06-30
*  ��    ע:������������ұ�����Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD3_GetLeftIntoTurnRound(CCD_Info *CCD3_info)
{
	uint8 Up_1 = 0, Down_1 = 0, Up_2 = 0;
	uint8  LeftLinePixel_Now = 25;
	
	while (LeftLinePixel_Now < 103)
	{
		if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//�������͵�ƽ
		{
			LeftLinePixel_Now += 1;
			
			while (LeftLinePixel_Now < 103)//��������Up_1
			{
				if (1 == CCD3_info->PixelBinary[LeftLinePixel_Now])//������Up_1
				{
					Up_1 = LeftLinePixel_Now;	//�õ�Up_1
					LeftLinePixel_Now += 1;
					
					while (LeftLinePixel_Now < 103)//��������Down_1
					{
						if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//�õ�Down_1
						{
							Down_1 = LeftLinePixel_Now;
							LeftLinePixel_Now += 1;

							while (LeftLinePixel_Now < 103)//��������Up_2
							{
								if (1 == CCD3_info->PixelBinary[LeftLinePixel_Now])//�õ�Up_2
								{
									Up_2 = LeftLinePixel_Now;
									LeftLinePixel_Now += 1;
									
									while (LeftLinePixel_Now < 103)//��������Down_2
									{
										if (0 == CCD3_info->PixelBinary[LeftLinePixel_Now])//�õ�Down_2
										{
											//�ж��Ƿ������ֵ�
											if (((int)(Down_1 - Up_1) > TurnRound_Pixel_MIN3) && ((int)(Down_1 - Up_1) < TurnRound_Pixel_MAX3)
												&& ((int)(Up_2 - Down_1) > TurnRound_Pixel_MIN3) && ((int)(Up_2 - Down_1) < TurnRound_Pixel_MAX3))
											{
												return TRUE;//�õ����ֵ�
											}
											else
											{
												return FALSE;//�õ���Up_1,Down_1,Up_2,Down_2,�����������ֵ�����
											}
										}
										
										LeftLinePixel_Now++;
									}
									
									return FALSE;//ֻ�õ���Up_1,Down_1,Up_2
								}

								LeftLinePixel_Now++;
							}
							
							return FALSE;//ֻ�õ���Up_1,Down_1
						}
						
						LeftLinePixel_Now++;
					}
					
					return FALSE;//ֻ�õ���Up_1
				}
				
				LeftLinePixel_Now++;
			}//end of while(LeftLinePixel_Now < 119)
			
			return FALSE;//û��������Up_1,ȫΪ�͵�ƽ
		}//end of if(0 == CCD2_info->PixelBinary[LeftLinePixel_Now])

		LeftLinePixel_Now++;
	}
	
	return FALSE;//û���������͵�ƽ��ȫΪ�ߵ�ƽ
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myCCD_CCD2_GetLeftLineOutTurnRound	
*  ����˵������ȡ�����ֵ�ʱ���ܵ���߽��ߺ���
*  ����˵�����������ΪCCD_info 
*  �������أ��ұ߽��Ӧ���ع��
*  �޸�ʱ�䣺2014-08-05
*  ��    ע�� 	���������ֵ���ͷ�����ֵ���ʱ����Ѱ�ܵ���߽磬
*				������߿�ʼ����������ߣ���������������һ�����������أ�
*				һ�������½��أ��������������ز������������޶���Χ��ʱ
*				��Ϊ����߽磬���������һ�������غ�һ���½��ز����½���
*				��������֮��ľ���Ϊ�����ܵ��������Ϊ���ܵ�
*************************************************************************/
int16_t myCCD_CCD2_GetLeftLineOutTurnRound(CCD_Info *CCD_info)
{
  	uint8 LeftLinePixel_Old = CCD_info->InvalidPixel_Num;
	
	int LeftLinePixel_Now = CCD_info->InvalidPixel_Num;
	int RightLinePixel_Now = CCD_info->InvalidPixel_Num;
	
	//���ұ������͵�ƽ
	while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)
	{
		if (0 == CCD_info->PixelBinary[LeftLinePixel_Old])//�������͵�ƽ
		{
			LeftLinePixel_Old += 1;//�ƶ�����һ����
			
			while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)//�������ұ�����
			{
				if (1 == CCD_info->PixelBinary[LeftLinePixel_Old])//������������
				{
					LeftLinePixel_Now = LeftLinePixel_Old;
					LeftLinePixel_Old += 1;
					
					while (LeftLinePixel_Old < 127 - CCD_info->InvalidPixel_Num)//���������½���
					{
						if (0 == CCD_info->PixelBinary[LeftLinePixel_Old])//�������½���
						{
							RightLinePixel_Now = LeftLinePixel_Old;
							
							if(((int)(RightLinePixel_Now - LeftLinePixel_Now) > ROAD_WIDETH_1_MIN) && ((int)(RightLinePixel_Now - LeftLinePixel_Now) < ROAD_WIDETH_1_MAX))
							{
								return LeftLinePixel_Now;
							}
						}
						
						LeftLinePixel_Old++;//�������ұ�����
					}
					
					if (LeftLinePixel_Now > Photo_Center)//û���������½���,ֻ�ѵ���߽�
					{
						return LeftLinePixel_Now;
					}
					else
						return -1;
				}
			  
				LeftLinePixel_Old++;//�������ұ�����
			}
			
			return -1;//û���������ߵ�ƽ
		}
		else 
			return -1;
	}

	return -1;//û�еõ�������
}

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myCCD_CCD2_GetRightIntoTurnRound		
*  ����˵��:�����ұ߽������ֵ�
*  ����˵��:
*  ��������:	1 - ���������ֵ�
*
*  �޸�ʱ��:2014-06-21
*  ��    ע:�����ұ������������Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD2_GetRightIntoTurnRound(CCD_Info *CCD2_info)
{
	uint8 Up_1 = 127, Down_1 = 127, Up_2 = 127;	
	uint8  RightLinePixel_Now = 111;
	
	while (RightLinePixel_Now > 18)
	{
		if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//�������͵�ƽ
		{
			RightLinePixel_Now -= 1;
			
			while (RightLinePixel_Now > 18)//��������Up_1
			{
				if (1 == CCD2_info->PixelBinary[RightLinePixel_Now])//������Up_1
				{
					Up_1 = RightLinePixel_Now;	//�õ�Up_1
					RightLinePixel_Now -= 1;
					
					while (RightLinePixel_Now > 18)//��������Down_1
					{
						if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//�õ�Down_1
						{
							Down_1 = RightLinePixel_Now;
							RightLinePixel_Now -= 1;
							
							while (RightLinePixel_Now > 18)//��������Up_2
							{
								if (1 == CCD2_info->PixelBinary[RightLinePixel_Now])//�õ�Up_2
								{
									Up_2 = RightLinePixel_Now;
									RightLinePixel_Now -= 1;
									
									while (RightLinePixel_Now > 18)//��������Down_2
									{
										if (0 == CCD2_info->PixelBinary[RightLinePixel_Now])//�õ�Down_2
										{
											//�ж��Ƿ������ֵ�
											if (((int)(Up_1 - Down_1) > TurnRound_Pixel_MIN2) && ((int)(Up_1 - Down_1) < TurnRound_Pixel_MAX2)
												&& ((int)(Down_1 - Up_2) > TurnRound_Pixel_MIN2) && ((int)(Down_1 - Up_2) < TurnRound_Pixel_MAX2))
											{
												return TRUE;//�õ����ֵ�
											}
											else
											{
												return FALSE;//�õ���Up_1,Down_1,Up_2,Down_2,���ǲ��������ֵ�����
											}
										}
										
										RightLinePixel_Now--;
									}//end of while (RightLinePixel_Now > 10)
									
									return FALSE;//ֻ�õ���Up_1,Down_1,Up_2
								}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])

								RightLinePixel_Now--;
							}//end of while (RightLinePixel_Now > 10)
							
							return FALSE;//ֻ�õ���Up_1,Down_1

						}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])
						
						RightLinePixel_Now--;
					}//end of while (RightLinePixel_Now > 10)
					
					return FALSE;//ֻ�õ���Up_1
				}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])
				
				RightLinePixel_Now--;
			}//end of while (RightLinePixel_Now > 10)
			
			return FALSE;//û�еõ�Up_1ȫΪ�͵�ƽ
		}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])

		RightLinePixel_Now--;
	}//end of while (RightLinePixel_Now > 10)
	
	return FALSE;//û���������͵�ƽ
}

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myCCD_CCD3_GetRightIntoTurnRound		
*  ����˵��:�����ұ߽������ֵ�
*  ����˵��:
*  ��������:	1 - ���������ֵ�
*
*  �޸�ʱ��:2014-06-30
*  ��    ע:�����ұ������������Up_1,Down_1,Up_2,Down_2
*************************************************************************/	
int16_t myCCD_CCD3_GetRightIntoTurnRound(CCD_Info *CCD3_info)
{
	uint8 Up_1 = 127, Down_1 = 127, Up_2 = 127;	
	uint8  RightLinePixel_Now = 103;
	
	while (RightLinePixel_Now > 25)
	{
		if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//�������͵�ƽ
		{
			RightLinePixel_Now -= 1;
			
			while (RightLinePixel_Now > 25)//��������Up_1
			{
				if (1 == CCD3_info->PixelBinary[RightLinePixel_Now])//������Up_1
				{
					Up_1 = RightLinePixel_Now;	//�õ�Up_1
					RightLinePixel_Now -= 1;
					
					while (RightLinePixel_Now > 25)//��������Down_1
					{
						if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//�õ�Down_1
						{
							Down_1 = RightLinePixel_Now;
							RightLinePixel_Now -= 1;
							
							while (RightLinePixel_Now > 25)//��������Up_2
							{
								if (1 == CCD3_info->PixelBinary[RightLinePixel_Now])//�õ�Up_2
								{
									Up_2 = RightLinePixel_Now;
									RightLinePixel_Now -= 1;
									
									while (RightLinePixel_Now > 25)//��������Down_2
									{
										if (0 == CCD3_info->PixelBinary[RightLinePixel_Now])//�õ�Down_2
										{
											//�ж��Ƿ������ֵ�
											if (((int)(Up_1 - Down_1) > TurnRound_Pixel_MIN3) && ((int)(Up_1 - Down_1) < TurnRound_Pixel_MAX3)
												&& ((int)(Down_1 - Up_2) > TurnRound_Pixel_MIN3) && ((int)(Down_1 - Up_2) < TurnRound_Pixel_MAX3))
											{
												return TRUE;//�õ����ֵ�
											}
											else
											{
												return FALSE;//�õ���Up_1,Down_1,Up_2,Down_2,���ǲ��������ֵ�����
											}
										}
										
										RightLinePixel_Now--;
									}//end of while (RightLinePixel_Now > 10)
									
									return FALSE;//ֻ�õ���Up_1,Down_1,Up_2
								}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])

								RightLinePixel_Now--;
							}//end of while (RightLinePixel_Now > 10)
							
							return FALSE;//ֻ�õ���Up_1,Down_1

						}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])
						
						RightLinePixel_Now--;
					}//end of while (RightLinePixel_Now > 10)
					
					return FALSE;//ֻ�õ���Up_1
				}//end of if (1 == CCD_info->PixelBinary[RightLinePixel_Now])
				
				RightLinePixel_Now--;
			}//end of while (RightLinePixel_Now > 10)
			
			return FALSE;//û�еõ�Up_1ȫΪ�͵�ƽ
		}//end of if (0 == CCD_info->PixelBinary[RightLinePixel_Now])

		RightLinePixel_Now--;
	}//end of while (RightLinePixel_Now > 10)
	
	return FALSE;//û���������͵�ƽ
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myCCD_CCD2_GetRightLineOutTurnRound	(��ֵ��)
*  ����˵������ȡ�����ֵ�ʱ���ܵ��ұ߽��ߺ���
*  ����˵�����������ΪCCD_info 
*  �������أ��ұ߽��Ӧ���ع��
*  �޸�ʱ�䣺2014-08-05
*  ��    ע�� 	���������ֵ���ͷ�����ֵ���ʱ����Ѱ�ܵ��ұ߽磬
*				�����ұ߿�ʼ����������ߣ���������������һ�����������أ�
*				һ�������½��أ��������������ز������������޶���Χ��ʱ
*				��Ϊ���ұ߽磬���������һ�������غ�һ���½��ز����½���
*				��������֮��ľ���Ϊ�����ܵ��������Ϊ���ܵ�
*************************************************************************/
int16_t myCCD_CCD2_GetRightLineOutTurnRound(CCD_Info *CCD_info)
{
  	uint8 RightLinePixel_Old = 127 - CCD_info->InvalidPixel_Num;
	
	int LeftLinePixel_Now = CCD_info->InvalidPixel_Num;
	int RightLinePixel_Now = CCD_info->InvalidPixel_Num;
	
	//����������͵�ƽ
	while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)
	{
		if (0 == CCD_info->PixelBinary[RightLinePixel_Old])//�������͵�ƽ
		{
			RightLinePixel_Old -= 1;
			
			while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)
			{
				if (1 == CCD_info->PixelBinary[RightLinePixel_Old])//������������
				{
					RightLinePixel_Now = RightLinePixel_Old;
					RightLinePixel_Old -= 1;
					
					while (RightLinePixel_Old > CCD_info->InvalidPixel_Num)//���������½���
					{
						if (0 == CCD_info->PixelBinary[RightLinePixel_Old])//�������½���
						{
							LeftLinePixel_Now = RightLinePixel_Old;
							
							if (((int)(RightLinePixel_Now - LeftLinePixel_Now) >= ROAD_WIDETH_1_MIN) && ((int)(RightLinePixel_Now - LeftLinePixel_Now) <= ROAD_WIDETH_1_MAX))
							{
								return RightLinePixel_Now;
							}
						}
						
						RightLinePixel_Old--;//�������������
					}
					
					if (RightLinePixel_Now < Photo_Center)//û���������½���,ֻ�ѵ��ұ߽�
					{
						return RightLinePixel_Now;
					}
					else
						return -1;
				}
				  
				RightLinePixel_Old--;//�������������
			}
			
			return -1;//û���������ߵ�ƽ
		}
		else 
			return -1;
	}

	return -1;//û�еõ�������
}

/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_GetObstacle
*  ����˵��:CCD ʶ��·��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-07-14
*  ��    ע��CCD2·��ͻȻ��խ��Ȼ��CCD1·��Ҳ��խ�����Ҵ�ʱCCD1��CCD2��
*			һ�߽߱����������߽߱�Ϊ·�ϣ�������һ��Ϊ�����߽�
*************************************************************************/
uint8_t myCCD_GetObstacle(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Steer_Info *Steer_info
						)
{
	if((0 == CCD1_info->LossLine_Flag) 
		&& (0 == CCD2_info->LossLine_Flag) 
		&& (fabs(Steer_info->SteerPWM_Error[0]) < 600)
		&& (fabs(CCD1_info->LineError[0]) < 16))//CCD1,CCD2û�ж���,��ҡͷ��С
	{
		if((int16_t)(CCD2_info->RoadWidth[0] - CCD2_info->RoadWidth[6]) < -8)//CCD2·����ͻ��
		{
			if((CCD2_info->LeftLine[0] - CCD1_info->LeftLine[0] >= 6)
				&& (fabs(CCD1_info->RightLine[0] - CCD2_info->RightLine[0]) < 6))//CCD1��CCD2��߽���ͬ���ұ߽粻ͬ
			{
				return 1;//·�������
			}
			else if((CCD1_info->RightLine[0] - CCD2_info->RightLine[0] >= 6)
					&& (fabs(CCD2_info->LeftLine[0] - CCD1_info->LeftLine[0]) < 6))//CCD1��CCD2�ұ߽���ͬ����߽粻ͬ
			{
				return 2;//·�����ұ�
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_JudgeWheelOutOfWay
*  ����˵��:CCD1 �ж��������Ƿ�����
*  ����˵��:
*  ��������:����(TRUE)/������(FALSE)
*  �޸�ʱ��:2014-08-07
*  ��    ע������CCD1���ж�ʹ��3��ʱ��ǰƫ���Ƿ�ᵼ�����ӳ���
*************************************************************************/
uint8_t myCCD_JudgeWheelOutOfWay(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Speed_Info *Speed_info
								)
{
	if(TRUE == Parameter_info.WheelOutOfWay_Enable)
	{
		//��������Ƿ����
		if((CCD2_info->LineError[0] < 0)
			&& (CCD1_info->RightLine[0] < CAR_RIGHT_WHEEL_PIXEL))
		{
			CCD3_info->CCD_Ready_Num = 5;

			return TRUE;
		}
		//��������Ƿ����
		else if((CCD2_info->LineError[0] > 0)
				&& (CCD1_info->LeftLine[0] > CAR_LEFT_WHEEL_PIXEL))
		{
			CCD3_info->CCD_Ready_Num = 5;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��                          
*
*  ��������:myCCD_Get_60_Angle
*  ����˵��:�������ֵ�60�ȼ��
*  ����˵��:
*  ��������:	TRUE - ���������
*				FALSE - δ���������
*
*  �޸�ʱ��:2014-08-12
*  ��    ע:
*************************************************************************/	
void myCCD_Get_60_Angle(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Speed_Info *Speed_info,Parameter_Info *Parameter_info,Car_State *Car_state)
{
	uint8_t ii = 0,LineTemp = 63;
	
	int16_t Up = 0,Down = 0;
	
	int32_t LeftOrRightLine_Sum = 0;
	int32_t LeftOrRightLine_Ava = 0;
	
	if(0 == CCD3_info->LossLine_Flag)
	{
		LineTemp = CCD3_info->CentralLine[0];
	}
	
	ii = LineTemp;
	
	if((1 == CCD3_info->PixelBinary[ii]) 
		&& (0 == CCD1_info->LeftLoseLine_Flag)
		&& (0 == CCD1_info->RightLoseLine_Flag)
		&& (0 == CCD2_info->LeftLoseLine_Flag)
		&& (0 == CCD2_info->RightLoseLine_Flag))
	{
		//�������������
		while(ii > CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])//�������½���
			{
				Down = ii;
				
				break;
			}
				
			ii--;
		}
		
		if(ii > CCD3_info->InvalidPixel_Num)//�Ѿ��������½���
		{
			ii -= 1;
				
			while(ii > CCD3_info->InvalidPixel_Num)
			{
				if(1 == CCD3_info->PixelBinary[ii])//������������
				{
					Up = ii;
					
					if((Down - Up) < 40)
					{
						CCD3_info->Angle_LeftFifo[0] = Down - Up;//���½Ƕȶ���
					}
					
					break;
				}
				
				ii--;
			}
		}
		
		//���ұ���������
		ii = LineTemp;
		
		while(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			if(0 == CCD3_info->PixelBinary[ii])//�������½���
			{
				Down = ii;
				
				break;
			}
				
			ii++;
		}
		
		if(ii < 127 - CCD3_info->InvalidPixel_Num)
		{
			ii += 1;
				
			while(ii < 127 - CCD3_info->InvalidPixel_Num)
			{
				if(1 == CCD3_info->PixelBinary[ii])//������������
				{
					Up = ii;
					
					if((Up - Down) < 40)
					{
						CCD3_info->Angle_RightFifo[0] = Up - Down;//���½Ƕȶ���
					}
					
					break;
				}
				
				ii++;
			}
		}	
	}

	//�ж�60�ȼ��
	if((1 == CCD3_info->LossLine_Flag) 
		&& (0 == Parameter_info->Left60Angle_Flag) 
		&& (0 == Parameter_info->Right60Angle_Flag))
	{
		//�ж���߼��
		if((1 == CCD3_info->LeftLoseLine_Flag) && (0 == CCD3_info->RightLoseLine_Flag))
		{
			for(ii = 0; ii < 3; ii++)
			{
				if((CCD3_info->Angle_LeftFifo[ii + 3] - CCD3_info->Angle_LeftFifo[ii]) <= 0)
				{
					break;
				}
			}
			
			if(3 == ii)
			{
				//------------------------�ж���߽��Ƿ���̫��仯-----------------------//
				LeftOrRightLine_Sum = CCD3_info->LeftLine[0] + CCD3_info->LeftLine[1]
									+ CCD3_info->LeftLine[2] + CCD3_info->LeftLine[3]
									+ CCD3_info->LeftLine[4] + CCD3_info->LeftLine[5];
				
				//����߽�ƽ��ֵ
				LeftOrRightLine_Ava = LeftOrRightLine_Sum / 6;
				
				//����߽緽��
				LeftOrRightLine_Sum = (CCD3_info->LeftLine[0] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[0] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[1] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[1] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[2] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[2] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[3] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[3] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[4] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[4] - LeftOrRightLine_Ava)
									+ (CCD3_info->LeftLine[5] - LeftOrRightLine_Ava)*(CCD3_info->LeftLine[5] - LeftOrRightLine_Ava);
				
				//�ж���߽緽���Ƿ���������Χ��
				if(LeftOrRightLine_Sum < Parameter_info->Angle60_MeanValue)
				{
					//�жϼ�ǳɹ�
					Parameter_info->Left60Angle_Flag = 1;
					Parameter_info->Right60Angle_Flag = 0;
					
					Speed_info->DistanceOf60Angle_Counter = 80;
					
					LED5_ON;
				}
			}
		}
		//�ж��ұ߼��
		else if((1 == CCD3_info->RightLoseLine_Flag) && (0 == CCD3_info->LeftLoseLine_Flag))
		{
			for(ii = 0; ii < 3; ii++)
			{
				if((CCD3_info->Angle_RightFifo[ii + 3] - CCD3_info->Angle_RightFifo[ii]) <= 0)
				{
					break;
				}
			}
			
			if(3 == ii)
			{
				//------------------------�ж��ұ߽��Ƿ���̫��仯-----------------------//
				LeftOrRightLine_Sum = CCD3_info->RightLine[0] + CCD3_info->RightLine[1]
									+ CCD3_info->RightLine[2] + CCD3_info->RightLine[3]
									+ CCD3_info->RightLine[4] + CCD3_info->RightLine[5];
				
				//���ұ߽�ƽ��ֵ
				LeftOrRightLine_Ava = LeftOrRightLine_Sum / 6;
				
				//���ұ߽緽��
				LeftOrRightLine_Sum = (CCD3_info->RightLine[0] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[0] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[1] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[1] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[2] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[2] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[3] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[3] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[4] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[4] - LeftOrRightLine_Ava)
									+ (CCD3_info->RightLine[5] - LeftOrRightLine_Ava)*(CCD3_info->RightLine[5] - LeftOrRightLine_Ava);
				
				//�ж��ұ߽緽���Ƿ���������Χ��
				if(LeftOrRightLine_Sum < Parameter_info->Angle60_MeanValue)
				{
					//�жϼ�ǳɹ�
					Parameter_info->Left60Angle_Flag = 0;
					Parameter_info->Right60Angle_Flag = 1;
					
					Speed_info->DistanceOf60Angle_Counter = 80;
					
					LED5_ON;
				}
			}
		}
	}
}

/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_GetRoadCondition
*  ����˵��:�����������߷�ʽ�����������ж�
*  ����˵��:
*  ��������:��ǰ��������
*  �޸�ʱ��:2014-08-13
*  ��    ע���÷��������������߷�ʽ���жϵ�ǰ���������ͣ��Ǵ�ͼ��ֱ��
*			��õ��������͡�
*************************************************************************/
#ifdef GetRoadCondition_Enable

Road_Condition myCCD_GetRoadCondition(	CCD_Info *CCD1_info,
										CCD_Info *CCD2_info,
										CCD_Info *CCD3_info,
										Speed_Info *Speed_info
									 )
{
	uint8_t ii = 0;
	
	//����CCD1�����ұ߽�����ĵ�
	CCD1_info->Rn_Left[0] = CCD1_info->LeftLine[0];
	CCD1_info->Rn_Right[0] = CCD1_info->RightLine[0];
	
	//����CCD2�����ұ߽�����ĵ�
	CCD2_info->Rn_Left[0] = CCD2_info->LeftLine[0];
	CCD2_info->Rn_Right[0] = CCD2_info->RightLine[0];
	
	
	
	return NeedJudge;
}

#endif
/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_GPIO_Init(void)
*  ����˵��:CCD IO��ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:
*  ��    ע��
*************************************************************************/	
void myCCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//CCD1_CK	B16
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_16;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD1_SI  	B11	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
		
	//CCD2_CK	B8
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD2_SI  	B7	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
		
	//CCD3_CK	B5
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);
	
	//CCD3_SI  	B4	
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	LDPY_GPIO_Init(PORTB,&GPIO_InitStructure);	
}
/*************************************************************************
*					��		��		ƫ		��                                
*
*  ��������:myCCD_CCD1_Sampling
*  ����˵��:CCD1����
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:
*  ��    ע: 
*************************************************************************/		
void myCCD_CCD1_Sampling(CCD_Info *CCD_info)
{
	uint8_t i;

	CCD1_SI=1;						//CCD��λ
	LDPY_Delay_10ns(3);
	CCD1_CK=1;						//
	LDPY_Delay_10ns(20);
	CCD1_SI=0;	
	LDPY_Delay_10ns(20);

	LDPY_Delay_10ns(3000);				//��ʱ�Ա�֤ǰ������ƽ��
	
	for (i = 0; i < 128; i++)
	{
		 
		CCD1_CK=1;							//�½���AO�����ѹֵ
		LDPY_Delay_10ns(5);
		 
		CCD_info->PixelOri[0][i] = LDPY_ADC1_GetValue(1); 	//ADC����

		CCD1_CK=0;
		LDPY_Delay_10ns(5);
	}
	 
	CCD1_CK=1;
	LDPY_Delay_10ns(20);
	CCD1_CK=0;
	LDPY_Delay_10ns(20);
}
/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:myCCD_CCD2_Sampling
*  ����˵��:CCD2����
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:
*  ��    ע: 
*************************************************************************/		
void myCCD_CCD2_Sampling(CCD_Info *CCD_info)
{
	uint8_t i;
	
	CCD2_SI=1;						//CCD��λ
	LDPY_Delay_10ns(3);
	CCD2_CK=1;						
	LDPY_Delay_10ns(20);
	CCD2_SI=0;	
	LDPY_Delay_10ns(20);
	
	LDPY_Delay_10ns(3000);				//��ʱ�Ա�֤ǰ������ƽ��
	
	for (i = 0; i < 128; i++)
	{
		 
		CCD2_CK=1;						//�½���AO�����ѹֵ
		LDPY_Delay_10ns(5);
		 
		CCD_info->PixelOri[0][i] = LDPY_ADC1_GetValue(2); //ADC����

		CCD2_CK=0;
		LDPY_Delay_10ns(5);
	}
	 
	CCD2_CK=1;
	LDPY_Delay_10ns(20);
	CCD2_CK=0;
	LDPY_Delay_10ns(20);
}	

/*************************************************************************
*					��		��		ƫ		��                             
*
*  ��������:CCD3_Sampling
*  ����˵��:CCD3����
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:
*  ��    ע: 
*************************************************************************/		
	void myCCD_CCD3_Sampling(CCD_Info *CCD_info)
	{
		uint8_t ii;
		
		CCD3_SI=1;						//CCD��λ
		LDPY_Delay_10ns(3);
		CCD3_CK=1;						//
		LDPY_Delay_10ns(20);
		CCD3_SI=0;	
		LDPY_Delay_10ns(20);
		
		LDPY_Delay_10ns(3000);				//��ʱ�Ա�֤ǰ������ƽ��
		
		for (ii = 0; ii < 128; ii++)
		 {
			 
			 CCD3_CK=1;				//�½���AO�����ѹֵ
			 LDPY_Delay_10ns(10);
			 
			 CCD_info->PixelOri[0][ii] = LDPY_ADC0_GetValue(3); //ADC����

			 CCD3_CK=0;
			 LDPY_Delay_10ns(10);
		 }
		 CCD3_CK=1;
		 LDPY_Delay_10ns(20);
		 CCD3_CK=0;
		 LDPY_Delay_10ns(20);
	}	

/*************************************************************************
*					��		��		ƫ		��                               
*
*  ��������:myCCD_Filter
*  ����˵��:CCD�����˲��Ͷ�ֵ��
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-05-21
*  ��    ע:��������ADֵԼΪ1000�����Ŵ��ʱ��Ϊ�� 2000���� 400
*************************************************************************/		
void myCCD_FilterAndBinarization(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Parameter_Info *Parameter_info)
{
	uint8 ii = 0;
	float CentralLine_ExposureVoltage = 0;
	uint32 AD1_MAX = 0;
	uint32 AD1_MIN = 4095;
	uint32 AD_MultK = 16;
	
	uint16_t BinaryEdgeValue = 800;

	//����CCD���ݲɼ��˲�
	for (ii = 0; ii < 128; ii++)//60us
	{
		CCD1_info->Pixel[ii] = (CCD1_info->PixelOri[0][ii] + CCD1_info->PixelOri[1][ii]) >> 1;			
		CCD2_info->Pixel[ii] = (CCD2_info->PixelOri[0][ii] + CCD2_info->PixelOri[1][ii]) >> 1;
		CCD3_info->Pixel[ii] = (CCD3_info->PixelOri[0][ii] + CCD3_info->PixelOri[1][ii]) >> 1;
		
		//�ƶ�CCD1ԭʼ���ݶ���
		CCD1_info->PixelOri[1][ii] = CCD1_info->PixelOri[0][ii];
		
		//�ƶ�CCD2ԭʼ���ݶ���
		CCD2_info->PixelOri[1][ii] = CCD2_info->PixelOri[0][ii];
		
		//�ƶ�CCD3ԭʼ���ݶ���
		CCD3_info->PixelOri[1][ii] = CCD3_info->PixelOri[0][ii];
		
		//ȥCCD1������
		if (CCD1_info->Pixel[ii] > 1000)
		{
		   CCD1_info->Pixel[ii] = CCD1_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD1_info->Pixel[ii] = 0;
		}
		
		//ȥCCD2������
	    if (CCD2_info->Pixel[ii] > 1000)
		{
		   CCD2_info->Pixel[ii] = CCD2_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD2_info->Pixel[ii] = 0;
		}
		
		//ȥCCD3������
	    if (CCD3_info->Pixel[ii] > 1000)
		{
		   CCD3_info->Pixel[ii] = CCD3_info->Pixel[ii] - 1000;
		}
	    else
		{
		   CCD3_info->Pixel[ii] = 0;
		}
	}

	//����ϴε����ߵ�������Χ���Ҳ��������ֵ���ʮ�ֵ����µ�������������ж�̬�Ŵ�
	if ((Turn_Round != Car_state)
		&& ((Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
		&& (1 != CCD1_info->Cross_Flag)
		&& (CCD1_info->CentralLine[0] > 50) && (CCD1_info->CentralLine[0] < 76))
	{
		//����һ�εķŴ���ԭʼֵ������Ŵ���
		CentralLine_ExposureVoltage = 	(	CCD1_info->Pixel[CCD1_info->CentralLine[0]]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] - 1]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] - 2]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] + 1]
										  +	CCD1_info->Pixel[CCD1_info->CentralLine[0] + 2]
										)/5;
		
		Parameter_info->AD_MultK_f = Parameter_info->CCD1_AD_TargetValue / CentralLine_ExposureVoltage;
	}
	
	//��ֹ�Ŵ�����С�͹���(1400/3096 = 0.45,1400/400 = 3.5)
	if (Parameter_info->AD_MultK_f >  3.5)
	{
		Parameter_info->AD_MultK_f = 3.5;
	}
	else if (Parameter_info->AD_MultK_f < 0.45)
	{
		Parameter_info->AD_MultK_f = 0.45;
	}
	
	//�ѷŴ����Ŵ�Ϊ��������
	AD_MultK = (uint32_t)(Parameter_info->AD_MultK_f*16);

	//����CCD1 CCD2 CCD3���ݷŴ�
	for (ii = 0; ii < 128; ii++)//80us
	{		
		CCD1_info->Pixel[ii] = (CCD1_info->Pixel[ii] >> 4)*AD_MultK;//��CCD1���зŴ�
		CCD2_info->Pixel[ii] = (CCD2_info->Pixel[ii] >> 4)*AD_MultK;//��CCD2���зŴ�
		CCD3_info->Pixel[ii] = (CCD3_info->Pixel[ii] >> 4)*AD_MultK;//��CCD3���зŴ�
		
		//��CCD1�Ŵ���ADֵ�����޷�
		if (CCD1_info->Pixel[ii] > 4095)
		{
			CCD1_info->Pixel[ii] = 4095;
		}
		
		//��CCD2�Ŵ���ADֵ�����޷�
		if (CCD2_info->Pixel[ii] > 4095)
		{
			CCD2_info->Pixel[ii] = 4095;
		}
		
		//��CCD3�Ŵ���ADֵ�����޷�
		if (CCD3_info->Pixel[ii] > 4095)
		{
			CCD3_info->Pixel[ii] = 4095;
		}
		
		if ((ii >= 16) && (ii <= 112))
		{
			if (AD1_MAX < CCD1_info->Pixel[ii])		//�ҳ�CCD1�Ŵ���AD���ֵ
			{
				AD1_MAX = CCD1_info->Pixel[ii];
			}
			
			if (AD1_MIN > CCD1_info->Pixel[ii])		//�ҳ�CCD1�Ŵ���AD��Сֵ
			{
				AD1_MIN = CCD1_info->Pixel[ii];
			}
		}
	}

	//����AD_MAX,AD_MIN����
	CCD1_info->AD_MAX[0] = AD1_MAX;
	CCD1_info->AD_MIN[0] = AD1_MIN;

	//����CCD1��ֵ���ķֽ�ֵ
	BinaryEdgeValue = (		CCD1_info->AD_MAX[3] + CCD1_info->AD_MAX[2] + CCD1_info->AD_MAX[1] + CCD1_info->AD_MAX[0]
						  + CCD1_info->AD_MIN[3] + CCD1_info->AD_MIN[2] + CCD1_info->AD_MIN[1] + CCD1_info->AD_MIN[0]
					  ) >> 3;

	if(CheckCCD == Car_mode)
	{
		myOLED_Dec(4,90,BinaryEdgeValue);
	}
	
	//���ƶ�ֵ���߽�ֵ�����ֵ��Сֵ(��ֵ��750����)
	if (BinaryEdgeValue < Parameter_info->CCD1_Binary_MIN)
	{
		BinaryEdgeValue = Parameter_info->CCD1_Binary_MIN;
	}
	else if (BinaryEdgeValue > Parameter_info->CCD1_Binary_MAX)
	{
		BinaryEdgeValue = Parameter_info->CCD1_Binary_MAX;
	}
	
	//CCD���ݵĶ�ֵ��
	for (ii = 0; ii < 128; ii++)
	{
		CCD1_info->PixelBinary[ii] = (CCD1_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD1���ݶ�ֵ��
		CCD2_info->PixelBinary[ii] = (CCD2_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD2���ݶ�ֵ��
		CCD3_info->PixelBinary[ii] = (CCD3_info->Pixel[ii] >= BinaryEdgeValue) ? 1 : 0;//CCD3���ݶ�ֵ��
	}
}

/*************************************************************************
*					��		��		ƫ		��                            
*
*  ��������:myCCD_FilterAtFirst
*  ����˵��:CCD����ͷ�����ƶ�
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-21
*  ��    ע:
*************************************************************************/	
void myCCD_FilterAtFirst(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	int16_t ii = 0;
	
	//��ʼ���ƶ�CCD��������
	for (ii = 0; ii < 128; ii++)
	{
		//�ƶ�CCD1ԭʼ���ݶ���
		CCD1_info->PixelOri[1][ii] = CCD1_info->PixelOri[0][ii];
		
		//�ƶ�CCD2ԭʼ���ݶ���
		CCD2_info->PixelOri[1][ii] = CCD2_info->PixelOri[0][ii];
		
		//�ƶ�CCD3ԭʼ���ݶ���
		CCD3_info->PixelOri[1][ii] = CCD3_info->PixelOri[0][ii];
	}
}

/*************************************************************************
*					��		��		ƫ		��                           
*
*  ��������:myCCD_GetBeginLineError
*  ����˵��:CCD����ͷ���βɼ�,������ƶ�
*  ����˵��:
*  ��������:��
*  �޸�ʱ��:2014-06-26
*  ��    ע:
*************************************************************************/	
void myCCD_GetBeginLineError(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info
							)
{
	uint8_t ii = 0;
	uint32_t Gyro_Sum = 0;
	
	//CCD���ݵ�ͷ�����˲�����
	ii = 5;
	
	do
	{
		if (1 == Parameter_info->CCD_SampleOK_Flag)
		{
			Parameter_info->CCD_SampleOK_Flag = 0;
			
			myCCD_FilterAtFirst(CCD1_info,CCD2_info,CCD3_info);
			
			ii--;
		}
	}while(ii > 0);
	
	//����ʱ��ƫ�������ƶ�
	ii = Line_SIZE;
	
	do
	{
		if (1 == Parameter_info->CCD_SampleOK_Flag)
		{
			Parameter_info->CCD_SampleOK_Flag = 0;
			
			//�����˲� ��ֵ��
			myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);

			//CCD�ѱ��ߺ�ƫ�����
			myCCD_CCD1_GetLineError(CCD1_info,Speed_info);
			myCCD_CCD2_GetLineError(CCD1_info,CCD2_info,Speed_info);
			myCCD_CCD3_GetLineError(CCD3_info,Speed_info);
			
			ii--;
		}
	}while(ii > 0);

	CCD1_info->RoadWidthOfStraight = CCD1_info->RightLine[0] - CCD1_info->LeftLine[0];
	CCD2_info->RoadWidthOfStraight = CCD2_info->RightLine[0] - CCD2_info->LeftLine[0];
	CCD3_info->RoadWidthOfStraight = CCD3_info->RightLine[0] - CCD3_info->LeftLine[0];
	
	//�趨��ʼ����λ
	if((0 == CCD1_info->LossLine_Flag) && (0 == CCD2_info->LossLine_Flag))
	{
		CCD1_info->CCD_Ready_Num = CCD1_DataReady_Num;
		CCD2_info->CCD_Ready_Num = CCD2_DataReady_Num;
	}
	
	Speed_info->SpeedMode = 2;//��ʼ��Ϊ2��
	
	//�ɼ����������
	for(ii = 0; ii < GYRO_LENGTH; ii++)//�����ɼ�16����ƽ��ֵ
	{
		Gyro_info.Gyroscope_Fifo[ii] = LDPY_ADC1_ENC();
		
		Gyro_Sum += Gyro_info.Gyroscope_Fifo[ii];
	}
	
	//�������
	Gyro_info.GyroscopeZero = Gyro_Sum >> 4;
}

/*---------------------------end of myCCD.c-----------------------------*/
