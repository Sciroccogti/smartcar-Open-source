/*********************************************************************************************************************
 * 
 * @file       		common.c
 *  		
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/

#include "common.h"


#define EX_REF_CLK  16000 //(�����ⲿ�ο�ʱ��Ϊ16000KHZ)
//uint32 ics_clk_khz = 40000;
//uint32 ftm_clk_khz = 40000;
//uint32 core_clk_khz = 40000;
//uint32 bus_clk_khz = 20000;


uint32 ics_clk_khz;
uint32 ftm_clk_khz;
uint32 core_clk_khz;
uint32 bus_clk_khz;


void get_clk(void)
{
    uint16 prescaler_factor[]={1,2,4,8,16,32,64,128,256,512,1024};
    
    ics_clk_khz  = EX_REF_CLK * 1280 ;
    ics_clk_khz = ics_clk_khz / prescaler_factor[((ICS->C1&ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT) + 5];    //����FLL���Ƶ��
    ics_clk_khz = ics_clk_khz / prescaler_factor[(ICS->C2&ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT];          //����ICSCLKOUT
    
    core_clk_khz = ics_clk_khz  / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT) + 1);
    bus_clk_khz  = core_clk_khz / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV2_MASK) >> SIM_CLKDIV_OUTDIV2_SHIFT) + 1);
    ftm_clk_khz  = ics_clk_khz  / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV3_MASK) >> SIM_CLKDIV_OUTDIV3_SHIFT) + 1);
    
    uart_init(DEBUG_PORT,DEBUG_BAUD);   //��ʼ�����Դ��ڣ������ʹ��printf��������
}
