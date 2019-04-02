/******************************************/
/*
  1�����С����ֵʱ���Ҵ��
  2�����������ֵʱ������
  3�����ٰ���ʵ�ʵ�������ֵ��ٶȲ�ֵ16PI����
  4�����㹫ʽ  ����Ŀ���ٶȼ���=�����ٶ�*(1+0.8*tan(���ֿɴ�����Ƕ�*(ʵ�ʶ�����pwm-�����ֱʱ��PWM���)/���PWM����ֵ�����ֵ�Ĳ�ֵ))��������ʱ�Ĳ��������
  5��tanΪ�����ȸ�����
*/
/******************************************/
#include "chasu.h"
#include "math.h"

void CHASU()
{
    if(angle_pwm>=angle_pwm_r && angle_pwm=<angle_pwm_l)  //�������ĽǶȽ�Сʱ �����в���
    {
        Speed_R=Speed_L=Speed_duty ;
    }
    else if(angle_pwm>angle_pwm_l)    //����Ĵ�Ǵ�������ֵʱΪ����ת  ��ʱ�Ҳ�Ϊ����  ���Ϊ����
    {
        Speed_L=Speed_duty ;
        Speed_R=Speed_L_Now*(1+0.8*tan(angle_max*(angle_pwm-angle_pwm_center)/angle_pwm_error)) ;
		                                              a=(int)(angle_max*(angle_pwm-angle_pwm_center)/angle_pwm_error) ;
													  error= shuzu [a]*p;
												r=	   error+setspeed;
												L=   setspeed;
    }
    else if(angle_pwm<angle_pwm_r)    //�����Ĵ��С����ֵʱΪ����ת  ��ʱ�Ҳ�Ϊ����  ���Ϊ����
    {
        Speed_R=Speed_duty ;
        Speed_L=Speed_R_Now*(1+0.8*tan(angle_max*(angle_pwm_center-angle_pwm)/angle_pwm_error)) ;
    }
}