#include "GeneralDeclare.h"
#include "Terminator.h"
#include "ProjectMath.h"
#include "SystemControl.h"
#include "RoadLoad.h"


#define  CameraHDot                (float)0.00498000       /////�������зֱ���
#define  CameraVDot                (float)0.00492000       //////�������зֱ���
#define  CarWheelRadius            (float)3.5              //////���ְ뾶

///////���¹����ϰ��궨��
#define  ObstacleDelayMask         0.8             /////X���ڲ������һ���ϰ�       
#define  ObstacleOutDelay          0.15            /////�����ӳ�
#define  ObstacleInternal          5 
#define  ObstacleFirstInternal     8
#define  ObstacleTrueBorder        5
#define  ObstacleOutClearInternal  10



//////���¹��������ߺ궨��
#define  StartDelay                10



/////���¹���Բ���궨��
#define  CiruqeMaskClearDistance    3



#define  CiruqeOutReliableCount     5
#define  CirqueOutWayReliableCount  3
#define  CirqueOutWayExtern         0
#define  LargeCirqueTest            2




ImageDealDatatypedef    ImageDeal[ISRow];
uint8                   ImageSensorData[600] = { 0 };    //һ��8λ����Ϊ8�����ص��ʾ��1����ڣ�0�����
uint8                   PicForNow[60][80] = { 0 };
float                  TrueXSite[60] = { 0 };
float                  TrueYSite[60] = { 0 };
ImageStatustypedef      ImageStatus;
RoadType_e              RoadType;
ImageInfotypedef        ImageInfo;
/*****************************************/
/*Auther :    ���ϴ�ѧ�ƾ���             */
/*Date   ��   2017/8/1                   */
/*Describe �� ����ͷ����                 */
/*Version ��  6.1                        */
/*****************************************/
float  Weighting[10] = { 0.96, 0.92, 0.88, 0.83, 0.77, 0.71, 0.65, 0.59, 0.53, 0.47 };
/////////////10��Ȩ�ز�����������ģ�������Ӱ�죬���°�����̬�ֲ�����




static int       Ysite = 0, Xsite = 0;    ///////Y����
static uint8     *PicTemp;
static int       IntervalLow = 0, IntervalHigh = 0;   /////����ߵ�ɨ������
static int       ytemp = 0, xtemp = 0;
static int       TFSite = 0, FTSite = 0;
static float     DetR = 0, DetL = 0;
/************************/
/*�ӵױ߿�ʼ��Զ��5��Ѱ�ұ����Լ����ߣ���ȡ��򵥵ķ���*/
static     int      BottomBorderRight = 79, BottomBorderLeft = 0, BottomCenter = 0;
static uint8 DrawLinesFirst(void)
{
	PicTemp = PicForNow[59];
	if (*(PicTemp + ImageSensorMid) == 0)                           //����ױ�ͼ���е�Ϊ��
	{
		for (Xsite = 0; Xsite < ImageSensorMid; Xsite++)
		{
			if (*(PicTemp + ImageSensorMid - Xsite) != 0)
				break;
			if (*(PicTemp + ImageSensorMid + Xsite) != 0)
				break;
		}
		if (Xsite > (ImageSensorMid - 15))return 'F';
		if (*(PicTemp + ImageSensorMid - Xsite) != 0)
		{
			BottomBorderRight = ImageSensorMid - Xsite + 1;
			for (Xsite = BottomBorderRight; Xsite > 0; Xsite--)
			{
				if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
				{
					BottomBorderLeft = Xsite;
					break;
				}
				else if (Xsite == 1)
				{
					BottomBorderLeft = 0;
					break;
				}
			}
		}
		else if (*(PicTemp + ImageSensorMid + Xsite) != 0)
		{
			BottomBorderLeft = ImageSensorMid + Xsite - 1;
			for (Xsite = BottomBorderLeft; Xsite < 79; Xsite++)
			{
				if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
				{
					BottomBorderLeft = Xsite;
					break;
				}
				else if (Xsite == 78)
				{
					BottomBorderLeft = 79;
					break;
				}
			}
		}
	}
	else
	{
		for (Xsite = ImageSensorMid; Xsite < 79; Xsite++)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
			{
				BottomBorderRight = Xsite;
				break;
			}
			else if (Xsite == 78)
			{
				BottomBorderRight = 79;
				break;
			}
		}
		for (Xsite = ImageSensorMid; Xsite > 0; Xsite--)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
			{
				BottomBorderLeft = Xsite;
				break;
			}
			else if (Xsite == 1)
			{
				BottomBorderLeft = 0;
				break;
			}
		}
	}
	BottomCenter = (BottomBorderLeft + BottomBorderRight) / 2;
	ImageDeal[59].LeftBorder = BottomBorderLeft;
	ImageDeal[59].RightBorder = BottomBorderRight;
	ImageDeal[59].Center = BottomCenter;                          ////ȷ����ױ�
	ImageDeal[59].Wide = BottomBorderRight - BottomBorderLeft;   /////�洢�����Ϣ
	ImageDeal[59].IsLeftFind = 'T';
	ImageDeal[59].IsRightFind = 'T';
	for (Ysite = 58; Ysite > 54; Ysite--)                         ////���м�������ȷ���ױ�����
	{
		PicTemp = PicForNow[Ysite];
		for (Xsite = ImageDeal[Ysite + 1].Center; Xsite < 79; Xsite++)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) == 0)
			{
				ImageDeal[Ysite].RightBorder = Xsite;
				break;
			}
			else if (Xsite == 78)
			{
				ImageDeal[Ysite].RightBorder = 79;
				break;
			}
		}
		for (Xsite = ImageDeal[Ysite + 1].Center; Xsite > 0; Xsite--)
		{
			if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) == 0)
			{
				ImageDeal[Ysite].LeftBorder = Xsite;
				break;
			}
			else if (Xsite == 1)
			{
				ImageDeal[Ysite].LeftBorder = 0;
				break;
			}
		}
		ImageDeal[Ysite].IsLeftFind = 'T';
		ImageDeal[Ysite].IsRightFind = 'T';
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;   //�洢�е�
		ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;         //�洢���
		if (ImageDeal[Ysite].Wide <= 15 && Ysite <= 56 && SystemSettings.StartCheck == 'T')
		{
			uint8 StartFlagCount = 0;
			for (Xsite = 1; Xsite <= 78; Xsite++)
			{
				if (*(PicTemp + Xsite) != *(PicTemp + Xsite + 1))
					StartFlagCount++;
			}
			if (StartFlagCount >= 10)   ///////��֤�ױ��г���10������㣬����Ϊ�����������߷�Χ����ʼִ��ͣ��
			{
				RoadType = Start;
				return 'S';
			}
		}
	}
	return 'T';
}  /////�������Ҫ��������������Ȳ����ܵ����ţ�����Ҫ�ڰ�װ��ʱ���������ͷ���ӽ�



/*ͼ����ֵת��Ϊ��ʵ�����µ�Y�ᣬ�Գ�����Ϊԭ��*/
static float TruePicToYSite(float CameraLine, float D, float angle)
{
	float Distance = 0;
	float Site = (CameraLine - 29.5) * CameraVDot;
	Distance = (ImageInfo.CameraFocusLength * D + ImageInfo.CameraFocusLength * Site) / (Site + ImageInfo.CameraFocusLength * tanf(PI - angle - ImageInfo.CameraInterAngle));
	Distance /= cos(PI - angle - ImageInfo.CameraInterAngle);
	Distance -= CarWheelRadius / tanf(angle) + D * cosf(ImageInfo.CameraInterAngle) / sinf(angle);
	if (Distance < 0)
		Distance = 1000;
	return Distance;
}




/*��ʵ�����µ�Y��ת��Ϊͼ����ֵ������ͼ���зֲ�*/
static int TrueYSiteToPic(float Distance, float D, float angle)
{
	float CaDot = 0;
	CaDot = (ImageInfo.CameraFocusLength * D - Distance * ImageInfo.CameraFocusLength * tanf(PI - angle - ImageInfo.CameraInterAngle)) / (Distance - ImageInfo.CameraFocusLength);
	CaDot /= CameraVDot;
	CaDot += 29.5;
	return (int)(CaDot);
}



/*ͼ����ֵת��Ϊ��ʵ����X�ᣬ�Գ�����Ϊԭ��*/
static float TruePicToXSite(float CameraX, float Ty, float angle)
{
	float Distance = 0;
	float Site = CameraX * CameraHDot;
	if (Ty >= 200)return 200;
	Distance = Ty * cosf(PI - angle - ImageInfo.CameraInterAngle);
	Distance = Site * (Distance - ImageInfo.CameraFocusLength) / ImageInfo.CameraFocusLength;
	return Distance;
}



///////Ѱ�������   �������
void GetJumpPointFromDet(uint8 *p, uint8 type, int L, int H, JumpPointtypedef *Q)
{
	int i = 0;
	if (type == 'L')
	{
		for (i = L; i <= H; i++)
		{
			if (*(p + i) == 0 && *(p + i + 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';         /////��ȷ����
				break;
			}
			else if (i == (H - 1))
			{
				if (*(p + (L + H) / 2) != 0)
				{
					Q->point = (L + H) / 2;
					Q->type = 'W';      /////����ȷ�������м�Ϊ�ף�
					break;
				}
				else                 /////����ȷ�������м�Ϊ��
				{
					Q->point = H;
					Q->type = 'H';
					break;
				}
			}
		}
	}
	else if (type == 'R')
	{
		for (i = H; i >= L; i--)
		{
			if (*(p + i) == 0 && *(p + i - 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == (L + 1))
			{
				if (*(p + (L + H) / 2) != 0)
				{
					Q->point = (L + H) / 2;
					Q->type = 'W';
					break;
				}
				else
				{
					Q->point = L;
					Q->type = 'H';
					break;
				}
			}
		}
	}
}



///////Ѱ�������   ���������߲���
void GetJumpPointFromCenter(uint8 *p, JumpPointtypedef *L, JumpPointtypedef *R, int Center)
{
	int n = 0;
	for (n = Center; n <= 79; n++)
	{
		if (*(p + n) != 0 && *(p + n + 1) == 0)
		{
			R->point = n;
			R->type = 'T';
			break;
		}
		else if (n == 78)
		{
			R->point = n + 1;
			R->type = 'W';
			break;
		}
	}
	for (n = Center; n >= 1; n--)
	{
		if (*(p + n) != 0 && *(p + n - 1) == 0)
		{
			L->point = n - 1;
			L->type = 'T';
			break;
		}
		else if (n == 1)
		{
			L->point = n - 1;
			L->type = 'W';
			break;
		}
	}
}


//////����
void GetJumpPointFromOther(uint8 *p, JumpPointtypedef *Q, uint8 type, int Start)
{
	int i = 0;
	if (type == 0)
	{
		for (i = (Start - 1); i >= 1; i--)
		{
			if (*(p + i) == 0 && *(p + i + 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == 1)
			{
				Q->point = i;
				Q->type = 'W';
				break;
			}
		}
	}
	else if (type == 1)
	{
		for (i = (Start + 1); i <= 78; i++)
		{
			if (*(p + i) == 0 && *(p + i - 1) != 0)
			{
				Q->point = i;
				Q->type = 'T';
				break;
			}
			else if (i == 78)
			{
				Q->point = i;
				Q->type = 'W';
				break;
			}
		}
	}
}



/*����׷����µõ�ȫ������*/
static void DrawLinesProcess(void)//////���ø���
{
	for (Ysite = 55; Ysite > ImageStatus.OFFLine; Ysite--)
	{
		PicTemp = PicForNow[Ysite];
		JumpPointtypedef JumpPoint[2];
		if (ImageStatus.CirqueOut == 'T' || ImageStatus.CirquePass == 'T')/////������Բ��ʱ�Ӵ�ɨ������
		{
			IntervalLow = ImageDeal[Ysite + 1].LeftBorder - ImageScanInterval - 3;
			IntervalHigh = ImageDeal[Ysite + 1].LeftBorder + ImageScanInterval + 3;
		}
		else
		{
			IntervalLow = ImageDeal[Ysite + 1].LeftBorder - ImageScanInterval;
			IntervalHigh = ImageDeal[Ysite + 1].LeftBorder + ImageScanInterval;
		}
		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////ȷ����ɨ�����䲢��������
		GetJumpPointFromDet(PicTemp, 'L', IntervalLow, IntervalHigh, &JumpPoint[0]);


		if (ImageStatus.CirqueOut == 'T' || ImageStatus.CirquePass == 'T')
		{
			IntervalLow = ImageDeal[Ysite + 1].RightBorder - ImageScanInterval - 3;
			IntervalHigh = ImageDeal[Ysite + 1].RightBorder + ImageScanInterval + 3;
		}
		else
		{
			IntervalLow = ImageDeal[Ysite + 1].RightBorder - ImageScanInterval;
			IntervalHigh = ImageDeal[Ysite + 1].RightBorder + ImageScanInterval;
		}
		LimitL(IntervalLow);
		LimitH(IntervalHigh);                       /////ȷ����ɨ�����䲢��������
		GetJumpPointFromDet(PicTemp, 'R', IntervalLow, IntervalHigh, &JumpPoint[1]);

		if (JumpPoint[0].type == 'W')
		{
			if (ImageStatus.CirqueOut == 'T' || ImageStatus.CirquePass == 'T')
				ImageDeal[Ysite].LeftBorder = 1;///////���λ�ڽ�Բ�����߳�Բ������ʹ�ñ߽�
			else
				ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 1].LeftBorder;
		}
		else
		{
			ImageDeal[Ysite].LeftBorder = JumpPoint[0].point;
		}

		if (JumpPoint[1].type == 'W')
		{
			if (ImageStatus.CirqueOut == 'T' || ImageStatus.CirquePass == 'T')
				ImageDeal[Ysite].RightBorder = 78;
			else
				ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 1].RightBorder;
		}
		else
		{
			ImageDeal[Ysite].RightBorder = JumpPoint[1].point;
		}

		ImageDeal[Ysite].IsLeftFind = JumpPoint[0].type;
		ImageDeal[Ysite].IsRightFind = JumpPoint[1].type;


		///////����ȷ����Щ������ı�Ե
		if ((ImageDeal[Ysite].IsLeftFind == 'H' || ImageDeal[Ysite].IsRightFind == 'H'))
		{
			if (ImageDeal[Ysite].IsLeftFind == 'H')
				for (Xsite = (ImageDeal[Ysite].LeftBorder + 1); Xsite <= (ImageDeal[Ysite].RightBorder - 1); Xsite++)
				{
					if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite + 1) != 0))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////�����ߵ���߻��кڰ�������Ϊ���Ա���ֱ��ȡ��
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
					else if (*(PicTemp + Xsite) != 0)   ///һ�����ְ׵���ֱ������
						break;
					else if (Xsite == (ImageDeal[Ysite].RightBorder - 1))
					{
						ImageDeal[Ysite].LeftBorder = Xsite;////�����ߵ���߻��кڰ�������Ϊ���Ա���ֱ��ȡ��
						ImageDeal[Ysite].IsLeftFind = 'T';
						break;
					}
				}
			if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <= 5)///////ͼ�����޶�
			{
				ImageStatus.OFFLine = Ysite + 1;
				break;
			}
			if (ImageDeal[Ysite].IsRightFind == 'H')
				for (Xsite = (ImageDeal[Ysite].RightBorder - 1); Xsite >= (ImageDeal[Ysite].LeftBorder + 1); Xsite--)
				{
					if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite - 1) != 0))
					{
						ImageDeal[Ysite].RightBorder = Xsite;
						ImageDeal[Ysite].IsRightFind = 'T';
						break;
					}
					else if (*(PicTemp + Xsite) != 0)
						break;
					else if (Xsite == (ImageDeal[Ysite].LeftBorder + 1))
					{
						ImageDeal[Ysite].RightBorder = Xsite;
						ImageDeal[Ysite].IsRightFind = 'T';
						break;
					}
				}
		}

		/////�˶����ڽ�ʮ�ֵ��۵�����������Ҫ���㲻��Բ��  
		if (Ysite <= 50 && ImageStatus.CirquePass == 'F' && ImageStatus.CirqueOut == 'F' && ImageStatus.IsCinqueOutIn == 'F')//////��Ȼ���Ǻܿ�ѧ������ͦ���õ�
		{
			if (ImageDeal[Ysite].IsLeftFind == 'T')
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 4].LeftBorder) <= -4 && (ImageDeal[Ysite + 4].LeftBorder - ImageDeal[Ysite + 8].LeftBorder) >= 1
					)
				{
					for (ytemp = Ysite; ytemp <= (Ysite + 5); ytemp++)
					{
						if (ImageDeal[ytemp].IsRightFind == 'W')
						{
							for (ytemp = (Ysite + 4); ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].LeftBorder = 2 * ImageDeal[ytemp + 1].LeftBorder - ImageDeal[ytemp + 2].LeftBorder;
								ImageDeal[ytemp].IsLeftFind = 'W';
							}
							ImageStatus.WhiteLine += 4;
							break;
						}
					}
				}

			if (ImageDeal[Ysite].IsRightFind == 'T')
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 4].RightBorder) >= 4 && (ImageDeal[Ysite + 4].RightBorder - ImageDeal[Ysite + 8].RightBorder) <= 1
					)
				{
					for (ytemp = Ysite; ytemp <= (Ysite + 5); ytemp++)
					{
						if (ImageDeal[ytemp].IsLeftFind == 'W')
						{
							for (ytemp = (Ysite + 4); ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].RightBorder = 2 * ImageDeal[ytemp + 1].RightBorder - ImageDeal[ytemp + 2].RightBorder;
								ImageDeal[ytemp].IsRightFind = 'W';
							}
							ImageStatus.WhiteLine += 4;
							break;
						}
					}
				}
		}

		if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W')
			ImageStatus.WhiteLine++;

		LimitL(ImageDeal[Ysite].LeftBorder);
		LimitH(ImageDeal[Ysite].LeftBorder);
		LimitL(ImageDeal[Ysite].RightBorder);
		LimitH(ImageDeal[Ysite].RightBorder);

		ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

		if (ImageDeal[Ysite].Wide <= 0)
		{
			ImageStatus.OFFLine = Ysite + 1;
			break;
		}
		else if (ImageDeal[Ysite].RightBorder <= 5 || ImageDeal[Ysite].LeftBorder >= 75)
		{
			ImageStatus.OFFLine = Ysite + 1;
			break;
		}//////��ͼ����С��0�������ұߴﵽһ��������ʱ������ֹѲ��
	}



	if (ImageStatus.OFFLine <= 40 && ImageStatus.IsCinqueOutIn == 'F' && ImageStatus.CirquePass == 'F')
		for (Ysite = ImageStatus.OFFLine; Ysite <= (ImageStatus.OFFLine + 5); Ysite++)
		{/////////���ڰ����Զ�˶Աߵ����ɾ����ʵ�õ��ǲ���ѧ
			if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite + 1].IsLeftFind == 'W'
				&& ImageDeal[Ysite].IsRightFind == 'T' && ImageDeal[Ysite + 1].IsRightFind == 'T')
			{
				while (Ysite >= (ImageStatus.OFFLine + 1))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsRightFind != 'T')break;
					if (ImageDeal[Ysite].IsLeftFind == 'T' && (ImageDeal[Ysite].Wide <= 10 || Ysite <= (ImageStatus.OFFLine + 5)))
					{
						ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 2].LeftBorder;
						ImageDeal[Ysite].IsLeftFind = 'W';
						ImageDeal[Ysite].RightBorder = 2 * ImageDeal[Ysite + 1].RightBorder - ImageDeal[Ysite + 2].RightBorder;
						LimitL(ImageDeal[Ysite].LeftBorder);
						LimitH(ImageDeal[Ysite].LeftBorder);
						LimitL(ImageDeal[Ysite].RightBorder);
						LimitH(ImageDeal[Ysite].RightBorder);

						ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
						ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

						if (ImageDeal[Ysite].Wide <= 0)
						{
							ImageStatus.OFFLine = Ysite + 1;
							break;
						}
					}
				}
				break;
			}
			else if (ImageDeal[Ysite].IsRightFind == 'W' && ImageDeal[Ysite + 1].IsRightFind == 'W'
				&& ImageDeal[Ysite].IsLeftFind == 'T' && ImageDeal[Ysite + 1].IsLeftFind == 'T')
			{
				while (Ysite >= (ImageStatus.OFFLine + 1))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsLeftFind != 'T')break;
					if (ImageDeal[Ysite].IsRightFind == 'T' && (ImageDeal[Ysite].Wide <= 10 || Ysite <= (ImageStatus.OFFLine + 5)))
					{
						ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 2].LeftBorder;
						ImageDeal[Ysite].IsRightFind = 'W';
						ImageDeal[Ysite].LeftBorder = 2 * ImageDeal[Ysite + 1].RightBorder - ImageDeal[Ysite + 2].RightBorder;
						LimitL(ImageDeal[Ysite].LeftBorder);
						LimitH(ImageDeal[Ysite].LeftBorder);
						LimitL(ImageDeal[Ysite].RightBorder);
						LimitH(ImageDeal[Ysite].RightBorder);

						ImageDeal[Ysite].Wide = ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder;
						ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

						if (ImageDeal[Ysite].Wide <= 0)
						{
							ImageStatus.OFFLine = Ysite + 1;
							break;
						}
					}
				}
				break;
			}
		}
	return;
}



///////�ӳ��߻��ƣ���������˵�Ǻ�׼ȷ��
static void DrawExtensionLine(uint8 CirqueType)/////�����ӳ��߲�����ȷ������ //////���ø���
{
	uint8 RExtern = 'T', LExtern = 'T'; //////��תԲ�����ӳ���� �ұ�ͬ��
	if (ImageStatus.IsCinqueOutIn == 'T')
	{
		if (CirqueType == 0)
			RExtern = 'F';
		else if (CirqueType == 1)
			LExtern = 'F';
	}
	if (LExtern == 'T')
		for (Ysite = 55; Ysite >= (ImageStatus.OFFLine + 2); Ysite--)
		{
			PicTemp = PicForNow[Ysite];
			if (ImageDeal[Ysite].IsLeftFind == 'W')
			{
				if (ImageDeal[Ysite + 1].LeftBorder >= 70)
				{
					ImageStatus.OFFLine = Ysite + 1;
					break;
				}
				while (Ysite >= (ImageStatus.OFFLine + 2))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsLeftFind == 'T' && ImageDeal[Ysite - 1].IsLeftFind == 'T' && ImageDeal[Ysite - 2].IsLeftFind == 'T')
					{
						FTSite = Ysite - 2;
						break;
					}
				}
				DetL = ((float)(ImageDeal[FTSite].LeftBorder - ImageDeal[TFSite].LeftBorder)) / ((float)(FTSite - TFSite));
				for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
				{
					ImageDeal[ytemp].LeftBorder = (int)(DetL * ((float)(ytemp - TFSite))) + ImageDeal[TFSite].LeftBorder;
				}
			}
			else
				TFSite = Ysite + 2;
		}
	if (RExtern == 'T')
		for (Ysite = 55; Ysite >= (ImageStatus.OFFLine + 2); Ysite--)
		{
			if (ImageDeal[Ysite].IsRightFind == 'W')
			{
				if (ImageDeal[Ysite + 1].RightBorder <= 10)
				{
					ImageStatus.OFFLine = Ysite + 1;
					break;
				}
				while (Ysite >= (ImageStatus.OFFLine + 2))
				{
					Ysite--;
					if (ImageDeal[Ysite].IsRightFind == 'T' && ImageDeal[Ysite - 1].IsRightFind == 'T' && ImageDeal[Ysite - 2].IsRightFind == 'T')
					{
						FTSite = Ysite - 2;
						break;
					}
				}
				DetR = ((float)(ImageDeal[FTSite].RightBorder - ImageDeal[TFSite].RightBorder)) / ((float)(FTSite - TFSite));
				for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
				{
					ImageDeal[ytemp].RightBorder = (int)(DetR * ((float)(ytemp - TFSite))) + ImageDeal[TFSite].RightBorder;
				}
			}
			else
				TFSite = Ysite + 2;
		}
	for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		ImageDeal[Ysite].Center = (ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder) / 2;
		ImageDeal[Ysite].Wide = -ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder;
	}
	return;
}



static void FeasibleRegionAnalysis(void)////����������������Լ����ϰ�////All done
{
	static float  LastObstacleDitance = 0;
	static uint8   LastObstacleMask = 'F';
	uint8          IsObstacleFind = 'F';
	uint8          TrueLeft = 0, TrueRight = 0;
	if (ImageStatus.ObstacleDelay == 'T')
	{
		if ((CarInfo.length - ImageStatus.ObstacleDistance) >= ImageInfo.ObstacleDelay)
			ImageStatus.ObstacleDelay = 'F';
		else
			return;           /////��������ϰ����ӳٽ׶��򲻶��ϰ������κ��жϳ��������ӳ�
	}
	if (LastObstacleMask == 'T')
	{
		if ((CarInfo.length - LastObstacleDitance) >= ObstacleDelayMask)
			LastObstacleMask = 'F';
		else
			return; //////0.8�׾����ڲ�������һ�ε��ϰ����  
	}
	if (ImageStatus.OFFLine >= 30)return;     /////������Ҫ����Ч��̫��ʱ������
	for (Ysite = 55; Ysite >= (ImageStatus.OFFLine + 1); Ysite--)
	{
		PicTemp = PicForNow[Ysite];
		if (*(PicTemp + ImageDeal[Ysite].Center) == 0)break;
		if (ImageDeal[Ysite].Center >= 70 || ImageDeal[Ysite].Center <= 10)
			break;
		if (ImageDeal[Ysite].IsLeftFind == 'T') TrueLeft++;
		else TrueLeft = 0;
		if (ImageDeal[Ysite].IsRightFind == 'T') TrueRight++;
		else TrueRight = 0;

		if (TrueLeft >= ObstacleTrueBorder)
			for (Xsite = (ImageDeal[Ysite].LeftBorder + 1); Xsite <= (ImageDeal[Ysite].Center - 1); Xsite++)
			{
				if (Xsite <= 0)Xsite = 0;
				if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite + 1) != 0)) //�����ϸ���������Ұ�
				{
					ImageDeal[Ysite].LeftTemp = Xsite;
					if ((ImageDeal[Ysite].LeftTemp - ImageDeal[Ysite].LeftBorder) >= ObstacleFirstInternal)
						ImageDeal[Ysite].IsLeftFind = 'P';  ////�ϰ�����һ�ߵļ�϶�����޶�ֵ
					break;
				}
			}

		if (TrueRight >= ObstacleTrueBorder)
			for (Xsite = (ImageDeal[Ysite].RightBorder - 1); Xsite >= (ImageDeal[Ysite].Center + 1); Xsite--)
			{
				if (Xsite >= 79)Xsite = 79;
				if ((*(PicTemp + Xsite) == 0) && (*(PicTemp + Xsite - 1) != 0)) //�����ϸ������Һ����
				{
					ImageDeal[Ysite].RightTemp = Xsite;
					if ((-ImageDeal[Ysite].RightTemp + ImageDeal[Ysite].RightBorder) >= ObstacleFirstInternal)
						ImageDeal[Ysite].IsRightFind = 'P';  ////�ϰ�����һ�ߵļ�϶�����޶�ֵ
					break;
				}
			}
		if (ImageDeal[Ysite].IsLeftFind == 'P' && ImageDeal[Ysite].IsRightFind != 'P')
			IsObstacleFind = 'L';
		else if (ImageDeal[Ysite].IsLeftFind != 'P' && ImageDeal[Ysite].IsRightFind == 'P')
			IsObstacleFind = 'R';
		if (IsObstacleFind != 'F')
		{
			ImageDeal[Ysite].IsLeftFind = 'T';
			ImageDeal[Ysite].IsRightFind = 'T';
			break;
		}
	}
	if (IsObstacleFind == 'L')
	{
		ytemp = Ysite;
		if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].Center) <= ObstacleInternal)return;
		for (; ytemp <= (Ysite + 4); ytemp++)
		{
			PicTemp = PicForNow[ytemp];
			ImageDeal[ytemp].LeftTemp = ImageDeal[ytemp].LeftBorder;
			for (xtemp = (ImageDeal[ytemp].Center - 1); xtemp >= ImageDeal[ytemp].LeftBorder; xtemp--)
			{
				if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp + 1) != 0)
				{
					ImageDeal[ytemp].LeftTemp = xtemp;
					break;
				}
			}
			ImageDeal[ytemp].LeftBorder = ImageDeal[ytemp].LeftTemp;
			for (xtemp = (ImageDeal[ytemp].Center + 1); xtemp <= ImageDeal[ytemp].RightBorder; xtemp++)
			{
				if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp - 1) != 0)
				{
					ImageDeal[ytemp].RightTemp = xtemp;
					break;
				}
			}
			if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].RightTemp) >= ObstacleInternal)
				return;
			if (ytemp >= (Ysite + 1))
				if ((ImageDeal[ytemp - 1].LeftTemp - ImageDeal[ytemp].LeftTemp) >= ObstacleInternal)
				{
					break;
				}
		}
		if (ytemp <= (Ysite + 3))
		{
			ytemp = Ysite;
			for (; ytemp >= ImageStatus.OFFLine; ytemp--)
			{
				PicTemp = PicForNow[ytemp];
				for (xtemp = (ImageDeal[ytemp].Center - 1); xtemp >= ImageDeal[ytemp].LeftBorder; xtemp--)
				{
					if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp + 1) != 0)
					{
						ImageDeal[ytemp].LeftTemp = xtemp;
						break;
					}
				}
				for (xtemp = (ImageDeal[ytemp].Center + 1); xtemp <= ImageDeal[ytemp].RightBorder; xtemp++)
				{
					if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp - 1) != 0)
					{
						ImageDeal[ytemp].RightTemp = xtemp;
						break;
					}
				}
				if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].RightTemp) >= ObstacleInternal)
					return;
				if (ytemp <= (Ysite - 1))
				{
					if ((ImageDeal[ytemp + 1].LeftTemp - ImageDeal[ytemp].LeftTemp) >= ObstacleInternal)
					{
						break;
					}
				}
			}
			if (ytemp <= (Ysite - 5))
				RoadType = Obstacle;
		}
	}
	else if (IsObstacleFind == 'R')
	{
		ytemp = Ysite;
		if ((ImageDeal[ytemp].Center - ImageDeal[ytemp].LeftBorder) <= ObstacleInternal)return;
		for (; ytemp <= (Ysite + 4); ytemp++)
		{
			PicTemp = PicForNow[ytemp];
			for (xtemp = (ImageDeal[ytemp].Center + 1); xtemp <= ImageDeal[ytemp].RightBorder; xtemp++)
			{
				if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp - 1) != 0)
				{
					ImageDeal[ytemp].RightTemp = xtemp;
					break;
				}
			}
			for (xtemp = (ImageDeal[ytemp].Center - 1); xtemp >= ImageDeal[ytemp].LeftBorder; xtemp--)
			{
				if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp + 1) != 0)
				{
					ImageDeal[ytemp].LeftTemp = xtemp;
					break;
				}
			}
			if ((ImageDeal[ytemp].LeftTemp - ImageDeal[ytemp].LeftBorder) >= ObstacleInternal)
				return;
			if (ytemp >= (Ysite + 1))
				if ((ImageDeal[ytemp].RightTemp - ImageDeal[ytemp - 1].RightTemp) >= ObstacleInternal)
				{
					break;
				}
		}
		if (ytemp <= (Ysite + 3))
		{
			ytemp = Ysite;
			for (; ytemp >= ImageStatus.OFFLine; ytemp--)
			{
				PicTemp = PicForNow[ytemp];
				for (xtemp = (ImageDeal[ytemp].Center + 1); xtemp <= ImageDeal[ytemp].RightBorder; xtemp++)
				{
					if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp - 1) != 0)
					{
						ImageDeal[ytemp].RightTemp = xtemp;
						break;
					}
				}
				for (xtemp = (ImageDeal[ytemp].Center - 1); xtemp >= ImageDeal[ytemp].LeftBorder; xtemp--)
				{
					if (*(PicTemp + xtemp) == 0 && *(PicTemp + xtemp + 1) != 0)
					{
						ImageDeal[ytemp].LeftTemp = xtemp;
						break;
					}
				}
				if ((ImageDeal[ytemp].LeftTemp - ImageDeal[ytemp].LeftBorder) >= ObstacleInternal)
					return;
				if (ytemp <= (Ysite - 1))
				{
					if ((ImageDeal[ytemp].RightTemp - ImageDeal[ytemp + 1].RightTemp) >= ObstacleInternal)
					{
						break;
					}
				}
			}
			if (ytemp <= (Ysite - 5))
				RoadType = Obstacle;
		}
	}
	if (RoadType == Obstacle)
	{
		if (IsObstacleFind == 'R')
		{
			ImageStatus.ObstacleCenter = ImageInfo.RObstacleCenter;
			ImageStatus.ObstacleType = 'R';
		}
		else if (IsObstacleFind == 'L')
		{
			ImageStatus.ObstacleCenter = ImageInfo.LObstacleCenter;
			ImageStatus.ObstacleType = 'L';
		}
		ImageStatus.ObstacleOverDistance = TruePicToYSite(Ysite, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle);
		ImageStatus.ObstacleDelay = 'T';
		LastObstacleMask = 'T';
		ImageStatus.ObstacleDistance = CarInfo.length;
		LastObstacleDitance = CarInfo.length;
	}
}



static void ObstacleClear(void)
{
	static uint8 ObstacleOutMask = 'F';
	static float ObstacleOutDistance = 0;
	if (ImageStatus.ObstacleDelay == 'T')
	{
		for (Ysite = 55; Ysite >= 40; Ysite--)
		{
			if (ImageStatus.ObstacleType == 'R')
			{
				if (ImageDeal[Ysite].IsRightFind == 'W' && ImageDeal[Ysite + 1].IsRightFind == 'T')
				{
					PicTemp = PicForNow[Ysite];
					for (Xsite = ImageDeal[Ysite].Center; Xsite <= 78; Xsite++)
					{
						if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite - 1) != 0)
						{
							ImageDeal[Ysite].RightTemp = Xsite;
							ImageDeal[Ysite].IsRightFind = 'T';
							break;
						}
					}
					if (ImageDeal[Ysite].IsRightFind == 'T' && (ImageDeal[Ysite].RightTemp - ImageDeal[Ysite + 1].RightBorder) >= 10)
					{
						ObstacleOutMask = 'T';
						ObstacleOutDistance = CarInfo.length;
					}
					break;
				}
			}
			else if (ImageStatus.ObstacleType == 'L')
			{
				if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite + 1].IsLeftFind == 'T')
				{
					PicTemp = PicForNow[Ysite];
					for (Xsite = ImageDeal[Ysite].Center; Xsite >= 1; Xsite--)
					{
						if (*(PicTemp + Xsite) == 0 && *(PicTemp + Xsite + 1) != 0)
						{
							ImageDeal[Ysite].LeftTemp = Xsite;
							ImageDeal[Ysite].IsLeftFind = 'T';
							break;
						}
					}
					if (ImageDeal[Ysite].IsLeftFind == 'T' && (-ImageDeal[Ysite].LeftTemp + ImageDeal[Ysite + 1].LeftBorder) >= ObstacleOutClearInternal)
					{
						ObstacleOutMask = 'T';
						ObstacleOutDistance = CarInfo.length;
					}
					break;
				}
			}
		}
		if (ObstacleOutMask == 'T')
		{
			if ((CarInfo.length - ObstacleOutDistance) >= ObstacleOutDelay)
			{
				ObstacleOutMask = 'F';
				ImageStatus.ObstacleDelay = 'F';
			}
		}
	}
}



static void ShiftCenter()
{
	uint8 BorderWhite = 0;
	uint8 IsCorrect = 'F';
	if (ImageStatus.OFFLine < 50)
	{
		for (ytemp = ImageStatus.OFFLine; ytemp < (ImageStatus.OFFLine + 10); ytemp++)
		{
			if (ImageDeal[ytemp].IsLeftFind == 'W' && ImageDeal[ytemp].IsRightFind == 'W')
				break;
			else if (ImageDeal[ytemp].IsLeftFind == 'W')
			{
				while (ytemp < (ImageStatus.OFFLine + 10))
				{
					if (ImageDeal[ytemp].IsRightFind == 'W')
						break;
					ytemp++;
					if (ytemp == (ImageStatus.OFFLine + 10))
						IsCorrect = 'L';
				}
			}
			else if (ImageDeal[ytemp].IsRightFind == 'W')
			{
				while (ytemp < (ImageStatus.OFFLine + 10))
				{
					if (ImageDeal[ytemp].IsLeftFind == 'W')
						break;
					ytemp++;
					if (ytemp == (ImageStatus.OFFLine + 10))
						IsCorrect = 'R';
				}
			}
		}

		if (IsCorrect == 'R')
		{
			for (Ysite = ImageStatus.OFFLine; Ysite <= 55; Ysite++)
			{
				if ((ImageDeal[Ysite].IsLeftFind == 'W' && Ysite <= 45) || ImageDeal[Ysite].RightBorder <= 60)return;
				if (ImageDeal[Ysite].IsRightFind == 'W')
					BorderWhite++;
				else if (ImageDeal[Ysite].IsRightFind == 'T')
					BorderWhite = 0;
				if (BorderWhite <= 5)continue;
				if (ImageDeal[Ysite].IsRightFind == 'W' && ImageDeal[Ysite + 1].IsRightFind == 'T' && ImageDeal[Ysite + 2].IsRightFind == 'T')
				{
					for (ytemp = (Ysite + 1); ytemp >= ImageStatus.OFFLine; ytemp--)
					{
						ImageDeal[ytemp].RightBorder = ImageDeal[ytemp + 1].RightBorder + (ImageDeal[ytemp].LeftBorder - ImageDeal[ytemp + 1].LeftBorder);
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].RightBorder + ImageDeal[ytemp].LeftBorder) / 2;
						if (ImageDeal[ytemp].Center > 79)
						{
							ImageDeal[ytemp].Center = 78;
							ImageStatus.OFFLine = ytemp;
							break;
						}
						else if (ImageDeal[ytemp].Center < 2)
						{
							ImageDeal[ytemp].Center = 1;
							ImageStatus.OFFLine = ytemp;
							break;
						}
					}
					break;
				}
			}
		}
		else if (IsCorrect == 'L')
		{
			for (Ysite = ImageStatus.OFFLine; Ysite <= 55; Ysite++)
			{
				if ((ImageDeal[Ysite].IsRightFind == 'W' && Ysite <= 45) || ImageDeal[Ysite].LeftBorder >= 20)return;
				if (ImageDeal[Ysite].IsLeftFind == 'W')
					BorderWhite++;
				else if (ImageDeal[Ysite].IsLeftFind == 'T')
					BorderWhite = 0;
				if (BorderWhite <= 5)continue;


				if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite + 1].IsLeftFind == 'T' && ImageDeal[Ysite + 2].IsLeftFind == 'T')
				{
					for (ytemp = (Ysite + 1); ytemp >= ImageStatus.OFFLine; ytemp--)
					{
						ImageDeal[ytemp].LeftBorder = ImageDeal[ytemp + 1].LeftBorder + (ImageDeal[ytemp].RightBorder - ImageDeal[ytemp + 1].RightBorder);
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].RightBorder + ImageDeal[ytemp].LeftBorder) / 2;
						if (ImageDeal[ytemp].Center > 79)
						{
							ImageDeal[ytemp].Center = 78;
							ImageStatus.OFFLine = ytemp;
							break;
						}
						else if (ImageDeal[ytemp].Center < 1)
						{
							ImageDeal[ytemp].Center = 1;
							ImageStatus.OFFLine = ytemp;
							break;
						}
					}
					break;
				}
			}
		}
	}
}




static void RouteFilter(void)
{
	for (Ysite = 58; Ysite >= (ImageStatus.OFFLine + 1); Ysite--)
	{
		//////Բ����                     ////Բ�����                     ////Բ������
		if (ImageStatus.CirqueOut == 'F' && ImageStatus.CirquePass == 'F' && ImageStatus.IsCinqueOutIn == 'F')
		{
			if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W')
			{
				ytemp = Ysite;
				while (ytemp >= (ImageStatus.OFFLine + 1))
				{
					ytemp--;
					if (ImageDeal[ytemp].IsLeftFind == 'T' && ImageDeal[ytemp].IsRightFind == 'T')
					{
						DetR = (float)(ImageDeal[ytemp - 1].Center - ImageDeal[Ysite + 2].Center) / (float)(ytemp - 1 - Ysite - 2);
						int CenterTemp = ImageDeal[Ysite + 2].Center;
						int LineTemp = Ysite + 2;
						while (Ysite >= ytemp)
						{
							ImageDeal[Ysite].Center = (int)(CenterTemp + DetR * (float)(Ysite - LineTemp));
							Ysite--;
						}
						break;
					}
				}
			}
		}
		ImageDeal[Ysite].Center = (ImageDeal[Ysite - 1].Center + ImageDeal[Ysite].Center + ImageDeal[Ysite + 1].Center) / 3;
	}
}




static void GetTrueSite(void)
{
	for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		TrueYSite[Ysite] = TruePicToYSite(Ysite, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle);
		if (ImageStatus.ObstacleDelay == 'T')
			TrueXSite[Ysite] = TruePicToXSite((ImageDeal[Ysite].Center - (79 - ImageStatus.ObstacleCenter)), TrueYSite[Ysite], ImageInfo.CameraCalAngle);
		else if (RunningData.IsStart == 0 && CarInfo.StartPass == 'T')
			TrueXSite[Ysite] = TruePicToXSite((ImageDeal[Ysite].Center - (79 - ImageInfo.StartCenter)), TrueYSite[Ysite], ImageInfo.CameraCalAngle);
		else
			TrueXSite[Ysite] = TruePicToXSite(((float)ImageDeal[Ysite].Center - ImageInfo.DirCenter), TrueYSite[Ysite], ImageInfo.CameraCalAngle);
		if (TrueXSite[Ysite] >= 50 || TrueXSite[Ysite] <= -50)  //////�߽�����
		{
			ImageStatus.OFFLine = Ysite;
			break;
		}
	}
	//////��ֵ�˲�
	for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		if (Ysite == 59)
			TrueXSite[Ysite] = (2 * TrueXSite[Ysite] + TrueXSite[Ysite - 1]) / 3;
		else if (Ysite == ImageStatus.OFFLine)
			TrueXSite[Ysite] = (2 * TrueXSite[Ysite] + TrueXSite[Ysite + 1]) / 3;
		else
			TrueXSite[Ysite] = (TrueXSite[Ysite] + TrueXSite[Ysite - 1] + TrueXSite[Ysite + 1]) / 3;
	}///////��ֵ�˲�

	ImageStatus.OFFRoute = TrueYSiteToPic(ImageStatus.OFFLine, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle);
}




static void CircleTest()   ////Բ�����
{
	float BlackPoint = 0;
	float WhitePoint = 0;
	uint8 JumpPoint = 0;
	float Complexity = 0;
	uint8  Cirquereliable = 0;
	uint8 IsFind = 'F';
	int   CirqueLine = 0;
	for (Ysite = 55; Ysite >= (ImageStatus.OFFLine); Ysite--)
	{
		if (Ysite > 50)
		{
			if (ImageDeal[Ysite].IsLeftFind == 'W' && ImageDeal[Ysite].IsRightFind == 'W')
				break;  //////////������Լ��ұ��Ҳ���ʱֱ������
			if ((ImageDeal[Ysite - 1].LeftBorder - ImageDeal[Ysite].LeftBorder) < -5
				|| (ImageDeal[Ysite - 1].RightBorder - ImageDeal[Ysite].RightBorder) > 5)
				break;//////////����������������ʱ ���ߵ��ұ������������ʱ
			else if (ImageDeal[Ysite].Center > 55 || ImageDeal[Ysite].Center < 25)
				break;//////������ƫ�ƹ���ʱ
		}/////�����������ʱ�򲻽���Բ�����
		else
		{
			while (Ysite >= (ImageStatus.OFFLine))
			{
				if (ImageDeal[Ysite].Center > 60 || ImageDeal[Ysite].Center < 20)
					break;//////������ƫ�ƹ���ʱֱ������
				if (ImageDeal[Ysite].IsLeftFind == 'W'
					&& ImageDeal[Ysite + 1].IsLeftFind == 'T'
					&& ImageDeal[Ysite + 2].IsLeftFind == 'T'
					&& Ysite >= (ImageStatus.OFFLine + 5))///////�Ӳ����ߵ�����
				{
					for (ytemp = (Ysite + 5); ytemp >= (Ysite - 5); ytemp--)
					{
						if ((ImageDeal[ytemp].IsRightFind == 'W' && ImageDeal[ytemp + 1].IsRightFind == 'T')
							|| (ImageDeal[ytemp].RightBorder - ImageDeal[ytemp + 4].RightBorder) >= 4)
						{
							if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].LeftBorder) >= 25)
								IsFind = 'T';
							break;
						}
					}
					break;
				}
				if (ImageDeal[Ysite].IsRightFind == 'W'
					&& ImageDeal[Ysite + 1].IsRightFind == 'T'
					&& ImageDeal[Ysite + 2].IsRightFind == 'T'
					&& Ysite >= (ImageStatus.OFFLine + 5))
				{
					for (ytemp = (Ysite + 5); ytemp >= (Ysite - 5); ytemp--)
					{
						if ((ImageDeal[ytemp].IsLeftFind == 'W' && ImageDeal[ytemp + 1].IsLeftFind == 'T')
							|| (ImageDeal[ytemp].LeftBorder - ImageDeal[ytemp + 4].LeftBorder) <= -4)
						{
							if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].LeftBorder) >= 25)
								IsFind = 'T';
							break;
						}
					}
					break;
				}
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 4].RightBorder) >= 4 && Ysite >= (ImageStatus.OFFLine + 5))
				{
					for (ytemp = (Ysite + 5); ytemp >= (Ysite - 5); ytemp--)
					{
						if ((ImageDeal[ytemp].LeftBorder - ImageDeal[ytemp + 4].LeftBorder) <= -4)
						{
							if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].LeftBorder) >= 25)
								IsFind = 'T';
							break;
						}
					}
					break;
				}
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 4].LeftBorder) <= -4 && Ysite >= (ImageStatus.OFFLine + 5))
				{
					for (ytemp = (Ysite + 5); ytemp >= (Ysite - 5); ytemp--)
					{
						if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp + 4].RightBorder) >= 4)
						{
							if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp].LeftBorder) >= 25)
								IsFind = 'T';
							break;
						}
					}
					break;
				}
				Ysite--;
			}
			break;
		}///////�����ҵ�һ�����Խ�������ѡ��ĳ���λ��
	}   ////////���ƿ����ȡԲ������//////����Ҫ����һ�����Ըı䷽��ĳ��ڣ���������ᷢ����ʮ���Լ�Բ����ڴ�
	if (IsFind == 'T')
	{
		FTSite = Ysite + 3;
		TFSite = 59;
		CirqueLine = Ysite + 3;
		if ((CirqueLine - ImageStatus.OFFLine) <= 8) ////��Բ���붥�߹���ʱ�����м��
			return;
		else
		{
			for (Ysite = CirqueLine; Ysite <= (CirqueLine + 3); Ysite++)
			{
				if (ImageDeal[Ysite].IsLeftFind == 'W' || ImageDeal[Ysite].IsRightFind == 'W')
					return;/////�ӳ��߱�����ȷ�е������
			}
		}
		DetR = (float)(ImageDeal[FTSite].RightBorder - ImageDeal[TFSite].RightBorder) / (float)(FTSite - TFSite);
		DetL = (float)(ImageDeal[FTSite].LeftBorder - ImageDeal[TFSite].LeftBorder) / (float)(FTSite - TFSite);
		for (Ysite = FTSite; Ysite >= (ImageStatus.OFFLine + 3); Ysite--)
		{

			PicTemp = PicForNow[Ysite];
			ImageDeal[Ysite].RightTemp = (int)((float)(Ysite - TFSite) * DetR) + ImageDeal[TFSite].RightBorder;
			ImageDeal[Ysite].LeftTemp = (int)((float)(Ysite - TFSite) * DetL) + ImageDeal[TFSite].LeftBorder;
			ImageDeal[Ysite].CenterTemp = (ImageDeal[Ysite].RightTemp + ImageDeal[Ysite].LeftTemp) / 2;
			if (*(PicTemp + ImageDeal[Ysite].CenterTemp) == 0 && ImageDeal[Ysite].CenterTemp <= 60 && ImageDeal[Ysite].CenterTemp >= 20)
			{
				BlackPoint = 0;
				JumpPoint = 0;
				for (Xsite = (ImageDeal[Ysite].RightTemp - 1); Xsite >= (ImageDeal[Ysite].LeftTemp + 1); Xsite--)
				{
					if (*(PicTemp + Xsite) == 0)
						BlackPoint++;
					else if (*(PicTemp + Xsite) != *(PicTemp + Xsite + 1))
						JumpPoint++;
				}
				Complexity = BlackPoint / (float)(ImageDeal[Ysite].RightTemp - ImageDeal[Ysite].LeftTemp);
				if (JumpPoint <= 2 && Complexity >= 0.8)
				{
					Cirquereliable++;
					if (Cirquereliable >= 3)
					{
						JumpPoint = 0;
						for (Xsite = 0; Xsite <= 78; Xsite++)
						{
							if (*(PicTemp + Xsite) != *(PicTemp + Xsite + 1))
								JumpPoint++;
						}
						if (JumpPoint < 3)return;//////����4�����䣬Ҳ����������Բ���İ뾶�����80CM����
						RoadType = Cirque;
						ImageStatus.CirquePass = 'T';
						ImageStatus.CirqueType = 'F';
						break;
					}
				}
			}
			else
			{
				Cirquereliable = 0;
			}
		}/////������ʽ�ܹ���׼ȷ��ʶ���СԲ��
		if (RoadType != Cirque)
		{
			WhitePoint = 0;
			Cirquereliable = 0;
			BlackPoint = 0;
			for (Ysite = CirqueLine; Ysite >= ImageStatus.OFFLine; Ysite--)
			{
				if (ImageDeal[Ysite].IsRightFind == 'W' && ImageDeal[Ysite].IsLeftFind == 'W')
				{
					WhitePoint++;
					if (WhitePoint >= 5)
					{
						while (Ysite >= ImageStatus.OFFLine)
						{
							Ysite--;
							if (ImageDeal[Ysite].IsRightFind != 'W' || ImageDeal[Ysite].IsLeftFind != 'W')
							{
								if ((Ysite - ImageStatus.OFFLine) >= 2 || PicForNow[Ysite - 4][39] != 0)
								{
									WhitePoint = 0;////////������Ч�ж��в�Զ��Ҳ����˵��Ч�е�ǰ�������Ҳ�����
								}
								break;
							}
						}
						break;
					}
				}
				else
					WhitePoint = 0;
			}
			if (WhitePoint == 0)return;
			uint8  OutPoint = 0;
			for (ytemp = (ImageStatus.OFFLine + 5); ytemp >= (ImageStatus.OFFLine - 4); ytemp--)
			{
				PicTemp = PicForNow[ytemp];
				WhitePoint = 0;
				BlackPoint = 0;
				for (xtemp = 0; xtemp < 39; xtemp++)
				{
					if (*(PicTemp + xtemp + 39) != 0 && *(PicTemp - xtemp + 39) != 0)
						WhitePoint += 2;
					else
						break;
				}
				if (WhitePoint >= 60)
				{
					OutPoint++;    /////�ױ߳��ڴ�
					if (OutPoint >= 4)
					{
						ytemp--;
						while (ytemp >= (ImageStatus.OFFLine - 4))
						{
							PicTemp = PicForNow[ytemp];
							BlackPoint = 0;
							JumpPoint = 0;
							if (*(PicTemp + 39) == 0)
								for (xtemp = 0; xtemp < 39; xtemp++)
								{
									if (*(PicTemp + xtemp + 39) == 0 && *(PicTemp - xtemp + 39) == 0)
									{
										BlackPoint += 2;
									}
									else
										break;
								}
							if (BlackPoint >= 30)////���ĺ�Բ
							{
								Cirquereliable++;
								if (Cirquereliable >= 2)
								{
									RoadType = Cirque;
									ImageStatus.CirquePass = 'T';
									ImageStatus.CirqueType = 'T';
									return;
								}
							}
							else
								Cirquereliable = 0;
							ytemp--;
						}
						break;
					}
				}
				else
					OutPoint = 0;
			}
		}
	}
	else
	{
		return;
	}
}




static void CirqueOut(uint8 CirqueDir) /////��Բ��
{
	uint8 Reliable = 0;
	uint8 WhitePoint = 0;
	uint8 IsOutFind = 'F';
	static uint8 IsCirqueOut = 'F';
	static uint8 IsCirquePassAway = 'F';
	static float CirqueOutDistance = 0;

	if (CirqueDir == 0)
	{
		if (IsCirqueOut == 'F')
		{
			Reliable = 0;
			for (Ysite = 50; Ysite >= (ImageStatus.OFFLine); Ysite--)
			{
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 2].RightBorder) >= 2)
				{
					Reliable++;
					if (Reliable >= CiruqeOutReliableCount)
					{
						IsCirqueOut = 'T';
						ImageStatus.IsCinqueOutIn = 'T';
						break;
					}
				}
				else if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 1].LeftBorder) >= 10 && Ysite >= (ImageStatus.OFFLine + 5))
				{
					IsCirqueOut = 'T';
					ImageStatus.IsCinqueOutIn = 'T';
					break;
				}
				else
					Reliable = 0;
			}
			if (IsCirqueOut == 'T')
			{
				CirqueOutDistance = CarInfo.length;
			}
		}
		if (IsCirqueOut == 'T')
		{
			Reliable = 0;
			IsOutFind = 'F';
			for (Ysite = 55; Ysite >= 5; Ysite--)
			{
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 1].RightBorder) <= -10)
					ImageDeal[Ysite].RightBorder = ImageDeal[Ysite + 1].RightBorder;
				PicTemp = PicForNow[Ysite];
				JumpPointtypedef LeftTemp;
				GetJumpPointFromOther(PicTemp, &LeftTemp, 0, ImageDeal[Ysite].RightBorder);

				ImageDeal[Ysite].LeftBorder = LeftTemp.point;
				ImageDeal[Ysite].IsLeftFind = LeftTemp.type;

				ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 1].LeftBorder) >= 10 && IsOutFind == 'F')
				{
					IsOutFind = 'T';
					for (ytemp = Ysite; ytemp <= 57; ytemp++)
					{
						if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp + 2].RightBorder) <= -1
							&& ImageDeal[ytemp].IsRightFind == 'T')
						{
							Reliable++;

						}
						else
							Reliable = 0;

						if (ytemp == 57)
						{
							Reliable = CirqueOutWayReliableCount;
						}

						if (Reliable >= CirqueOutWayReliableCount)
						{
							ytemp = (Ysite + ytemp) / 2 + CirqueOutWayExtern;
							DetL = (float)(ImageDeal[ytemp].LeftBorder - ImageDeal[Ysite].LeftBorder) / (float)(ytemp - Ysite);
							for (; ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].LeftBorder = ImageDeal[Ysite].LeftBorder + (int)(DetL * (float)(ytemp - Ysite));
								ImageDeal[ytemp].Center = (ImageDeal[ytemp].RightBorder + ImageDeal[ytemp].LeftBorder) / 2;
								ImageDeal[ytemp].IsLeftFind = 'T';
							}
							break;
						}
					}
				}
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <= 10)
				{
					ImageStatus.OFFLine = Ysite;
					break;
				}
			}
			if ((CarInfo.length - CirqueOutDistance) >= CiruqeMaskClearDistance)
			{
				IsCirqueOut = 'F';
				IsCirquePassAway = 'F';
				ImageStatus.CirqueOut = 'F';
				ImageStatus.IsCinqueOutIn = 'F';
				return;
			}
			else if (IsCirquePassAway == 'F' && (CarInfo.length - RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]]) >= 0.3)
			{
				WhitePoint = 0;
				for (Ysite = 55; Ysite >= 45; Ysite--)
				{
					if (ImageDeal[Ysite].IsLeftFind == 'W')
					{
						WhitePoint++;
						if (WhitePoint >= 10)
						{
							IsCirquePassAway = 'T';
						}
					}
					else
						WhitePoint = 0;
				}
			}
			else if (IsCirquePassAway == 'T')
			{
				WhitePoint = 0;
				for (Ysite = 55; Ysite >= 45; Ysite--)
				{
					if (ImageDeal[Ysite].IsLeftFind == 'T')
					{
						WhitePoint++;
						if (WhitePoint >= 7)
						{
							IsCirqueOut = 'F';
							IsCirquePassAway = 'F';
							ImageStatus.CirqueOut = 'F';
							ImageStatus.IsCinqueOutIn = 'F';
							return;
						}
					}
					else
						WhitePoint = 0;
				}
			}
		}
	}
	else if (CirqueDir == 1)
	{
		if (IsCirqueOut == 'F')
		{
			Reliable = 0;
			for (Ysite = 50; Ysite >= (ImageStatus.OFFLine); Ysite--)
			{
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 2].LeftBorder) <= -2)
				{
					Reliable++;
					if (Reliable >= CiruqeOutReliableCount)
					{
						IsCirqueOut = 'T';
						ImageStatus.IsCinqueOutIn = 'T';
						break;
					}
				}
				else if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 1].RightBorder) <= -10 && Ysite >= (ImageStatus.OFFLine + 5))
				{
					IsCirqueOut = 'T';
					ImageStatus.IsCinqueOutIn = 'T';
					break;
				}
				else
					Reliable = 0;
			}
			if (IsCirqueOut == 'T')
			{
				CirqueOutDistance = CarInfo.length;
			}
		}
		if (IsCirqueOut == 'T')
		{
			Reliable = 0;
			IsOutFind = 'F';
			for (Ysite = 55; Ysite >= ImageStatus.OFFLine; Ysite--)
			{
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 1].LeftBorder) >= 10)
					ImageDeal[Ysite].LeftBorder = ImageDeal[Ysite + 1].LeftBorder;
				PicTemp = PicForNow[Ysite];
				JumpPointtypedef RightTemp;
				GetJumpPointFromOther(PicTemp, &RightTemp, 1, ImageDeal[Ysite].LeftBorder);

				ImageDeal[Ysite].RightBorder = RightTemp.point;
				ImageDeal[Ysite].IsRightFind = RightTemp.type;
				ImageDeal[Ysite].Center = (ImageDeal[Ysite].RightBorder + ImageDeal[Ysite].LeftBorder) / 2;

				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 1].RightBorder) <= -10 && IsOutFind == 'F')
				{
					IsOutFind = 'T';
					for (ytemp = Ysite; ytemp <= 57; ytemp++)
					{
						if ((ImageDeal[ytemp].LeftBorder - ImageDeal[ytemp + 2].LeftBorder) >= 1
							&& ImageDeal[ytemp].IsLeftFind == 'T')
						{
							Reliable++;
						}
						else
							Reliable = 0;

						if (ytemp == 57)
						{
							Reliable = CirqueOutWayReliableCount;
						}

						if (Reliable >= CirqueOutWayReliableCount)
						{
							ytemp = (Ysite + ytemp) / 2 + CirqueOutWayExtern;
							DetR = (float)(ImageDeal[ytemp].RightBorder - ImageDeal[Ysite].RightBorder) / (float)(ytemp - Ysite);
							for (; ytemp >= Ysite; ytemp--)
							{
								ImageDeal[ytemp].RightBorder = ImageDeal[Ysite].RightBorder + (int)(DetR * (float)(ytemp - Ysite));
								ImageDeal[ytemp].Center = (ImageDeal[ytemp].RightBorder + ImageDeal[ytemp].LeftBorder) / 2;
								ImageDeal[ytemp].IsRightFind = 'T';
							}
							break;
						}
					}
				}
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite].LeftBorder) <= 10)
				{
					ImageStatus.OFFLine = Ysite;
					break;
				}
			}
			if ((CarInfo.length - CirqueOutDistance) >= CiruqeMaskClearDistance)
			{
				IsCirqueOut = 'F';
				IsCirquePassAway = 'F';
				ImageStatus.CirqueOut = 'F';
				ImageStatus.IsCinqueOutIn = 'F';
				return;
			}
			else if (IsCirquePassAway == 'F' && (CarInfo.length - RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]]) >= 0.3)
			{
				WhitePoint = 0;
				for (Ysite = 55; Ysite >= 45; Ysite--)
				{
					if (ImageDeal[Ysite].IsRightFind == 'W')
					{
						WhitePoint++;
						if (WhitePoint >= 10)
						{
							IsCirquePassAway = 'T';
						}
					}
					else
						WhitePoint = 0;
				}
			}
			else if (IsCirquePassAway == 'T')
			{
				WhitePoint = 0;
				for (Ysite = 55; Ysite >= 45; Ysite--)
				{
					if (ImageDeal[Ysite].IsRightFind == 'T')
					{
						WhitePoint++;
						if (WhitePoint >= 7)
						{
							IsCirqueOut = 'F';
							IsCirquePassAway = 'F';
							ImageStatus.CirqueOut = 'F';
							ImageStatus.IsCinqueOutIn = 'F';
							return;
						}
					}
					else
						WhitePoint = 0;
				}
			}
		}
	}
}




static void CirquePass(uint8 CirqueDir)/////����Բ��
{
	uint8 IsTFSure = 'F';
	uint8 IsFTSure = 'F';
	uint8 OtherSiteWhitePoint = 0;
	static uint8 IsCirqueIn = 'F';
	static uint8 IsCirqueTurn = 'F';
	for (Ysite = 54; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		PicTemp = PicForNow[Ysite];
		if (CirqueDir == 0)
		{
			if (IsCirqueIn == 'F')   ////Բ��λ�ò���Բ����
			{
				if (ImageDeal[Ysite].IsLeftFind == 'W' && Ysite >= 45)
				{
					OtherSiteWhitePoint++;
					if (OtherSiteWhitePoint >= 10)
						IsCirqueIn = 'T';
				}
				else
					OtherSiteWhitePoint = 0;
			}
			else
			{
				if (ImageDeal[55].IsLeftFind == 'T'
					&& ImageDeal[54].IsLeftFind == 'T'
					&& ImageDeal[53].IsLeftFind == 'T')
				{
					IsCirqueIn = 'F';
					ImageStatus.CirquePass = 'F';
					ImageStatus.CirqueOut = 'T';
					return;
				}
			}
			if (IsTFSure == 'F')
			{
				if ((ImageDeal[Ysite].LeftBorder - ImageDeal[Ysite + 4].LeftBorder) <= -3)
				{
					TFSite = Ysite + 3;
					ImageStatus.CirquePassLine = Ysite;
					IsTFSure = 'T';
				}
				else if (ImageDeal[Ysite].IsLeftFind == 'W')
				{
					if (ImageDeal[Ysite].IsRightFind == 'W')
					{
						TFSite = Ysite + 3;
						ImageStatus.CirquePassLine = Ysite;
						IsTFSure = 'T';
					}
					else if (Ysite <= 30 && ImageStatus.OFFLine <= 30)
					{
						TFSite = Ysite + 3;
						ImageStatus.CirquePassLine = Ysite;
						IsTFSure = 'T';
					}
					else if (ImageStatus.OFFLine <= 30)
					{
						ytemp = Ysite;
						for (; ytemp >= 30; ytemp--)
						{
							if (ImageDeal[ytemp].IsLeftFind == 'T')
								break;
						}
						if (ytemp <= 30)
						{
							TFSite = Ysite + 3;
							ImageStatus.CirquePassLine = Ysite;
							IsTFSure = 'T';
						}
						else
							Ysite = ytemp;
					}
				}
			}
			else if (IsTFSure == 'T' && IsFTSure == 'F')
			{
				if (*(PicTemp + 39) == 0
					|| *(PicTemp + 38) == 0
					|| *(PicTemp + 37) == 0)
				{
					FTSite = Ysite;
					IsFTSure = 'T';
				}
			}
			if (IsTFSure == 'T' && IsFTSure == 'T' && FTSite <= 50)
			{
				if (IsCirqueTurn == 'F' && TFSite >= TrueYSiteToPic(ImageInfo.CirqueLimitRoute, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle))
				{
					IsCirqueTurn = 'T';
				}
				if (IsCirqueTurn == 'T')
				{
					uint8 IsLeftDone = 'F';
					DetL = (float)(39 - ImageDeal[TFSite].LeftBorder) / (float)(FTSite - TFSite);
					for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
					{
						ImageDeal[ytemp].LeftBorder = (int)((float)(ytemp - TFSite) * DetL) + ImageDeal[TFSite].LeftBorder;
						ImageDeal[ytemp].IsLeftFind = 'W';
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) / 2;
					}
					for (ytemp = (FTSite - 1); ytemp >= ImageStatus.OFFLine; ytemp--)
					{
						PicTemp = PicForNow[ytemp];
						ImageDeal[ytemp].IsLeftFind = 'W';
						JumpPointtypedef LeftTemp;
						GetJumpPointFromOther(PicTemp, &LeftTemp, 0, ImageDeal[ytemp].RightBorder);

						ImageDeal[ytemp].LeftBorder = LeftTemp.point;
						ImageDeal[ytemp].IsLeftFind = LeftTemp.type;
						if ((-ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) < 10)
						{
							ImageStatus.OFFLine = ytemp + 1;
							break;
						}
						if ((ImageDeal[ytemp + 1].LeftBorder - ImageDeal[ytemp].LeftBorder) >= 5)
						{
							ImageStatus.OFFLine = ytemp + 1;
							break;
						}
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) / 2;
						if (ImageDeal[ytemp].IsLeftFind == 'T' && ImageDeal[ytemp + 1].IsLeftFind == 'T' && IsLeftDone == 'F')
						{
							IsLeftDone = 'T';
							DetL = (float)(ImageDeal[ytemp].LeftBorder - ImageDeal[TFSite].LeftBorder) / (float)(ytemp - TFSite);
							for (Ysite = TFSite; Ysite >= ytemp; Ysite--)
							{
								ImageDeal[Ysite].LeftBorder = (int)((float)(Ysite - TFSite) * DetL) + ImageDeal[TFSite].LeftBorder;
								ImageDeal[Ysite].IsLeftFind = 'W';
								ImageDeal[Ysite].Center = (ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder) / 2;
							}
						}
					}

				}
				return;
			}
		}
		else if (CirqueDir == 1)
		{
			if (IsCirqueIn == 'F')   ////Բ��λ�ò���Բ����
			{
				if (ImageDeal[Ysite].IsRightFind == 'W'  && Ysite >= 45)
				{
					OtherSiteWhitePoint++;
					if (OtherSiteWhitePoint >= 10)
						IsCirqueIn = 'T';
				}
				else
					OtherSiteWhitePoint = 0;
			}
			else
			{
				if (ImageDeal[55].IsRightFind == 'T'
					&& ImageDeal[54].IsRightFind == 'T'
					&& ImageDeal[53].IsRightFind == 'T')
				{
					IsCirqueIn = 'F';
					ImageStatus.CirquePass = 'F';
					ImageStatus.CirqueOut = 'T';
					IsCirqueTurn = 'F';
					return;
				}
			}
			if (IsTFSure == 'F')
			{
				if ((ImageDeal[Ysite].RightBorder - ImageDeal[Ysite + 4].RightBorder) >= 3)
				{
					TFSite = Ysite + 3;
					ImageStatus.CirquePassLine = Ysite;
					IsTFSure = 'T';
				}
				else if (ImageDeal[Ysite].IsRightFind == 'W')
				{
					if (ImageDeal[Ysite].IsLeftFind == 'W')
					{
						TFSite = Ysite + 3;
						ImageStatus.CirquePassLine = Ysite;
						IsTFSure = 'T';
					}
					else if (Ysite <= 30 && ImageStatus.OFFLine <= 30)
					{
						TFSite = Ysite + 3;
						ImageStatus.CirquePassLine = Ysite;
						IsTFSure = 'T';
					}
					else if (ImageStatus.OFFLine <= 30)
					{
						ytemp = Ysite;
						for (; ytemp >= 30; ytemp--)
						{
							if (ImageDeal[ytemp].IsRightFind == 'T')
								break;
						}
						if (ytemp <= 30)
						{
							TFSite = Ysite + 3;
							ImageStatus.CirquePassLine = Ysite;
							IsTFSure = 'T';
						}
						else
							Ysite = ytemp;
					}
				}
			}
			else if (IsTFSure == 'T' && IsFTSure == 'F')
			{
				if (*(PicTemp + 39) == 0
					|| *(PicTemp + 40) == 0
					|| *(PicTemp + 41) == 0)
				{
					FTSite = Ysite;
					IsFTSure = 'T';
				}
			}
			if (IsTFSure == 'T' && IsFTSure == 'T' && FTSite <= 50)
			{
				if (IsCirqueTurn == 'F' && TFSite >= TrueYSiteToPic(ImageInfo.CirqueLimitRoute, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle))
				{
					IsCirqueTurn = 'T';
				}
				if (IsCirqueTurn == 'T')
				{
					uint8 IsRightDone = 'F';
					DetR = (float)(39 - ImageDeal[TFSite].RightBorder) / (float)(FTSite - TFSite);
					for (ytemp = TFSite; ytemp >= FTSite; ytemp--)
					{
						ImageDeal[ytemp].RightBorder = (int)((float)(ytemp - TFSite) * DetR) + ImageDeal[TFSite].RightBorder;
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) / 2;
						ImageDeal[ytemp].IsRightFind = 'F';
					}
					for (ytemp = (FTSite - 1); ytemp >= ImageStatus.OFFLine; ytemp--)
					{
						PicTemp = PicForNow[ytemp];
						JumpPointtypedef RightTemp;
						GetJumpPointFromOther(PicTemp, &RightTemp, 1, ImageDeal[ytemp].LeftBorder);

						ImageDeal[ytemp].RightBorder = RightTemp.point;
						ImageDeal[ytemp].IsRightFind = RightTemp.type;
						if ((-ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) < 10)
						{
							ImageStatus.OFFLine = ytemp + 1;
							break;
						}
						if ((ImageDeal[ytemp].RightBorder - ImageDeal[ytemp + 1].RightBorder) >= 5)
						{
							ImageStatus.OFFLine = ytemp + 1;
							break;
						}
						ImageDeal[ytemp].Center = (ImageDeal[ytemp].LeftBorder + ImageDeal[ytemp].RightBorder) / 2;
						if (ImageDeal[ytemp].IsRightFind == 'T' && ImageDeal[ytemp + 1].IsRightFind == 'T' && IsRightDone == 'F')
						{
							IsRightDone = 'T';
							DetR = (float)(ImageDeal[ytemp].RightBorder - ImageDeal[TFSite].RightBorder) / (float)(ytemp - TFSite);
							for (Ysite = TFSite; Ysite >= ytemp; Ysite--)
							{
								ImageDeal[Ysite].RightBorder = (int)((float)(Ysite - TFSite) * DetR) + ImageDeal[TFSite].RightBorder;
								ImageDeal[Ysite].IsRightFind = 'W';
								ImageDeal[Ysite].Center = (ImageDeal[Ysite].LeftBorder + ImageDeal[Ysite].RightBorder) / 2;
							}
						}
					}
				}
				return;
			}
		}
	}
}




//////�µ����
extern volatile uint8  SpecialOn;
extern volatile uint8  RampOn;
void RampTest(void)
{
	if (ImageStatus.RampWayFlag == 'F' && ImageStatus.OFFLine <= 40)
	{
		if (ImageStatus.WhiteLine >= 5)return;
		for (Ysite = 55; Ysite > (ImageStatus.OFFLine + 2); Ysite--)
		{
			if (ImageDeal[Ysite].IsLeftFind != 'T' || ImageDeal[Ysite].IsRightFind != 'T')
				break;
			if (ImageDeal[Ysite].Center >= 47 || ImageDeal[Ysite].Center <= 33)
				break;
		}
		if (Ysite == (ImageStatus.OFFLine + 2))
		{
			ImageStatus.LBorderK = 100 * (float)(ImageDeal[ImageStatus.OFFLine + 8].LeftBorder - ImageDeal[55].LeftBorder) / (float)(54 - ImageStatus.OFFLine);
			ImageStatus.RBorderK = 100 * (float)(ImageDeal[ImageStatus.OFFLine + 8].RightBorder - ImageDeal[55].RightBorder) / (float)(54 - ImageStatus.OFFLine);
		}
		else
			return;
		if (CarInfo.Motor_ON == 'F')return;
		if (ImageStatus.LBorderK >= 25 && ImageStatus.LBorderK <= (ImageInfo.RampLKMax - (ImageStatus.PitchAngle - 5) * 0.2)
			&& ImageStatus.RBorderK <= -25 && ImageStatus.RBorderK >= (ImageInfo.RampRKMIN + (ImageStatus.PitchAngle - 5) * 0.2))
			RoadType = Ramp;


		if (RoadType == Ramp)
		{
			ImageStatus.RampStartDistance = CarInfo.length;
			ImageStatus.RampWayFlag = 'T';
			SpecialData.End += 3;
		}
	}
	else if (ImageStatus.RampWayFlag == 'T')
	{
		if ((CarInfo.length - ImageStatus.RampStartDistance) <= 2.2)
			RoadType = Ramp;
		else
		{
			ImageStatus.RampWayFlag = 'F';
			SpecialOn = 'F';
			RampOn = 'F';
		}
	}
}



uint8 CirqueDirComfirm(void) /////Բ������ȷ��
{
	if (CarInfo.Motor_ON == 'F')return CirqueDirBox[0];
	if (RealRouteBox[RealCirqueType] >= 7)
		return CirqueDirBox[5];
	else
		return CirqueDirBox[RealRouteBox[RealCirqueType] - 1];
}



static uint8   StartCheck = 'F'; 
static uint8   CirqueOutCheck = 'F';
static uint8   CirqueNoCheckMask = 'F';
static float   CirqueOutDistance = 0;  ////��һ��Բ���ľ���
uint8 ImageClear(void)//////ͼ�����ݳ�ʼ��
{
	ImageStatus.PitchAngle = CarInfo.pitch;
	ImageInfo.CameraCalAngle = (ImageInfo.CameraZeroAngle + 90 - CarInfo.pitch) * PI / 180;////���㵱ǰ���������
	ImageInfo.CameraCalD = (ImageInfo.CameraAxasLength * sinf(ImageInfo.CameraCalAngle) + CarWheelRadius) / cos(PI - ImageInfo.CameraCalAngle - ImageInfo.CameraInterAngle);
	//////////ת���᳤
	ImageStatus.OFFLine = 5;   ///////���ֵ������ʵ����õ��������������
	ImageStatus.WhiteLine = 0;
	RoadType = Normal;
	for (Ysite = 59; Ysite >= ImageStatus.OFFLine; Ysite--)
	{
		ImageDeal[Ysite].IsLeftFind = 'F';
		ImageDeal[Ysite].IsRightFind = 'F';
		ImageDeal[Ysite].LeftBorder = 0;
		ImageDeal[Ysite].RightBorder = 79;
		ImageDeal[Ysite].LeftTemp = 0;
		ImageDeal[Ysite].RightTemp = 79;
	}/////////�߽����־λ��ʼ��
	ImageStatus.CirquePassLine = 59;
	if (StartCheck == 'F')
	{
		if ((CarInfo.StartPass == 'T' && (CarInfo.length - ImageStatus.StartDistance) >= 0.7) || CarInfo.length > 2.5)
		{
			StartCheck = 'T';
		}
	}
	if (CirqueOutCheck == 'T')
	{
		if ((CarInfo.length - CirqueOutDistance) >= 1)
			CirqueOutCheck = 'F';
	}


	if (SystemSettings.CrossMask == 'T' &&
            (RouteMask[RealCrossType] == 'T'
	     || RouteStatus[RealCrossType] == 'I'
                  ))
		return 'T';


	if (RouteMask[RealObstacleType] == 'F' 
		&& RouteMask[RealCrossType] == 'F'  
		&& ImageStatus.CirqueOut == 'F'   
		&& ImageStatus.IsCinqueOutIn == 'F' 
		&& ImageStatus.CirquePass == 'F'  
		&& (CarInfo.length >= RunningData.StartDistance || CarInfo.Motor_ON == 'F')
		&& CirqueOutCheck == 'F')
		return 'F';
	else
		return 'T';
}



void Image_MainDeal(void)
{
	CirqueNoCheckMask = ImageClear();
	if (ImageStatus.IsStartPass == 'T' && CarInfo.IsStop == 'F')
	{
		if ((CarInfo.length - ImageStatus.StartDistance) > CarInfo.StopDelay)//////����10CM�ľ����ó���ͷ
		{
			CarInfo.IsStop = 'T';
			ImageStatus.ObstacleDelay = 'F';
		}
	}
	else if (CarInfo.IsStop == 'T')
	{
	}


	if (DrawLinesFirst() == 'T')
	{
		DrawLinesProcess();
	}
	else
	{
		if (RoadType == Start && CarInfo.Motor_ON == 'T')
		{
			if (CarInfo.length > 10 && ImageStatus.IsStartPass == 'F')
			{
				ImageStatus.IsStartPass = 'T';
				ImageStatus.StartDistance = CarInfo.length;
			}
			else if (CarInfo.StartPass == 'F')
			{
				CarInfo.StartPass = 'T';
				ImageStatus.StartDistance = CarInfo.length;
			}
		}
		ImageStatus.ExpectCur = 0;
		return;
	}


	if (ImageStatus.IsStartPass == 'F' && (StartCheck == 'T' || CarInfo.Motor_ON == 'F'))
	{
		if (CirqueNoCheckMask == 'F')
		{
			CircleTest();
			if (RoadType == Cirque)
			{
				if (SpecialOn == 'T')
				{
					SpecialOn = 'F';
					RampOn = 'F';
				}
			}
		}
		else if (ImageStatus.CirqueOut == 'T')
		{
			CirqueOut(CirqueDirComfirm()); 
			if (ImageStatus.IsCinqueOutIn == 'T' && CirqueOutCheck == 'F')
			{
				CirqueOutCheck = 'T';
				CirqueOutDistance = CarInfo.length;
			}
		}

		if (ImageStatus.CirquePass == 'T') 
		{
			RoadType = Cirque;
		}

		if (RoadType != Cirque && ImageStatus.CirquePass == 'F')
		{
			DrawExtensionLine(CirqueDirComfirm()); 

			if (ImageStatus.CirqueOut == 'F'
				&& ImageStatus.IsStartPass == 'F'
				&& ImageStatus.RampWayFlag == 'F'
                                && RouteStatus[RealCrossType] == 'O')
				FeasibleRegionAnalysis(); 

			ObstacleClear();

			ShiftCenter();
		}


		if (RouteMaskClear() == 'T' && CarInfo.Motor_ON == 'T' && StartCheck == 'T' && ImageStatus.RampWayFlag == 'F')
		{
			RoadTypeTest();

			RouteTypeDeal(RoadType);
		}

		if (ImageStatus.CirquePass == 'T')
		{
			CirquePass(CirqueDirComfirm());
		}

		if (RouteStatus[RealCrossType] == 'I' && ImageStatus.CirqueOut == 'F' && ImageStatus.CirquePass == 'F')
			RoadType = Cross;
	}
	else if (ImageStatus.ObstacleDelay == 'T')
	{
		ObstacleClear();
	}

	RouteFilter();  

	GetTrueSite(); 

	if ((SystemSettings.RampCheck == 'T' && RampOn == 'T' && RoadType != Cirque && RoadType != Cross) || CarInfo.Motor_ON == 'F')
		RampTest();


	float  DetTemp = 0;
	float  TowPointSite = 0;
	float  TrueTowPoint = 0;
	float  SpeedGain = 0;

	SpeedGain = (CarInfo.speed_Mtrue - ImageInfo.TowPointAdjustTh) * ImageInfo.TowPointAdjust;
	if (SpeedGain >= ImageInfo.TowPointAdjustMax)SpeedGain = ImageInfo.TowPointAdjustMax;
	else if (SpeedGain <= ImageInfo.TowPointAdjustMin)SpeedGain = ImageInfo.TowPointAdjustMin;


	TowPointSite = ImageInfo.TowPointSite;


	if (ImageStatus.IsCinqueOutIn == 'T')
	{
		TowPointSite += ImageInfo.CirqueOutSpecial;
		if ((CarInfo.length - RealRouteDistance[RealCirqueType][RealRouteBox[RealCirqueType]]) >= LargeCirqueTest)
			TowPointSite += ImageInfo.LargeCirqueSpecial;
	}
	else if (ImageStatus.CirquePass == 'T')
		TowPointSite += ImageInfo.CirqueInSpecial;


	if (RoadType == Ramp)
		TowPointSite -= 5;
	else
		TowPointSite += SpeedGain;

	int TowSite = TrueYSiteToPic(TowPointSite, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle);

	if (TowSite >= 49)
		TowSite = 49;
	float UnitAll = 0;


	if ((TowSite - 10) >= ImageStatus.OFFLine)
	{
		for (Ysite = (TowSite - 10); Ysite < TowSite; Ysite++)
		{
			DetTemp += Weighting[TowSite - Ysite - 1] * TrueXSite[Ysite];
			UnitAll += Weighting[TowSite - Ysite - 1];
		}
		for (Ysite = (TowSite + 10); Ysite > TowSite; Ysite--)
		{
			DetTemp += Weighting[-TowSite + Ysite - 1] * TrueXSite[Ysite];
			UnitAll += Weighting[-TowSite + Ysite - 1];
		}
		DetTemp = (TrueXSite[TowSite] + DetTemp) / (UnitAll + 1);
		DetTemp = 2 * 100 * DetTemp / (TowPointSite * TowPointSite);
	}
	else if (TowSite > ImageStatus.OFFLine)
	{
		for (Ysite = ImageStatus.OFFLine; Ysite < TowSite; Ysite++)
		{
			DetTemp += Weighting[TowSite - Ysite - 1] * TrueXSite[Ysite];
			UnitAll += Weighting[TowSite - Ysite - 1];
		}
		for (Ysite = (TowSite + TowSite - ImageStatus.OFFLine); Ysite > TowSite; Ysite--)
		{
			DetTemp += Weighting[-TowSite + Ysite - 1] * TrueXSite[Ysite];
			UnitAll += Weighting[-TowSite + Ysite - 1];
		}
		DetTemp = (TrueXSite[TowSite] + DetTemp) / (UnitAll + 1);
		DetTemp = 2 * 100 * DetTemp / (TowPointSite * TowPointSite);
	}
	else if (ImageStatus.OFFLine < 49)
	{
		for (Ysite = (ImageStatus.OFFLine + 2); Ysite > ImageStatus.OFFLine; Ysite--)
		{
			DetTemp += Weighting[-TowSite + Ysite - 1] * TrueXSite[Ysite];
			UnitAll += Weighting[-TowSite + Ysite - 1];
		}
		DetTemp = (TrueXSite[ImageStatus.OFFLine] + DetTemp) / (UnitAll + 1);
		TrueTowPoint = TruePicToYSite(ImageStatus.OFFLine, ImageInfo.CameraCalD, ImageInfo.CameraCalAngle);
		DetTemp = 2 * 100 * DetTemp / (TrueTowPoint * TrueTowPoint);
	}
	else
		DetTemp = ImageStatus.ExpectCur;
	if (ImageStatus.ObstacleDelay == 'T' && ImageStatus.ObstacleOverDistance <= 60)
	{
		DetTemp *= ImageInfo.ObstacleGain;
	}
	else if (ImageStatus.CirqueType == 'T' && ImageStatus.CirquePass == 'T')
		DetTemp *= 1.1;////��û��һ��
	ImageStatus.ExpectCur = DetTemp;
}
