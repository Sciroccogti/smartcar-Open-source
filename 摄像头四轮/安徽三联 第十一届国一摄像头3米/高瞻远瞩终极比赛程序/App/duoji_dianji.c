#include "include.h"
#include "common.h"
s16 speed_a,speed_b;



void duoji_dianji_Init(void)
{
//FTM_PWM_init(FTM1, CH0, 50, steer_mid);//���PWM��ʼ��
ftm_pwm_init(FTM1, FTM_CH0, 50, steer_mid);//steer_mid);

  ftm_pwm_init(FTM0, FTM_CH5, 10000,80000);  //���1--PWM��ʼ��
  ftm_pwm_init(FTM0, FTM_CH4, 10000,100000);  //���1--PWM��ʼ��
  
  ftm_pwm_init(FTM0, FTM_CH7, 10000,100000);  //���2--PWM��ʼ��
  ftm_pwm_init(FTM0, FTM_CH6, 10000,80000);  //���2--PWM��ʼ��
 /* 
  ftm_pwm_init(FTM0, FTM_CH5, 10000,75000);  //���1--PWM��ʼ��
  ftm_pwm_init(FTM0, FTM_CH4, 10000,100000);  //���1--PWM��ʼ��
  
  ftm_pwm_init(FTM0, FTM_CH7, 10000,75000);  //���2--PWM��ʼ��
  ftm_pwm_init(FTM0, FTM_CH6, 10000,100000);  //���2--PWM��ʼ��
  */
  gpio_init (PTD3 ,GPO,LOW);        //���1--ʹ��IO��ʼ��
  gpio_init (PTD8 ,GPO,LOW);        //���2--ʹ��IO��ʼ��

  //gpio_init (PTB23, GPI, 1u);  //up
}



void FTM_QUAD_init()
{
    /*�����˿�ʱ��*/
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

    /*ѡ��ܽŸ��ù���*/
   // PORTA_PCR12 = PORT_PCR_MUX(7);
   // PORTA_PCR13 = PORT_PCR_MUX(7);
    PORTA_PCR10 = PORT_PCR_MUX(6);
    PORTA_PCR11 = PORT_PCR_MUX(6);

    /*ʹ��FTM1��FTM2ʱ��*/
   // SIM_SCGC6|=SIM_SCGC6_FTM1_MASK;
    SIM_SCGC3|=SIM_SCGC3_FTM2_MASK;

  //  FTM1_MOD = 65535; //�ɸ�����Ҫ����
    FTM2_MOD = 65535;

  //  FTM1_CNTIN = 0;
    FTM2_CNTIN = 0;

  //  FTM1_MODE |= FTM_MODE_WPDIS_MASK; //��ֹд����
    FTM2_MODE |= FTM_MODE_WPDIS_MASK; //��ֹд����
  //  FTM1_MODE |= FTM_MODE_FTMEN_MASK; //FTMEN=1,�ر�TPM����ģʽ������FTM���й���
    FTM2_MODE |= FTM_MODE_FTMEN_MASK; //FTMEN=1,�ر�TPM����ģʽ������FTM���й���

  //  FTM1_QDCTRL &= ~FTM_QDCTRL_QUADMODE_MASK; //ѡ������ģʽΪA����B�����ģʽ
  //  FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK; //ʹ����������ģʽ
  //  FTM2_QDCTRL &= ~FTM_QDCTRL_QUADMODE_MASK; //ѡ������ģʽΪA����B�����ģʽ   0x8u  0x00001000ȡ���� 0x11110111
   FTM2_QDCTRL |= 0x08;//~0x00u; 
    
    FTM2_QDCTRL |= FTM_QDCTRL_QUADEN_MASK; //ʹ����������ģʽ
//QUADMODE=1;
 //   FTM1_SC |= FTM_SC_CLKS(3);  //ѡ���ⲿʱ��
//   FTM1_CONF |=FTM_CONF_BDMMODE(3); //�ɸ�����Ҫѡ��
    FTM2_SC |= FTM_SC_CLKS(3);
//   FTM2_CONF |=FTM_CONF_BDMMODE(3);

}


void get_maichong(void)
{
    speed_a=FTM2_CNT;
    FTM2_CNT=0;
    
    speed_b=lptmr_pulse_get();
    lptmr_pulse_clean();
    //lptmr_counter_clean();
    //LPT_INT_count=0;
}


void dianji_zhengzhuan_zuo(int zhankongbi)
{
  ftm_pwm_duty(FTM0, FTM_CH5,(100000-zhankongbi));  //���1--PWM��ʼ��  65000�� �ٷ�֮35��ռ�ձ�
  ftm_pwm_duty(FTM0, FTM_CH4, 100000);  //���1--PWM��ʼ��
}

void dianji_zhengzhuan_you(int zhankongbi)
{
  ftm_pwm_duty(FTM0, FTM_CH7,(100000-zhankongbi));  //���2--PWM��ʼ��
  ftm_pwm_duty(FTM0, FTM_CH6, 100000);  //���2--PWM��ʼ��
}

void dianji_fanzhuan_zuo(int zhankongbi)
{
  ftm_pwm_duty(FTM0, FTM_CH5, 100000);  //���1--PWM��ʼ��
  ftm_pwm_duty(FTM0, FTM_CH4, (100000-zhankongbi));  //���1--PWM��ʼ��
}

void dianji_fanzhuan_you(int zhankongbi)
{
  ftm_pwm_duty(FTM0, FTM_CH7, 100000);  //���2--PWM��ʼ��
  ftm_pwm_duty(FTM0, FTM_CH6, (100000-zhankongbi));  //���2--PWM��ʼ��
}


int zuo_MC_piancha,d_zuo_MC_piancha,zuo_last_MC;
int zuo_kp,zuo_kd;
int zuo_zhankongbi,last_zuo_zhankongbi;
void dianji_PID_zuo(int QWMC_zuo)
{
   zuo_MC_piancha=QWMC_zuo-speed_b;
   d_zuo_MC_piancha=zuo_MC_piancha-zuo_last_MC;
   zuo_last_MC=zuo_MC_piancha;
   zuo_zhankongbi=last_zuo_zhankongbi+zuo_kp*zuo_MC_piancha+zuo_kd*d_zuo_MC_piancha;
   if(zuo_zhankongbi>80000)
    zuo_zhankongbi=80000; 
   if(zuo_zhankongbi<0)
     zuo_zhankongbi=0; 
  // zuo_zhankongbi_queding=1000000-zuo_zhankongbi;
   last_zuo_zhankongbi=zuo_zhankongbi;
   
   
   if(zuo_MC_piancha<-15)
    {dianji_fanzhuan_zuo(5000);} 
    else 
   {dianji_zhengzhuan_zuo(zuo_zhankongbi);}
}



int you_MC_piancha,d_you_MC_piancha,you_last_MC;
int you_kp,you_kd;
int you_zhankongbi,last_you_zhankongbi;

void dianji_PID_you(int QWMC_you)
{
   you_MC_piancha=QWMC_you-speed_a;
   d_you_MC_piancha=you_MC_piancha-you_last_MC;
   you_last_MC=you_MC_piancha;
   you_zhankongbi=last_you_zhankongbi+you_kp*you_MC_piancha+you_kd*d_you_MC_piancha;
   if(you_zhankongbi>80000)
    you_zhankongbi=80000; 
   if(you_zhankongbi<0)
     you_zhankongbi=0; 
  // zuo_zhankongbi_queding=1000000-zuo_zhankongbi;
   last_you_zhankongbi=you_zhankongbi;
   
   if(you_MC_piancha<-15)
   {dianji_fanzhuan_you(5000);}
   else
   {dianji_zhengzhuan_you(you_zhankongbi);}
}

void dianji_canshu_init(void)
{
zuo_MC_piancha=0;
d_zuo_MC_piancha=0;
zuo_last_MC=0;
zuo_kp=500;
zuo_kd=200;
zuo_zhankongbi=0;
last_zuo_zhankongbi=0;

you_MC_piancha=0;
d_you_MC_piancha=0;
you_last_MC=0;
you_kp=500;
you_kd=200;
you_zhankongbi=0;
last_you_zhankongbi=0;

}
