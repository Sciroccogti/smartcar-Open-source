/*!
 *     �⳵����
 */

#include "common.h"
#include "include.h"

/*******************************************************************************
* Function Name: main();
* Description  : ������
* Author Data  : 2018/1/28
******************************************************************************/
void main(void)
{ 
    sys_init();                   // ϵͳ��ʼ��    
    while(1)
    {
        camera_get_img();         // ����ͷ��ȡͼ��
        elec_get_mess();
        img_ps();                // ͼ����             
        bumper_car();          //������
  //      if(Total_distance>1000)
  //      start_stop();
        led_led();
        time_dis_limit();
        deal_key_event();
        beep();
        LCD_UI();   
    }
}






