/*!
 *		湖北工程学院直立摄像头
 *
 * @file       main.c
 * @brief      主程序
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 */

#include "common.h"
#include "include.h"

/**
 * @file		小车硬件初始化
 * @author		湖北工程学院
 * @date		2017   
 */
void car_init(void)
{
  //设置中断优先级
  	NVIC_SetPriorityGrouping(4);	//设置优先级分组,4bit抢占优先级
    set_irq_priority(DMA0_IRQn,2);
    set_irq_priority(PORTA_IRQn,2);
    set_irq_priority(PORTC_IRQn,1);
    set_irq_priority(PIT0_IRQn,3);
    
  //辅助部分初始化
  	led_init(LED0);				//LED初始化
	led(LED0, LED_ON);
	gpio_init (BUZZ, GPO,0);	//初始化蜂鸣器
    LCD_Init();					//OLED初始化
    flash_init();				//flash初始化
	uart_init (UART2, 115200);	//串口2初始化用于蓝牙
	lptmr_delay_ms(1000);		//延时1s

  //拨码开关或备用接口初始化(复用为拨码开关或备用接口)
	gpio_init (K1, GPO,0);	//K1	备用接口，K1-4若用于拨码开关则改为GPI;
	gpio_init (K2, GPO,0);	//K2	备用接口
	gpio_init (K3, GPO,0);	//K3	备用接口
	gpio_init (K4, GPO,0);	//K4	备用接口
	
  //红外接收外部中断初始化
    infrared_init();
    set_vector_handler(PORTC_VECTORn,Infrared_Handler);
    enable_irq(PORTC_IRQn);
	
  //控制初始化
	UpstandInit();
	MotorInit();
	
  //PIT中断初始化
    pit_init_ms(PIT0, 5);
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler); 
    enable_irq (PIT0_IRQn);
	
  //摄像头初始化 
	camera_init(p_imgbuff1);    //摄像头引脚配置和图像地址设置
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);     
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);
}

/**
 * @file		主函数
 * @author		湖北工程学院
 * @date		2017-3 
 */
void main(void)
{  
	DisableInterrupts; 				  //禁止全部中断
	param_in();			
	car_init();			
	EnableInterrupts;				  //使能全部中断
	ov7725_eagle_img_flag = IMG_START;//开始采集图像
	enable_irq(PORTA_IRQn);           //允许PTA的中断
  while(1)
  {

	//以下为红外蓝牙调试参数部分
     if(debug_flag!=0)					//如果按下菜单键
      {
        UI_debug();						//菜单调参界面
        debug_flag=0;
        set_param(&parameter);			//参数存在flash里
        oled_fill(0x00);
        oled_display_16x8str(16,0,"OK");//调参完毕，显示ok
      }
	 //以下为红外蓝牙调试参数部分 
	 else 
	   Draw_BMP(0,0,127,7,bmp_buff);	//oled显示图像（调参数时oled显示调参界面，否则显示图像）
	 
	 //vcan_sendimg(img, CAMERA_W * CAMERA_H);//山外上位机显示图像
  }
}



