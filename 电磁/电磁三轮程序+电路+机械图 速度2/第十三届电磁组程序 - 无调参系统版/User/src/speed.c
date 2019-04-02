/*********************************************************************************************************************
 * 
 * @file       		speed.c
 *  				�ٶȿ���
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "includefile.h"


/**********ȫ�ֱ�������********/
int32 LeftDead = 0;
int32 RighDead = 0;
float Ratio_Encoder_Left = 207/(1175*0.02);// �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������)
float Ratio_Encoder_Righ = 207/(1175*0.02);// �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������)
float g_fRealSpeed = 0;				//��ʵ�ٶ�
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;			//�����˲��ٶ�
float g_fExpectSpeed = 2800;		//�����ٶ�
float g_fSpeedError;				//�ٶ�ƫ��
float g_fSpeedErrorTemp[5] = {0};
float fSpeedErrorInteg = 0;			
int32 g_nLeftpulse = 0,g_nRighpulse = 0;
float g_PWMOut;
int32 g_nLeftPWM, g_nRighPWM;
//�ٶȿ���
float g_speedControl_P = 320;		//�ٶ�P
float g_speedControl_I = 100;		//�ٶ�I
float g_fSpeedControlOut = 0;		//�ٶ����
						

/**
 * @file		PWM���
 * @date		2018
 */
void PWMOut(void)
{
	int32 nLeftPWM = 0, nRighPWM = 0;
	
	if(Flag_Speed == OFF)		g_fSpeedControlOut = 0;		//���Flag_Speed == OFF		������ٶ�
	if(Flag_Direction == OFF)	g_fDirectionControlOut = 0;	//���Flag_Direction == OFF ���������
		
	g_nLeftPWM = (int32)(g_fSpeedControlOut - g_fDirectionControlOut);
	g_nRighPWM = (int32)(g_fSpeedControlOut + g_fDirectionControlOut); 

//	g_nLeftPWM = g_nLeftPWM*1.05;//������첹��
//	g_nRighPWM = g_nRighPWM;
	
	if(Flag_Stop == OFF) 									//���Flag_Stop == OFF������0
	{
	  	g_nLeftPWM = 0;g_nRighPWM = 0;
	}

  	if(g_nLeftPWM < 0)
	{
		nLeftPWM = LeftDead - g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 900? 900: nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch2, nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch3, 0);
	}
	else
	{
		nLeftPWM = LeftDead + g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 900? 900: nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch2, 0);
		ftm_pwm_duty(ftm2, ftm_ch3, nLeftPWM);
	}

	if(g_nRighPWM < 0)
	{
		nRighPWM = RighDead - g_nRighPWM;
		nRighPWM = (nRighPWM > 900? 900: nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch4, 0);
		ftm_pwm_duty(ftm2, ftm_ch5, nRighPWM);
	}
	else
	{
		nRighPWM = RighDead + g_nRighPWM;
		nRighPWM = (nRighPWM > 900? 900: nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch4, nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch5, 0);
	}	
}
   

/**
 * @file		�����ٶ�ƫ��
 * @note      	����ȫ�ֱ���g_fSpeedError
 * @date		2017
 */
void CalSpeedError(void)
{
	static float fSpeedOld = 0, fSpeedNew = 0;
	
	g_nLeftpulse = (gpio_get(Coder_dir_left) ==1?ftm_count_get(ftm0):-ftm_count_get(ftm0));//��ȡ��������
	ftm_count_clean(ftm0);
	g_nRighpulse = (gpio_get(Coder_dir_right)==0?ftm_count_get(ftm1):-ftm_count_get(ftm1));//��ȡ��������
	ftm_count_clean(ftm1);	
	
	g_fLeftRealSpeed = g_nLeftpulse*Ratio_Encoder_Left;
	g_fLeftRealSpeed = (g_fLeftRealSpeed>3400?3400:g_fLeftRealSpeed);		//���������������
	g_fRighRealSpeed = g_nRighpulse*Ratio_Encoder_Righ;
	g_fRighRealSpeed = (g_fRighRealSpeed>3400?3400:g_fRighRealSpeed);		//���ұ�����������
	
	g_fRealSpeed = (g_fLeftRealSpeed + g_fRighRealSpeed)*0.5;				//��ʵ�ٶ�
	
	//�ٶȲɼ��ݶ�ƽ����ÿ�βɼ����仯200
	fSpeedOld = g_fSpeedFilter;
	fSpeedNew = g_fRealSpeed;
	
	if(fSpeedNew>=fSpeedOld)
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)>300?(fSpeedOld+300):fSpeedNew);
	else
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)<-300?(fSpeedOld-300):fSpeedNew);
	
	//flash�в���
//	g_fExpectSpeed = float05;  //2800
	
	g_fSpeedError =  g_fExpectSpeed - g_fSpeedFilter;
	g_fSpeedErrorTemp[4] = g_fSpeedErrorTemp[3];
	g_fSpeedErrorTemp[3] = g_fSpeedErrorTemp[2];
	g_fSpeedErrorTemp[2] = g_fSpeedErrorTemp[1];
	g_fSpeedErrorTemp[1] = g_fSpeedErrorTemp[0];
	g_fSpeedErrorTemp[0] = g_fSpeedError;
	
}


/**
 * @file		�ٶȿ���
 * @note      	�ٶ��ݶ�ƽ��
 * @date		2017
 */
void SpeedControl(void)
{
	int8 index=1;

	CalSpeedError();	//�����ٶ�ƫ��
	g_fSpeedError = (g_fSpeedError>800?800:g_fSpeedError);//�ٶ�ƫ���޷�
	//flash�в���
//	g_speedControl_P = float06;	//320
//	g_speedControl_I = float07;	//100
	
	//���ַ���
	if((g_fSpeedError<=300)&&(g_fSpeedError>=-300))
	index=1;
	else
	index=0;	
	fSpeedErrorInteg = index *float08 * g_fSpeedError * 0.00001;
	
	if(Flag_Stop==OFF|Flag_Speed==OFF)
	{
		fSpeedErrorInteg = 0;	//ͣ����������
	}	
	//fSpeedErrorInteg = (fSpeedErrorInteg < 0.0? 0.0: fSpeedErrorInteg);//��������
	//fSpeedErrorInteg = (fSpeedErrorInteg > 400.0? 400.0: fSpeedErrorInteg);//��������
	//�ٶȿ����㷨������ʽPI��
	g_fSpeedControlOut += g_speedControl_P*0.005*(g_fSpeedErrorTemp[0]-g_fSpeedErrorTemp[1]) + fSpeedErrorInteg;
	g_fSpeedControlOut = (g_fSpeedControlOut>=6000?6000:g_fSpeedControlOut);
	
}







