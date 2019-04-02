#include "common.h"
#include "include.h"
#include "LED.H"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"

void M8266WIFI_Test(void);
void main(void)
{
   uint8 success;
 
#ifdef USE_LED_AND_KEY_FOR_TEST			
  LED_Init();					// Initialise LEDs
  KEY_Init();					// Initialise KEYs
#endif
		
  /////////////////////////////////////////////////////////////////////////////////////////////////////
	//1. Call M8266HostIf_Init() to initialize the MCU Host Interface
	//   - including SPI and nREST/nCS GPIO pins
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	M8266HostIf_Init();
	
#ifdef USE_LED_AND_KEY_FOR_TEST	
  u8 i;	
  //Flash 5 times both of the two LEDs simutaneously indicating that next will initialisation the M8266WIFI module
	for(i=0; i<5; i++)
  {
		LED_set(0, 0); LED_set(1, 0);  M8266WIFI_Module_delay_ms(50);
		LED_set(0, 1); LED_set(1, 1);  M8266WIFI_Module_delay_ms(50);
	}
#endif
	
	
  /////////////////////////////////////////////////////////////////////////////////////////////////////
	//2. Call M8266WIFI_Module_Init_Via_SPI() to initialize the wifi module via SPI Interface
  //   - Including: Module reset, module select, module connecting wifi if required, and etc	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	success = M8266WIFI_Module_Init_Via_SPI();
  if(success)
	{
#ifdef USE_LED_AND_KEY_FOR_TEST			
	    for(i=0; i<3; i++)  // Flash 3 times the Two LEDs alternatively in the Main Board indicating M8266WIFI initialised successfully
	    {
	       LED_set(0, 1); LED_set(1, 0);  M8266WIFI_Module_delay_ms(100);
		     LED_set(0, 0); LED_set(1, 1);  M8266WIFI_Module_delay_ms(100);
			}
		  LED_set(0, 1); LED_set(1, 1);
#endif
	}	
	else // If M8266WIFI module initialisation failed, two led constantly flash in 2Hz
	{
		  while(1)
			{
#ifdef USE_LED_AND_KEY_FOR_TEST					
	       LED_set(0, 1); LED_set(1, 1);  M8266WIFI_Module_delay_ms(250);
		     LED_set(0, 0); LED_set(1, 0);  M8266WIFI_Module_delay_ms(250);
#endif
			}
	}

	//enable_timer_test();
	
  /////////////////////////////////////////////////////////////////////////////////////////////////////
	//3. Call M8266WIFI_Test() to setup socket connection and tansmit/receive data test in a high-speed manner
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	M8266WIFI_Test(); //

	
	while(1)
	{
#ifdef USE_LED_AND_KEY_FOR_TEST					
	       LED_set(0, 1); LED_set(1, 1);  M8266WIFI_Module_delay_ms(1000);
		     LED_set(0, 0); LED_set(1, 0);  M8266WIFI_Module_delay_ms(1000);
#endif
	}
} //end of main 

