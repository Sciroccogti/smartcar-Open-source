/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @brief      		小钻风(二值化摄像头)函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		MDK 5.17
 * @Target core		STM32F407ZGT6
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-10-11
 * @note
					OV7725接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA                 C1
						SCL                 C2
						场中断              E2
						像素中断            A12           
						数据口              F0-F7 
					------------------------------------ 
	
					默认分辨率              查看SEEKFREE_7725.h 文件内的宏定义 OV7725_W OV7725_H
					默认FPS                 50帧
 ********************************************************************************************************************/



#include "SEEKFREE_7725.h"


u8 image_bin[OV7725_SIZE];
u8 finish_flag = 0;
u8 OV7725_IDCode = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头内部寄存器初始化(内部使用，用户无需调用)
//  @param      NULL
//  @return     u8			返回0则出错，返回1则成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
u8 ov7725_reg_init(void)
{
    
    SCCB_WR_Reg ( OV7725_COM7, 0x80 );	//复位摄像头
    delay_ms(50);
	OV7725_IDCode = SCCB_RD_Reg( OV7725_VER);
    if( OV7725_IDCode != OV7725_ID )    return 0;			//校验摄像头ID号


    if(OV7725_IDCode == OV7725_ID)
    {
        //ID号确认无误   然后配置寄存器
        SCCB_WR_Reg(OV7725_COM4         , 0xC1);  
        SCCB_WR_Reg(OV7725_CLKRC        , 0x01);
        SCCB_WR_Reg(OV7725_COM2         , 0x03);
        SCCB_WR_Reg(OV7725_COM3         , 0xD0);
        SCCB_WR_Reg(OV7725_COM7         , 0x40);
        SCCB_WR_Reg(OV7725_HSTART       , 0x3F);
        SCCB_WR_Reg(OV7725_HSIZE        , 0x50);
        SCCB_WR_Reg(OV7725_VSTRT        , 0x03);
        SCCB_WR_Reg(OV7725_VSIZE        , 0x78);
        SCCB_WR_Reg(OV7725_HREF         , 0x00);
        SCCB_WR_Reg(OV7725_SCAL0        , 0x0A);
        SCCB_WR_Reg(OV7725_AWB_Ctrl0    , 0xE0);
        SCCB_WR_Reg(OV7725_DSPAuto      , 0xff);
        SCCB_WR_Reg(OV7725_DSP_Ctrl2    , 0x0C);
        SCCB_WR_Reg(OV7725_DSP_Ctrl3    , 0x00);
        SCCB_WR_Reg(OV7725_DSP_Ctrl4    , 0x00);
      
      
        if(OV7725_W == 80)              SCCB_WR_Reg(OV7725_HOutSize    , 0x14);
        else if(OV7725_W == 160)        SCCB_WR_Reg(OV7725_HOutSize    , 0x28);
        else if(OV7725_W == 240)        SCCB_WR_Reg(OV7725_HOutSize    , 0x3c);
        else if(OV7725_W == 320)        SCCB_WR_Reg(OV7725_HOutSize    , 0x50);
		
        if(OV7725_H == 60)              SCCB_WR_Reg(OV7725_VOutSize    , 0x1E);
        else if(OV7725_H == 120)        SCCB_WR_Reg(OV7725_VOutSize    , 0x3c);
        else if(OV7725_H == 180)        SCCB_WR_Reg(OV7725_VOutSize    , 0x5a);
        else if(OV7725_H == 240)        SCCB_WR_Reg(OV7725_VOutSize    , 0x78);
      
      
        SCCB_WR_Reg(OV7725_REG28        , 0x01);
        SCCB_WR_Reg(OV7725_EXHCH        , 0x10);
		SCCB_WR_Reg(OV7725_EXHCL        , 0x1F);
        SCCB_WR_Reg(OV7725_GAM1         , 0x0c);
        SCCB_WR_Reg(OV7725_GAM2         , 0x16);
        SCCB_WR_Reg(OV7725_GAM3         , 0x2a);
        SCCB_WR_Reg(OV7725_GAM4         , 0x4e);
        SCCB_WR_Reg(OV7725_GAM5         , 0x61);
        SCCB_WR_Reg(OV7725_GAM6         , 0x6f);
        SCCB_WR_Reg(OV7725_GAM7         , 0x7b);
        SCCB_WR_Reg(OV7725_GAM8         , 0x86);
        SCCB_WR_Reg(OV7725_GAM9         , 0x8e);
        SCCB_WR_Reg(OV7725_GAM10        , 0x97);
        SCCB_WR_Reg(OV7725_GAM11        , 0xa4);
        SCCB_WR_Reg(OV7725_GAM12        , 0xaf);
        SCCB_WR_Reg(OV7725_GAM13        , 0xc5);
        SCCB_WR_Reg(OV7725_GAM14        , 0xd7);
        SCCB_WR_Reg(OV7725_GAM15        , 0xe8);
        SCCB_WR_Reg(OV7725_SLOP         , 0x20);
        SCCB_WR_Reg(OV7725_LC_RADI      , 0x00);
        SCCB_WR_Reg(OV7725_LC_COEF      , 0x13);
        SCCB_WR_Reg(OV7725_LC_XC        , 0x08);
        SCCB_WR_Reg(OV7725_LC_COEFB     , 0x14);
        SCCB_WR_Reg(OV7725_LC_COEFR     , 0x17);
        SCCB_WR_Reg(OV7725_LC_CTR       , 0x05);
        SCCB_WR_Reg(OV7725_BDBase       , 0x99);
        SCCB_WR_Reg(OV7725_BDMStep      , 0x03);
        SCCB_WR_Reg(OV7725_SDE          , 0x04);
        SCCB_WR_Reg(OV7725_BRIGHT       , 0x00);
        SCCB_WR_Reg(OV7725_CNST         , 0x36);
        SCCB_WR_Reg(OV7725_SIGN         , 0x06);
        SCCB_WR_Reg(OV7725_UVADJ0       , 0x11);
        SCCB_WR_Reg(OV7725_UVADJ1       , 0x02);
        return 1;
    }
    else        return 0;//错误
    
}


void dma_init(void)
{
    //DMA配置
    DMA_InitTypeDef  DMA_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA,GPIOE时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //KEY0 KEY1 KEY2对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE2,3,4

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//等待DMA2_Stream1可配置
	
    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_6;                    //通道1 DCMI通道 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&GPIOF->IDR;      //外设地址为:DCMI->DR
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)image_bin;                    //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;           //外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = OV7725_SIZE;                   //数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;           //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据宽度:32位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;   //存储器数据宽度 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                   // 使用循环模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;               //高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;             //FIFO模式        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;     //使用全FIFO 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       //外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//存储器突发单次传输

    DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
}

void time_trrige_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA12
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_TIM1);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //使能TIM1时钟
    TIM_Cmd(TIM1,DISABLE);
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period =1;  
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;   
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);  
    TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Reset); //复位模式

    TIM_ETRClockMode1Config(TIM1,  TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted, 0x0);
    TIM_SelectInputTrigger(TIM1,TIM_TS_ETRF);
}

void exit_init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

    //场中断
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //输出模式，采用输入模式容易误触发，而输出模式则不会，采用输出模式也可以触发定时器的，因此不必担心
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  //下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);          //初始化GPIOE2
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
    /* 配置EXTI_Line2 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE2
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE2
    EXTI_Init(&EXTI_InitStructure);//配置
  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      dma 定时器 外部中断相关初始化
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_port_init(void)
{
    //采集思路如下，由于stm32不能直接采用io触发DMA，因为我们将定时器配置为外部触发模式，然后由定时器触发DMA工作，
    //这样就间接的变成了IO触发DMA。一个外部脉冲DMA搬运一次数据。
    //由于时序的原因小钻风摄像头无法使用DCMI进行采集
    dma_init();         //DMA初始化
    time_trrige_init(); //定时器配置为外部触发模式
    exit_init();        //外部中断初始化


}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头初始化(调用之后设置好相关中断函数即可采集图像)
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
u8 ov7725_init(void)
{
	SCCB_Init();
	ov7725_reg_init();                                          //摄像头寄存器配置
    ov7725_port_init();                                         //摄像头中断引脚及DMA配置
    return 0;
}


//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
    if(SET == EXTI_GetITStatus(EXTI_Line2))
    {
        DMA_Cmd(DMA2_Stream0, ENABLE);//开启DMA2,Stream1
        TIM_DMACmd(TIM1,TIM_DMA_Trigger,ENABLE);
        TIM_ClearITPendingBit(TIM1,TIM_IT_Trigger);
    }
    EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE8上的中断标志位  
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头数据解压函数
//  @param      *data1				源地址
//  @param      *data2				目的地址
//  @return     void
//  @since      v1.0
//  Sample usage:					Image_Decompression(da1,dat2[0]);//将一维数组dat1的内容解压到二维数组dat2里.
//-------------------------------------------------------------------------------------------------------------------
void Image_Decompression(u8 *data1,u8 *data2)
{
    u8  temp[2] = {0,255};
    u16 lenth = OV7725_SIZE;
    u8  i = 8;

    while(lenth--)
    {
        i = 8;
        while(i--)
        {
            *data2++ = temp[(*data1 >> i) & 0x01];
        }
        data1++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风摄像头未解压图像发送至上位机查看图像
//  @param      *imgaddr			压缩图像数据地址
//  @param      *imgsize			图像大小(直接填写OV7725_SIZE)
//  @return     void
//  @since      v1.0
//  Sample usage:					调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_7725(void *imgaddr, u32 imgsize)
{
    u32 i;
    put_char(USART1,0x00);
    put_char(USART1,0xff);
    put_char(USART1,0x01);
    put_char(USART1,0x01);
    for(i=0; i<(imgsize); i++)
    {
        put_char(USART1,((u8 *)(imgaddr))[i]);
    }
}
