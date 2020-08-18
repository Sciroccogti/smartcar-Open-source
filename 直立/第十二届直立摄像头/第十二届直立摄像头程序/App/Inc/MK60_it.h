/*!
 *		��������ѧԺֱ������ͷ
 *
 * @file       MK60_it.h
 * @brief      �жϷ�����ͷ�ļ�
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */
#ifndef __MK60_IT_H__
#define __MK60_IT_H__

#include  "common.h"

//�ⲿʹ�ñ��ļ��ĺ����ڴ�����
/******************��������*********************/
extern void Infrared_Handler(void);  //�����ж�
extern void PORTA_IRQHandler(void); //����ͷ�ж�
extern void DMA0_IRQHandler(void);  //����ͷ�ж�
extern void PIT0_IRQHandler(void);

//�ⲿʹ�ñ��ļ���ȫ�ֱ����ڴ�����
/**************ȫ�ֱ����ⲿ����*****************/
extern uint8 Flag_SpeedControl;
extern uint8 Flag_Stop;			//=1ͣ��
extern uint8 Flag_Speed;			//=0���ٶȿ���
extern uint8 Flag_Direction;		//=0���ٶȿ���
extern uint8 Flag_Out;				//����ʾ������ʾ��־
extern uint8 Flag_Buzz;			//=1����������


#endif  //__MK60_IT_H__
