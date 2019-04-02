#define UART_port         PORTB_BASE_PTR
#define UART_RXpin        16
#define UART_TXpin        17
#define UART_BAUD         (uint32)(115200 * 4)
#define BUFFSIZE          2048

void DMA1_IRQHandler(void);
void Communicate_uart_init(void);
void Communicate_dma_init(uint8 *Rx);
void SendChar(uint8 ch);
void SendBuff(uint8 *str, uint16 size);