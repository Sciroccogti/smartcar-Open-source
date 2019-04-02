

#ifndef  __LDPY_EXTI_H__
#define  __LDPY_EXTI_H__

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"

typedef enum
{
	Trigger_Rising  = 0x00090002,        //�����ش������ڲ�����
	Trigger_Falling = 0x000A0003,        //�½��ش������ڲ�����
	Trigger_Rising_Falling = 0x000B0002  //���ش�����  �ڲ�����
	
}EXTI_Trigger_mode;
	
void EXTI_Init(PORT_Type * PORTx,uint8_t GPIO_Pin_x,EXTI_Trigger_mode  Trigger_mode );
void LDPY_NVIC_PriorityGroupConfig(uint8_t NVIC_Group);
void LDPY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_SubPriority,uint8_t NVIC_Channel,uint8_t NVIC_Group);
void ENABLE_IRQ(uint8_t  IRQ_number);
void Un_ENABLE_IRQ(uint8_t  IRQ_number);

#endif

/*----------------------------end of LDPY_EXTI.h-----------------------------------*/
