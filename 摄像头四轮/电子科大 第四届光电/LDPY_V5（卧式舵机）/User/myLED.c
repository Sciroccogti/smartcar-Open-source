
#include "myLED.h"

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myLED_LEDInit
*  ����˵��:  LED��ʼ������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/ 
void myLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//LED3
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_26;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(26)=1;
	
	//LED4
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_27;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(27)=1;
	
	//LED5
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_28;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(28)=1;
	
	//������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_25;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(25)=0;
	
}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myLED_LED_ON
*  ����˵��:  LED��˸����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע: 
*************************************************************************/ 
void myLED_LED_ON(void)
{	  
	 LED3_ON;    
	 LDPY_Delay_ms(100);
	 LED3_OFF;
	 LDPY_Delay_ms(100);
	
	 LED4_ON;    
	 LDPY_Delay_ms(100);
	 LED4_OFF;
	 LDPY_Delay_ms(100);
	
	 LED5_ON;   
	 LDPY_Delay_ms(100);
	 LED5_OFF;
	 LDPY_Delay_ms(100);
}
	
/*----------------------------end of myLED.c-------------------------------*/
	
	
