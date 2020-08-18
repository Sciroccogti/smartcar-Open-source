/*!
 *		��������ѧԺֱ������ͷ
 *		
 *		��Ҫ���������Ƴ������ⲿ�ж�PORTA��
 *
 * @file       MK60_it.c
 * @brief      �жϷ������
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */

#include "common.h"
#include "include.h"

/****************ȫ�ֱ�������******************/
uint8 Flag_SpeedControl = 0;
uint8 Flag_Stop = OFF;			//=OFFͣ��
uint8 Flag_Speed = ON;			//=ON���ٶȿ���
uint8 Flag_Direction = ON;		//=ON���ٶȿ���
uint8 Flag_Out = 2;				//����ʾ������ʾ��־
uint8 Flag_Buzz = ON;			//=OFF����������

/**
 * @brief		PIT0�жϷ�����
 * @author		��������ѧԺ
 * @date		2017
 */
void PIT0_IRQHandler(void)
{

	PIT_Flag_Clear(PIT0);
//����ΪPIT�жϴ�����
	
	if(Flag_ir>0)Flag_ir--;	
	else enable_irq(PORTC_IRQn);	//�������һ������֮����ʱһ���ٿ����ⲿ�жϣ���ֹ��һ�μ���η�Ӧ
	
//����ΪPIT�жϴ�����
}


/**
 * @brief		��������ⲿ�жϷ�����
 * @author		��������ѧԺ
 * @date		2017
 */
void Infrared_Handler(void)
{
    uint8  n = 0;   
       n = 4;		//���ź�
    if(PORTC_ISFR & (1 << n))       
    {
       
       
            PORTC_ISFR  = (1 << n);        //д1���жϱ�־λ      
			
            IR_Decoding();                //������� 

#if 0         /*  ����Ϊled��ʾ4��������ֵ */   
            oled_fill(0);
            oled_print_16x8short(0,0,UserCode);//���û���
            oled_print_16x8short(0,2,UserData);//�û�����
            oled_print_16x8short(0,4,KeyCode);//������
            oled_print_16x8short(0,6,KeyData);//��������
#endif            
    }   
}

/**
 * @brief		����ͷPORTA�жϷ�����
 *	
 *	������жϱ�־ͼ��Ĳɼ���ʼ
 *	ͬʱ�����Ŀ��ƿ�ʼ��ʹ�õ���һ�ε�ͼ�����ת��
 *	����һ��ͼ��ɼ���һ��С�����ơ�
 *	С�����Ƶ�ʱ�䲻�ó���ͼ��ɼ����ڣ�1/150��s
 *
 * @author		��������ѧԺ
 * @date		2017
 */
void PORTA_IRQHandler(void)
{
  	//��־��������
	static uint16 Flag_Led = 0;	
  
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                //���жϱ�־λ
    n = 29;                          //���ж�
    if(flag & (1 << n))              //PTA29�����ж�
    {
		gpio_set (K1, 1);//�����߼�����
		
		//0.5sLED��˸
		Flag_Led++;
		if(Flag_Led >= 40)  
        {    
            Flag_Led = 0;
            led_turn(LED0);         //LEDָʾ 
			gpio_set (BUZZ, 0);		//��ͣ�عرշ���������һֱ��
        }	
		
        camera_vsync(); 			//��ʼ�ɼ�ͼ��(����ͼ���ַ)1.67us
		flag_imgbuff != 0?img_extract(img, p_imgbuff1,CAMERA_SIZE):img_extract(img, p_imgbuff2,CAMERA_SIZE);
									//��ѹ��һ��ͼ��0.99ms
//����Ϊ��Ҫ���Ʋ��֣���Ҫ����5.5ms��		
		DirectionControl();		//����������
		GalAcceAndGyroY();		//�ɼ�������ٶȼƺ������� ����ʱ0.78ms��
		KalmanFilter();			//�������˲����ó��Ƕȣ���ʱ0.03ms��
		BalanceControl();		//ֱ���������
		PWMOut();				//����PWM�������������εķ��������ֱ��������ϴε��ٶ�������ӣ�
		Flag_SpeedControl++;	
		if(Flag_SpeedControl >= 20)
		{
			SpeedControl();		//�ٶȿ�������20*(1/150)s
			Flag_SpeedControl = 0;
		}
		SpeedControlOut();		//�ٶ�������ڣ�1/150��s��Ҳ����ÿ�ε��ٶȿ��Ʒ�20�����
		
		//gpio_set (K3, 1);//�����߼�����
		//gpio_set (K3, 0);//�����߼�����	
		switch(Flag_Out)		//����ʾ����
		{
		  case 0 :	OutData[0] = g_fAngleOfAcce;//g_nValueOfAcce;//���ٶȽǶ�ֵ
		  			OutData[1] = g_fAngleSpeed[GyroY];//g_nValueOfGyro[GyroY];
					OutData[2] = g_PWMOut;
					OutData[3] = g_AngleOfCar;		//�������ںϽǶ�
					break;
					
		  case 1 : 	OutData[0] = (int)g_fRealSpeed;//g_nLeftpulse;//
		  			OutData[1] = (int)g_fSpeedError;//��ʵ�˲��ٶ��������ٶȵ�ƫ��//g_nRighpulse;
					OutData[2] = (int)g_fSpeedControlOut;//g_fSpeedFilter;		
					OutData[3] = (int)g_fExpectSpeed;
					break;
					
		  case 2 :	OutData[0] = g_nDirectionError;
		  			OutData[1] = g_fDirectionError_dot;//
					//OutData[2] = g_fDirectionControlOut;//g_fDirectionError_dot;
					//OutData[3] = 0;//fAngleTurn;
					break;
					
		  case 3 :	OutData[0] = (int)g_fSpeedControlOut;		//�ٶȿ���������죩
		  			OutData[1] = (int)g_fDirectionControlOut;	//����������ƣ�
					OutData[2] = (int)g_PWMOut;					//��PWM������
					OutData[3] = (int)g_BlanceControlOut;		//ƽ�����������ϣ�
					break;
		  default :break;
		}
		OutPut_Data();	//����ʾ�������
		gpio_set (K1, 0);//�����߼�����
//����Ϊ��Ҫ���Ʋ���
		
    }    
}

/**
 * @brief		����ͷDMA0�жϷ�����
 *	
 *	������жϱ�־��һ��ͼ��ɼ���ϣ�
 *	��ʼ����һ�βɼ�ͼ��Ĵ�ŵ�ַ
 *	����ͼ��洢��ַ������ʹ�ã�
 *	���βɼ�ͼ���ʱ�򣬾Ϳ���ʹ���ϴβɼ���ͼ��
 *	�����ÿ���������̡�
 *
 * @author		��������ѧԺ
 * @date		2017
 */
void DMA0_IRQHandler(void)	//1.875us(dma��������һ�����жϿ�ʼ���0.22ms)
{
	//gpio_set (K2, 1);//�����߼�����
    camera_dma();		//dma�жϱ�־ͼ�������
	camera_get_img();	//�ȴ�ͼ������ϣ���ɱ�־��Ϊ��ʼ��־��
	flag_imgbuff =! flag_imgbuff;
	ov7725_eagle_img_buff = (flag_imgbuff == 0?p_imgbuff1:p_imgbuff2);//�����ʼ��ͼ���ַ
	//gpio_set (K2, 0);//�����߼�����
}


