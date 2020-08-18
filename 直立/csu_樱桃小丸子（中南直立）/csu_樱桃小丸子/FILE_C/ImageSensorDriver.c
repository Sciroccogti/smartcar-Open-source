#include "GeneralDeclare.h"
#include "ImageSensorDriver.h"
#include "ProjectMath.h"
#include "TimerEventDriver.h"
#include "Terminator.h"
#include "SystemControl.h"


/******************************************************/
/*Author  :    ���ϴ�ѧ�ƾ���                         */
/*Date    ��   2016/12/22                             */
/*Describe�� ����ͷ����                               */
/*Version ��  1.1                                     */
/*note    ��  ��δ���MT9V032�Ĵ������ã�Ŀǰ��       */
/*             ��0V7725������M60DNZ512                */
/******************************************************/

Registermaptypedef OV7725Registermap[] = {
	//�Ĵ������Ĵ���ֵ
   {OV7725_COM4         , 0xC1},               // 1100 0001        6x  ȫ���ع����֡��150HZ
   {OV7725_CLKRC        , 0x00},
   {OV7725_COM2         , 0x03},
   {OV7725_COM3         , 0xD0},
   {OV7725_COM7         , 0x40},
   {OV7725_COM8         , 0xCF},               //����Ӧ�ع����   1100 1111  ��ϸ��DATASHEET
   {OV7725_COM5         , 0x79},                //16:2 �Զ��ع��������
   //{OV7725_AEC         , 0xD0},               //���ع�ֵ�趨15:0
   {OV7725_HSTART       , 0x3F},
   {OV7725_HSIZE        , 0x50},
   {OV7725_VSTRT        , 0x03},
   {OV7725_VSIZE        , 0x78},
   {OV7725_HREF         , 0x00},
   {OV7725_SCAL0        , 0x0A},
   {OV7725_AWB_Ctrl0    , 0xE0},
   {OV7725_DSPAuto      , 0xff},
   {OV7725_DSP_Ctrl2    , 0x0C},
   {OV7725_DSP_Ctrl3    , 0x00},
   {OV7725_DSP_Ctrl4    , 0x00},
   {OV7725_HOutSize     , 0x14},
   {OV7725_VOutSize     , 0x1E},
   {OV7725_EXHCH        , 0x00},
   {OV7725_GAM1         , 0x0c},
   {OV7725_GAM2         , 0x16},
   {OV7725_GAM3         , 0x2a},
   {OV7725_GAM4         , 0x4e},
   {OV7725_GAM5         , 0x61},
   {OV7725_GAM6         , 0x6f},
   {OV7725_GAM7         , 0x7b},
   {OV7725_GAM8         , 0x86},
   {OV7725_GAM9         , 0x8e},
   {OV7725_GAM10        , 0x97},
   {OV7725_GAM11        , 0xa4},
   {OV7725_GAM12        , 0xaf},
   {OV7725_GAM13        , 0xc5},
   {OV7725_GAM14        , 0xd7},
   {OV7725_GAM15        , 0xe8},
   {OV7725_SLOP         , 0x20},
   {OV7725_LC_RADI      , 0x00},
   {OV7725_LC_COEF      , 0x13},
   {OV7725_LC_XC        , 0x08},
   {OV7725_LC_COEFB     , 0x14},
   {OV7725_LC_COEFR     , 0x17},
   {OV7725_LC_CTR       , 0x05},
   {OV7725_BDBase       , 0x99},
   {OV7725_BDMStep      , 0x03},
   {OV7725_SDE          , 0x04},
   {OV7725_BRIGHT       , 0x00},
   {OV7725_CNST         , 0x49},     //��ֵ
   {OV7725_SIGN         , 0x06},
   {OV7725_UVADJ0       , 0x11},
   {OV7725_UVADJ1       , 0x02},
};

#define SCCB_DELAY  SCCB_delay(900);

void SCCB_init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORT_ISFR_REG(PORTB_BASE_PTR) = (1 << SCCB_SDA_pin);
	PORT_ISFR_REG(PORTB_BASE_PTR) = (1 << SCCB_SCL_pin);
	PORTB_PCR(SCCB_SDA_pin) = ALT1 | ODO | PULLUP;//������©����GPIO
	PORTB_PCR(SCCB_SCL_pin) = ALT1 | ODO | PULLUP;
	SCCB_SDA_DDR_OUT;                     //���
	SCCB_SCL_DDR_OUT;
}

static void SCCB_delay(volatile uint16 t)
{
	while (t--);
}

static uint8 SCCB_start()    //��ʼ�ź�
{
	SCCB_SDA_H;
	SCCB_SCL_H;
	SCCB_SDA_DDR_IN;
	if (!SCCB_SDA_IN)         //æ���
	{
		SCCB_SDA_DDR_OUT;
		return 'F';
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SDA_L;
	SCCB_DELAY;
	SCCB_SCL_L;

	if (SCCB_SDA_IN)         //æ���
	{
		SCCB_SDA_DDR_OUT;
		return 'F';
	}
	return 'T';
}

static uint8 SCCB_stop()   //ֹͣ�ź�
{
	SCCB_SCL_L;
	SCCB_SDA_L;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	SCCB_SDA_H;
	SCCB_DELAY;
	return 'T';
}

static uint8 SCCB_waitack() //�ȴ�Ӧ��
{
	SCCB_SCL_L;
	SCCB_SDA_DDR_IN;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	if (SCCB_SDA_IN) // ����ⲿ
	{
		SCCB_SDA_DDR_OUT;
		SCCB_SCL_L;
		return 'F';
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SCL_L;
	return 'T';
}

static void SCCB_ack()     //Ӧ��
{
	SCCB_SCL_L;
	SCCB_DELAY;
	SCCB_SDA_L;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	SCCB_SCL_L;
	SCCB_DELAY;
}

static void SCCB_noack()    //��Ӧ��
{
	SCCB_SCL_L;
	SCCB_DELAY;
	SCCB_SDA_H;
	SCCB_DELAY;
	SCCB_SCL_H;
	SCCB_DELAY;
	SCCB_SCL_L;
	SCCB_DELAY;
}

static void SCCB_writebyte(uint8 data)
{
	uint8 i = 8;
	while (i--)
	{
		if (data & 0x80)          //SDA �������
		{
			SCCB_SDA_H;
		}
		else
		{
			SCCB_SDA_L;
		}
		data <<= 1;
		SCCB_DELAY;
		SCCB_SCL_H;
		SCCB_DELAY;
		SCCB_SCL_L;
	}
}

static uint8 SCCB_readbyte()
{
	uint8 data = 0;
	uint8 i = 8;
	while (i--)
	{
		data <<= 1;
		SCCB_SCL_L;
		SCCB_DELAY;
		SCCB_SCL_H;
		SCCB_DELAY;
		if (SCCB_SDA_IN)
		{
			data |= 0x01;
		}
		else
		{
			data &= ~0x01;
		}
	}
	SCCB_SDA_DDR_OUT;
	SCCB_SCL_L;
	return data;
}

uint8 SCCB_sendbyte(uint8 ADDR, uint8 OFFSET, uint8 data)
{
	//    static uint8 count;
	//    uint8 Sign = 'T';
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);   //for write
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_writebyte(data);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return  'F';
	}
	SCCB_stop();
	//    while('F' == Sign && count < 10)
	//    {
	//      SCCB_sendbyte(ADDR, OFFSET, data);   //���ǲ�Ҫ�ݹ�ʵ�֣����ڴ˳�ʼ��ֻ�����һ�Σ����ֻ��Ҫ��дֵʱ�鿴����ֵ����
	//      count++;
	//    }
	return 'T';
}

uint8 SCCB_receivedata(uint8 ADDR, uint8 OFFSET, uint8 *data, uint16 length)
{
	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 0);   //for write
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_writebyte(OFFSET);
	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}
	SCCB_stop();              //ָ��Ŀ��Ĵ���

	if (SCCB_start() == 'F')
	{
		return 'F';
	}
	SCCB_writebyte((ADDR << 1) | 1);  //for read

	if (SCCB_waitack() == 'F')
	{
		SCCB_stop();
		return 'F';
	}

	while (length)
	{
		*data = SCCB_readbyte();
		if (length == 1)
		{
			SCCB_noack();
		}
		else
		{
			SCCB_ack();
		}
		data++;
		length--;
	}
	SCCB_stop();
	return 'T';
}

static uint8 ImageSensorRegister_init()
{
	int wait = 0;
	// uint8 ID = 0;
	SCCB_init();

	wait = 10;
#if (USINGCAMERA == 0)
	while ('F' == SCCB_sendbyte(OV7725ADDR, OV7725_COM7, 0x80) && wait > 0)  //Restart
	{
		wait--;
	}
	if (wait == 0)return 'F';

	//if('F' == SCCB_receivedata(OV7725ADDR, OV7725_VER, &ID, 1))
	//{
   //   return 'F';
   // }
   // if(ID == OV7725_ID)
   // {
	for (uint8 n = 0; n < ARRAY_SIZE(OV7725Registermap); n++)
	{
		if ('F' == SCCB_sendbyte(OV7725ADDR, OV7725Registermap[n].addr, OV7725Registermap[n].val))
		{
			return 'F';
		}
		/* if(SCCB_receivedata(OV7725ADDR, OV7725Registermap[n].addr, &val, 1) == 'T')
		 {
		   if(val != OV7725Registermap[n].val)
			 return 'F';
		 }
		 else
		   return 'F';*/
	}
	if ('F' == SCCB_sendbyte(OV7725ADDR, OV7725_CNST, ImageInfo.threshold))
	{
		return 'F';
	}
	// }
	// else
	//   return 'F';
#elif (USINGCAMERA == 1)
	return 'F';
#else
	return 'F';
#endif

	return 'T';
}


volatile uint8 BuffNum = 0;
uint8 *DMAaddr;
void ImagePCLK_IRQHandler(void);
void ImageVSY_IRQHandler(void);
void ImageSensor_init(uint8 *imgaddr, uint16 imgsize)
{
	int wait = 10;
	DMAaddr = imgaddr;
	while ('F' == ImageSensorRegister_init() && wait > 0)
	{
		wait--;
	}
	if (wait == 0)                  //�����ɣ�����ͷ���������˻���ʲô��<<<<<<<<
	{
		NVIC_SystemReset();  //System Restart
	}

	/**********************************************/
	/*�˿ڳ�ʼ��*/
	/**********************************************/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;            //����B�˿�ʱ��
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;            //����A�˿�ʱ��
	for (uint8 i = 0; i < 8; i++)         //ʹ�ô�˿ڵ���λ8���ܽţ���ʵ���Ը�Ϊ����������˸�
	{
		PORT_ISFR_REG(ImgDATA_port) = (1 << i);
		ImgDATA_port->PCR[i] = PORT_PCR_MUX(1) | (PULLDOWN);       //���ݶ˿�����������Ĭ�Ͼ�Ϊ�͵�ƽ 

		ImgDATA_GPIO_port->PDIR &= ~(1 << i);
	}
	PORT_ISFR_REG(ImgPCLK_port) = (1 << ImgPCLK_pin);
	ImgPCLK_port->PCR[ImgPCLK_pin] = PORT_PCR_MUX(1) | (DMA_FALLING) | (PULLUP);//����
	ImgPCLK_GPIO_port->PDIR &= ~(1 << ImgPCLK_pin);                      //����Ϊ����
	PORT_ISFR_REG(ImgVSY_port) = (1 << ImgVSY_pin);
	ImgVSY_port->PCR[ImgVSY_pin] = PORT_PCR_MUX(1) | (IRQ_RISING) | (PULLDOWN);//����
	ImgVSY_GPIO_port->PDIR &= ~(1 << ImgVSY_pin);         //�½��������ض����ԣ�Ƶ��150HZ   (0x0A << PORT_PCR_IRQC_SHIFT)  //�½���
														 //(0x09 << PORT_PCR_IRQC_SHIFT)    ������
  /**********************************************/

	//��16·DMAͨ��ʹ��0��ͨ����DMA0ͨ�������ȼ����

	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;              //����DMAʱ��
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;           //DMA��·����ʱ��
	DMAMUX_CHCFG0 = 0x00;
	DMAMUX_CHCFG0 &= ~DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG0 &= ~DMAMUX_CHCFG_TRIG_MASK;



	DMA_BASE_PTR->TCD[0].SADDR = (uint32)((void *)&(ImgDATA_GPIO_port->PDIR));  //ѡ��Դ��ַ
	DMA_BASE_PTR->TCD[0].SOFF = 0x00u;
	DMA_BASE_PTR->TCD[0].DADDR = (uint32)((void *)DMAaddr); //����Ŀ���ַ
	DMA_BASE_PTR->TCD[0].DOFF = 1;              //Ŀ�ĵ�ַƫ��1���ֽ�
	DMA_BASE_PTR->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(imgsize);
	DMA_BASE_PTR->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(imgsize);
	DMA_BASE_PTR->TCD[0].CSR = DMA_CSR_BWC(3) | DMA_CSR_DREQ_MASK | DMA_CSR_INTMAJOR_MASK;   //��ʱ ���ͺ�����жϲ�ֹͣDMA����
	DMA_BASE_PTR->TCD[0].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(1);          //ÿ�δ���1�ֽ�
	DMA_BASE_PTR->TCD[0].ATTR = DMA_ATTR_SMOD(0x0) | DMA_ATTR_SMOD(0x0) | DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);  //���ݸ�ʽ����
	DMA_BASE_PTR->TCD[0].SLAST = 0;              //�����Դͷ��ƫ��
	DMA_BASE_PTR->TCD[0].DLAST_SGA = 0;          //�����Ŀ�ĵ�ַƫ��������ָ��
	DMA_CR &= ~DMA_CR_EMLM_MASK;


	DMAMUX_CHCFG0 |= DMAMUX_CHCFG_SOURCE(49);             //ѡ�񴥷�ԴPORTB
	DMAMUX_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;

	DMA_ERQ &= ~(DMA_ERQ_ERQ0_MASK << 0);
	DMA_INT |= (DMA_INT_INT0_MASK << 0);       //���жϱ�־λ

	set_vector_handler(DMA0_VECTORn, ImagePCLK_IRQHandler);
	set_vector_handler(PORTA_VECTORn, ImageVSY_IRQHandler);
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_SetPriority(PORTA_IRQn, 1);
	NVIC_EnableIRQ(DMA0_IRQn);
	NVIC_DisableIRQ(PORTA_IRQn);               //���ϴ�����BUG���ɼ�ʱ����޸�
}


extern uint8 ImageSensorTestChoose;
extern volatile uint8 ImageSensorOK;
void ImageVSY_IRQHandler(void)
{
	if (ImgVSY_port->ISFR &  (1 << ImgVSY_pin))
	{
		ImgVSY_port->ISFR = 1 << ImgVSY_pin;
		if (1 == ((ImgVSY_GPIO_port->PDIR >> ImgVSY_pin) & 1))
		{
			NVIC_DisableIRQ(PORTA_IRQn);   ///////       
			NVIC_SetPriority(DMA0_IRQn, 1);   ////�ж���Ϊ��߼�
			DMA_INT |= (DMA_INT_INT0_MASK << 0);
			ImgPCLK_port->ISFR |= 1 << ImgPCLK_pin;
			DMA_ERQ |= (DMA_ERQ_ERQ0_MASK << 0);
			DMA_BASE_PTR->TCD[0].DADDR = (uint32)DMAaddr;
			ImgPCLK_port->ISFR |= 1 << ImgPCLK_pin;
		}
	}
}


volatile uint8 IsSendPicOK = 'F';
uint8 SendPicBuff[600] = { 0 };
extern volatile uint8  IsWifiSendOK;
void ImagePCLK_IRQHandler(void)
{  //////�ɼ�������Ҫ6.33ms                  ////DMA�жϴμ�
	DMA_INT |= (DMA_INT_INT0_MASK << 0);


	NVIC_SetPriority(DMA0_IRQn, 2);            ////�жϽ���
	UnZip((uint8 *)PicForNow, DMAaddr, 600);  /////��ѹͼ������
	Image_MainDeal();


	if (SystemSettings.Wifi_Init == 'T' && IsWifiSendOK == 'F')
	{
		IsWifiSendOK = 'T';
	}
	ImgVSY_port->ISFR = 1 << ImgVSY_pin;

	if (ImageSensorTestChoose == 0)
	{
		NVIC_EnableIRQ(PORTA_IRQn);
	}
	else
	{
		ImageSensorOK = 1;
	}
}


