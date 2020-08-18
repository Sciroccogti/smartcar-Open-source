#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
//#include  "MK60_wdog.h"
//#include  "MK60_SysTick.h"
//#include  "MK60_rtc.h"      //RTC
//#include  "MK60_adc.h"      //ADC
//#include  "MK60_dac.h"      //DAC
//#include  "MK60_can.h"      //CAN
//#include  "MK60_usb.h"      //usb
//#include  "MK60_i2c.h"      //I2C
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_sdhc.h"     //SDHC


#include  "VCAN_LED.H"          //LED
#include  "VCAN_camera.h"       //����ͷ��ͷ�ļ�
#include  "ff.h"                //FatFs

#include  "VCAN_BMP.h"          //BMP
#include  "vcan_img2sd.h"       //�洢ͼ��sd��һ���ļ�
#include  "vcan_sd_app.h"       //SD��Ӧ�ã���ʾsd����ͼƬ�̼���

#include  "VCAN_computer.h"     //�๦�ܵ�������

#include  "I2C.h"   
#include  "MK60_it.h"
#include  "picture_deal.h"
#include  "scope.h"
#include  "upstand.h"
#include  "speed.h"
#include  "direction.h"
/*************************����Ϊ��д�ļ�*****************************/
#include  "bsp_macro.h"
#include  "bsp_global_var.h"
#include  "bsp_infrared.h"
#include  "bsp_oled.h"
#include  "bsp_my_oled.h"
#include  "ui_debug.h"
#include  "ui_setpage.h"
#include  "flash_param.h"
#include  "ui_debug_common.h"




#endif  //__INCLUDE_H__
