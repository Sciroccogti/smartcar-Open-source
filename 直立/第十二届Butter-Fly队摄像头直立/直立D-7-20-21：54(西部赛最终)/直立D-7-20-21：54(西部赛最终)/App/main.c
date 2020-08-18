#include "common.h"
#include "include.h"

/*** 调试参数 ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

/* 
	0：120 速度约 2.19
	1：130 速度约 2.25
	2：140 速度约 2.30
	3：150 速度约 2.45
	4：155 速度约 2.6
	5：160 速度约
	6：165 速度约
*/
char Mode_Set = 4;	// 模式选择 	very important !!!!!!

/*********** main函数***********/
void main(void)
{  
  	System_Init();		//初始化所有模块
	
  	while(1)
  	{
		/************************ 图像采集和显示  ***********************/
		while (!Image_Flag);
		Image_Flag = 0;
		img_extract(Img, ImageBuff,CAMERA_SIZE);	// 解压
		camera_get_img();                       // 摄像头获取图像 
		
			
		/************************* 图像算法分析 ************************/
		if (!Annulus_Flag)
		{
			Image_Handle(Img);			// 普通图像处理
		}
		if (Annulus_Flag)
		{
			Annulus_Handle(Img);		// 环路专用图像处理
		}

		Difference = Point_Average();	// 中线法
		Radius = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);	// 动态PID控制转向
		
		Start_Control();	// 起跑线检测
		
		
		/************************ 无线数据发送   ***********************/  
		if(Image_Send_Flag && Send_OK) //选择是否发送图像信息至上位机
		{
			Send_OK = 0;
			img_recontract(Img, imgbuff,CAMERA_SIZE);		// 图像压缩
			seekfree_sendimg_032(imgbuff, CAMERA_SIZE/2);	// 发送二值化图像至逐飞上位机
//			vcan_sendimg(imgbuff, CAMERA_SIZE);				// 发送二值化图像至山外上位机
//			data_send();							   		// 发送无线调试器
		}
	}
}











