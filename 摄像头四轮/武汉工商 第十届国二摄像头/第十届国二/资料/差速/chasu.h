/******************************************/
/*
  1�����С����ֵʱ���Ҵ��
  2�����������ֵʱ������
  3�����ٰ���ʵ�ʵ�������ֵ��ٶȲ�ֵ16PI����
  4�����㹫ʽ  ����Ŀ���ٶȼ���=�����ٶ�*(1+0.8*tan(���ֿɴ�����Ƕ�*(ʵ�ʶ�����pwm-�����ֱʱ��PWM���)/���PWM����ֵ�����ֵ�Ĳ�ֵ))��������ʱ�Ĳ��������
  5��tanΪ�����ȸ�����
*/
/******************************************/
#ifndef _CHASU_H
#define _CHASU_H


#define Speed_duty              //�趨�ٶ�
#define Speed_R                 //���ֵ��趨�ٶ�
#define Speed_L                 //���ֵ��趨�ٶ�
#define Speed_R_Now             //���ֵ�ʵ���ٶ�
#define Speed_L_Now             //���ֵ�ʵ���ٶ�
#define angle_max             45      //���ֿɴ�����Ƕ�
#define angle_pwm_center      1482    //�����ֱʱ��PWM���
#define angle_pwm_error       200     //���PWM����ֵ�����ֵ�Ĳ�ֵ
#define angle_pwm_l           1502    //������ʱ���ֿ�ʼ���ٵķ�ֵ
#define angle_pwm_r           1462    //���Ҵ��ʱ���ֿ�ʼ���ٵķ�ֵ

void CHASU(void);              //��ʼ������

#endif  /*  _CHASU_H  */