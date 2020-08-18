
#ifndef  __MYCCD_H__
#define  __MYCCD_H__

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"
#include "LDPY_SysTick.h"
#include "LDPY_ADC.h"
#include "LDPY_DMA.h"

#include "myLabVIEW.h"
#include "myGyro.h"
#include "myStruct.h"

#define  	CCD1_CK  	PBout(16)
#define  	CCD1_SI  	PBout(11)

#define  	CCD2_CK  	PBout(8)
#define  	CCD2_SI  	PBout(7)

#define  	CCD3_CK  	PBout(5)
#define  	CCD3_SI  	PBout(4)

#define 	Photo_Center	(63)

#define 	TurnRoundSteer_Max (800)

#define 	ROAD_WIDETH_1_MAX	(90)		//CCD1 ֱ����ռ55����
#define 	ROAD_WIDETH_1_MIN	(20)

#define 	ROAD_WIDETH_2_MAX	(90)		//CCD2 ֱ����ռ60����
#define 	ROAD_WIDETH_2_MIN	(20)

#define 	ROAD_WIDETH_3_MAX	(80)		//CCD3 ֱ����ռ40����
#define 	ROAD_WIDETH_3_MIN	(10)

#define 	TurnRound_Pixel_MIN2	(10)	//CCD2���ֵ���������ռ������Сֵ
#define		TurnRound_Pixel_MAX2	(40)	//CCD2���ֵ���������ռ�������ֵ

#define 	TurnRound_Pixel_MIN3	(3)		//CCD3���ֵ���������ռ������Сֵ
#define		TurnRound_Pixel_MAX3	(30)	//CCD3���ֵ���������ռ�������ֵ

#define		StartEndLine_Pixel_MIN	(5)		//�����ߵ���С���
#define		StartEndLine_Pixel_MAX	(20)	//�����ߵ������

#define 	CAR_LEFT_WHEEL_PIXEL	(58)	//CCD1ָʾ����߳��ֵ�
#define 	CAR_RIGHT_WHEEL_PIXEL	(68)	//CCD1ָʾ���ұ߳��ֵ�

#define 	CCD1_DataReady_Num	(20)		//CCD1������Ч��������
#define 	CCD2_DataReady_Num	(20)		//CCD2������Ч��������
#define 	CCD3_DataReady_Num	(20)		//CCD3������Ч��������

void myCCD_GPIO_Init(void);
void myCCD_CCD1_Sampling(CCD_Info *CCD_info);
void myCCD_CCD2_Sampling(CCD_Info *CCD_info);
void myCCD_CCD3_Sampling(CCD_Info *CCD_info);

void myCCD_DataInit(CCD_Info *CCD_info);
void myCCD_FilterAndBinarization(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Parameter_Info *Parameter_info
								);
void myCCD_FilterAtFirst(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);
void myCCD_DataHandle(	CCD_Info *CCD1_info,
						CCD_Info *CCD2_info,
						CCD_Info *CCD3_info,
						HeadSteer_Info *HeadSteer_info,
						Speed_Info *Speed_info,
						Parameter_Info *Parameter_info,
						Road_Condition *Road_condition,
						Car_State *Car_state,
						Car_State *Car_state_Pre
					 );

//���ܵ�ʱ���õ�·ƫ��
void myCCD_GetBeginLineError(	CCD_Info *CCD1_info,
								CCD_Info *CCD2_info,
								CCD_Info *CCD3_info,
								Speed_Info *Speed_info,
								Parameter_Info *Parameter_info
							);

//�µ�Ԥ�к���
uint8_t myCCD_JudgeRampInAdvance(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);
//CCD�ж����º���
uint8_t myCCD_JudgeRampDown(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Road_Condition *Road_condition);

//��ֱ������ʮ�ֵ��ж�
uint8_t myCCD_JudgeLongStraight(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									HeadSteer_Info *HeadSteer_info
								);

//CCD1ʶ�������ߺ���
uint8_t myCCD_CCD1_GetStartEndLine(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Road_Condition *Road_condition,
									Car_State *Car_state
								  );

//CCD��Ѱ���ߺ���
void myCCD_CCD1_GetLineError(CCD_Info *CCD1_info,Speed_Info *Speed_info);
void myCCD_CCD2_GetLineError(CCD_Info *CCD1_info,CCD_Info *CCD2_info,Speed_Info *Speed_info);
void myCCD_CCD3_GetLineError(CCD_Info *CCD3_info,Speed_Info *Speed_info);
void myCCD_ChooseSpeedMode(CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Speed_Info *Speed_info,
							Parameter_Info *Parameter_info
							);

//���ֵ���߽������������
int16_t myCCD_CCD2_GetLeftIntoTurnRound(CCD_Info *CCD2_info);
int16_t myCCD_CCD3_GetLeftIntoTurnRound(CCD_Info *CCD3_info);
int16_t myCCD_CCD2_GetLeftLineOutTurnRound(CCD_Info *CCD_info);

//���ֵ��ұ߽������������
int16_t myCCD_CCD2_GetRightIntoTurnRound(CCD_Info *CCD2_info);
int16_t myCCD_CCD3_GetRightIntoTurnRound(CCD_Info *CCD3_info);
int16_t myCCD_CCD2_GetRightLineOutTurnRound(CCD_Info *CCD_info);

//·��ʶ����
uint8_t myCCD_GetObstacle(	CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							Steer_Info *Steer_info
							);
				
//�жϳ����Ƿ����				
uint8_t myCCD_JudgeWheelOutOfWay(	CCD_Info *CCD1_info,
									CCD_Info *CCD2_info,
									CCD_Info *CCD3_info,
									Speed_Info *Speed_info
								);
								
//���ֵ�60�ȼ��ʶ��
void myCCD_Get_60_Angle(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info,Speed_Info *Speed_info,Parameter_Info *Parameter_info,Car_State *Car_state);

#endif

/*----------------------------end of myCCD.h------------------------------*/
