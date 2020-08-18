/*!
 *		湖北工程学院直立摄像头
 *		
 *		主要的整车控制程序在外部中断PORTA中
 *
 * @file       MK60_it.c
 * @brief      中断服务程序
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 */

#include "common.h"
#include "include.h"

/****************全局变量定义******************/
uint8 Flag_SpeedControl = 0;
uint8 Flag_Stop = OFF;			//=OFF停车
uint8 Flag_Speed = ON;			//=ON加速度控制
uint8 Flag_Direction = ON;		//=ON加速度控制
uint8 Flag_Out = 2;				//虚拟示波器显示标志
uint8 Flag_Buzz = ON;			//=OFF蜂鸣器静音

/**
 * @brief		PIT0中断服务函数
 * @author		湖北工程学院
 * @date		2017
 */
void PIT0_IRQHandler(void)
{

	PIT_Flag_Clear(PIT0);
//以下为PIT中断处理部分
	
	if(Flag_ir>0)Flag_ir--;	
	else enable_irq(PORTC_IRQn);	//红外接收一次数据之后延时一会再开启外部中断，防止按一次键多次反应
	
//以上为PIT中断处理部分
}


/**
 * @brief		红外接收外部中断服务函数
 * @author		湖北工程学院
 * @date		2017
 */
void Infrared_Handler(void)
{
    uint8  n = 0;   
       n = 4;		//引脚号
    if(PORTC_ISFR & (1 << n))       
    {
       
       
            PORTC_ISFR  = (1 << n);        //写1清中断标志位      
			
            IR_Decoding();                //红外解码 

#if 0         /*  以下为led显示4个红外码值 */   
            oled_fill(0);
            oled_print_16x8short(0,0,UserCode);//看用户吗
            oled_print_16x8short(0,2,UserData);//用户反码
            oled_print_16x8short(0,4,KeyCode);//看键码
            oled_print_16x8short(0,6,KeyData);//看键反吗
#endif            
    }   
}

/**
 * @brief		摄像头PORTA中断服务函数
 *	
 *	进入此中断标志图像的采集开始
 *	同时整车的控制开始，使用的上一次的图像完成转向
 *	这样一边图像采集，一边小车控制。
 *	小车控制的时间不得超过图像采集周期（1/150）s
 *
 * @author		湖北工程学院
 * @date		2017
 */
void PORTA_IRQHandler(void)
{
  	//标志变量定义
	static uint16 Flag_Led = 0;	
  
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                //清中断标志位
    n = 29;                          //场中断
    if(flag & (1 << n))              //PTA29触发中断
    {
		gpio_set (K1, 1);//用于逻辑分析
		
		//0.5sLED闪烁
		Flag_Led++;
		if(Flag_Led >= 40)  
        {    
            Flag_Led = 0;
            led_turn(LED0);         //LED指示 
			gpio_set (BUZZ, 0);		//不停地关闭蜂鸣器避免一直响
        }	
		
        camera_vsync(); 			//开始采集图像(设置图像地址)1.67us
		flag_imgbuff != 0?img_extract(img, p_imgbuff1,CAMERA_SIZE):img_extract(img, p_imgbuff2,CAMERA_SIZE);
									//解压上一次图像0.99ms
//以下为主要控制部分（不要超过5.5ms）		
		DirectionControl();		//方向控制输出
		GalAcceAndGyroY();		//采集计算加速度计和陀螺仪 （用时0.78ms）
		KalmanFilter();			//卡尔曼滤波，得出角度（用时0.03ms）
		BalanceControl();		//直立控制输出
		PWMOut();				//最终PWM输出给电机（本次的方向输出、直立输出和上次的速度输出叠加）
		Flag_SpeedControl++;	
		if(Flag_SpeedControl >= 20)
		{
			SpeedControl();		//速度控制周期20*(1/150)s
			Flag_SpeedControl = 0;
		}
		SpeedControlOut();		//速度输出周期（1/150）s，也就是每次的速度控制分20次输出
		
		//gpio_set (K3, 1);//用于逻辑分析
		//gpio_set (K3, 0);//用于逻辑分析	
		switch(Flag_Out)		//虚拟示波器
		{
		  case 0 :	OutData[0] = g_fAngleOfAcce;//g_nValueOfAcce;//加速度角度值
		  			OutData[1] = g_fAngleSpeed[GyroY];//g_nValueOfGyro[GyroY];
					OutData[2] = g_PWMOut;
					OutData[3] = g_AngleOfCar;		//卡尔曼融合角度
					break;
					
		  case 1 : 	OutData[0] = (int)g_fRealSpeed;//g_nLeftpulse;//
		  			OutData[1] = (int)g_fSpeedError;//真实滤波速度与期望速度的偏差//g_nRighpulse;
					OutData[2] = (int)g_fSpeedControlOut;//g_fSpeedFilter;		
					OutData[3] = (int)g_fExpectSpeed;
					break;
					
		  case 2 :	OutData[0] = g_nDirectionError;
		  			OutData[1] = g_fDirectionError_dot;//
					//OutData[2] = g_fDirectionControlOut;//g_fDirectionError_dot;
					//OutData[3] = 0;//fAngleTurn;
					break;
					
		  case 3 :	OutData[0] = (int)g_fSpeedControlOut;		//速度控制输出（红）
		  			OutData[1] = (int)g_fDirectionControlOut;	//方向输出（黄）
					OutData[2] = (int)g_PWMOut;					//总PWM（蓝）
					OutData[3] = (int)g_BlanceControlOut;		//平衡控制输出（紫）
					break;
		  default :break;
		}
		OutPut_Data();	//虚拟示波器输出
		gpio_set (K1, 0);//用于逻辑分析
//以上为主要控制部分
		
    }    
}

/**
 * @brief		摄像头DMA0中断服务函数
 *	
 *	进入此中断标志着一副图像采集完毕，
 *	初始化下一次采集图像的存放地址
 *	两个图像存储地址，交替使用，
 *	本次采集图像的时候，就可以使用上次采集的图像，
 *	这样让控制周期最短。
 *
 * @author		湖北工程学院
 * @date		2017
 */
void DMA0_IRQHandler(void)	//1.875us(dma结束与下一个场中断开始间隔0.22ms)
{
	//gpio_set (K2, 1);//用于逻辑分析
    camera_dma();		//dma中断标志图像传输完毕
	camera_get_img();	//等待图像传输完毕（完成标志变为开始标志）
	flag_imgbuff =! flag_imgbuff;
	ov7725_eagle_img_buff = (flag_imgbuff == 0?p_imgbuff1:p_imgbuff2);//交替初始化图像地址
	//gpio_set (K2, 0);//用于逻辑分析
}


