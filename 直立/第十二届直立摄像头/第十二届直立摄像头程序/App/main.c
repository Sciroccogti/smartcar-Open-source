/*!
 *		��������ѧԺֱ������ͷ
 *
 * @file       main.c
 * @brief      ������
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */

#include "common.h"
#include "include.h"

/**
 * @file		С��Ӳ����ʼ��
 * @author		��������ѧԺ
 * @date		2017   
 */
void car_init(void)
{
  //�����ж����ȼ�
  	NVIC_SetPriorityGrouping(4);	//�������ȼ�����,4bit��ռ���ȼ�
    set_irq_priority(DMA0_IRQn,2);
    set_irq_priority(PORTA_IRQn,2);
    set_irq_priority(PORTC_IRQn,1);
    set_irq_priority(PIT0_IRQn,3);
    
  //�������ֳ�ʼ��
  	led_init(LED0);				//LED��ʼ��
	led(LED0, LED_ON);
	gpio_init (BUZZ, GPO,0);	//��ʼ��������
    LCD_Init();					//OLED��ʼ��
    flash_init();				//flash��ʼ��
	uart_init (UART2, 115200);	//����2��ʼ����������
	lptmr_delay_ms(1000);		//��ʱ1s

  //���뿪�ػ��ýӿڳ�ʼ��(����Ϊ���뿪�ػ��ýӿ�)
	gpio_init (K1, GPO,0);	//K1	���ýӿڣ�K1-4�����ڲ��뿪�����ΪGPI;
	gpio_init (K2, GPO,0);	//K2	���ýӿ�
	gpio_init (K3, GPO,0);	//K3	���ýӿ�
	gpio_init (K4, GPO,0);	//K4	���ýӿ�
	
  //��������ⲿ�жϳ�ʼ��
    infrared_init();
    set_vector_handler(PORTC_VECTORn,Infrared_Handler);
    enable_irq(PORTC_IRQn);
	
  //���Ƴ�ʼ��
	UpstandInit();
	MotorInit();
	
  //PIT�жϳ�ʼ��
    pit_init_ms(PIT0, 5);
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler); 
    enable_irq (PIT0_IRQn);
	
  //����ͷ��ʼ�� 
	camera_init(p_imgbuff1);    //����ͷ�������ú�ͼ���ַ����
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);     
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);
}

/**
 * @file		������
 * @author		��������ѧԺ
 * @date		2017-3 
 */
void main(void)
{  
	DisableInterrupts; 				  //��ֹȫ���ж�
	param_in();			
	car_init();			
	EnableInterrupts;				  //ʹ��ȫ���ж�
	ov7725_eagle_img_flag = IMG_START;//��ʼ�ɼ�ͼ��
	enable_irq(PORTA_IRQn);           //����PTA���ж�
  while(1)
  {

	//����Ϊ�����������Բ�������
     if(debug_flag!=0)					//������²˵���
      {
        UI_debug();						//�˵����ν���
        debug_flag=0;
        set_param(&parameter);			//��������flash��
        oled_fill(0x00);
        oled_display_16x8str(16,0,"OK");//������ϣ���ʾok
      }
	 //����Ϊ�����������Բ������� 
	 else 
	   Draw_BMP(0,0,127,7,bmp_buff);	//oled��ʾͼ�񣨵�����ʱoled��ʾ���ν��棬������ʾͼ��
	 
	 //vcan_sendimg(img, CAMERA_W * CAMERA_H);//ɽ����λ����ʾͼ��
  }
}



