
#include "myOLED.h"

uint8_t OLED_GRAM[8][128];
extern uint8_t  ASCII_code[101][6];
extern uint16_t ADC_value[128];

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_GPIO_Init
*  ����˵��:  OLED���ų�ʼ������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/ 
void myOLED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//OREST
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//ODC
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//OSCK
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//OSDA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);

}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Init
*  ����˵��:  OLED��ʼ��
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/ 

void myOLED_Init(void)
{
	myOLED_GPIO_Init();
	myOLED_REST();
	
	myOLED_Write_REG(0xAE);	//�ر���ʾ
	
	myOLED_Write_REG(0xD5);	//ʱ�ӷ�Ƶ���ӣ���Ƶ��
	myOLED_Write_REG(0x80);	//[3:0]��Ƶ����;[7:4]��Ƶ��
	
	myOLED_Write_REG(0xA8);	//��������
	myOLED_Write_REG(0x3F);	//1/64
	
	myOLED_Write_REG(0xD3);	//��ʾƫ��
	myOLED_Write_REG(0x02);	//0
	
	myOLED_Write_REG(0x40);	//��ʾ��ʼ��[5:0]����
	
	myOLED_Write_REG(0x8D);	//��ɱ�
	myOLED_Write_REG(0x14);	//bit2����/�ر�
	
	myOLED_Write_REG(0x20);	//�ڴ��ַģʽ
	myOLED_Write_REG(0x02);	//[1:0]00��ģʽ;01��ģʽ;10Ĭ��ҳ��ַģʽ
		
	myOLED_Write_REG(0xA1);	//���ض���,����ɨ��
	
	myOLED_Write_REG(0xC0);	//COMɨ�跽��bit3:0��ͨģʽ;1�ض���ģʽ
	
	myOLED_Write_REG(0xDA);	//COMӲ������
	myOLED_Write_REG(0x10);	//[5:4]����
	
	myOLED_Write_REG(0x81);	//�Աȶ�
	myOLED_Write_REG(0xEF);	//
	
	myOLED_Write_REG(0xD9);	//Ԥ�������
	myOLED_Write_REG(0xF1);	//[3:0]PHASE 1[7:4]PHASE 2
	
	myOLED_Write_REG(0xDB);	//VCOMH��ѹ����
	myOLED_Write_REG(0x30);	//
	
	myOLED_Write_REG(0xA4);	//ȫ����ʾbit0:1������0�ر�
	myOLED_Write_REG(0xA6);	//��ʾ��ʽbit0:1����0����
	
	myOLED_Write_REG(0xAF);	//������ʾ
	
	myOLED_Clear();
	
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_OLEDInit
*  ����˵��:  OLEDд�Ĵ�������
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/ 
void myOLED_Write_REG(uint8_t COM)
{
	uint8_t i = 0;
	OLED_DC=0;	//0:ָ��
	for(i=0;i<8;i++)
	{
			if(COM&0x80)
				OLED_DA=1;
			else
				OLED_DA=0;
				
			OLED_CK=0;
	//		Delay_us(10);
			OLED_CK=1;
	//		Delay_us(10);
			
			COM<<=1;
	}
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Write_DAT
*  ����˵��:  OLEDд����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/ 	
void myOLED_Write_DAT(uint8_t DAT)
{
	uint8_t i = 0;
	OLED_DC = 1;		//1����
	for(i=0;i<8;i++)
	{
			if(DAT&0x01)		//����Ӧ���Ǹ�λ��ǰ��������ʾ����ʱ�����ģ�����ѡ���λ��ǰ
				OLED_DA=1;
			else 
				OLED_DA=0;
				
			OLED_CK=0;
	//		Delay_us(10);
			OLED_CK=1;
	//		Delay_us(10);
			
			DAT>>=1;			//��λ��ǰ
	}
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_REST
*  ����˵��:  OLED��λ
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_REST(void)
{
	OLED_RST=0;
	LDPY_Delay_ms(50);
	OLED_RST=1;
	LDPY_Delay_us(50);
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Clear
*  ����˵��:  OLED����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_Clear(void)
{
	uint8_t H = 0,L = 0;
	for(H=0;H<8;H++)
	  for(L=0;L<128;L++)
			OLED_GRAM[H][L]=0x00;
	myOLED_RefreshGRAM();
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_RefreshGRAM
*  ����˵��:  OLEDд����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  	��ʱû���õ�
*************************************************************************/
void myOLED_RefreshGRAM(void)
{
		uint8_t i = 0,n = 0;
		for(i=0;i<8;i++)
		{
				myOLED_Write_REG(0xB0+i);	//
				myOLED_Write_REG(0x02);	  //
				myOLED_Write_REG(0x10);	  //
				for(n=0;n<128;n++)
					myOLED_Write_DAT(OLED_GRAM[i][n]);//
		}
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_String
*  ����˵��:  OLED��ʾ�ַ���
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_String(uint8_t x,uint8_t y,const char *P)
{
	uint8_t i = 0;
	myOLED_Set_XY(x,y);//
	for(i=0;i<strlen(P);i++)
		{
			myOLED_Char(P[i]-32);//
		}

}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Char
*  ����˵��:  OLED��ʾ�ַ�
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_Char(uint8_t ch)
{
	uint8_t Lie = 0;//û���ַ���6��
			  //�ַ���ASCII���е�λ��
	
	for(Lie=0;Lie<6;Lie++) //��6�д�ӡ
	{
		myOLED_Write_DAT(ASCII_code[ch][Lie]);
	}
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  
*  ����˵��:  
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  ��XYλ�û���X��0~7(ҳ��ʽ);Y:0~128(�з�ʽ)
*********************************************************/
void myOLED_XY_point(uint8_t x,uint8_t y,uint8_t ch)
{
	myOLED_Set_XY( x, y);//
	myOLED_Write_DAT(ch);//
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Set_XY
*  ����˵��:  OLED����ҳ��
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_Set_XY(uint8_t x,uint8_t y)
{
	myOLED_Write_REG(0xB0+x);//
	myOLED_Write_REG(((y&0xF0)>>4)|0x10);//
	myOLED_Write_REG((y&0x0F)|0x01);//
}
/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Dec
*  ����˵��:  OLED��ʾ5λ����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_Dec(uint8_t x,uint8_t y, uint16_t DEC)
{
	uint8_t x5 = 0,x4 = 0,x3 = 0,x2 = 0,x1 = 0,row = 0;
	
	x5=DEC/10000+16;//
	x4=DEC%10000/1000+16;//
	x3=DEC%1000/100+16;//
	x2=DEC%100/10+16;//
	x1=DEC%10+16;//
	myOLED_Set_XY( x, y);//
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x5][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x4][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x3][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x2][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x1][row]);
}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Dec
*  ����˵��:  OLED��ʾ5λ����
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  
*************************************************************************/
void myOLED_Dec4(uint8_t x,uint8_t y, int16_t DEC)
{
	uint8_t x5 = 0,x4 = 0,x3 = 0,x2 = 0,x1 = 0,row = 0;
	uint8_t i = 0;
	
	if( DEC < 0)
	{
		DEC = 0 - DEC;
		i = 1;
	}
	x5=DEC/10000+16;//
	x4=DEC%10000/1000+16;//
	x3=DEC%1000/100+16;//
	x2=DEC%100/10+16;//
	x1=DEC%10+16;//
	myOLED_Set_XY( x, y);//
	if(i == 0)
	{
		for(row=0;row<6;row++)
			myOLED_Write_DAT(ASCII_code[x5][row]);
	}
	else
	{
		myOLED_Char( 13 );
	}
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x4][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x3][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x2][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x1][row]);
}

/*************************************************************************
*				��	��	ƫ	��   
*  ��������:  myOLED_Write_DAT
*  ����˵��:  ��ʾADC��ֵ
*  ����˵��:	
*            
*  ��������:
*  �޸�ʱ��:2014-05-09
*  ��ע:  	��ʾADC��ֵ
		ADC��ֵΪ16λ���ݣ���Ϊ8�Σ��ֱ���ʾ��0~7ҳ
		128�����ݸպ���ʾ�꣬ÿһ��ֻ��ӡ0x18���γɻ���
*********************************************************/
void myOLED_ADC_value(uint16 * value)
{
	uint8_t n = 0,PA = 0;
	uint16_t temp = 0;
	myOLED_Set_XY(0,0);
	for(n=0;n<128;n++)
	{
		temp=*(value+n);//
		
		if(temp>0x00&&temp<0x01F4)
				PA=0;
		else if((temp>=0x01F5)&&(temp<=0x03F8))
				PA=1;
		else if((temp>=0x03F9)&&(temp<=0x05DC))
				PA=2;
		else if((temp>=0x05DD)&&(temp<=0x07D0))
				PA=3;
		else if((temp>=0x07D1)&&(temp<=0x09C4))
				PA=4;
		else if((temp>=0x09C5)&&(temp<=0x0BB8))
				PA=5;
		else if((temp>=0x0BB9)&&(temp<=0x0DAC))
				PA=6;
		else if((temp>=0x0DAD)&&(temp<=0x1004))
				PA=7;
		
		myOLED_XY_point( PA, n,0x18);//	

	}
	myOLED_Clear();
}
/***********************************************************/


/*************************************************************************
*                             ���ƫ��
*  ��������:myOLED_Decimals
*  ����˵��:��ʾС��  ��ʽ _._ _ _
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:
*************************************************************************/ 
void myOLED_Decimals(uint8_t x,uint8_t y, float als)
{
	uint16_t temp0 = 0,temp1 = 0,temp2 = 0 , temp3 = 0;
	uint16_t temp16 = 0;
	
	temp16 = (uint16_t)(als * 100);
	temp0 = (uint16_t)(temp16 / 1000);
	temp1 = (uint16_t)(temp16 % 1000 / 100);
	temp2 = (uint16_t)(temp16 % 100 / 10);
	temp3 = (uint16_t)(temp16 % 10 / 1);
	
	
	myOLED_Set_XY( x, y);

	myOLED_Char( temp0 + 16 );
	myOLED_Char( temp1 + 16 );
	myOLED_Char( 14 );
	myOLED_Char(temp2 + 16);
	myOLED_Char(temp3 + 16 );
	
}
/*************************************************************************
*                             ���ƫ��
*  ��������:myOLED_
*  ����˵��:
*  ����˵��:
*            
*  ��������:
*  �޸�ʱ��:
*  ��		 ע:
*************************************************************************/ 
void myOLED_CCDwave(CCD_Info * CCD1_info,CCD_Info * CCD2_info, CCD_Info * CCD3_info)
{
	
	uint8_t i = 0, PA1 = 0, PA2 = 0, PA3 = 0;

	myOLED_Set_XY( 0, 0);

	for( i=0; i<128; i++)
	{
		PA1 = CCD1_info->PixelBinary[i];
		PA2 = CCD2_info->PixelBinary[i];
		PA3 = CCD3_info->PixelBinary[i];
		
		if(PA1 == 1)
		{
			myOLED_XY_point( 1, i,0x02);
		}
		else
		{
			myOLED_XY_point( 1, i,0x40);
		}
		if(PA2 == 1)
		{
			myOLED_XY_point( 2, i,0x02);
		}
		else
		{
			myOLED_XY_point( 2, i,0x40);
		}
		if(PA3 == 1)
		{
			myOLED_XY_point( 3, i,0x02);
		}
		else
		{
			myOLED_XY_point( 3, i,0x40);
		}
		
	}
	
	myOLED_XY_point( 1, CCD1_info->CentralLine[0],7);
	myOLED_XY_point( 2, CCD2_info->CentralLine[0],7);
	myOLED_XY_point( 3, CCD3_info->CentralLine[0],7);
	
}









