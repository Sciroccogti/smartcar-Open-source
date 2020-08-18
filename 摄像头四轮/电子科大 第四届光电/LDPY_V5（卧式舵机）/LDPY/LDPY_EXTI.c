

#include "LDPY_EXTI.h"
/*************************************************************************
*				��	��	ƫ	��
*  ��������:  EXTI_Init�ⲿ�����ж����ú���
*  ����˵��:  
*  ����˵��:	IRQ_number�жϺ����ֲ�20ҳ

*  ��������:
*  �޸�ʱ��:
*  ��		 ע:	ֻ��ʼ��PORTx,�ⲿ�����жϣ�		
*************************************************************************/ 	

void EXTI_Init(PORT_Type * PORTx,uint8_t GPIO_Pin_x,EXTI_Trigger_mode  Trigger_mode )
{
	uint8_t IRQ_number=0x00,IRQ_Group,IRQ_Posit;
	
	PORTx->PCR[GPIO_Pin_x]&=(~0x000F0703);		//��[19:16][1:0]����Ϊȱʡֵ
	PORTx->PCR[GPIO_Pin_x]|=PORT_PCR_MUX(1);  	//GPIO
	PORTx->PCR[GPIO_Pin_x]|=Trigger_mode;		//���ô���ģʽ
	
	/*-----------���ùܽ�Ϊ����ģʽ------------------*/
	
	if((uint32_t)PORTx==PORTA_BASE)
	{
		GPIOA->PDDR&=~(1<<GPIO_Pin_x);  		//����Ӧ�ܽ���0����Ϊ����ģʽ
		IRQ_number=87;					// �ж������IRQ
	}
	else if((uint32_t)PORTx==PORTB_BASE)
	{
		GPIOB->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=88;
	}
	else if((uint32_t)PORTx==PORTC_BASE)
	{
		GPIOC->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=89;
	}
	else if((uint32_t)PORTx==PORTD_BASE)
	{
		GPIOD->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=90;
	}
	else if((uint32_t)PORTx==PORTE_BASE)
	{
		GPIOE->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=91;
	}
									
	IRQ_Group=IRQ_number/32;	//�����жϺ��ڼĴ��������ƫ�ƣ����[0~7]
	IRQ_Posit=IRQ_number%32;	//�����жϺ��ڼĴ������λ��ƫ�ƣ�λ��[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // �����жϹ��𣬱������
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // ʹ���ж�			
}

/*************************************************************************
*				��	��	ƫ	��    
*
*  ��������:LDPY_Isr_SetIrqPriority
*  ����˵��:�����жϷ���
*  ����˵��: NVIC_Group(0~4)
*			 �� ��		AIRCR[10:8]		 IPR[7:0](x1:x2  x1λ��ռ���ȼ�,x2λ��Ӧ���ȼ�)
			   0			111			   0:4
			   1			110			   1:3
			   2			101			   2:2
			   3			100			   3:1
			   4			011			   4:0
*  ��������:
*  �޸�ʱ��:2014-08-10
*  ��    ע:
*************************************************************************/
void LDPY_NVIC_PriorityGroupConfig(uint8_t NVIC_Group)
{
	uint32_t Group_temp = 0;
	uint32_t SCB_temp = 0;
	
	Group_temp = (~NVIC_Group) & 0x07;
	Group_temp <<= 8;
	
	SCB_temp = SCB->AIRCR;	//��ȡ
	SCB_temp &= 0x0000F8FF; //����
	SCB_temp |= 0x05FA0000; //д���ܳ�
	SCB_temp |= Group_temp; //�������
	
	SCB->AIRCR = SCB_temp;  //д��
}
/*************************************************************************
*				��	��	ƫ	��    
*
*  ��������:LDPY_NVIC_Init
*  ����˵��:�����ж����ȼ�
*  ����˵��:	irq Ϊ�ж�������	
*			prioΪ���ȼ���0Ϊ������ȼ���Ȼ��1,2��3.....���ν���
*  ��������:
*  �޸�ʱ��:2014-05-14
*  ��    ע:
*************************************************************************/	
void LDPY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_SubPriority,uint8_t NVIC_Channel,uint8_t NVIC_Group) 
{
	uint8_t IPRADDR = 0, IPROFFSET = 0;
	uint8_t IPR_temp = 0;
	
	IPRADDR = NVIC_Channel / 4;		//���ַ
	IPROFFSET = NVIC_Channel % 4;	//����ƫ��
	IPROFFSET = IPROFFSET * 8 + 4;  //����λ��
	
	IPR_temp  = NVIC_PreemptionPriority << (4 - NVIC_Group);
	IPR_temp |= NVIC_SubPriority & (0x0F >> NVIC_Group);
	IPR_temp &= 0x0F;
	
	LDPY_NVIC_PriorityGroupConfig(NVIC_Group);
	
	if(NVIC_Channel<32)
	{
		NVIC->ISER[0]|=1<<NVIC_Channel;
	}
	else 			
	{		
		NVIC->ISER[1]|=1<<(NVIC_Channel-32); 
	}
	
	NVIC->IP[IPRADDR]|=IPR_temp<<IPROFFSET;		
}

/*************************************************************************
*				��	��	ƫ	��
*  ��������:  ENABLE_IRQ
*  ����˵��:  �ж�ʹ�ܺ���
*  ����˵��:	IRQ_number�жϺ����ֲ�20ҳ
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:
*************************************************************************/ 	
void ENABLE_IRQ(uint8_t  IRQ_number)
{
	uint8_t  IRQ_Group,IRQ_Posit;
	
	IRQ_Group=IRQ_number/32;	//�����жϺ��ڼĴ��������ƫ�ƣ����[0~7]
	IRQ_Posit=IRQ_number%32;	//�����жϺ��ڼĴ������λ��ƫ�ƣ�λ��[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // �����жϹ��𣬱������
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // ʹ���ж�	

}
	
/*************************************************************************
*                             ���ƫ��
*  ��������:  Un_ENABLE_IRQ
*  ����˵��:  �жϳ��ܺ���
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:
*************************************************************************/
void Un_ENABLE_IRQ(uint8_t  IRQ_number)
{
	uint8_t  IRQ_Group,IRQ_Posit;
	
	IRQ_Group=IRQ_number/32;	//�����жϺ��ڼĴ��������ƫ�ƣ����[0~7]
	IRQ_Posit=IRQ_number%32;	//�����жϺ��ڼĴ������λ��ƫ�ƣ�λ��[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // �����жϹ��𣬱������
	NVIC->ICER[IRQ_Group]=(1<<IRQ_Posit);  // 
}
