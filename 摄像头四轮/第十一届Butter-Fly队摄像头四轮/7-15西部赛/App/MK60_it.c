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
 *  @brief      UART4中断服务函数
 *  @since      v5.0
 */
void uart4_handler(void)
{
    char str[2];

    if(uart_querychar (UART4, str) != 0)   //接收数据寄存器满
    {
        //用户需要处理接收数据
        if (str[0] == 1)
		{
			Run_Flag = 1;
		}
		else if (str[0] == 2)
		{
			Run_Flag = 0;
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
*  @brief      PIT0中断服务函数
*  @since      v5.0
*/
void PIT0_IRQHandler()
{	
	static uchar Time_100ms = 0;
	
	Time_100ms++;
	if (Time_100ms == 10)
	{
		Time_100ms = 0;
		Send_OK = 1;
	}
	
	speed_measure();	//测速及停车	测试期间暂时不用
   
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
	if (Time == 3)			//延时5s
	{
		Time = 0;
		
		Run_Flag = 1;		//出发
		led(LED_MAX,LED_ON);//打开LED指示灯
		
		pit_close(PIT1);	//关闭定时器1
	}
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
/*			if (RC_Get > 1300)
			{
				Run_Flag = 1;
			}
			else
			{
				Run_Flag = 0;
			}*/
		}       
    }
}

/******************************中断服务函数结束***********8*******************/



