#include "System.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多

uchar Image_Send_Flag = 0;
uchar Send_OK = 0;
uchar System_OK = 0;

void System_Init()
{
	uint32 i;

	/************************ 配置 K60 的优先级  ***********************/

	NVIC_SetPriorityGrouping(5);            //设置优先级分组,4bit 抢占优先级,没有亚优先级

	NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
	NVIC_SetPriority(PORTE_IRQn,1);         //配置优先级
	NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
	NVIC_SetPriority(PIT0_IRQn,3);          //配置优先级
	NVIC_SetPriority(PIT1_IRQn,4);          //配置优先级
  
  	/*********************** 舵机 初始化 ***********************/  
	ftm_pwm_init(S_D5_FTM, S_D5_CH,S_D5_HZ,S_D5_MID);   //初始化 舵机 PWM
	
	
	/************************ 电机 初始化  ***********************/
	ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
	ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机 PWM 
	
	
  	/************************** LED 初始化  ***************************/
  	led_init(LED_MAX);
  
  
  	/************************ LCD 液晶屏 初始化  ***********************/
	LCD_init();
 
  
  	/************************ 无线模块 NRF 初始化  ***********************/
	if (Image_Send_Flag)
	{
		while(!nrf_init());
		//配置中断服务函数
		set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn
		enable_irq(PORTE_IRQn);
		nrf_msg_init();                                                     //无线模块消息初始化

		//摄像头模块，需要 发送 空命令给 调试板模块，这样可以清掉 接收图像失败而产生多余数据
		i = 20;
		while(i--)
		{
			nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //发送多个 空 命令过去，用于清空 接收端的缓存数据
		}
	}
  
	
	/*********************** 输入捕获 初始化  **********************/
//	ftm_input_init(FTM2, FTM_CH0, FTM_Rising_or_Falling,FTM_PS_64);     //初始化FTM输入捕捉模式，跳变沿捕捉(FTM2_CH0 为 PTA10)
//	ftm_input_init(FTM2, FTM_CH1, FTM_Rising_or_Falling,FTM_PS_2);     //初始化FTM输入捕捉模式，跳变沿捕捉(FTM2_CH1 为 PTA11)
 // set_vector_handler(FTM2_VECTORn ,FTM2_INPUT_IRQHandler); //设置FTM2的中断服务函数为 FTM2_INPUT_IRQHandler
//	enable_irq (FTM2_IRQn);                                  //使能FTM2中断	
  

	/************************ 摄像头 初始化  ***********************/
	camera_init(imgbuff);                                   //摄像头初始化，把图像采集到 imgbuff 地址
	//配置中断服务函数
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler 

   
	/************************ 正交解码  ***********************/   
	ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
														  //使用定时器0触发，10ms执行一次

	/******************** 定时器 初始化  *********************/ 
	pit_init_ms(PIT0, 10);                                  //初始化PIT0，定时时间为： 10ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 								  //使能中断
	
	pit_init_ms(PIT1, 1000);                                //初始化PIT0，定时时间为： 1s
	set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
//	enable_irq (PIT1_IRQn); 								//使能中断
	
	
	/************************** 串口 初始化  ***********************/
	uart_init(UART5, 115200);								//HMI液晶屏
	
 	HMI_Send_End();											//发送一次结束符，清除上电产生的串口杂波数据
  

	/********************** PID参数初始化 ***********************/
	PlacePID_Init(&S_D5_PID);//舵机PID参数初始化   参数还未调节
	IncPID_Init(&MOTOR_PID);	//电机PID参数初始化


	/************************ 图像采集 **************************/
	camera_get_img();//先采集一次图像
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
	char  UART5_Buff[3];
	
	if (uart_query (UART5) == 1)
	{
		uart_getchar(UART5, UART5_Buff);
		 //用户需要处理接收数据
        if (UART5_Buff[0] == 0x66)
		{			
			uart_putstr(UART5, "page Run");	//HMI屏转到运动界面
			HMI_Send_End();
			uart_putstr(UART5, "dim=10");	//降低显示屏亮度，节约用电
			HMI_Send_End();
			System_OK = 1;					//调参结束，开始定时起跑
			enable_irq (PIT1_IRQn);			//使能定时器1，做自动起跑准备
		}
		else if (UART5_Buff[0] == 0x01)
		{			
			uart_putstr(UART5, "t2.txt=\"匀速140\"");
			HMI_Send_End();
			Set = 1;	//匀速140
		}
		else if (UART5_Buff[0] == 0x02)
		{
			Set = 2;	//匀速150
			uart_putstr(UART5, "t2.txt=\"匀速150\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x03)
		{
			Set = 3;	//匀速160
			uart_putstr(UART5, "t2.txt=\"匀速160\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x04)
		{
			Set = 4;	//匀速170
			uart_putstr(UART5, "t2.txt=\"匀速170\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x06)
		{
			Set = 5;	//稳定模式2米3
			uart_putstr(UART5, "t2.txt=\"稳定模式\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x07)
		{
			Set = 6;	//快速模式2米35
			uart_putstr(UART5, "t2.txt=\"快速模式\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x08)
		{
			Set = 7;	//疯狗模式，看脸
			uart_putstr(UART5, "t2.txt=\"疯狗模式\"");
			HMI_Send_End();
		}
                else if (UART5_Buff[0] == 0x09)
                {
                        Set = 8;
                        uart_putstr(UART5, "t2.txt=\"极限模式\"");
			HMI_Send_End();
                }
		else if (UART5_Buff[0] == 0x11)
		{
			SCCB_WriteByte (OV7725_CNST, 50);	//阈值改为50，明亮使用
			uart_putstr(UART5, "t1.txt=\"50\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x12)
		{
			SCCB_WriteByte (OV7725_CNST, 55);	//阈值改为60
			uart_putstr(UART5, "t1.txt=\"60\"");
			HMI_Send_End();
		}
		else if (UART5_Buff[0] == 0x13)
		{
			SCCB_WriteByte (OV7725_CNST, 40);	//阈值改为70，昏暗使用
			uart_putstr(UART5, "t1.txt=\"40\"");
			HMI_Send_End();
		}
                else if (UART5_Buff[0] == 0x0f)
                {
                        if (Game_End)
                        {
                              Game_End = 0;
                              uart_putstr(UART5, "t0.txt=\"停车关闭\"");
                              HMI_Send_End();
                        }
                        else
                        {
                              Game_End = 1;
                              uart_putstr(UART5, "t0.txt=\"终点停车\"");
                              HMI_Send_End();
                        }
                }
		UART5_Buff[0] = 0;
	}
}

void HMI_Send_End(void)		//发送HMI指令结束符
{
	uart_putchar(UART5, 0xff); //从串口丢一个字节数据出去
	uart_putchar(UART5, 0xff); //从串口丢一个字节数据出去
	uart_putchar(UART5, 0xff); //从串口丢一个字节数据出去
}
