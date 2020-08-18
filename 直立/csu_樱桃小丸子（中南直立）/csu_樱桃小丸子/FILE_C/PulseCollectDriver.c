 /*  
 * @file       PulseCollectDriver.c
 * @brief      电机脉冲PWM底层驱动
 * @author     中南大学黄竞辉
 * @version    v1.0
 * @date       2016-12-13
 */

#include "GeneralDeclare.h"
#include "PulseCollectDriver.h"
#include "ProjectMath.h"


uint32 MotorMOD = 0;

uint8 MotorPwm_init(void)
{
  uint32 CLKHZ;
  uint16 mod;
  uint8  ps;
  uint16 cv;
  uint32 n;
  
  SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
  for(n = 4; n < 8; n++)
  {
    PORT_ISFR_REG(PORTD_BASE_PTR) = (1 << n);
    PORTD_BASE_PTR->PCR[n] = PORT_PCR_MUX(4);  //////4号功能FTM
  }
  
  CLKHZ = (bus_clk_khz * 1000);              //取得总线时钟,与PIT定时器相同，FTM也是总线时钟
  mod = (CLKHZ >> 16) / MotorFrequency;      //////
  ps = 0;
  while((mod >> ps) >= 1)
  {
    ps++;            ///////获取PS最小值  预分频因子最小，这样得到的计数值最精确
  }
  if(ps > 0x07) return 'F';
  mod = (CLKHZ >> ps) / MotorFrequency;                     ///////求取模值
  MotorMOD = mod;                                           //////记录
  cv = (0 * (mod + 1)) / MOTORPRECISON;                 ////占空比初始化为0
  
  FTM0_BASE_PTR->SC = (FTM_SC_PS(ps) | FTM_SC_CLKS(1));    /////设置FTM0时钟及预分频
  FTM0_BASE_PTR->MOD = mod;
  FTM0_BASE_PTR->CNTIN = 0;    ///////计数器初始化
  for(n = 4; n < 8; n++)
  {
    FTM_CnSC_REG(FTM0_BASE_PTR, n) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTM0_BASE_PTR, n) = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    FTM_CnV_REG(FTM0_BASE_PTR, n) = cv;
  }
  FTM0_BASE_PTR->CNT = 0;      /////装载
  
  
  //////FTM模块使能完毕
  PORTD_BASE_PTR->PCR[2] = PORT_PCR_MUX(1);
  PORTD_BASE_PTR->PCR[3] = PORT_PCR_MUX(1);
  PTD_BASE_PTR->PDDR |= (1 << 2);
  PTD_BASE_PTR->PDDR |= (1 << 3);
  PTD_BASE_PTR->PDOR |= (1 << 2);
  PTD_BASE_PTR->PDOR |= (1 << 3);  
  return 'T';
}

void QuadratureDecode_init(void)
{
  SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
  SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;    
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTA_BASE_PTR->PCR[12] = PORT_PCR_MUX(7);
  PORTA_BASE_PTR->PCR[13] = PORT_PCR_MUX(7);
  PORTB_BASE_PTR->PCR[18] = PORT_PCR_MUX(6);
  PORTB_BASE_PTR->PCR[19] = PORT_PCR_MUX(6);
  
  FTM1_BASE_PTR->MODE |= FTM_MODE_WPDIS_MASK;   /////写保护
  FTM2_BASE_PTR->MODE |= FTM_MODE_WPDIS_MASK;
  FTM1_BASE_PTR->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK; //////使能正交解码功能
  FTM2_BASE_PTR->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;
  FTM1_BASE_PTR->CNTIN = 0;
  FTM2_BASE_PTR->CNTIN = 0;
  FTM1_BASE_PTR->MOD = FTM_MOD_MOD_MASK;
  FTM2_BASE_PTR->MOD = FTM_MOD_MOD_MASK;
  FTM1_BASE_PTR->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
  FTM2_BASE_PTR->QDCTRL |= FTM_QDCTRL_QUADEN_MASK; 
  FTM1_BASE_PTR->MODE |= FTM_QDCTRL_QUADEN_MASK;
  FTM2_BASE_PTR->MODE |= FTM_QDCTRL_QUADEN_MASK;
  FTM1_BASE_PTR->CNT = 0;
  FTM2_BASE_PTR->CNT = 0;
  return;
}


float testpause = 0;
int16_t psuae = 0;

extern uint32 SendFloat[30];
void GetPulse(PulseCounttypedef *P)    /////获取脉冲数并清零
{
  P->Rcount = -FTM1_BASE_PTR->CNT;
  P->Lcount = FTM2_BASE_PTR->CNT;
  FTM1_BASE_PTR->CNT = 0;
  FTM2_BASE_PTR->CNT = 0;
}

void PWM_Set(int MotorR, int MotorL)  ////应该具有死区矫正 （100- 999）（-100- -999）
{                                      /////但是该矫正不应放置于这里，它应该存在于电机实际PI控制中
  uint16 cvr, cvl;
  MotorR = -MotorR;
  if(MotorR < 0 && MotorL < 0)
  {
    MotorR = ABS(MotorR);
    MotorL = ABS(MotorL);
    cvr = (MotorR * (MotorMOD + 1)) / MOTORPRECISON;
    cvl = (MotorL * (MotorMOD + 1)) / MOTORPRECISON;
    FTM_CnV_REG(FTM0_BASE_PTR, 7) = cvr;
    FTM_CnV_REG(FTM0_BASE_PTR, 6) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 5) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 4) = cvl;
  }
  else if(MotorR < 0 && MotorL >= 0)
  {
    MotorR = ABS(MotorR);
    cvr = (MotorR * (MotorMOD + 1)) / MOTORPRECISON;
    cvl = (MotorL * (MotorMOD + 1)) / MOTORPRECISON;
    FTM_CnV_REG(FTM0_BASE_PTR, 7) = cvr;
    FTM_CnV_REG(FTM0_BASE_PTR, 6) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 5) = cvl;
    FTM_CnV_REG(FTM0_BASE_PTR, 4) = 0;
  }
  else if(MotorR >= 0 && MotorL < 0)
  {
    MotorL = ABS(MotorL);
    cvr = (MotorR * (MotorMOD + 1)) / MOTORPRECISON;
    cvl = (MotorL * (MotorMOD + 1)) / MOTORPRECISON;
    FTM_CnV_REG(FTM0_BASE_PTR, 7) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 6) = cvr;
    FTM_CnV_REG(FTM0_BASE_PTR, 5) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 4) = cvl;    
  }
  else
  {
    cvr = (MotorR * (MotorMOD + 1)) / MOTORPRECISON;
    cvl = (MotorL * (MotorMOD + 1)) / MOTORPRECISON;
    FTM_CnV_REG(FTM0_BASE_PTR, 7) = 0;
    FTM_CnV_REG(FTM0_BASE_PTR, 6) = cvr;
    FTM_CnV_REG(FTM0_BASE_PTR, 5) = cvl;
    FTM_CnV_REG(FTM0_BASE_PTR, 4) = 0;    
  }
}

void ADC_init(void)
{
    SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );
    SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
    SIM_SOPT7 |= SIM_SOPT7_ADC1TRGSEL(0); 
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
    
    PORTE_BASE_PTR->ISFR |= (1 << 0);
    PORTE_BASE_PTR->PCR[0] = PORT_PCR_MUX(0);
    
    ADC_CFG1_REG(ADC1_BASE_PTR) |=(
                                     ADC_CFG1_ADIV(2)|     //4分频
                                     ADC_CFG1_MODE(3)|     //单通道,12位精度  0->8位, 1->12位, 2->10位, 3->16位
                                     ADC_CFG1_ADICLK(0)    //总线时钟
                                   );

    ADC_CFG1_REG(ADC1_BASE_PTR) &=~ ADC_CFG1_ADLSMP_MASK; //长采样时间关闭

    ADC_CFG2_REG(ADC1_BASE_PTR) |=(
                                      ADC_CFG2_ADHSC_MASK|  //最高转换速度
                                      ADC_CFG2_ADLSTS(3)    //采样时间
                                    );
}

uint16 BAT_AD(void)
{
    uint16 result = 0;
    ADC_SC1_REG(ADC1_BASE_PTR, 0) = ADC_SC1_ADCH(4);   //选择通道
    while((ADC_SC1_REG(ADC1_BASE_PTR,0) & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK); //等待转换完成
    result = ADC_R_REG(ADC1_BASE_PTR,0); //读取通道值
    ADC_SC1_REG(ADC1_BASE_PTR,0) &=~ ADC_SC1_COCO_MASK; //清除转换完成标志
    return result;
}
