
#include "LDPY_PIT.h"
	
/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT0_Init(void)
*  ����˵��:PIT0 ��ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: Bus clock = 100MHz
*************************************************************************/		
void LDPY_PIT0_Init(void)
{
	uint32_t IRQ_Group,IRQ_Posit,IRQ_number;
	IRQ_number=68;
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK ;//ʹ�ܶ�ʱ��ʱ��
	
	PIT->MCR &=~ (PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );//ʹ�ܶ�ʱ��ʱ�ӣ�����ģʽ��������

	IRQ_Group=IRQ_number/32;	//�����жϺ��ڼĴ��������ƫ�ƣ����[0~7],ÿ�����ܲ���32���ж�
	IRQ_Posit=IRQ_number%32;	//�����жϺ��ڼĴ������λ��ƫ�ƣ�λ��[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // �����жϹ��𣬱������
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // ʹ���ж�		
}	
	
/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT0_ms(uint32_t ms_Load)
*  ����˵��:PIT0 ms��ʱ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: ���ʱ429ms
*************************************************************************/			
void LDPY_PIT0_ms(uint32_t ms_Load)
{
	
	PIT->CHANNEL[0].LDVAL= ms_Load*100000;//����ʱ��
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;//����жϱ�־λ

	PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//��ʱ��ʹ�ܣ��ж�ʹ��		
}	

/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT0_us(uint32_t ms_Load)
*  ����˵��:PIT0 us��ʱ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: 
*************************************************************************/
void LDPY_PIT0_us(uint32_t us_Load)
{	
	PIT->CHANNEL[0].LDVAL= us_Load*100;//����ʱ��
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;//����жϱ�־λ
	//
	PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//��ʱ��ʹ�ܣ��ж�ʹ��

}
	
/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT1_Init(void)
*  ����˵��:PIT1 ��ʼ��
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע:
*************************************************************************/
void LDPY_PIT1_Init(void)
{
	uint32_t IRQ_Group,IRQ_Posit,IRQ_number;
	IRQ_number=69;
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK ;//ʹ�ܶ�ʱ��ʱ��
	
	PIT->MCR &=~ (PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );//ʹ�ܶ�ʱ��ʱ�ӣ�����ģʽ��������

	IRQ_Group=IRQ_number/32;	//�����жϺ��ڼĴ��������ƫ�ƣ����[0~7],ÿ�����ܲ���32���ж�
	IRQ_Posit=IRQ_number%32;	//�����жϺ��ڼĴ������λ��ƫ�ƣ�λ��[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // �����жϹ��𣬱������
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // ʹ���ж�		
}
/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT0_10ns(uint32_t ns_Load)
*  ����˵��:PIT0 ns��ʱ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: ���ʱ 429 496 728 ns
*************************************************************************/			
void LDPY_PIT1_10ns(uint32_t ns_Load)
{		
	PIT->CHANNEL[1].LDVAL= ns_Load;//����ʱ��
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//����жϱ�־λ
	//
	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//��ʱ��ʹ�ܣ��ж�ʹ��	
}

/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT1_us(uint32_t us_Load)
*  ����˵��:PIT1 us��ʱ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: ���ʱ 429 495 us
*************************************************************************/			
void LDPY_PIT1_us(uint32_t us_Load)
{	
	PIT->CHANNEL[1].LDVAL= us_Load*100;//����ʱ��
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//����жϱ�־λ

	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//��ʱ��ʹ�ܣ��ж�ʹ��

}
/*************************************************************************
*				��	��	ƫ	��                             
*
*  ��������:LDPY_PIT1_ms(uint32_t ms_Load)
*  ����˵��:PIT1 ms��ʱ
*  ����˵��:
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��    ע: ���ʱ 428 ms
*************************************************************************/			
void LDPY_PIT1_ms(uint32_t ms_Load)
{

	PIT->CHANNEL[1].LDVAL= ms_Load*100000;//����ʱ��
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//����жϱ�־λ

	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//��ʱ��ʹ�ܣ��ж�ʹ��		
}	
	
	
/*------------------------end of myPIT.c----------------------------------*/

