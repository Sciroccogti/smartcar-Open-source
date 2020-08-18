#include "GeneralDeclare.h"
#include "SoftIIC.h"
#include "TimerEventDriver.h"


__ramfunc void IIC_DELAY()
{
  uint8 i = 12;    /////为了让采集速度更快，是不是可以使用更少的延时呢，待测
  while(i--);
}


__ramfunc void IIC_start(void)////起始信号
{
  IIC_SDA_OUT;
  IIC_DELAY();
  IIC_DELAY();
  IIC_SDA_H; 
  IIC_SCL_H;
  IIC_DELAY();
  IIC_DELAY();
  IIC_SDA_L;
  IIC_DELAY();
  IIC_DELAY();
  IIC_SCL_L;
  IIC_DELAY();
  IIC_DELAY(); 
}

__ramfunc void IIC_stop(void)
{ 
    IIC_SDA_H;
    IIC_SCL_L; 
    IIC_DELAY();
    IIC_DELAY();
    IIC_SCL_H;
    IIC_DELAY();
    IIC_DELAY();
    IIC_SDA_H;
    IIC_DELAY();
    IIC_DELAY();
    IIC_SCL_H;
}

__ramfunc void IIC_ack(void)
{	
  IIC_SCL_L;
  IIC_DELAY();
  
  IIC_SDA_L;
  IIC_DELAY();
  
  IIC_SCL_H;
  IIC_DELAY();
  
  IIC_SCL_L;
  IIC_DELAY();
}

__ramfunc void IIC_noack(void)
{
  IIC_SCL_L;
  IIC_DELAY();
  IIC_SDA_H;
  IIC_DELAY();
  IIC_SCL_H;
  IIC_DELAY();
  IIC_SCL_L;
  IIC_DELAY(); 
}

__ramfunc void IIC_sendbyte(uint8 data)
{
  uint8 i=8;
  while(i--)
  {
    IIC_SCL_L;
    if(data&0x80)
    IIC_SDA_H; 
    else 
    IIC_SDA_L;   
    data<<=1;
    IIC_DELAY();
    IIC_SCL_H;
    IIC_DELAY();
  }
  IIC_SCL_L;
  IIC_DELAY();
  IIC_GPIO->PDIR |= (1 << IIC_SDApin);
  IIC_DELAY();
  IIC_SCL_H; 
  IIC_DELAY();
  IIC_SCL_L;
}

__ramfunc uint8 IIC_readbyte(void)
{
  uint8 i=8;
  uint8 ReceiveByte=0;
  
  IIC_SDA_H;	
  IIC_SDA_IN;	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    IIC_SCL_L;
    IIC_DELAY();
    IIC_SCL_H;
    IIC_DELAY();	
    if(IIC_DATA)
    {
      ReceiveByte|=0x01;
    }  
  }
  IIC_SDA_OUT;
  IIC_SCL_L;
  return ReceiveByte; 
}

void IIC_Write_Reg(uint8 addr, uint8 offset, uint8 data)
{
  IIC_start();
  IIC_sendbyte(addr << 1);
  IIC_sendbyte(offset);
  IIC_sendbyte(data);
  IIC_stop();
}

uint8 IIC_Read_Reg(uint8 addr, uint8 offset)
{
  uint8 data;
  IIC_start();
  IIC_sendbyte(addr << 1);
  IIC_sendbyte(offset);
  IIC_start();
  IIC_sendbyte((addr << 1) + 1);
  data = IIC_readbyte();
  IIC_noack();
  IIC_stop();
  return data;
}

uint8 ID = 0;
void IIC_init(void)
{
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  IIC_PORT->PCR[IIC_SDApin] = ALT1 | PULLUP | ODO;
  IIC_PORT->PCR[IIC_SCLpin] = ALT1 | PULLUP | ODO;
  IIC_GPIO->PDDR |= (1 << IIC_SDApin);
  IIC_GPIO->PDDR |= (1 << IIC_SCLpin);
  IIC_SDA_H;
  IIC_SCL_H;

#if (UsingSensor == 1)

  IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG1, 0x00);
  CommonDelay(10);
  IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG0, 0x01);
  CommonDelay(10);
  IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG2, 0x03);
  CommonDelay(10);
  IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG1, 0x02);
  CommonDelay(10);
  
  
  IIC_Write_Reg(IIC_FXOS8700_ADR, FXOS8700_SYSMOD, 0x00);  ///standby
  CommonDelay(10);
  IIC_Write_Reg(IIC_FXOS8700_ADR, FXOS8700_XYZ_DATA_CFG, 0x01);
  CommonDelay(10);
  IIC_Write_Reg(IIC_FXOS8700_ADR, FX0S8700_CTRL_REG1, 0x05);
  CommonDelay(10);
  
  
  
  
#elif  (UsingSensor == 0)
  IIC_Write_Reg(IIC_MPU3050_ADR, 0x3e, 0x80);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MPU3050_ADR, 0x15, 0x00);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MPU3050_ADR, 0x16, 0x11);
  CommonDelay(10); 
  IIC_Write_Reg(IIC_MPU3050_ADR, 0x17, 0x00);
  CommonDelay(10); 
  IIC_Write_Reg(IIC_MPU3050_ADR, 0x3e, 0x00);
  CommonDelay(10);
 
  
  IIC_Write_Reg(IIC_MMA8451_ADR, 0x2a, 0x00);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MMA8451_ADR, 0x0e, 0x02);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MMA8451_ADR, 0x2b, 0x02);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MMA8451_ADR, 0x2d, 0x00);
  CommonDelay(10);
  IIC_Write_Reg(IIC_MMA8451_ADR, 0x2a, 0x01);
  CommonDelay(10);
#endif
}

__ramfunc void IIC_Read_Sensor(Gyrotypedef *g, Acctypedef *a)
{
  uint8 Register[12];
#if (UsingSensor == 0)
  IIC_start();
  IIC_sendbyte(IIC_MPU3050_ADR << 1); 
  IIC_DELAY();
  IIC_sendbyte(0X1D); 
  IIC_DELAY();
  IIC_start();	
  IIC_sendbyte((IIC_MPU3050_ADR<<1)+1); 
  IIC_DELAY();
  Register[0]= IIC_readbyte();
  IIC_ack();
  Register[1]= IIC_readbyte();
  IIC_ack();
  Register[2]= IIC_readbyte();
  IIC_ack();
  Register[3]= IIC_readbyte();
  IIC_ack();
  Register[4]= IIC_readbyte();
  IIC_ack();
  Register[5]= IIC_readbyte();
  IIC_noack();
  IIC_stop();

  IIC_start();
  IIC_sendbyte(IIC_MMA8451_ADR<<1); 
  IIC_DELAY();
  IIC_sendbyte(0X01); 
  IIC_DELAY();
  IIC_start();	
  IIC_sendbyte((IIC_MMA8451_ADR<<1)+1); 
  IIC_DELAY();
  Register[6]= IIC_readbyte();
  IIC_ack();
  Register[7]= IIC_readbyte();
  IIC_ack();
  Register[8]= IIC_readbyte();
  IIC_ack();
  Register[9]= IIC_readbyte();
  IIC_ack();
  Register[10]= IIC_readbyte();
  IIC_ack();
  Register[11]= IIC_readbyte();
  IIC_noack();
  IIC_stop();
#elif (UsingSensor == 1)
  IIC_start();
  IIC_sendbyte(IIC_FXAS2100_ADR << 1); 
  IIC_DELAY();
  IIC_sendbyte(0X01); 
  IIC_DELAY();
  IIC_start();	
  IIC_sendbyte((IIC_FXAS2100_ADR<<1)+1); 
  IIC_DELAY();
  Register[0]= IIC_readbyte();
  IIC_ack();
  Register[1]= IIC_readbyte();
  IIC_ack();
  Register[2]= IIC_readbyte();
  IIC_ack();
  Register[3]= IIC_readbyte();
  IIC_ack();
  Register[4]= IIC_readbyte();
  IIC_ack();
  Register[5]= IIC_readbyte();
  IIC_noack();
  IIC_stop();

  IIC_start();
  IIC_sendbyte(IIC_FXOS8700_ADR<<1); 
  IIC_DELAY();
  IIC_sendbyte(0X01); 
  IIC_DELAY();
  IIC_start();	
  IIC_sendbyte((IIC_FXOS8700_ADR<<1)+1); 
  IIC_DELAY();
  Register[6]= IIC_readbyte();
  IIC_ack();
  Register[7]= IIC_readbyte();
  IIC_ack();
  Register[8]= IIC_readbyte();
  IIC_ack();
  Register[9]= IIC_readbyte();
  IIC_ack();
  Register[10]= IIC_readbyte();
  IIC_ack();
  Register[11]= IIC_readbyte();
  IIC_noack();
  IIC_stop();  
#endif
  
  g->Xdata = (float)((int16)(Register[1] |(Register[0] <<8)));//3050格式   XH XL YH YL ZH ZL
  g->Ydata = (float)((int16)(Register[3] |(Register[2] <<8)));
  g->Zdata = (float)((int16)(Register[5] |(Register[4] <<8)));
  a->Xdata = (float)((int16)(Register[7] |(Register[6] <<8))>>2);//8451格式  xyz（LSB|MSB<<8）>> 2
  a->Ydata = (float)((int16)(Register[9] |(Register[8] <<8))>>2);
  a->Zdata= (float)((int16)(Register[11]|(Register[10]<<8))>>2);
  return;
}



