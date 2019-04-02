#ifndef __USART2_H
#define __USART2_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//串口2初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	
 
 #include "headfile.h"
 
 
void usart2_init(u32 pclk1,u32 bound); 
void put_char(USART_TypeDef* USARTx, u8 Data);

void uart_putbuff(USART_TypeDef* USARTx, u8 *p, u16 len);
void USART2_IRQHandler(void);                	//串口1中断服务程序

#endif	   
















