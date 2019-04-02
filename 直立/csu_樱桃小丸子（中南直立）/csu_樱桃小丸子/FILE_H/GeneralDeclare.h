#ifndef GENERALDECLARE_H
#define GENERALDECLARE_H


typedef unsigned char            uint8; 
typedef unsigned short int       uint16;
typedef unsigned long int         uint32;
typedef unsigned long long int    uint64;
typedef long long int             int64;
typedef char                       int8;
typedef short int                  int16;
typedef long  int                  int32;

typedef struct{
  uint8 addr;
  uint8 val;
}Registermaptypedef;         //外部寄存器


////////////包含Cortex-M内核的通用头文件

#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc

#include    "Startup_K6X.h"                //Kinetis K6x系列启动头文件
#define     __NVIC_PRIO_BITS    4           //此处宏定义是为core_cm4提供抢占优先级组
#include    "math.h"
#include    "arm_math.h"                   //此处会添加core_cm4 CORTEX_M4通用事务处理器
            //   NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);  设置中断优先级
            //   uint32_t NVIC_GetPriority(IRQn_Type IRQn);            取得优先级
            //   NVIC_SystemReset();                                   System Restart
            //   NVIC_DisableIRQ(IRQn_Type IRQn);
            //   NVIC_EnableIRQ(IRQn_Type IRQn);
            //   NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
            //   uint32_t NVIC_GetPriorityGrouping(void);

/////////////////芯片寄存器选择
#if   (defined(MK60DZ10))
#include    "MK60DZ10.h"
#elif (defined(MK60F15))
#include    "MK60F15.h"
#elif (defined(MK66F18))
#include    "MK66F18.h"
#endif
#include "K6X_PORT_CFG.h"


#if   (defined(IAR))
#include "intrinsics.h"
#endif

#define EnableInterrupts                __enable_irq()              //使能全部中断
#define DisableInterrupts               __disable_irq()             //禁止全部中断 
#define ARRAY_SIZE(ADDR)                (sizeof((ADDR)) / sizeof(((ADDR)[0])))//取得数组元素个数

#endif



