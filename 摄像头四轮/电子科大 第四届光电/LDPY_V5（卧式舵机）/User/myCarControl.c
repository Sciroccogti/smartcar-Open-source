
#include "myCarControl.h"

extern DMA_InitTypeDef dma_init_struct;

extern uint16_t StopType;
extern Car_Mode Car_mode;

#ifdef RemRoad_Control_Enable
	
	extern RemSpeedUpDown_Info RemSpeedUpDown_info;
	
#endif

#ifdef DMA_CH2_SendLabVIEW_Info_Enable
	
	extern LabVIEW_Info LabVIEW_info;
	
#endif

	extern DMA_Required_Variable DMA_Variable_1;

/*************************************************************************
*					��		��		ƫ		��   
*  ��������: myCarControl_CarControl
*  ����˵��: С���ۺϿ��ƺ���
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-07-04
*  ��	ע:
*************************************************************************/
void myCarControl_CarControl(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								HeadSteer_Info *HeadSteer_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info,
								RemSpeedUpDown_Info *RemSpeedUpDown_info,
								Road_Condition *Road_condition,
								Car_State *Car_state,
								Car_State *Car_state_Pre
							)
{	
	Bell_OFF;

	//�����������ٶȺ;���
	myMotor_UpdateEncoder(CCD1_info,CCD2_info,Motor_info,Speed_info,Parameter_info,RemSpeedUpDown_info,Road_condition,Car_state,Car_state_Pre);
	
	//����ƫ��ͼ����ٶ�
	switch(*Car_state)
	{
		case Go_Forward://����ǰ��
			{
				//����ƫ��
				myCCD_DataHandle(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info,Road_condition,Car_state,Car_state_Pre);
				
				//����Ŀ���ٶ�
				switch(*Road_condition)//���ݲ�ͬ���������͸�����ͬ�ٶ�
				{
					case Ramp_Up://����
						{
							Speed_info->SpeedMode = 1;//���µ�ʹ��1��
							Speed_info->TargetSpeed[0] = Speed_info->RampUp_Speed;
						}
						break;
					case Ramp_Down://����
						{
							if(3 == Speed_info->SpeedMode)
							{
								Speed_info->SpeedMode = 2;
							}
							
							Speed_info->TargetSpeed[0] = Speed_info->RampDown_Speed;
						}
						break;
					case Obstacle://·��
						{
							if(3 == Speed_info->SpeedMode)//�����3����Ϊ2��
							{
								Speed_info->LastSpeedMode = Speed_info->SpeedMode;
								Speed_info->SpeedMode = 2;
							}
							
							//·��ʱͼ�����ĵ�ƫ��
							if(1 == Parameter_info->LeftOrRightObstacle_Flag)//���·��
							{
								CCD2_info->CCD_PhotoCenter = 63 - CCD2_info->CCD_ObstacleShift;//����ͼ�����ĵ�
							}
							else if(2 == Parameter_info->LeftOrRightObstacle_Flag)//�ұ�·��
							{
								CCD2_info->CCD_PhotoCenter = 63 + CCD2_info->CCD_ObstacleShift;//����ͼ�����ĵ�
							}
						
							Speed_info->TargetSpeed[0] = Speed_info->Obstacle_Speed;
							
							Bell_ON;
						}
						break;
					default://������������(�������ֵ�)
						{
							myMotor_CalculateTargetSpeed(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Speed_info);

						}
						break;
				}
			}
			break;//end of Go_Forward
			
		case Turn_Round://��ͷ
			{
				Bell_ON;
				
				Parameter_info->Left60Angle_Flag = 0;
				Parameter_info->Right60Angle_Flag = 0;
				
				//����������ֵ�ǰΪ����ģʽ���������ֵ�֮������ñ��
				if(1 == Parameter_info->SlowBeforeTurnRound_Flag)
				{
					Parameter_info->SlowBeforeTurnRound_Flag = 0;
				}
				
				if(3 == Parameter_info->CCD2OrCCD3_GetTurnRound)
				{
					LED4_ON;
				}
				else if(2 == Parameter_info->CCD2OrCCD3_GetTurnRound)
				{
					LED3_ON;
				}
				
				Speed_info->SpeedMode = 2;//Ĭ��ʹ��CCD2
				
				CCD1_info->CCD_Ready_Num = 0;
				CCD2_info->CCD_Ready_Num = 0;
				CCD3_info->CCD_Ready_Num = 0;
				
				Parameter_info->GetRamp_Enable = FALSE;//ȷ�����ֵ���ʶ���µ�
				
				//CCD�����˲��Ͷ�ֵ��
				myCCD_FilterAndBinarization(CCD1_info,CCD2_info,CCD3_info,Parameter_info);
				
				//���ݴ�ʱ���ٶȼ��㵽��Ƕ��پ���ת����׼ȷ�����ֵ�
				myCarControl_CalDistanceToTurnRound(Speed_info);
				
				if(1 == Parameter_info->LeftIntoTurnRound_Flag)//����������ֵ�
				{
					//���ﵽת�����(����CCD2ʶ�����ֵ�)ʱ��ת��
					if((Speed_info->DistanceOfTurnRound_Counter >= Speed_info->DistanceBeginTurn) || (2 == Parameter_info->CCD2OrCCD3_GetTurnRound))
					{
						CCD2_info->LineError[0] = 20;//ǿ��ҡͷ���Ҽ���
						
						HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_RightMAX;
					}
					
					if((HeadSteer_info->SteerPWM_Error[0] > 600) &&(Speed_info->DistanceOfTurnRound_Counter > 60))//60cm��ʼѲ��
					{
						if(-1 != myCCD_CCD2_GetLeftLineOutTurnRound(CCD2_info))//�Ѿ���Ѱ����߽�
						{
							*Car_state = Go_Forward;
							*Car_state_Pre = Turn_Round;
							
							CCD2_info->LeftLine[0] = myCCD_CCD2_GetLeftLineOutTurnRound(CCD2_info);
							CCD2_info->RightLine[0] = CCD2_info->LeftLine[0] + CCD2_info->RoadWidth[0];
							CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0]) >> 1;
							
							//�������ֵ����
							Parameter_info->LeftIntoTurnRound_Flag = 0;
							Parameter_info->RightIntoTurnRound_Flag = 0;
						}
					}
				}
				else if(1 == Parameter_info->RightIntoTurnRound_Flag)//���ұ������ֵ�
				{
					//���ﵽת�����(����CCD2ʶ�����ֵ�)ʱ��ת��
					if((Speed_info->DistanceOfTurnRound_Counter >= Speed_info->DistanceBeginTurn) || (2 == Parameter_info->CCD2OrCCD3_GetTurnRound))
					{
						CCD2_info->LineError[0] = -20;//ǿ��ҡͷ������
						
						HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_LeftMAX;
					}
					
					if((HeadSteer_info->SteerPWM_Error[0] < -600) && (Speed_info->DistanceOfTurnRound_Counter > 60))//60cm��ʼѲ��
					{
						if(-1 != myCCD_CCD2_GetRightLineOutTurnRound(CCD2_info))//�Ѿ���Ѱ���ұ߽�
						{
							*Car_state = Go_Forward;
							*Car_state_Pre = Turn_Round;
							
							CCD2_info->RightLine[0] = myCCD_CCD2_GetRightLineOutTurnRound(CCD2_info);
							CCD2_info->LeftLine[0] = CCD2_info->RightLine[0] - CCD2_info->RoadWidth[0];
							CCD2_info->CentralLine[0] = (CCD2_info->LeftLine[0] + CCD2_info->RightLine[0]) >> 1;
							
							//�������ֵ����
							Parameter_info->LeftIntoTurnRound_Flag = 0;
							Parameter_info->RightIntoTurnRound_Flag = 0;
						}
					}
				}		
	
				//�趨���ֵ�ͷ·���ٶ�
				Speed_info->TargetSpeed[0] = Speed_info->TurnRound_Speed;//�趨Ϊת���ٶ�
			}
			break;//end of Turn_Round
			
		case Stop://ͣ��
			{
				if(3 == Speed_info->SpeedMode)
				{
					Speed_info->SpeedMode = 2;//ͣ��ʹ��2��������У��
				}
				
				Steer_info->HeadToSteer_2 = 0.6;
				Steer_info->KP2 = 6;
				
				Parameter_info->GetTurnRound_Enable = FALSE;//ͣ���Ժ󲻼�����ֵ�

				if(	   (0 == Speed_info->SpeedFifo[0]) 
					&& (0 == Speed_info->SpeedFifo[1])
					&& (0 == Speed_info->SpeedFifo[2]))//ͣ����ɺ���ʾ�������
				{
					myOLED_Dec(6,10,StopType);//��ʾ�����߼��ͣ��ʱ����ܵı��
					
					myOLED_Dec(5,10,Parameter_info->RoadDistance);//��ʾ��ʻ����

					myOLED_Decimals(4,10,Parameter_info->Time);//��ʾ��ʻʱ��
				}
				
				//����ƫ��
				myCCD_DataHandle(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Parameter_info,Road_condition,Car_state,Car_state_Pre);
				
				//Ŀ���ٶȸ���
				Speed_info->TargetSpeed[0] = 0;
				
				Parameter_info->StartEndLine_Flag = 1;//��λ�����߼�⵽���,�൱�ڼ�⵽������
			}
			break;//end of Stop
			
		default :
			break;
		
	}//end of switch(*Car_state)

	//���ǵ���ģʽ�ſ��ƶ���͵��
	if((CheckCCD != Car_mode) && (CheckSL != Car_mode))
	{
		//����ҡͷ���
		mySteer_HeadSteerControl(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Speed_info,Car_state,Car_state_Pre);
		
		//����ת����
		mySteer_DirectionControl(CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Speed_info);
		
		//���Ƶ��
		if(CheckRamp != Car_mode)
		{
			myMotor_Control(Motor_info,Speed_info,Parameter_info);
		}
	}
	
#ifdef DMA_CH0_Enable//��DMA������CCDͼ��
	
	LDPY_DMA_SendThreeCCDsImage(&DMA_Variable_1,CCD1_info,CCD2_info,CCD3_info);
	
	DMA0->ERQ |= (1 << DMA_CH0);
	
#endif
//end of DMA_CH0_Enable
	
//������ʱ��������λ��
#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		
	LDPY_DMA_SendTempVar(&LabVIEW_info,&DMA_Variable_1,CCD1_info,CCD2_info,CCD3_info,HeadSteer_info,Steer_info,Motor_info,Speed_info);
	
	DMA0->ERQ |= (1 << DMA_CH2);
	
#endif
	
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myCarControl_CalDistanceToTurnRound
*  ����˵��: ���ݵ�ǰ�ٶȼ��㵽���ֵ���Ƕ�Զ������ת��
*  ����˵��:	
*  ��������:
*  �޸�ʱ��:2014-07-07
*  ��	ע:
*************************************************************************/
void myCarControl_CalDistanceToTurnRound(Speed_Info *Speed_info)
{	
	//������������ʱ�ľ���   = ��ǰ����ǵľ��� - K*��ǰ���ٶ�
	Speed_info->DistanceBeginTurn = Speed_info->DiatanceToTurnRound - (0.80*Speed_info->Speed_Now);
	
	//����������ȷ��
	if(Speed_info->DistanceToTurn_MIN > Speed_info->DistanceToTurn_MAX)//������С�޶�ֵ��������޶�ֵ
	{
		Speed_info->DistanceToTurn_MIN = Speed_info->DistanceToTurn_MAX;//������޶�ֵΪ׼
	}
	
	//�����ֵ���ʼת�ǵľ����޷�
    if(Speed_info->DistanceBeginTurn > Speed_info->DistanceToTurn_MAX)//��ൽ�˼�ǲ�ת��
	{
		Speed_info->DistanceBeginTurn = Speed_info->DistanceToTurn_MAX;
	}
	else if(Speed_info->DistanceBeginTurn < Speed_info->DistanceToTurn_MIN)//����5cm��ת��
	{
		Speed_info->DistanceBeginTurn = Speed_info->DistanceToTurn_MIN;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myCarControl_Ready_Go
*  ����˵��: ������ʱ
*  ����˵��:         
*  ��������:
*  �޸�ʱ��:2014-06-21
*  ��	ע:
*************************************************************************/
void myCarControl_Ready_Go(void)
{	   
	 LDPY_Delay_s(1);
	 Bell_ON;
	 LDPY_Delay_s(1);
	 Bell_OFF;	 

	 myOLED_Clear();
}

/*----------------------------end of myCarControl.c-----------------------*/
