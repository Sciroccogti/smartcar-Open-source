 /*  
 * @file       TimerEventDriver.c
 * @brief      ʱ���¼����Ƶײ�����
 * @author     ���ϴ�ѧ�ƾ���
 * @version    v1.0
 * @date       2016-12-13
 */

#include "GeneralDeclare.h"
#include "TimerEventDriver.h"
#include "KeyBoardDriver.h"
#include "SystemControl.h"



uint32 TimerEventMASK; 
volatile   uint32 SysTicks = 0;
void systick_timing(uint32 time)
{
    SYST_RVR = time;                        //������ʱʱ��
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */
    SYST_CVR = 0x00;                        //��ռ�����
    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 | SysTick_CSR_TICKINT_MASK      //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );
    set_vector_handler(SysTick_VECTORn, SysTick_IRQHandler);
}


void SysTick_IRQHandler(void)
{
  SysTicks++;
}

float GetSysTime(void)
{
  return ((float)SysTicks * 0.001);
}

void EventControlTimer_init(void)   /////ʱ���¼����Ƴ�ʼ��
{
  uint32 cint = 0;
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;   //PITʱ�Ӵ�
  PIT_MCR   &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );
  
  cint = Event_0Cycle * bus_clk_khz; //ȡ�û���busʱ�ӵļ���ֵ
  PIT_BASE_PTR->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
  PIT_BASE_PTR->CHANNEL[0].LDVAL = cint - 1;
  PIT_BASE_PTR->CHANNEL[0].TFLG |=  PIT_TFLG_TIF_MASK; 
  
  PIT_BASE_PTR->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);        //ʹ�� PITn��ʱ��
  set_vector_handler(PIT0_VECTORn, TimerEvent_0_IRQHandler);
  NVIC_SetPriorityGrouping(4);
  NVIC_SetPriority(PIT0_IRQn, 0);      //������ȼ� �ȱ�ֱ֤��
  
  cint = Event_1Cycle * bus_clk_khz; //ȡ�û���busʱ�ӵļ���ֵ
  PIT_BASE_PTR->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
  PIT_BASE_PTR->CHANNEL[1].LDVAL = cint - 1;
  PIT_BASE_PTR->CHANNEL[1].TFLG |=  PIT_TFLG_TIF_MASK;
  
  PIT_BASE_PTR->CHANNEL[1].TCTRL = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);        //ʹ�� PITn��ʱ��
  set_vector_handler(PIT1_VECTORn, TimerEvent_1_IRQHandler);
  NVIC_SetPriority(PIT1_IRQn, 3);   ////////����ǰ2�����ȼ�
  
  ///////////////////��ʼ�����PIT0 �Լ� PIT1
  TimerEventMASK = 0;
}




void TimerEvent_0_IRQHandler(void)  ////2ms�ж�
{
  PIT_BASE_PTR->CHANNEL[0].TFLG |=  PIT_TFLG_TIF_MASK;  //���жϱ�־λ
  SystemStart();
  Car_motorControl();


}


void TimerEvent_1_IRQHandler(void)////5ms�ж�
{
  PIT_BASE_PTR->CHANNEL[1].TFLG |=  PIT_TFLG_TIF_MASK;  //���жϱ�־λ
  if(CarInfo.Motor_ON == 'F')
  {
    key_scan();
  }
}

void EventTimerDelay_IRQHandler(void)  //////�˺�����������ϵͳ�����в���һЩ���⴦�����������ⲿ����
{
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  ////�����־λ
  NVIC_DisableIRQ(PIT2_IRQn);  ////�ص��ж�
  TimerEventMASK = 0;   /////////����ϵͳ�ǲ����Ѿ���ȫ��Order�������ִ�����ˣ���ʱ�������
}

uint8 EventTimerDelay(uint16 ms, uint32 Order)   /////�ɹ��ⲿ���õ�����ж�
{
  if((TimerEventMASK & 0x80000000) == 0)   /////��ǰû������ִ�е����񣬱�������������֮ǰ���¼�δ���������д���µ����񣬴�ʱִ�������˳�򽫱����ң�һЩ��Ҫ������Ҳ������޷����
  {
    TimerEventMASK |= Order;        /////����Ҫִ�е�ָ��Ž�ȥ��ִ����֮�����λ��������
    PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~PIT_TCTRL_TEN_MASK;
    PIT_BASE_PTR->CHANNEL[2].LDVAL = ms * bus_clk_khz - 1;         ///��ʱ��װ�ؽ�ȥ
    PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK; 
    PIT_BASE_PTR->CHANNEL[2].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
    set_vector_handler(PIT2_VECTORn, EventTimerDelay_IRQHandler);
    NVIC_SetPriority(PIT2_IRQn, 0); //////��߼����ȼ�����Ϊʹ����һ����������ͷ�����л����ǲ������κβ�����ʱ�̣�������Ҳ��һ��
    NVIC_EnableIRQ(PIT2_IRQn);   ////����PIT2�ж�
    return 'T';
  }
  return 'F';
}

////////////////////////////����������ʱ������������ʱ�䣬ʹ��PIT2��ʱ��
void CommonDelay(uint32 ms) //ʹ��PIT2��ʱ�����ȿ��¼����ƺ���
{
  uint32 cint = 0;
  cint = bus_clk_khz * ms;
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~( PIT_TCTRL_TEN_MASK );
  PIT_BASE_PTR->CHANNEL[2].LDVAL = cint;
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  //���жϱ�־λ
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~( PIT_TCTRL_TEN_MASK );
  PIT_BASE_PTR->CHANNEL[2].TCTRL|= PIT_TCTRL_TEN_MASK;
  while(!(PIT_BASE_PTR->CHANNEL[2].TFLG & PIT_TFLG_TIF_MASK));
  return;
}

void TimeBegin(void)   //////////////���ڳ����ʱ
{
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
  PIT_BASE_PTR->CHANNEL[2].LDVAL = ~0;    //װ��������ֵ
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  //���жϱ�־λ
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
  PIT_BASE_PTR->CHANNEL[2].TCTRL|= PIT_TCTRL_TEN_MASK;  //����������
}



float TimeEnd(void)   //////////////���ڳ����ʱ
{
  uint32 a = 0;
  float  t = 0;
  a = (~0) - PIT_BASE_PTR->CHANNEL[2].CVAL;
  if(PIT_BASE_PTR->CHANNEL[2].TFLG & PIT_TFLG_TIF_MASK)
  {
    PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;
    PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK); //��ֹʹ��
    return (~0);
  }
  else
  {
    PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
  }
  if(a == (~0))
  {
    a--;
  }
  t = (float)a / 73.3;
  return t;
}