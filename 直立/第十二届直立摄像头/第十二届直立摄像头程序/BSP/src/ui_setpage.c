/******************************************************************************
*@file:调参界面显示
*@author:zrobot
*@doc:调参界面修改就改这里
*******************************************************************************/
#include"ui_setpage.h"

uint8 set_page_1(void)
{
	uint8 page_num = PAGE_MIN;
	int8  exit_flag = 0;
	int8  button_value = 0;

	oled_fill(0x00);
	oled_display_16x8str(16,0,"Smartcar");

	while(!exit_flag)
	{

      button_value = GetIRKey();
        SetIRKey(0);//
		switch(button_value)
		{
			case BT_UP_PRESS: break;
			case BT_DN_PRESS: break;
			case BT_RI_PRESS: page_num++;
			                  exit_flag = 1;
							  break;
			case BT_LE_PRESS: page_num = PAGE_MAX;
			                  exit_flag = 1;
							  break;
			default:          break;
		}
	}

    return page_num;  

}

uint8 set_page_2(struct PARAM *p_param)
{
	 return set_page_16x8common("int01",   &(p_param->int01),
							    "int02",   &(p_param->int02),
							    "int03",     &(p_param->int03),
							    "int04",         &(p_param->int04),
							    2
							    );
}

uint8 set_page_3(struct PARAM *p_param)
{
	 return set_page_16x8common("int05",   &(p_param->int05),
							    "int06",   &(p_param->int06),
							    "int07",     &(p_param->int07),
							    "int08",        &(p_param->int08),
							    3
							    );
}

uint8 set_page_4(struct PARAM *p_param)
{
	 return set_page_16x8common("int09",   &(p_param->int09),
							    "int10",   &(p_param->int10),
							    "int11",     &(p_param->int11),
							    "int12",        &(p_param->int12),
							    4
							    );
}

uint8 set_page_5(struct PARAM *p_param)
{
	 return set_page_16x8common("upstand_P",   &(p_param->float01),
							     "upstand_D",   &(p_param->float02),
							     "speed_P",     &(p_param->float03),
							     "float04",        &(p_param->float04),
							    5
							    );
}

uint8 set_page_6(struct PARAM *p_param)
{
	 return set_page_16x8common("dir_P",   &(p_param->float05),
							    "dir_D",   &(p_param->float06),
							    "float07",     &(p_param->float07),
							    "float08",        &(p_param->float08),
							    6
							    );
}

uint8 set_page_7(struct PARAM *p_param)
{
	 return set_page_16x8common("float09",   &(p_param->float09),
							    "float10",   &(p_param->float10),
							    "float11",     &(p_param->float11),
							    "float12",        &(p_param->float12),
							    7
							    );
}

uint8 set_page_8(struct PARAM *p_param)
{
	uint8 page_num = PAGE_MAX;
	int8  exit_flag = 0;
	int8  button_value = 0;

	oled_fill(0x00);
	oled_display_16x8str(5,1,"Exit set param ?");

	while(!exit_flag)
	{

      button_value = GetIRKey();
        SetIRKey(0);//

		switch(button_value)
		{
			case BT_UP_PRESS: break;
			case BT_DN_PRESS: break;
			case BT_RI_PRESS: page_num = PAGE_MIN;
			                  exit_flag = 1;
							  break;
			case BT_LE_PRESS: page_num--;
			                  exit_flag = 1;
							  break;
			case BT_OK_PRESS: page_num = EXIT_SET;
							  exit_flag = 1;
                              param_save(p_param);
							  break;
			default:          break;
		}
	}

    return page_num;  
    
}

