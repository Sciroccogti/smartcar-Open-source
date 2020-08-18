/*
* @file       ProjectMath.c
* @brief      工程函数库
* @author     中南大学黄竞辉
* @version    v1.0
* @date       2016-12-13
*/

#include "GeneralDeclare.h"
#include "ProjectMath.h"
#include "FLASH.h"
#include "Terminator.h"
#include "SystemControl.h"
#include "RoadLoad.h"

void UnZip(uint8 *dst, uint8 *src, uint32 srclen)
{
	uint8_t colour[2] = { 0xff, 0x00 }; //0 和 1 分别对应的颜色///山外摄像头，原数据为0为白
	uint8_t * mdst = dst;
	uint8_t * msrc = src;
	uint8_t tmpsrc;
	while (srclen--)
	{
		tmpsrc = *msrc++;
		*mdst++ = colour[(tmpsrc >> 7) & 0x01];
		*mdst++ = colour[(tmpsrc >> 6) & 0x01];
		*mdst++ = colour[(tmpsrc >> 5) & 0x01];
		*mdst++ = colour[(tmpsrc >> 4) & 0x01];
		*mdst++ = colour[(tmpsrc >> 3) & 0x01];
		*mdst++ = colour[(tmpsrc >> 2) & 0x01];
		*mdst++ = colour[(tmpsrc >> 1) & 0x01];
		*mdst++ = colour[(tmpsrc >> 0) & 0x01];
	}
}

int ABS(int data)
{
	return ((data >= 0) ? data : (-data));
}

float ABSF(float data)
{
	return ((data >= 0) ? data : (-data));
}

int ABSDet(int datai, int dataj)
{
	int temp = datai - dataj;
	return ((temp >= 0) ? temp : (-temp));
}

/*
void FloattoUint32(float value,uint8 *buffer)
{
	uint8 *point;
	point = (uint8*)(&value);
	buffer[0] = point[0];
	buffer[1] = point[1];
	buffer[2] = point[2];
	buffer[3] = point[3];
}

void Uint32toFloat(uint8 *buffer,float * result)
{
	*result = *((float *)buffer);
	return;
}
*/

float Uint32toFloat(uint32 value)
{
	float n;
	int32 buff;
	if (value & 0x80000000)  //原来存入的数小于零
	{
		uint32 i = 1;
		value &= ~(i << 31);
		buff = (int32)value;
		n = (float)-buff / 10000;
	}
	else
	{
		n = (float)value / 10000;
	}
	return n;
}

uint32 FloattoUint32(float value)
{
	uint32 n;
	value *= 10000;
	if (value < 0)
	{
		uint32 i = 1;
		value = -value;
		n = (uint32)value;
		n |= (i << 31);
	}
	else
	{
		n = (uint32)value;
	}
	return n;
}


extern FLASH_WRITE_TYPE FLASH_DATA[FLASHNUM];
void Data_Convert_Float(void)
{
	RunningData.ExpectAngleMax = Uint32toFloat(FLASH_DATA[0]);
	RunningData.ExpectAngleMin = Uint32toFloat(FLASH_DATA[1]);
	RunningData.ExpectBalanceAngle = Uint32toFloat(FLASH_DATA[2]);
	RunningData.ExpectRampAngle = Uint32toFloat(FLASH_DATA[3]);
	Gyro.Xoffset = Uint32toFloat(FLASH_DATA[4]);
	Gyro.Yoffset = Uint32toFloat(FLASH_DATA[5]);
	Gyro.Zoffset = Uint32toFloat(FLASH_DATA[6]);
	SpeedControl.P = Uint32toFloat(FLASH_DATA[7]);
	RunningData.ExpectSpeed = Uint32toFloat(FLASH_DATA[8]);
	RunningData.StartAngle = Uint32toFloat(FLASH_DATA[9]);
	RunningData.StartDistance = Uint32toFloat(FLASH_DATA[10]);
	pitchPIDcontrol.P = Uint32toFloat(FLASH_DATA[11]);
	pitchPIDcontrol.D = Uint32toFloat(FLASH_DATA[12]);

        CarInfo.WheelType = FLASH_DATA[13];
        
	yawPIDcontrol[0].P = Uint32toFloat(FLASH_DATA[20]);
	yawPIDcontrol[0].D = Uint32toFloat(FLASH_DATA[21]);
	yawPIDcontrol[1].P = Uint32toFloat(FLASH_DATA[22]);
	yawPIDcontrol[1].D = Uint32toFloat(FLASH_DATA[23]);
	yawPIDcontrol[0].I = Uint32toFloat(FLASH_DATA[24]);
	yawPIDcontrol[1].I = Uint32toFloat(FLASH_DATA[25]);
	SpeedRadius.P = Uint32toFloat(FLASH_DATA[26]);
	SpeedRadius.D = Uint32toFloat(FLASH_DATA[27]);


	CarInfo.StopDelay = Uint32toFloat(FLASH_DATA[28]);
	CarInfo.ForceStopR = Uint32toFloat(FLASH_DATA[29]);
	CarInfo.ForceStopL = Uint32toFloat(FLASH_DATA[30]);
	CarInfo.MinR = Uint32toFloat(FLASH_DATA[31]);
	CarInfo.MaxR = Uint32toFloat(FLASH_DATA[32]);
	CarInfo.Run_Distance = Uint32toFloat(FLASH_DATA[33]);
	CarInfo.SpeedLimitGrowTh = Uint32toFloat(FLASH_DATA[34]);
	CarInfo.SpeedLimitReduceTh = Uint32toFloat(FLASH_DATA[35]);
	CarInfo.MaxAcc = Uint32toFloat(FLASH_DATA[36]);
	CarInfo.LimitRmax = Uint32toFloat(FLASH_DATA[37]);


	ImageInfo.DirCenter = Uint32toFloat(FLASH_DATA[38]);
	ImageInfo.CirqueLimitRoute = Uint32toFloat(FLASH_DATA[39]);
	ImageInfo.RObstacleCenter = Uint32toFloat(FLASH_DATA[40]);
	ImageInfo.LObstacleCenter = Uint32toFloat(FLASH_DATA[41]);
	ImageInfo.CameraAxasLength = Uint32toFloat(FLASH_DATA[42]);
	ImageInfo.CameraFocusLength = Uint32toFloat(FLASH_DATA[43]);
	ImageInfo.CameraInterAngle = Uint32toFloat(FLASH_DATA[44]);
	ImageInfo.CameraZeroAngle = Uint32toFloat(FLASH_DATA[45]);
	ImageInfo.threshold = (uint8)FLASH_DATA[46];
	ImageInfo.TowPointSite = Uint32toFloat(FLASH_DATA[47]);
	ImageInfo.LargeCirqueSpecial = Uint32toFloat(FLASH_DATA[49]);
	ImageInfo.ObstacleDelay = Uint32toFloat(FLASH_DATA[50]);
	ImageInfo.TowPointAdjustMax = Uint32toFloat(FLASH_DATA[51]);
	ImageInfo.StartCenter = Uint32toFloat(FLASH_DATA[52]);
	ImageInfo.CirqueInSpecial = Uint32toFloat(FLASH_DATA[53]);
	ImageInfo.CirqueOutSpecial = Uint32toFloat(FLASH_DATA[54]);
	ImageInfo.TowPointAdjustMin = Uint32toFloat(FLASH_DATA[55]);
	ImageInfo.TowPointAdjustTh = Uint32toFloat(FLASH_DATA[56]);
	ImageInfo.TowPointAdjust = Uint32toFloat(FLASH_DATA[57]);
	ImageInfo.RampLKMax = Uint32toFloat(FLASH_DATA[58]);
	ImageInfo.RampRKMIN = Uint32toFloat(FLASH_DATA[59]);


	SystemSettings.Wifi_ON = FLASH_DATA[60];
	SystemSettings.ResetTest = FLASH_DATA[61];
	SystemSettings.Wifi_SendWave = FLASH_DATA[62];
	SystemSettings.Run_DistanceLimit = FLASH_DATA[63];
	SystemSettings.StartCheck = FLASH_DATA[64];
	SystemSettings.CrossCheck = FLASH_DATA[65];
	SystemSettings.RampCheck = FLASH_DATA[66];
	SystemSettings.SpeedLimit = FLASH_DATA[67];
	SystemSettings.CrossMask = FLASH_DATA[68];
	ImageInfo.ObstacleGain = Uint32toFloat(FLASH_DATA[69]);


	for (uint8 i = 0; i < 6; i++)
		CirqueDirBox[i] = (uint8)FLASH_DATA[70 + i];


	for (uint8 k = 0; k < 8; k++)
	{
		PicErr[k] = Uint32toFloat(FLASH_DATA[80 + k]);
		ExpectCurvature[k] = Uint32toFloat(FLASH_DATA[88 + k]);
	}

	for (uint8 j = 0; j < 10; j++)
	{
		SpecialRoute[j].Mask = FLASH_DATA[100 + j * 7];
		SpecialRoute[j].Road = (RoadType_e)FLASH_DATA[100 + j * 7 + 1];
		SpecialRoute[j].RoadNum = FLASH_DATA[100 + j * 7 + 2];
		SpecialRoute[j].StartDistance = Uint32toFloat(FLASH_DATA[100 + j * 7 + 3]);
		SpecialRoute[j].EndDistance = Uint32toFloat(FLASH_DATA[100 + j * 7 + 4]);
		SpecialRoute[j].Angle = Uint32toFloat(FLASH_DATA[100 + j * 7 + 5]);
		SpecialRoute[j].IsRamp = FLASH_DATA[100 + j * 7 + 6];
	}

	for (uint8 i = 0; i < 6; i++)
		CrossBox[i] = FLASH_DATA[200 + i];
}

void Default_Settings(void)
{
	RunningData.ExpectAngleMax = 26;
	RunningData.ExpectAngleMin = 13;
	RunningData.ExpectBalanceAngle = 21;
	RunningData.ExpectRampAngle = 5;
	Gyro.Xoffset = 0;
	Gyro.Yoffset = 0;
	Gyro.Zoffset = 0;
	SpeedControl.P = 1.5;
	RunningData.ExpectSpeed = 300;
	RunningData.StartAngle = 26;
	RunningData.StartDistance = 2;
	pitchPIDcontrol.P = 180;
	pitchPIDcontrol.D = 3.5;


	yawPIDcontrol[0].P = 4.5;
	yawPIDcontrol[0].D = 0.2;
	yawPIDcontrol[1].P = 4.5;
	yawPIDcontrol[1].D = 0.232;
	yawPIDcontrol[0].I = 0;
	yawPIDcontrol[1].I = 0;
	SpeedRadius.P = 0.0232;
	SpeedRadius.D = 1.5;

	CarInfo.ForceStopR = 0;
	CarInfo.ForceStopL = 0;
	CarInfo.MinR = 45;
	CarInfo.MaxR = 49;
	CarInfo.Run_Distance = 80;
	CarInfo.SpeedLimitGrowTh = 130;
	CarInfo.SpeedLimitReduceTh = -100;
	CarInfo.MaxAcc = 17.8;
	CarInfo.LimitRmax = 50;

	ImageInfo.CirqueLimitRoute = 80;
	ImageInfo.RObstacleCenter = 33.3;
	ImageInfo.LObstacleCenter = 44.5;
	ImageInfo.CameraAxasLength = 23.5;
	ImageInfo.CameraFocusLength = 0.195;
	ImageInfo.CameraInterAngle = 1.25;
	ImageInfo.CameraZeroAngle = 0;
	ImageInfo.threshold = 70;
	ImageInfo.TowPointSite = 40;
	ImageInfo.LargeCirqueSpecial = -3;
	ImageInfo.ObstacleDelay = 0.9;
	ImageInfo.BodyLimit = 0;
	ImageInfo.TowPointAdjustTh = 290;
	ImageInfo.TowPointAdjust = 0.8;
	ImageInfo.RampLKMax = 48;
	ImageInfo.RampRKMIN = -48;
	ImageInfo.TowPointAdjustMax = 5;
	ImageInfo.TowPointAdjustMin = -2;
	ImageInfo.CirqueInSpecial = -3;
	ImageInfo.CirqueOutSpecial = 1;
	ImageInfo.ObstacleGain = 1.8;
	ImageInfo.LargeCirqueSpecial = -3;


	SystemSettings.Wifi_ON = 'F';
	SystemSettings.ResetTest = 'F';
	SystemSettings.Wifi_SendWave = 'F';
	SystemSettings.Run_DistanceLimit = 'F';
	SystemSettings.StartCheck = 'T';
	SystemSettings.CrossCheck = 'T';
	SystemSettings.RampCheck = 'T';
	SystemSettings.SpeedLimit = 'T';
	SystemSettings.CrossMask = 'T';


	for (uint8 i = 0; i < 6; i++)
		CirqueDirBox[i] = 1;


	for (uint8 k = 0; k < 8; k++)
	{
		PicErr[k] = 0;
		ExpectCurvature[k] = 0;
	}
	PicErr[0] = 0.4;
	PicErr[1] = 0.6;
	PicErr[2] = 0.8;
	PicErr[3] = 1.0;
	PicErr[4] = 1.2;
	PicErr[5] = 1.5;
	PicErr[6] = 1.6;
	PicErr[7] = 1.7;
	ExpectCurvature[0] = 0.31;
	ExpectCurvature[1] = 0.53;
	ExpectCurvature[2] = 0.74;
	ExpectCurvature[3] = 0.95;
	ExpectCurvature[4] = 1.16;
	ExpectCurvature[5] = 1.8;
	ExpectCurvature[6] = 2.1;
	ExpectCurvature[7] = 2.5;

	for (uint8 j = 0; j < 10; j++)
	{
		SpecialRoute[j].Mask = 'F';
		SpecialRoute[j].Road = (RoadType_e)0;
		SpecialRoute[j].RoadNum = 0;
		SpecialRoute[j].StartDistance = 0;
		SpecialRoute[j].EndDistance = 0;
		SpecialRoute[j].Angle = 0;
		SpecialRoute[j].IsRamp = 'F';
	}
        CarInfo.WheelType = 0;
}


void Data_Convert_Uint32(void)
{
	FLASH_DATA[0] = FloattoUint32(RunningData.ExpectAngleMax);
	FLASH_DATA[1] = FloattoUint32(RunningData.ExpectAngleMin);
	FLASH_DATA[2] = FloattoUint32(RunningData.ExpectBalanceAngle);
	FLASH_DATA[3] = FloattoUint32(RunningData.ExpectRampAngle);
	FLASH_DATA[4] = FloattoUint32(Gyro.Xoffset);
	FLASH_DATA[5] = FloattoUint32(Gyro.Yoffset);
	FLASH_DATA[6] = FloattoUint32(Gyro.Zoffset);
	FLASH_DATA[7] = FloattoUint32(SpeedControl.P);
	FLASH_DATA[8] = FloattoUint32(RunningData.ExpectSpeed);
	FLASH_DATA[9] = FloattoUint32(RunningData.StartAngle);
	FLASH_DATA[10] = FloattoUint32(RunningData.StartDistance);
	FLASH_DATA[11] = FloattoUint32(pitchPIDcontrol.P);
	FLASH_DATA[12] = FloattoUint32(pitchPIDcontrol.D);


        
        FLASH_DATA[13] = CarInfo.WheelType;
	FLASH_DATA[20] = FloattoUint32(yawPIDcontrol[0].P);
	FLASH_DATA[21] = FloattoUint32(yawPIDcontrol[0].D);
	FLASH_DATA[22] = FloattoUint32(yawPIDcontrol[1].P);
	FLASH_DATA[23] = FloattoUint32(yawPIDcontrol[1].D);
	FLASH_DATA[24] = FloattoUint32(yawPIDcontrol[0].I);
	FLASH_DATA[25] = FloattoUint32(yawPIDcontrol[1].I);
	FLASH_DATA[26] = FloattoUint32(SpeedRadius.P);
	FLASH_DATA[27] = FloattoUint32(SpeedRadius.D);


	FLASH_DATA[28] = FloattoUint32(CarInfo.StopDelay);
	FLASH_DATA[29] = FloattoUint32(CarInfo.ForceStopR);
	FLASH_DATA[30] = FloattoUint32(CarInfo.ForceStopL);
	FLASH_DATA[31] = FloattoUint32(CarInfo.MinR);
	FLASH_DATA[32] = FloattoUint32(CarInfo.MaxR);
	FLASH_DATA[33] = FloattoUint32(CarInfo.Run_Distance);
	FLASH_DATA[34] = FloattoUint32(CarInfo.SpeedLimitGrowTh);
	FLASH_DATA[35] = FloattoUint32(CarInfo.SpeedLimitReduceTh);
	FLASH_DATA[36] = FloattoUint32(CarInfo.MaxAcc);
	FLASH_DATA[37] = FloattoUint32(CarInfo.LimitRmax);


	FLASH_DATA[38] = FloattoUint32(ImageInfo.DirCenter);
	FLASH_DATA[39] = FloattoUint32(ImageInfo.CirqueLimitRoute);
	FLASH_DATA[40] = FloattoUint32(ImageInfo.RObstacleCenter);
	FLASH_DATA[41] = FloattoUint32(ImageInfo.LObstacleCenter);
	FLASH_DATA[42] = FloattoUint32(ImageInfo.CameraAxasLength);
	FLASH_DATA[43] = FloattoUint32(ImageInfo.CameraFocusLength);
	FLASH_DATA[44] = FloattoUint32(ImageInfo.CameraInterAngle);
	FLASH_DATA[45] = FloattoUint32(ImageInfo.CameraZeroAngle);
	FLASH_DATA[46] = (uint32)ImageInfo.threshold;
	FLASH_DATA[47] = FloattoUint32(ImageInfo.TowPointSite);
	FLASH_DATA[49] = FloattoUint32(ImageInfo.LargeCirqueSpecial);
	FLASH_DATA[50] = FloattoUint32(ImageInfo.ObstacleDelay);
	FLASH_DATA[51] = FloattoUint32(ImageInfo.TowPointAdjustMax);
	FLASH_DATA[52] = FloattoUint32(ImageInfo.StartCenter);
	FLASH_DATA[53] = FloattoUint32(ImageInfo.CirqueInSpecial);
	FLASH_DATA[54] = FloattoUint32(ImageInfo.CirqueOutSpecial);
	FLASH_DATA[55] = FloattoUint32(ImageInfo.TowPointAdjustMin);
	FLASH_DATA[56] = FloattoUint32(ImageInfo.TowPointAdjustTh);
	FLASH_DATA[57] = FloattoUint32(ImageInfo.TowPointAdjust);
	FLASH_DATA[58] = FloattoUint32(ImageInfo.RampLKMax);
	FLASH_DATA[59] = FloattoUint32(ImageInfo.RampRKMIN);



	FLASH_DATA[60] = SystemSettings.Wifi_ON;
	FLASH_DATA[61] = SystemSettings.ResetTest;
	FLASH_DATA[62] = SystemSettings.Wifi_SendWave;
	FLASH_DATA[63] = SystemSettings.Run_DistanceLimit;
	FLASH_DATA[64] = SystemSettings.StartCheck;
	FLASH_DATA[65] = SystemSettings.CrossCheck;
	FLASH_DATA[66] = SystemSettings.RampCheck;
	FLASH_DATA[67] = SystemSettings.SpeedLimit;
	FLASH_DATA[68] = SystemSettings.CrossMask;


	FLASH_DATA[69] = FloattoUint32(ImageInfo.ObstacleGain);

	for (uint8 i = 0; i < 6; i++)
		FLASH_DATA[70 + i] = CirqueDirBox[i];



	for (uint8 k = 0; k < 8; k++)
	{
		FLASH_DATA[80 + k] = FloattoUint32(PicErr[k]);
		FLASH_DATA[88 + k] = FloattoUint32(ExpectCurvature[k]);
	}


	for (uint8 j = 0; j < 10; j++)
	{
		FLASH_DATA[100 + j * 7] = SpecialRoute[j].Mask;
		FLASH_DATA[100 + j * 7 + 1] = SpecialRoute[j].Road;
		FLASH_DATA[100 + j * 7 + 2] = SpecialRoute[j].RoadNum;
		FLASH_DATA[100 + j * 7 + 3] = FloattoUint32(SpecialRoute[j].StartDistance);
		FLASH_DATA[100 + j * 7 + 4] = FloattoUint32(SpecialRoute[j].EndDistance);
		FLASH_DATA[100 + j * 7 + 5] = FloattoUint32(SpecialRoute[j].Angle);
		FLASH_DATA[100 + j * 7 + 6] = SpecialRoute[j].IsRamp;
	}

	for (uint8 i = 0; i < 6; i++)
		FLASH_DATA[200 + i] = CrossBox[i];

}
