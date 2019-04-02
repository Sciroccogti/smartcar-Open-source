/*****************************************************/
/*Author: 中南大学黄竞辉                             */
/*DATE  ： 2016/12/23                                */
/*DISCRIBE:用户执行main函数                          */
/*****************************************************/

#include "GeneralDeclare.h"
#include "PulseCollectDriver.h"
#include "ImageSensorDriver.h"
#include "DisPlayDeviceDriver.h"
#include "TimerEventDriver.h"
#include "Terminator.h"
#include "FLASH.h"
#include "System_UI.h"
#include "KeyBoardDriver.h"
#include "SoftIIC.h"
#include "Wifi.h"



void main(void)
{
	EnableInterrupts;            
	OLED_init();                 
	EventControlTimer_init();   
	systick_timing(220000);   
       // ESP8266_Init(); 
	IIC_init();                
	FLASH_init();                    
	CommonDelay(100);         
	My_FlashRead(0);           
	key_init();                
	ParameterInit();              

	SystemSettings.Wifi_Init = 'F';
	//if (SystemSettings.Wifi_ON == 'T')
	//	ESP8266_Init();    

	BUZZUP;
	CommonDelay(100);
	BUZZDOWN;
	CommonDelay(50);
	BUZZUP;
	CommonDelay(100);
	BUZZDOWN;

	if (SystemSettings.ResetTest == 'T')
	{
		if (RCM->SRS0 & RCM_SRS0_POR_MASK) OLED_PrintStr(0, 0, "Power_On Reset", 16, 1);

		if (RCM->SRS0 & RCM_SRS0_PIN_MASK) OLED_PrintStr(0, 1, "External Reset", 16, 1);

		if (RCM->SRS0 & RCM_SRS0_LVD_MASK) OLED_PrintStr(0, 2, "LowPower Reset", 16, 1);

		if (RCM->SRS1 & RCM_SRS1_SW_MASK) OLED_PrintStr(0, 3, "Soft Reset", 16, 1);

		if (RCM->SRS1 & RCM_SRS1_LOCKUP_MASK) OLED_PrintStr(0, 4, "CoreLock Reset", 16, 1);

		if (RCM->SRS0 & RCM_SRS0_LOC_MASK) OLED_PrintStr(0, 5, "LossClock Reset", 16, 1);


		while (GPIO_get(14) && GPIO_get(15) && GPIO_get(16) && GPIO_get(17));
	}




	while ('F' == MotorPwm_init()); 
	QuadratureDecode_init();       

	ADC_init();                   


         ImageSensor_init((uint8 *)ImageSensorData, 600);
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_EnableIRQ(PIT1_IRQn);
	OS_menu();
	while (1);
}
