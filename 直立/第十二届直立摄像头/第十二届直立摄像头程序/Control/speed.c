/*��
 *		��������ѧԺֱ������ͷ
 * @file       speed.c
 * @brief      �ٶȲ��ֳ���
 * @version    v5.2
 * @author     ��������ѧԺ
 * @date       2017
 */
#include  "common.h"
#include  "include.h"



/**********ȫ�ֱ�������********/
int32 LeftDead = 0;
int32 RighDead = 0;
float Ratio_Encoder_Left = 203/(1080*(20/150.0));// �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������)
float Ratio_Encoder_Righ = 203/(1080*(20/150.0));//  �����ٶ�=counter*�����ܳ�(mm)/(����תһȦ��Ӧ��������*��������)
float g_fRealSpeed = 0;
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;
float g_fExpectSpeed = 3100;	//�����ٶȣ���λcm/s
float g_fSpeedError;
float g_fSpeedErrorTemp[5] = {0};
float fSpeedErrorInteg = 0;
int32 g_nLeftpulse = 0,g_nRighpulse = 0;
float g_PWMOut;
int32 g_nLeftPWM, g_nRighPWM;
//�廪�ٶȿ���
float g_speedControl_P = 0.85;//�ٶ�P
float g_speedControl_I = 0;	//����û�и�I����P���е��ٶȿ��ƣ�������̬����ǰ�Ƽ��ٶȼ�ƫ����
//λ�Ʊ���
float g_fSpeedControlOutOld = 0,g_fSpeedControlOutNew = 0;
float g_fSpeedControlOut = 0;


/**
 * @file		�������������ʼ��
 * @author		��������ѧԺ
 * @date		2017
 */
void MotorInit(void)
{	
  	//��ʼ�����ҵ���˿�
  	ftm_pwm_init(FTM0, FTM_CH4, 10*1000, 0);
  	ftm_pwm_init(FTM0, FTM_CH5, 10*1000, 0);
	ftm_pwm_init(FTM0, FTM_CH6, 10*1000, 0);
  	ftm_pwm_init(FTM0, FTM_CH7, 10*1000, 0);
	
	//��ʼ�������������
  	ftm_quad_init(FTM2);//��ʼ�� TPM2 (PA10\11)Ϊ��������,�������
    ftm_quad_init(FTM1);//��ʼ�� TPM1 (PA12\13)Ϊ��������,�ұ�����
}							


/**
 * @file		PWM���
 * @author		��������ѧԺ
 * @date		2017
 */
void PWMOut(void)
{
	int32 nLeftPWM, nRighPWM;
	
	g_PWMOut = g_BlanceControlOut - g_fSpeedControlOut;
	if(Flag_Speed == OFF)g_PWMOut = g_BlanceControlOut;		//���Flag_Speed == OFF������ٶ�
	
	if(g_fDirectionControlOut>=0)
	{
		g_nLeftPWM = (int32)(g_PWMOut + g_fDirectionControlOut);
		g_nRighPWM = (int32)(g_PWMOut - g_fDirectionControlOut); 
	}
	if(g_fDirectionControlOut<0)
	{
		g_nLeftPWM = (int32)(g_PWMOut + g_fDirectionControlOut);
		g_nRighPWM = (int32)(g_PWMOut - g_fDirectionControlOut);
	}

//	g_nLeftPWM = g_nLeftPWM*1.05;//���ݾ������������첹��
//	g_nRighPWM = g_nRighPWM;
	
	
	if(Flag_Direction == OFF)
	{
	 g_nLeftPWM = (int32)g_PWMOut;		//���Flag_Direction == OFF���������
	 g_nRighPWM = (int32)g_PWMOut;		//���Flag_Direction == OFF���������
	}
	
	if(Flag_Stop == OFF) 		//���Flag_Stop == OFF������0
	{
	  	g_nLeftPWM = 0;g_nRighPWM = 0;//ͣ��ʱ���Ҳ������������������ֵ
	}

  	if(g_nLeftPWM < 0)
	{
		nLeftPWM = LeftDead - g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 800? 800: nLeftPWM);
		ftm_pwm_duty(FTM0, FTM_CH6, nLeftPWM);
		ftm_pwm_duty(FTM0, FTM_CH7, 0);
	}
	else
	{
		nLeftPWM = LeftDead + g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 800? 800: nLeftPWM);
		ftm_pwm_duty(FTM0, FTM_CH6, 0);
		ftm_pwm_duty(FTM0, FTM_CH7, nLeftPWM);
	}

	if(g_nRighPWM < 0)
	{
		nRighPWM = RighDead - g_nRighPWM;
		nRighPWM = (nRighPWM > 800? 800: nRighPWM);
		ftm_pwm_duty(FTM0, FTM_CH4, 0);
		ftm_pwm_duty(FTM0, FTM_CH5, nRighPWM);
	}
	else
	{
		nRighPWM = RighDead + g_nRighPWM;
		nRighPWM = (nRighPWM > 800? 800: nRighPWM);
		ftm_pwm_duty(FTM0, FTM_CH4, nRighPWM);
		ftm_pwm_duty(FTM0, FTM_CH5, 0);
	}	
}
   

/**
 * @file		�����ٶ�ƫ��
 * @note      	����ȫ�ֱ���g_fSpeedError
 * @author		��������ѧԺ
 * @date		2017
 */
void CalSpeedError(void)
{
	static float fSpeedOld = 0, fSpeedNew = 0;
	
	g_nLeftpulse = ftm_quad_get(FTM2);//��ȡ��������
	ftm_input_clean (FTM2);
	g_nRighpulse =-ftm_quad_get(FTM1);//��ȡ��������
	ftm_input_clean (FTM1);
	
	g_fLeftRealSpeed = g_nLeftpulse*Ratio_Encoder_Left;
	g_fLeftRealSpeed = (g_fLeftRealSpeed>3200?3200:g_fLeftRealSpeed);		//���������������
	g_fRighRealSpeed = g_nRighpulse*Ratio_Encoder_Righ;
	g_fRighRealSpeed = (g_fRighRealSpeed>3200?3200:g_fRighRealSpeed);		//���ұ�����������
	
	g_fRealSpeed = (g_fLeftRealSpeed + g_fRighRealSpeed)*0.5;				//����ƽ��Ϊ��ʵ�ٶ�
	//�ٶȲɼ��ݶ�ƽ����ÿ�βɼ����仯200
	fSpeedOld = g_fSpeedFilter;
	fSpeedNew = g_fRealSpeed;
	
	if(fSpeedNew>=fSpeedOld)
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)>200?(fSpeedOld+200):fSpeedNew);
	else
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)<-200?(fSpeedOld-200):fSpeedNew);

	g_fSpeedError = g_fExpectSpeed - g_fSpeedFilter;
//	g_fSpeedError = float04 - g_fSpeedFilter;
	
	g_fSpeedErrorTemp[4] = g_fSpeedErrorTemp[3];
	g_fSpeedErrorTemp[3] = g_fSpeedErrorTemp[2];
	g_fSpeedErrorTemp[2] = g_fSpeedErrorTemp[1];
	g_fSpeedErrorTemp[1] = g_fSpeedErrorTemp[0];
	g_fSpeedErrorTemp[0] = g_fSpeedError;
	
}


/**
 * @file		�ٶȿ��ƣ�δ�õ����֣����ֺ��ԣ�
 * @note      	�ٶ��ݶ�ƽ��
 * @author		��������ѧԺ���ο��廪������
 * @date		2017
 */
void SpeedControl(void)
{
//	static float fSpeedErrorDot = 0;

	CalSpeedError();
	g_fSpeedError = (g_fSpeedError>800?800:g_fSpeedError);//�ٶ�ƫ���޷�
	//��̬�����ٶ�
	//g_fExpectSpeed = 2900 + g_fSpeedError/5.9;					//��2.9����
	//g_fExpectSpeed = (g_fExpectSpeed>3100?3100:g_fExpectSpeed);
	//g_fExpectSpeed = (g_fExpectSpeed<2800?2800:g_fExpectSpeed);
	
	fSpeedErrorInteg += (g_speedControl_I/100.0) * g_fSpeedError;
	if(Flag_Stop==1|Flag_Speed==1)fSpeedErrorInteg = 0;		//ͣ����������
		
	fSpeedErrorInteg = (fSpeedErrorInteg < 0.0? 0.0: fSpeedErrorInteg);//��������
	fSpeedErrorInteg = (fSpeedErrorInteg > 50.0? 50.0: fSpeedErrorInteg);//��������
	
	g_fSpeedControlOutOld = g_fSpeedControlOutNew;
	g_fSpeedControlOutNew = g_speedControl_P*g_fSpeedError + fSpeedErrorInteg;//�ٶȿ������
//	g_fSpeedControlOutNew = float03*g_fSpeedError + fSpeedErrorInteg;
	
}

/**
 * @file		�ٶȿ���ƽ�����
 * @note      	��20�����
 * @author		��������ѧԺ���ο��廪������
 * @date		2017
 *			   ��ӭ��У���������У��Ͻ�������ͬ������
 */
void SpeedControlOut(void)
{
	g_fSpeedControlOut = (g_fSpeedControlOutNew - g_fSpeedControlOutOld)*Flag_SpeedControl/20 + 
	  g_fSpeedControlOutOld;

}






