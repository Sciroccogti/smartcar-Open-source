/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @brief      		总钻风(灰度摄像头)历程
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		MDK 5.17
 * @Target core		STM32F407ZGT6
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-10-11
 * @note
					OV7725接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA                A2
						SCL                A3
						场中断              B7
                        行中断              A4   
						像素中断            A6       
                        
						D0                 C6
                        D1                 C7
                        D2                 C8
                        D3                 C9
                        D4                 C11
                        D5                 B6
                        D6                 E5
                        D7                 E6
                        
					------------------------------------ 
                    USB转TTL接线定义         
                        USB转TTL引脚        单片机引脚
                        TX                  A10
                        RX                  A9
                        
					默认分辨率              查看SEEKFREE_MT9V032.h 文件内的宏定义 COL ROW
					默认FPS                 50帧
 ********************************************************************************************************************/
#include "headfile.h"



int main(void)
{ 


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);                                //初始化延时函数
	uart_init(115200);		                        //初始化串口波特率为115200,用于发送图像给上位机
    mt9v032_init();

 	while(1)
	{	
        if(finish_flag_032) 
        {
            finish_flag_032 = 0;
            seekfree_sendimg_032();
        }
    }
}
