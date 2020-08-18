#include "include.h"
/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��sccb.c
 * ����         ����������ͷOV7620 sccb����
 * ʵ��ƽ̨     ������ӡ�󿪷���
 * ��汾       ������Ұ���
 * Ƕ��ϵͳ     ��
 * ����         ��xuxu
**********************************************************************************/






/*************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_init
*  ����˵������ʼ��SCCB  ����SCL��PTD10 SDA��PTD11
*  ����˵������
*  �������أ���
*
*************************************************************************/
//-------��ʼ��PE11  PE12ΪGPIO--------//
void sccb_init(void)
{
  gpio_init(PORTE,11, GPO, HIGH); 
  gpio_init(PORTE,12, GPO, HIGH); 
}

/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_wait
*  ����˵����SCCB��ʱ����Ӧ̫С
*  ����˵������
*  �������أ���
*
*************************************************************************/
void sccb_wait(void)
{
  unsigned int i;
  for( i=0; i<2000; i++)
  {
    asm ("nop");
  }
}
/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_start
*  ����˵����SCCB����λ
*  ����˵������
*  �������أ���
*
*************************************************************************/
void sccb_start(void)
{
  SCL_OUT;
  SDA_OUT;
 
  SDA_HIGH;
  //sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_LOW;
  sccb_wait();
  SCL_LOW;
}

/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_stop
*  ����˵����SCCBֹͣλ
*  ����˵������
*  �������أ���
*
*************************************************************************/
void sccb_stop(void)
{
  SCL_OUT;
  SDA_OUT;
  
  SDA_LOW;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  SDA_HIGH;
  sccb_wait();
}

/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_sendByte
*  ����˵������SCCB�����Ϸ���һ���ֽ�
*  ����˵����data Ҫ���͵��ֽ�����
*  �������أ���
*
*************************************************************************/
unsigned char sccb_sendByte(unsigned char data)
{
  unsigned char i;
  v ack;
  SDA_OUT;
  for( i=0; i<8; i++)
  {
    if(data & 0x80)
      SDA_HIGH;
    else 
      SDA_LOW;
    data <<= 1;
    sccb_wait();
    SCL_HIGH;
    sccb_wait();
    SCL_LOW;
    sccb_wait();
  }
  SDA_HIGH;
  SDA_IN;
  sccb_wait();
  SCL_HIGH;
  sccb_wait();
  ack = SDA_DATA;
  SCL_LOW;
  sccb_wait();
  return ack;
}


/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_regWrite
*  ����˵����ͨ��SCCB������ָ���豸��ָ����ַ����ָ������
*  ����˵����device---�豸��  ��д������
*            address---д���ݵļĴ���
*            data---д������
*  �������أ�ack=1δ�յ�Ӧ��(ʧ��)    ack=0�յ�Ӧ��(�ɹ�)
*
*************************************************************************/
unsigned char sccb_regWrite(unsigned char device,unsigned char address,unsigned char data)
{
  unsigned char i;
  unsigned char ack;
  for( i=0; i<20; i++)
  {
    sccb_start();
    ack = sccb_sendByte(device);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(address);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    ack = sccb_sendByte(data);
    if( ack == 1 )
    {
      sccb_stop();
      continue;
    }
    
    sccb_stop();
    if( ack == 0 ) break;
  }
  return ack;
}


/************************************************************************
*                             �Ĭ�Ƽ�DEMOK Kinetis����С��
*
*  �������ƣ�sccb_refresh
*  ����˵����sccb�Ĵ���ˢ��
*  ����˵������
*  �������أ�ACK=1δ�յ�Ӧ��(ʧ��)    ACK=0�յ�Ӧ��(�ɹ�)
*
*************************************************************************/
unsigned char sccb_refresh()
{
    //-----------SCCB�ָ�Ĭ�ϳ�������----------//
    //--PCLK:73ns   HREF:63.6us   VSYN:16.64ms--//
    //--Ĭ�ϸ���ɨ�裬ȫ�ֱ���Ϊ640*480���ɼ�VSYN�ֱ�����640*240  
    //--��ÿ����HREF֮����640��PCLK
    //--��ÿ����VSYN֮����240��HREF    
    //sccb_regWrite(0x42,0x11,0x00);   
    //sccb_regWrite(0x42,0x14,0x04);
    //sccb_regWrite(0x42,0x28,0x20);
    //---------------------------------------//
    unsigned char ack1,ack2,ack3;
    unsigned char ACK = 1;
    for(u8 sccb_time=0; sccb_time<20; sccb_time++)
    {
     
      ack1 = sccb_regWrite(0x42,0x11,0x03);    //��ַ0X11-�ж��ķ�Ƶ(640*240)           PCLK:292ns   HREF:254.4us   VSYN:66.7ms
      ack2 = sccb_regWrite(0x42,0x14,0x24);    //��ַ0X14-QVGA(320*120)                 PCLK:584ns   HREF:508.8us   VSYN:66.7ms
      ack3 = sccb_regWrite(0x42,0x28,0x20);    //��ַ0X28-�����ɼ�ģʽ(320*240)         PCLK:584ns   HREF:254.4us   VSYN:66.7ms
      
      if( (ack1 == 0) && (ack2 == 0) && (ack3 == 0)) 
      {
        gpio_set (PORTE, 0, 0);  //д�ɹ�  ���� 
        sccb_wait();
        ACK = 0;
        break;
      }
      
      else
      {
        gpio_set (PORTE, 0, 1);  //дʧ��  ����
        sccb_wait();
        ACK = 1;
        continue;
      }
    }
    return ACK;
}