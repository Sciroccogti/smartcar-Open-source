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
}Registermaptypedef;         //�ⲿ�Ĵ���


////////////����Cortex-M�ں˵�ͨ��ͷ�ļ�

#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc

#include    "Startup_K6X.h"                //Kinetis K6xϵ������ͷ�ļ�
#define     __NVIC_PRIO_BITS    4           //�˴��궨����Ϊcore_cm4�ṩ��ռ���ȼ���
#include    "math.h"
#include    "arm_math.h"                   //�˴������core_cm4 CORTEX_M4ͨ����������
            //   NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);  �����ж����ȼ�
            //   uint32_t NVIC_GetPriority(IRQn_Type IRQn);            ȡ�����ȼ�
            //   NVIC_SystemReset();                                   System Restart
            //   NVIC_DisableIRQ(IRQn_Type IRQn);
            //   NVIC_EnableIRQ(IRQn_Type IRQn);
            //   NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
            //   uint32_t NVIC_GetPriorityGrouping(void);

/////////////////оƬ�Ĵ���ѡ��
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

#define EnableInterrupts                __enable_irq()              //ʹ��ȫ���ж�
#define DisableInterrupts               __disable_irq()             //��ֹȫ���ж� 
#define ARRAY_SIZE(ADDR)                (sizeof((ADDR)) / sizeof(((ADDR)[0])))//ȡ������Ԫ�ظ���

#endif



