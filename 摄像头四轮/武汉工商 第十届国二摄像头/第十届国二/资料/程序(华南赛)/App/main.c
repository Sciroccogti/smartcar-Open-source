/**************************************************
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2015-05-28
 * author      
 * ��ע        FXӲ�������ѿ�
               ��Ƶ210M  ֡��112  ���ȶ����̣� BUS 105M   ��ֵ 0X45
               ��Ƶ150M  ֡��75   ���ȶ�
//�����ֵ��    845
//�����  :     953
//�Ҵ�����      736
 **************************************************/
#include "common.h"
#include "include.h" 
#include "init.h"
#include "tx_process.h"
 
extern uint16  val_z, val_y; 
uint8 _1MS_flag=0,heixian_flag_0=1;   //1MSʱ���־
int8   tingche_flag=0;
/**************************************************/
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void sendimg(uint8 *imgaddr, uint32 imgsize);
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void PIT0_IRQHandler();
void PIT1_IRQHandler();
/************************************************************
 *    @brief      main����
      v5.2
 *    @note      
***********************************************************/
uint8 imgbuff[CAMERA_SIZE];    //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];
extern int32 angle_pwm;
void  main(void)
{         
    //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ� 
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�
   // NVIC_SetPriority(UART3_RX_TX_IRQn,0); //�����ȼ�  ���ߵ����������Freecale��λ��
    NVIC_SetPriority(PIT0_IRQn,1);          //�������ȼ�
    NVIC_SetPriority(DMA0_IRQn,2);          //�������ȼ�
    //NVIC_SetPriority(PIT1_IRQn,3);          //�������ȼ�

    FTM_init();
    DangWei_();
    //Һ����ʼ��
    Site_t site     = {0, 0}  ;                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H} ;             //ͼ���С
    Size_t size;  
    //��ʾ����ͼ���С         
     LCD_init();   
     size.H = CAMERA_H;
     size.W = CAMERA_W;
    //Һ����ʼ������
    camera_init(imgbuff);                                   //�����жϸ�λ����
    
    //set_vector_handler(UART3_RX_TX_VECTORn , uart3_test_handler);
    //uart_rx_irq_en(UART3); 
    
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn  , DMA0_IRQHandler);     //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    pit_init_ms(PIT0,4);                                               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //���� PIT0 ���жϷ�����Ϊ 
    enable_irq(PIT0_IRQn);
    
    /*********************** ������Ϣ ��ʼ��  ***********************/
   //  key_event_init();                                                   //������Ϣ��ʼ��
   //  pit_init_ms(PIT1,10);                                               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
   //  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);                  //���� PIT0 ���жϸ�λ����Ϊ PIT0_IRQHandler
   //  enable_irq(PIT1_IRQn);
    /*********************** ������Ϣ ��ʼ��  ***********************/
    while(1)
    {
            
         // PTD15_OUT=0;                         	//ʾ������������
            camera_get_img();                                       //9MS  ����ͷ��ȡͼ��
          
            if(PTA12_IN==1&&PTA13_IN==1&&PTA14_IN==1&&PTA15_IN==1&&PTA16_IN==1&&PTA17_IN==1)
            {
              if( CAMERA_COLOR == 0)                                
              LCD_Img_Binary_Z(site, size, imgbuff, imgsize);  
            }                                                        //��ʾͼ��
            img_extract((uint8 *)img,(uint8 *)imgbuff, CAMERA_SIZE);  //Ѱ���ߺ��ߣ������߻�ȡ       
            PROCESS();
         // send_to_scope();      //��������ʱ��Ҫ��������������
         // deal_key_event();     //ɽ��СҺ������������ã��ò��뿪���ȶ���
         // PTD15_OUT=1;                                //��������
    }
}    
//�����ֵ��    845
//�����  :     953
//�Ҵ�����      736

void PIT0_IRQHandler()   //4MS����һ���ж�    20MS���ƶ�����һ��   ��ȡһ���ٶ�   ���Ƶ��һ��
{   
            JianChe();              
            if(PTD0_IN&&PTD1_IN)
            {
                heixian_flag_0=0; 
            }
            else
                heixian_flag_0=1;
            if(PTD6_IN)
            {
                tingche_flag=1;
            }
            
            _1MS_flag=_1MS_flag+1;
            if(_1MS_flag==5)   
            {
               if( angle_pwm>DJ_zuo )  angle_pwm=DJ_zuo ;      //����Χ0��330
               if( angle_pwm<DJ_you )  angle_pwm=DJ_you ;   
               ftm_pwm_duty(S3010_FTM,S3010_CH,angle_pwm);      // ������
         
               get_speed();
               Speed_Contorl();
               _1MS_flag=0;              
             }
             PIT_Flag_Clear(PIT0);
}


void PIT1_IRQHandler()  //10MS����һ���ж�    ���񰴼�״̬  ���������ж�
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT1);
}


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
/*
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
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_����ͷ���ڵ��� ʹ�õ�����

    uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //�ȷ�������

    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //�ٷ���ͼ��
}

//ѹ����ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
//srclen �Ƕ�ֵ��ͼ���ռ�ÿռ��С
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)      //��ѹΪ��ά����
{
    uint8 colour[2] = {1,0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1 ��ʾ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}