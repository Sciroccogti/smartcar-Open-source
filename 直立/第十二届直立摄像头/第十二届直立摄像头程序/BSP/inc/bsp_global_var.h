#ifndef _bsp_global_var_
#define _bsp_global_var_

#include "common.h"
#include "include.h"
/**********************************************/

extern uint8 debug_flag;
extern uint8 display_flag;
extern uint8 picture_already_flag;

/******************С�����������ṹ��***********/
extern struct PARAM parameter;

extern int16 int01;
extern int16 int02;
extern int16 int03;
extern int16 int04;
extern int16 int05;
extern int16 int06;
extern int16 int07;
extern int16 int08;
extern int16 int09;
extern int16 int10;
extern int16 int11;
extern int16 int12;

extern float float01;
extern float float02;
extern float float03;
extern float float04;
extern float float05;
extern float float06;
extern float float07;
extern float float08;
extern float float09;
extern float float10;
extern float float11;
extern float float12;

/**************************����ͷ���ս�ѹ����******************************/
extern uint8  imgbuff1[CAMERA_SIZE];         //�ɼ�ͼ������1
extern uint8  imgbuff2[CAMERA_SIZE];         //�ɼ�ͼ������2
extern uint8 *  p_imgbuff1;   //ͼ���ַ1
extern uint8 *  p_imgbuff2;   //ͼ���ַ2
extern uint8  flag_imgbuff;
extern uint8  img[CAMERA_H][CAMERA_W];

extern uint8  bmp_buff[1024];
extern uint8  compress_buff[120][128];

/************************ͼ����******************************************/
extern uint8 find;  //ͼ��Ѱ�߱�־λ������Ѱ�߷�ʽ,Ĭ�ϸ�������

extern int16 left_line[120];      //�����
extern int16 right_line[120];     //�ұ���

extern int16 left_deal[120];      //����ߴ�������
extern int16 right_deal[120];     //�ұ��ߴ�������

extern int16 center_line[120];    //�����ߴ������

extern int16 last_start;

extern uint8 steer_run_flag;

extern int16 time_count;

extern int8 pit0_flag;

extern int16 lost_count;

extern int16 far_line;
/******************************************/
extern Site_t site;                           //��ʾͼ�����Ͻ�λ��
extern Size_t imgsize;             //ͼ���С
extern Size_t size;                   //��ʾ����ͼ���С
extern Site_t site1;
#endif /*_bsp_global_var_*/