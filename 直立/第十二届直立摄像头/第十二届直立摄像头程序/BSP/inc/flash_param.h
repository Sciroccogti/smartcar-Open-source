/******************************************************************************
*@file:��flash�����ȡ����
*@author:zrobot
*@doc:
*******************************************************************************/

#ifndef _flash_param_
#define _flash_param_

#include "common.h"
#include "include.h"

#define PARAM_INIT_FLAG     (159)   //���ر�־λ

extern void param_in(void);   //��flash�ж��������õ�������
extern void param_load(struct PARAM * p_param); //��flash������ݸ��ṹ��
extern void param_init(struct PARAM * p_param); //���ز������ṹ��
extern void set_param(struct PARAM *p_param);   //�ṹ�������
extern void param_save(struct PARAM * p_param); //�ṹ�屣�������flash






#endif  /*_flash_param_*/