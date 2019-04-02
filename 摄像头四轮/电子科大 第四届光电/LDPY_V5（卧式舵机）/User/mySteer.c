

#include "mySteer.h"

	Steer_Info Steer_info = {0};
	HeadSteer_Info HeadSteer_info = {0};
	
	extern CCD_Info CCD2_info;
	extern CCD_Info CCD3_info;
	
	extern Parameter_Info Parameter_info;
	
	extern Road_Condition Road_condition;
	
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		
		extern LabVIEW_Info LabVIEW_info;
	
	#endif

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�mySteer_HeadSteerDataInit
*  ����˵����ҡͷ���������ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-29
*  ��    ע��
*************************************************************************/
void mySteer_HeadSteerDataInit(HeadSteer_Info *HeadSteer_info)
{
	uint8 ii = 0;
	
	HeadSteer_info->KPH = 30.0;
	HeadSteer_info->KIH = 1.40;
	
	//ҡͷ���Ƶ��-200Hz	
	HeadSteer_info->Steer_Center = 5000;
	HeadSteer_info->Steer_LeftMAX = 4200;//2000�� -90 ��,4250
	HeadSteer_info->Steer_RightMAX = 5800;//7800�� +90 ��,5890
	
	HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_Center;
	
	for(ii = 0; ii < 10; ii++)
	{
		HeadSteer_info->SteerPWM_Error[ii] = 0;
		HeadSteer_info->SteerPWM_Error_D[ii] = 0;
	}
	
	FTM2->CONTROLS[1].CnV = HeadSteer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�mySteer_DataInit
*  ����˵����ת����������ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-29
*  ��    ע��
*************************************************************************/
void mySteer_DataInit(Steer_Info *Steer_info)
{
	uint8_t ii = 0;
	
 	Steer_info->KP1 = 20;
	Steer_info->KP2 = 10;
	Steer_info->KP3 = 6;
	
	Steer_info->KI = 0;
	
	Steer_info->HeadToSteer_1 = 1;
	Steer_info->HeadToSteer_2 = 1.05;
	Steer_info->HeadToSteer_3 = 0.6;
	
	Steer_info->KP_Mult = 0;
	Steer_info->KD_Mult = 0;
	Steer_info->KI_Mult = 0;

	//ת����Ƶ��-200Hz	
	Steer_info->Steer_Center = 5040;
	Steer_info->Steer_RightMAX = 4100;
	Steer_info->Steer_LeftMAX = 5980;
	
	for(ii = 0; ii < 10; ii++)
	{
		Steer_info->SteerPWM_Error[ii] = 0;
	}
	
	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
	
	Steer_info->RampUp_SteerBackToCenter_Flag = 0;
	
	FTM2->CONTROLS[0].CnV = Steer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�mySteer_HeadSteerControl
*  ����˵����ҡͷ�������ʽPID���㼰ҡͷ����Ŀ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-07-07
*  ��    ע��
***************************************w**********************************/
void mySteer_HeadSteerControl(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								HeadSteer_Info *HeadSteer_info,
								Speed_Info *Speed_info,
								Car_State *Car_state,
								Car_State *Car_state_Pre
							)
{
	float HeadSteerError = 0.0;//ҡͷ�����Ҫ���ڵ�ƫ��
	
	if(Turn_Round != *Car_state)//���������ֵ�ʱ����ҡͷ
	{
		if(1 != CCD1_info->LossLine_Flag)//CCD1û�ж���
		{	
			if(1 != CCD2_info->Cross_Flag)//CCD2û�д���ʮ�ֵ�
			{
				switch(Speed_info->SpeedMode)//���ݵ�λѡ��ʹ����ӦCCD��ƫ��
				{
					case 1://CCD1
						{
							HeadSteerError =  HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1])
											+ HeadSteer_info->KIH*CCD1_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD1_info->LineError[0];
							
							#endif
						}
						break;
					case 2://CCD2
						{
							HeadSteerError =  HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1])
											+ HeadSteer_info->KIH*CCD2_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD2_info->LineError[0];
							
							#endif
						}
						break;
					case 3://CCD2
						{
							HeadSteerError =  10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
											+ 0.8*CCD3_info->LineError[0];
							
							#ifdef DMA_CH2_SendLabVIEW_Info_Enable
								
								LabVIEW_info.HeadSteer_KP_Item = 10*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
								LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD2_info->LineError[0];
							
							#endif
						}
						break;
					default:
						break;
				}
				
				//����ҡͷ��������ֵ
				HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
			}
			else//CCD2ʮ�ֵ�����
			{
				if (CCD3_info->CCD_Ready_Num >= 1)//CCD3������Ч
				{
					HeadSteerError = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
								   + 0.8*CCD3_info->LineError[0];

					//����ҡͷ��������ֵ
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD3_info->LineError[0];
						
					#endif
				}
				else if (0 == CCD1_info->LossLine_Flag)//CCD3������Ч,CCD1������Ч
				{
					HeadSteerError =  HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1])
									+ HeadSteer_info->KIH*CCD1_info->LineError[0];
					
					//����ҡͷ��������ֵ
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD1_info->LineError[0] - CCD1_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD1_info->LineError[0];
						
					#endif
				}
				else if(1 == Speed_info->DistanceOf1Cm_Flag)//CCD3������Ч��CCD1������Ч����ҡͷ����𽥻���
				{
					if(HeadSteer_info->SteerPWM_Error[0] >= 40)
					{
						HeadSteer_info->Steer_PWMOutputDuty -= 40;
					}
					else if(HeadSteer_info->SteerPWM_Error[0] <= -40)
					{
						HeadSteer_info->Steer_PWMOutputDuty += 40;
					}
				}
			}
		}
		else//CCD1ȫ����
		{	
			if((1 == CCD1_info->LossLine_Flag) && ((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition)))//�µ�����
			{
				HeadSteerError = 0;
				CCD1_info->LineError[0] = 0;
				CCD1_info->LineError[1] = 0;
				
				HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_Center;//CCD1����֮����ҡͷ�������ֱ��
			}
			else if(1 == CCD1_info->Cross_Flag)//CCD1ʮ�ֵ�����
			{
				Speed_info->SpeedMode = 2;//ʮ�ֵ���2���ٶ�ͨ��
				
				if (0 == CCD2_info->LossLine_Flag)//CCD2û�ж���
			{
					HeadSteerError =  HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1])
									+ HeadSteer_info->KIH*CCD2_info->LineError[0];
					
					//����ҡͷ��������ֵ
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = HeadSteer_info->KPH*(CCD2_info->LineError[0] - CCD2_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = HeadSteer_info->KIH*CCD2_info->LineError[0];
						
					#endif
					
				}
				else if (CCD3_info->CCD_Ready_Num >= 1)//CCD2������Ч,CCD3������Ч
				{
					HeadSteerError =  10*(CCD3_info->LineError[0] - CCD3_info->LineError[1])
									+ 0.8*CCD3_info->LineError[0];
					
					//����ҡͷ��������ֵ
					HeadSteer_info->Steer_PWMOutputDuty += HeadSteerError;
					
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
							
							LabVIEW_info.HeadSteer_KP_Item = 10*(CCD3_info->LineError[0] - CCD3_info->LineError[1]);
							LabVIEW_info.HeadSteer_KI_Item = 0.8*CCD3_info->LineError[0];
						
					#endif
				}
				else if(1 == Speed_info->DistanceOf1Cm_Flag)//CCD3������Ч��CCD2������Ч����ҡͷ����𽥻���
				{
					if(HeadSteer_info->SteerPWM_Error[0] >= 40)
					{
						HeadSteer_info->Steer_PWMOutputDuty -= 40;
					}
					else if(HeadSteer_info->SteerPWM_Error[0] <= -40)
					{
						HeadSteer_info->Steer_PWMOutputDuty += 40;
					}
				}
			}
		}
	}//end of if(TurnRound != *Car_state)
	 
	/*-------------------------���������Ϳ����ֱ��ʹ���----------------------*/
	if((HeadSteerError > 0) && (HeadSteer_info->Steer_PWMOutputDuty >= HeadSteer_info->Steer_RightMAX))
	{
		HeadSteerError = 0;
	}
	else if((HeadSteerError < 0) && (HeadSteer_info->Steer_PWMOutputDuty <= HeadSteer_info->Steer_LeftMAX))
	{
		HeadSteerError = 0;
	}
	//-------------------------���������Ϳ����ֱ��������----------------------//
	
	//--------------------------ҡͷ�����������޷�---------------------------//
	if(HeadSteer_info->Steer_PWMOutputDuty < HeadSteer_info->Steer_LeftMAX)//ҡͷ��������
	{
		HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_LeftMAX;
	}
	else if(HeadSteer_info->Steer_PWMOutputDuty > HeadSteer_info->Steer_RightMAX)//ҡͷ�����Ҽ���
	{
		HeadSteer_info->Steer_PWMOutputDuty = HeadSteer_info->Steer_RightMAX; 
	}
	
	//ҡͷ������
	FTM2->CONTROLS[1].CnV = HeadSteer_info->Steer_PWMOutputDuty;
	
	if(1 == Speed_info->DistanceOf1Cm_Flag)//1cm�ƶ�һ�ζ���
	{
		Speed_info->DistanceOf1Cm_Flag = 0;//����1cm�����
		
		//�ƶ�ҡͷ���ƫ�����
		HeadSteer_info->SteerPWM_Error[9] = HeadSteer_info->SteerPWM_Error[8];
		HeadSteer_info->SteerPWM_Error[8] = HeadSteer_info->SteerPWM_Error[7];
		HeadSteer_info->SteerPWM_Error[7] = HeadSteer_info->SteerPWM_Error[6];
		HeadSteer_info->SteerPWM_Error[6] = HeadSteer_info->SteerPWM_Error[5];
		HeadSteer_info->SteerPWM_Error[5] = HeadSteer_info->SteerPWM_Error[4];
		HeadSteer_info->SteerPWM_Error[4] = HeadSteer_info->SteerPWM_Error[3];
		HeadSteer_info->SteerPWM_Error[3] = HeadSteer_info->SteerPWM_Error[2];
		HeadSteer_info->SteerPWM_Error[2] = HeadSteer_info->SteerPWM_Error[1];
		HeadSteer_info->SteerPWM_Error[1] = HeadSteer_info->SteerPWM_Error[0];
		
		//�ƶ�ҡͷ���ƫ���ʶ���
		HeadSteer_info->SteerPWM_Error_D[9] = HeadSteer_info->SteerPWM_Error_D[8];
		HeadSteer_info->SteerPWM_Error_D[8] = HeadSteer_info->SteerPWM_Error_D[7];
		HeadSteer_info->SteerPWM_Error_D[7] = HeadSteer_info->SteerPWM_Error_D[6];
		HeadSteer_info->SteerPWM_Error_D[6] = HeadSteer_info->SteerPWM_Error_D[5];
		HeadSteer_info->SteerPWM_Error_D[5] = HeadSteer_info->SteerPWM_Error_D[4];
		HeadSteer_info->SteerPWM_Error_D[4] = HeadSteer_info->SteerPWM_Error_D[3];
		HeadSteer_info->SteerPWM_Error_D[3] = HeadSteer_info->SteerPWM_Error_D[2];
		HeadSteer_info->SteerPWM_Error_D[2] = HeadSteer_info->SteerPWM_Error_D[1];
		HeadSteer_info->SteerPWM_Error_D[1] = HeadSteer_info->SteerPWM_Error_D[0];
		
		Steer_info.RampUp_SteerBackToCenter_Flag = 1;//ת�����������
	}
	
	//����ҡͷ�������Ƕ�ƫ�����(��ռ�ձȱ�ʾ)
	HeadSteer_info->SteerPWM_Error[0] = HeadSteer_info->Steer_PWMOutputDuty - HeadSteer_info->Steer_Center;
	
	//����ҡͷ����ĽǶ�ƫ����
	HeadSteer_info->SteerPWM_Error_D[0] = HeadSteer_info->SteerPWM_Error[0] - HeadSteer_info->SteerPWM_Error[9];
	
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�mySteer_DirectionControl
*  ����˵����ת����λ��ʽPID���㼰ת�����Ŀ���
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע������ҡͷ����Ĵ���Լ�ƫ���ۺϼ���õ�ת�����Ĵ��
*			��ת���������ҡͷ�����ǳɱ�����ϵ�������������
*			�������
*************************************************************************/
void mySteer_DirectionControl(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,HeadSteer_Info *HeadSteer_info,Steer_Info *Steer_info,Speed_Info *Speed_info)
{
	float LineError = 0.0;
	float HeadToSteer_Temp = 0;
	
	//��ʼ���پ������У��
	if(Parameter_info.Mode_HighSpeedDistance <= 0)
	{
		switch(Speed_info->SpeedMode)
		{
			case 1://CCD1
					LineError = Steer_info->KP1*CCD1_info->LineError[0];
					Bell_ON;
					break;
			case 2://CCD2
					LineError = Steer_info->KP2*CCD2_info->LineError[0];
					break;
			case 3://CCD2
					LineError = Steer_info->KP3*CCD3_info->LineError[0];
					break;
			default:
					break;
		}
	}

	//------------------------����ת����������Ƕ�---------------------------//
	//����
	if(Ramp_Up == Road_condition)
	{
		if(1 != CCD1_info->LossLine_Flag)//CCD1û�ж���
		{
			Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - Steer_info->HeadToSteer_1*HeadSteer_info->SteerPWM_Error[1] - 0.4*LineError);
		}
		else if((1 == Steer_info->RampUp_SteerBackToCenter_Flag) && (1 == CCD1_info->LossLine_Flag))//CCD1�Ѿ�����,ת�����𽥻���ֱ��
		{
			//�������»�����Ƿ��ڶ��ƫ������ƶ�֮��
			
			if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_Center)
			{	
				if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_Center + 20)
				{
					Steer_info->Steer_PWMOutputDuty -= 20;
				}
				else
				{
					Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
				}
			}
			else if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_Center)
			{
				if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_Center - 20)
				{
					Steer_info->Steer_PWMOutputDuty += 20;
				}
				else
				{
					Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center;
				}
			
			}
		}
	}
	else if ((Ramp_Down == Road_condition) && (1 != CCD1_info->LossLine_Flag))//����,CCD1�ѵ����ߵ���,������ܹ���ǹ���
	{
		Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadSteer_info->SteerPWM_Error[1] - 0.4*LineError);
	
		if(Steer_info->SteerPWM_Error[0] > 400)
		{
			Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center + 400;
		}
		else if(Steer_info->SteerPWM_Error[0] < -400)
		{
			Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_Center - 400;
		}
	
	}
	else if((1 == CCD2_info->Cross_Flag) || (1 == CCD1_info->Cross_Flag))//CCD2����ʮ�ֵ�
	{		
		LineError = 0;
		
		Steer_info->Steer_PWMOutputDuty = (uint32_t)(Steer_info->Steer_Center - Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1]);
	
		//������ʱ����
		#ifdef DMA_CH2_SendLabVIEW_Info_Enable
						
			LabVIEW_info.Steer_Head_Item = HeadSteer_info->SteerPWM_Error[1];
			LabVIEW_info.Steer_Err_Item = LineError;
					
		#endif
	
	}
	else//��������:K*ҡͷ���������Ƕ� 	+ 	LineError
	{
		switch(Speed_info->SpeedMode)
		{
			case 1://ʹ��CCD1
				{
					//����ҡͷ�������ת�����ĽǶ�
					HeadToSteer_Temp = Steer_info->HeadToSteer_1*HeadSteer_info->SteerPWM_Error[1];

					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
					
					//������ʱ����
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = HeadToSteer_Temp;
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}
				break;
			case 2://ʹ��CCD2
				{
					//����ҡͷ�������ת�����ĽǶ�
					if(Parameter_info.Mode_HighSpeedDistance <= 0)
					{
						HeadToSteer_Temp = Steer_info->HeadToSteer_2*HeadSteer_info->SteerPWM_Error[1];
					}
					else
					{
						HeadToSteer_Temp = 0.8*Steer_info->HeadToSteer_2*HeadSteer_info->SteerPWM_Error[1];
					}
						
					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
				
					//������ʱ����
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = HeadToSteer_Temp;
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}	
				break;
			case 3://ʹ��CCD3
				{
					if(Parameter_info.Mode_HighSpeedDistance <= 0)
					{
						HeadToSteer_Temp = Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
					}
					else
					{
						HeadToSteer_Temp = 0.8*Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
					}
					
					
					Steer_info->Steer_PWMOutputDuty = (uint32)(Steer_info->Steer_Center - HeadToSteer_Temp - LineError);
				
					//������ʱ����
					#ifdef DMA_CH2_SendLabVIEW_Info_Enable
									
						LabVIEW_info.Steer_Head_Item = Steer_info->HeadToSteer_3*HeadSteer_info->SteerPWM_Error[1];
						LabVIEW_info.Steer_Err_Item = LineError;
					
					#endif
				}
				break;
			default:
					break;
		}
	}
	
	//--------------------------ת������������޷�---------------------------//
	if(Steer_info->Steer_PWMOutputDuty > Steer_info->Steer_LeftMAX)	//��������
	{
	 	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_LeftMAX;
	}
	else if(Steer_info->Steer_PWMOutputDuty < Steer_info->Steer_RightMAX)//�����Ҽ���
	{
	 	Steer_info->Steer_PWMOutputDuty = Steer_info->Steer_RightMAX; 
	}
	
	//�ƶ�ת����ƫ�����
	if(1 == Steer_info->RampUp_SteerBackToCenter_Flag)
	{
		Steer_info->RampUp_SteerBackToCenter_Flag = 0;	//�������»������
		
		Steer_info->SteerPWM_Error[9] = Steer_info->SteerPWM_Error[8];
		Steer_info->SteerPWM_Error[8] = Steer_info->SteerPWM_Error[7];
		Steer_info->SteerPWM_Error[7] = Steer_info->SteerPWM_Error[6];
		Steer_info->SteerPWM_Error[6] = Steer_info->SteerPWM_Error[5];
		Steer_info->SteerPWM_Error[5] = Steer_info->SteerPWM_Error[4];
		Steer_info->SteerPWM_Error[4] = Steer_info->SteerPWM_Error[3];
		Steer_info->SteerPWM_Error[3] = Steer_info->SteerPWM_Error[2];
		Steer_info->SteerPWM_Error[2] = Steer_info->SteerPWM_Error[1];
		Steer_info->SteerPWM_Error[1] = Steer_info->SteerPWM_Error[0];
	}
	
	//����ת����ƫ�����
	Steer_info->SteerPWM_Error[0] = Steer_info->Steer_PWMOutputDuty - Steer_info->Steer_Center;
	
	//ת�������
	FTM2->CONTROLS[0].CnV = Steer_info->Steer_PWMOutputDuty;
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�mySteer_Init
*  ����˵����ҡͷ�����ת�����ĳ�ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void mySteer_Init(void)
{
	PORTB->PCR[18]=PORT_PCR_MUX(3);				//B18:FTM2_CH0
	PORTB->PCR[19]=PORT_PCR_MUX(3);				//B19:FTM2_CH1
	
	SIM->SCGC3|=SIM_SCGC3_FTM2_MASK;			//ʹ��ʱ��
			
	FTM2->MODE|=FTM_MODE_WPDIS_MASK;			//д������ֹ
			
	//--------------------CnSCͨ��״̬����----------------------------//
	//------------------���ض���PWM�ȸߺ��	
	FTM2->CONTROLS[0].CnSC |= FTM_CnSC_ELSB_MASK;			//����룬�ȸߺ��
	FTM2->CONTROLS[0].CnSC &=~FTM_CnSC_ELSA_MASK; 			//
	FTM2->CONTROLS[0].CnSC |= FTM_CnSC_MSB_MASK;			//ģʽѡ�ű��ض���ģʽ
	
	FTM2->CONTROLS[1].CnSC |= FTM_CnSC_ELSB_MASK;			//����룬�ȸߺ��
	FTM2->CONTROLS[1].CnSC &=~FTM_CnSC_ELSA_MASK; 			//
	FTM2->CONTROLS[1].CnSC |= FTM_CnSC_MSB_MASK;			//ģʽѡ�ű��ض���ģʽ

	//------------------FTM2״̬����-------------------------------
	//[7]TOF��ʱ�������־
	//[6]TOFIE��ʱ������ж�ʹ��
	//[5]CPWMS=0 �������ӷ�����
	//[4:3]CLKS=01 system clock
	//[2:0]PS Ԥ��Ƶ��=2^PS			
	FTM2->SC=0x0D;					//�жϽ�ֹ,ʱ��Դ����Ƶ4����������

	//-------------------����ģʽѡ��-----------------------------
  	FTM2->MODE &=~ 1;					//ʹ��FTM2
			
	//-------------------������μĴ���---------------------------
	FTM2->OUTMASK =~ (FTM_OUTINIT_CH0OI_MASK | FTM_OUTINIT_CH1OI_MASK);	//ͨ��0,1�����������ֹ
	FTM2->OUTINIT |=0x01;
	
	//-------------------��������״̬����-------------------------
	FTM2->QDCTRL &=~ FTM_QDCTRL_QUADEN_MASK;	//��ֹ��������ģʽ
			
	//-------------------ͨ�����Ϲ���-----------------------------
	FTM2->COMBINE = 0 ;					//˫���ز�׽��ֹ��������
			
	FTM2->CNTIN = 0 ;					//��������ʼֵ 0
	FTM2->MOD = 15626;					//��������ֵֹ,��������������趨(15625-200Hz)
	
	//-------------------����ֵ-------------------------------------
	FTM2->CONTROLS[0].CnV = 5000;		//�ߵ�ƽʱ��,��ʼ�����ʱ��ռ�ձ�(5000/15625)
	FTM2->CONTROLS[1].CnV = 5000;
		
	FTM2->CNT = 0;						//��������0
}

/*------------------------------end of mySteer.c--------------------------*/
