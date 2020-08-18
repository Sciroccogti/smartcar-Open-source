
#ifndef __MYSTRUCT_H__
#define __MYSTRUCT_H__

#include "MK60DZ10.h"
#include "math.h"
#include "LDPY_TypeChange.h"

/*-----------------------ѡ��ʹ�ø������ܸ���ģ��-------------------*/
//#define LabVIEW_Control_Enable	(1)					//ʹ����λ��
//#define	DMA_Enable (2)								//��DMA
//#define   Remote_Control_Enable		(3)				//ң��������ʹ��
#define   RemRoad_Control_Enable	(4)	//�����㷨ʹ��,ע�⣬���㷨���ܹرգ�������ܵ����µ�����
//#define   PROTECT_CAR_ENABLE		(5)				//���籣��
//#define   GetRoadCondition_Enable	(6)				//��ȡ��������

/*-----------------------DMAͨ��ѡ��----------------------------------*/
//#define DMA_CH0_Enable	(1)//����CCDͼ��
//#define DMA_CH1_Enable	(2)//������ͼ��,ֻ���Ͷ��������������ǣ���·����Ϣ 
//#define DMA_CH2_Enable 	(3)//����ѹ��ͼ�������ʱ��������߲���ͬʱ����

/*-----------------------DMA��������ѡ��------------------------------*/
//#define DMA_CH2_SendImage_Enable			(4)//CH2ͨ�����ù���1������ѹ��ͼ��
//#define DMA_CH2_SendLabVIEW_Info_Enable		(5)//CH2ͨ�����ù���2��������ʱ������
			
#define 	SPEED_FIFO_LENGTH		(20)
#define     Line_SIZE   			(45)		//�洢���ұ߽��ߣ����ߵȵĶ��г���
#define 	GYRO_LENGTH				(16)		//�洢�����ǵ�AD��ѹֵ
#define 	ANGLE_LENGTH			(20)		//60�ȼ�Ƕ��г���

/*--------------------DMA���ͻ�������С-------------------------*/
#define  image_SendBuff_size   (358)//���建�����Ĵ�С����λ�ֽ�

#define MAX(a,b) (((a)>(b))?(a):(b))	//�����
#define MIN(a,b) (((a)<(b))?(a):(b))	//��С��

typedef enum Car_State_	//-------------------------------�г�״̬
{
	Go_Forward,		//����ǰ��
	Turn_Round,		//��ͷ
	Stop			//ͣ��
}Car_State;

typedef enum Road_Condition_//---------------------------·��
{
	NeedJudge,			//�����ж�
	Straight,			//ֱ��
	Cross,				//ʮ�ֵ�
	Obstacle,			//·��
	Ramp_Up,			//����
	Ramp_Down			//����	
}Road_Condition;

typedef enum Road_Type_	//--------------------------------��������(�ֶ��趨�Ƿ�����µ������֣�·��)
{
	None,					//�����٣�û���µ������֣�·��(Ԥ����������)
	Ramp_Only,				//ֻ���µ�(Ԥ����������)
	All						//ȫ�����Ͷ���
}Road_Type;

typedef enum Car_Mode_//----------------------------------------ѡ���ٶ�ģʽ(����ʱ�ٶȵȼ���ͬ)
{
	High2,
	High1,
	Safe,
	Low1,
	Low2,
	SelfDef,				//�Զ����ٶ�
	CheckCCD,				//����(ǰհ)ģʽ
	CheckSL,				//����������ģʽ
	CheckRamp				//�����µ�ģʽ
}Car_Mode;

typedef struct CCD_Info_//--------------------------------CCD����
{
	uint16_t PixelOri[2][128];      	// CCDԭʼֵ
	uint16_t Pixel[128];				// CCD�˲���ֵ
	uint16_t PixelAfterMult[128];		// CCD�Ŵ��ֵ
	uint16_t PixelBinary[128];			// CCD��ֵ��ֵ
	uint8_t  CCD_PhotoValue[16];		// CCD��άѹ��ͼ��洢ֵ
	
	uint16_t AD_MAX[4];					//AD���ֵ
	uint16_t AD_MIN[4];					//AD��Сֵ
	
	int16_t CCD_PhotoCenter;			//ͼ������ĵ�
	int16_t CCD_ObstacleShift;			//·�ϵ�ʱ��ͼ��ƫ�Ƶ���
	
	int16_t  LeftLine[Line_SIZE];		//��߽����
	int16_t  CentralLine[Line_SIZE];	//���߶���
	int16_t  RightLine[Line_SIZE];		//�ұ߽����
	int16_t  LineError[Line_SIZE];		//ƫ�����
	int16_t  LineError_D[Line_SIZE];	//ƫ��D����
	int16_t  RoadWidth[10];				//·�����
	int16_t  Angle_LeftFifo[ANGLE_LENGTH];	//60�ȼ�Ƕ���
	int16_t  Angle_RightFifo[ANGLE_LENGTH];	//60�ȼ�Ƕ���
	
	int16_t LeftLossLinePixel;			//��¼�������ʱ��ߵĵ�
	int16_t RightLossLinePixel;			//��¼���ұ���ʱ�ұߵĵ�
	
	uint8_t LeftLoseLine_Flag;
	uint8_t RightLoseLine_Flag;
	
	int16_t  RoadWidthOfStraight;		//ֱ����·��
	
	uint8_t  InvalidPixel_Num;			//CCDͼ����Ч����
	uint8_t  CCD_Ready_Num;				//CCD������Ч����
	
	uint8_t AddLine_Flag;				//���߱��
	uint8_t LossLine_Flag;				//CCD���߱�־
	uint8_t Cross_Flag;					//ʮ�ֵ���־
	uint8_t RoadInvalid_Flag;			//���������Ч��ǣ���ֹ�ܵ�
	
}CCD_Info;

typedef struct Gyro_Info_//------------------------------------------------������
{
	int16_t	GyroscopeZero;						//��¼������-���ٶȼƵ�����ѹADֵ
	int16_t	Gyroscope_Fifo[GYRO_LENGTH];		//�洢������-���ٶȼƵ�ѹADֵ
	int32_t Gyro_Sum;
	
	int16_t RampThresholdValue;	
	uint8_t RampUpDown_Num;		//��¼��⵽�����ǵ�ѹͻ��Ĵ���
	
}Gyro_Info;

typedef struct Speed_Info_//-------------------------------------------------�ٶ�
{
	int16_t Straight_Speed;					//ֱ���ٶ�
	int16_t Cur_Speed;				 		//����ٶ�
	int16_t Snake_Speed;					//СS�ٶ�
	int16_t TurnRound_Speed;				//��ͷ�ٶ�
	int16_t Obstacle_Speed;					//·���ٶ�
	int16_t RampUp_Speed;					//���µ��ٶ�
	int16_t RampDown_Speed;					//���µ��ٶ�
	int16_t Angle60_Speed;					//60�ȼ���ٶ�
	
	int16_t SpeedAdd_3;						//3����ֱ������ٶ�����ֵ
	float SpeedMode3Add;					//3���ٶ�����ֵ
	int16_t SpeedMode3Acc_Counter;			//ʹ��2������ʹ��3���ٶȵ�ʱ��������Ч����(cm)
	uint8_t CurSpeedAcc_Flag;				//��������ٱ��
	float CurSpeedAcc;						//�������ֵ
	
	int16_t Speed_Now;						//��ǰ�ٶ�
	int16_t TargetSpeed[2];					//Ŀ���ٶ�
	int16_t SpeedRAcc;						//���ٶ�ֵ
	
	uint8_t SpeedMode;						//�ٶ�ģʽѡ��
	uint8_t LastSpeedMode;					//��һ�ε��ٶ�ģʽ
	
	int8_t  Speed_Direction;				//�ٶȷ���
	int16_t EncoderData;					//�������Ĵ�����ֵ
	int16_t SpeedFifo[SPEED_FIFO_LENGTH];	//�ٶȶ���
	int16_t SpeedError;						//�ٶ�ƫ��
	int16_t LastSpeedError;					//��һ�ε��ٶ�ƫ��
	
	int32_t DistanceTo1CmCount;				//1cm�������
	uint8_t DistanceOf1Cm_Flag;				// 1cm���˱��
	int16_t DistanceOfCross_Counter;		//ʮ�ֵ�1cm�������
	int16_t DistanceOfTurnRound_Counter;	//���ֵ�1cm�������
	int16_t DiatanceToTurnRound;			//ʶ�����ֵ�ʱ�����ǵľ���
	int16_t DistanceBeginTurn;				//��ʼ����ת��ʱ�ľ���
	int16_t DistanceToTurn_MIN;				//���ֵ���ת����С����
	int16_t DistanceToTurn_MAX;				//���ֵ���ת��������
	int16_t DistanceOfRampUp[2];			//���¾���
	int16_t DistanceOfRampDown;				//���¾���
	int16_t DistanceOfRampUp_Counter;		//���µ�1cm�������
	int16_t DistanceOfObstacle_Counter;		//·��1cm�������
	int16_t DistanceOfSpeedMode3_Counter;	//3������1cm�������
	int16_t DistanceOfSpeedMode3_Max;		//ʹ��3���������پ���
	int16_t DistanceOfRoadInvalid_Counter;	//��·��Ч�����,����������ǿ����Ч
	int16_t DistanceOfCCD1GetSL_Counter;	//CCD1Ԥ�������ߺ���������
	int16_t DistanceOf60Angle_Counter;		//���ֵ�60�ȼ��Ԥ�к������ּ�����
	
	uint8_t SpeedCalculate_Enable;			//�����ٶ�ʹ��
	
	int16_t Error_K;
	int16_t Error_D_K;
	int32_t HeadSteerPWMError_K;
    int32_t HeadSteerPWMError_D_K;
	
}Speed_Info;

typedef struct HeadSteer_Info_//------------------------------------ҡͷ���
{
	float KPH;
	float KIH;

	int32_t Steer_Center;				
	int32_t Steer_LeftMAX;				
	int32_t Steer_RightMAX;			

	int32_t Steer_PWMOutputDuty;

	int32_t SteerPWM_Error[10];
	int32_t SteerPWM_Error_D[10];
  
}HeadSteer_Info;

typedef struct Steer_Info_//--------------------------------------���
{
	float KP1;
	float KP2;
	float KP3;
	float KI;

	float HeadToSteer_1;
	float HeadToSteer_2;
	float HeadToSteer_3;

	uint8 KP_Mult;				//PID�Ŵ���
	uint8 KD_Mult;
	uint8 KI_Mult;

	int32_t Steer_Center;				
	int32_t Steer_LeftMAX;				
	int32_t Steer_RightMAX;			

	int32_t Steer_PWMOutputDuty;	
	int32_t SteerPWM_Error[10];

	uint8_t RampUp_SteerBackToCenter_Flag;

}Steer_Info;

typedef struct Motor_Info_//-----------------------------------------------���
{
	float KPT;
	float KP;
	float KI;
	float KD;

	uint8 KP_Mult;				//PID�Ŵ���
	uint8 KI_Mult;
	uint8 KD_Mult;

	int32 PWMOutputDuty;				
  
}Motor_Info;

typedef struct Parameter_Info_//------------------------------------------һЩ����
{
	uint8 Parameter_OK;		
	uint8 UART_RecvData;				//���ڴ������ݽ���
	float StartEndLine_Fifo[10][2];		//�����߶���,10��2�У���һ�м�¼����ܱ�ţ��ڶ��м�¼�ɼ�ʱ��
	
	float AD_MultK_f;
	float CCD1_AD_TargetValue;			//CCD��ѹ�Ŵ��Ŀ��ֵ
	int16_t CCD1_Binary_MAX;			//CCD��ֵ����
	int16_t CCD1_Binary_MIN;			//CCD��ֵ����

	uint8_t GetTurnRound_Enable;		//������ֵ�ʹ��(TRUE/FALSE)
	uint8_t GetRamp_Enable;				//����µ�ʹ��(TRUE/FALSE)
	uint8_t GetStartEndLine_Enable;		//���������ʹ��(TRUE/FALSE)
	uint8_t GetStartEndLine_Flag;		//��������߱�ǣ�1/0��
	uint8_t GetObstacle_Enable;			//���·��ʹ��(TRUE/FALSE)
	uint8_t RoadInvalid_Enable;			//����·��Ч��Ч�ж�(TRUE/FALSE)
	uint8_t GetStartEndLine_SameLight_Enable;//�����߼��ͬ������ʹ��
	uint8_t CCD1GetSL_Enable;			//CCD1ʶ��������ʹ��
	uint8_t WheelOutOfWay_Enable;		//�ж����ӳ���ʹ��
	
	int16_t RampJgeInAdvce_Num;			//�µ�Ԥ�м���
	int16_t TurnRound_OK_Num;			//���ֵ�ȷ�ϴ���,�������μ�⵽���ֵ�����Ϊ�����ֵ�
	int16_t TurnRound_Num_MAX;			//���ֵ����ȷ�ϴ���
	int16_t Obstacle_OK_Num;			//·���жϴ����������������μ�⵽·�ϲ���Ϊ��·��
	
	int32_t Angle60_MeanValue;			//���ֵ�����ж�ʱ��һ���߽�ķ������ֵ
	
	int16_t Mode_HighSpeedDistance;		//���پ���
	
	float	Time;						//��ʻʱ��(��)
	float   GetStartEndLineTime;		//�����߼�⵹��ʱ��������ʱ��ż��������
	float	GetTurnRoundTime;			//��ʼ������ֵ���ʱ��
	float	GetRampTime;				//��ʼ����µ���ʱ��
	float   GetObstacleTime;			//��ʼ���·�ϵ�ʱ��
	float	DebugTime;					//����ʱ��(��)
	float	LoseLineTime;				//����ͣ��ʱ��
	int32_t RoadDistance;				//��ʻ����(����)
	int16_t StopSpeed;					//ͣ��ǰһ���ٶ�

	uint8_t CCD_SampleOK_Flag;  		//CCD������ɱ�־
	uint8_t StartEndLine_Flag; 			//��⵽�����߱�־

	uint8_t	LeftIntoTurnRound_Flag;		//��������ֵ�
	uint8_t RightIntoTurnRound_Flag;	//�ұ������ֵ�
	uint8_t CCD2OrCCD3_GetTurnRound;	//ָʾ��CCD2����CCD3��⵽���ֵ�
	
	uint8_t LeftOrRightObstacle_Flag;	//·�ϱ��(���·��/�ұ�·��)
	
	int32_t OLED_NoAction_Counter;		//����û��Ӧ����
	uint8_t OLED_NoAction_Flag;			//����û�з�Ӧ��־
	uint8_t RampReady_Distance;			//Ԥ���µ������þ��뻹δ�жϵ��µ������Ԥ�б�ǣ���ֹԤ�д���ʱ��ʱ�����
	uint8_t RampReady_FLag;				//�µ�Ԥ�б��
	uint8_t SlowBeforeTurnRound_Flag;	//����ǰ���ٶȲ�ͬ�趨���
	uint8_t LongStraight_Flag;
	uint8_t CCD1_GetedStartEndLine_Flag;//CCD1ʶ�������߱��
	uint8_t	Left60Angle_Flag;			//���60�ȼ�Ǳ��
	uint8_t Right60Angle_Flag;			//�ұ�60�ȼ�Ǳ��
  
}Parameter_Info;

#ifdef  RemRoad_Control_Enable

typedef struct RemSpeedUpDown_Info_//-----------------------------------�����㷨�ṹ��
{
	uint8_t Ramp_Counter;				//�µ�����
	uint16_t RampSpeedUp[2][3];			//��2���µ�
	uint8_t Ramp_Num[2];
	int16_t RampRemDistance[4];
	
	uint8_t Cross_Counter;				//ʮ�ֵ�����
	uint16_t CrossSpeedUp[16][3];		//��16��ʮ�ֵ�
	uint8_t Cross_Num[2];
	int16_t CrossRemDistance[4];
	
	uint8_t TurnRound_Counter;			//���ֵ�����
	uint16_t TurnRoundSpeedUp[2][3];	//��2�����ֵ�
	uint8_t TurnRound_Num[2];
	int16_t TurnRoundRemDistance[4];
	
	uint8_t RemRoad_Enable;				//�����㷨ʹ�ܣ�TRUE/FALSE��
}RemSpeedUpDown_Info;

#endif

typedef struct Flash_Info_//------------------------------------------------Flash����
{
	float   Flash_Array_1[100];
	float   Flash_Array_2[100];
	float   Flash_Array_3[100];
  
}Flash_Info;

/*************************DMA����������ò���********************************/

#ifdef LabVIEW_Control_Enable

typedef struct LabVIEW_Info_
{
	float HeadSteer_KP_Item;					//ҡͷ���KP������	
	float HeadSteer_KI_Item;					//ҡͷ���KI������
	
	float Steer_Head_Item;						//ת����ҡͷ������
	float Steer_Err_Item;						//ת����ƫ��������
	
	float Motor_KP_Item;						//���KP������
	float Motor_KI_Item;						//���KI������
	float Motor_KD_Item;						//���KD������
		
}LabVIEW_Info;

#endif

typedef struct DMA_Required_Variable
{
	uint8_t CCD_Image[image_SendBuff_size];	
	uint8_t ThreeCCDs_Image[420];
}DMA_Required_Variable;

typedef struct 
{
	uint8_t  enter_exit;
	uint16_t choice_flag;
	uint8_t  add_sub;
	uint8_t  enter_exit_temp;
	uint8_t  Clear;
	uint8_t  Ready_Go;
	
}menu;

#endif

/*-----------------------------end of myStruct.h----------------------------------*/	
