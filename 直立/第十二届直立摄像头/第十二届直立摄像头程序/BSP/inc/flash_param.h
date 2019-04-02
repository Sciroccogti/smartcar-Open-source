/******************************************************************************
*@file:从flash里面读取参数
*@author:zrobot
*@doc:
*******************************************************************************/

#ifndef _flash_param_
#define _flash_param_

#include "common.h"
#include "include.h"

#define PARAM_INIT_FLAG     (159)   //重载标志位

extern void param_in(void);   //从flash中读参数设置到变量中
extern void param_load(struct PARAM * p_param); //从flash里读数据给结构体
extern void param_init(struct PARAM * p_param); //重载参数给结构体
extern void set_param(struct PARAM *p_param);   //结构体给变量
extern void param_save(struct PARAM * p_param); //结构体保存参数给flash






#endif  /*_flash_param_*/