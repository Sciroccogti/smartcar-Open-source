
#include "LDPY_UART.h"

/*************************************************************************
*                     	��	��	ƫ	��
*  ��������:LDPY_UART5_Init
*  ����˵��:��ʼ������5�����ò�����115200
*  ����˵��:�������Ϊ������
*            
*  ��������:
*  �޸�ʱ��:2014-05-10
*  ��	ע:UART0��1ʹ���ں�ʱ�ӣ�����ʹ������busʱ��
*************************************************************************/
void LDPY_UART5_Init(uint32_t UART_baud_rate)
{
	uint16_t sbr, brfa;
	uint8_t temp;
	uint32_t UART_module_clock = 100000 * 1000;//100MHz

	PORTD->PCR[9] = PORT_PCR_MUX(0x3);         //ʹ��PTD9   UART5_TXD
      PORTD->PCR[8] = PORT_PCR_MUX(0x3);         //ʹ��PTD8   UART5_RXD
      SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;     	 //ʹ��UART5ʱ��
	                    
	 //�Ƚ�ֹ���͡�����
	UART5->C2 &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

	//8λģʽ������żУ��
		
	UART5->C1 = 0;	
	 
	// UART_module_clock = 50000 * 1000;
	sbr = (uint16_t)(UART_module_clock / (UART_baud_rate << 4));
	 
	// Save off the current value of the UARTx_BDH except for the SBR field 
	temp = UART5->BDH & ~(UART_BDH_SBR(0x1F));

	UART5->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
	UART5->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);

	//brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
	brfa = (((UART_module_clock << 5) / (UART_baud_rate << 4)) - (sbr << 5));

	// Save off the current value of the UARTx_C4 register except for the BRFA field 
	temp = UART5->C4 & ~(UART_C4_BRFA(0x1F));

	UART5->C4 = temp |  UART_C4_BRFA(brfa);//���ò�����΢��ֵ

	UART5->C2 |= UART_C2_RIE_MASK;//ʹ�ܽ����ж�
	
	/*--------------DMA����-----------------*/
	UART5->C2 |= UART_C2_RIE_MASK;//TDRE�жϺ�DMA��������ʹ��
	UART5->C2 |= UART_C2_TIE_MASK;//TC�ж��������
	UART5->C5 |= UART_C5_TDMAS_MASK;//����ΪDMA����

	//ʹ�ܷ��͡�����
	UART5->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
 
	Un_ENABLE_IRQ(55);//��ֹUART5�����жϣ�55Ϊ����5״̬�жϺ�
}
 
/*************************************************************************
*                     	��	��	ƫ	��
*  ��������:LDPY_UART5_PutChar
*  ����˵��:����5����һ���ַ�
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:2014-05-10
*  ��	ע:
*************************************************************************/
void LDPY_UART5_PutChar(uint8_t ch)
{
	while(!(UART_X->S1 & UART_S1_TC_MASK));//��������ɱ�־=1ʱ����UART_S1_TC_MASK
	
	UART_X->D = ch;		
}

/*************************************************************************
*                     	��	��	ƫ	��
*  ��������:LDPY_UART5_PutCharArr
*  ����˵��:����5����һ���ַ�
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:2014-05-10
*  ��	ע:
*************************************************************************/
void LDPY_UART5_PutCharArr(char *ch,int len)
{
  while(len--)
  {
    LDPY_UART5_PutChar(*(ch++));
  }
}

/*************************************************************************
*                     	��	��	ƫ	��
*  ��������:LDPY_UART5_GetChar
*  ����˵��:����5����һ���ַ�
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:2014-05-10
*  ��	ע:
*************************************************************************/ 
uint8_t LDPY_UART5_GetChar(void)
{
	while(!(UART_X->S1 & UART_S1_RDRF_MASK));//�ȴ���������ֵ
	
	return UART_X->D;
}

/*************************************************************************
*                     	��	��	ƫ	��
*  ��������:LDPY_UART5_ClearReceiveBuffer
*  ����˵��:�崮�ڻ�����
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:2014-05-10
*  ��	ע:
*************************************************************************/ 
void LDPY_UART5_ClearReceiveBuffer(void)
{

  while ((UART_X->S1 & UART_S1_RDRF_MASK))//������������ֵ��ʱ��Ͷ�ȡ
  {
    UART_X->D;
  }
}

/*------------------------end of LDPY_UART.c--------------------------------*/
	
	
