#include "System.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��

uint8 ImageBuff[CAMERA_W*CAMERA_H/8];							// ͼ��ɼ������ַ
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);	// ͼ���ַ��������λ���۲�
uint8 Img[CAMERA_W*CAMERA_H]; //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��

uchar Image_Send_Flag = 0;	// ͼ���Ϳ��ƣ�1Ϊ���ͣ�0Ϊ������
uchar Send_OK = 0;
uchar System_OK = 0;
uchar HMI_OK = 0;

void System_Init()
{
	uint32 i, j;

	/************************ ���� K60 �����ȼ�  *****************************/

	NVIC_SetPriorityGrouping(4);            // �������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�

	NVIC_SetPriority(PORTA_IRQn,0);         // �������ȼ�
	NVIC_SetPriority(PORTE_IRQn,1);         // �������ȼ�
	NVIC_SetPriority(DMA0_IRQn,3);          // �������ȼ�
	NVIC_SetPriority(PIT0_IRQn,4);          // �������ȼ�
	NVIC_SetPriority(PIT1_IRQn,5);          // �������ȼ�
	NVIC_SetPriority(UART4_RX_TX_IRQn,6);   // �������ȼ�

	
	/************************ ������ʼ�� **********************************/
	Image_Para_Init();
	PID_Parameter_Init(&MOTOR_PID);		// �ٶȻ�PID������ʼ��
	PID_Parameter_Init(&Angle_PID);		// �ǶȻ�PID������ʼ��
	PID_Parameter_Init(&Ang_Vel_PID);	// ���ٶȻ�PID������ʼ��
	PID_Parameter_Init(&Direct_PID);	// ת��PID������ʼ��
	PID_Parameter_Init(&Distance_PID);	// λ�û�PID������ʼ��
	Radius = 0;				// ��ʼ��Ŀ��ת��뾶����Ϊ0
	Speed_Set = 0;			// ��ʼ��Ŀ���ٶ�Ϊ0
	Target_Angle.Y = Zero_Angle*100;
	
	
	/************************ ��� ��ʼ�� ************************************/
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);	// ��ʼ�� ��� PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);	// ��ʼ�� ��� PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);	// ��ʼ�� ��� PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);	// ��ʼ�� ��� PWM
	
	
	/************************ ���� ��ʼ�� ************************************/
	uart_init(UART4, 460800);	// ����
	uart_init(UART0, 115200);	// HMIҺ����
	set_vector_handler(UART4_RX_TX_VECTORn ,UART4_IRQHandler);	// ���� UART4 ���жϷ�����Ϊ UART4_IRQHandler
	uart_rx_irq_en(UART4);		// ������4�����ж�
	HMI_Send_End();				// ����һ�ν�����������ϵ�����Ĵ����Ӳ�����
	
	
	/********************** HMI���������շ�   **********************/ 
	while (!HMI_OK)
	{
		HMI_Data_Handle();
	}
	
	/************************ ������ ��ʼ�� **********************************/
	MPU6050_OPEN();
	while (mpu_dmp_init());		// ��ʼ��DMP
//	while (MPU6050_Init());		// �����ǳ�ʼ���ɹ�����0
	MPU6050_Offset();
//	LCD_init();	// ������Ϊʲô��Ҫ������ͷ��ͼ��ͱ���������������ע��ܽ�������ͻ��ֻ�����ν��
	
	for (i = 0; i < 400; i++)
	{
		for (j = 0; j < 5; j++)
		{
			MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
			Data_Filter();					// ��ԭʼ���ݻ����˲�
			mpu_dmp_get_data(&Attitude_Angle.Y, &Attitude_Angle.X, &Attitude_Angle.Z);	// ʹ��DMPֱ�Ӷ�ȡŷ����
		}
//		Get_Attitude();	// ��̬����
		Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), Zero_Angle*100);
	}
	
	Run_Flag = 1;

	
	/************************ �������� ***************************************/   
	ftm_quad_init(FTM1);	// ���������ʼ��
	ftm_quad_init(FTM2);	// ʹ�ö�ʱ��0������50msִ��һ��
 
	

	/************************ ����ͷ ��ʼ�� **********************************/
	camera_init(ImageBuff);                                   // ����ͷ��ʼ������ͼ��ɼ��� ImageBuff ��ַ
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);	// ���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      // ���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler
	
	
	
	
  	/************************ ����ģ�� NRF ��ʼ�� ****************************/
//	if (Image_Send_Flag)
//	{
//		while(!nrf_init());
//		set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);	//���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
//		enable_irq(PORTE_IRQn);
//		nrf_msg_init();                                        	//����ģ����Ϣ��ʼ��
//		i = 20;	// ����ͷģ�飬��Ҫ ���� ������� ���԰�ģ�飬����������� ����ͼ��ʧ�ܶ�������������
//		while(i--)
//		{
//			nrf_msg_tx(COM_RETRAN,nrf_tx_buff);               	//���Ͷ�� �� �����ȥ��������� ���ն˵Ļ�������
//		}
//	}


	/************************ ��ʱ�� ��ʼ��  *********************************/ 
	pit_init_ms(PIT0, 2);                              	// ��ʼ��PIT0����ʱʱ��Ϊ�� 2ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);	// ����PIT0���жϷ�����Ϊ PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 							// ʹ���ж�
	pit_init_ms(PIT1, 1000);                           	// ��ʼ��PIT1����ʱʱ��Ϊ�� 1s
	set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// ����PIT1���жϷ�����Ϊ PIT1_IRQHandler
//	enable_irq (PIT1_IRQn); 							// ʹ���ж�
	
	
	/************************ LED ��ʼ��  ************************************/
  	led_init(LED_MAX);
	led(LED_MAX,LED_ON);// ��LEDָʾ��		ȫ����ʼ���ɹ�
	
	while (!System_OK)
	{
		if (Attitude_Angle.Y < 10 && Attitude_Angle.Y > -20)	// ��̬����������Χ
		{
			System_OK = 1;
		}
	}
	Speed_Control();	// �ٶȿ���
	led(LED_MAX,LED_OFF);// �ر�LEDָʾ��
	
	/************************ ͼ��ɼ� ***************************************/
	SCCB_WriteByte (OV7725_CNST, 65);	//�ı�ͼ����ֵ
	camera_get_img();	// �Ȳɼ�һ��ͼ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ͷͼ��������λ���鿴ͼ��
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(void *imgaddr, uint32_t imgsize)
{
	uart_putchar(UART4,0x00);uart_putchar(UART4,0xff);uart_putchar(UART4,0x01);uart_putchar(UART4,0x01);//��������
    uart_putbuff(UART4, (uint8_t *)imgaddr, imgsize);  //����ͼ��
}


uchar UART_Str[30];

void My_DEBUG(void)
{
	sprintf(UART_Str, "%d\n", Speed_Now);
	uart_putstr(UART4, UART_Str);
}


void data_send()
{
	Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
	Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
	Size_t size     = {80, 60}; //{LCD_W, LCD_H};       //��ʾ����ͼ���С
		  
	com_e     com;
	nrf_result_e nrf_result;
	uint8 imgtxflag = 0;    //0��ʾ����ͼ��
  
  	/************************ ���߷��ͺͽ�������  ***********************/
  	do
  	{
		nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
		if(nrf_result == NRF_RESULT_RX_VALID)
		{
		  	switch(com)
		  	{	
		  		case COM_IMG:
				//��ʾ �� ��Ļ��
					#if     ( CAMERA_COLOR == 1 )                                       //�Ҷ�����ͷ
						LCD_Img_gray_Z(site,size, imgbuff,imgsize);
					#elif   ( CAMERA_COLOR == 0 )                                       //�ڰ�����ͷ
						LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
					#endif
					break;
			
		  		case COM_VAR:
					if(last_tab == CAR_CTRL)                    //����� ���� С�� ����
					{
			  			switch(car_ctrl_get())                  //��ȡС��������������д���
			  			{
			  			case CAR_START:                         //����
							//�˴����� ���� ����
							if (Run_Flag)
							{
								Run_Flag = 0;
								led(LED0,LED_OFF);
							}
							else
							{
								Run_Flag = 1;
								led(LED0,LED_ON);
							}														
							
							break;
				
						case CAR_STOP:                          //ͣ��
							//�˴����� ͣ�� ���� �����磺�رյ���������ر�ʹ�ܣ�
							
							break;
								
						case CAR_LEFT:					
							
							break;
								
						case CAR_RIGHT:
							
							break;
				
			  			case CAR_IMG_OPEN:
							imgtxflag = 0;
							break;
				
			  			case CAR_IMG_CLOSE:
							imgtxflag = 1;
							break;
								
			  			default:
							//��Ч���������κδ���
							break;
			  			}
					}			
					break;
					
		  		default:
					break;
		  	}
		}
	}while(nrf_result != NRF_RESULT_RX_NO);         //���ղ������� ���˳�
  
	if(!imgtxflag)
	{
		//��ͼ�� ���ͳ�ȥ
		nrf_msg_tx(COM_IMG,nrf_tx_buff);
	 }
  
	#if     ( CAMERA_COLOR == 1 )                                       //�Ҷ�����ͷ
  		LCD_Img_gray_Z       (site,size, imgbuff,imgsize);
	#elif   ( CAMERA_COLOR == 0 )                                       //�ڰ�����ͷ
  		LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
	#endif
  
  	if(!imgtxflag)
  	{
    	while(nrf_tx_state() == NRF_TXING);             //�ȴ��������
 	 }
  
	 //  var_syn(VAR_MAX);
	 //   var_syn(VAR1);
	 var_syn(VAR2);
	 var_syn(VAR3);
	 var_syn(VAR4);
	 var_syn(VAR5);
	 var_syn(VAR6);
}


void HMI_Data_Handle(void)
{
	char  UART0_Buff[3];
	
	if (uart_query (UART0) == 1)
	{
		uart_getchar(UART0, UART0_Buff);
		 //�û���Ҫ�����������
        if (UART0_Buff[0] == 0x66)
		{			
			uart_putstr(UART0, "page Run");	//HMI��ת���˶�����
			HMI_Send_End();
			uart_putstr(UART0, "dim=10");	//������ʾ�����ȣ���Լ�õ�
			HMI_Send_End();
			HMI_OK = 1;					//���ν�������ʼ��ʱ����
		}
		else if (UART0_Buff[0] == 0x0f)
		{			
			uart_putstr(UART0, "t0.txt=\"�ر�ͣ��\"");
			HMI_Send_End();
			Run_Stop = 0;
		}
		else if (UART0_Buff[0] == 0x01)
		{		
			Mode_Set = 0;	//����120
			uart_putstr(UART0, "t2.txt=\"����120\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x02)
		{
			Mode_Set = 1;	//����130
			uart_putstr(UART0, "t2.txt=\"����130\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x03)
		{
			Mode_Set = 2;	//����140
			uart_putstr(UART0, "t2.txt=\"����140\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x04)
		{
			Mode_Set = 3;	//����150
			uart_putstr(UART0, "t2.txt=\"����150\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x05)
		{
			Mode_Set = 4;	//����166
			uart_putstr(UART0, "t2.txt=\"����155\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x06)
		{
			Mode_Set = 5;	//����160
			uart_putstr(UART0, "t2.txt=\"����160\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x07)
		{
			Mode_Set = 6;	//����165
			uart_putstr(UART0, "t2.txt=\"����165\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x11)	// ��·����
		{
			if (Annulus_Count[0] == 0)
			{
				Annulus_Count[0] = 1;
				uart_putstr(UART0, "b0.txt=\"1.��\"");
				HMI_Send_End();
			}
			else
			{
				Annulus_Count[0] = 0;
				uart_putstr(UART0, "b0.txt=\"1.��\"");
				HMI_Send_End();
			}
		}
		else if (UART0_Buff[0] == 0x12)	// ��·����
		{
			if (Annulus_Count[1] == 0)
			{
				Annulus_Count[1] = 1;
				uart_putstr(UART0, "b1.txt=\"2.��\"");
				HMI_Send_End();
			}
			else
			{
				Annulus_Count[1] = 0;
				uart_putstr(UART0, "b1.txt=\"2.��\"");
				HMI_Send_End();
			}
		}
		else if (UART0_Buff[0] == 0x13)	// ��·����
		{
			if (Annulus_Count[2] == 0)
			{
				Annulus_Count[2] = 1;
				uart_putstr(UART0, "b2.txt=\"3.��\"");
			}
			else
			{
				Annulus_Count[2] = 0;
				uart_putstr(UART0, "b2.txt=\"3.��\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x14)	// ��·����
		{
			if (Annulus_Count[3] == 0)
			{
				Annulus_Count[3] = 1;
				uart_putstr(UART0, "b3.txt=\"4.��\"");
			}
			else
			{
				Annulus_Count[3] = 0;
				uart_putstr(UART0, "b3.txt=\"4.��\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x15)	// ��·����
		{
			if (Annulus_Count[4] == 0)
			{
				Annulus_Count[4] = 1;
				uart_putstr(UART0, "b4.txt=\"5.��\"");
			}
			else
			{
				Annulus_Count[4] = 0;
				uart_putstr(UART0, "b4.txt=\"5.��\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x16)	// ��·����
		{
			if (Annulus_Count[5] == 0)
			{
				Annulus_Count[5] = 1;
				uart_putstr(UART0, "b5.txt=\"6.��\"");
			}
			else
			{
				Annulus_Count[5] = 0;
				uart_putstr(UART0, "b5.txt=\"6.��\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x17)	// ��·����
		{
			if (Annulus_Count[6] == 0)
			{
				Annulus_Count[6] = 1;
				uart_putstr(UART0, "b6.txt=\"7.��\"");
			}
			else
			{
				Annulus_Count[6] = 0;
				uart_putstr(UART0, "b6.txt=\"7.��\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x18)	// ��·����
		{
			if (Annulus_Count[7] == 0)
			{
				Annulus_Count[7] = 1;
				uart_putstr(UART0, "b7.txt=\"8.��\"");
			}
			else
			{
				Annulus_Count[7] = 0;
				uart_putstr(UART0, "b7.txt=\"8.��\"");
			}
			HMI_Send_End();
		}

		UART0_Buff[0] = 0;
	}
}

void HMI_Send_End(void)		//����HMIָ�������
{
	uart_putchar(UART0, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
	uart_putchar(UART0, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
	uart_putchar(UART0, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
}
