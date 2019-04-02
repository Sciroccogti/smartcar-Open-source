#ifndef __SCCB_H__
#define __SCCB_H__


//--- Ĭ��SCL��PE11   SDA��PE12------//
void sccb_init(void);                           //��ʼ��SCCB�˿�ΪGPIO
void sccb_wait(void);                           //SCCBʱ����ʱ
void sccb_start(void);                          //��ʼ��־
void sccb_stop(void);                           //ֹͣ��־
unsigned char sccb_sendByte(unsigned char data);
unsigned char sccb_regWrite(unsigned char device,unsigned char address,unsigned char data);
unsigned char sccb_refresh(void);


#define SCL_OUT   DDRE11=1     
#define SCL_HIGH  PTE11_OUT=1
#define SCL_LOW   PTE11_OUT=0

#define SDA_OUT   DDRE12=1      
#define SDA_HIGH  PTE12_OUT=1
#define SDA_LOW   PTE12_OUT=0
#define SDA_IN    DDRE12=0      
#define SDA_DATA  PTE12_IN



#endif