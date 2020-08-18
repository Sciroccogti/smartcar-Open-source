/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */
#include    "MK60_it.h"
#include    "common.h"
#include    "include.h"

/*********************************中断服务函数********************************/
/*!
*  @brief      PIT0中断服务函数
*  @since      v5.0
*/
void PIT0_IRQHandler()	// 2ms进一次中断
{	
	static uchar Time_10ms = 0;
	static uchar Time_50ms = 0;
	static uchar Time_100ms = 0;
	static uchar Time_200ms = 0;
	
	Time_10ms++;
	Time_50ms++;
	Time_100ms++;
	Time_200ms++;
	
	if (Annulus_Delay)
	{
		Annulus_Delay--;
	}
	
	Ang_Velocity_Flag = 1;	// 角速度2ms获取一次
	
	if (Time_10ms == 5)
	{
		Time_10ms = 0;
		Angle_Flag = 1;		// 姿态角度10ms控制一次	
	}
	
	if (Time_50ms == 25)
	{
		Time_50ms = 0;
		
//		Data_Send_Scope();//发送至Data_Scope上位机看波形
//		Data_Send_Status();//发送至匿名上位机看波形
	}
	
	if (Time_100ms == 50)
	{
		Time_100ms = 0;
		Speed_Flag = 1;	// 速度100ms控制一次
	}
	
	if (Time_200ms == 100)
	{
		Time_200ms = 0;
		
		Send_OK = 1;	// 图像200ms发送一次
		My_DEBUG();
	}
	
	Balance_Control();	// 车身平衡控制
	PIT_Flag_Clear(PIT0);//清除定时器中断标志位
}


/*!
*  @brief      PIT1中断服务函数
*  @since      v5.0
*/
void PIT1_IRQHandler()
{
	static uchar Time = 0;
	
	PIT_Flag_Clear(PIT1);//清除定时器中断标志位
	
	Time++;
	if (Time == 5)			//延时5s
	{
		Time = 0;
		
		Run_Flag = 1;		//出发
		led(LED_MAX,LED_ON);//打开LED指示灯
		
		pit_close(PIT1);	//关闭定时器1
	}
}


/*!
*  @brief      UART4中断服务函数
*  @since      v5.0
*/
void UART4_IRQHandler(void)
{
	char Buff;
	UARTn_e uratn = UART4;
	
//	if (uart_querychar(UART4, &Buff))
	if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) //接收数据寄存器满
	{
		uart_getchar(uratn, &Buff);
		switch (Buff)
		{
			case 'q': 	Speed_Set = 30; break;			// 前进
			case 'w': 	Speed_Set = 0;	
						Tar_Ang_Vel.Z = 0;	break;		// 停车
			case 's':	Tar_Ang_Vel.Z = 0;	break;		// 停止转向
			case 'a':	Tar_Ang_Vel.Z = -7000;	break;	// 大左转
			case 'd':	Tar_Ang_Vel.Z = 7000;	break;	// 大右转
			case 'z':	Tar_Ang_Vel.Z = -3000;	break;	// 小左转
			case 'c':	Tar_Ang_Vel.Z = 3000;	break;	// 小右转
			case 'x':	Speed_Set = -20;	break;		// 后退
			default:	break;
		}
	}
}

/*!
*  @brief      PORTA中断服务函数
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8  n;    //引脚号
  uint32 flag;
  
  //while(!PORTA_ISFR);
  flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //清中断标志位
  
  n = 29;                                             //场中断
  if(flag & (1 << n))                                 //PTA29触发中断
  {
    camera_vsync();
  }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
  n = 28;
  if(flag & (1 << n))                                 //PTA28触发中断
  {
    camera_href();
  }
#endif
  
}


/*!
*  @brief      PORTE中断服务函数
*  @since      v5.0
*/
void PORTE_IRQHandler()
{
  uint8  n;    //引脚号
  uint32 flag;
  
  flag = PORTE_ISFR;
  PORTE_ISFR  = ~0;                                   //清中断标志位
  
  n = 27;
  if(flag & (1 << n))                                 //PTE27触发中断
  {
    nrf_handler();
  }
}


/*!
*  @brief      DMA0中断服务函数
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}


/*!
*  @brief      输入捕获中断服务函数
*  @since      v5.0
*/
void FTM2_INPUT_IRQHandler(void)
{
    uint8 s = FTM2_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;
	int32 temp;

    FTM2_STATUS = 0x00;				//清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
        temp = ftm_input_get(FTM2, FTM_CH0); //保存
		if (gpio_get(PTA10))				 //上升沿
		{
			ftm_input_clean(FTM2);            //清 计数器计数值
		}
		else	//下降沿
		{
			RC_Get= 1000000/(bus_clk_khz*1000/(1<<FTM_PS_64)/temp);	//保存高电平时间即通道0的值
		}       
    }
}

/******************************中断服务函数结束***********8*******************/



