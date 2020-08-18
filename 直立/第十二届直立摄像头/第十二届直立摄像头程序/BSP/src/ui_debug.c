/******************************************************************************
*@file:oled调参界面
*@author:zrobot
*@doc:8个页面
*******************************************************************************/


#include"ui_debug.h"



uint8 page_num =0;
void UI_debug(void)
{
	uint8 page_num = 1;
    
    while(page_num != EXIT_SET)
    {
            switch(page_num)
        {
            case 1: page_num = set_page_1();
                    break;
            case 2: page_num = set_page_2(&parameter);
                    break;
            case 3: page_num = set_page_3(&parameter);
                    break;
            case 4: page_num = set_page_4(&parameter);
                    break;
            case 5: page_num = set_page_5(&parameter);
                    break;
            case 6: page_num = set_page_6(&parameter);
                    break;
            case 7: page_num = set_page_7(&parameter);
                    break;
            case 8: page_num = set_page_8(&parameter);
                    break;                    
            default:break;
        }
    }
  
}