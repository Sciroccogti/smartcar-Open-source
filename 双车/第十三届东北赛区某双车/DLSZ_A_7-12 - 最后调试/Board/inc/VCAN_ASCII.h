/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_ASCII.h
 * @brief      常用ASCII表
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-02
 */

#ifndef _VCAN_ASCII_H_
#define _VCAN_ASCII_H

extern unsigned char vcan_str[4*16*2];

//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
extern unsigned char const ascii_8x16[1536];


extern unsigned char const shou[1*2*16]; //首
extern unsigned char const ye[1*2*16]; //页
extern unsigned char const break_to[4*2*16]; //返回首页
extern unsigned char const jiantou[1*2*16];//>
extern unsigned char const track[4*2*16]; //赛道信息
extern unsigned char const servo[4*2*16]; //舵机参数
extern unsigned char const motor[4*2*16]; //电机参数
extern unsigned char const other[4*2*16]; //其它参数
extern unsigned char const start_go[4*2*16]; //司机开车
extern unsigned char const page_up[3*2*16]; //上一页
extern unsigned char const page_down[3*2*16]; //下一页
extern unsigned char const set[4*2*16]; //系统设置
extern unsigned char const stop[4*2*16]; //司机停车
extern unsigned char const fly[7*2*16]; //小车已经跑飞
extern unsigned char const go_on[7*2*16]; //请按任意键继续
extern unsigned char const open[8*2*16]; 



extern const unsigned short ScreenBitmap[];

#endif  //_VCAN_ASCII_H
