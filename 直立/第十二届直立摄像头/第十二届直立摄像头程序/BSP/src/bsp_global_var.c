/*!
 *		��������ѧԺֱ������ͷ
 *
 * @file       bsp_global_var.c
 * @brief      ����ȫ�ֱ�������
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */
#include"bsp_global_var.h"

uint8 debug_flag=0;
uint8 picture_already_flag=0;
/******************С�����������ṹ��***********/
/*
 *����int01��float12�Ǵ洢��flash��ȫ�ֱ���
 *�����κβ���������ڵ��Բ����粻��ʧ������Щ��������
 *���ң������oled����
 */
struct PARAM parameter;

int16 int01=0;
int16 int02=0;
int16 int03=0;
int16 int04=0;
int16 int05=0;
int16 int06=0;
int16 int07=0;
int16 int08=0;
int16 int09=0;
int16 int10=0;
int16 int11=0;
int16 int12=0;

float float01=0;
float float02=0;
float float03=0;
float float04=0;
float float05=0;
float float06=0;
float float07=0;
float float08=0;
float float09=0;
float float10=0;
float float11=1;
float float12=0;

/**************************����ͷ���ս�ѹ����******************************/
uint8  imgbuff1[CAMERA_SIZE];         //�ɼ�ͼ��������1
uint8  imgbuff2[CAMERA_SIZE];         //�ɼ�ͼ��������2
uint8 *  p_imgbuff1 = (uint8 *)((uint8 *)&imgbuff1);   //ͼ���ַ1
uint8 *  p_imgbuff2 = (uint8 *)((uint8 *)&imgbuff2);   //ͼ���ַ2
uint8  flag_imgbuff = 0;
uint8  img[CAMERA_H][CAMERA_W];							//��ѹ֮��ͼ��
//����oled��ʾͼ�����������
uint8  bmp_buff[1024]={0};			
uint8  compress_buff[120][128]={0};


