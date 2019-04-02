/*
						
                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                           				|
|             ��˼�������ܳ�-�����-���ƫ��             |
|              												|
|                              Oooo                         |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)     

*/

#include "myIncludes.h"

	 Parameter_Info  Parameter_info = {0};
	 Road_Condition  Road_condition = NeedJudge;
	 Road_Condition  Road_condition_Item = NeedJudge;
	 Car_State  	 Car_state = Go_Forward;
	 Car_State 		 Car_state_Pre = Go_Forward;
	 
	 Car_Mode Car_mode = SelfDef;//��ʼ��Ϊ��ȫ�ٶ�
	 
	 Road_Type 		Road_type = All;//Ĭ����������Ϊȫ��(�����µ�,���ֺ�·��)
	 
	 menu Menu = {0};
	 
	 uint16_t StopType = 0;

	 extern CCD_Info CCD1_info;
     extern CCD_Info CCD2_info;
	 extern CCD_Info CCD3_info;
	 
	 extern Steer_Info Steer_info;
	 extern HeadSteer_Info HeadSteer_info;
	 
	 extern Motor_Info Motor_info;
	 extern Speed_Info Speed_info;
	 
	 extern Gyro_Info Gyro_info;
	    
	 extern DMA_Required_Variable DMA_Variable_1;
	 extern DMA_InitTypeDef dma_init_struct;
	 extern Flash_Info Flash_info;
	 extern RemSpeedUpDown_Info RemSpeedUpDown_info;
	 
void myMain_ParametersInit(Parameter_Info *Parameter_info);
void myMain_ChangeRoadType(Road_Type *Road_type,Parameter_Info *Parameter_info);
void myMain_TimeControl(Car_Mode *Car_mode,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							Car_State *Car_state,
							Car_State *Car_state_Pre
						   );

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: main
*  ����˵��: main����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-06-23
*  ��	ע:
*************************************************************************/
int main(void)
{ 		
	LDPY_SysTickInit();	
	LDPY_ADC0_Init();
	LDPY_ADC1_Init();
	LDPY_PIT0_Init();	
	LDPY_Flash_Init();
	
	myLED_Init();
	myOLED_Init();
	myKey_Init();
	myCCD_GPIO_Init();
	mySteer_Init();
	myMotor_Init();
	myMotor_EncoderInit();
	
/*--------------------���ݳ�ʼ��------------------------*/
	myMain_ParametersInit(&Parameter_info);					//һЩ���������ʼ��
	myCCD_DataInit(&CCD1_info);								//CCD1������ʼ��
	myCCD_DataInit(&CCD2_info);								//CCD2������ʼ��
	myCCD_DataInit(&CCD3_info);								//CCD3������ʼ��
	mySteer_HeadSteerDataInit(&HeadSteer_info);				//ҡͷ���������ʼ��
	mySteer_DataInit(&Steer_info);							//ת����������ʼ��
	myMotor_DataInit(&Motor_info);							//���������ʼ��
	myMotor_SpeedDataInit(&Speed_info);						//�ٶȲ�����ʼ��
	myGyro_DataInit(&Gyro_info);							//�����ǲ�����ʼ��
	
	CCD1_info.InvalidPixel_Num = 20;
	
	#ifdef RemRoad_Control_Enable
	
		myRemRoad_DataInit(&RemSpeedUpDown_info);
		
	#endif
	
/*--------------------------------------------------------*/

	Bell_ON;
	LDPY_Delay_ms(150);
	Bell_OFF;
	
/*--------------------------��λ������-----------------------------*/
#ifdef LabVIEW_Control_Enable

	LDPY_UART5_DisableIrq();				//�رմ����ж�
	
	myLabVIEW_UART_Init(115200);			//��λ����ʼ��
	Parameter_info.Parameter_OK = 0;
	do
	{
		myLabVIEW_ParametersInit(	&Parameter_info,
									&CCD1_info,
									&Steer_info,
									&Motor_info,
									&Speed_info
								);
	}while(0 == Parameter_info.Parameter_OK);			//�ȴ���ʼ���������
	
	Parameter_info.Parameter_OK = 0;
	
	LDPY_UART5_EnableIrq();						//�򿪴����ж�,���յ��Բ���
	
#endif
//end of LabVIEW_Control_Enable
/*-----------------------------------------------------------------*/
	
#ifdef DMA_Enable
	//DMA��ʼ��
	LDPY_DMA_InitAndChoice(&dma_init_struct,&DMA_Variable_1,&CCD1_info);
#endif
//end of DMA_Enable
	
	//�ѳ�ʼ���Ĳ���д��Flash
	//write_Data2Flash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);
	
	//��Flash��ȡ����
	read_DataFromFlash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);
	
	while(lcd_menu_display_init(&Menu))
	{
		if(1 == Parameter_info.OLED_NoAction_Flag)//������û�з�Ӧ��ʱ���ʱ�˳�
		{
			if(Parameter_info.OLED_NoAction_Counter > 0)
			{
				Parameter_info.OLED_NoAction_Counter--;
			}
			else if(Parameter_info.OLED_NoAction_Counter <= 0)
			{
				break;//ֱ������OLED�ȴ�
			}
		}
	}

	//������д��Flash
	write_Data2Flash(&Parameter_info,&CCD1_info,&CCD2_info,&HeadSteer_info,&Steer_info,&Speed_info,&Flash_info,&Motor_info);


	//�����ֶ��趨������������������ص���������ʶ��(�µ������֣�·��)
	myMain_ChangeRoadType(&Road_type,&Parameter_info);
	
	#ifdef RemRoad_Control_Enable
	
		myRemRoad_DataSetting(&RemSpeedUpDown_info);
	
	#endif
	
	myCarControl_Ready_Go();				//������ʱ
	
	if(1 == Parameter_info.GetStartEndLine_Flag)//��Ҫ���������ʱ��ʼ�������߼��
	{
		myKey_StartEndLineInit();				
	}
	
	LDPY_PIT0_us(3000);						//���������趨
	
	//����ʱ�ĳ������ݲɼ��ʹ���
	myCCD_GetBeginLineError(	&CCD1_info,
								&CCD2_info,
								&CCD3_info,
								&Speed_info,
								&Parameter_info
							);

	while(1)
	{	
		if(1 == Parameter_info.CCD_SampleOK_Flag)			//���ݲɼ����
		{
			Parameter_info.CCD_SampleOK_Flag = 0;			//������ݲɼ���ɱ�־
			
			myCarControl_CarControl(&CCD1_info,
									&CCD2_info,
									&CCD3_info,
									&HeadSteer_info,
									&Steer_info,
									&Motor_info,
									&Speed_info,
									&Parameter_info,
									&RemSpeedUpDown_info,
									&Road_condition,
									&Car_state,
									&Car_state_Pre
									);	//(��ʱ500us)

			//----------------------------ʱ�����----------------------------//
			if(Stop != Car_state)//������ʻ��ʱ��
			{
				myMain_TimeControl(	&Car_mode,
									&CCD1_info,
									&CCD2_info,
									&CCD3_info,
									&Speed_info,
									&Parameter_info,
									&Car_state,
									&Car_state_Pre
								  );
			}
		}//end of if(1 == Parameter_info.CCD_SampleOK_Flag)
	}//end of while(1)	
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myMain_ParameterInit
*  ����˵��: һЩ���������ʼ������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-06-21
*  ��	ע:
*************************************************************************/
void myMain_ParametersInit(Parameter_Info *Parameter_info)
{
	uint8_t ii = 0;
	
	Parameter_info->Parameter_OK = 0;
	Parameter_info->UART_RecvData = 0;
	
	for(ii = 0; ii < 10; ii++)
	{
		Parameter_info->StartEndLine_Fifo[ii][0] = 0;		//��¼����ܱ��
		Parameter_info->StartEndLine_Fifo[ii][1] = 0;		//��¼�ж�ʱ��
	}
	
	Parameter_info->AD_MultK_f = 1.0;
	Parameter_info->CCD1_AD_TargetValue = 1400;
	Parameter_info->CCD1_Binary_MAX = 1000;
	Parameter_info->CCD1_Binary_MIN = 700;
	
	
	Parameter_info->GetTurnRound_Enable = TRUE;		//Ĭ��ʶ�����ֵ�
	Parameter_info->GetRamp_Enable = TRUE;			//Ĭ�ϼ���µ�
	Parameter_info->GetStartEndLine_Enable = TRUE;	//Ĭ�ϼ��������
	Parameter_info->GetObstacle_Enable = TRUE;		//Ĭ�ϼ��·��
	Parameter_info->RoadInvalid_Enable = TRUE;		//Ĭ�ϼ���·�Ƿ���Ч����ֹ�ܵ�
	Parameter_info->GetStartEndLine_Flag = 1;		//Ĭ�ϼ��������,���1
	Parameter_info->GetStartEndLine_SameLight_Enable = FALSE;//�����߼��ͬ������ʹ��
	Parameter_info->CCD1GetSL_Enable = FALSE;		//CCD1ʶ��������ʹ��
	Parameter_info->WheelOutOfWay_Enable = FALSE;	
	
	Parameter_info->Mode_HighSpeedDistance = 400.0;			//Ĭ��400cm�ĸ��پ���
	Parameter_info->OLED_NoAction_Counter = 20000;
	
	Parameter_info->RampJgeInAdvce_Num = 0;
	Parameter_info->TurnRound_OK_Num = 0;
	Parameter_info->TurnRound_Num_MAX = 3;
	Parameter_info->Obstacle_OK_Num = 0;
	
	Parameter_info->Angle60_MeanValue = 10;
	
	Parameter_info->Time = 0;	
	Parameter_info->GetStartEndLineTime = 5;			//Ĭ��5���ڲ����������
	Parameter_info->GetTurnRoundTime = 2;				//Ĭ��2���ڲ�������ֵ�
	Parameter_info->GetRampTime = 2;					//Ĭ��2���ڲ�����µ�
	Parameter_info->GetObstacleTime = 2;				//Ĭ��2���ڲ����·��
	Parameter_info->DebugTime = 0;						//Ĭ�ϵ��� 0 ��,������ģʽ������ʱ
	Parameter_info->LoseLineTime = 0;				
	Parameter_info->RoadDistance = 0;		
	Parameter_info->StopSpeed = 0;
	
	Parameter_info->CCD_SampleOK_Flag = 0;
	Parameter_info->StartEndLine_Flag = 0;
	
	Parameter_info->LeftIntoTurnRound_Flag = 0;
	Parameter_info->LeftIntoTurnRound_Flag = 0;
	Parameter_info->CCD2OrCCD3_GetTurnRound = 0;
	
	Parameter_info->LeftOrRightObstacle_Flag = 0;
	
	Parameter_info->OLED_NoAction_Flag = 1;		
	Parameter_info->RampReady_Distance = 0;
	Parameter_info->RampReady_FLag = 0;
	Parameter_info->SlowBeforeTurnRound_Flag = 0;	//Ĭ��ȫ�̸��٣��˱����1����ʶ������֮ǰʹ�õ���ģʽ
	Parameter_info->LongStraight_Flag = 0;			//��ֱ�����
	Parameter_info->CCD1_GetedStartEndLine_Flag = 0;
	Parameter_info->Left60Angle_Flag = 0;
	Parameter_info->Right60Angle_Flag = 0;
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myMain_SetCarMode
*  ����˵��: ���ñ���ʱ��С��ģʽ�����٣���ȫ�ٶȣ����٣�
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-07-12
*  ��	ע:
*************************************************************************/
void myMain_SetCarMode(	Car_Mode *Car_mode,
						HeadSteer_Info *HeadSteer_info,
						Steer_Info *Steer_info,
						Motor_Info *Motor_info,
						Speed_Info *Speed_info
					  )
{
	switch(*Car_mode)//ѡ���ٶ�ģʽ
	{
		case High2://�����
			{
				
			}
			break;
		case High1:
			{
				
			}
			break;
		case Safe:
			{
				
			}
			break;
		case Low1:
			{
				
			}
			break;
		case Low2:
			{
				
			}
			break;
		default:
			break;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myMain_ChangeRoadType
*  ����˵��: �ı���������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-07-08
*  ��	ע:
*************************************************************************/
void myMain_ChangeRoadType(Road_Type *Road_type,Parameter_Info *Parameter_info)
{
	switch(*Road_type)
	{
		case None://ȫ���٣�û���µ������ֺ�·��(Ԥ����������)
			Parameter_info->GetRamp_Enable = FALSE;
			Parameter_info->GetTurnRound_Enable = FALSE;
			Parameter_info->GetObstacle_Enable = FALSE;
			break;
		case Ramp_Only://ֻ���µ�(Ԥ����������)
			Parameter_info->GetRamp_Enable = FALSE;
			Parameter_info->GetTurnRound_Enable = FALSE;
			Parameter_info->GetObstacle_Enable = TRUE;
			break;
		case All://ȫ����
			Parameter_info->GetRamp_Enable = TRUE;
			Parameter_info->GetTurnRound_Enable = TRUE;
			Parameter_info->GetObstacle_Enable = TRUE;
			break;
		default:
			break;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*  ��������: myMain_TimeControl
*  ����˵��: ����ʱ����صĿ���
*  ����˵��:	
*            
*  ��������:��
*  �޸�ʱ��:2014-07-22
*  ��	ע:�˺���ÿ3ms����һ�Σ��ʿ�����ʱ����ƺ���
*************************************************************************/
void myMain_TimeControl(	Car_Mode *Car_mode,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info,
							Car_State *Car_state,
							Car_State *Car_state_Pre
					   )
{
	//������ʻ��ʱ��
	Parameter_info->Time += 0.003;
	
	#ifndef DMA_CH0_Enable
		//CCD1��������ʱ���ж��߱���
		if(    (1 == CCD1_info->LossLine_Flag)
			&& (1 == CCD2_info->LossLine_Flag)
			&& (0 == CCD1_info->Cross_Flag)
			&& (Ramp_Up != Road_condition)
			&& (Ramp_Down != Road_condition)
			&& (Turn_Round != *Car_state)
			&& (CheckCCD != *Car_mode))
		{
			if(Parameter_info->LoseLineTime < 1)//CCD1��������1����ͣ������
			{
				Parameter_info->LoseLineTime += 0.003;
			}
			else
			{
				*Car_state = Stop;
				
				Parameter_info->StartEndLine_Flag = 1;
				
				StopType = 9;
			}
		}
		else
		{
			Parameter_info->LoseLineTime = 0;//һ��CCD1�ٴ�Ѱ���߽������㶪�߼�ʱ
		}
	#endif
	
	//------------------------------���Կ���ģʽ---------------------------//
	//������ʱ��Ϊ�������������ʻ״̬�������е��ԡ����CCDʱ����ʱ��
	if(    (CheckCCD != *Car_mode)
		&& (0 != Parameter_info->DebugTime) 
		&& (Parameter_info->Time >= Parameter_info->DebugTime))//����ʱ�䵽,ͣ��
	{
		if(Stop != *Car_state)
		{
			*Car_state_Pre = *Car_state;
			*Car_state = Stop;
			
			Parameter_info->StopSpeed = Speed_info->Speed_Now;
			
			Parameter_info->StartEndLine_Flag = 1;
			
			StopType = 8;
		}
	}
	else if(CheckCCD == *Car_mode)//���CCDģʽ
	{
		myOLED_Dec4(7,10,CCD3_info->LeftLine[0]);
		myOLED_Dec4(7,50,CCD3_info->CentralLine[0]);
		myOLED_Dec4(7,90,CCD3_info->RightLine[0]);
		
		myOLED_Dec4(6,10,CCD2_info->LeftLine[0]);
		myOLED_Dec4(6,50,CCD2_info->CentralLine[0]);
		myOLED_Dec4(6,90,CCD2_info->RightLine[0]);
		
		myOLED_Dec4(5,10,CCD1_info->LeftLine[0]);
		myOLED_Dec4(5,50,CCD1_info->CentralLine[0]);
		myOLED_Dec4(5,90,CCD1_info->RightLine[0]);
		
		myOLED_Dec4(4,10,Speed_info->SpeedFifo[0]);				//������ʾ�������Ƿ���������
		myOLED_Decimals(4,50,Parameter_info->AD_MultK_f);		//������ʾCCD�ķŴ���
		
		myOLED_CCDwave(CCD1_info,CCD2_info,CCD3_info);
		
		//���ڵ���CCDģʽʱ����㶨ռ�ձ�
		FTM0->CONTROLS[6].CnV = 2000;
		FTM0->CONTROLS[7].CnV = 0;
	}
	else if(CheckRamp == *Car_mode)
	{
		//��ʾ��ǰ·�����Ƿ�Ϊ�µ���4 - ���£�5 - ���£�
		myOLED_String(6,1,"RoadCndition");
		myOLED_Dec(6,90,Road_condition);
		
		//��ʾ�µ������¼
		myOLED_String(5,1,"RampDistance");
		myOLED_Dec4(5,90,Speed_info->DistanceOfRampUp_Counter);
		
		//��ʾ�����Ƿ�Ԥ�гɹ�
		myOLED_String(4,1,"RampUp_YuPan");
		myOLED_Dec(4,90,Parameter_info->RampReady_FLag);
		
		//��ʾ�����Ƿ�Ԥ�гɹ�
		myOLED_String(3,1,"RampDn_YuPan");
		myOLED_Dec(3,90,myCCD_JudgeRampDown(CCD1_info,CCD2_info,CCD3_info,&Road_condition));
		
		//���ڵ����µ�ģʽʱ�رյ��
		FTM0->CONTROLS[6].CnV = 0;
		FTM0->CONTROLS[7].CnV = 0;
	}
}

/*------------------------end of myMain.c---------------------------------*/		
