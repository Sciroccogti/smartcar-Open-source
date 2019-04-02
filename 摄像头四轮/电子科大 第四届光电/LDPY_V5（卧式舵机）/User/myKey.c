
#include "myKey.h"

/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:myKey_Init(void)
*  ����˵��:������ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: 
*************************************************************************/
void myKey_Init(void)
{
	EXTI_Init(PORTE, GPIO_Pin_2,Trigger_Falling );//���� 1
	EXTI_Init(PORTE, GPIO_Pin_3,Trigger_Falling );//���� 2
	EXTI_Init(PORTE, GPIO_Pin_4,Trigger_Falling );//���� 3

	EXTI_Init(PORTE, GPIO_Pin_5,Trigger_Falling );//���� 4
	EXTI_Init(PORTE, GPIO_Pin_6,Trigger_Falling );//���� 5
	EXTI_Init(PORTE, GPIO_Pin_7,Trigger_Falling );//���� 6
	
	EXTI_Init(PORTD, GPIO_Pin_11,Trigger_Rising );//ң����
}

/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:myKey_SatrtEndLineInit(void)
*  ����˵��:�����߼�β�Ƴ�ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: 
*************************************************************************/
void myKey_StartEndLineInit(void)
{
	EXTI_Init(PORTD, GPIO_Pin_12,Trigger_Falling );//  ����  ǰ����
	EXTI_Init(PORTD, GPIO_Pin_13,Trigger_Falling );//  ����  ǰ��
	EXTI_Init(PORTD, GPIO_Pin_14,Trigger_Falling );//  ����  ǰ��
	EXTI_Init(PORTD, GPIO_Pin_15,Trigger_Falling );//  ����  ǰ����
	
	NVIC_SetPriorityGrouping(1);
	NVIC_SetPriority(PORTD_IRQn,1);
	
	NVIC_SetPriorityGrouping(1);
	NVIC_SetPriority(PIT0_IRQn,2);
}

/*-------------------------end of myKey.c-------------------------*/
