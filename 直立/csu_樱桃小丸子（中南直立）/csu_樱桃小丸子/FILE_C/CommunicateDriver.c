 /*  
 * @file       CommunicateDriver.c
 * @brief      通信底层驱动
 * @author     中南大学黄竞辉
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
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;         //开启D端口时钟
  UART_port->ISFR = (1 << UART_RXpin) | (1 << UART_TXpin);
  UART_port->PCR[UART_RXpin] = PORT_PCR_MUX(3);  //PTB16   UART_RX
  UART_port->PCR[UART_TXpin] = PORT_PCR_MUX(3);  //PTB17   UART_TX
  
  SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;           //开启UART0时钟
  UART_C2_REG(UART0_BASE_PTR) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );  
  UART_C1_REG(UART0_BASE_PTR) |= 0;
  
  //UART 波特率 = UART 模块时钟 / (16 × (SBR[12:0] + BRFA))
  uartclk = core_clk_khz * 1000;            //选用核心时钟  
  sbr = (uint16)(uartclk / (baud * 16));
  if(sbr > 0x1fff) sbr = 0x1fff;
  brfa = uartclk/baud - 16 * sbr;     ///确定了波特率直接计算把，这样精度很高
  
  temp = UART_BDH_REG(UART0_BASE_PTR) & (~UART_BDH_SBR_MASK);           //缓存 清空 SBR 的 UART0_BDH的值
  UART_BDH_REG(UART0_BASE_PTR) = temp |  UART_BDH_SBR(sbr >> 8);        //先写入SBR高位
  UART_BDL_REG(UART0_BASE_PTR) = UART_BDL_SBR(sbr);                     //再写入SBR低位

    //写 BRFD
  temp = UART_C4_REG(UART0_BASE_PTR) & (~UART_C4_BRFA_MASK) ;           //缓存 清空 BRFA 的 UARTx_C4 的值
  UART_C4_REG(UART0_BASE_PTR) = temp |  UART_C4_BRFA(brfa); 
  
  //使能FIFO
  UART_PFIFO_REG(UART0_BASE_PTR) |= (UART_PFIFO_TXFE_MASK );
  UART_PFIFO_REG(UART0_BASE_PTR) |= (UART_PFIFO_RXFE_MASK );
  UART_CFIFO_REG(UART0_BASE_PTR) |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
   
  UART_C2_REG(UART0_BASE_PTR) |= UART_C2_RIE_MASK;                 //使能接收中断
  UART_C5_REG(UART0_BASE_PTR) |= UART_C5_RDMAS_MASK;                //使能接收DMA中断
  UART_C2_REG(UART0_BASE_PTR) |= (UART_C2_TE_MASK| UART_C2_RE_MASK );//使能发送接收
  
}

void Communicate_dma_init(uint8 *Rx)
{
  ///////以上为UART0初始化
  //////以下为DMA采集RX数据初始化
    //开启DMA时钟
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
    ///////DMA通道选择   DMA1
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
    DMA_BASE_PTR->TCD[1].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(BUFFSIZE);//当前传输的主循环的次数
    DMA_BASE_PTR->TCD[1].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(BUFFSIZE);//每次传输的主循环的次数
    DMA_BASE_PTR->TCD[1].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(1);//每次主循环传输的字节数
    DMA_BASE_PTR->TCD[1].SADDR = (uint32)((void *)&(UART0_BASE_PTR->D)); //源地址
    DMA_BASE_PTR->TCD[1].SOFF  = 0x00u;                 //源地址偏移
    DMA_BASE_PTR->TCD[1].ATTR  = DMA_ATTR_SSIZE(0); //8bits  源的地址的模数，每次偏移的数据宽度
    DMA_BASE_PTR->TCD[1].SLAST = (uint32)(0);   //源地址偏移0
    DMA_BASE_PTR->TCD[1].DADDR = (uint32)((void *)Rx);   //目的地址
    DMA_BASE_PTR->TCD[1].DOFF  = 1;                  //目的地址偏移 1个字节
    DMA_BASE_PTR->TCD[1].ATTR  = DMA_ATTR_DSIZE(0);  //8bits  目的地址的模数，每次偏移的数据宽度
    DMA_BASE_PTR->TCD[1].DLAST_SGA = (uint32)(-BUFFSIZE);  // 循环完成后指向目的地址首址

    DMA_BASE_PTR->TCD[1].CSR &=~ (0
                      |DMA_CSR_MAJORELINK_MASK //Disabled channel-to-channel linking on major loop complete
                      |DMA_CSR_ESG_MASK
                      |DMA_CSR_INTHALF_MASK    //The half-point interrupt is disabled
                      |DMA_CSR_DREQ_MASK       //主循环完成后不停止使能
                      );
    DMA_BASE_PTR->TCD[1].CSR |= (DMA_CSR_INTMAJOR_MASK | DMA_CSR_BWC(0));  //完成一次主循环产生中断
      
    DMA_ERQ &=~(DMA_ERQ_ERQ0_MASK << 1);                //关闭该中断
    DMA_INT |= (DMA_INT_INT0_MASK << 1);                //清中断标志位
  
    DMA_ERQ |= (DMA_ERQ_ERQ0_MASK << 1);  
    DMA_BASE_PTR->TCD[1].CSR &=~ DMA_CSR_START_MASK;    // 
    
    set_vector_handler(DMA1_VECTORn, DMA1_IRQHandler);
    NVIC_SetPriority(DMA1_IRQn, 0);                    ////最高中断，此中断过程短，可以放最高
    NVIC_EnableIRQ(DMA1_IRQn);                         /////开启DMA1中断
    return; 
}

extern volatile size_t DMALoopCount;
void DMA1_IRQHandler(void)
{
   DMA_INT |= (DMA_INT_INT0_MASK << 1);                //清中断标志位
   DMALoopCount++;
}

void SendChar(uint8 ch)               //发送单个字符
{
    while(!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_TDRE_MASK));
    UART_D_REG(UART0_BASE_PTR) = ch;
}

uint8 RecvChar()
{
    uint8 ch;
    while (!(UART_S1_REG(UART0_BASE_PTR) & UART_S1_RDRF_MASK));       //等待接收满了
    ch =  UART_D_REG(UART0_BASE_PTR);
    return ch;
}

void SendBuff(uint8 *str, uint16 size) //发送一个数据包
{
  while(size--)
  {
    SendChar(*str);
    str++;
  }
}

int fputc(int ch, FILE *f)   //printf重定向
{
  SendChar((uint8)ch);
  return ch;
}


int fgetc(FILE *f)          //scanf重定向
{
  return RecvChar();
}

