/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		KEA128_gpio
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#ifndef _KEA128_gpio_h
#define _KEA128_gpio_h



#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"




void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);//��ʼ��gpio
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);            //�������ŷ���
uint8   gpio_get(PTX_n ptx_n);                          //��ȡ����״̬
void    gpio_set(PTX_n ptx_n, uint8 dat);               //��������״̬
void    gpio_turn(PTX_n ptx_n);                         //��ת����״̬


#endif
