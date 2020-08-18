/*!
 *		湖北工程学院直立摄像头
 *
 * @file       MK60_it.h
 * @brief      中断服务函数头文件
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 */
#ifndef __MK60_IT_H__
#define __MK60_IT_H__

#include  "common.h"

//外部使用本文件的函数在此申明
/******************函数申明*********************/
extern void Infrared_Handler(void);  //红外中断
extern void PORTA_IRQHandler(void); //摄像头中断
extern void DMA0_IRQHandler(void);  //摄像头中断
extern void PIT0_IRQHandler(void);

//外部使用本文件的全局变量在此申明
/**************全局变量外部申明*****************/
extern uint8 Flag_SpeedControl;
extern uint8 Flag_Stop;			//=1停车
extern uint8 Flag_Speed;			//=0加速度控制
extern uint8 Flag_Direction;		//=0加速度控制
extern uint8 Flag_Out;				//虚拟示波器显示标志
extern uint8 Flag_Buzz;			//=1蜂鸣器静音


#endif  //__MK60_IT_H__
