 /*  
 * @file       CommunicateDriver.c
 * @brief      ͨ�ŵײ�����
 * @author     ���ϴ�ѧ�ƾ���
 * @version    v1.0
 * @date       2016-12-13
 */

#include "GeneralDeclare.h"
#include "CommunicateDriver.h"


void Communicate_uart_init(void)
{
  register uint16 sbr, brfa;
  uint8 temp;
  uint32 uartclk;
  uint32 baud;
  baud = UART_BAUD;
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;         //����D�˿�ʱ��
  UART_port->ISFR = (1 << UART_RXpin) | (1 << UART_TXpin);
  UART_port->PCR[UART_RXpin] = PORT_PCR_MUX(3);  //PTB16   UART_RX
  UART_port->PCR[UART_TXpin] = PORT_PCR_MUX(3);  //PTB17   UART_TX
  
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;           //����UART0ʱ��
  UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );  
  UART_C1_REG(UART0_BASE_PTR) |= 0;
  
  //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA))
  uartclk = core_clk_khz * 1000;            //ѡ�ú���ʱ��  
  sbr = (uint16)(uartclk / (baud * 16));
  if(sbr > 0x1fff) sbr = 0x1fff;
  brfa = uartclk/baud - 16 * sbr;     ///ȷ���˲�����ֱ�Ӽ���ѣ��������Ⱥܸ�
  
  temp = UART_BDH_REG(UART0_BASE_PTR) & (~UART_BDH_SBR_MASK);           //���� ��� SBR �� UART0_BDH��ֵ
  UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(sbr >> 8);        //��д��SBR��λ
  UART_BDL_REG(UART0_BASE_PTR) = UART_BDL_SBR(sbr);                     //��д��SBR��λ

    //д BRFD
  temp = UART_C4_REG(UART0_BASE_PTR) & (~UART_C4_BRFA_MASK) ;           //���� ��� BRFA �� UARTx_C4 ��ֵ
  UART_C4_REG(UART0_BASE_PTR) = temp |  UART_C4_BRFA(brfa); 
  
  //ʹ��FIFO
  UART_PFIFO_REG(UART0_BASE_PTR) |= (UART_PFIFO_TXFE_MASK );
  UART_PFIFO_REG(UART0_BASE_PTR) |= (UART_PFIFO_RXFE_MASK );
  UART_CFIFO_REG(UART0_BASE_PTR) |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
   
  UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;                 //ʹ�ܽ����ж�
  UART_C5_REG(UART0_BASE_PTR) |= UART_C5_RDMAS_MASK;                //ʹ�ܽ���DMA�ж�
  UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK| UART_C2_RE_MASK );//ʹ�ܷ��ͽ���
  
}

void Communicate_dma_init(uint8 *Rx)
{
  ///////����ΪUART0��ʼ��
  //////����ΪDMA�ɼ�RX���ݳ�ʼ��
    //����DMAʱ��
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
    ///////DMAͨ��ѡ��   DMA1
    DMAMUX_CHCFG1 = 0x00;
    DMAMUX_CHCFG1 &=~ DMAMUX_CHCFG_ENBL_MASK;
    DMAMUX_CHCFG1 &=~ DMAMUX_CHCFG_TRIG_MASK;
    DMAMUX_CHCFG1 |=  DMAMUX_CHCFG_SOURCE(2);         //UART0 Recieve
    DMAMUX_CHCFG1 |=  DMAMUX_CHCFG_ENBL_MASK;
    
    DMA_CR &=~ DMA_CR_EDBG_MASK;
    DMA_CR |= DMA_CR_EMLM_MASK;                            //Enable Minor Loop Mapping
    
    DMA_BASE_PTR->TCD[1].CITER_ELINKNO = 0x00000000;
    DMA_BASE_PTR->TCD[1].BITER_ELINKNO = 0x00000000;
    DMA_BASE_PTR->TCD[1].CITER_ELINKNO &=~ DMA_CITER_ELINKNO_ELINK_MASK;//The channel-to-channel linking is disabled
    DMA_BASE_PTR->TCD[1].BITER_ELINKNO &=~ DMA_BITER_ELINKNO_ELINK_MASK;//The channel-to-channel linking is disabled
    DMA_BASE_PTR->TCD[1].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(BUFFSIZE);//��ǰ�������ѭ���Ĵ���
    DMA_BASE_PTR->TCD[1].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(BUFFSIZE);//ÿ�δ������ѭ���Ĵ���
    DMA_BASE_PTR->TCD[1].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(1);//ÿ����ѭ��������ֽ���
    DMA_BASE_PTR->TCD[1].SADDR = (uint32)((void *)&(UART0_BASE_PTR->D)); //Դ��ַ
    DMA_BASE_PTR->TCD[1].SOFF  = 0x00u;                 //Դ��ַƫ��
    DMA_BASE_PTR->TCD[1].ATTR  = DMA_ATTR_SSIZE(0); //8bits  Դ�ĵ�ַ��ģ����ÿ��ƫ�Ƶ����ݿ��
    DMA_BASE_PTR->TCD[1].SLAST = (uint32)(0);   //Դ��ַƫ��0
    DMA_BASE_PTR->TCD[1].DADDR = (uint32)((void *)Rx);   //Ŀ�ĵ�ַ
    DMA_BASE_PTR->TCD[1].DOFF  = 1;                  //Ŀ�ĵ�ַƫ�� 1���ֽ�
    DMA_BASE_PTR->TCD[1].ATTR  = DMA_ATTR_DSIZE(0);  //8bits  Ŀ�ĵ�ַ��ģ����ÿ��ƫ�Ƶ����ݿ��
    DMA_BASE_PTR->TCD[1].DLAST_SGA = (uint32)(-BUFFSIZE);  // ѭ����ɺ�ָ��Ŀ�ĵ�ַ��ַ

    DMA_BASE_PTR->TCD[1].CSR &=~ (0
                      |DMA_CSR_MAJORELINK_MASK //Disabled channel-to-channel linking on major loop complete
                      |DMA_CSR_ESG_MASK
                      |DMA_CSR_INTHALF_MASK    //The half-point interrupt is disabled
                      |DMA_CSR_DREQ_MASK       //��ѭ����ɺ�ֹͣʹ��
                      );
    DMA_BASE_PTR->TCD[1].CSR |= (DMA_CSR_INTMAJOR_MASK | DMA_CSR_BWC(0));  //���һ����ѭ�������ж�
      
    DMA_ERQ &=~(DMA_ERQ_ERQ0_MASK << 1);                //�رո��ж�
    DMA_INT |= (DMA_INT_INT0_MASK << 1);                //���жϱ�־λ
  
    DMA_ERQ |= (DMA_ERQ_ERQ0_MASK << 1);  
    DMA_BASE_PTR->TCD[1].CSR &=~ DMA_CSR_START_MASK;    // 
    
    set_vector_handler(DMA1_VECTORn, DMA1_IRQHandler);
    NVIC_SetPriority(DMA1_IRQn, 0);                    ////����жϣ����жϹ��̶̣����Է����
    NVIC_EnableIRQ(DMA1_IRQn);                         /////����DMA1�ж�
    return; 
}

extern volatile size_t DMALoopCount;
void DMA1_IRQHandler(void)
{
   DMA_INT |= (DMA_INT_INT0_MASK << 1);                //���жϱ�־λ
   DMALoopCount++;
}

void SendChar(uint8 ch)               //���͵����ַ�
{
    while(!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_TDRE_MASK));
    UART_D_REG(UART0_BASE_PTR) = ch;
}

uint8 RecvChar()
{
    uint8 ch;
    while (!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK));       //�ȴ���������
    ch =  UART_D_REG(UART0_BASE_PTR);
    return ch;
}

void SendBuff(uint8 *str, uint16 size) //����һ�����ݰ�
{
  while(size--)
  {
    SendChar(*str);
    str++;
  }
}

int fputc(int ch, FILE *f)   //printf�ض���
{
  SendChar((uint8)ch);
  return ch;
}


int fgetc(FILE *f)          //scanf�ض���
{
  return RecvChar();
}

