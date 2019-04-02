/*********************************************************************************************************************
 * 
 * @file       		isr.c
 *  				�жϷ�����
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include "main.h"

/****************ȫ�ֱ�������******************/
uint8 Stop_Flag=0;		//=OFFͣ��
uint8 Flag_Speed=ON;		//=ON�����ٶȿ���
uint8 Flag_Direction=ON;	//=ON���ӷ������
uint8 Flag_Debuge=OFF;		//=ON������ν���
uint8 Out_Flag=2;		//����ʾ������ʾ��־(2��ʾ�������ֵ�˲��ĵ��ֵ)

void PIT_CH0_IRQHandler(void){  //���ȼ���ߵ��ж� 5ms
    PIT_FLAG_CLR(PIT0);   
	//��־��������
//	static uint16 i = 0;
//	static uint16 j = 0;	
	//0.5sLED��˸
//      i++;
//      if(i >= 100)  {    
//      i = 0;
//      gpio_turn(REDLED);}
	//20msһ���ٶȿ���
//	j++;
//	if(j >= 4)  {    
//    	j = 0;
//	SpeedControl();}	
	DirectionControl();	//�������
	PWMOut();		//����PWM���	
	//����Ϊ����ʾ��������
	switch(Out_Flag){		
		case 0 :OutData[0] = (int)g_fRealSpeed;	//��ʵ�ٶ�
		  	 OutData[1] = (int)g_fSpeedError;	//�ٶ�ƫ��
			 OutData[2] = (int)g_fSpeedFilter;	//�ٶ��˲�
			 OutData[3] = (int)g_fExpectSpeed;	//�����ٶ�
			 break;					
		case 1 :OutData[0] = (int)sensor[0];	//ˮƽ����
		  	 OutData[1] = (int)sensor[1];	//ˮƽ�ҵ��
			 OutData[2] = (int)(1000*DirectionErr[0]);		//����ƫ��	  ���Ŵ�1000����
			 OutData[3] = (int)(1000*DirectionErr_dot[0]);	//����ƫ��΢�֣��Ŵ�1000����
			 break;					
		case 2 : OutData[0] = (int)sensor[0];//ˮƽ����
		  	 OutData[1] = (int)sensor[1];//ˮƽ�ҵ��
			 OutData[2] = (int)sensor[2];//��ֱ����
		         OutData[3] = (int)sensor[3];//��ֱ�ҵ��
                         OutData[4] = (int)sensor[0];//ˮƽ����
		  	 OutData[5] = (int)sensor[1];//ˮƽ�ҵ��
			 OutData[6] = (int)sensor[2];//��ֱ����
		         OutData[7] = (int)sensor[3];//��ֱ�ҵ��                         
			 break;						
		case 3 :OutData[0] = (int)g_fSpeedControlOut;		//�ٶȿ������	���죩
		  	 OutData[1] = (int)DirectionOut;	//�������		���ƣ�
			 OutData[2] = (int)g_PWMOut;					//��PWM			������
			 OutData[3] = 0;
			 break;
		default :break;
	}
        Send_Scope(OutData,8);
	//DataOutPut();
        PIT_FLAG_CLR(PIT0);  //�����־λ
}

void PIT_CH1_IRQHandler(void)
{
    PIT_FLAG_CLR(PIT1); 
}

void IRQ_IRQHandler(void)
{  
    CLEAR_IRQ_FLAG; 
    gpio_set(BUZZ,1);		//��������
    ir_decode();		//�������ʹ���    
    gpio_set(BUZZ,0); 		//��������
}

void KBI0_IRQHandler(void)
{
    CLEAN_KBI0_FLAG;

}

/*
�ǵý����жϺ������־λ
FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/



