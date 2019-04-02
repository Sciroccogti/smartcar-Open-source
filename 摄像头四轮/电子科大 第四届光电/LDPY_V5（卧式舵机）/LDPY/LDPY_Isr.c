
#include "LDPY_Isr.h"
#include "LDPY_DMA.h"

 extern Car_Mode Car_mode;
 
 extern CCD_Info CCD1_info;
 extern CCD_Info CCD2_info;
 extern CCD_Info CCD3_info;
 
 extern Steer_Info Steer_info;
 extern HeadSteer_Info HeadSteer_info;
 
 extern Motor_Info Motor_info;
 extern Speed_Info Speed_info;
 
 extern Car_State  	Car_state;
 extern Car_State 	Car_state_Pre ;
 extern Parameter_Info  Parameter_info;
 
 extern Road_Condition Road_condition;
 extern Gyro_Info Gyro_info;
 
 extern menu Menu;
 
 extern uint16_t StopType;
 
 #ifdef Remote_Control_Enable
 
 uint8_t RemoteStopNum = 0;
 float   RemoteGetTime = 0;
 
#endif

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  PORTE_IRQHandler
*  ����˵��:  PORTE�жϷ�����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:  ���ڷ��񰴼�����
*************************************************************************/ 
	
	void PORTE_IRQHandler(void)
	{
		uint32_t Pin_2=2,Pin_3=3,Pin_4=4,Pin_5=5,Pin_6=6,Pin_7=7;
		uint32_t K_num = 0;
		
		
		Parameter_info.OLED_NoAction_Flag = 0;//�����Ѿ��з�Ӧ��
		
		K_num=PORTE->ISFR;
		
		switch (K_num)
		{
			case 4: //k1  E2
							 LDPY_Delay_ms(10);
							 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
							 {	
								switch(Menu.enter_exit)
								{
								    case 1: Menu.choice_flag +=100;break;
									case 2: Menu.choice_flag +=10 ;break;
								}
								Menu.Clear = 1;
							 }
//								while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0) 
//								{
//								
//								}							  
								PORTE->ISFR |=(1 << Pin_2);//		�����жϱ�־		
								PORTE->PCR[Pin_2]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;
			case 8:  //k2  E3
					     LDPY_Delay_ms(10);
							 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
							 {	
								 
								switch(Menu.enter_exit)
								{
									case 1: Menu.choice_flag -=100;break;											      
									case 2: Menu.choice_flag -=10 ;break;													  
								}	
								Menu.Clear = 1;
							 }
//							 while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0) 
//								{
//								
//								}
								PORTE->ISFR |=(1 << Pin_3);//		�����жϱ�־		
								PORTE->PCR[Pin_3]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;
			case 16:  //k3  E4
				       LDPY_Delay_ms(10);
							 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
							 {
								//Steer_info.Steer_PWMOutputDuty -= 20;
								 
				  			     switch(Menu.enter_exit_temp)
								 {
									 case 3: Menu.enter_exit = 2;Menu.enter_exit_temp = 2;Menu.choice_flag = Menu.choice_flag /10 * 10;break;
									 case 2: Menu.enter_exit = 1;Menu.enter_exit_temp = 1;Menu.choice_flag = Menu.choice_flag /100 * 100;break;
									 case 1: Menu.enter_exit = 1;Menu.enter_exit_temp = 1;break;
								 }
								 Menu.Clear = 1;
							 }
//							 while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0) 
//								{
//								
//								}
								PORTE->ISFR |=(1 << Pin_4);//		�����жϱ�־		
								PORTE->PCR[Pin_4]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;
			case 32:  //k4  E5
				       LDPY_Delay_ms(10);
							 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0)
							 {  
								 //Steer_info.Steer_PWMOutputDuty += 20;
								 
								 switch(Menu.enter_exit_temp)
								 {
								    case 0: Menu.enter_exit = 1;Menu.enter_exit_temp = 1;Menu.choice_flag = Menu.choice_flag /10 * 10;break;
									case 1: Menu.enter_exit = 2;Menu.enter_exit_temp = 2;Menu.choice_flag = Menu.choice_flag /100 * 100+10;break;
								    case 2: Menu.enter_exit = 3;Menu.enter_exit_temp = 3;Menu.choice_flag += 1;break;
								 } 
								 Menu.Clear = 1;
							 }
//							 while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0) 
//								{
//								
//								}
								PORTE->ISFR |=(1 << Pin_5);//		�����жϱ�־		
								PORTE->PCR[Pin_5]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;
			case 64:  //k5  E6
				       LDPY_Delay_ms(10);
							 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0)
							 {	 
								Menu.add_sub =2;
								Menu.Clear = 1;
							 }
//							 while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0) 
//								{
//								
//								}
								PORTE->ISFR |=(1 << Pin_6);//		�����жϱ�־		
								PORTE->PCR[Pin_6]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;
			case 128:  //k6  E7
							LDPY_Delay_ms(10);
						 if(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)==0)
						 {	 
							Menu.add_sub = 1;
							Menu.Clear = 1;
						 }
	//							 while(LDPY_GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)==0) 
	//								{
	//								
	//								}
							PORTE->ISFR |=(1 << Pin_7);//		�����жϱ�־		
							PORTE->PCR[Pin_7]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
							 break;	
			default:
					break;
		}


		PORTE->ISFR =0xFFFFFFFF;
	  	
	}
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  PORTD_IRQHandler
*  ����˵��:  PORTD�жϷ�����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:  
*************************************************************************/ 

	void PORTD_IRQHandler(void)
	{
		uint8_t ii = 0;
		
		uint8_t Pin_11 = 11, Pin_12 = 12, Pin_13 = 13, Pin_14 = 14, Pin_15 = 15;
		uint32_t D_num = 0;
		
		D_num = PORTD->ISFR;
		
		//��������Ƿ���������
		if(CheckSL == Car_mode)
		{
			switch(D_num)
			{
				case 0x1000:
					myOLED_Dec(4,50,2);
					break;
				case 0x2000:
					myOLED_Dec(4,50,1);
					break;
				case 0x8000:
					myOLED_Dec(4,50,3);
					break;
				case 0x4000:
					myOLED_Dec(4,50,4);
					break;
				default:
					myOLED_String(4,50,"NULL");
					break;
			}
			
			LED5_ON;
		}
		
		//����ң��������ж����ƶ������߶���(����ܱ��Ϊ������: 1����: 2����: 3������: 4)
		if(0x0800 != D_num)
		{
			//�ƶ�����ܱ�Ŷ���
			Parameter_info.StartEndLine_Fifo[9][0] = Parameter_info.StartEndLine_Fifo[8][0];
			Parameter_info.StartEndLine_Fifo[8][0] = Parameter_info.StartEndLine_Fifo[7][0];
			Parameter_info.StartEndLine_Fifo[7][0] = Parameter_info.StartEndLine_Fifo[6][0];
			Parameter_info.StartEndLine_Fifo[6][0] = Parameter_info.StartEndLine_Fifo[5][0];
			Parameter_info.StartEndLine_Fifo[5][0] = Parameter_info.StartEndLine_Fifo[4][0];
			Parameter_info.StartEndLine_Fifo[4][0] = Parameter_info.StartEndLine_Fifo[3][0];
			Parameter_info.StartEndLine_Fifo[3][0] = Parameter_info.StartEndLine_Fifo[2][0];
			Parameter_info.StartEndLine_Fifo[2][0] = Parameter_info.StartEndLine_Fifo[1][0];
			Parameter_info.StartEndLine_Fifo[1][0] = Parameter_info.StartEndLine_Fifo[0][0];
			
			//�ƶ�ʱ�����
			Parameter_info.StartEndLine_Fifo[9][1] = Parameter_info.StartEndLine_Fifo[8][1];
			Parameter_info.StartEndLine_Fifo[8][1] = Parameter_info.StartEndLine_Fifo[7][1];
			Parameter_info.StartEndLine_Fifo[7][1] = Parameter_info.StartEndLine_Fifo[6][1];
			Parameter_info.StartEndLine_Fifo[6][1] = Parameter_info.StartEndLine_Fifo[5][1];
			Parameter_info.StartEndLine_Fifo[5][1] = Parameter_info.StartEndLine_Fifo[4][1];
			Parameter_info.StartEndLine_Fifo[4][1] = Parameter_info.StartEndLine_Fifo[3][1];
			Parameter_info.StartEndLine_Fifo[3][1] = Parameter_info.StartEndLine_Fifo[2][1];
			Parameter_info.StartEndLine_Fifo[2][1] = Parameter_info.StartEndLine_Fifo[1][1];
			Parameter_info.StartEndLine_Fifo[1][1] = Parameter_info.StartEndLine_Fifo[0][1];
		}

		switch(D_num)
		{
			case 0x1000:	//Pin_12,��
				{
					 LDPY_Delay_us(20);
					
					 if(LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) == 0)
					 {	
						//���������
						if(	   (Stop != Car_state)
							&& (TRUE == Parameter_info.GetStartEndLine_Enable) 
							&& (Turn_Round != Car_state) 
							&& (Turn_Round != Car_state_Pre)
							&& (Parameter_info.Time >= Parameter_info.GetStartEndLineTime))
						{
							 if(1 == Parameter_info.CCD1_GetedStartEndLine_Flag)//CCD1Ԥ�������߳ɹ�
							 {
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
								
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 1002;
							 }
							 //�����һ������
							 else if((LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)==0) 
									|| (LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)==0))
							 {
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
								
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 2;
							}
							//���ͬ������
							else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
									&& (1 == Parameter_info.LongStraight_Flag)
									&& (0 == LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)))
							{
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
							 
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 221;
									 
							}
							else
							{
								//��ѯ��������
								for(ii = 0; ii < 10; ii++)
								{
									//����ʱ�ұߺ����3,4��200ms�������й���⵽�����ߣ���ͣ��
									if(((3 == Parameter_info.StartEndLine_Fifo[ii][0]) || (4 == Parameter_info.StartEndLine_Fifo[ii][0]))
										&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME)
									  )
									{
										Car_state_Pre = Car_state;
										Car_state = Stop;
									 
										Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										Parameter_info.StartEndLine_Flag = 1;
										 
										StopType = 234;
									}
									//���ͬ������
									else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
											&& (1 == Parameter_info.LongStraight_Flag)
											&& (1 == Parameter_info.StartEndLine_Fifo[ii][0])
											&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
									{
										 Car_state_Pre = Car_state;
										 Car_state = Stop;
									 
										 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										 Parameter_info.StartEndLine_Flag = 1;
										 
										 StopType = 221;
										
									}
								}
								
								//û��ͣ�����¼�´�ʱ�ĺ���ܱ�ź��ж�ʱ��
								Parameter_info.StartEndLine_Fifo[0][0] = 2;
								Parameter_info.StartEndLine_Fifo[0][1] = Parameter_info.Time;
							}
						}
						
						//���籣��
						#ifdef PROTECT_CAR_ENABLE
						
							if((Steer_info.SteerPWM_Error[0] > 800) 
								&& (Stop != Car_state)
								&& (Turn_Round != Car_state) && (Turn_Round != Car_state_Pre)
								&& (Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
							{
								Car_state_Pre = Car_state;
								Car_state = Stop;
							 
								Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								Parameter_info.StartEndLine_Flag = 1;
								 
								StopType = 222;
							}
						
						#endif
					 }	
					
					PORTD->ISFR |=(1 << Pin_12);//		�����жϱ�־		
					PORTD->PCR[Pin_12]|=PORT_PCR_ISF_MASK ; //�����жϱ�־
				}
				break;
			case 0x2000: 	//Pin_13,����
				{
						if(    (Stop != Car_state)
							&& (TRUE == Parameter_info.GetStartEndLine_Enable) 
							&& (Turn_Round != Car_state) 
							&& (Turn_Round != Car_state_Pre)
							&& (Parameter_info.Time >= Parameter_info.GetStartEndLineTime))
						{
							 LDPY_Delay_us(20);
							
							 if(LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==0)
							 {	
								 if(1 ==Parameter_info.CCD1_GetedStartEndLine_Flag)//CCD1Ԥ�������߳ɹ�
								 {
									 Car_state_Pre = Car_state;
									 Car_state = Stop;
									
									 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
									 
									 Parameter_info.StartEndLine_Flag = 1;
									 
									 StopType = 1001;
								 }
								 //�����һ������
								 else if((LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)==0) 
									 || (LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)==0))
								 {
									 Car_state_Pre = Car_state;
									 Car_state = Stop;
								 
									 Parameter_info.StopSpeed = Speed_info.Speed_Now;
									 
									 Parameter_info.StartEndLine_Flag = 1;
									 
									 StopType = 1;
									 
								 }
								 //���ͬ������
								else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
										&& (1 == Parameter_info.LongStraight_Flag)
										&& (0 == LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)))
								{
									 Car_state_Pre = Car_state;
									 Car_state = Stop;
								 
									 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
									 
									 Parameter_info.StartEndLine_Flag = 1;
									 
									 StopType = 112;
										 
								}
								else
								{
									//��ѯ��������
									for(ii = 0; ii < 10; ii++)
									{
										//����ʱ�ұߺ����3,4��200ms�������й���⵽�����ߣ���ͣ��
										if(((3 == Parameter_info.StartEndLine_Fifo[ii][0]) || (4 == Parameter_info.StartEndLine_Fifo[ii][0]))
											&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
										{
											Car_state_Pre = Car_state;
											Car_state = Stop;
										 
											Parameter_info.StopSpeed = Speed_info.Speed_Now; 
											 
											Parameter_info.StartEndLine_Flag = 1;
											 
											StopType = 134;
										}
										//���ͬ������
										else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
												&& (1 == Parameter_info.LongStraight_Flag)
												&& (2 == Parameter_info.StartEndLine_Fifo[ii][0])
												&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
										{
											 Car_state_Pre = Car_state;
											 Car_state = Stop;
										 
											 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
											 
											 Parameter_info.StartEndLine_Flag = 1;
											 
											 StopType = 112;
										}
									}
									 
									//û��ͣ�����¼�´�ʱ�ĺ���ܱ�ź��ж�ʱ��
									Parameter_info.StartEndLine_Fifo[0][0] = 1;
									Parameter_info.StartEndLine_Fifo[0][1] = Parameter_info.Time;
								}

							 }
						 }							  
								
						PORTD->ISFR |=(1 << Pin_13);//		�����жϱ�־		
						PORTD->PCR[Pin_13]|=PORT_PCR_ISF_MASK ; //�����жϱ�־
				}
				break;
			case 0x4000:	//Pin_14,����
				{
					if(    (Stop != Car_state)
						&& (TRUE == Parameter_info.GetStartEndLine_Enable) 
						&& (Turn_Round != Car_state) 
						&& (Turn_Round != Car_state_Pre)
						&& (Parameter_info.Time >= Parameter_info.GetStartEndLineTime))
					{
						 LDPY_Delay_us(20);
						
						 if(LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)==0)
						 {	
							 if(1 ==Parameter_info.CCD1_GetedStartEndLine_Flag)//CCD1Ԥ�������߳ɹ�
							 {
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
								
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 1004;
							 }
							 //�����һ������
							 else if((LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0)
								 || (LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==0))
							 {
								Car_state_Pre = Car_state;
								Car_state = Stop;
							 
								Parameter_info.StopSpeed = Speed_info.Speed_Now;
								 
								Parameter_info.StartEndLine_Flag = 1;
								 
								StopType = 4;
							 }
							 //���ͬ������
							else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
									&& (1 == Parameter_info.LongStraight_Flag)
									&& (0 == LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)))
							{
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
							 
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 443;
									 
							}
							else
							{
								//��ѯ��������
								for(ii = 0; ii < 10; ii++)
								{
									//����ʱ��ߺ����1,2��200ms�������й���⵽�����ߣ���ͣ��
									if(((1 == Parameter_info.StartEndLine_Fifo[ii][0]) || (2 == Parameter_info.StartEndLine_Fifo[ii][0]))
										&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
									{
										Car_state_Pre = Car_state;
										Car_state = Stop;
									 
										Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										Parameter_info.StartEndLine_Flag = 1;
										 
										StopType = 412;
									}
									//���ͬ������
									else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
											&& (1 == Parameter_info.LongStraight_Flag)
											&& (3 == Parameter_info.StartEndLine_Fifo[ii][0])
											&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
									{
										 Car_state_Pre = Car_state;
										 Car_state = Stop;
									 
										 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										 Parameter_info.StartEndLine_Flag = 1;
										 
										 StopType = 443;
									}
								}
								 
								//û��ͣ�����¼�´�ʱ�ĺ���ܱ�ź��ж�ʱ��
								Parameter_info.StartEndLine_Fifo[0][0] = 4;
								Parameter_info.StartEndLine_Fifo[0][1] = Parameter_info.Time;
							}
						 }
					 }
														  
					PORTD->ISFR |=(1 << Pin_14);//		�����жϱ�־		
					PORTD->PCR[Pin_14]|=PORT_PCR_ISF_MASK ; //�����жϱ�־
				
				}
				break;
			case 0x8000:	//Pin_15,��
				{
					 LDPY_Delay_us(20);
					 if(LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == 0)
					 {
						 //���������
						 if(	(Stop != Car_state)
							 && (TRUE == Parameter_info.GetStartEndLine_Enable) 
							 && (Turn_Round != Car_state) 
							 && (Turn_Round != Car_state_Pre)
							 && (Parameter_info.Time >= Parameter_info.GetStartEndLineTime))
						{
							 if(1 ==Parameter_info.CCD1_GetedStartEndLine_Flag)//CCD1Ԥ�������߳ɹ�
							 {
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
								
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 1003;
							 }
							 //�����һ������
							 else if((LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0)
								 || (LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)==0))
							 {
								Car_state_Pre = Car_state;
								Car_state = Stop;
							 
								Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								Parameter_info.StartEndLine_Flag = 1;
								 
								StopType = 3;
							 }
							 //���ͬ������
							else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
									&& (1 == Parameter_info.LongStraight_Flag)
									&& (0 == LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)))
							{
								 Car_state_Pre = Car_state;
								 Car_state = Stop;
							 
								 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								 Parameter_info.StartEndLine_Flag = 1;
								 
								 StopType = 334;
									 
							}
							else
							{
								//��ѯ��������
								for(ii = 0; ii < 10; ii++)
								{
									//����ʱ�ұߺ����1,2��200ms�������й���⵽�����ߣ���ͣ��
									if(((1 == Parameter_info.StartEndLine_Fifo[ii][0]) || (2 == Parameter_info.StartEndLine_Fifo[ii][0]))
										&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
									{
										Car_state_Pre = Car_state;
										Car_state = Stop;
									 
										Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										Parameter_info.StartEndLine_Flag = 1;
										 
										StopType = 312;
									}
									//���ͬ������
									else if((TRUE == Parameter_info.GetStartEndLine_SameLight_Enable)
											&& (1 == Parameter_info.LongStraight_Flag)
											&& (4 == Parameter_info.StartEndLine_Fifo[ii][0])
											&& (Parameter_info.Time - Parameter_info.StartEndLine_Fifo[ii][1] < StartEndLine_TIME))
									{
										 Car_state_Pre = Car_state;
										 Car_state = Stop;
									 
										 Parameter_info.StopSpeed = Speed_info.Speed_Now; 
										 
										 Parameter_info.StartEndLine_Flag = 1;
										 
										 StopType = 334;
									}
								}
								 
								//û��ͣ�����¼�´�ʱ�ĺ���ܱ�ź��ж�ʱ��
								Parameter_info.StartEndLine_Fifo[0][0] = 3;
								Parameter_info.StartEndLine_Fifo[0][1] = Parameter_info.Time;
							}
						 }
						 
						//���籣��
						#ifdef PROTECT_CAR_ENABLE
						 
							if((Steer_info.SteerPWM_Error[0] < -800) 
								&& (Stop != Car_state)
								&& (Turn_Round != Car_state) && (Turn_Round != Car_state_Pre)
								&& (Ramp_Up != Road_condition) && (Ramp_Down != Road_condition))
							{
								Car_state_Pre = Car_state;
								Car_state = Stop;
							 
								Parameter_info.StopSpeed = Speed_info.Speed_Now; 
								 
								Parameter_info.StartEndLine_Flag = 1;
								 
								StopType = 333;
							}
							
						#endif
					 }
													  
					PORTD->ISFR |=(1 << Pin_15);//		�����жϱ�־		
					PORTD->PCR[Pin_15]|=PORT_PCR_ISF_MASK ; //�����жϱ�־
				
				}
				break;
			case 0x0800: //ң����
				{
					#ifdef Remote_Control_Enable
			
						 LDPY_Delay_us(25);
					
						 if(LDPY_GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 1)
						 {	
//							if((Stop != Car_state) && (0 == RemoteStopNum))//��һ�μ�⵽ң����
//							{
//								RemoteStopNum++;//��¼ң����������
//								RemoteGetTime = Parameter_info.Time;//��¼��ʱ��ʱ��
//							}
//							else if((Stop != Car_state) && (RemoteStopNum > 0))//�Ѿ���⵽��ң����
//							{
//								if((Parameter_info.Time - RemoteGetTime >= 0.2) && ((Parameter_info.Time - RemoteGetTime <= 1.5)))//300ms����,2s����
//								{
//									if(Stop != Car_state)//ֻͣ��������������
									{
										Car_state_Pre = Car_state;
										Car_state = Stop;
										
										Parameter_info.StopSpeed = Speed_info.Speed_Now;
										
										Parameter_info.StartEndLine_Flag = 1;
										
										StopType = 6;
									}
//								}
//								else
//								{
//									RemoteStopNum = 0;
//								}
//							}
						 }
						 
					#endif 
						 
						PORTD->ISFR |=(1 << Pin_11);//		�����жϱ�־		
						PORTD->PCR[Pin_11]|=PORT_PCR_ISF_MASK ; //�����жϱ�־	
				}
				break;
			default:
				{
					PORTD->ISFR = 0xFFFFFFFF;//����ȫ���ж�
				}
				break;
		}
	}
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  UART5_RX_TX_IRQHandler
*  ����˵��:  �����жϷ�����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-16
*  ��	ע:
*************************************************************************/ 		
void UART5_RX_TX_IRQHandler(void)
{ 
	Un_ENABLE_IRQ(55);			//�ر�UART5�����ж�
	
#ifdef LabVIEW_Control_Enable//������λ������״̬
	
	Parameter_info.UART_RecvData = LDPY_UART5_GetChar();//��ȡ��һ���ַ�	
	
	if(('Q' == Parameter_info.UART_RecvData) && (Stop != Car_state))//�յ�ͣ��ָ��
	{
		Car_state_Pre = Car_state;
		Car_state = Stop;
		
		Parameter_info.StopSpeed = Speed_info.Speed_Now;
		
		Parameter_info.StartEndLine_Flag = 1;
		
		StopType = 7;
	}
	
#endif
	
	ENABLE_IRQ(55);				//ʹ��UART5�����ж�
}

/*************************************************************************
*				��	��	ƫ	��    
*
*  ��������:PIT0_IRQHandler
*  ����˵��:PIT0 �жϷ�����,CCD���ݲɼ��Ͷ�ȡ��������ֵ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-15
*  ��    ע:2��CCD�ɼ�ʱ��Ϊ1.40ms(12λAD),3��CCD�ɼ�ʱ��Ϊ2.10ms(12λAD)
*************************************************************************/	
void PIT0_IRQHandler(void)
{
	//CCD���ݲ���
	myCCD_CCD1_Sampling(&CCD1_info);  
	
	myCCD_CCD2_Sampling(&CCD2_info);
	
	myCCD_CCD3_Sampling(&CCD3_info);

	//�ٶȲɼ�
	Speed_info.EncoderData = FTM1->CNT;						//��ȡFTM1�Ĵ�����ֵ
	Speed_info.Speed_Direction = ((uint8_t)FTM1->QDCTRL & 0x04) >> 2;//��ȡ�ٶȷ���
	
	FTM1->CNT = 0;    								// ����������
	
	//�����ǽ����µ��ɼ�
	if(((Go_Forward == Car_state) && (TRUE == Parameter_info.GetRamp_Enable)
		&& (Parameter_info.Time >= Parameter_info.GetRampTime))
		|| ((Ramp_Up == Road_condition) || (Ramp_Down == Road_condition)))
	{
		Gyro_info.Gyroscope_Fifo[0] = LDPY_ADC1_ENC();//�ɼ����������ݶ���
	}
	
	Parameter_info.CCD_SampleOK_Flag = 1;			//��λ���ݲɼ���ɱ�־

	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;		//����жϱ�־λ
}

/*************************************************************************
*				��	��	ƫ	��    
*
*  ��������:LDPY_Isr_SetIrqPriority
*  ����˵��:�����ж����ȼ�
*  ����˵��:	irq Ϊ�ж�������	
*			prioΪ���ȼ���0Ϊ������ȼ���Ȼ��1,2��3.....���ν���
*  ��������:
*  �޸�ʱ��:2014-05-14
*  ��    ע:
*************************************************************************/	
void LDPY_Isr_SetIrqPriority (int irq, int prio)
{
    /*irq priority pointer*/
    uint8 *prio_reg;
    
    /* Determine which of the NVICIPx corresponds to the irq */
    prio_reg = (uint8 *)(((uint32)&NVIC->IP) + irq);
    /* Assign priority to IRQ */
    *prio_reg = ( (prio&0xF) << (8 - ARM_INTERRUPT_LEVEL_BITS) );             
}

void NMI_Handler(void)
{
	//�رյ��
	FTM0->CONTROLS[7].CnV = 0;			
	FTM0->CONTROLS[6].CnV = 0;
	
}

void HardFault_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void MemManage_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void BusFault_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void UsageFault_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void SVC_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void DebugMon_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void PendSV_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}

void SysTick_Handler(void)
{
	while(1)
	{
		//�رյ��
		FTM0->CONTROLS[7].CnV = 0;			
		FTM0->CONTROLS[6].CnV = 0;	
		
		/*******���Ӳ�������������1��********/
		Bell_ON;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		Bell_OFF;
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);
		LDPY_Delay_ms(100);		
		LDPY_Delay_ms(100);
	}
}
/*-------------------------------end of LDPY_isr.c-----------------------------*/
