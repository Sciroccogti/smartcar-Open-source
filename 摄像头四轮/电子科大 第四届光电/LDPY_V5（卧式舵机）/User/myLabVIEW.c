
#include "myLabVIEW.h"

#ifdef DMA_CH2_SendLabVIEW_Info_Enable

	LabVIEW_Info LabVIEW_info = {0};
	
#endif

//�ⲿ��������
extern Parameter_Info Parameter_info;

extern CCD_Info CCD1_info;
extern CCD_Info CCD2_info;
extern CCD_Info CCD3_info;

extern Car_State Car_state;
extern Car_State Car_state_Pre;

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_UART_Init
*  ����˵��������λ��ͨ�ų�ʼ�����ã������ʵ�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_UART_Init(uint32 baud)
{
  uint8 Start[2]={0};

  LDPY_UART5_Init(baud);							//��ʼ������UART5��������baud(115200)
  Bluetooth_Break();								//��������ָʾ��
  
  LDPY_UART5_ClearReceiveBuffer();					//���UART5�еĽ��ջ�����������
  
  do
  {
    Start[0] = Start[1];
    Start[1] = LDPY_UART5_GetChar();
	  
  }while(('T' != Start[0]) || ('S' != Start[1]));	//�����յ��Ĵ������ݲ���'SD'ʱ��˵�����������͵����룬����һֱ��ȡ����ս��ջ�����
  
  if((Start[0] == 'T') && (Start[1] == 'S'))		//���յ����ַ���Ϊ��λ����������������
  {
	  LDPY_UART5_PutChar('O');						//���յ�������������ɹ��󷵻�"O"
  }
  
  Bluetooth_Connected();							//Ϩ����������ָʾ��
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_UART5ReceiveIsr
*  ����˵����UART5���ڽ����жϺ���,�����ڳ������й����н�����Ϣ
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_ParametersInit(  Parameter_Info *Parameter_info,
								CCD_Info *CCD_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info
							 )
{	
  	uint8 UART5_RecvData = 0;
    
	UART5_RecvData = LDPY_UART5_GetChar();//����һ���ֽڣ�֡ͷ��
	
	if('O' == UART5_RecvData)//�趨��ʼ������
	{
	  	myLabVIEW_ParametersSet(Parameter_info,CCD_info,Steer_info,Motor_info,Speed_info);
	
		LDPY_UART5_PutCharArr("OK",2);//���Ͳ����趨��ɱ�־	
		
		LDPY_UART5_PutChar('x');
		LDPY_UART5_PutChar('x');
			  
		//--------------���Ͷ��PID��Ϣ����λ��---------------//
		LDPY_UART5_PutChar('P');//��д��ĸ
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KP_Mult);
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KI_Mult);
		myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KD_Mult);
					  
		//--------------���͵��PID��Ϣ����λ��---------------//
		LDPY_UART5_PutChar('p');//Сд��ĸ
		myLabVIEW_Send_Float(Motor_info->KP,Motor_info->KP_Mult);
		myLabVIEW_Send_Float(Motor_info->KI,Motor_info->KI_Mult);
		myLabVIEW_Send_Float(Motor_info->KD,Motor_info->KD_Mult);
					  
		//--------------������Ӧ���ٶ�ֵ---------------------//
		LDPY_UART5_PutChar('s');//Сд��ĸ		
		myLabVIEW_Send_Uint16(Speed_info->Straight_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Cur_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Snake_Speed);
		myLabVIEW_Send_Uint16(Speed_info->TurnRound_Speed);
		myLabVIEW_Send_Uint16(Speed_info->Obstacle_Speed);
		
		Parameter_info->Parameter_OK = 1;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_DebugParameters
*  ����˵��������С������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_DebugParameters( Parameter_Info *Parameter_info,
								CCD_Info *CCD_info,
								Steer_Info *Steer_info,
								Motor_Info *Motor_info,
								Speed_Info *Speed_info
							  )
{
	do
	{
		LDPY_UART5_PutChar('x');
		
	}while(('Q' != Parameter_info->UART_RecvData) && (0 == (UART_X->S1 & UART_S1_RDRF_MASK)));//�����ջ�����û��ֵ��ʱ��
	
	switch(Parameter_info->UART_RecvData)
	{
		case 'Q'://--ͣ��/����
				{
					if(Stop != Car_state)//ֻͣ����������
					{
						Car_state_Pre = Car_state;
						Car_state = Stop;
						
						Parameter_info->StopSpeed = Speed_info->Speed_Now;
						
						Parameter_info->StartEndLine_Flag = 1;
					}
				}
				break;//end of 'Q'
				
		case 'S':
				{  	
					myLabVIEW_GetSteerDebugNews(Steer_info);
			
					//���Ͷ������
					LDPY_UART5_PutChar('S');
					myLabVIEW_Send_Uint16((uint16)Steer_info->Steer_PWMOutputDuty);	//���ʵ��PWMֵ
					myLabVIEW_Send_Uint16((uint16)(CCD_info->LineError[0] + 1000));//����1000��Ϊ��ת��Ϊ��������						//ʵ������ƫ��
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'S'
				
		case 's'://�趨��ǰ�ĸ����ٶ�
				{
					myLabVIEW_GetSpeedSettingNews(Speed_info);
					
					//--------------������Ӧ���ٶ�ֵ------------//
					LDPY_UART5_PutChar('s');//Сд��ĸ		
					myLabVIEW_Send_Uint16(Speed_info->Straight_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Cur_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Snake_Speed);
					myLabVIEW_Send_Uint16(Speed_info->TurnRound_Speed);
					myLabVIEW_Send_Uint16(Speed_info->Obstacle_Speed);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 's'
				
		case 'P':	//���PID��������
				{
					myLabVIEW_GetSteerPIDNews(Steer_info);
					
					//------------------���Ͷ��PID��Ϣ����λ��------------//
					LDPY_UART5_PutChar('P');
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KP_Mult);
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KI_Mult);
					myLabVIEW_Send_Float(Steer_info->KI,Steer_info->KD_Mult);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'P'
				
		case 'p':	//���PID��������
				{
					myLabVIEW_GetMotorPIDNews(Motor_info);
					
					//���͵��PID��Ϣ����λ��
					LDPY_UART5_PutChar('p');
					myLabVIEW_Send_Float(Motor_info->KP,Motor_info->KP_Mult);
					myLabVIEW_Send_Float(Motor_info->KI,Motor_info->KI_Mult);
					myLabVIEW_Send_Float(Motor_info->KD,Motor_info->KD_Mult);
					
					Parameter_info->Parameter_OK = 1;
				}
				break;//end of 'p'
				
		default:
				break;
			
	}

	LDPY_UART5_PutChar('x');
	
//	DMA0->ERQ |= (1<<dma_init_struct.DMA_CHx);//��DMA
	
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_GetSteerDebugNews
*  ����˵�������ն���ĵ���PWM�����ڵ��Զ�������Ҽ��޺�����ֵ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_GetSteerDebugNews(Steer_Info *Steer_info)
{
  	uint8 SteerPWMDuty[2] = {0};//���ڽ���PWM�����ֽڵ�����
	
	SteerPWMDuty[0] = LDPY_UART5_GetChar();//���ո��ֽ�
	SteerPWMDuty[1] = LDPY_UART5_GetChar();//���յ��ֽ�
	
  	Steer_info->Steer_PWMOutputDuty =(uint32)(((uint16)SteerPWMDuty[0] << 8) | SteerPWMDuty[1]); 
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_GetSteerPIDNews
*  ����˵�������ն����PID����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_GetSteerPIDNews(Steer_Info *Steer_info)
{
  	uint8 SteerPID_KP[3] = {0};//���ڽ���PID�������������ֽڣ��Ŵ�����һ���ֽڣ�
	uint8 SteerPID_KI[3] = {0};
	uint8 SteerPID_KD[3] = {0};
	
	SteerPID_KP[0] = LDPY_UART5_GetChar();		//����KP���ֽ�
	SteerPID_KP[1] = LDPY_UART5_GetChar();		//����KP���ֽ�
	SteerPID_KP[2] = LDPY_UART5_GetChar();		//���շŴ���
	
	SteerPID_KI[0] = LDPY_UART5_GetChar();		//����KI���ֽ�
	SteerPID_KI[1] = LDPY_UART5_GetChar();		//����KI���ֽ�
	SteerPID_KI[2] = LDPY_UART5_GetChar();		//���շŴ���
	
	SteerPID_KD[0] = LDPY_UART5_GetChar();		//����KD���ֽ�
	SteerPID_KD[1] = LDPY_UART5_GetChar();		//����KD���ֽ�
	SteerPID_KD[2] = LDPY_UART5_GetChar();		//���շŴ���
	
  	Steer_info->KI = (float)(((uint16)SteerPID_KP[0] << 8) | SteerPID_KP[1])/(pow(10,SteerPID_KP[2])); 
	Steer_info->KP_Mult = SteerPID_KP[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KI[0] << 8) | SteerPID_KI[1])/(pow(10,SteerPID_KI[2])); 
	Steer_info->KI_Mult = SteerPID_KI[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KD[0] << 8) | SteerPID_KD[1])/(pow(10,SteerPID_KD[2])); 
	Steer_info->KD_Mult = SteerPID_KD[2];
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_GetMotorPIDNews
*  ����˵�������յ����PID����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_GetMotorPIDNews(Motor_Info *Motor_info)
{
  	uint8 MotorPID_KP[3] = {0};//���ڽ���PID�������������ֽڣ��Ŵ�����һ���ֽڣ�
	uint8 MotorPID_KI[3] = {0};
	uint8 MotorPID_KD[3] = {0};
	
	MotorPID_KP[0] = LDPY_UART5_GetChar();		//����KP���ֽ�
	MotorPID_KP[1] = LDPY_UART5_GetChar();		//����KP���ֽ�
	MotorPID_KP[2] = LDPY_UART5_GetChar();		//���շŴ���
	
	MotorPID_KI[0] = LDPY_UART5_GetChar();		//����KI���ֽ�
	MotorPID_KI[1] = LDPY_UART5_GetChar();		//����KI���ֽ�
	MotorPID_KI[2] = LDPY_UART5_GetChar();		//���շŴ���
	
	MotorPID_KD[0] = LDPY_UART5_GetChar();		//����KD���ֽ�
	MotorPID_KD[1] = LDPY_UART5_GetChar();		//����KD���ֽ�
	MotorPID_KD[2] = LDPY_UART5_GetChar();		//���շŴ���
	
  	Motor_info->KP = (float)(((uint16)MotorPID_KP[0] << 8) | MotorPID_KP[1])/(pow(10,MotorPID_KP[2])); 
	Motor_info->KP_Mult = MotorPID_KP[2];
	Motor_info->KI = (float)(((uint16)MotorPID_KI[0] << 8) | MotorPID_KI[1])/(pow(10,MotorPID_KI[2])); 
	Motor_info->KI_Mult = MotorPID_KI[2];
	Motor_info->KD = (float)(((uint16)MotorPID_KD[0] << 8) | MotorPID_KD[1])/(pow(10,MotorPID_KD[2])); 
  	Motor_info->KD_Mult = MotorPID_KD[2];
	
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_GetSpeedSettingNews
*  ����˵�������ո���·���µ��ٶ��趨
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_GetSpeedSettingNews(Speed_Info *Speed_info)
{
  	uint8 Speed[10] = {0};//���ڽ����趨���ٶȣ������ֽڣ�
	
	Speed[0] = LDPY_UART5_GetChar();			//����ֱ���ٶȸ��ֽ�
	Speed[1] = LDPY_UART5_GetChar();			//����ֱ���ٶȵ��ֽ�
	
	Speed[2] = LDPY_UART5_GetChar();			//������ͨ����ٶȸ��ֽ�
	Speed[3] = LDPY_UART5_GetChar();			//������ͨ����ٶȵ��ֽ�	
	
	Speed[4] = LDPY_UART5_GetChar();			//����СS����ٶȸ��ֽ�
	Speed[5] = LDPY_UART5_GetChar();			//����СS����ٶȵ��ֽ�	
	
	Speed[6] = LDPY_UART5_GetChar();			//���յ�ͷ�����ٶȸ��ֽ�
	Speed[7] = LDPY_UART5_GetChar();			//���յ�ͷ�����ٶȵ��ֽ�
	
	Speed[8] = LDPY_UART5_GetChar();			//����·���ٶȸ��ֽ�
	Speed[9] = LDPY_UART5_GetChar();			//����·���ٶȵ��ֽ�
	
	Speed_info->Straight_Speed  = ((uint16)Speed[0] << 8) | Speed[1];
	Speed_info->Cur_Speed 	    = ((uint16)Speed[2] << 8) | Speed[3];
	Speed_info->Snake_Speed     = ((uint16)Speed[4] << 8) | Speed[5];
	Speed_info->TurnRound_Speed = ((uint16)Speed[6] << 8) | Speed[7];
	Speed_info->Obstacle_Speed  = ((uint16)Speed[8] << 8) | Speed[9];
  
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_Send_Uint16
*  ����˵��������˫�ֽ�����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע���ȷ��͵�8λ���ٷ��͸�8λ
*************************************************************************/
void myLabVIEW_Send_Uint16(uint16 num)
{
  char x;
  uint16 uint16_num = num;
  
  x = uint16_num & 0xFF;//��8λ
  LDPY_UART5_PutChar(x);
  
  x = (uint16_num & 0xFF00)>>8;//��8λ
  LDPY_UART5_PutChar(x);
}

/*************************************************************************
*					��		��		ƫ		��   
*
*  �������ƣ�myLabVIEW_Send_Uint32
*  ����˵�����������ֽ�����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_Send_Uint32(uint32 num)
{
  char x;
  uint32 uint32_num = num;
  
  x = uint32_num & 0xFF;//��8λ
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF00)>>8;//����8λ
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF0000)>>8;//����8λ
  LDPY_UART5_PutChar(x);
  
  x = (uint32_num & 0xFF000000)>>8;//��8λ
  LDPY_UART5_PutChar(x);
  
}

/*************************************************************************
*					��		��		ƫ		��   
*
*  �������ƣ�myLabVIEW_Send_Float
*  ����˵������float�͵����ݷ��͸���λ��
*  ����˵��������ΪҪ���͵�float�����ݺͷŴ���
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_Send_Float(float Float_In,uint8 Mult)
{
	  uint16 Data = 0;
	  
	  Data = (uint16)(Float_In*pow(10,Mult)); 
	  
	  myLabVIEW_Send_Uint16(Data);			//���ͷŴ������
	  LDPY_UART5_PutChar(Mult);				//���ͷŴ���
  
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_ParameterSet
*  ����˵��������С���Ĳ���������PID��CCD�߶ȣ�ǰհ���ع�ʱ��,
*  	     ����ĳ�ʼPID,����ĳ�ʼPID����·�����ٶȵ�,
*	     ����������Ϣ���͸���λ����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_ParametersSet(Parameter_Info *Parameter_info,CCD_Info *CCD_info,Steer_Info *Steer_info,Motor_Info *Motor_info,Speed_Info *Speed_info)
{   
	uint8 SpeedCalculate_Enable = 0;			//��ŵ����/�ջ�����
	
	uint8 Speed[10] = {0};						//�����趨���ٶ�
  	
	uint8 SteerPID_KP[3] = {0};					//���ն��PID�������������ֽڣ��Ŵ�����һ���ֽڣ�
	uint8 SteerPID_KI[3] = {0};
	uint8 SteerPID_KD[3] = {0};
	
	uint8 MotorPID_KP[3] = {0};					//���յ��PID�������������ֽڣ��Ŵ�����һ���ֽڣ�
	uint8 MotorPID_KI[3] = {0};
	uint8 MotorPID_KD[3] = {0};
	
	SpeedCalculate_Enable = LDPY_UART5_GetChar();	//�����ٶȼ��㿪��
	
	SteerPID_KP[0] = LDPY_UART5_GetChar();			//����KP���ֽ�
	SteerPID_KP[1] = LDPY_UART5_GetChar();			//����KP���ֽ�
	SteerPID_KP[2] = LDPY_UART5_GetChar();			//���շŴ���
	
	SteerPID_KI[0] = LDPY_UART5_GetChar();			//����KI���ֽ�
	SteerPID_KI[1] = LDPY_UART5_GetChar();			//����KI���ֽ�
	SteerPID_KI[2] = LDPY_UART5_GetChar();			//���շŴ���
		
	SteerPID_KD[0] = LDPY_UART5_GetChar();			//����KD���ֽ�
	SteerPID_KD[1] = LDPY_UART5_GetChar();			//����KD���ֽ�
	SteerPID_KD[2] = LDPY_UART5_GetChar();			//���շŴ���
	
	MotorPID_KP[0] = LDPY_UART5_GetChar();			//����KP���ֽ�
	MotorPID_KP[1] = LDPY_UART5_GetChar();			//����KP���ֽ�
	MotorPID_KP[2] = LDPY_UART5_GetChar();			//���շŴ���
	
	MotorPID_KI[0] = LDPY_UART5_GetChar();			//����KI���ֽ�
	MotorPID_KI[1] = LDPY_UART5_GetChar();			//����KI���ֽ�
	MotorPID_KI[2] = LDPY_UART5_GetChar();			//���շŴ���
	
	MotorPID_KD[0] = LDPY_UART5_GetChar();			//����KD���ֽ�
	MotorPID_KD[1] = LDPY_UART5_GetChar();			//����KD���ֽ�
	MotorPID_KD[2] = LDPY_UART5_GetChar();			//���շŴ���
	
	Speed[0] = LDPY_UART5_GetChar();				//����ֱ���ٶȸ��ֽ�
	Speed[1] = LDPY_UART5_GetChar();				//����ֱ���ٶȵ��ֽ�
	
	Speed[2] = LDPY_UART5_GetChar();				//������ͨ����ٶȸ��ֽ�
	Speed[3] = LDPY_UART5_GetChar();				//������ͨ����ٶȵ��ֽ�	
	
	Speed[4] = LDPY_UART5_GetChar();				//����СS����ٶȸ��ֽ�
	Speed[5] = LDPY_UART5_GetChar();				//����СS����ٶȵ��ֽ�	
	
	Speed[6] = LDPY_UART5_GetChar();				//���յ�ͷ�����ٶȸ��ֽ�
	Speed[7] = LDPY_UART5_GetChar();				//���յ�ͷ�����ٶȵ��ֽ�
	
	Speed[8] = LDPY_UART5_GetChar();				//����·���ٶȸ��ֽ�
	Speed[9] = LDPY_UART5_GetChar();				//����·���ٶȵ��ֽ�
	
  	Steer_info->KI = (float)(((uint16)SteerPID_KP[0] << 8) | SteerPID_KP[1])/(pow(10,SteerPID_KP[2])); 
	Steer_info->KP_Mult = SteerPID_KP[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KI[0] << 8) | SteerPID_KI[1])/(pow(10,SteerPID_KI[2])); 
	Steer_info->KI_Mult = SteerPID_KI[2];
	Steer_info->KI = (float)(((uint16)SteerPID_KD[0] << 8) | SteerPID_KD[1])/(pow(10,SteerPID_KD[2])); 
	Steer_info->KD_Mult = SteerPID_KD[2];
	
	Motor_info->KP = (float)(((uint16)MotorPID_KP[0] << 8) | MotorPID_KP[1])/(pow(10,MotorPID_KP[2])); 
	Motor_info->KP_Mult = MotorPID_KP[2];
	Motor_info->KI = (float)(((uint16)MotorPID_KI[0] << 8) | MotorPID_KI[1])/(pow(10,MotorPID_KI[2])); 
	Motor_info->KI_Mult = MotorPID_KI[2];
	Motor_info->KD = (float)(((uint16)MotorPID_KD[0] << 8) | MotorPID_KD[1])/(pow(10,MotorPID_KD[2])); 
  	Motor_info->KD_Mult = MotorPID_KD[2];
	
	Speed_info->Straight_Speed  = ((uint16)Speed[0] << 8) | Speed[1];
	Speed_info->Cur_Speed 	    = ((uint16)Speed[2] << 8) | Speed[3];
	Speed_info->Snake_Speed     = ((uint16)Speed[4] << 8) | Speed[5];
	Speed_info->TurnRound_Speed = ((uint16)Speed[6] << 8) | Speed[7];
	Speed_info->Obstacle_Speed  = ((uint16)Speed[8] << 8) | Speed[9];
 
	if(1 == SpeedCalculate_Enable)//����ջ�
	{
		Speed_info->SpeedCalculate_Enable = TRUE;
	}
	else//�������
	{
		Speed_info->SpeedCalculate_Enable =	FALSE;
	}
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_SendCCDValueOfTwoCCDs
*  ����˵��������CCD��������ݸ���λ��������CCD���ݣ��ع�ʱ�䣬ƽ����ѹ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��
*************************************************************************/
void myLabVIEW_SendCCDValueOfTwoCCDs(CCD_Info *CCD1_info,CCD_Info *CCD2_info)
{
	uint8 ii;
	
	/*-----------------------����CCD1�ɼ�������-----------------------*/
	LDPY_UART5_PutChar('C');//֡ͷ����дC
	for(ii = 0; ii < 128; ii++)
	{	
		LDPY_UART5_PutChar((uint8)(CCD1_info->Pixel[ii] >> 4));	//�����˲��������
	}
	
	/*-----------------------����CCD2�ɼ�������-----------------------*/
	LDPY_UART5_PutChar('c');//֡ͷ��Сдc
	for(ii = 0; ii < 128; ii++)
	{	
		LDPY_UART5_PutChar((uint8)(CCD2_info->Pixel[ii] >> 4));	//�����˲��������
	}
	
	/*--------------------����CCD1���ݴ����ĸ���ֵ--------------------*/
	LDPY_UART5_PutChar('R');							//֡ͷ
	LDPY_UART5_PutChar(CCD1_info->LeftLine[0]);			//��߽��
	LDPY_UART5_PutChar(CCD1_info->RightLine[0]);		//�ұ߽��
	LDPY_UART5_PutChar(CCD1_info->CentralLine[0] + 64);	//���ߵ�
	LDPY_UART5_PutChar(CCD1_info->LineError[0] + 1000);	//�е�ƫ�����100��Ϊ��ת��Ϊ�������ͣ���λ��Ӧ�ü�ȥ64

	/*--------------------����CCD2���ݴ����ĸ���ֵ--------------------*/
	LDPY_UART5_PutChar('r');							//֡ͷ
	LDPY_UART5_PutChar(CCD2_info->LeftLine[0]);			//��߽��
	LDPY_UART5_PutChar(CCD2_info->RightLine[0]);		//�ұ߽��
	LDPY_UART5_PutChar(CCD2_info->CentralLine[0] + 64);	//���ߵ�
	LDPY_UART5_PutChar(CCD2_info->LineError[0] + 1000);	//�е�ƫ�����100��Ϊ��ת��Ϊ�������ͣ���λ��Ӧ�ü�ȥ64
	
	LDPY_UART5_PutChar('O');							//�������ݱ�������
	
}

/*************************************************************************
*					��		��		ƫ		��  
*
*  �������ƣ�myLabVIEW_SendCCDBianaryValue
*  ����˵��������CCD��ֵ�����������ݸ���λ��������CCD���ݣ��ع�ʱ�䣬ƽ����ѹ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2014-06-21
*  ��    ע��ѹ�����128�������ȷ��͵��ֽ�(0)���ٷ��͸��ֽ�(127)
*************************************************************************/
void myLabVIEW_SendCCDBianaryValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8 ii = 0;
	
	//ѹ��CCD��ֵ���������
	for(ii = 0; ii < 16; ii++)
	{
		CCD1_info->CCD_PhotoValue[ii] = 	(CCD1_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD1_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD1_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD1_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD1_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD1_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD1_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD1_info->PixelBinary[0 + 8*ii])
										;
	
		CCD2_info->CCD_PhotoValue[ii] = 	(CCD2_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD2_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD2_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD2_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD2_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD2_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD2_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD2_info->PixelBinary[0 + 8*ii])
										;
		
		CCD3_info->CCD_PhotoValue[ii] = 	(CCD3_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD3_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD3_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD3_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD3_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD3_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD3_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD3_info->PixelBinary[0 + 8*ii])
										;
	}
	
	//-------------------����3��CCD��ѹ���������--------------------//
	//CCD1
	LDPY_UART5_PutChar('G');
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD1_info->CCD_PhotoValue[15]);
	
	//CCD2
	LDPY_UART5_PutChar('g');
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD2_info->CCD_PhotoValue[15]);
	
	//CCD3
	LDPY_UART5_PutChar('H');
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[0]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[1]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[2]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[3]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[4]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[5]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[6]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[7]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[8]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[9]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[10]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[11]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[12]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[13]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[14]);
	LDPY_UART5_PutChar(CCD3_info->CCD_PhotoValue[15]);

}
/*---------------------------end of myLabVIEW.c-------------------------------------*/
