 /*  
 * @file       TimerEventDriver.c
 * @brief      时间事件控制底层驱动
 * @author     中南大学黄竞辉
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
    SYST_RVR = time;                        //设置延时时间
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */
    SYST_CVR = 0x00;                        //清空计数器
    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //使能 systick
                 | SysTick_CSR_TICKINT_MASK      //使能中断 (注释了表示关闭中断)
                 | SysTick_CSR_CLKSOURCE_MASK    //时钟源选择 ( core clk)
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

void EventControlTimer_init(void)   /////时间事件控制初始化
{
  uint32 cint = 0;
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;   //PIT时钟打开
  PIT_MCR   &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );
  
  cint = Event_0Cycle * bus_clk_khz; //取得基于bus时钟的计数值
  PIT_BASE_PTR->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
  PIT_BASE_PTR->CHANNEL[0].LDVAL = cint - 1;
  PIT_BASE_PTR->CHANNEL[0].TFLG |=  PIT_TFLG_TIF_MASK; 
  
  PIT_BASE_PTR->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);        //使能 PITn定时器
  set_vector_handler(PIT0_VECTORn, TimerEvent_0_IRQHandler);
  NVIC_SetPriorityGrouping(4);
  NVIC_SetPriority(PIT0_IRQn, 0);      //最高优先级 先保证直立
  
  cint = Event_1Cycle * bus_clk_khz; //取得基于bus时钟的计数值
  PIT_BASE_PTR->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
  PIT_BASE_PTR->CHANNEL[1].LDVAL = cint - 1;
  PIT_BASE_PTR->CHANNEL[1].TFLG |=  PIT_TFLG_TIF_MASK;
  
  PIT_BASE_PTR->CHANNEL[1].TCTRL = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);        //使能 PITn定时器
  set_vector_handler(PIT1_VECTORn, TimerEvent_1_IRQHandler);
  NVIC_SetPriority(PIT1_IRQn, 3);   ////////保留前2级优先级
  
  ///////////////////初始化完成PIT0 以及 PIT1
  TimerEventMASK = 0;
}




void TimerEvent_0_IRQHandler(void)  ////2ms中断
{
  PIT_BASE_PTR->CHANNEL[0].TFLG |=  PIT_TFLG_TIF_MASK;  //清中断标志位
  SystemStart();
  Car_motorControl();


}


void TimerEvent_1_IRQHandler(void)////5ms中断
{
  PIT_BASE_PTR->CHANNEL[1].TFLG |=  PIT_TFLG_TIF_MASK;  //清中断标志位
  if(CarInfo.Motor_ON == 'F')
  {
    key_scan();
  }
}

void EventTimerDelay_IRQHandler(void)  //////此函数用于在主系统运行中插入一些特殊处理函数，不可外部调用
{
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  ////清除标志位
  NVIC_DisableIRQ(PIT2_IRQn);  ////关掉中断
  TimerEventMASK = 0;   /////////不管系统是不是已经把全部Order里的任务执行完了，到时间就清零
}

uint8 EventTimerDelay(uint16 ms, uint32 Order)   /////可供外部调用的随机中断
{
  if((TimerEventMASK & 0x80000000) == 0)   /////当前没有正在执行的任务，必须这样，否则之前有事件未完成又重新写入新的任务，此时执行任务的顺序将被打乱，一些重要的任务也会因此无法完成
  {
    TimerEventMASK |= Order;        /////把需要执行的指令放进去，执行完之后最高位必须清零
    PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~PIT_TCTRL_TEN_MASK;
    PIT_BASE_PTR->CHANNEL[2].LDVAL = ms * bus_clk_khz - 1;         ///把时间装载进去
    PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK; 
    PIT_BASE_PTR->CHANNEL[2].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
    set_vector_handler(PIT2_VECTORn, EventTimerDelay_IRQHandler);
    NVIC_SetPriority(PIT2_IRQn, 0); //////最高级优先级，因为使用它一定是在摄像头处理中或者是不进行任何操作的时刻，而触发也是一样
    NVIC_EnableIRQ(PIT2_IRQn);   ////开启PIT2中断
    return 'T';
  }
  return 'F';
}

////////////////////////////以下用于延时与计算程序运行时间，使用PIT2定时器
void CommonDelay(uint32 ms) //使用PIT2延时必须先开事件控制函数
{
  uint32 cint = 0;
  cint = bus_clk_khz * ms;
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~( PIT_TCTRL_TEN_MASK );
  PIT_BASE_PTR->CHANNEL[2].LDVAL = cint;
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  //清中断标志位
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~( PIT_TCTRL_TEN_MASK );
  PIT_BASE_PTR->CHANNEL[2].TCTRL|= PIT_TCTRL_TEN_MASK;
  while(!(PIT_BASE_PTR->CHANNEL[2].TFLG & PIT_TFLG_TIF_MASK));
  return;
}

void TimeBegin(void)   //////////////用于程序计时
{
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
  PIT_BASE_PTR->CHANNEL[2].LDVAL = ~0;    //装入最大计数值
  PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;  //清中断标志位
  PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
  PIT_BASE_PTR->CHANNEL[2].TCTRL|= PIT_TCTRL_TEN_MASK;  //开启计数器
}



float TimeEnd(void)   //////////////用于程序计时
{
  uint32 a = 0;
  float  t = 0;
  a = (~0) - PIT_BASE_PTR->CHANNEL[2].CVAL;
  if(PIT_BASE_PTR->CHANNEL[2].TFLG & PIT_TFLG_TIF_MASK)
  {
    PIT_BASE_PTR->CHANNEL[2].TFLG |=  PIT_TFLG_TIF_MASK;
    PIT_BASE_PTR->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK); //禁止使能
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