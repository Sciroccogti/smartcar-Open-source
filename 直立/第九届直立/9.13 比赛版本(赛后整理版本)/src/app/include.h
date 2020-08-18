#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"
#include  "define.h"

/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵�����û��Զ����ͷ�ļ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
#include  "gpio.h"      //IO�ڲ���
#include  "uart.h"      //����
#include  "adc.h"       //ADCģ��
#include  "FTM.h"       //FTMģ�飨FTM0��������� / ͨ�� /PWM     FTM1��2���������� / ͨ�� /PWM ��
#include  "PIT.h"       //�����жϼ�ʱ��
#include  "lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "exti.h"      //EXTI�ⲿGPIO�ж�
#include  "arm_math.h"  //DSP��
#include  "delay.h"
#include  "OV7725.h"
#include  "dma.h"
#include  "ff.h"
#include  "flash.h"

#include  "Car_init.h"
#include  "LCD.h"
#include  "IIC.h"
#include  "ISR_fun.h"
#include  "About_Image.h"
#include  "Startline_Ren_Block.h"
#include  "Process.h"
#include  "SD_SendDriver.h"
#include  "SmallS_Cross.h"


/*************************************************************************
*  ģ�����ƣ��ṹ��ͱ���ģ��
*  ����˵����Include �û��Զ���Ľṹ��ͱ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
struct CarAngle
{
    //���ٶȲŵõ�ֵ��a_sinΪ����z����ٶȣ���sinֵ
    float a_sin;
    float a_cos;
    //�����ǲɼ��Ľ��ٶ�ֵ
    float Gyro;   			
    float speed;
    //����ĽǶȣ�m_angleΪmma845x��õĽǶȣ�G_angleΪ�����ǻ��ֳ����ĽǶ�
    float m_angle;
    float G_angle;
    float OFFSET;//OFFSET_angleΪ��ֹʱ�ĽǶ�important    				
    //ƽ�ⲿ��PIDֵ
    float P;
    float D;   			
    float PWM;    			
};
struct CarSpeed
{
    //��λ
    unsigned char Mode;
    //�ٶȲ��֣���������ٶ�
    int L_5ms;
    int R_5ms;
    int L_100ms;
    int R_100ms;
    float Car;
    float Goal;
    float Goal_init;
    //�ٶȲ��֣�pid����
    float P;              //�ٶȿ���P����
    float I;              //�ٶȿ���I����
    float PWM_Per;  	//�ٶ�ÿ������ֵ
    float I_Error_Start;
    float I_Limit_PWM_max;
    float I_Limit_PWM_min;
    float PWM_Integral;	//�ٶȸ���PWM��ֵ���洢�Ļ���
    float PWM;        	//�ٶȸ���PWM��ֵ    			
};
struct CarDirection
{
    //���������ǵ�ֵ
    float Gyro;
    //pid����
    float P;  //*****************dir����P����**********************
    float D;	 //*****************dir����D����**********************	
    float PWM_Per;  		//����ÿ������ֵ
    float PWM;
    unsigned char output_enable;
};
struct CarStartLine
{
    unsigned char enable;//ʹ��
    unsigned char flag; //�����߱�־
    unsigned int time_record; 
    unsigned int Delaytime;
};
struct CarRen
{
    unsigned char enable;
    unsigned char Multiple_enable;
    int record_time;
    int Delaytime1;
    int Delaytime2;
    unsigned char flag; //�����߱�־
    unsigned int Loaction;//�������
    int Strength;
    float Angle_OFFSET[2];
};
struct CarBlock
{ 
    unsigned char enable;
    unsigned char flag;//�ϰ��ı�־
    int time;
    int delaytime;
    int Offset;
    int Offset_Init;
};


/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵����Include �û��Զ����ȫ�ֱ�������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
extern struct CarAngle Angle;
extern struct CarSpeed Speed;
extern struct CarDirection Direction;
extern struct CarStartLine StartLine;
extern struct CarRen Ren;
extern struct CarBlock Block;
extern long int Time_1ms;





/***************** ucos ר�� *****************/
#define USOC_EN     0u      //0Ϊ��ֹuC/OS������0������uC/OS
#if USOC_EN > 0u
#include  "ucos_ii.h"  		//uC/OS-IIϵͳ����ͷ�ļ�
#include  "BSP.h"			//�뿪������صĺ���
#include  "app.h"			//�û�������


#endif  //if  USOC_EN > 0


#endif  //__INCLUDE_H__
