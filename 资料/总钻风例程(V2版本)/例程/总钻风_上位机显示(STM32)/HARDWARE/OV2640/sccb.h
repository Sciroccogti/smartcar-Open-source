#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//OVϵ������ͷ SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//IO��������
#define SCCB_SDA_IN()  {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=0<<1*2;}	//PD7 ����
#define SCCB_SDA_OUT() {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=1<<1*2;} 	//PD7 ���


//IO��������	 
#define SCCB_SCL    		PCout(2)	 	//SCL
#define SCCB_SDA    		PCout(1) 		//SDA	 

#define SCCB_READ_SDA    	PCin(1)  		//����SDA    
#define SCCB_ID   			0x42  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













