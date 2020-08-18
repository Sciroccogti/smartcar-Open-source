/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */
#include    "MK60_it.h"
#include    "common.h"
#include    "include.h"

/*********************************�жϷ�����********************************/
/*!
*  @brief      PIT0�жϷ�����
*  @since      v5.0
*/
void PIT0_IRQHandler()	// 2ms��һ���ж�
{	
	static uchar Time_10ms = 0;
	static uchar Time_50ms = 0;
	static uchar Time_100ms = 0;
	static uchar Time_200ms = 0;
	
	Time_10ms++;
	Time_50ms++;
	Time_100ms++;
	Time_200ms++;
	
	if (Annulus_Delay)
	{
		Annulus_Delay--;
	}
	
	Ang_Velocity_Flag = 1;	// ���ٶ�2ms��ȡһ��
	
	if (Time_10ms == 5)
	{
		Time_10ms = 0;
		Angle_Flag = 1;		// ��̬�Ƕ�10ms����һ��	
	}
	
	if (Time_50ms == 25)
	{
		Time_50ms = 0;
		
//		Data_Send_Scope();//������Data_Scope��λ��������
//		Data_Send_Status();//������������λ��������
	}
	
	if (Time_100ms == 50)
	{
		Time_100ms = 0;
		Speed_Flag = 1;	// �ٶ�100ms����һ��
	}
	
	if (Time_200ms == 100)
	{
		Time_200ms = 0;
		
		Send_OK = 1;	// ͼ��200ms����һ��
		My_DEBUG();
	}
	
	Balance_Control();	// ����ƽ�����
	PIT_Flag_Clear(PIT0);//�����ʱ���жϱ�־λ
}


/*!
*  @brief      PIT1�жϷ�����
*  @since      v5.0
*/
void PIT1_IRQHandler()
{
	static uchar Time = 0;
	
	PIT_Flag_Clear(PIT1);//�����ʱ���жϱ�־λ
	
	Time++;
	if (Time == 5)			//��ʱ5s
	{
		Time = 0;
		
		Run_Flag = 1;		//����
		led(LED_MAX,LED_ON);//��LEDָʾ��
		
		pit_close(PIT1);	//�رն�ʱ��1
	}
}


/*!
*  @brief      UART4�жϷ�����
*  @since      v5.0
*/
void UART4_IRQHandler(void)
{
	char Buff;
	UARTn_e uratn = UART4;
	
//	if (uart_querychar(UART4, &Buff))
	if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) //�������ݼĴ�����
	{
		uart_getchar(uratn, &Buff);
		switch (Buff)
		{
			case 'q': 	Speed_Set = 30; break;			// ǰ��
			case 'w': 	Speed_Set = 0;	
						Tar_Ang_Vel.Z = 0;	break;		// ͣ��
			case 's':	Tar_Ang_Vel.Z = 0;	break;		// ֹͣת��
			case 'a':	Tar_Ang_Vel.Z = -7000;	break;	// ����ת
			case 'd':	Tar_Ang_Vel.Z = 7000;	break;	// ����ת
			case 'z':	Tar_Ang_Vel.Z = -3000;	break;	// С��ת
			case 'c':	Tar_Ang_Vel.Z = 3000;	break;	// С��ת
			case 'x':	Speed_Set = -20;	break;		// ����
			default:	break;
		}
	}
}

/*!
*  @brief      PORTA�жϷ�����
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8  n;    //���ź�
  uint32 flag;
  
  //while(!PORTA_ISFR);
  flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //���жϱ�־λ
  
  n = 29;                                             //���ж�
  if(flag & (1 << n))                                 //PTA29�����ж�
  {
    camera_vsync();
  }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
  n = 28;
  if(flag & (1 << n))                                 //PTA28�����ж�
  {
    camera_href();
  }
#endif
  
}


/*!
*  @brief      PORTE�жϷ�����
*  @since      v5.0
*/
void PORTE_IRQHandler()
{
  uint8  n;    //���ź�
  uint32 flag;
  
  flag = PORTE_ISFR;
  PORTE_ISFR  = ~0;                                   //���жϱ�־λ
  
  n = 27;
  if(flag & (1 << n))                                 //PTE27�����ж�
  {
    nrf_handler();
  }
}


/*!
*  @brief      DMA0�жϷ�����
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}


/*!
*  @brief      ���벶���жϷ�����
*  @since      v5.0
*/
void FTM2_INPUT_IRQHandler(void)
{
    uint8 s = FTM2_STATUS;          //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;
	int32 temp;

    FTM2_STATUS = 0x00;				//���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        temp = ftm_input_get(FTM2, FTM_CH0); //����
		if (gpio_get(PTA10))				 //������
		{
			ftm_input_clean(FTM2);            //�� ����������ֵ
		}
		else	//�½���
		{
			RC_Get= 1000000/(bus_clk_khz*1000/(1<<FTM_PS_64)/temp);	//����ߵ�ƽʱ�伴ͨ��0��ֵ
		}       
    }
}

/******************************�жϷ���������***********8*******************/



