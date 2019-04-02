/*!
 *		湖北工程学院直立摄像头
 *
 * @file       bsp_global_var.c
 * @brief      部分全局变量定义
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 */
#include"bsp_global_var.h"

uint8 debug_flag=0;
uint8 picture_already_flag=0;
/******************小车调整参数结构体***********/
/*
 *以下int01到float12是存储在flash的全局变量
 *若有任何参数，想便于调试并掉电不丢失可用这些变量代替
 *配合遥控器和oled调试
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

/**************************摄像头接收解压数组******************************/
uint8  imgbuff1[CAMERA_SIZE];         //采集图像存放数组1
uint8  imgbuff2[CAMERA_SIZE];         //采集图像存放数组2
uint8 *  p_imgbuff1 = (uint8 *)((uint8 *)&imgbuff1);   //图像地址1
uint8 *  p_imgbuff2 = (uint8 *)((uint8 *)&imgbuff2);   //图像地址2
uint8  flag_imgbuff = 0;
uint8  img[CAMERA_H][CAMERA_W];							//解压之后图像
//用于oled显示图像的两个数组
uint8  bmp_buff[1024]={0};			
uint8  compress_buff[120][128]={0};


