#include "include.h"
/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
* �ļ���       ��L3G4200.c
* ����         �����������ģ��SPI��������
* ʵ��ƽ̨     ������ӡ�󿪷���
* ��汾       ������Ұ���
* Ƕ��ϵͳ     ��
* ����         ��xuxu
**********************************************************************************/


/************************************************
*  �������ƣ�L3g4200_Init
*  ����˵����L3G4200���ģ��spi��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
void L3G4200_Init()
{
  gpio_init(PORTB, 4,GPO,HIGH);
  gpio_init(PORTC, 6,GPO,HIGH);
  gpio_init(PORTC, 7,GPO,HIGH);
  gpio_init(PORTC,18,GPI,HIGH);
  SPI_Write_Byte(CTRL_REG1,0x0f);
  SPI_Write_Byte(CTRL_REG2,0x00);
  SPI_Write_Byte(CTRL_REG3,0x08);
  SPI_Write_Byte(CTRL_REG4,0x30);
  SPI_Write_Byte(CTRL_REG5,0x00);
}


/************************************************
*  �������ƣ�SPI_delay
*  ����˵����L3G4200���ģ��spi delay����
*  ����˵����lengthΪʱ��
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
void SPI_delay(signed int length)
{
  while (length >0)
    length--;
}



/************************************************
*  �������ƣ�SPI_Write_Byte
*  ����˵����L3G4200���ģ��spi д�ֽڳ���
*  ����˵����RegisterAddressΪ�Ĵ�����ַ��CommandΪ���ݻ�����
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
void SPI_Write_Byte(unsigned char RegisterAddress, unsigned char Command)
{
  unsigned char	ControlValue = 0;
  unsigned short int ValueToWrite = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress;
  SPC_H;SPI_delay(1);
  CS_H;SPI_delay(1);
  CS_L;SPI_delay(1);
  for(i=0; i<8; i++)//Write out the address
  { 	
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;		
    else
      SDI_L;		 
    SPI_delay(1);
    SPC_H;SPI_delay(1);
    ControlValue<<= 1;	
  }
  ValueToWrite = Command;
  for (i=0; i<8; i++)//Write out the data
  {
    SPC_L;
    if(0x80 == (ValueToWrite & 0x80))
      SDI_H;	  
    else
      SDI_L;
    SPI_delay(1);
    SPC_H;SPI_delay(1);
    ValueToWrite <<= 1;	
  }
  CS_H;SPI_delay(2);
}


/************************************************
*  �������ƣ�SPI_Read_Byte
*  ����˵����L3G4200���ģ��spi �����ֽڳ���
*  ����˵����RegisterAddressΪ�Ĵ�����ַ
*  �������أ�ReceiveDataΪ����ֵ
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
unsigned char SPI_Read_Byte(unsigned char RegisterAddress)
{
  unsigned char	ControlValue = 0;
  unsigned char ReceiveData = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress | 0x80;
  SPC_H;delay(1);	
  CS_H;delay(1);
  CS_L;delay(1);
  for(i=0; i<8; i++)//Write out the control word
  {
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;	  
    else
      SDI_L; 
    delay(1);
    SPC_H;
    delay(1);
    ControlValue <<= 1;	
  }
  for(i=0; i<8; i++) //Read data in
  {
    SPC_L;delay(1);
    SPC_H;
    ReceiveData <<= 1;		
    if(SDO)
      ReceiveData |= 1;	
    delay(1);
  }
  CS_H;	
  return ReceiveData;	
} 



/************************************************
*  �������ƣ�SPI_Read_2Byte
*  ����˵����L3G4200���ģ��spi ��˫�ֽڳ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    no����
*************************************************/
unsigned short int SPI_Read_2Byte(unsigned char RegisterAddress)
{
  unsigned char	ControlValue = 0;
  unsigned short int ReceiveData = 0;
  unsigned char	i = 0;
  ControlValue = RegisterAddress | 0xC0;
  SPC_H;SPI_delay(1);	
  CS_H;SPI_delay(1);
  CS_L;SPI_delay(1);
  for(i=0; i<8; i++)//Write out the control word
  {
    SPC_L;
    if(0x80 == (ControlValue & 0x80))
      SDI_H;	  
    else
      SDI_L; 
    SPI_delay(1);
    SPC_H;
    SPI_delay(1);
    ControlValue <<= 1;	
  }
  for(i=0; i<16; i++) //Read data in
  {
    SPC_L;SPI_delay(1);
    SPC_H;
    ReceiveData <<= 1;		
    if(SDO)
      ReceiveData |= 1;	
    SPI_delay(1);
  }
  CS_H;	
  return ReceiveData;	
} 


