
#include "LDPY_GPIO.h"
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_Init
*  ����˵��:  IO�ڳ�ʼ������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/ 
void LDPY_GPIO_Init(PORT_Type * PORTx, GPIO_InitTypeDef *GPIO_InitStruct)
{
	uint32_t PI,clear;
		
	//----------------------���ù��ܣ�������-------------	
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&=(~0x00000703);
		
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|=GPIO_InitStruct->GPIO_Mode;
	
	clear=(1<<GPIO_InitStruct->GPIO_Pin);
	PI=(GPIO_InitStruct->GPIO_Direct <<GPIO_InitStruct->GPIO_Pin);
	
	if((uint32_t)PORTx==PORTA_BASE)  		//��Ϊ���ƼĴ��������üĴ�����ͬ�����Ժ�ɵ���ж���һ��
	{
		GPIOA->PDDR&=(~clear);			//������Ӧ�ܽ�
		GPIOA->PDDR|=PI;  			//��������룬PI=0;��������,PI=1
	}
	else if((uint32_t)PORTx==PORTB_BASE)
	{
		GPIOB->PDDR&=(~clear);
		GPIOB->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTC_BASE)
	{
		GPIOC->PDDR&=(~clear);
		GPIOC->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTD_BASE)
	{
		GPIOD->PDDR&=(~clear);
		GPIOD->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTE_BASE)
	{
		GPIOE->PDDR&=(~clear);
		GPIOE->PDDR|=PI;
	}
		
}


/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_SetBits
*  ����˵��:  ����IO�ڵĵ�ƽ
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/ 
void LDPY_GPIO_SetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin)
{
	GPIO_Pin=1<<GPIO_Pin;
	GPIOx->PSOR=GPIO_Pin;	
}

	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_ResetBits
*  ����˵��:  ��һ���˿ڽ�������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/ 

void LDPY_GPIO_ResetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin)
{
	GPIO_Pin=1<<GPIO_Pin;		
	GPIOx->PCOR=GPIO_Pin;	
}
	
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_WtiteBit
*  ����˵��:  ��һ���˿ڽ�������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/
void GPIO_WriteBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin, BitAction BitVal)
{
	GPIO_Pin=1<<GPIO_Pin;
	
	if (BitVal != Bit_RESET)
	{
		GPIOx->PSOR=GPIO_Pin;
	}
	else
	{
		GPIOx->PCOR=GPIO_Pin;
	}
}
	
		
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_Write
*  ����˵��:  ��һ���˿ڽ�������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  ��һ��˿�д����
**************************************************************************/
void LDPY_GPIO_Write(GPIO_Type* GPIOx, uint32_t PortVal)
{
	GPIOx->PDOR=PortVal;
}
	
	
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_ReadInputDataBit
*  ����˵��:  ��ȡһ���˿ڵ�����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/

uint8_t LDPY_GPIO_ReadInputDataBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	GPIO_Pin=1<<GPIO_Pin;
	
	if ((GPIOx->PDIR & GPIO_Pin) != (uint32_t)Bit_RESET)	//����˿�Pin=1
	{
		bitstatus = (uint8_t)Bit_SET;  			//�򷵻�1
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	
	
	return bitstatus;
}
	
	
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  LDPY_GPIO_ReadInputData
*  ����˵��:  ��һ��˿ڵ�����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-11
*  ��		 ע:  
*************************************************************************/
uint32_t LDPY_GPIO_ReadInputData(GPIO_Type* GPIOx)
{
  return (GPIOx->PDIR);
}






