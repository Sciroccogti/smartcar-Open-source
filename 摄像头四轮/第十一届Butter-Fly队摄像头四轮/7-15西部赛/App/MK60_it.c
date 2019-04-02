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
 *  @brief      UART4�жϷ�����
 *  @since      v5.0
 */
void uart4_handler(void)
{
    char str[2];

    if(uart_querychar (UART4, str) != 0)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        if (str[0] == 1)
		{
			Run_Flag = 1;
		}
		else if (str[0] == 2)
		{
			Run_Flag = 0;
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
*  @brief      PIT0�жϷ�����
*  @since      v5.0
*/
void PIT0_IRQHandler()
{	
	static uchar Time_100ms = 0;
	
	Time_100ms++;
	if (Time_100ms == 10)
	{
		Time_100ms = 0;
		Send_OK = 1;
	}
	
	speed_measure();	//���ټ�ͣ��	�����ڼ���ʱ����
   
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
	if (Time == 3)			//��ʱ5s
	{
		Time = 0;
		
		Run_Flag = 1;		//����
		led(LED_MAX,LED_ON);//��LEDָʾ��
		
		pit_close(PIT1);	//�رն�ʱ��1
	}
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
/*			if (RC_Get > 1300)
			{
				Run_Flag = 1;
			}
			else
			{
				Run_Flag = 0;
			}*/
		}       
    }
}

/******************************�жϷ���������***********8*******************/



