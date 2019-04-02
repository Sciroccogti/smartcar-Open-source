#include "Init.h"
#include "ftm.h"
#include "gpio.h"

extern int WheelSpeed;

void Init()
{
FTM_InitTypeDef FTM_InitStruct1;
GPIO_InitTypeDef GPIO_InitStruct1;
//������ת
FTM_InitStruct1.Frequency = 10000;
FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned;
FTM_InitStruct1.InitalDuty = 0000;
FTM_InitStruct1.FTMxMAP = FTM0_CH1_PC2; 
FTM_Init(&FTM_InitStruct1);
//���ַ�ת
FTM_InitStruct1.Frequency = 10000;
FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned;
FTM_InitStruct1.InitalDuty = 1500;
FTM_InitStruct1.FTMxMAP = FTM0_CH2_PC3; 
FTM_Init(&FTM_InitStruct1);	
//ǰ�ֶ��
FTM_InitStruct1.Frequency = 300;
FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned;
FTM_InitStruct1.InitalDuty = 4900;
FTM_InitStruct1.FTMxMAP = FTM2_CH1_PB19; 
FTM_Init(&FTM_InitStruct1);	
//�Ұ���
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_14;  //PD14��������
GPIO_Init(&GPIO_InitStruct1);
//�󰴼�
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10;  //PD10��������
GPIO_Init(&GPIO_InitStruct1);
//�ϰ���
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_13;  //PD13��������
GPIO_Init(&GPIO_InitStruct1);
//�°���
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_15;  //PD15��������
GPIO_Init(&GPIO_InitStruct1);
//��������1
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_11;  //PD11��������
GPIO_Init(&GPIO_InitStruct1);
//��������2
GPIO_InitStruct1.GPIOx=PTD;
GPIO_InitStruct1.GPIO_InitState=Bit_SET;
GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_12;  //PD12��������
GPIO_Init(&GPIO_InitStruct1);
}