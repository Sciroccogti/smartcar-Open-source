/********************************************************************
 * M8266HostIf.c
 * .Description
 *     Source file of M8266WIFI Host Interface 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#include "common.h"
#include "include.h"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"

/***********************************************************************************
 * M8266HostIf_GPIO_SPInCS_nRESET_Pin_Init                                         *
 * Description                                                                     *
 *    To initialise the GPIOs for SPI nCS and nRESET output for M8266WIFI module   *
 *    You may update the macros of GPIO PINs usages for nRESET from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_GPIO_CS_RESET_Init(void)
{
    gpio_init(M8266WIFI_nRESET_GPIO, GPO, 1);  // set nRESET pin output and initially output high
    gpio_init(M8266WIFI_nCS_GPIO,    GPO, 1);  // set nCS    pin output and initially output high
}

/***********************************************************************************
 * M8266HostIf_SPI_Init                                                            *
 * Description                                                                     *
 *    To Initialise the SPI Host IF for M8266WIFI module                           *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
uint32 M8266HostIf_SPI_Init(uint32 baud)
{
    uint32 fit_clk;
  
  
    SIM_SCGCx_FOR_SPI |= SIM_SCGC_DSPIx_MASK;           // SIM(System integration module) low-power logic. Enable DSPI clock

    port_init(SPI_SCK_GPIO,     ALT2  );                // Configure the GPIO to SPI alternative functions
    port_init(SPI_MOSI_GPIO,    ALT2  );
    port_init(SPI_MISO_GPIO ,   ALT2  );


    SPI_MCR_REG(M8266WIFI_INTERFACE_SPI) = ( 0
                                                | SPI_MCR_CLR_TXF_MASK     // Clear Tx FIFO Counter
                                                | SPI_MCR_CLR_RXF_MASK     // Clear Rx FIFO Counter
                                                | SPI_MCR_HALT_MASK        // Stop SPI Operation
                                                  );
    SPI_SR_REG(M8266WIFI_INTERFACE_SPI) = (0
                                                | SPI_SR_EOQF_MASK        // ���Ͷ��п��ˣ��������
                                                | SPI_SR_TFUF_MASK        // ����FIFO�����־λ��SPIΪ�ӻ�ģʽ��Tx FIFOΪ�գ����ⲿSPI����ģʽ�������䣬��־λ�ͻ���1��д1��0
                                                | SPI_SR_TFFF_MASK        // ����FIFO����־λ�� д1����DMA���������ִ���FIFO���˾ͻ���0�� 0��ʾTx FIFO����
                                                | SPI_SR_RFOF_MASK        // ����FIFO�����־λ��
                                                | SPI_SR_RFDF_MASK        // ����FIFO��ı�־λ��д1����DMA���������ִ���FIFO���˾ͻ���0��0��ʾRx FIFO��
                                          );

    SPI_MCR_REG(M8266WIFI_INTERFACE_SPI)= (0
                                                |  SPI_MCR_MSTR_MASK      // Master,����ģʽ
                                                |  SPI_MCR_PCSIS_MASK
                                          );
    
    {  // searching the closet frequency scalers
       uint8  br,pbr;
       uint32 clk = bus_clk_khz*1000/baud;
       uint32 Scaler[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
       uint8  Prescaler[] = {2,3,5,7};
       uint32 fit_br=0,fit_pbr,min_diff =~0,diff;
       uint32 tmp;

       for(br = 0;br < 0x10;br++)
       {
            for(pbr = 0;pbr < 4;pbr++)
            {
                tmp = Scaler[br] * Prescaler[pbr];
                diff = abs(tmp - clk);
                if(min_diff > diff)
                {
                    //��ס �������
                    min_diff = diff;
                    fit_br = br;
                    fit_pbr = pbr;
                    
                    if(min_diff == 0)
                    {
                        //�պ�ƥ��
                        goto SPI_CLK_EXIT;
                    }
                }
            }
       }
       SPI_CLK_EXIT:
            fit_clk =  bus_clk_khz *1000 /(Scaler[fit_br] * Prescaler[fit_pbr]);
            
            SPI_CTAR_REG(M8266WIFI_INTERFACE_SPI, M8266WIFI_SPI_USE_CATRx_No) 
                                        = (0
                                              //| SPI_CTAR_DBR_MASK       // ˫������ ������ DBR=1��CPHA=1��PBR=00����SCK Duty Cycle Ϊ 50/50
                                              //| SPI_CTAR_CPHA_MASK      // ������SCK�����ظı䣨����������½��ر���׽�������ȡ���������0����֮��M8266WIFI �������ض�ȡ����
                                                | SPI_CTAR_PBR(fit_pbr)   // �����ʷ�Ƶ�� ��0~3 ��Ӧ�ķ�ƵֵPrescalerΪ 2��3��5��7
                                                | SPI_CTAR_PDT(0x00)      // ��ʱ����Ϊ PDT*2+1 ������PDTΪ3������ʱ����Ϊ7��PDTΪ2bit
                                                | SPI_CTAR_BR(fit_br)     // �����ʼ�����ֵ ,��BR<=3,��ƵScaler Ϊ 2*��BR+1�� ����BR>=3����ƵScaler Ϊ 2^BR  ��BRΪ4bit
                                                                          // SCK ������ = (Bus clk/Prescaler) x [(1+DBR)/Scaler ]
                                                                          // 50M / 2  x [ 1  /  2  ] = 25M   ��������������
                                              //| SPI_CTAR_CPOL_MASK      // ʱ�Ӽ��ԣ�1��ʾ SCK ����Ծ״̬Ϊ�ߵ�ƽ, 0 ����ԾΪ�͵�ƽ, M8266WIFI����ԾΪ�͵�ƽ
                                                | SPI_CTAR_FMSZ(0x07)     // ÿ֡���� 7bit+1 ����8bit ��FMSZĬ�Ͼ���8��
                                             // | SPI_CTAR_LSBFE_MASK     // 1Ϊ��λ��ǰ��M8266WIFIΪMSB First
                                             // | SPI_CTAR_CSSCK(1)       //
                                             // | SPI_CTAR_PCSSCK(2)      // ����Ƭѡ�ź���Ч��ʱ�ӵ�һ�����س��ֵ���ʱ��Ԥ��Ƶֵ��tcsc��ʱԤ��Ƶ 2*x+1��
                                      );

    }
    
    SPI_MCR_REG(M8266WIFI_INTERFACE_SPI) &= ~SPI_MCR_HALT_MASK;     //����SPI���䡣1Ϊ��ͣ��0Ϊ����

    return fit_clk;

}

/***********************************************************************************
 * M8266HostIf_SPI_SetSpeed                                                        *
 * Description                                                                     *
 *    To setup the SPI Clock Speed for M8266WIFI module                            *
 * Parameter(s):                                                                   *
 *    SPI_BaudRatePrescaler: SPI BaudRate Prescaler                                *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_SPI_SetSpeed(u32 SPI_BaudRatePrescaler)
{
} 

/***********************************************************************************
 * M8266HostIf_Init                                                                *
 * Description                                                                     *
 *    To initialise the Host interface for M8266WIFI module                        *
 * Parameter(s):                                                                   *
 *    baud: baud rate to set                                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/ 
void M8266HostIf_Init(void)
{
    M8266HostIf_GPIO_CS_RESET_Init();
#if      defined(MK60F15)   
    M8266HostIf_SPI_Init(15000000);  // 15 MHz  // = 60 / 2*2
#elif    defined(MK60DZ10)
    M8266HostIf_SPI_Init(12000000);  // 12 MHz  // = 48 / 2*2
#else    
#error   UKNOWN MK60 Subtype, F15 or Z10?
#endif                                                
}

//////////////////////////////////////////////////////////////////////////////////////
// BELOW FUNCTIONS ARE REQUIRED BY M8266WIFIDRV.LIB. 
// PLEASE IMPLEMENTE THEM ACCORDING TO YOUR HARDWARE
//////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * M8266HostIf_Set_nRESET_Pin                                                      *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI nRESET                *
 *    You may update the macros of GPIO PIN usages for nRESET from brd_cfg.h       *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to nRESET pin                                         *
 *              0 = output LOW  onto nRESET                                        *
 *              1 = output HIGH onto nRESET                                        *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_nRESET_Pin(u8 level)
{
     if(level!=0)
       gpio_set(M8266WIFI_nRESET_GPIO, 1);
     else
       gpio_set(M8266WIFI_nRESET_GPIO, 0);
}
/***********************************************************************************
 * M8266HostIf_Set_SPI_nCS_PIN                                                     *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI SPI nCS               *
 *    You may update the macros of GPIO PIN usages for SPI nCS from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to SPI nCS pin                                        *
 *              0 = output LOW  onto SPI nCS                                       *
 *              1 = output HIGH onto SPI nCS                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_SPI_nCS_Pin(u8 level)
{
     if(level!=0)
       gpio_set(M8266WIFI_nCS_GPIO, 1);
     else
       gpio_set(M8266WIFI_nCS_GPIO, 0);
}

/***********************************************************************************
 * M8266WIFIHostIf_delay_us                                                        *
 * Description                                                                     *
 *    To loop delay some micro seconds.                                            *
 * Parameter(s):                                                                   *
 *    1. nus: the micro seconds to delay                                           *
 * Return:                                                                         *
 *    none                                                                         *
 ***********************************************************************************/
void M8266HostIf_delay_us(u8 nus)
{
   systick_delay_us(nus);
}

/***********************************************************************************
 * M8266HostIf_SPI_ReadWriteByte                                                   *
 * Description                                                                     *
 *    To write a byte onto the SPI bus from MCU MOSI to the M8266WIFI module       *
 *    and read back a byte from the SPI bus MISO meanwhile                         *
 *    You may update the macros of SPI usage from brd_cfg.h                        *
 * Parameter(s):                                                                   *
 *    1. TxdByte: the byte to be sent over MOSI                                    *
 * Return:                                                                         *
 *    1. The byte read back from MOSI meanwhile                                    *                                                                         *
 ***********************************************************************************/
u8 M8266HostIf_SPI_ReadWriteByte(u8 TxdByte)
{
   u8 data;
   spi_mosi(SPI1,SPIn_PCS0,&TxdByte,&data,1); 
   return data;
}
