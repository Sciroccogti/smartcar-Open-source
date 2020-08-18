#include "common.h"
#include "include.h"
  
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);	//图像地址
uint8 img[CAMERA_W*CAMERA_H]; //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
uint8 screen_mid = 40;		  //屏幕中点

uint8  threshold_value = 43,var_speed = 5;
uint16 ADC_light,			//摄像头阈值调节
	   ADC_voltage; 		//电池电压检测
int32 S_D5_Duty = 4455;  	//(3330—4480—5630)

/*** 调试参数 ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

/*********** main函数***********/
  void  main(void)
{	
	uchar i;          	//计数变量
	uchar Send_Count; 	//串口需要发送的数据个数
	uchar Point = 40;	//目标点横坐标
	uint8 Mid_Count;	//保存识别到的中点数量
  
  	System_Init();		//初始化所有模块
	SCCB_WriteByte (OV7725_CNST, 45);				//改变图像阈值
							 				//晚上用55
  	while(1)
  	{   
		/************************ 图像采集和显示  ***********************/
		camera_get_img();                       //摄像头获取图像   
		img_extract(img, imgbuff,CAMERA_SIZE);	//解压
			
		
		/************************* 图像算法分析 ************************/
		Mid_Count = Image_GetLine(img);	//图像分析，得到中点数量
		
		
		/************************ 计算目标角度 *************************/
		Point = Point_Weight();				//使用加权平均或取目标点横坐标	
//		Point = Wave_Filtering(Point, 3);
		save_var(VAR3, Point);				//目标点坐标发回上位机
		save_var(VAR4, Foresight);			//将处理后的前瞻发回上位机
		
		
		/************************ 电机速度控制 *************************/
		Start_Control();	//起跑线检测与停车//		//速度控制放在定时器中断中，10ms执行一次
/*		Send_Count = DataScope_Data_Generate(10); 	//生成10个通道的 格式化帧数据，返回帧数据长度
		for( i = 0 ; i < Send_Count; i++)  			//循环发送,直到发送完毕   
		{  
			uart_putchar(UART4, DataScope_OutPut_Buffer[i]); //从串口丢一个字节数据出去      
		}*/
		
		
		/************************* 舵机调节 ****************************/			 
		S_D5_Duty = PlacePID_Control(&S_D5_PID, 40, Point);	//使用位置式PID解算	
//		S_D5_Duty = range_protect(S_D5_Duty, 3330, 5630);	//舵机占空比限幅保护
		S_D5_Duty = range_protect(S_D5_Duty, 3320, 5700);
		ftm_pwm_duty(S_D5_FTM, S_D5_CH, S_D5_Duty);			//修改舵机占空比5404 5540
				
		
		/********************** HMI串口数据收发   **********************/ 
		if (!System_OK)
		{
			HMI_Data_Handle();
		}
	   
		
		/************************ 无线数据发送   ***********************/  
		if(Image_Send_Flag && Send_OK) //选择是否发送图像信息至上位机
		{
			Send_OK = 0;
			
			img_recontract(img, imgbuff,CAMERA_SIZE); //图像压缩
			data_send();							   //发送至上位机
		}
	}
}











