/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ���ѧ��̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       init.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       
 * author      
 */
#include "common.h"
#include "include.h"
#include  "init.h"
#include  "PID.h"

#define   angle_max        45

float  KKi;
float  KKp;
float  KKd;

float   P_1; 
float   P_2; 
float   P_3; 
float   D_1;
float   D_2; 
float   D_3;

float  S_1;
float  S_2;
float  S_3;

float   DP_1 ;       // 6         �����
float   DD_1 ;       // 4  
float   DS_1 ;       //1.2
float   DP_2 ;       //7        С���
float   DD_2 ;       // 5
float   DS_2 ;       //1.2
float   DP_3 ;       //2       ֱ��
float   DD_3 ;       //1
float   DS_3 ;       // 0.5
     
float    CS_P_1,CS_P_2;
float speed_error[45]={   0,   1.0139,  1.0279, 1.0419, 1.0559, 1.0700, 1.0840,    //7
                      1.0981, 1.1124, 1.1266, 1.1410, 1.1554, 1.1700, 1.1846,     //7
                      1.1994, 1.2143, 1.2293, 1.2445, 1.2599, 1.2754, 1.2911,     //7
                      1.3070, 1.3232, 1.3395, 1.3561, 1.3730, 1.4076, 1.4253,     //7
                      1.4434, 1.4618, 1.4806, 1.4998, 1.5195, 1.5396, 1.5601,     //7
                      1.5812, 1.6028, 1.6249, 1.6477, 1.6712, 1.6953, 1.7203,     //7
                      1.7460, 1.7724, 1.800};   //3

int32  val_z=0, val_y=0;  
extern uint8 wandao_flag,inwandao_flag,s_zhidao_flag,Mode2_flag;
extern int8 tingche_flag;
void FTM_init()
{  
    //����
    gpio_init(PTD0,GPI,0);
    port_init_NoALT(PTD0,PULLUP);
    gpio_init(PTD1,GPI,0);
    port_init_NoALT(PTD1,PULLUP);
    //����
    
    //һ��ͣ��
    gpio_init(PTD6,GPI,0);
    port_init_NoALT(PTD6,PULLDOWN);
    //һ��ͣ��
    
    
    //�����
    gpio_init(PTD2,GPI,0);
    port_init_NoALT(PTD2,PULLDOWN);
    gpio_init(PTD3,GPI,0);
    port_init_NoALT(PTD3,PULLDOWN);
    //�����
    
   
    //���뿪��   ��������
    gpio_init(PTA17,GPI,0);
    port_init_NoALT(PTA17,PULLUP);
    
    gpio_init(PTA16,GPI,0);
    port_init_NoALT(PTA16,PULLUP);
    
    gpio_init(PTA15,GPI,0);
    port_init_NoALT(PTA15,PULLUP);
    
    gpio_init(PTA14,GPI,0);
    port_init_NoALT(PTA14,PULLUP);
    
    gpio_init(PTA13,GPI,0);
    port_init_NoALT(PTA13,PULLUP);
    
    gpio_init(PTA12,GPI,0);
    port_init_NoALT(PTA12,PULLUP);
    //���뿪��   ��������

    
    //������
    gpio_init(PTD15,GPO,0);
    //������
   
    adc_init(Gyro1);         //�Ǽ��ٶ�  Angular1
    adc_init (ZOUT); 

    ftm_quad_init(FTM1);
    ftm_quad_init(FTM2);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ, DJ_cen);     //��ʼ�� ��� PWM        
    ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);         //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);         //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);         //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);         //��ʼ�� ��� PWM  
  
}
  
void  DangWei_()
{
  if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //���ٵ���
  {
      P_1=3.1;   //4   3.2
      P_2=2.2;   //2
      P_3=3.6;   //4
      D_1=15;  //15  
      D_2=20;    //20
      D_3=25;    //20
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       �����
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.5   ;       //7        С���
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        ֱ��
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      
      CS_P_1=3.8;      //���ٱ� 
      CS_P_2=3.8;      //���ٱ� 
  } 
  if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //�Т��ٵ�
  {
      P_1=3.1;   //4
      P_2=2.2;   //2
      P_3=3.5;   //4
      D_1=15;    
      D_2=15;    
      D_3=20;
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.2   ;       // 6       �����
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.2  ;       //7        С���
      DD_2=2.5  ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2.5   ;       //2        ֱ��
      DD_3=1.5   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      CS_P_1=5;      //���ٱ� 
      CS_P_2=5;      //���ٱ� 
  }
  if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //�Т��ٵ�
  {
      P_1=3.5;   //4
      P_2=2.2;   //2
      P_3=3.5;   //4
      D_1=15;    
      D_2=15;  //  
      D_3=20;
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4   ;       // 6       �����
      DD_1=2   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4   ;       //7        С���
      DD_2=2   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        ֱ��
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      CS_P_1=5;      //���ٱ� 
      CS_P_2=5;      //���ٱ� 
  }
  if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //���ٵ�
  {
      P_1=3;   //3  6��30�Ų�ֵ
      P_2=2;   //1  6��30�Ų�ֵ
      P_3=3;   //4  6��30�Ų�ֵ
      D_1=6;    
      D_2=1;    
      D_3=15;
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=5   ;       // 6       �����
      DD_1=2   ;       // 4  
      DS_1=1.2 ;      //1.2
        
      DP_2=6   ;       //7        С���
      DD_2=2   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        ֱ��
      DD_3=1   ;      //1
      DS_3=0.5 ;      // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      CS_P_1=5;      //���ٱ� 
      CS_P_2=5;      //���ٱ� 
  }
  if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)  //���ٵ���
  {
      P_1=3.5;   //4   3.2
      P_2=2.3;   //2
      P_3=3.0;   //4
      D_1=15;  //15  
      D_2=25;    //20
      D_3=20;    //20
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       �����
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;       //1.2
        
      DP_2=4.5   ;       //7        С���
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        ֱ��
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      
      CS_P_1=5.3;      //���ٱ� 
      CS_P_2=5.3;      //���ٱ� 
  } 
  if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //ԭ���ٵ���
  {
      P_1=3.1;   //4   3.2
      P_2=2.2;   //2
      P_3=3.3;   //4
      D_1=15;    //15  
      D_2=20;    //20
      D_3=20;    //20
      S_1=0.5;    //����   
      S_2=0.5;
      S_3=0.5;
      
      DP_1=4.5   ;       // 6       �����
      DD_1=2.5   ;       // 4  
      DS_1=1.2 ;        //1.2
        
      DP_2=4.5   ;       //7        С���
      DD_2=2.5   ;       // 5
      DS_2=1.2 ;       //1.2
        
      DP_3=2   ;       //2        ֱ��
      DD_3=1   ;       //1
      DS_3=0.5 ;       // 0.5
      
      KKi=7;          //�ٶ�
      KKp=6;          //�ٶ�
      KKd=3.5;        //�ٶ�
      
      
      CS_P_1=3.8;      //���ٱ� 
      CS_P_2=3.8;      //���ٱ� 
  }
}
/************************************************************
 *  @brief        ���ٺ���
     v5.0
 *   @note      
 ***********************************************************/
int8 num_00,num_11,flag_00;
extern int8  ZJ_Process_flag;
void get_speed()   //
{
    val_z = ftm_quad_get(FTM1);          //��ȡFTM �������� ��������(������ʾ������)
    val_y = ftm_quad_get(FTM2);          //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM1); 
    ftm_quad_clean(FTM2);  
    if(val_z>=0)
    {
       val_z=val_z;
    }
    else
    {
        val_z=-val_z;
    }
    if(val_y>=0)
    {
        val_y=val_y;
    }
    else
    {
        val_y=-val_y;
    }
/*�ߢ�*/if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //���ٵ���
    {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 42 ;           //��36
                       num_11= 42 ;           //��40
                  }
                  else if((val_z+val_y)/2>=250)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //��38
                       num_11= 38 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //��38
                       num_11= 36 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 27 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
      }
/*�Т�*/if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //�Т��ٵ�
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 39 ;           //��36
                       num_11= 39 ;           //��40
                  }
                  else if((val_z+val_y)/2>=250)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 35 ;           //��38
                       num_11= 35 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 33 ;           //��38
                       num_11= 33 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 26 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
      }
/*�Т�*/if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //�Т��ٵ�
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>310)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 40 ;           //��36
                       num_11= 40 ;           //��40
                  }
                  else if((val_z+val_y)/2>=250)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 35 ;           //��38
                       num_11= 35 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 33 ;           //��38
                       num_11= 33 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 27 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
      }
/*��*/if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //���ٵ�
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 39 ;           //��36
                       num_11= 39 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //��38
                       num_11= 36 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 27 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
     }
/*�ߢ�*/if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)  //���ٵ���
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 40 ;           //��36
                       num_11= 40 ;           //��40
                  }
                  else if((val_z+val_y)/2>=250)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //��38
                       num_11= 38 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //��38
                       num_11= 36 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 27 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
      }
/*�ߢ�*/if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)
      {
              if(ZJ_Process_flag&&flag_00==0)
              {
                  if((val_z+val_y)/2>330)  //�����ٶ�2.5M 160
                  {
                       flag_00=1;
                       num_00= 41 ;           //��36
                       num_11= 41 ;           //��40
                  }
                  else if((val_z+val_y)/2>=250)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 38 ;           //��38
                       num_11= 38 ;           //��40
                  }
                  else if((val_z+val_y)/2>=200)  //�����ٶ�2M 160
                  {
                       flag_00=1;
                       num_00= 36 ;           //��38
                       num_11= 36 ;           //��40
                  }
                  else if((val_z+val_y)/2>=150)  //����С��1.5M 160
                  {
                       flag_00=1;
                       num_00= 30 ;         //��34
                       num_11= 30 ;         //��35
                  }
                  else if((val_z+val_y)/2>=0)  //�ٶ�С��1.5M  160
                  {
                       flag_00=1;
                       num_00= 26 ;         //��30
                       num_11= 27 ;         //��31
                  }
                  else
                  {
                    
                  }
              }
      }
      if(val_y==0&&val_z==0)
      {
        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
      }
}
//�����ֵ��   1320       
//�����  :    1530      210
//�Ҵ�����     1110     -210
int32   error_0;
uint32  PWM_OUT_1, PWM_OUT_2;
int32   angle,     Speed_Set;    //�����Ҹ�  
int16    a;
extern   int8 shizhi_flag;
void CS_HS()
{
  if(angle<=10&&angle>=-10)   //ֱ��
  {
      PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
      PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
  }
  else if(angle>10)   //zuo ��
  {
     a=(int16)(angle_max*(angle)*1.0/(DJ_zuo-DJ_cen))-1 ;  //angle_max   45
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(int32)(speed_error[a]*CS_P_1*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-error_0/2,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set+(int32)(error_0*1.5),val_y);  
  }
  else if(angle<-10)   //you ��
  {
     a=(int16)(angle_max*(-angle)*1.0/(DJ_cen-DJ_you))-1 ;
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_2*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set+(int32)(error_0*1.5),val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-error_0/2,val_y);  
  }
}

//8��
void CS_HS_1()
{
  if(angle<=25&&angle>=-25)   //ֱ��
  {
      PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
      PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
  }
  else if(angle>25)   //zuo ��
  {
     a=(int16)(angle_max*(angle)*1.0/(DJ_zuo-DJ_cen))-1 ;  //angle_max   45
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(int32)(speed_error[a]*CS_P_1*10);
     if(Speed_Set-error_0/2<100)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set-error_0/2,val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set+(int32)(error_0*1.5),val_y);  
  }
  else if(angle<-25)   //you ��
  {
     a=(int16)(angle_max*(-angle)*1.0/(DJ_cen-DJ_you))-1 ;
     if(a>45)  a=0;
     if(a<0)   a=0;
     error_0=(uint32)(speed_error[a]*CS_P_2*10);
     if(Speed_Set-error_0/2<0)  Speed_Set=error_0/2;
     PWM_OUT_1=(uint32)speed_pid_1(Speed_Set+(int32)(error_0*1.5),val_z);  
     PWM_OUT_2=(uint32)speed_pid_2(Speed_Set-error_0/2,val_y);  
  }
}

////////////////////////////////////////////////////
int8 Cont_1=0,Trg_1=0,ReadData_1=0,count_flag_1=0,qipao_flag_0=1;

void  JianChe()    //5MS���һ�Σ������μ쳵�������źţ�������ߵ�ƽ��������һ������
{
  
  if(PTD2_IN&&PTD3_IN)
  {
      qipao_flag_0=0;    //��⵽�����ź�
  }
  else
  {
      qipao_flag_0=1;
  }
                                           //û��    ��һ�μ��   �ڶ��μ��    �����μ��     û��   �ڶ���û��
   ReadData_1=0x01^qipao_flag_0;           // 0       1            1             1              0      0
   Trg_1=ReadData_1&(ReadData_1^Cont_1);   // 0       1            0             0              0      0 
   Cont_1=ReadData_1;                      // 0       1            1             1              0      0
   if(Trg_1==1)
   {
        count_flag_1= count_flag_1+1;  
   }
}

////////////////////////////////////////////////////


extern int8    y_flag,  z_flag;
extern int32   angle_pwm;  
extern int8    zj_flag,zj_flag_1,bridge_flag;
extern float  error_4;
int8  FLAG_Count=0, FLAG_Count_1=0, count_000=0,qipao_kaishi=0;

int16  num_flag;
void Speed_Contorl()
{
     qipao_kaishi=0;
     angle=angle_pwm-DJ_cen;
     if(tingche_flag==1)    
     {
            FLAG_Count=1;
            PWM_OUT_2=0;
            PWM_OUT_1=0;
            ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
            ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
            ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);  
     }
     else   
     {
/*�ߢ�*/if(PTA17_IN==0&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //���ٵ���
       {
                  // Site_t site = {0,110};   //x = 10 ,y = 20
                 //  LCD_str(site,"GS", BLUE,RED);
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<200&&val_y<200&&(-10<angle&&angle<10))               //���ܼ���
                       {
                           Speed_Set=500;  
                       }
                       else if(val_z<280&&val_y<280&&(-50<angle&&angle<50))               //���ܼ���
                       {
                           Speed_Set=400;  
                       }
                       else if((val_z<270||val_y<270)&&(angle>50||angle<-50))      //���
                       {
                           Speed_Set=350;//250; 
                       }

                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //���  ����
                       {
                           Speed_Set=50;  //����ٶ� 
                       } 
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=250;  //����ٶ� 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=200;  //����ٶ� 
                       }
                       else                                                         //����
                       {
                           Speed_Set=350; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=150;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>5)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }                       }
                          else    CS_HS();
                       }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=180;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*�Т�*/if(PTA16_IN==0&&PTA17_IN==1&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)  //�Т��ٵ�
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag)
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //���ܼ���
                       {
                           Speed_Set=450;  
                       }
                       else if((val_z>250||val_y>250)&&(angle>20||angle<-20))       //����
                       {
                           Speed_Set=200;//150; 
                       }
                       else if((val_z<230||val_y<230)&&(angle>50||angle<-50))      //���
                       {
                           Speed_Set=260;//250; 
                       }
                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //���  ����
                       {
                           Speed_Set=50;  //����ٶ� 
                       }
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=250;  //����ٶ� 
                       }
                       else                                                         //����
                       {
                           Speed_Set=320; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=150;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>3)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }             
                       }
                           else    CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           Speed_Set=300;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=220;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
               /*     else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }  */
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*�Т�*/if(PTA15_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA14_IN==1&&PTA13_IN==1)   //�Т��ٵ�
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //���ܼ���
                       {
                           Speed_Set=450;  
                       }
                       else if((val_z>250||val_y>250)&&(angle>20||angle<-20))       //����
                       {
                           Speed_Set=200;//150; 
                       }
                       else if((val_z<230||val_y<230)&&(angle>50||angle<-50))      //���
                       {
                           Speed_Set=260;//250; 
                       }
                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //���  ����
                       {
                           Speed_Set=50;  //����ٶ� 
                       }
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=250;  //����ٶ� 
                       }
                       else                                                         //����
                       {
                           Speed_Set=320; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=100;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                           num_flag++;
                           if(num_flag>3)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else   CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;//250 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*��*/if(PTA14_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA13_IN==1)   //���ٵ�
     {
                   if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag)
                     {
                       if(val_z<150&&val_y<150&&(-10<angle&&angle<10))               //���ܼ���
                       {
                           Speed_Set=420;  
                       }
                       else if((val_z<200||val_y<200)&&(angle>50||angle<-50))      //���
                       {
                           Speed_Set=230;//250; 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=150;  //����ٶ� 
                       }
                       else                                                         //����
                       {
                           Speed_Set=270; 
                       }   
                       if(bridge_flag==1)
                       {
                           num_flag++;
                           if(num_flag>5)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else   CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           Speed_Set=310;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=250;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                    //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;//ͣ��ǰ���� 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=1000;   
                          PWM_OUT_2=9000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=9000;  
                          PWM_OUT_2=1000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }
     }
     
/*�ߢ�*/if(PTA13_IN==0&&PTA17_IN==1&&PTA16_IN==1&&PTA15_IN==1&&PTA14_IN==1)
       {
                   if(!shizhi_flag)
                   {
                     if((angle_pwm<DJ_zuo&&angle_pwm>DJ_you)&&! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<300&&val_y<300&&(-25<angle&&angle<25))               //���ܼ���
                       {  
                             Speed_Set=550;  
                       }
                       else if(val_z<350&&val_y<350&&(-25<angle&&angle<25))               //���ܼ���
                       {
                           Speed_Set=350;  
                       }
                       else if(angle>80||angle<-80)             //���
                       {
                           Speed_Set=300;//250;380 
                       }
                       else if((val_z>290||val_y>290)&&(error_4>10.0||error_4<-10.0)&&(angle<80&&angle>-80))      //���  ����
                       {
                           Speed_Set=50;  //����ٶ� 
                       } 
                       else if((val_z<290||val_y<290)&&(error_4>10.0||error_4<-10.0)&&(angle<80&&angle>-80))      //���  ����
                       {
                           Speed_Set=220;  //����ٶ� 
                       } 
                       else                                                         //����
                       {
                           Speed_Set=330; 
                       }
                       if(bridge_flag==1)                    
                       {
                           Speed_Set=100;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           num_flag++;
                           if(num_flag>8)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else    CS_HS_1();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                           Speed_Set=320;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=250;       //ͣ��ǰ���� 
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
                    else if(angle_pwm>=DJ_zuo-0)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                    else if(angle_pwm<=DJ_you+0)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
     }
/*�ߢ�*/if(PTA17_IN==0&&PTA16_IN==0&&PTA15_IN==1&&PTA14_IN==1&&PTA13_IN==1)
        {
                    if(!shizhi_flag)
                   {
                     if(! ZJ_Process_flag&&!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))
                     {
                       if(val_z<200&&val_y<200&&(-10<angle&&angle<10))               //���ܼ���
                       {
                           //Speed_Set=500;  
                             Speed_Set=550;  
                       }
                       else if(val_z<280&&val_y<280&&(-50<angle&&angle<50))               //���ܼ���
                       {
                           Speed_Set=400;  
                       }
                       else if((val_z<270||val_y<270)&&(angle>50||angle<-50))      //���
                       {
                           Speed_Set=380;//250;380 
                       }

                       else if((val_z>300&&val_y>300)&&(error_4>10.0||error_4<-10.0))      //���  ����
                       {
                           Speed_Set=50;  //����ٶ� 
                       } 
                       else if((val_z>260&&val_y>260)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=250;  //����ٶ� 
                       }
                       else if((val_z>230&&val_y>230)&&(error_4>10.0||error_4<-10.0))      //���   ����
                       {
                           Speed_Set=200;  //����ٶ� 
                       }
                       else                                                         //����
                       {
                           Speed_Set=360; 
                       }
                       if(bridge_flag==1)
                       {
                           Speed_Set=120;
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           num_flag++;
                           if(num_flag>8)  
                           { 
                             bridge_flag=0;
                             num_flag=0;
                           }
                       }
                       else    CS_HS();
                     }
                     else if(ZJ_Process_flag&&zj_flag)    //����ת9000  �ҷ�ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=9000;  
                        PWM_OUT_2=4000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                     else if(ZJ_Process_flag&&zj_flag_1)    //����ת9000  ��ת4000  ��ֱ��
                     {
                       
                        PWM_OUT_1=4000;  
                        PWM_OUT_2=9000;
                        ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);
                        ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                     }
                     else 
                     {
                           if(!(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)))   
                           Speed_Set=330;//250 
                           PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                           PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                           ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                     }
                  }
                  if(shizhi_flag)
                  {
                       Speed_Set=300;//250 
                       PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                       PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y); 
                  }
                  if(!ZJ_Process_flag)
                  {
                     //����ͣ������
                    if(count_flag_1==1&&PTD2_IN&&PTD3_IN)
                    { 
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(count_flag_1==2&&(PTD2_IN==1||PTD3_IN==1)&&PTA12_IN==1)
                    {
                          Speed_Set=200;     //ͣ��ǰ����
                          PWM_OUT_1=(uint32)speed_pid_1(Speed_Set,val_z);  
                          PWM_OUT_2=(uint32)speed_pid_2(Speed_Set,val_y);  
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }   
                    else if(count_flag_1==2&&(PTD2_IN==0&&PTD3_IN==0)&&PTA12_IN==1)
                    { 
                          count_000=count_000+1;
                        
                          if((val_z+val_y)/2<50)
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          else 
                          {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 8000);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 8000);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                          }
                          if(count_000>50) count_000=0;
                    }
                    else if(count_flag_1==3&&PTA12_IN==1)
                    {
                              ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                              ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);  //��
                              ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    //����ͣ������
                    else if(angle_pwm>=DJ_zuo-0&&val_z>200&&val_y>200)              //����ת9000  ��ת4000
                    {
                          PWM_OUT_1=2000;   
                          PWM_OUT_2=8000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, PWM_OUT_1);  //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, 0);
                    }
                    else if(angle_pwm<=DJ_you+0&&val_z>200&&val_y>200)              //����ת9000  �ҷ�ת4000
                    {
                          PWM_OUT_1=8000;  
                          PWM_OUT_2=2000;
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, PWM_OUT_2);   //��
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);   //��
                        
                    }
                    else
                    {
                          ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM, PWM_OUT_2);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM, 0);
                          ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM, PWM_OUT_1);
                    }
                 }    
        }
   }
}

/************************�����ٶȿ���**************************/


//-------------------------------------------------------------------------*
//@brief         ʾ��������                                                *
//name��  FreeCars��λ��ʾ��������                                         *
//author��                                                          *
//Date:   2015-6-26                                                        *
//��Ȩ��  �˴���ΪFreeCars��λ��Э����룬��������ʹ�ã�                   *
//        Ҳ����������ҵ��;�����뱣���˶����֣�                           *                                                         *
//-------------------------------------------------------------------------*

#define FreeCarsUARTPort  UART3
#define UartDataNum 17
uint8 uSendBuf[UartDataNum*2]={0};
uint8 FreeCarsDataNum=UartDataNum*2;
/*
��ĳ��ͨ���������������
chanel��ͨ��
data  ������-32768~32767
*/
void push(uint8 chanel,uint16 data)
{
    uSendBuf[chanel*2]=data/256;
    uSendBuf[chanel*2+1]=data%256;
}
/*
��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
����ʱ����㿴�����ĵ�
*/
void sendDataToScope(void)
{
  
    uint8 i,sum=0; 
    //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
    uart_putchar (UART3,251);
    uart_putchar (UART3,109);
    uart_putchar (UART3,37);
    sum+=(251);      //ȫ�����ݼ���У��
    sum+=(109);
    sum+=(37);
    for(i=0;i<FreeCarsDataNum;i++)
    {
       uart_putchar(UART3,uSendBuf[i]);
       sum+=uSendBuf[i];         //ȫ�����ݼ���У��
    }
    uart_putchar (UART3,sum);
}
extern int8 z_z,y_z,D1,z_flag;
extern int16 gyro1; 
void send_to_scope()
{
   //push(0,(uint16)((val_z+val_y)/2));
  // push(1,(uint16)z_z);
  // push(2,(uint16)num_00);
  // push(3,(uint16)num_11);

   // push(4,(uint16)y_z);
   //  push(5,bridge_flag);
   //  push(7,gyro1);
   
   //  sendDataToScope();
}
/*******************���Ϸ��ͺ���**********************************/


/************************************************************
 *  @brief        PID����
     v5.0
 *   @note      
***********************************************************/
int32 EE0=0;  //��ǰ��� 
int32 EE1=0;  //ǰ�����
int32 EE2=0;  //ǰǰ�ε����
int32 IncPid=0;
float sp=0;
float speed_pid_1(int32 speed1,int32 speed2 ) //speed1=�趨�������ٶȣ�speed2=�ɼ������ķ����ٶ�
{ 

   EE2=EE1;   //�洢��ǰ�������´μ���
   EE1=EE0; 
   EE0=speed1-speed2; //�������� �����ٶ�ƫ��
   IncPid=(int32)(KKi*EE0+KKp*(EE0-EE1)+KKd*(EE0-2*EE1+EE2));   
   sp=sp+IncPid; 
   if(sp>=9500)  
    sp=9500;
   if(sp<0)
    sp=0; //�˴���0���ᵼ���쳣ͣ��
   return sp ;                                        
}
int32 ee0=0;  //��ǰ��� 
int32 ee1=0;  //ǰ�����
int32 ee2=0;  //ǰǰ�ε����
int32 incpid=0;
float SP=0;
float speed_pid_2(int32 speed1,int32 speed2 ) //speed1=�趨�������ٶȣ�speed2=�ɼ������ķ����ٶ�
 { 
   ee2=ee1;   //�洢��ǰ�������´μ���
   ee1=ee0; 
   ee0=speed1-speed2; //�������� �����ٶ�ƫ��
   incpid=(int32)(KKi*ee0+KKp*(ee0-ee1)+KKd*(ee0-2*ee1+ee2));   
   SP=SP+incpid; 
   if(SP>=9500)  
    SP=9500;
   if(SP<0)
    SP=0; //�˴���0���ᵼ���쳣ͣ��
   return SP ;                                        
 }
