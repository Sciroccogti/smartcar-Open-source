#include "gpio.h"
#include "xjb.h"
#include "ftm.h"
#include "dmacnt.h"
#include "extern.h"

void XJB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct1;
  FTM_InitTypeDef FTM_InitStruct1;
  DMACNT_InitTypeDef DMACNT_InitStruct1;
  GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = PTA;
  GPIO_Init(&GPIO_InitStruct1);
//**********************
  GPIO_InitStruct1.GPIO_Pin = Pin_Minus; // KEY SUB
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_Minus;
  GPIO_Init(&GPIO_InitStruct1);

  GPIO_InitStruct1.GPIO_Pin = Pin_Add; //KEY ADD
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_Add;
  GPIO_Init(&GPIO_InitStruct1);

  GPIO_InitStruct1.GPIO_Pin = Pin_Up; //KEY UP
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_Up;
  GPIO_Init(&GPIO_InitStruct1);

  GPIO_InitStruct1.GPIO_Pin = Pin_Down; //KEY DOWN
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_Down;
  GPIO_Init(&GPIO_InitStruct1);

  GPIO_InitStruct1.GPIO_Pin = Pin_In; //KEY K1 IN
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_In;
  GPIO_Init(&GPIO_InitStruct1);

  GPIO_InitStruct1.GPIO_Pin = Pin_Out; //KEY K2 OUT
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct1.GPIOx = Port_Out;
  GPIO_Init(&GPIO_InitStruct1);
	
  GPIO_InitStruct1.GPIO_Pin = Pin_Buzzer; //Buzzer
  GPIO_InitStruct1.GPIO_InitState = Bit_RESET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
  GPIO_InitStruct1.GPIOx = Port_Buzzer;
  GPIO_Init(&GPIO_InitStruct1);

//**********************
  FTM_InitStruct1.Frequency = 10000; // 1KHZ
  FTM_InitStruct1.FTMxMAP = FTM_Right_Forward;
  FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned; //���ض���ģʽ
  FTM_InitStruct1.InitalDuty = 0;                  //��ʼ�������40%��ռ�ձ�
  FTM_Init(&FTM_InitStruct1);

  FTM_InitStruct1.Frequency = 10000; // 1KHZ
  FTM_InitStruct1.FTMxMAP = FTM_Left_Forward;
  FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned; //���ض���ģʽ
  FTM_InitStruct1.InitalDuty = 0;                  //��ʼ�������40%��ռ�ձ�
  FTM_Init(&FTM_InitStruct1);
  
  FTM_InitStruct1.Frequency = 10000; // 1KHZ
  FTM_InitStruct1.FTMxMAP = FTM_Right_Backward;
  FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned; //���ض���ģʽ
  FTM_InitStruct1.InitalDuty = 0;
  FTM_Init(&FTM_InitStruct1);
  
  FTM_InitStruct1.Frequency = 10000; // 1KHZ
  FTM_InitStruct1.FTMxMAP = FTM_Left_Backward;
  FTM_InitStruct1.FTM_Mode = FTM_Mode_EdgeAligned; //���ض���ģʽ
  FTM_InitStruct1.InitalDuty = 0;                  //��ʼ�������40%��ռ�ձ�
  FTM_Init(&FTM_InitStruct1);

//**********************

  DMACNT_InitStruct1.DMACNT_Mode = DMACNT_MODE_RISING;  //�����ؼ���
  DMACNT_InitStruct1.DMA_Chl = DMA_CH1;                 //ʹ��ͨ�� 0
  DMACNT_InitStruct1.GPIOx = Port_Encoder_Left;                       
  DMACNT_InitStruct1.GPIO_Pin = Pin_Encoder_Left;
  DMACNT_Init(&DMACNT_InitStruct1);

  DMACNT_InitStruct1.DMACNT_Mode = DMACNT_MODE_RISING;
  DMACNT_InitStruct1.DMA_Chl = DMA_CH2;
  DMACNT_InitStruct1.GPIOx = Port_Encoder_Right;
  DMACNT_InitStruct1.GPIO_Pin = Pin_Encoder_Right;
  DMACNT_Init(&DMACNT_InitStruct1);	

  //������������
  GPIO_InitStruct1.GPIOx = Port_Encoder_Direction_Left;
  GPIO_InitStruct1.GPIO_InitState = Bit_SET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
  GPIO_InitStruct1.GPIO_Pin = Pin_Encoder_Direction_Left;
  GPIO_Init(&GPIO_InitStruct1);

  //�ұ����������
  GPIO_InitStruct1.GPIOx = Port_Encoder_Direction_Right;
  GPIO_InitStruct1.GPIO_InitState = Bit_SET;
  GPIO_InitStruct1.GPIO_IRQMode = GPIO_IT_DISABLE;
  GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_OPP;
  GPIO_InitStruct1.GPIO_Pin = Pin_Encoder_Direction_Right;
  GPIO_Init(&GPIO_InitStruct1);
  
}

