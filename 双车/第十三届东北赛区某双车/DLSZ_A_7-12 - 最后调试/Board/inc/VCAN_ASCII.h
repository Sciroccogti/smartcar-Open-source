/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_ASCII.h
 * @brief      ����ASCII��
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */

#ifndef _VCAN_ASCII_H_
#define _VCAN_ASCII_H

extern unsigned char vcan_str[4*16*2];

//����ASCII��
//ƫ����32
//ASCII�ַ���
//ƫ����32
//��С:12*6
extern unsigned char const ascii_8x16[1536];


extern unsigned char const shou[1*2*16]; //��
extern unsigned char const ye[1*2*16]; //ҳ
extern unsigned char const break_to[4*2*16]; //������ҳ
extern unsigned char const jiantou[1*2*16];//>
extern unsigned char const track[4*2*16]; //������Ϣ
extern unsigned char const servo[4*2*16]; //�������
extern unsigned char const motor[4*2*16]; //�������
extern unsigned char const other[4*2*16]; //��������
extern unsigned char const start_go[4*2*16]; //˾������
extern unsigned char const page_up[3*2*16]; //��һҳ
extern unsigned char const page_down[3*2*16]; //��һҳ
extern unsigned char const set[4*2*16]; //ϵͳ����
extern unsigned char const stop[4*2*16]; //˾��ͣ��
extern unsigned char const fly[7*2*16]; //С���Ѿ��ܷ�
extern unsigned char const go_on[7*2*16]; //�밴���������
extern unsigned char const open[8*2*16]; 



extern const unsigned short ScreenBitmap[];

#endif  //_VCAN_ASCII_H
