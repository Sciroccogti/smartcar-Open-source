/*!
 *     封车程序
 */

#include "common.h"
#include "include.h"

/*******************************************************************************
* Function Name: main();
* Description  : 主函数
* Author Data  : 2018/1/28
******************************************************************************/
void main(void)
{ 
    sys_init();                   // 系统初始化    
    while(1)
    {
        camera_get_img();         // 摄像头获取图像
        elec_get_mess();
        img_ps();                // 图像处理             
        bumper_car();          //碰碰车
  //      if(Total_distance>1000)
  //      start_stop();
        led_led();
        time_dis_limit();
        deal_key_event();
        beep();
        LCD_UI();   
    }
}






