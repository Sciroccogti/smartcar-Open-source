/********************************************************************
 * brd_cfg.h
 * .Description
 *     header file of main board configuration macros 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/

#ifndef _BRG_CFG_H_
#define _BRG_CFG_H_

#include "common.h"
#include "include.h"

//#define M8266WIFI_CONFIG_VIA_USART
#define M8266WIFI_CONFIG_VIA_SPI

#define MCU_IS_FREESCALE_MK60
   
//#define  USE_LED_AND_KEY_FOR_TEST   
   
///////////////////////////////////////////////////////////////
// M8266WIFI Module Interface Board Usages Macros
//      -- User SPI1, PTE5 override PTE0 for software SPI nCS
//         PIN   GND - nRST - nCS - MOSI - MISO - CLK  - 3V3
///////////////////////////////////////////////////////////////
#define  M8266WIFI_nRESET_GPIO                  PTE6
#define  M8266WIFI_nCS_GPIO                     PTE5 //PTB20 // 
#define  M8266WIFI_SPI_NO                       0               // use SPI0, SPI1, or SPI2
#define  M8266WIFI_SPI_USE_CATRx_No             0               // use 0 or 1


//////////////////////////////////////////////////////////////////////////////////////////
#if    ( M8266WIFI_SPI_NO == 0 )

#define  SPI_SCK_GPIO                           PTA15       // PTA15、PTC5、PTD1
#define  SPI_MOSI_GPIO                          PTA16       // PTA16、PTC6、PTD2
#define  SPI_MISO_GPIO                          PTA17       // PTA17、PTC7、PTD3
#define  M8266WIFI_INTERFACE_SPI                SPI0_BASE_PTR
#define  SIM_SCGCx_FOR_SPI                      SIM_SCGC6                              

#define  SIM_SCGC_DSPIx_MASK                    SIM_SCGC6_DSPI0_MASK

//////////////////////////////////////////////////////////////////////////////////////////
#elif (M8266WIFI_SPI_NO == 1)

#define  SPI_SCK_GPIO                           PTE2        // PTE2、PTB11
#define  SPI_MOSI_GPIO                          PTE1        // PTE1、PTB16、PTE3
#define  SPI_MISO_GPIO                          PTE3        // PTE3、PTB17、PTE1
#define  M8266WIFI_INTERFACE_SPI                SPI1_BASE_PTR
#define  SIM_SCGCx_FOR_SPI                      SIM_SCGC6

#if      defined(MK60F15)   
#define  SIM_SCGC_DSPIx_MASK                    SIM_SCGC6_DSPI1_MASK
#elif    defined(MK60DZ10)
#define  SIM_SCGC_DSPIx_MASK                    SIM_SCGC6_SPI1_MASK
#else
#error   UKNOWN MK60 Subtype, F15 or Z10?
#endif                                                  

//////////////////////////////////////////////////////////////////////////////////////////
#elif (M8266WIFI_SPI_NO == 2)

#define  SPI_SCK_GPIO                           PTB21       // PTB21、PTD12
#define  SPI_MOSI_GPIO                          PTB22       // PTB22、PTD13
#define  SPI_MISO_GPIO                          PTB23       // PTB23、PTD14
#define   M8266WIFI_INTERFACE_SPI               SPI2_BASE_PTR
#define  SIM_SCGCx_FOR_SPI                      SIM_SCGC3                              

#if      defined(MK60F15)   
#define  SIM_SCGC_DSPIx_MASK                    SIM_SCGC3_DSPI2_MASK
#elif    defined(MK60DZ10)
#define  SIM_SCGC_DSPIx_MASK                    SIM_SCGC3_SPI2_MASK
#else
#error   UKNOWN MK60 Subtype, F15 or Z10?
#endif                                                  

//////////////////////////////////////////////////////////////////////////////////////////
#else
#error "Unsupported Macro M8266WIFI_SPI_NO definition, should be 0~2 for K60"

#endif  // endif to elif (M8266WIFI_SPI_NO == 0/1/2)

#endif  //endif to #ifndef _BRG_CFG_H_
