#ifndef SOFTIIC_H
#define SOFTIIC_H


#define UsingSensor  1

#include "SystemControl.h"

#define IICDelay      8
#define IICACKDealy   5
#define IIC_PORT      PORTC_BASE_PTR
#define IIC_GPIO      PTC_BASE_PTR

#define IIC_SCLpin     1
#define IIC_SDApin     0
#define IIC_SDA_OUT    (IIC_GPIO->PDDR |= (1 << IIC_SDApin))
#define IIC_SDA_IN     (IIC_GPIO->PDDR &= ~(1 << IIC_SDApin))
#define IIC_DATA       ((IIC_GPIO->PDIR >> IIC_SDApin) & 0x01)
#define IIC_SDA_H      (IIC_GPIO->PDOR |= (1 << IIC_SDApin))
#define IIC_SCL_H      (IIC_GPIO->PDOR |= (1 << IIC_SCLpin))
#define IIC_SDA_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SDApin))
#define IIC_SCL_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SCLpin))

#define IIC_MMA8451_ADR           0X1C
#define IIC_MPU3050_ADR           0x68
#define IIC_FXAS2100_ADR          0x20
#define IIC_FXOS8700_ADR          0X1E

#define MPU3050_PWR_MGM           0x3E
#define MPU3050_SMPLRT_DIV        0x15
#define MPU3050_DLPF_FS_SYNC      0X16
#define MPU3050_INT_CFG           0X17


#define MMA8451_XYZ_DATA_CFG_REG  0x0E        /////Êä³öÄ£Ê½
#define MMA8451_CTRL_REG1_REG     0x2A
#define MMA8451_CTRL_REG2_REG     0x2B
#define MMA8451_CTRL_REG3_REG     0x2C
#define MMA8451_CTRL_REG4_REG     0x2D
#define MMA8451_CTRL_REG5_REG     0x2E



#define FXAS2100_CTRL_REG0        0x0D          ////        0x01   1000dps
#define FXAS2100_CTRL_REG1        0x13          ///  0x00  Stand By    0x02  Active  800HZ
#define FXAS2100_CTRL_REG2        0x14          //// 0x03
#define FXAS2100_WHO_AM_I         0x0C          /////ID  0xD7

#define FXOS8700_WHO_AM_I         0x0D          /////ID  0xD7
#define FXOS8700_XYZ_DATA_CFG     0x0E          ////0x01  4G
#define FXOS8700_SYSMOD           0x0B
#define FX0S8700_CTRL_REG1        0x2A 


void IIC_init(void);
__ramfunc void IIC_Read_Sensor(Gyrotypedef *g, Acctypedef *a);

#endif