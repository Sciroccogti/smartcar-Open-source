#ifndef _DEFINE_H
#define _DEFINE_H


/*************************************************************************
*  ģ�����ƣ�defineģ��
*  ����˵����Include �û��Զ���ĺ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
//ͼ��
#define Process_Hang 240
#define Lie 320
#define WHITE 0
#define BLACK 1
#define CENTER 160
//���뿪��
#define sw_PORT PTB_BYTE0_IN
#define sw1 PTB0_IN
#define sw2 PTB1_IN
#define sw3 PTB2_IN
#define sw4 PTB3_IN
#define sw5 PTB4_IN
#define sw6 PTB5_IN
#define sw7 PTB6_IN
#define sw8 PTB7_IN
//PWM
#define DIR_L PTE25_OUT
#define DIR_R PTE24_OUT
#define DIR_L_Forward  DIR_L=0
#define DIR_L_Backward DIR_L=1
#define DIR_R_Forward  DIR_R=0
#define DIR_R_Backward DIR_R=1
//������
#define buzzer PTD15_OUT
//4��LED��
#define _LED1 PTD3_OUT
#define _LED2 PTD4_OUT
#define _LED3 PTD5_OUT
#define _LED4 PTD6_OUT
//����
#define Test_IO PTE7_OUT
#define Stop_IO PTE8_IN
#define Stand_Time 3000



#endif