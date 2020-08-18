#include"include.h"




/************************************************
*  �������ƣ�System_Init
*  ����˵����ϵͳ���й��ܳ�ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
void System_Init()
{
    static unsigned char ID1,ID2;
    
    //buzzer��ʼ��
    gpio_init(PORTD,15, GPO, LOW);
    Buzzer_Ring_ms(100);
    
    
    //LED:ȫ��
    gpio_init(PORTD, 3, GPO, HIGH);
    gpio_init(PORTD, 4, GPO, HIGH);
    gpio_init(PORTD, 5, GPO, HIGH);
    gpio_init(PORTD, 6, GPO, HIGH);
    
    
    //��ʱ�����
    //gpio_init(PORTE,7, GPO, LOW);
    //����ͣ��ָ��ӿ�
    //gpio_init(PORTE,8, GPI, LOW);
    
    
    //LCDģ���ʼ��
    LCD_init(FALSE);    //TRUE  �� FALSE�����Ż��Ƿ���ˢ�� 
    Disp_single_colour(Yellow);//Black
    //Print_child();    //С�ƺ�ˢ��
    
    
    //���뿪�ض˿ڳ�ʼ��
    Switch_init();
    //��Ҫ������ʼ��
    Parameters_init_CAR();
    LCD_Put_Int(0, 0,"Set_speed   :",(int)Speed.Goal_init,Red,Green);
    LCD_Put_Int(0,20,"Start enable:",StartLine.enable,Red,Green);
    LCD_Put_Int(0,40,"Ren   enable:",Ren.enable,Red,Green);
    LCD_Put_Int(0,60,"Slope enable:",Slope_Check_enable,Red,Green);
    if(Multiple_line_enable==1)
        LCD_PutString(0,80,"Multiple line mode",Red,Green);
    else
        LCD_PutString(0,80,"Single line mode",Red,Green);
    //LCD_Put_Int(0,100,"SD    enable:",SD_enable,Red,Green);

    
    //���ڳ�ʼ��
    uart_init(UART3,115200);
    uart_irq_DIS(UART3);//(����ҪDIS��������EN)
    
    
    //AD��ʼ��
    //adc_init(ADC1, SE14);//PB10
    //adc_init(ADC1, SE15);//PB11
    
    
    //1ms�Ķ�ʱ��
    pit_init_ms(PIT0, 1);
    set_irq_priority(68,2);
    enable_irq(68);
    
    
    //��ʼ��������L3G4200D,��ʼ�����ٶȼ�mma8451
    Init_L3G4200D();//11010011		
    ID1 = Single_Read(L3G4200_Addr,WHO_AM_I);
    MMA845x_init();//00011010
    ID2 = Single_Read(MMA845x_IIC_ADDRESS,WHO_AM_I_REG);
    if(ID1 == 0xD3 && ID2==0x1A)
    {
        LCD_PutString(0,120,"L3G + MMA OK!",Red,Green);
        //printf("IIC OK!!!");
    }
    else
    {
        //printf("IIC ERROR!!!");
    }
    
    
    //pwmģ��,���������ٳ�ʼ��
    Pwm_Port_init();
    Decoder_init();
    
    
    //ӥ������ͷ��ʼ��,�������жϺ�DMA
    Ov7725_Init();
    
    
    //sd��ģ���ʼ����dflash��ʼ��(�����ɵ�)
    if(SD_enable==1)
    {
        flash_init();
        for(int i=186;i<255;i++)
            flash_erase_sector(i);
    }
    //SDcard_Init();
    
    
    //������ʼ��
    delayms(100);
    Buzzer_Ring_ms(100);
}



/************************************************
*  �������ƣ�Parameters_init_CAR2
*  ����˵������Ҫ������ʼ��(CAR2)
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Parameters_init_CAR()
{
    //ƽ����Ҫ����
    Angle.G_angle = 12;
    Angle.OFFSET = 10;
    Angle.P = 400;
    Angle.D = 16;
    
    
    //�������
    //bit7-->>������ʹ�� 
    //bit6-->>����ʹ��
    //bit5-->>�ϰ�ʹ��
    //bit4-->>���к͵��е�ѡ��
    //bit3-->>SD��ʹ�ܣ�����ʱ��ص������ҿ��Ի��ɲ�����ʮ��ʹ�ܣ�
    //bit2-->>speed.bit2
    //bit1-->>speed.bit1
    //bit0-->>speed.bit0
    
    
    //**************������**************
    StartLine.enable = (sw_PORT&0x80)>>7;
    StartLine.Delaytime = 200;
    
    //**************����**************
    Ren.enable = (sw_PORT&0x40)>>6;
    //������ʱ�뿴��ϸ�趨
    
    //**************�µ�**************
    Slope_Check_enable = (sw_PORT&0x20)>>5;
    Block.enable = 1;
    
    
    //**************���л��Ǽ�Ȩƽ��**************
    Multiple_line_enable = (sw_PORT&0x10)>>4;
    
    //**************SD�Ͷ������**************
    //    SD_enable  = (sw_PORT&0x08)>>3;
    //    Ren.Multiple_enable = 0;
    if(Ren.enable==1)
        Ren.Multiple_enable = (sw_PORT&0x08)>>3;
    
    
    //�趨�ٶȺͷ���ĳ�ʼ��
    if((sw_PORT&0x07) == 0)
    {
        Speed.Mode = 0;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        //direction
        near_hang = 180;
        far_hang = 70;
        Direction.P = 0;
        Direction.D = 0;
    }
    else if((sw_PORT&0x07) == 1)//��಻�õ� 1.74m/s
    {
        Speed.Mode = 1;
        //speed
        Speed.Goal_init = 4700;
        Speed.P = 2;
        Speed.I = 0.3;
        //speed�����޷�
        Speed.I_Error_Start = 1000;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 200;
            far_hang = 85;
            Direction.P = 23 * 0.8 * 2;
            Direction.D = 0.6;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 100;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 35 * 0.8 * 2;
            Direction.D = 0.7;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //�ϰ�
        Block.delaytime = 150;
        Block.Offset_Init = 15;
        //�µ�
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 2)//��಻�õ�2.0m/s
    {
        Speed.Mode = 2;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5300;
        Speed.P = 2;
        Speed.I = 0.2;
        //speed�����޷�
        Speed.I_Error_Start = 1000;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -2000;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 180;
            far_hang = 65;
            Direction.P = 21 * 0.8 * 2;
            Direction.D = 0.45;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 95;
            Ren.Delaytime2 = 250;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 250;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        
        //�ϰ�
        Block.delaytime = 150;
        Block.Offset_Init = 15; 
        //�µ�
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 3)//��಻�õ�2.2m/s
    {
        Speed.Mode = 3;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5650;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 190;
            far_hang = 40;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 90;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //�ϰ�
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 4)//��಻�õ�2.4m/s��������
    {
        Speed.Mode = 4;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5850;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 190;
            far_hang = 40;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 100;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 75;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //�ϰ�
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 5)//�������Һ�С��鶼�����˵�~~~
    {
        Speed.Mode = 5;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 6100;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 175;
            far_hang = 10;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 105;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //����
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //�ϰ�
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 6)//��ʱ�����ӵĹ��ٵ�λ����ֹС�µ�
    {
        Speed.Mode = 6;
        //speed
        Speed.Goal_init = 4000;
        Speed.P = 2;
        Speed.I = 0.3;
        //speed�����޷�
        Speed.I_Error_Start = 1000;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 200;
            far_hang = 85;
            Direction.P = 23 * 0.8 * 2;
            Direction.D = 0.6;
            //����
            Ren.Delaytime1 = 80;
            Ren.Strength = 100;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 35 * 0.8 * 2;
            Direction.D = 0.7;
            //����
            Ren.Delaytime1 = 100;
            Ren.Strength = 65;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //�ϰ�
        Block.delaytime = 150;
        Block.Offset_Init = 15;
        //�µ�
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 7)//�������������������˵�λ
    {
        Speed.Mode = 7;//��1����ͼ��
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        
        Speed.I_Error_Start = 1000;//�����ٶ�
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = 1500;
        Speed.PWM_Integral = 1200;
        //direction
        Direction.P = 38 * 0.8 * 2;
        Direction.D = 0.6;
        //����
        Ren.Delaytime1 = 0;
        Ren.Strength = 80;
        Ren.Delaytime2 = 200;
        Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
    }
    else
    {
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        //direction
        Direction.P = 0;
        Direction.D = 0;
    }
}




/************************************************
*  �������ƣ�Buzzer_Ring_ms
*  ����˵������������
*  ����˵����unsigned int n��nΪn����
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Buzzer_Ring_ms(unsigned int n)
{
    buzzer = 1;
    delayms(n);
    buzzer = 0;
}



/************************************************
*  �������ƣ�Switch_init
*  ����˵�������뿪��8�����ų�ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Switch_init()
{
    gpio_init(PORTB, 0, GPI, LOW);
    gpio_init(PORTB, 1, GPI, LOW);
    gpio_init(PORTB, 2, GPI, LOW);
    gpio_init(PORTB, 3, GPI, LOW);
    gpio_init(PORTB, 4, GPI, LOW);
    gpio_init(PORTB, 5, GPI, LOW);
    gpio_init(PORTB, 6, GPI, LOW);
    gpio_init(PORTB, 7, GPI, LOW);
}


/************************************************
*  �������ƣ�PwmPort_init
*  ����˵������·pwm�����ʼ��,FTM0��CH6��ӦӲ��D6��FTM0��CH5��ӦӲ��D5
             DIR_L��ӦӲ��A19��DIR_R��ӦӲ��A24
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Pwm_Port_init()
{
    FTM_PWM_init(FTM0, CH2, 10000, 0);       //��·pwm
    FTM_PWM_init(FTM0, CH1, 10000, 0);       //��·pwm
    gpio_init(PORTE,25, GPO, LOW);              //DIR_L
    gpio_init(PORTE,24, GPO, LOW);              //DIR_R
    FTM_PWM_Duty(FTM0, CH2 ,  0);            //���
    FTM_PWM_Duty(FTM0, CH1 ,  0);            //���
}


/************************************************
*  �������ƣ�Decoder_init
*  ����˵������·���ٳ�ʼ��,FTM1��ӦӲ��A8��A9��FTM1��ӦӲ��A10��A11��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void Decoder_init(void)
{
    /*************FTM1******************/
    SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK;   //ʹ��PortB��ʱ��
    SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_FTM1_MASK;    //ʹ��FlexTimer0(FTM1)��ʱ��
    PORTA_PCR8 |= PORT_PCR_MUX(0x6);              //����PortB0�����Ź���ΪFTM1_QD_PHA
    PORTA_PCR9 |= PORT_PCR_MUX(0x6);              //����PortB1�����Ź���ΪFTM1_QD_PHB

    FTM1_CNT = 0;
    FTM1_MOD = 10000;
    FTM1_CNTIN = 0;
    FTM1_CNT = 0;
    FTM1_MODE |= FTM_MODE_WPDIS_MASK;
    FTM1_MODE |= FTM_MODE_FTMEN_MASK;
    FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTM1_CONF |= FTM_CONF_BDMMODE(3);
    FTM1_SC |= FTM_SC_PS(0);                      //��ʱ��ʱ��Ԥ��Ƶ����Ϊ 1
    FTM1_SC |= FTM_SC_TOIE_MASK;
    (void)FTM1_SC;
    FTM1_SC &= ~FTM_SC_TOF_MASK;
    FTM1_SC |= FTM_SC_CLKS(1);     //���òο�ʱ��Ϊ����ʱ�Ӳ�ʹ�����

    /***********FTM2*************/
    //SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTB_MASK; //ʹ��PortB��ʱ��
    SIM_BASE_PTR->SCGC3 |= SIM_SCGC3_FTM2_MASK;//ʹ��FlexTimer2(FTM2)��ʱ��
    PORTA_PCR10 |= PORT_PCR_MUX(0x6);    //����PortB18�����Ź���ΪFTM2_QD_PHA
    PORTA_PCR11 |= PORT_PCR_MUX(0x6);   //����PortB19�����Ź���ΪFTM2_QD_PHB

    FTM2_CNT = 0;
    FTM2_MOD = 10000;
    FTM2_CNTIN = 0;
    FTM2_CNT = 0;
    FTM2_MODE |= FTM_MODE_WPDIS_MASK;
    FTM2_MODE |= FTM_MODE_FTMEN_MASK;
    FTM2_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTM2_CONF |= FTM_CONF_BDMMODE(3);
    FTM2_SC |= FTM_SC_PS(0);
    FTM2_SC |= FTM_SC_TOIE_MASK;//��ʱ��ʱ��Ԥ��Ƶ����Ϊ 1
    (void)FTM2_SC;
    FTM2_SC &= ~FTM_SC_TOF_MASK;
    FTM2_SC |= FTM_SC_CLKS(1);//���òο�ʱ��Ϊ����ʱ�Ӳ�ʹ�����
}


