#include "include.h"


/*************************************************************************
*  �������ƣ�VSYNC_IRQ
*  ����˵����PORTD�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�Լ���ʼ�������
*************************************************************************/
void VSYNC_IRQ(void)
{    
    static u16 czd=0;
    static u32 flag;
    //Clear Interrupt flag
    flag = PORTD_ISFR;
    PORTD_ISFR = flag;
    czd++;
    if(img_flag == IMG_START)	//��Ҫ��ʼ�ɼ�ͼ��
    {
        DMA_PORTx2BUFF_Init(CAMERA_DMA_CH, (void *)&PTB_BYTE2_IN, (void *)Image_fire, PTE27, DMA_BYTE1, CAMERA_SIZE , DMA_falling);
        DMA_EN(CAMERA_DMA_CH);            		//ʹ��ͨ��CHn Ӳ������
        DMA_DADDR(CAMERA_DMA_CH) = (u32)Image_fire; //�ָ���ַ
        img_flag = IMG_GATHER;		        //���ͼ��ɼ���
        disable_irq(90);  
    }
    else					//ͼ��ɼ�����
    {
        disable_irq(90); 			//�ر�PTA���ж�
        img_flag = IMG_FAIL;		//���ͼ��ɼ�ʧ��
    }
}




/*************************************************************************
*  �������ƣ�DMA0_IRQHandler
*  ����˵����DMA0
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
void DMA0_IRQHandler()
{
    DMA_DIS(CAMERA_DMA_CH);            	//�ر�ͨ��CHn Ӳ������
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);           //���ͨ�������жϱ�־λ
    img_flag = IMG_FINISH ;  
}






/*************************************************************************
*  �������ƣ�PIT0_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѳ���
*  ��    ע��
*************************************************************************/
void PIT0_IRQHandler(void)
{
    static unsigned char TimeCnt_20ms = 0;	  //20msʱ�������
    static unsigned char flag = 0;                //5ms�ķ�ʱ��־
    PIT_Flag_Clear(PIT0);
    Time_1ms++; 
    _LED1 = (Time_1ms/500)%2;
    TimeCnt_20ms++;
    if(TimeCnt_20ms >= 20)
    {TimeCnt_20ms = 0;}
    flag = TimeCnt_20ms%5;
    //����ΪֹT=0.5us
    /**************************************************************/
    /*********************Own code*********************************/
    /**************************************************************/   

    
    //�ٶȿ��� Tmax=8.5us
    if(flag==0 && Time_1ms>Stand_Time)
    {
        Measure_speed();
        Speed_control();
    }
    //����angle_Out������ֱ������ T=449us IICʱ�䳤
    else if(flag==1)
    {
        Measure_Acc();//T=340us
        Angle_control();//T=100us
        Motor_Control();//T=9us
    }
    else if(Ren.flag==1 && flag==2)//�����ֵķ������
    {
        Direction_control_with_Ren();//T=58us
        Motor_Control();
    }
    //�������
    if(Ren.flag==0 && Direction.output_enable==1)
    {
        Direction_control();//T=54us
        Motor_Control();
        Direction.output_enable = 0;
    }
    
    
    //���ٽ׶� T=3us��ֱ�Ӿʹﵽ�趨ֵ
    if(Time_1ms > Stand_Time && Speed.Goal < Speed.Goal_init)
    { 
        Speed.Goal = Speed.Goal_init + 1;
        buzzer = 1;
    }
    else
    {
        if(Ren.flag==1 || Cross_flag==1 || Slope_flag==1 || Time_1ms < Block.time + 1000 || smallS_flag==1)
            buzzer = 1;
        else
            buzzer = 0;
    }
    
    //LED��ˮ�ƣ�֤���ڽ���ʱ���ж�
    //Water_LEDs();
    /*********************************************************************/
    /**************************Own code end*******************************/
    /*********************************************************************/
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}







/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�USART3_IRQHandler
*  ����˵��������3 �ж� ���� ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѳ���
*  ��    ע��
*************************************************************************/
void USART3_IRQHandler(void)
{
    unsigned char ch;
    //����һ���ֽ����ݲ��ط�
    ch = uart_getchar (UART3);      //���յ�һ������
    switch(ch)
    {
//      case 'a':break;
//      case 'b': Angle.P+=10;  printf("angle_P:");SCISendFloat(UART3, Angle.P);break;
//      case 'c': Angle.P-=10;  printf("angle_P:");SCISendFloat(UART3, Angle.P);break;
//      case 'd': Angle.D+=10;  printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'e': Angle.D-=10;  printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'f': Angle.D+=1;   printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'g': Angle.D-=1;   printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case '0': Angle.OFFSET+=1;  printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '1': Angle.OFFSET-=1;  printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '2': Angle.OFFSET+=0.1;printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '3': Angle.OFFSET-=0.1;printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      //case '4':tg+=1;printf("tg:");SCISendFloat(UART3,tg);break;
      //case '5':tg-=1;printf("tg:");SCISendFloat(UART3,tg);break;
      //������ӵġ���
      case 'a':Speed.P+=0.2; printf("speed_P:");SCISendFloat(UART3,Speed.P);break;
      case 'b':Speed.P-=0.2; printf("speed_P:");SCISendFloat(UART3,Speed.P);break;
      case 'e':Speed.I+=0.1;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'f':Speed.I-=0.1;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'g':Speed.I+=0.01;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'h':Speed.I-=0.01;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      //case 'j':SD_enable=1;StartLine.flag=1;break;
//      case '6':pwm_Dead_L+=10;printf("pwm_Dead_L:");SCISendFloat(UART3,pwm_Dead_L);break;
//      case '7':pwm_Dead_L-=10;printf("pwm_Dead_L:");SCISendFloat(UART3,pwm_Dead_L);break;
//      case '8':pwm_Dead_R+=10;printf("pwm_Dead_R:");SCISendFloat(UART3,pwm_Dead_R);break;
//      case '9':pwm_Dead_R-=10;printf("pwm_Dead_R:");SCISendFloat(UART3,pwm_Dead_R);break;
      //default: printf("error");
    }
    uart_putchar(UART3,'\n');
}




/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�SysTick_Handler
*  ����˵����ϵͳ�δ�ʱ���жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��ucos���õõ�
*************************************************************************/
void SysTick_Handler(void)
{
    //    OSIntEnter();
    //    OSTimeTick();
    //    OSIntExit();
}





/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�HardFault_Handler
*  ����˵����Ӳ���Ϸ��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-4    �Ѳ���
*  ��    ע��������LED��˸��ָʾ������Ӳ���Ϸ�
*************************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
        printf("\n****Ӳ���Ϸô���!!!*****\r\n\n");
    }
}




/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�PendSV_Handler
*  ����˵����PendSV��������ϵͳ���ã��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-15    �Ѳ���
*  ��    ע��uC/OS�����л�����
*************************************************************************/
void PendSV_Handler(void)
{
}





/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�FTM0_IRQHandler
*  ����˵����FTM0���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/
void FTM0_IRQHandler()
{
}




/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*
*  �������ƣ�FTM1_IRQHandler
*  ����˵����FTM1���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/
void FTM1_IRQHandler()
{
}



