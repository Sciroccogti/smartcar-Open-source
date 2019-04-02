/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @brief      		�����(�Ҷ�����ͷ)����
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		MDK 5.17
 * @Target core		STM32F407ZGT6
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-10-11
 * @note
					OV7725���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA                A2
						SCL                A3
						���ж�              B7
                        ���ж�              A4   
						�����ж�            A6       
                        
						D0                 C6
                        D1                 C7
                        D2                 C8
                        D3                 C9
                        D4                 C11
                        D5                 B6
                        D6                 E5
                        D7                 E6
                        
					------------------------------------ 
                    USBתTTL���߶���         
                        USBתTTL����        ��Ƭ������
                        TX                  A10
                        RX                  A9
                        
					Ĭ�Ϸֱ���              �鿴SEEKFREE_MT9V032.h �ļ��ڵĺ궨�� COL ROW
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/
 
#ifndef _SEEKFREE_MT9V032_H
#define _SEEKFREE_MT9V032_H


#include "headfile.h"

#define COL     188//ͼ����    ��Χ1-752    STM32�ɲɼ�752*480�ֱ��ʣ�������Ҫ�ⲿSRAM���ڲ���RAM�����Դ洢һ��ͼ��
#define ROW     120//ͼ��߶�    ��Χ1-480


//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
	
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;

extern __align(4) u8 mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern u8 image[ROW][COL];      //�ڲ�ʹ��



void mt9v032_init(void);
void mt9v032_cof_uart_interrupt(void);
void seekfree_sendimg_032(void);


#endif
