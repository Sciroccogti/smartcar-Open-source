#include "System.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多

uint8 ImageBuff[CAMERA_W*CAMERA_H/8];							// 图像采集缓存地址
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);	// 图像地址，用于上位机观察
uint8 Img[CAMERA_W*CAMERA_H]; //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

uchar Image_Send_Flag = 0;	// 图像发送控制，1为发送，0为不发送
uchar Send_OK = 0;
uchar System_OK = 0;
uchar HMI_OK = 0;

void System_Init()
{
	uint32 i, j;

	/************************ 配置 K60 的优先级  *****************************/

	NVIC_SetPriorityGrouping(4);            // 设置优先级分组,4bit 抢占优先级,没有亚优先级

	NVIC_SetPriority(PORTA_IRQn,0);         // 配置优先级
	NVIC_SetPriority(PORTE_IRQn,1);         // 配置优先级
	NVIC_SetPriority(DMA0_IRQn,3);          // 配置优先级
	NVIC_SetPriority(PIT0_IRQn,4);          // 配置优先级
	NVIC_SetPriority(PIT1_IRQn,5);          // 配置优先级
	NVIC_SetPriority(UART4_RX_TX_IRQn,6);   // 配置优先级

	
	/************************ 参数初始化 **********************************/
	Image_Para_Init();
	PID_Parameter_Init(&MOTOR_PID);		// 速度环PID参数初始化
	PID_Parameter_Init(&Angle_PID);		// 角度环PID参数初始化
	PID_Parameter_Init(&Ang_Vel_PID);	// 角速度环PID参数初始化
	PID_Parameter_Init(&Direct_PID);	// 转向环PID参数初始化
	PID_Parameter_Init(&Distance_PID);	// 位置环PID参数初始化
	Radius = 0;				// 初始化目标转向半径倒数为0
	Speed_Set = 0;			// 初始化目标速度为0
	Target_Angle.Y = Zero_Angle*100;
	
	
	/************************ 电机 初始化 ************************************/
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);	// 初始化 电机 PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);	// 初始化 电机 PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);	// 初始化 电机 PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);	// 初始化 电机 PWM
	
	
	/************************ 串口 初始化 ************************************/
	uart_init(UART4, 460800);	// 蓝牙
	uart_init(UART0, 115200);	// HMI液晶屏
	set_vector_handler(UART4_RX_TX_VECTORn ,UART4_IRQHandler);	// 设置 UART4 的中断服务函数为 UART4_IRQHandler
	uart_rx_irq_en(UART4);		// 开串口4接收中断
	HMI_Send_End();				// 发送一次结束符，清除上电产生的串口杂波数据
	
	
	/********************** HMI串口数据收发   **********************/ 
	while (!HMI_OK)
	{
		HMI_Data_Handle();
	}
	
	/************************ 陀螺仪 初始化 **********************************/
	MPU6050_OPEN();
	while (mpu_dmp_init());		// 初始化DMP
//	while (MPU6050_Init());		// 陀螺仪初始化成功返回0
	MPU6050_Offset();
//	LCD_init();	// 不晓得为什么，要用摄像头看图像就必须调用这个函数，注意管脚与电机冲突，只能屏蔽解决
	
	for (i = 0; i < 400; i++)
	{
		for (j = 0; j < 5; j++)
		{
			MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
			Data_Filter();					// 对原始数据滑动滤波
			mpu_dmp_get_data(&Attitude_Angle.Y, &Attitude_Angle.X, &Attitude_Angle.Z);	// 使用DMP直接读取欧拉角
		}
//		Get_Attitude();	// 姿态解算
		Tar_Ang_Vel.Y = PID_Realize(&Angle_PID, Angle, (int32)(Attitude_Angle.Y*100), Zero_Angle*100);
	}
	
	Run_Flag = 1;

	
	/************************ 正交解码 ***************************************/   
	ftm_quad_init(FTM1);	// 正交解码初始化
	ftm_quad_init(FTM2);	// 使用定时器0触发，50ms执行一次
 
	

	/************************ 摄像头 初始化 **********************************/
	camera_init(ImageBuff);                                   // 摄像头初始化，把图像采集到 ImageBuff 地址
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);	// 设置 PORTA 的中断服务函数为 PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      // 设置 DMA0 的中断服务函数为 DMA0_IRQHandler
	
	
	
	
  	/************************ 无线模块 NRF 初始化 ****************************/
//	if (Image_Send_Flag)
//	{
//		while(!nrf_init());
//		set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);	//设置 PORTE 的中断服务函数为 PORTE_VECTORn
//		enable_irq(PORTE_IRQn);
//		nrf_msg_init();                                        	//无线模块消息初始化
//		i = 20;	// 摄像头模块，需要 发送 空命令给 调试板模块，这样可以清掉 接收图像失败而产生多余数据
//		while(i--)
//		{
//			nrf_msg_tx(COM_RETRAN,nrf_tx_buff);               	//发送多个 空 命令过去，用于清空 接收端的缓存数据
//		}
//	}


	/************************ 定时器 初始化  *********************************/ 
	pit_init_ms(PIT0, 2);                              	// 初始化PIT0，定时时间为： 2ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);	// 设置PIT0的中断服务函数为 PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 							// 使能中断
	pit_init_ms(PIT1, 1000);                           	// 初始化PIT1，定时时间为： 1s
	set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// 设置PIT1的中断服务函数为 PIT1_IRQHandler
//	enable_irq (PIT1_IRQn); 							// 使能中断
	
	
	/************************ LED 初始化  ************************************/
  	led_init(LED_MAX);
	led(LED_MAX,LED_ON);// 打开LED指示灯		全部初始化成功
	
	while (!System_OK)
	{
		if (Attitude_Angle.Y < 10 && Attitude_Angle.Y > -20)	// 姿态进入正常范围
		{
			System_OK = 1;
		}
	}
	Speed_Control();	// 速度控制
	led(LED_MAX,LED_OFF);// 关闭LED指示灯
	
	/************************ 图像采集 ***************************************/
	SCCB_WriteByte (OV7725_CNST, 65);	//改变图像阈值
	camera_get_img();	// 先采集一次图像
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(void *imgaddr, uint32_t imgsize)
{
	uart_putchar(UART4,0x00);uart_putchar(UART4,0xff);uart_putchar(UART4,0x01);uart_putchar(UART4,0x01);//发送命令
    uart_putbuff(UART4, (uint8_t *)imgaddr, imgsize);  //发送图像
}


uchar UART_Str[30];

void My_DEBUG(void)
{
	sprintf(UART_Str, "%d\n", Speed_Now);
	uart_putstr(UART4, UART_Str);
}


void data_send()
{
	Site_t site     = {0, 0};                           //显示图像左上角位置
	Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小
	Size_t size     = {80, 60}; //{LCD_W, LCD_H};       //显示区域图像大小
		  
	com_e     com;
	nrf_result_e nrf_result;
	uint8 imgtxflag = 0;    //0表示发送图像
  
  	/************************ 无线发送和接收数据  ***********************/
  	do
  	{
		nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
		if(nrf_result == NRF_RESULT_RX_VALID)
		{
		  	switch(com)
		  	{	
		  		case COM_IMG:
				//显示 到 屏幕上
					#if     ( CAMERA_COLOR == 1 )                                       //灰度摄像头
						LCD_Img_gray_Z(site,size, imgbuff,imgsize);
					#elif   ( CAMERA_COLOR == 0 )                                       //黑白摄像头
						LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
					#endif
					break;
			
		  		case COM_VAR:
					if(last_tab == CAR_CTRL)                    //如果是 控制 小车 命令
					{
			  			switch(car_ctrl_get())                  //获取小车控制命令，并进行处理
			  			{
			  			case CAR_START:                         //开车
							//此处加入 开车 代码
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
				
						case CAR_STOP:                          //停车
							//此处加入 停车 代码 ，例如：关闭电机驱动（关闭使能）
							
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
							//无效，不进行任何处理
							break;
			  			}
					}			
					break;
					
		  		default:
					break;
		  	}
		}
	}while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出
  
	if(!imgtxflag)
	{
		//把图像 发送出去
		nrf_msg_tx(COM_IMG,nrf_tx_buff);
	 }
  
	#if     ( CAMERA_COLOR == 1 )                                       //灰度摄像头
  		LCD_Img_gray_Z       (site,size, imgbuff,imgsize);
	#elif   ( CAMERA_COLOR == 0 )                                       //黑白摄像头
  		LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
	#endif
  
  	if(!imgtxflag)
  	{
    	while(nrf_tx_state() == NRF_TXING);             //等待发送完成
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
		 //用户需要处理接收数据
        if (UART0_Buff[0] == 0x66)
		{			
			uart_putstr(UART0, "page Run");	//HMI屏转到运动界面
			HMI_Send_End();
			uart_putstr(UART0, "dim=10");	//降低显示屏亮度，节约用电
			HMI_Send_End();
			HMI_OK = 1;					//调参结束，开始定时起跑
		}
		else if (UART0_Buff[0] == 0x0f)
		{			
			uart_putstr(UART0, "t0.txt=\"关闭停车\"");
			HMI_Send_End();
			Run_Stop = 0;
		}
		else if (UART0_Buff[0] == 0x01)
		{		
			Mode_Set = 0;	//匀速120
			uart_putstr(UART0, "t2.txt=\"匀速120\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x02)
		{
			Mode_Set = 1;	//匀速130
			uart_putstr(UART0, "t2.txt=\"匀速130\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x03)
		{
			Mode_Set = 2;	//匀速140
			uart_putstr(UART0, "t2.txt=\"匀速140\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x04)
		{
			Mode_Set = 3;	//匀速150
			uart_putstr(UART0, "t2.txt=\"匀速150\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x05)
		{
			Mode_Set = 4;	//匀速166
			uart_putstr(UART0, "t2.txt=\"匀速155\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x06)
		{
			Mode_Set = 5;	//匀速160
			uart_putstr(UART0, "t2.txt=\"匀速160\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x07)
		{
			Mode_Set = 6;	//匀速165
			uart_putstr(UART0, "t2.txt=\"匀速165\"");
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x11)	// 环路控制
		{
			if (Annulus_Count[0] == 0)
			{
				Annulus_Count[0] = 1;
				uart_putstr(UART0, "b0.txt=\"1.左\"");
				HMI_Send_End();
			}
			else
			{
				Annulus_Count[0] = 0;
				uart_putstr(UART0, "b0.txt=\"1.右\"");
				HMI_Send_End();
			}
		}
		else if (UART0_Buff[0] == 0x12)	// 环路控制
		{
			if (Annulus_Count[1] == 0)
			{
				Annulus_Count[1] = 1;
				uart_putstr(UART0, "b1.txt=\"2.左\"");
				HMI_Send_End();
			}
			else
			{
				Annulus_Count[1] = 0;
				uart_putstr(UART0, "b1.txt=\"2.右\"");
				HMI_Send_End();
			}
		}
		else if (UART0_Buff[0] == 0x13)	// 环路控制
		{
			if (Annulus_Count[2] == 0)
			{
				Annulus_Count[2] = 1;
				uart_putstr(UART0, "b2.txt=\"3.左\"");
			}
			else
			{
				Annulus_Count[2] = 0;
				uart_putstr(UART0, "b2.txt=\"3.右\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x14)	// 环路控制
		{
			if (Annulus_Count[3] == 0)
			{
				Annulus_Count[3] = 1;
				uart_putstr(UART0, "b3.txt=\"4.左\"");
			}
			else
			{
				Annulus_Count[3] = 0;
				uart_putstr(UART0, "b3.txt=\"4.右\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x15)	// 环路控制
		{
			if (Annulus_Count[4] == 0)
			{
				Annulus_Count[4] = 1;
				uart_putstr(UART0, "b4.txt=\"5.左\"");
			}
			else
			{
				Annulus_Count[4] = 0;
				uart_putstr(UART0, "b4.txt=\"5.右\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x16)	// 环路控制
		{
			if (Annulus_Count[5] == 0)
			{
				Annulus_Count[5] = 1;
				uart_putstr(UART0, "b5.txt=\"6.左\"");
			}
			else
			{
				Annulus_Count[5] = 0;
				uart_putstr(UART0, "b5.txt=\"6.右\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x17)	// 环路控制
		{
			if (Annulus_Count[6] == 0)
			{
				Annulus_Count[6] = 1;
				uart_putstr(UART0, "b6.txt=\"7.左\"");
			}
			else
			{
				Annulus_Count[6] = 0;
				uart_putstr(UART0, "b6.txt=\"7.右\"");
			}
			HMI_Send_End();
		}
		else if (UART0_Buff[0] == 0x18)	// 环路控制
		{
			if (Annulus_Count[7] == 0)
			{
				Annulus_Count[7] = 1;
				uart_putstr(UART0, "b7.txt=\"8.左\"");
			}
			else
			{
				Annulus_Count[7] = 0;
				uart_putstr(UART0, "b7.txt=\"8.右\"");
			}
			HMI_Send_End();
		}

		UART0_Buff[0] = 0;
	}
}

void HMI_Send_End(void)		//发送HMI指令结束符
{
	uart_putchar(UART0, 0xff); //从串口丢一个字节数据出去
	uart_putchar(UART0, 0xff); //从串口丢一个字节数据出去
	uart_putchar(UART0, 0xff); //从串口丢一个字节数据出去
}
