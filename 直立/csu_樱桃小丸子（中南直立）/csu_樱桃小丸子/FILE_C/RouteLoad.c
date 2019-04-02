#include "GeneralDeclare.h"
#include "SystemControl.h"
#include "Terminator.h"
#include "RoadLoad.h"

#define   CirqueRoadDelay    5
#define   CrossRoadDelay     0.4


uint8                RouteMask[3] = { 'F', 'F', 'F' };  
uint8                RouteStatus[3] = { 'O', 'O', 'O' }; 
uint8                RealRouteBox[3] = { 0 };           
float               RealRouteDistance[3][10] = { 0 };  
uint8                StraightRoute = 0;
Specialtypedef       SpecialRoute[10];
volatile uint8      SpecialOn = 'F';
volatile uint8      RampOn = 'F';                 
SpecialDatatypedef   SpecialData;                  
uint8                CirqueDirBox[6] = { 0 };       
uint8                CrossBox[6] = { 0 };      


void RoadTypeTest(void)
{
	int i = 0;
	if (RoadType != Cirque && RoadType != Obstacle && ImageStatus.CirqueOut == 'F' && ImageStatus.IsCinqueOutIn == 'F')
	{
		if (ImageStatus.WhiteLine >= 20)
		{
			StraightRoute = 0;
			if (SystemSettings.CrossCheck == 'F')return;
			uint8 WhiteBorder = 0;
			for (i = 59; i >= ImageStatus.OFFLine; i--)
			{
				if (ImageDeal[i].IsLeftFind == 'W' && ImageDeal[i].IsRightFind == 'W' && ImageDeal[i].Wide >= 30)
				{
					if (i >= 54)
					{
						while (i >= ImageStatus.OFFLine)
						{
							if (ImageDeal[i].IsLeftFind != 'W' || ImageDeal[i].IsRightFind != 'W' && ImageDeal[i].Wide < 30)
							{
								break;
							}
							WhiteBorder++;
							i--;
						}
						if (WhiteBorder >= 20) 
							RoadType = Cross;
					}
					break;
				}
			}
		}
		else
		{
			for (i = 55; i >= ImageStatus.OFFLine; i--)
			{
				if (ImageDeal[i].Center >= 44 || ImageDeal[i].Center <= 35)
					break;
			}
			if (i <= (ImageStatus.OFFLine + 5) && ImageStatus.OFFRoute >= 70)
			{
				StraightRoute++;
				if (StraightRoute > 4)
					StraightRoute = 4;
			}
			else
				StraightRoute = 0;
			if (StraightRoute > 3)
				RoadType = Straight;
		}
	}
	else
		StraightRoute = 0;
}


static float CrossNowSite = 0;
void RouteTypeDeal(RoadType_e NowRoute)
{
	if (NowRoute == Cross)
	{
		if (RouteStatus[RealCrossType] == 'O')
		{
			RouteStatus[RealCrossType] = 'I'; 
			CrossNowSite = CarInfo.length;
			RouteMask[RealCrossType] = 'T';
		}
		else if (RouteStatus[RealCrossType] == 'I'
                         && (CarInfo.length - CrossNowSite) >= 1.5)
		{
                  CrossNowSite = CarInfo.length;
			RealRouteBox[RealCrossType]++;
			RouteStatus[RealCrossType] = 'O';  
			RealRouteDistance[RealCrossType][RealRouteBox[RealCrossType]] = CarInfo.length;
			RouteMask[RealCrossType] = 'T'; 
		} 
	}
	else if (NowRoute == Obstacle)
	{
		RealRouteBox[RealObstacleType]++;
		RouteStatus[RealObstacleType] = 'O';   ///���ϰ���־
		RealRouteDistance[RealObstacleType][RealRouteBox[RealObstacleType]] = CarInfo.length;////��¼���ϰ�λ��
		RouteMask[RealObstacleType] = 'T'; ////�����ϰ�����
	}
	else if (NowRoute == Cirque)
	{
		RealRouteBox[RealCirqueType]++;/////��¼�ϰ�����
		RouteStatus[RealCirqueType] = 'O';   ///���ϰ���־
		RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]] = CarInfo.length;////��¼���ϰ�λ��
		RouteMask[RealCirqueType] = 'T'; ////�����ϰ�����
	}
	SpecialDeal();          ////���⴦��
}


uint8 RouteMaskClear(void)     //////�������
{
	if (RouteMask[RealObstacleType] == 'T')////���ϰ����ο���
	{
		if (ImageStatus.ObstacleDelay == 'F')
		{
			RouteMask[RealObstacleType] = 'F';
			return 'T';
		}
		else if ((-RealRouteDistance[RealObstacleType][RealRouteBox[RealObstacleType]] + CarInfo.length) < ImageInfo.ObstacleDelay)
			return 'F';       //////�趨�����ڲ�����ϰ�
		else
		{
			RouteMask[RealObstacleType] = 'F';
			return 'T';
		}
	}
	else if (RouteMask[RealCrossType] == 'T')/////��ʮ�����ο���ʱ
	{
		if ((-CrossNowSite + CarInfo.length) < CrossRoadDelay)
			return 'F';  /////0.5���ڲ����
		else
		{
			RouteMask[RealCrossType] = 'F';
			return 'T';
		}
	}
	else if (RouteMask[RealCirqueType] == 'T')
	{
		if (ImageStatus.IsCinqueOutIn == 'T')
		{
			RouteMask[RealCirqueType] = 'F';///////���������Բ��������ǿ������Բ��
			return 'T';
		}
		else if ((-RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]] + CarInfo.length) < CirqueRoadDelay)
			return 'F';/////3���ڲ����
		else
		{
			RouteMask[RealCirqueType] = 'F';
			return 'T';
		}
	}
	else
		return 'T';
}


void SpecialDeal(void)
{
	if (SpecialOn == 'T')return;
	for (uint8 i = 0; i < 10; i++)
	{
		if (SpecialRoute[i].Mask == 'T')
		{
			if (SpecialRoute[i].Road == Cross && SpecialRoute[i].RoadNum == RealRouteBox[RealCrossType])
			{
				SpecialOn = 'T';
				SpecialData.Type = SpecialRoute[i].Road;
				SpecialData.Start = SpecialRoute[i].StartDistance + RealRouteDistance[RealCrossType][RealRouteBox[RealCrossType]];
				SpecialData.End = SpecialRoute[i].EndDistance + RealRouteDistance[RealCrossType][RealRouteBox[RealCrossType]];
				SpecialData.Angle = SpecialRoute[i].Angle;
			}
			else if (SpecialRoute[i].Road == Cirque && SpecialRoute[i].RoadNum == RealRouteBox[RealCirqueType])
			{
				SpecialOn = 'T';
				SpecialData.Type = SpecialRoute[i].Road;
				SpecialData.Start = SpecialRoute[i].StartDistance + RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]];
				SpecialData.End = SpecialRoute[i].EndDistance + RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]];
				SpecialData.Angle = SpecialRoute[i].Angle;
			}
			else if (SpecialRoute[i].Road == Obstacle && SpecialRoute[i].RoadNum == RealRouteBox[RealObstacleType])
			{
				SpecialOn = 'T';
				SpecialData.Type = SpecialRoute[i].Road;
				SpecialData.Start = SpecialRoute[i].StartDistance + RealRouteDistance[RealObstacleType][RealRouteBox[RealObstacleType]];
				SpecialData.End = SpecialRoute[i].EndDistance + RealRouteDistance[RealObstacleType][RealRouteBox[RealObstacleType]];
				SpecialData.Angle = SpecialRoute[i].Angle;
			}
			else if (SpecialRoute[i].Road == Normal)
			{
				SpecialOn = 'T';
				SpecialData.Type = SpecialRoute[i].Road;
				SpecialData.Start = SpecialRoute[i].StartDistance;
				SpecialData.End = SpecialRoute[i].EndDistance;
				SpecialData.Angle = SpecialRoute[i].Angle;
			}
			if (SpecialOn == 'T')
                        {
		            SpecialRoute[i].Mask = 'F';
			    if (SpecialRoute[i].IsRamp == 'T')
				RampOn = 'T';
                        }
			break;
		}
	}
}