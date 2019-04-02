/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
* �ļ���       ��uart.c
* ����         �����ں���
* ��ע         ���ο����ݴ�ѧ�����̺ͷ�˼�����ٷ�������
*
* ʵ��ƽ̨     ��Ұ��kinetis������
* ��汾       ��
* Ƕ��ϵͳ     ��
*
* ����         ��
* �Ա���       ��http://firestm32.taobao.com
* ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "uart.h"
#include "assert.h"
#include "include.h"

volatile struct UART_MemMap *UARTx[6] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR}; //�������ָ�����鱣�� UARTx �ĵ�ַ

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_init
*  ����˵������ʼ�����ڣ����ò�����
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            baud        �����ʣ���9600��19200��56000��115200��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ڹٷ��������޸�
*************************************************************************/
void uart_init (UARTn uratn, u32 baud)
{
    register uint16 sbr, brfa;
    uint8 temp;
    u32 sysclk;     //ʱ��

    /* ���� UART���ܵ� GPIO �ӿ� ����ʱ�� */
    switch(uratn)
    {
      case UART0:
        if(UART0_RX == PTA1)
            PORTA_PCR1 = PORT_PCR_MUX(0x2);      //��PTA1��ʹ��UART0_RXD
        else if(UART0_RX == PTA15)
            PORTA_PCR15 = PORT_PCR_MUX(0x3);     //��PTA15��ʹ��UART0_RXD
        else if(UART0_RX == PTB16)
            PORTB_PCR16 = PORT_PCR_MUX(0x3);     //��PTB16��ʹ��UART0_RXD
        else if(UART0_RX == PTD6)
            PORTD_PCR6 = PORT_PCR_MUX(0x3);      //��PTD6��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);   //���ùܽ�����

        if(UART0_TX == PTA2)
            PORTA_PCR2 = PORT_PCR_MUX(0x2);     //��PTA2��ʹ��UART0_RXD
        else if(UART0_TX == PTA14)
            PORTA_PCR14 = PORT_PCR_MUX(0x3);     //��PTA14��ʹ��UART0_RXD
        else if(UART0_TX == PTB17)
            PORTB_PCR17 = PORT_PCR_MUX(0x3);     //��PTB17��ʹ��UART0_RXD
        else if(UART0_TX == PTD7)
            PORTD_PCR7 = PORT_PCR_MUX(0x3);     //��PTD7��ʹ��UART0_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����


        SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UARTn ʱ��
        break;

      case UART1:
        if(UART1_RX == PTC3)
            PORTC_PCR3 = PORT_PCR_MUX(0x3);     //��PTC3��ʹ��UART1_RXD
        else if(UART1_RX == PTE1)
            PORTE_PCR1 = PORT_PCR_MUX(0x3);     //��PTE1��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        if(UART1_TX == PTC4)
            PORTC_PCR4 = PORT_PCR_MUX(0x3);     //��PTC4��ʹ��UART1_RXD
        else if(UART1_TX == PTE0)
            PORTE_PCR0 = PORT_PCR_MUX(0x3);     //��PTE0��ʹ��UART1_RXD
        else
            assert_failed(__FILE__, __LINE__);  //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
        break;

      case UART2:
        PORTD_PCR3 = PORT_PCR_MUX(0x3);         //��PTD3��ʹ��UART2_TXD����
        PORTD_PCR2 = PORT_PCR_MUX(0x3);         //��PTD2��ʹ��UART2_RXD
        SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
        break;

      case UART3:
        if(UART3_RX == PTB10)
            PORTB_PCR10 = PORT_PCR_MUX(0x3);     //��PTB10��ʹ��UART3_RXD
        else if(UART3_RX == PTC16)
            PORTC_PCR16 = PORT_PCR_MUX(0x3);     //��PTC16��ʹ��UART3_RXD
        else if(UART3_RX == PTE5)
            PORTE_PCR5 = PORT_PCR_MUX(0x3);      //��PTE5��ʹ��UART3_RXD
        else 
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART3_TX == PTB11)
            PORTB_PCR11 = PORT_PCR_MUX(0x3);     //��PTB11��ʹ��UART3_RXD
        else if(UART3_TX == PTC17)
            PORTC_PCR17 = PORT_PCR_MUX(0x3);     //��PTC17��ʹ��UART3_RXD
        else if(UART3_TX == PTE4)
            PORTE_PCR4 = PORT_PCR_MUX(0x3);     //��PTE4��ʹ��UART3_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
        break;

      case UART4:
        if(UART4_RX == PTC14)
            PORTC_PCR14 = PORT_PCR_MUX(0x3);     //��PTC14��ʹ��UART4_RXD
        else if(UART4_RX == PTE25)
            PORTE_PCR25 = PORT_PCR_MUX(0x3);     //��PTE25��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART4_TX == PTC15)
            PORTC_PCR15 = PORT_PCR_MUX(0x3);     //��PTC15��ʹ��UART4_RXD
        else if(UART4_TX == PTE24)
            PORTE_PCR24 = PORT_PCR_MUX(0x3);     //��PTE24��ʹ��UART4_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;
        break;

      case UART5:
        if(UART5_RX == PTD8)
            PORTD_PCR8 = PORT_PCR_MUX(0x3);     //��PTD8��ʹ��UART5_RXD
        else if(UART5_RX == PTE9)
            PORTE_PCR9 = PORT_PCR_MUX(0x3);     //��PTE9��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        if(UART5_TX == PTD9)
            PORTD_PCR9 = PORT_PCR_MUX(0x3);     //��PTD9��ʹ��UART5_RXD
        else if(UART5_TX == PTE8)
            PORTE_PCR8 = PORT_PCR_MUX(0x3);     //��PTE8��ʹ��UART5_RXD
        else
            assert_failed(__FILE__, __LINE__);                   //���ùܽ�����

        SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;
        break;
      default:
        break;
    }


    //���õ�ʱ��Ӧ�ý�ֹ���ͽ���
    UART_C2_REG(UARTx[uratn]) &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UART_C1_REG(UARTx[uratn]) = 0;	// ȫ��ֱ��ʹ��Ĭ�����þ��У�����ֱ����0

    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ������ʱ��
    if ((uratn == UART0) | (uratn == UART1))
        sysclk = core_clk_khz * 1000;            //�ں�ʱ��
    else
        sysclk = bus_clk_khz * 1000;  //����ʱ��

    //���� UART ����ͨѶ�����ʡ�ͨ������ UART ģ��Ĳ����ʼĴ���
    sbr = (u16)(sysclk / (baud << 4));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(UARTx[uratn]) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(UARTx[uratn]) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(UARTx[uratn]) = (u8)(sbr & UART_BDL_SBR_MASK);

    //brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
    brfa = (((sysclk << 5) / (baud << 4)) - (sbr << 5));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(UARTx[uratn]) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(UARTx[uratn]) = temp |  UART_C4_BRFA(brfa);

    /* �����ͺͽ��� */
    UART_C2_REG(UARTx[uratn]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );

    //�����Ƿ�������պͷ����жϡ�ͨ������ UART ģ��� C2 �Ĵ�����
    //RIE �� TIE λʵ�֡����ʹ���жϣ���������ʵ���жϷ������
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_getchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
char uart_getchar (UARTn uratn)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(UARTx[uratn]);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendchar
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendchar (UARTn uratn, char *ch)
{
    u32 i = 0;

    while(++i < 0xffffff)                                         //ʱ������
    {
        if(UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK)         //��ѯ�Ƿ���ܵ�����
        {
            *ch  =   UART_D_REG(UARTx[uratn]);                    //���ܵ�8λ������
            return  1;                                            //���� 1 ��ʾ���ճɹ�
        }
    }

    *ch = 0;                                                     //���ղ�����Ӧ������˽�����
    return 0;                                                    //����0��ʾ����ʧ��
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_pendstr
*  ����˵��������ʱ��ȴ����ڽ����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ����յ����ֽ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
char uart_pendstr (UARTn uratn, char *str)
{
    u32 i = 0;
    while(uart_pendchar(uratn, str + i++));

    return (i <= 1 ? 0 : 1);
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_putchar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ����̣�printf������⺯��
*************************************************************************/
void uart_putchar (UARTn uratn, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_S1_REG(UARTx[uratn]) & UART_S1_TDRE_MASK));
    //��������
    UART_D_REG(UARTx[uratn]) = (u8)ch;
}



/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_query
*  ����˵������ѯ�Ƿ���ܵ�һ���ֽ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ�1           ���յ�һ���ֽ���
*            0           û�н��յ�
*  �޸�ʱ�䣺2012-1-20
*  ��    ע���ٷ�����
*************************************************************************/
int uart_query (UARTn uratn)
{
    return (UART_S1_REG(UARTx[uratn]) & UART_S1_RDRF_MASK);
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendN
*  ����˵�������� ����ָ��len���ֽڳ����ַ��� ������ NULL Ҳ�ᷢ�ͣ�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            buff        ���ͻ�����
*            len         ���ͳ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendN (UARTn uratn, uint8 *buff, uint16 len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        uart_putchar(uratn, buff[i]);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_sendStr
*  ����˵�������з����ַ���
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*            str         �ַ���
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_sendStr (UARTn uratn, const u8 *str)
{
    while(*str)
    {
        uart_putchar(uratn, *str++);
    }
}

/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_EN
*  ����˵���������ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_EN(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) |= UART_C2_RIE_MASK;    //����UART�����ж�
    enable_irq((uratn << 1) + 45);			        //���������ŵ�IRQ�ж�
}


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�uart_irq_DIS
*  ����˵�����ش��ڽ����ж�
*  ����˵����UARTn       ģ��ţ�UART0~UART5��
*  �������أ���
*  �޸�ʱ�䣺2012-1-20
*  ��    ע��
*************************************************************************/
void uart_irq_DIS(UARTn uratn)
{
    UART_C2_REG(UARTx[uratn]) &= ~UART_C2_RIE_MASK;   //��ֹUART�����ж�
    disable_irq((uratn << 1) + 45);			        //�ؽ������ŵ�IRQ�ж�
}


/************************************************
*  �������ƣ�SCISend_UnsignedInt
*  ����˵����SCI����UnsignedInt����
*  ����˵����UARTnΪ�ڼ������ڣ�pnumΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_UnsignedInt(UARTn uratn,unsigned int pnum)
{
    if(pnum<10)
    {  uart_putchar(uratn,pnum+'0');}
    else if((10<=pnum)&&(pnum<100))
    {
        uart_putchar(uratn,pnum/10+'0');
        uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=999)
    {
        uart_putchar(uratn,(unsigned char)(pnum/100+'0'));
        uart_putchar(uratn,(unsigned char)(pnum/10%10+'0'));
        uart_putchar(uratn,(unsigned char)(pnum%10+'0'));
    }
    else if(pnum<=9999)
    {
        uart_putchar(uratn,pnum/1000+'0');uart_putchar(uratn,pnum/100%10+'0');
        uart_putchar(uratn,pnum/10%10+'0');uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=99999)
    {
        uart_putchar(uratn,pnum/10000+48);uart_putchar(uratn,pnum/1000%10+48);
        uart_putchar(uratn,pnum/100%10+48);uart_putchar(uratn,pnum/10%10+48);
        uart_putchar(uratn,pnum%10+48);
    }
}


/************************************************
*  �������ƣ�SCISend_Int
*  ����˵����SCI����SCISend_Int����
*  ����˵����UARTnΪ�ڼ������ڣ�pnumΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_Int(UARTn uratn,int pnum)
{
    if(pnum<0)
    { pnum=-pnum; uart_putchar(uratn,'-');}
    if(pnum<10)
    {  uart_putchar(uratn,pnum+'0');}
    else if((10<=pnum)&&(pnum<100))
    {
        uart_putchar(uratn,pnum/10+'0');uart_putchar(uratn,pnum%10+'0');}
    else if(pnum<=999)
    {
        uart_putchar(uratn,(unsigned char)(pnum/100+'0'));
        uart_putchar(uratn,(unsigned char)(pnum/10%10+'0'));
        uart_putchar(uratn,(unsigned char)(pnum%10+'0'));
    }
    else if(pnum<=9999)
    {
        uart_putchar(uratn,pnum/1000+'0');uart_putchar(uratn,pnum/100%10+'0');
        uart_putchar(uratn,pnum/10%10+'0');uart_putchar(uratn,pnum%10+'0');
    }
    else if(pnum<=99999)
    {
        uart_putchar(uratn,pnum/10000+48);uart_putchar(uratn,pnum/1000%10+48);
        uart_putchar(uratn,pnum/100%10+48);uart_putchar(uratn,pnum/10%10+48);
        uart_putchar(uratn,pnum%10+48);
    }
}

/************************************************
*  �������ƣ�SCISendFloat
*  ����˵����SCI����float����
*  ����˵����UARTnΪ�ڼ������ڣ�fΪ����ֵ
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISendFloat(UARTn uratn,float f)
{				
    if(f<0)
    {
        uart_putchar(uratn,'-');
        f=(-1)*f;
    }
    uart_putchar(uratn,((int)(f))/100%10+'0');
    uart_putchar(uratn,((int)(f))/10%10+'0');
    uart_putchar(uratn,((int)(f))%10+'0');
    uart_putchar(uratn,'.');
    uart_putchar(uratn,((int)(f*10))%10+'0');
    uart_putchar(uratn,((int)(f*100))%10+'0');
    uart_putchar(uratn,' ');
}


/************************************************
*  �������ƣ�SCISend_to_PIDDebug
*  ����˵����SCI���͵�����ʾ������ѧ������λ��
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_to_PIDDebug(UARTn uratn)
{ 		
    uart_putchar(uratn,'I');	 		            	
    SCISend_Int(uratn,(int)(0));		//red,m_angle
    uart_putchar(uratn,'|');	
    SCISend_Int(uratn,(int)(0));		//blue,	
    uart_putchar(uratn,'|');
    SCISend_Int(uratn,(int)(0));			//Gyro_valuelight blue
    uart_putchar(uratn,'\r');
    uart_putchar(uratn,'\n');
}




/************************************************
*  �������ƣ�SCISend_to_PIDDebug
*  ����˵����SCI���͵�����ʾ�������Լ�����λ��
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SCISend_to_Own(UARTn uratn)
{ 	
    //1.��������
    static unsigned char i,j;
    static unsigned short int send_data[3][8]={{0},{0},{0}};//3��ͼ��ÿ�����8�����ߣ�һ��������1����
       	
    
#define speed_mode
    
    
#ifdef angle_speed_mode
    static unsigned char send_num[3]={2,2,1}; 
    send_data[0][0]=(unsigned short int)(Angle.Gyro);
    send_data[0][1]=(unsigned short int)(Direction.Gyro);
    
    send_data[1][0]=(unsigned short int)(Speed.Car);
    send_data[1][1]=(unsigned short int)(Speed.Goal);
    send_data[1][2]=(unsigned short int)(Speed.Goal - Speed.Car); 
    
    send_data[2][0]=(unsigned short int)(Speed.PWM_Integral);
    send_data[2][1]=(unsigned short int)(Speed.PWM);
    send_data[2][2]=(unsigned short int)(0);
#endif    
    
    
#ifdef angle_mode
    static unsigned char send_num[3]={2,2,1}; 
    send_data[0][0]=(unsigned short int)(Angle.a_sin*100);  
    send_data[0][1]=(unsigned short int)(Angle.a_cos*100);
    
    send_data[1][0]=(unsigned short int)(Angle.m_angle);
    send_data[1][1]=(unsigned short int)(Angle.G_angle); 
        
    send_data[2][0]=(unsigned short int)(Angle.Gyro);
#endif
    
    
#ifdef speed_mode   
    static unsigned char send_num[3]={2,2,1}; 
    send_data[0][0]=(unsigned short int)(Speed.L_100ms); 
    send_data[0][1]=(unsigned short int)(Speed.R_100ms);
    
    send_data[1][0]=(unsigned short int)(Speed.Car);
    send_data[1][1]=(unsigned short int)(Speed.Goal); 

    send_data[2][0]=(unsigned short int)(Speed.PWM_Integral);
#endif
   
    
    
#ifdef Dir_mode   
    static unsigned char send_num[3]={2,0,0}; //ÿ��ͼ����������
    send_data[0][0]=(unsigned short int)(Direction.Gyro);   //black_L[70]
    send_data[0][1]=(unsigned short int)(0);
#endif
    

#ifdef halfwidth_mode   
    static unsigned char send_num[3]={3,1,0}; //ÿ��ͼ����������
    send_data[0][0]=(unsigned short int)(halfwidth[80]); //242  
    send_data[0][1]=(unsigned short int)(halfwidth[40]); //174
    send_data[0][2]=(unsigned short int)(halfwidth[20]); //133
    
    send_data[1][0]=(unsigned short int)(Angle.G_angle); //41
#endif
    
    
    
    //3.������ͼ����ͷ,start��ǰ������ĸ
    //������������ԼΪ1/65536=0.0000152587890625��Ӧ����ô��ɣ�������ô����˼����׼ȷ
    //����Ǵ�Ů����ǿ��֢�����ڵ�Ƭ���ϼ��Ϲ��ˣ���������������ϼ��ϸ����ϸ����ͷ
    uart_putchar(uratn,'S');
    uart_putchar(uratn,'T');

    //4.��������
    for(i=0;i < 3;i++)
      for(j=0;j < send_num[i];j++)
      {
          uart_putchar(uratn,send_data[i][j]);
          uart_putchar(uratn,send_data[i][j]>>8u);
      }
}


/************************************************
*  �������ƣ�SCISend_to_PIDDebug
*  ����˵����SCI���͵�����ʾ�������Լ���λ����ccd��ʾ
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Send_Ccddata_to_PC(UARTn uratn)//����һ��ccd�����ݣ�����λ����ҲҪ����ѡ��ccddata0Ϊ�û��Լ������ccd����ȫ�ֱ���
{
	//��ͷΪ0��255
	uart_putchar(uratn,0);
	uart_putchar(uratn,255);
	for(int i=0;i<100;i++)
	    uart_putchar(uratn,(unsigned char)(centre[i]-220));
        for(int i=100;i<128;i++)
	    uart_putchar(uratn,0);
}



/************************************************
*  �������ƣ�Send_to_DataScope
*  ����˵����SCI���͵�����ʾ������DataScope 
*  ����˵����UARTnΪ�ڼ�������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
float OutData[4] = {0};
void Send_to_DataScope()
{
    delayms(5);
    OutData[0]=Speed.Car;
    OutData[1]=Speed.PWM_Integral;
    OutData[2]=Speed.Goal+500;
    OutData[3]=Speed.Goal-500;
    OutPut_Data();
}
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {
        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];
    }
    for(i=0;i<4;i++) 
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }
    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;
    for(i=0;i<10;i++)
        uart_putchar(UART3,databuf[i]);
}