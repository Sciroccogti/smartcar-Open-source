/*!
 *		��������ѧԺֱ������ͷ
 * @file       direction.c
 * @brief      ���򲿷ֳ���
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */
#include  "common.h"
#include  "include.h"
#include  "math.h"  


/**********ȫ�ֱ�������********/ 
float g_dirControl_P = 4.3;		//����P
float g_dirControl_D = 40;		//����D
int16 g_nDirectionError;  		//����ƫ��
float g_fDirectionError_dot;	//����ƫ��΢��
float g_fDirectionControlOut;	//����������


/**
 * @file		�������
 * @author		��������ѧԺ
 * @date		2017
 */
void DirectionControl(void)
{
	static float g_nDirectionErrorTemp[7];
		
	g_nDirectionError = (int16)get_picture_diff(89,1);//����ƫ�80��-80��
	g_nDirectionError = (g_nDirectionError>=50?50:g_nDirectionError);//�޷�
	g_nDirectionError = (g_nDirectionError<=-50?-50:g_nDirectionError);
	 
	g_nDirectionErrorTemp[5] = g_nDirectionErrorTemp[4];
	g_nDirectionErrorTemp[4] = g_nDirectionErrorTemp[3];
	g_nDirectionErrorTemp[3] = g_nDirectionErrorTemp[2];
	g_nDirectionErrorTemp[2] = g_nDirectionErrorTemp[1];
	g_nDirectionErrorTemp[1] = g_nDirectionErrorTemp[0];
	g_nDirectionErrorTemp[0] = g_nDirectionError;
	g_fDirectionError_dot = g_nDirectionErrorTemp[0]-g_nDirectionErrorTemp[3];//ƫ��΢��
	g_fDirectionError_dot = (g_fDirectionError_dot>50?50:g_fDirectionError_dot);//΢���޷�
	g_fDirectionError_dot = (g_fDirectionError_dot<-50?-50:g_fDirectionError_dot);
	
	//�����ٶȵĶ�̬PD
/*
	if(g_fSpeedFilter<2000)	   {g_dirControl_P =180 ;g_dirControl_D = 300;}
	else if(g_fSpeedFilter<2000){g_dirControl_P =220 ;g_dirControl_D = 330;}
	else if(g_fSpeedFilter<2100){g_dirControl_P =250 ;g_dirControl_D = 340;}
	else if(g_fSpeedFilter<2200){g_dirControl_P =300 ;g_dirControl_D = 360;}
	else if(g_fSpeedFilter<2300){g_dirControl_P =320 ;g_dirControl_D = 380;}
	else if(g_fSpeedFilter<2400){g_dirControl_P =360 ;g_dirControl_D = 410;}
	else if(g_fSpeedFilter<2500){g_dirControl_P =380 ;g_dirControl_D = 430;}
	else if(g_fSpeedFilter<2600){g_dirControl_P =410 ;g_dirControl_D = 450;}
	else if(g_fSpeedFilter<2700){g_dirControl_P =420 ;g_dirControl_D = 470;}
	else if(g_fSpeedFilter<2800){g_dirControl_P =430 ;g_dirControl_D = 500;}
	else if(g_fSpeedFilter<2900){g_dirControl_P =440 ;g_dirControl_D = 550;}
	else if(g_fSpeedFilter<3000){g_dirControl_P =450 ;g_dirControl_D = 600;}
	else if(g_fSpeedFilter>3000){g_dirControl_P =470 ;g_dirControl_D = 750;}
	else if(g_fSpeedFilter>3100){g_dirControl_P =470 ;g_dirControl_D = 800;}
*/	
	
	g_fDirectionControlOut = g_nDirectionError*g_dirControl_P + g_fDirectionError_dot*g_dirControl_D;//ת��������
//	g_fDirectionControlOut = g_nDirectionError*float05 + g_fDirectionError_dot*float06;//PDת��

//	g_fDirectionControlOut = (g_fDirectionControlOut > 350? 350: g_fDirectionControlOut);//ת������޷�
//	g_fDirectionControlOut = (g_fDirectionControlOut < -350? -350: g_fDirectionControlOut);	
}

