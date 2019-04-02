#include "usart_file.h"
#include "control.h"
#include "img_array.h"
#include "path.h"
#include "usart_file.h"
#include  "info_deal_save.h"
#include "my_UI.h"
#include "direction.h"
#include "speed_new.h"

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint32 total_time=0;
//uint8 img[CAMERA_H][CAMERA_W];

//�������� z120 
void sendimg(void *imgaddr, uint32 imgsize);
void PORTA_IRQHandler();
void DMA0_IRQHandler();
uint8 my_debug_flag=0;
void my_delay(uint32_t z)
{
    while(z--);
}
void draw_mark_line()
{
    uint16 x=0,y=0;
    Site_t site;
    for(y=0;y<IMG_H;y+=10)
    {
        for(x=0;x<IMG_W;x++)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
    for(y=0;y<IMG_H;y++)
    {
        for(x=0;x<IMG_W;x+=10)
        {
            site.x=x;
            site.y=y;
            LCD_point(site,RED);
        }
    }
}

//void motor_dir_init()
//{/************************************************************************
//  ���õ������
//  */////////////////////////////////////////
//  gpio_init(PTD6,GPI,0);//��ʼ�͵�ƽ
//  port_init_NoALT(PTD6,PULLUP);
//  
////  gpio_set (PTD6, 1);//�������Ÿߵ�ƽ
//
//}
void init_fun()
{
    key_init(KEY_MAX);//������ʼ��  
    duoji_init();//���pwm��ʼ��
    LCD_init();  //������ʼ��    
    //init_Par_from_FLASH();//�ڲ�flash��ʼ�������ڴ���������ڵ��εĲ���
    usart_init();//���ڳ�ʼ��
    camera_init(imgbuff);//�����ʼ��
   //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
}

void check_start_key()
{
    if(key_get(KEY_U)==0||key_get(KEY_R)==0||key_get(KEY_START)==0||key_get(KEY_A)==0)
    {
        DELAY_MS(1000);
        run_start_flag=~run_start_flag;
        total_time=0;
    }
    else if(key_get(KEY_D)==0||key_get(KEY_L)==0)
    {
        DELAY_MS(500);
        if(key_get(KEY_D)==0||key_get(KEY_L)==0)
        {
            if(my_debug_flag) 
              my_debug_flag=0;
            else my_debug_flag=1;
        }
    }
}
/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ������ͷ LCD ---------------------����ʵ��
 */

/*
��ʮ����  �������ͷ
*/
    



void  main(void) 
{

 //����������****һ��Ҫ�ȶ�my_cfg.h�е�ע��**��***������������
    run_start_flag=0;
    init_fun();//���ֳ�ʼ��  (���� ��� ���� flash ����ͷ �ж�����) 
    init_ctrl();//��ʼ���ٶȿ��ƣ��ж� �����
    renew_UI();//�����ʼ������ͨ���������� ��ֱ�����������   
    MOTOR_init();//��ʼ�����
    PID_init(); 
    //DELAY_MS(2000);//��ʱ���뷢��
    run_start_flag=1;//��ʼ�ܵı�־λ
    total_time=40;//��¼���д����������߼��ʱ�õ���������һ��ʱ���ڼ�������� 
    
    // change_angle(6600);//�������λ�ã����ڲ��ԣ�
    while(1)
    {      
        //cmd_deal();        //���ڴ����ڷ��������� ������������         
      if(img_deal_flag)
      {
          img_deal_flag=0;        
          camera_get_img();  //����ͷ��ȡͼ�� 
          ctrl_main() ;       //����ͼ�񣬼�������Ƶ���ں���   
          show_run_info();
          total_time++;      //��¼���ƵĴ���
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

    while(!PORTA_ISFR);
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
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


//����ͼ����λ����ʾ
//��ͬ����λ������ͬ���������ʹ�� yy_����ͷ���ڵ��� ���
//���ʹ��������λ��������Ҫ�޸Ĵ���
void sendimg(void *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_����ͷ���ڵ��� ʹ�õ�����

    uart_putbuff(VCAN_PORT, (uint8_t *)cmd, sizeof(cmd));    //�ȷ�������

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //�ٷ���ͼ��
}
