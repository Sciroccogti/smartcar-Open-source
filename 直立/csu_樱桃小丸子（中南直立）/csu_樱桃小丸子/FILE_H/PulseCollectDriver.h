#define MotorFrequency  10000
#define MOTORPRECISON   1000

typedef struct{
  int16_t Rcount;
  int16_t Lcount;
}PulseCounttypedef;


uint8 MotorPwm_init(void);
void PWM_Set(int MotorR, int MotorL);
void QuadratureDecode_init(void);
void GetPulse(PulseCounttypedef *P);
void ADC_init(void);
uint16 BAT_AD(void);
