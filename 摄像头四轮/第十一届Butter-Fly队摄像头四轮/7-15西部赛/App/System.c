#include "System.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��

uchar Image_Send_Flag = 0;
uchar Send_OK = 0;
uchar System_OK = 0;

void System_Init()
{
	uint32 i;

	/************************ ���� K60 �����ȼ�  ***********************/

	NVIC_SetPriorityGrouping(5);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�

	NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�
	NVIC_SetPriority(PORTE_IRQn,1);         //�������ȼ�
	NVIC_SetPriority(DMA0_IRQn,2);          //�������ȼ�
	NVIC_SetPriority(PIT0_IRQn,3);          //�������ȼ�
	NVIC_SetPriority(PIT1_IRQn,4);          //�������ȼ�
  
  	/*********************** ��� ��ʼ�� ***********************/  
	ftm_pwm_init(S_D5_FTM, S_D5_CH,S_D5_HZ,S_D5_MID);   //��ʼ�� ��� PWM
	
	
	/************************ ��� ��ʼ��  ***********************/
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM 
	
	
  	/************************** LED ��ʼ��  ***************************/
  	led_init(LED_MAX);
  
  
  	/************************ LCD Һ���� ��ʼ��  ***********************/
	LCD_init();
 
  
  	/************************ ����ģ�� NRF ��ʼ��  ***********************/
	if (Image_Send_Flag)
	{
		while(!nrf_init());
		//�����жϷ�����
		set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
		enable_irq(PORTE_IRQn);
		nrf_msg_init();                                                     //����ģ����Ϣ��ʼ��

		//����ͷģ�飬��Ҫ ���� ������� ���԰�ģ�飬����������� ����ͼ��ʧ�ܶ�������������
		i = 20;
		while(i--)
		{
			nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //���Ͷ�� �� �����ȥ��������� ���ն˵Ļ�������
		}
	}
  
	
	/*********************** ���벶�� ��ʼ��  **********************/
//	ftm_input_init(FTM2, FTM_CH0, FTM_Rising_or_Falling,FTM_PS_64);     //��ʼ��FTM���벶׽ģʽ�������ز�׽(FTM2_CH0 Ϊ PTA10)
//	ftm_input_init(FTM2, FTM_CH1, FTM_Rising_or_Falling,FTM_PS_2);     //��ʼ��FTM���벶׽ģʽ�������ز�׽(FTM2_CH1 Ϊ PTA11)
 // set_vector_handler(FTM2_VECTORn ,FTM2_INPUT_IRQHandler); //����FTM2���жϷ�����Ϊ FTM2_INPUT_IRQHandler
//	enable_irq (FTM2_IRQn);                                  //ʹ��FTM2�ж�	
  

	/************************ ����ͷ ��ʼ��  ***********************/
	camera_init(imgbuff);                                   //����ͷ��ʼ������ͼ��ɼ��� imgbuff ��ַ
	//�����жϷ�����
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler 

   
	/************************ ��������  ***********************/   
	ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
														  //ʹ�ö�ʱ��0������10msִ��һ��

	/******************** ��ʱ�� ��ʼ��  *********************/ 
	pit_init_ms(PIT0, 10);                                  //��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 								  //ʹ���ж�
	
	pit_init_ms(PIT1, 1000);                                //��ʼ��PIT0����ʱʱ��Ϊ�� 1s
	set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
//	enable_irq (PIT1_IRQn); 								//ʹ���ж�
	
	
	/************************** ���� ��ʼ��  ***********************/
	uart_init(UART5, 115200);								//HMIҺ����
	
 	HMI_Send_End();											//����һ�ν�����������ϵ�����Ĵ����Ӳ�����
  

	/********************** PID������ʼ�� ***********************/
	PlacePID_Init(&S_D5_PID);//���PID������ʼ��   ������δ����
	IncPID_Init(&MOTOR_PID);	//���PID������ʼ��


	/************************ ͼ��ɼ� **************************/
	camera_get_img();//�Ȳɼ�һ��ͼ��
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
								Stop_Flag = 1;
								led(LED0,LED_OFF);
							}
							else
							{
								Run_Flag = 1;
								Stop_Flag = 0;
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
	char  UART5_Buff[3];
	
	if (uart_query (UART5) == 1)
	{
		uart_getchar(UART5, UART5_Buff);
		 //�û���Ҫ�����������
        if (UART5_Buff[0] == 0x66)
		{			
			uart_putstr(UART5, "page Run");	//HMI��ת���˶�����
			HMI_Send_End();
			uart_putstr(UART5, "dim=10");	//������ʾ�����ȣ���Լ�õ�
			HMI_Send_End();
			System_OK = 1;					//���ν�������ʼ��ʱ����
			enable_irq (PIT1_IRQn);			//ʹ�ܶ�ʱ��1�����Զ�����׼��
		}
		else if (UART5_Buff[0] == 0x01)
		{			
			uart_putstr(UART5, "t2.txt=\"����140\"");
			HMI_Send_End();
			Set = 1;	//����140
		}
		else if (UART5_Buff[0] == 0x02)
		{
			Set = 2;	//����150
			uart_putstr(UART5, "t2.txt=\"����150\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x03)
		{
			Set = 3;	//����160
			uart_putstr(UART5, "t2.txt=\"����160\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x04)
		{
			Set = 4;	//����170
			uart_putstr(UART5, "t2.txt=\"����170\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x06)
		{
			Set = 5;	//�ȶ�ģʽ2��3
			uart_putstr(UART5, "t2.txt=\"�ȶ�ģʽ\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x07)
		{
			Set = 6;	//����ģʽ2��35
			uart_putstr(UART5, "t2.txt=\"����ģʽ\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x08)
		{
			Set = 7;	//�蹷ģʽ������
			uart_putstr(UART5, "t2.txt=\"�蹷ģʽ\"");
			HMI_Send_End();
		}
                else if (UART5_Buff[0] == 0x09)
                {
                        Set = 8;
                        uart_putstr(UART5, "t2.txt=\"����ģʽ\"");
			HMI_Send_End();
                }
		else if (UART5_Buff[0] == 0x11)
		{
			SCCB_WriteByte (OV7725_CNST, 50);	//��ֵ��Ϊ50������ʹ��
			uart_putstr(UART5, "t1.txt=\"50\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x12)
		{
			SCCB_WriteByte (OV7725_CNST, 55);	//��ֵ��Ϊ60
			uart_putstr(UART5, "t1.txt=\"60\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x13)
		{
			SCCB_WriteByte (OV7725_CNST, 40);	//��ֵ��Ϊ70���谵ʹ��
			uart_putstr(UART5, "t1.txt=\"40\"");
			HMI_Send_End();
		}
                else if (UART5_Buff[0] == 0x0f)
                {
                        if (Game_End)
                        {
                              Game_End = 0;
                              uart_putstr(UART5, "t0.txt=\"ͣ���ر�\"");
                              HMI_Send_End();
                        }
                        else
                        {
                              Game_End = 1;
                              uart_putstr(UART5, "t0.txt=\"�յ�ͣ��\"");
                              HMI_Send_End();
                        }
                }
		UART5_Buff[0] = 0;
	}
}

void HMI_Send_End(void)		//����HMIָ�������
{
	uart_putchar(UART5, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
	uart_putchar(UART5, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
	uart_putchar(UART5, 0xff); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
}
