#include "GeneralDeclare.h"
#include "TimerEventDriver.h"
#include "KeyBoardDriver.h"
#include "System_UI.h"
#include "DisPlayDeviceDriver.h"
#include "FLASH.h"
#include "SystemControl.h"
#include "Terminator.h"
#include "ImageSensorDriver.h"
#include "CommunicateDriver.h"
#include "Wifi.h"
#include "RoadLoad.h"
#include "ProjectMath.h"

#define  Main_menuL  12
#define  Main_menuH  9

Key_msg_t         KeyStatus;
int               Cursor = 0;
int               NowPage = 0;
int               Menustatus = 0;
uint8             uintForNow = 0;
float             DataInterval = 0;
float             DataToChange[40] = { 0 };
double            uintArray[8] = { 100, 10, 1, 0, 0.1, 0.01, 0.001, 0.0001 };
int               DataToChangeNum = 0;
extern volatile uint8 IsStart;

uint8 Main_menu[Main_menuH][Main_menuL] = {
  "Pitch_Set  ",
  "Yaw_Set    ",
  "Image_Set  ",
  "Camera_View",
  "SystemCheck",
  "SpeialDeal",
  "Data_save  ",
  "Motor_View ",
  "Gyro_Offset",
};



void AutoMode(void);
void Key_Event(void)   //按键事件  
{
	if (IsStart == 'T')
	{
		AutoMode();
	}
	if (Get_KeyFifo(&KeyStatus))    //有未处理的按键消息
	{
		switch (KeyStatus.key)        //选按键
		{
		case Key_U:       if (!Menustatus)            //Menustatus为0时为选择菜单 1数据菜单
		{
			if (KeyStatus.status == 0)
			{
				Cursor--;
				if (Cursor < 0)
					Cursor = 0;
			}
		}
						  else
						  {
							  if (KeyStatus.status == 0)
							  {
								  DataInterval++;
							  }
						  }
						  break;
		case Key_D:       if (!Menustatus)
		{
			if (KeyStatus.status == 0)
			{
				Cursor++;
			}
		}
						  else
						  {
							  if (KeyStatus.status == 0)
							  {
								  DataInterval--;
							  }
						  }
						  break;
		case Key_Y:       if (KeyStatus.status == 0)
		{
			if (!Menustatus)
				NowPage++;
			else
			{
				uintForNow++;
				if (uintForNow == 8) uintForNow = 0;
			}
		}
						  break;
		case Key_N:       if (KeyStatus.status == 0)
		{
			NowPage--;
			if (Menustatus == 1)
				Menustatus = 0;
		}
						  break;
		default:         break;
		}
	}
	return;
}



////////数据修改
void ChangeData(uint8 *Object, uint8 Num, uint8 LastPage)
{
	OLED_CLS();
	Menustatus = 1;
	uintForNow = 2;
	OLED_PrintString(0, 0, Object, 0);
	while (1)
	{
		Key_Event();
		if (NowPage == LastPage)
		{
			Menustatus = 0;
			OLED_CLS();
			break;
		}

		if (uintArray[uintForNow] != 0)
			DataToChange[Num] += DataInterval * uintArray[uintForNow];
		else if (uintArray[uintForNow] == 0 && DataInterval != 0)
			DataToChange[Num] = 0;

		DataInterval = 0;


		OLED_PrintVar(0, 1, DataToChange[Num], 11, 4);

		if (uintArray[uintForNow] == 0)
			OLED_PrintStr(0, 2, "Clear To  0", 14, 1);
		else
			OLED_PrintVar(0, 2, uintArray[uintForNow], 14, 5);
	}
}



////////俯仰角控制系数修改
#define BalanceDataH  18
#define BalanceDataL  14
uint8 BalanceData[BalanceDataH][BalanceDataL] =
{
  "Pitch_P  ",
  "Pitch_D  ",
  "ExSpeed  ",
  "Speed.P  ",
  "NormalAngle",
  "RampAngle  ",
  "AngleMin ",
  "AngleMax ",
  "StartAngle",
  "StartDistance",
  "RunDistance",
  "SpeedGrowTh",
  "SpeedReduceTh",
  "MaxAccelerate",
  "StopRPWM",
  "StopLPWM",
  "StopDelay",
  "WheelType"
};
void BalanceData_Set(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	DataToChangeNum = BalanceDataH;
	DataToChange[0] = pitchPIDcontrol.P;
	DataToChange[1] = pitchPIDcontrol.D;
	DataToChange[2] = RunningData.ExpectSpeed;
	DataToChange[3] = SpeedControl.P;
	DataToChange[4] = RunningData.ExpectBalanceAngle;
	DataToChange[5] = RunningData.ExpectRampAngle;
	DataToChange[6] = RunningData.ExpectAngleMin;
	DataToChange[7] = RunningData.ExpectAngleMax;
	DataToChange[8] = RunningData.StartAngle;
	DataToChange[9] = RunningData.StartDistance;
	DataToChange[10] = CarInfo.Run_Distance;
	DataToChange[11] = CarInfo.SpeedLimitGrowTh;
	DataToChange[12] = CarInfo.SpeedLimitReduceTh;
	DataToChange[13] = CarInfo.MaxAcc;
	DataToChange[14] = CarInfo.ForceStopR;
	DataToChange[15] = CarInfo.ForceStopL;
	DataToChange[16] = CarInfo.StopDelay;
        DataToChange[17] = CarInfo.WheelType;
	while (1)
	{
		Key_Event();
		if (Cursor == BalanceDataH) Cursor = BalanceDataH - 1;
		if (NowPage == 4)
		{
			ChangeData(BalanceData[Cursor], Cursor, 3);
		}
		else if (NowPage == 2)
		{
			pitchPIDcontrol.P = DataToChange[0];
			pitchPIDcontrol.D = DataToChange[1];
			RunningData.ExpectSpeed = DataToChange[2];
			SpeedControl.P = DataToChange[3];
			RunningData.ExpectBalanceAngle = DataToChange[4];
			RunningData.ExpectRampAngle = DataToChange[5];
			RunningData.ExpectAngleMin = DataToChange[6];
			RunningData.ExpectAngleMax = DataToChange[7];
			RunningData.StartAngle = DataToChange[8];
			RunningData.StartDistance = DataToChange[9];
			CarInfo.Run_Distance = DataToChange[10];
			CarInfo.SpeedLimitGrowTh = DataToChange[11];
			CarInfo.SpeedLimitReduceTh = DataToChange[12];
			CarInfo.MaxAcc = DataToChange[13];
			CarInfo.ForceStopR = DataToChange[14];
			CarInfo.ForceStopL = DataToChange[15];
			CarInfo.StopDelay = DataToChange[16];
                        CarInfo.WheelType = (uint8)DataToChange[17];
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, BalanceData[y], BalanceDataL, 0);
				else
					OLED_PrintStr(0, y, BalanceData[y], BalanceDataL, 1);

				OLED_PrintVar(90, y, DataToChange[y], 6, 1);
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, BalanceData[Cursor - 7 + y], BalanceDataL, 0);
				else
					OLED_PrintStr(0, y, BalanceData[Cursor - 7 + y], BalanceDataL, 1);

				OLED_PrintVar(90, y, DataToChange[Cursor - 7 + y], 6, 1);
			}
		}
	}
}



////////摄像头图像预览
static uint8 OLED_IMG[60][80] = { 0 };
uint8 ImageSensorTestChoose = 0;
volatile uint8 ImageSensorOK = 0;
#define ImageSensorH  4
#define ImageSensorL  7
uint8 ImageSensorMenu[ImageSensorH][ImageSensorL] =
{
  "Camera",
  "Border",
  "Real  ",
  "Test  ",
};
void ImageSensorView(void)
{
	uint8 y = 0;
	uint8 x = 0;
	OLED_CLS();
	Cursor = 0;
	int YLine = 59;
	ImageSensorTestChoose = 1;
	ImageSensorOK = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == ImageSensorH) Cursor = ImageSensorH - 1;
		if (NowPage == 4)
		{
			Menustatus = 1;
		}

		if (NowPage == 4)
		{
			YLine += DataInterval;
			if (YLine >= 59)YLine = 59;
			else if (YLine <= ImageStatus.OFFLine) YLine = ImageStatus.OFFLine;
			DataInterval = 0;
		}

		ImageSensorTestChoose = Cursor + 1;

		if (NowPage == 2)
		{
			ImageSensorTestChoose = 0;
			ImageSensorOK = 0;
			OLED_CLS();
			break;
		}
		ImgVSY_port->ISFR = 1 << ImgVSY_pin;
		NVIC_EnableIRQ(PORTA_IRQn);     ////循环中开启场中断
		while (ImageSensorOK == 0);
		for (y = 0; y < 2; y++)
		{
			if (Cursor < 2)
			{
				if (Cursor == y)
					OLED_PrintStr(80, y, ImageSensorMenu[y], ImageSensorL, 0);
				else
					OLED_PrintStr(80, y, ImageSensorMenu[y], ImageSensorL, 1);
			}
			else
			{
				if ((Cursor - 2) == y)
					OLED_PrintStr(80, y, ImageSensorMenu[y + 2], ImageSensorL, 0);
				else
					OLED_PrintStr(80, y, ImageSensorMenu[y + 2], ImageSensorL, 1);
			}
		}

		switch (ImageSensorTestChoose)
		{
		case 1:
			for (x = 0; x < 60; x++)
				for (y = 0; y < 80; y++)
				{
					OLED_IMG[x][y] = 0xff;

					if (PicForNow[x][y] == 0x00)
						OLED_IMG[x][y] = 0xff;
					else
						OLED_IMG[x][y] = 0x00;

					if (x >= ImageStatus.OFFLine)
					{
						if (y == ImageDeal[x].Center)
							OLED_IMG[x][y] = 0xff;

						if (ImageDeal[x].RightTemp == y)
							OLED_IMG[x][y] = 0xff;
						if (ImageDeal[x].LeftTemp == y)
							OLED_IMG[x][y] = 0xff;
					}
					OLED_IMG[x][39] = 0xff;
				}
			OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
			break;
		case 2:
			for (x = 0; x < 60; x++)
			{
				for (y = 0; y < 80; y++)
				{
					OLED_IMG[x][y] = 0xff;
					if (ImageStatus.OFFLine <= x)
					{
						if (y == ImageDeal[x].Center)
							OLED_IMG[x][y] = 0x00;
					}
					OLED_IMG[x][39] = 0x00;
				}
			}
			OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
			break;
		case 3:
			for (x = 0; x < 60; x++)
			{
				for (y = 0; y < 80; y++)
				{
					OLED_IMG[x][y] = 0xff;
					if (ImageStatus.OFFLine <= x)
					{
						if (y == ImageDeal[x].Center)
							OLED_IMG[x][y] = 0x00;
                                                if (y == ImageDeal[x].LeftBorder)
							OLED_IMG[x][y] = 0x00;
                                                if (y == ImageDeal[x].RightBorder)
							OLED_IMG[x][y] = 0x00;
					}
					OLED_IMG[x][39] = 0x00;
				}
			}
			OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
			break;
		case 4:
			for (x = 0; x < 60; x++)
				for (y = 0; y < 80; y++)
				{
					if (PicForNow[x][y] == 0x00)
						OLED_IMG[x][y] = 0xff;
					else
						OLED_IMG[x][y] = 0x00;
					if (x >= ImageStatus.OFFLine)
					{
						if (y == ImageDeal[x].Center)
							OLED_IMG[x][y] = 0xff;
					}
					OLED_IMG[x][39] = 0xff;
					if (x == YLine)
						OLED_IMG[x][y] = 0xff;
				}
			OLED_PrintImg((uint8 *)OLED_IMG, 60, 80);
			break;
		default:break;
		}


		if (RoadType == Obstacle || ImageStatus.ObstacleDelay == 'T')
			OLED_PrintStr(80, 2, "Obstacl", 7, 0);
		else if (ImageStatus.IsCinqueOutIn == 'T')
			OLED_PrintStr(80, 2, "CirOut", 7, 0);
		else if (ImageStatus.CirqueOut == 'T')
			OLED_PrintStr(80, 2, "CirIn", 7, 0);
		else if (RoadType == Cirque)
			OLED_PrintStr(80, 2, "Cirque", 7, 0);
		else if (RoadType == Straight)
			OLED_PrintStr(80, 2, "Straig", 7, 0);
		else if (RoadType == Cross)
			OLED_PrintStr(80, 2, "Cross", 7, 0);
		else if (RoadType == Ramp)
			OLED_PrintStr(80, 2, "Ramp", 7, 0);
		else if (RoadType == Normal)
			OLED_PrintStr(80, 2, "Normal", 7, 0);

		if (ImageSensorTestChoose == 4)
		{
			OLED_PrintStr(80, 3, "L:", 2, 1);
			OLED_PrintVar(92, 3, YLine, 6, 1);
		}
		else
		{
			OLED_PrintStr(80, 3, "E:", 2, 1);
			OLED_PrintVar(92, 3, ImageStatus.ExpectCur, 6, 1);
		}
		OLED_PrintStr(80, 4, "R:", 2, 1);
                OLED_PrintStr(80, 6, "R:", 2, 1);
                
		OLED_PrintVar(92, 4, ImageStatus.RBorderK, 4, 1);
                OLED_PrintVar(92, 5, ImageStatus.LBorderK, 4, 1);
                
		if (ImageDeal[YLine].IsRightFind == 'T')
			OLED_PrintStr(120, 6, "T", 1, 0);
		else if (ImageDeal[YLine].IsRightFind == 'P')
			OLED_PrintStr(120, 6, "P", 1, 0);
		else
			OLED_PrintStr(120, 6, "W", 1, 0);
                
                
		OLED_PrintStr(80, 5, "L:", 2, 1);
                OLED_PrintStr(80, 7, "L:", 2, 1);
		
		if (ImageDeal[YLine].IsLeftFind == 'T')
			OLED_PrintStr(120, 7, "T", 1, 0);
		else if (ImageDeal[YLine].IsLeftFind == 'P')
			OLED_PrintStr(120, 7, "P", 1, 0);
		else
			OLED_PrintStr(120, 7, "W", 1, 0);
                

		OLED_PrintVar(92, 6, ImageDeal[YLine].RightBorder, 4, 1);
		OLED_PrintVar(92, 7, ImageDeal[YLine].LeftBorder, 4, 1);
		ImageSensorOK = 0;
	}
}



////////偏航系数修改
#define YawDataH   24
#define YawDataL   14
uint8 YawData[YawDataH][YawDataL] =
{
  "StartYaw_P",
  "StartYaw_P",
  "RunYaw_P",
  "RunYaw_D",
  "MinR    ",
  "MaxR    ",
  "Radius.P",
  "PicDet.D",
  "DevErr[0]",
  "DevErr[1]",
  "DevErr[2]",
  "DevErr[3]",
  "DevErr[4]",
  "DevErr[5]",
  "DevErr[6]",
  "DevErr[7]",
  "ExpCur[0]",
  "ExpCur[1]",
  "ExpCur[2]",
  "ExpCur[3]",
  "ExpCur[4]",
  "ExpCur[5]",
  "ExpCur[6]",
  "ExpCur[7]",
};
void YawData_Set(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	DataToChangeNum = YawDataH;
	DataToChange[0] = yawPIDcontrol[0].P;
	DataToChange[1] = yawPIDcontrol[0].D;
	DataToChange[2] = yawPIDcontrol[1].P;
	DataToChange[3] = yawPIDcontrol[1].D;
	DataToChange[4] = CarInfo.MinR;
	DataToChange[5] = CarInfo.MaxR;
	DataToChange[6] = SpeedRadius.P;
	DataToChange[7] = SpeedRadius.D;
	DataToChange[8] = PicErr[0];
	DataToChange[9] = PicErr[1];
	DataToChange[10] = PicErr[2];
	DataToChange[11] = PicErr[3];
	DataToChange[12] = PicErr[4];
	DataToChange[13] = PicErr[5];
	DataToChange[14] = PicErr[6];
	DataToChange[15] = PicErr[7];
	DataToChange[16] = ExpectCurvature[0];
	DataToChange[17] = ExpectCurvature[1];
	DataToChange[18] = ExpectCurvature[2];
	DataToChange[19] = ExpectCurvature[3];
	DataToChange[20] = ExpectCurvature[4];
	DataToChange[21] = ExpectCurvature[5];
	DataToChange[22] = ExpectCurvature[6];
	DataToChange[23] = ExpectCurvature[7];
	while (1)
	{
		Key_Event();
		if (Cursor == YawDataH) Cursor = YawDataH - 1;
		if (NowPage == 4)
		{
			ChangeData(YawData[Cursor], Cursor, 3);
		}
		else if (NowPage == 2)
		{
			yawPIDcontrol[0].P = DataToChange[0];
			yawPIDcontrol[0].D = DataToChange[1];
			yawPIDcontrol[1].P = DataToChange[2];
			yawPIDcontrol[1].D = DataToChange[3];
			CarInfo.MinR = DataToChange[4];
			CarInfo.MaxR = DataToChange[5];
			SpeedRadius.P = DataToChange[6];
			SpeedRadius.D = DataToChange[7];
			PicErr[0] = DataToChange[8];
			PicErr[1] = DataToChange[9];
			PicErr[2] = DataToChange[10];
			PicErr[3] = DataToChange[11];
			PicErr[4] = DataToChange[12];
			PicErr[5] = DataToChange[13];
			PicErr[6] = DataToChange[14];
			PicErr[7] = DataToChange[15];
			ExpectCurvature[0] = DataToChange[16];
			ExpectCurvature[1] = DataToChange[17];
			ExpectCurvature[2] = DataToChange[18];
			ExpectCurvature[3] = DataToChange[19];
			ExpectCurvature[4] = DataToChange[20];
			ExpectCurvature[5] = DataToChange[21];
			ExpectCurvature[6] = DataToChange[22];
			ExpectCurvature[7] = DataToChange[23];
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, YawData[y], YawDataL, 0);
				else
					OLED_PrintStr(0, y, YawData[y], YawDataL, 1);

				OLED_PrintVar(90, y, DataToChange[y], 6, 1);
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, YawData[Cursor - 7 + y], YawDataL, 0);
				else
					OLED_PrintStr(0, y, YawData[Cursor - 7 + y], YawDataL, 1);

				OLED_PrintVar(90, y, DataToChange[Cursor - 7 + y], 6, 1);
			}
		}
	}
}



#define ImageDataH  19
#define ImageDataL  13
uint8 ImageData[ImageDataH][ImageDataL] =
{
  "ObsDelay",
  "RObsCenter",
  "LObsCenter",
  "ObstacleGain",
  "StartCenter",
  "DirCenter",
  "TowThSpeed",
  "TowAdjust",
  "TowMAX",
  "TowMin",
  "CirInSpe",
  "CirOutSpe",
  "LargeCiruqe",
  "RampRightK",
  "RampLeftK",
  "TowPoint",
  "InterAngle",  ///轴线夹角
  "ZeroAngle",  ///零度角
  "Threshold",
};
void ImageData_Set(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	DataToChangeNum = ImageDataH;
	DataToChange[0] = ImageInfo.ObstacleDelay;
	DataToChange[1] = ImageInfo.RObstacleCenter;
	DataToChange[2] = ImageInfo.LObstacleCenter;
	DataToChange[3] = ImageInfo.ObstacleGain;
	DataToChange[4] = ImageInfo.StartCenter;
	DataToChange[5] = ImageInfo.DirCenter;
	DataToChange[6] = ImageInfo.TowPointAdjustTh;
	DataToChange[7] = ImageInfo.TowPointAdjust;
	DataToChange[8] = ImageInfo.TowPointAdjustMax;
	DataToChange[9] = ImageInfo.TowPointAdjustMin;
	DataToChange[10] = ImageInfo.CirqueInSpecial;
	DataToChange[11] = ImageInfo.CirqueOutSpecial;
	DataToChange[12] = ImageInfo.LargeCirqueSpecial;
	DataToChange[13] = ImageInfo.RampRKMIN;
	DataToChange[14] = ImageInfo.RampLKMax;
	DataToChange[15] = ImageInfo.TowPointSite;
	DataToChange[16] = ImageInfo.CameraInterAngle;
	DataToChange[17] = ImageInfo.CameraZeroAngle;
	DataToChange[18] = ImageInfo.threshold;
	while (1)
	{
		Key_Event();
		if (Cursor == ImageDataH) Cursor = ImageDataH - 1;
		if (NowPage == 4)
		{
			ChangeData(ImageData[Cursor], Cursor, 3);
		}
		else if (NowPage == 2)
		{
			ImageInfo.ObstacleDelay = DataToChange[0];
			ImageInfo.RObstacleCenter = DataToChange[1];
			ImageInfo.LObstacleCenter = DataToChange[2];
			ImageInfo.ObstacleGain = DataToChange[3];
			ImageInfo.StartCenter = DataToChange[4];
			ImageInfo.DirCenter = DataToChange[5];
			ImageInfo.TowPointAdjustTh = DataToChange[6];
			ImageInfo.TowPointAdjust = DataToChange[7];
			ImageInfo.TowPointAdjustMax = DataToChange[8];
			ImageInfo.TowPointAdjustMin = DataToChange[9];
			ImageInfo.CirqueInSpecial = DataToChange[10];
			ImageInfo.CirqueOutSpecial = DataToChange[11];
			ImageInfo.LargeCirqueSpecial = DataToChange[12];
			ImageInfo.RampRKMIN = DataToChange[13];
			ImageInfo.RampLKMax = DataToChange[14];
			ImageInfo.TowPointSite = DataToChange[15];
			ImageInfo.CameraInterAngle = DataToChange[16];
			ImageInfo.CameraZeroAngle = DataToChange[17];
			ImageInfo.threshold = (uint8)DataToChange[18];
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, ImageData[y], ImageDataL, 0);
				else
					OLED_PrintStr(0, y, ImageData[y], ImageDataL, 1);

				OLED_PrintVar(80, y, DataToChange[y], 7, 2);
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, ImageData[Cursor - 7 + y], ImageDataL, 0);
				else
					OLED_PrintStr(0, y, ImageData[Cursor - 7 + y], ImageDataL, 1);

				OLED_PrintVar(80, y, DataToChange[Cursor - 7 + y], 7, 2);
			}
		}
	}
}



#define SystemCheckH 8
#define SystemCheckL 14
uint8 SystemCheck[SystemCheckH][SystemCheckL] = {
  "Wifi_ON",
  "ResetTest",
  "CrossMask",
  "DistanceLimit",
  "StartCheck",
  "CrossCheck",
  "RampCheck",
  "SpeedLimit",
};
void Set_SystemCheck(void)  //系统初始参数设定
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == SystemCheckH) Cursor = 0;
		if (NowPage == 5) NowPage = 4;
		if (NowPage == 4)    //4号界面
		{
			Menustatus = 1;    //转为数据修改模式

			switch (Cursor)    //扫描光标位置
			{
			case 0: if (DataInterval > 0) SystemSettings.Wifi_ON = 'T';
					else if (DataInterval < 0)SystemSettings.Wifi_ON = 'F';
					break;
			case 1: if (DataInterval > 0) SystemSettings.ResetTest = 'T';
					else if (DataInterval < 0)SystemSettings.ResetTest = 'F';
					break;
			case 2:if (DataInterval > 0) SystemSettings.CrossMask = 'T';
				   else if (DataInterval < 0)SystemSettings.CrossMask = 'F';
				   break;
			case 3: if (DataInterval > 0) SystemSettings.Run_DistanceLimit = 'T';
					else if (DataInterval < 0)SystemSettings.Run_DistanceLimit = 'F';
					break;
			case 4: if (DataInterval > 0) SystemSettings.StartCheck = 'T';
					else if (DataInterval < 0)SystemSettings.StartCheck = 'F';
					break;
			case 5: if (DataInterval > 0) SystemSettings.CrossCheck = 'T';
					else if (DataInterval < 0)SystemSettings.CrossCheck = 'F';
					break;
			case 6: if (DataInterval > 0) SystemSettings.RampCheck = 'T';
					else if (DataInterval < 0)SystemSettings.RampCheck = 'F';
					break;
			case 7:if (DataInterval > 0) SystemSettings.SpeedLimit = 'T';
				   else if (DataInterval < 0)SystemSettings.SpeedLimit = 'F';
				   break;
			default:break;
			}
			DataInterval = 0;   //修改值清零
		}
		else if (NowPage == 2)
		{
			OLED_CLS();
			break;
		}
		if (SystemSettings.Wifi_ON == 'T') SystemSettings.Wifi_SendWave = 'T';
		else if (SystemSettings.Wifi_ON == 'F')SystemSettings.Wifi_SendWave = 'F';

		for (y = 0; y < SystemCheckH; y++)
		{
			if (y == Cursor)
				OLED_PrintString(0, y, SystemCheck[y], 0);
			else
				OLED_PrintString(0, y, SystemCheck[y], 1);
		}
		if (SystemSettings.Wifi_ON == 'T')
			OLED_PrintString(90, 0, "ON ", 1);
		else OLED_PrintString(90, 0, "OFF", 1);

		if (SystemSettings.ResetTest == 'T')
			OLED_PrintString(90, 1, "ON ", 1);
		else OLED_PrintString(90, 1, "OFF", 1);

		if (SystemSettings.CrossMask == 'T')
			OLED_PrintString(90, 2, "ON ", 1);
		else OLED_PrintString(90, 2, "OFF", 1);

		if (SystemSettings.Run_DistanceLimit == 'T')
			OLED_PrintString(90, 3, "ON ", 1);
		else OLED_PrintString(90, 3, "OFF", 1);

		if (SystemSettings.StartCheck == 'T')
			OLED_PrintString(90, 4, "ON ", 1);
		else OLED_PrintString(90, 4, "OFF", 1);

		if (SystemSettings.CrossCheck == 'T')
			OLED_PrintString(90, 5, "ON ", 1);
		else OLED_PrintString(90, 5, "OFF", 1);

		if (SystemSettings.RampCheck == 'T')
			OLED_PrintString(90, 6, "ON ", 1);
		else OLED_PrintString(90, 6, "OFF", 1);

		if (SystemSettings.SpeedLimit == 'T')
			OLED_PrintString(90, 7, "ON ", 1);
		else OLED_PrintString(90, 7, "OFF", 1);
	}
}



#define CirqueDirSetH 6
#define CirqueDirSetL 10
uint8 CirqueDirCheck[CirqueDirSetH][CirqueDirSetL] = {
  "Cirque_1",
  "Cirque_2",
  "Cirque_3",
  "Cirque_4",
  "Cirque_5",
  "Cirque_6",
};
void CirqueDirSet(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == CirqueDirSetH)Cursor = 0;
		if (NowPage == 3)
		{
			OLED_CLS();
			break;
		}
		else if (NowPage == 5)
		{
			Menustatus = 1;    //转为数据修改模式     
			if (DataInterval > 0) CirqueDirBox[Cursor] = 0;
			else if (DataInterval < 0)CirqueDirBox[Cursor] = 1;
			DataInterval = 0;   //修改值清零
		}
		else if (NowPage == 6)
		{
			NowPage = 5;
		}
		for (y = 0; y < CirqueDirSetH; y++)
		{
			if (y == Cursor)
				OLED_PrintStr(0, y, CirqueDirCheck[y], CirqueDirSetL, 0);
			else
				OLED_PrintStr(0, y, CirqueDirCheck[y], CirqueDirSetL, 1);

			if (y == Cursor && Menustatus == 1)
			{
				if (CirqueDirBox[y] == 0)
					OLED_PrintStr(80, y, "Right", 6, 0);
				else if (CirqueDirBox[y] == 1)
					OLED_PrintStr(80, y, "Left", 6, 0);
			}
			else
			{
				if (CirqueDirBox[y] == 0)
					OLED_PrintStr(80, y, "Right", 6, 1);
				else if (CirqueDirBox[y] == 1)
					OLED_PrintStr(80, y, "Left", 6, 1);
			}
		}
	}
}




#define CrossDirSetH 6
#define CrossDirSetL 10
uint8 CrossDirCheck[CrossDirSetH][CrossDirSetL] = {
  "Cross_1",
  "Cross_2",
  "Cross_3",
  "Cross_4",
  "Cross_5",
  "Cross_6",
};
void CrossDirSet(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == CrossDirSetH)Cursor = 0;
		if (NowPage == 3)
		{
			OLED_CLS();
			break;
		}
		else if (NowPage == 5)
		{
			Menustatus = 1;    //转为数据修改模式     
			if (DataInterval > 0) CrossBox[Cursor] = 0;
			else if (DataInterval < 0)CrossBox[Cursor] = 1;
			DataInterval = 0;   //修改值清零
		}
		else if (NowPage == 6)
		{
			NowPage = 5;
		}
		for (y = 0; y < CrossDirSetH; y++)
		{
			if (y == Cursor)
				OLED_PrintStr(0, y, CrossDirCheck[y], CrossDirSetL, 0);
			else
				OLED_PrintStr(0, y, CrossDirCheck[y], CrossDirSetL, 1);

			if (y == Cursor && Menustatus == 1)
			{
				if (CrossBox[y] == 0)
					OLED_PrintStr(80, y, "No", 6, 0);
				else if (CrossBox[y] == 1)
					OLED_PrintStr(80, y, "Yes", 6, 0);
			}
			else
			{
				if (CrossBox[y] == 0)
					OLED_PrintStr(80, y, "No", 6, 1);
				else if (CrossBox[y] == 1)
					OLED_PrintStr(80, y, "Yes", 6, 1);
			}
		}
	}
}



#define SpecialDataH 7
#define SpecialDataL 8
uint8   SpecialDataCheck[SpecialDataH][SpecialDataL] = {
  "Mask",
  "Type",
  "Num",
  "Start",
  "End",
  "Angle",
  "Ramp",
};
void SpecialDataSet(Specialtypedef *Q)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	RoadType_e RoadTemp[4] = { Normal, Cross, Cirque, Obstacle };
	uint8 RoadTypeCount = 0;
	for (uint8 i = 0; i < 4; i++)
	{
		if (RoadTemp[i] == Q->Road)
			RoadTypeCount = i;
	}
	while (1)
	{
		Key_Event();
		if (Cursor == SpecialDataH) Cursor = 0;
		if (NowPage == 3)
		{
			OLED_CLS();
			break;
		}
		else if (NowPage == 5)
		{
			Menustatus = 1;    //转为数据修改模式
			switch (Cursor)
			{
			case 0:  if (DataInterval > 0) Q->Mask = 'T';
					 else if (DataInterval < 0)Q->Mask = 'F';
			case 1: if (DataInterval > 0) RoadTypeCount++;
					else if (DataInterval < 0 && RoadTypeCount > 0)RoadTypeCount--;
					if (RoadTypeCount == 4)
						RoadTypeCount = 3;
					Q->Road = RoadTemp[RoadTypeCount];
					break;
			case 2: if (DataInterval > 0) Q->RoadNum++;
					else if (DataInterval < 0 && Q->RoadNum > 0)Q->RoadNum--;
					break;
			case 3: NowPage++;
				DataToChange[0] = Q->StartDistance;
				ChangeData(SpecialDataCheck[3], 0, 5);
				Q->StartDistance = DataToChange[0];
				NowPage--;
				break;
			case 4: NowPage++;
				DataToChange[0] = Q->EndDistance;
				ChangeData(SpecialDataCheck[4], 0, 5);
				Q->EndDistance = DataToChange[0];
				NowPage--;
				break;
			case 5: NowPage++;
				DataToChange[0] = Q->Angle;
				ChangeData(SpecialDataCheck[5], 0, 5);
				Q->Angle = DataToChange[0];
				NowPage--;
				break;
			case 6:if (DataInterval > 0) Q->IsRamp = 'T';
				   else if (DataInterval < 0)Q->IsRamp = 'F';
			}
			DataInterval = 0;   //修改值清零
		}
		for (y = 0; y < SpecialDataH; y++)
		{
			if (y == Cursor)
				OLED_PrintStr(0, y, SpecialDataCheck[y], SpecialDataL, 0);
			else
				OLED_PrintStr(0, y, SpecialDataCheck[y], SpecialDataL, 1);
		}
		if (Q->Mask == 'T')
			OLED_PrintStr(70, 0, "On.", 6, 1);
		else
			OLED_PrintStr(70, 0, "OFF", 6, 1);

		if (Q->Road == Cross)
			OLED_PrintStr(70, 1, "Cross", 9, 1);
		else if (Q->Road == Cirque)
			OLED_PrintStr(70, 1, "Crique", 9, 1);
		else if (Q->Road == Obstacle)
			OLED_PrintStr(70, 1, "Obstacle", 9, 1);
		else
			OLED_PrintStr(70, 1, "Normal", 9, 1);
		OLED_PrintVar(70, 2, Q->RoadNum, 4, 1);
		OLED_PrintVar(70, 3, Q->StartDistance, 4, 2);
		OLED_PrintVar(70, 4, Q->EndDistance, 4, 2);
		OLED_PrintVar(70, 5, Q->Angle, 4, 2);
		if (Q->IsRamp == 'T')
			OLED_PrintStr(70, 6, "On.", 6, 1);
		else
			OLED_PrintStr(70, 6, "OFF", 6, 1);
	}
}



#define SpecialH 12
#define SpecialL 13
uint8 SpecialCheck[SpecialH][SpecialL] = {
  "CirqueDir",
  "CrossTest",
  "Special_1",
  "Special_2",
  "Special_3",
  "Special_4",
  "Special_5",
  "Special_6",
  "Special_7",
  "Special_8",
  "Special_9",
  "Special_10",
};
void SpecialDealChange(void)
{
	uint8 y = 0;
	OLED_CLS();
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == SpecialH) Cursor = 0;

		if (NowPage == 4)
		{
			if (Cursor == 0)
			{
				CirqueDirSet();
			}
			else if (Cursor == 1)
			{
				CrossDirSet();
			}
			else
			{
				SpecialDataSet(&SpecialRoute[Cursor - 2]);
			}
		}
		else if (NowPage == 2)
		{
			OLED_CLS();
			break;
		}
		for (y = 0; y < 8; y++)
		{
			if (Cursor < 8)
			{
				if (y == Cursor)
					OLED_PrintStr(0, y, SpecialCheck[y], ImageDataL, 0);
				else
					OLED_PrintStr(0, y, SpecialCheck[y], ImageDataL, 1);
				if (y >= 2)
				{
					if (SpecialRoute[y - 2].Mask == 'T')
					{
						OLED_PrintStr(90, y, "ON.", 3, 1);
					}
					else
						OLED_PrintStr(90, y, "OFF", 3, 1);
				}
				else
				{
					OLED_PrintStr(90, y, "   ", 3, 1);
				}
			}
			else
			{
				if (y == 7)
					OLED_PrintStr(0, y, SpecialCheck[Cursor - 7 + y], ImageDataL, 0);
				else
					OLED_PrintStr(0, y, SpecialCheck[Cursor - 7 + y], ImageDataL, 1);
				if (Cursor >= (9 - y))
				{
					if (SpecialRoute[Cursor - 9 + y].Mask == 'T')
					{
						OLED_PrintStr(90, y, "ON.", 3, 1);
					}
					else
						OLED_PrintStr(90, y, "OFF", 3, 1);
				}
				else
					OLED_PrintStr(90, y, "     ", 4, 1);
			}
		}
	}
}



#define  FlashL 15
#define  FlashH 5
uint8 Flashch[FlashH][FlashL] = {
  "CH_1_ToSet",
  "CH_2_ToSet",
  "CH_3_ToSet",
  "CH_4_ToSet",
  "Default",
};
void Data_WR(void)
{
	OLED_CLS();
	uint8 y = 0;
	Cursor = 0;
	while (1)
	{
		Key_Event();
		if (Cursor == FlashH)Cursor = FlashH - 1;
		if (NowPage == 5) NowPage = 5;
		if (NowPage == 2)
		{
			OLED_CLS();
			break;
		}
		else if (NowPage == 4)
		{
			if (Cursor == 4)
			{
				Default_Settings();
				OLED_PrintString(70, 1, "Default", 1);
				CommonDelay(500);
				OLED_PrintString(70, 1, "       ", 1);
				NowPage = 3;
				continue;
			}
			OLED_PrintString(70, 1, "U_ForLOAD", 1);
			OLED_PrintString(70, 2, "D_ForSAVE", 1);
			Menustatus = 1;
			while (1)
			{
				DataInterval = 0;
				Key_Event();
				if (NowPage == 5) NowPage = 4;
				if (DataInterval >= 1)
				{
					My_FlashRead(Cursor);
					OLED_CLS();
					OLED_PrintString(20, 3, "Load Success", 1);
					CommonDelay(500);
					OLED_CLS();
					NowPage--;
					break;
				}
				else if (DataInterval <= -1)
				{
					if (My_FlashWrite(Cursor) == 'T')
					{
						OLED_CLS();
						OLED_PrintString(20, 3, "Save Success", 1);
						CommonDelay(500);
						OLED_CLS();
						NowPage--;
					}
					else
					{
						OLED_CLS();
						OLED_PrintString(20, 3, "Save Failed", 1);
						CommonDelay(500);
						OLED_CLS();
						NowPage--;
					}
					break;
				}
				else if (NowPage == 3)
				{
					OLED_CLS();
					break;
				}
			}
			Menustatus = 0;
		}
		else if (NowPage == 3)
		{
			OLED_PrintString(70, 1, "      ", 1);
			OLED_PrintString(70, 2, "      ", 1);
		}

		for (y = 0; y < FlashH; y++)
		{
			if (y == Cursor)
				OLED_PrintString(0, y, Flashch[y], 0);
			else
				OLED_PrintString(0, y, Flashch[y], 1);
		}
	}
}



extern float AccZAngle;
#define  GyroTestH 5
#define  GyroTestL 11
uint8 GyroTest[GyroTestH][GyroTestL] = {
  "Pitch    ",
  "PitchRate",
  "AccPitch ",
  "Yaw      ",
  "GyroReSet",
};
void Gyro_offsetview(void)  //陀螺仪加速度计数据查看
{
	uint8 y = 0;
	Cursor = 0;
	CommonDelay(400);
	NVIC_DisableIRQ(PIT0_IRQn);   //关闭传感器中断
	CarInfo.Quaternion_init = 'F';/////四元数重新初始化
	CarInfo.yaw = 0;             ////重新指向偏航角
	NVIC_EnableIRQ(PIT0_IRQn);   //打开计时中断
	while (1)
	{
		Key_Event();
		if (Cursor == GyroTestH) Cursor = 0;

		if (NowPage == 2)    //返回上一菜单
		{
			OLED_CLS();
			break;
		}
		if (NowPage == 4)
		{
			if (Cursor == 4)      //只可由光标选择初始化陀螺仪零漂
			{
				CommonDelay(500); ///延时等待静止
				NVIC_DisableIRQ(PIT0_IRQn);   //关闭传感器中断
				NVIC_DisableIRQ(PIT1_IRQn);   //关闭计时中断
				OLED_PrintStr(80, Cursor, "RST", 3, 1);
				Gyro.GyrooffsetOK = 0;
				CommonDelay(100);
				while (Gyro.GyrooffsetOK != 1)
				{
					Get_Gyro_Offset();
				}
				OLED_PrintStr(80, Cursor, "OK!", 4, 1);
				CommonDelay(100);
				OLED_PrintStr(80, Cursor, "   ", 4, 1);
				NVIC_EnableIRQ(PIT0_IRQn);   //打开计时中断
				NVIC_EnableIRQ(PIT1_IRQn);   //打开传感器中断
			}
			NowPage = 3;
		}

		for (y = 0; y < GyroTestH; y++)
		{
			if (y == Cursor)
				OLED_PrintStr(0, y, GyroTest[y], GyroTestL, 0);
			else
				OLED_PrintStr(0, y, GyroTest[y], GyroTestL, 1);
		}
		OLED_PrintVar(70, 0, CarInfo.pitch, 8, 3);
		OLED_PrintVar(70, 1, CarInfo.pitchrate, 8, 3);
		OLED_PrintVar(70, 2, AccZAngle, 8, 3);
		OLED_PrintVar(70, 3, CarInfo.yaw, 8, 3);
		OLED_PrintStr(0, 5, "GX", 2, 1);
		OLED_PrintStr(0, 6, "GY", 2, 1);
		OLED_PrintStr(0, 7, "GZ", 2, 1);
		OLED_PrintStr(64, 5, "AX", 2, 1);
		OLED_PrintStr(64, 6, "AY", 2, 1);
		OLED_PrintStr(64, 7, "AZ", 2, 1);
		OLED_PrintVar(14, 5, Gyro.Xdata, 8, 4);
		OLED_PrintVar(14, 6, Gyro.Ydata, 8, 4);
		OLED_PrintVar(14, 7, Gyro.Zdata, 8, 4);
		OLED_PrintVar(84, 5, Acc.Xdata, 7, 4);
		OLED_PrintVar(84, 6, Acc.Ydata, 7, 4);
		OLED_PrintVar(84, 7, Acc.Zdata, 7, 4);
	}
}



void Motor_view(void)    //////编码器数据查看
{
	CarInfo.length = 0;
	CarInfo.lengthcarry = 0;
	CarInfo.lengthcount = 0;
	OLED_CLS();
	while (1)
	{
		Key_Event();
		if (NowPage == 2)
		{
			break;
		}
		else if (NowPage == 4)
		{
			CarInfo.length = 0;
			CarInfo.lengthcarry = 0;
			CarInfo.lengthcount = 0;
			NowPage = 3;
		}
		OLED_PrintStr(0, 0, "R_Speed/cm", 10, 1);
		OLED_PrintStr(0, 1, "L_Speed/cm", 10, 1);
		OLED_PrintStr(0, 2, "M_TSpeed  ", 10, 1);
		OLED_PrintStr(0, 3, "R_TSpeed  ", 10, 1);
		OLED_PrintStr(0, 4, "L_TSpeed  ", 10, 1);
		OLED_PrintStr(0, 5, "Curvature ", 10, 1);
		OLED_PrintStr(0, 6, "Distance  ", 10, 1);
		OLED_PrintStr(0, 7, "Car_BAT   ", 10, 1);
		OLED_PrintVar(70, 0, CarInfo.speed_R, 8, 3);
		OLED_PrintVar(70, 1, CarInfo.speed_L, 8, 3);
		OLED_PrintVar(70, 2, CarInfo.speed_Mtrue, 8, 3);
		OLED_PrintVar(70, 3, CarInfo.speed_Rtrue, 8, 3);
		OLED_PrintVar(70, 4, CarInfo.speed_Ltrue, 8, 3);
		OLED_PrintVar(70, 5, CarInfo.curvature, 8, 3);
		OLED_PrintVar(70, 6, CarInfo.length, 8, 3);
		OLED_PrintVar(70, 7, CarInfo.Battery, 8, 3);
	}
}

#include "PulseCollectDriver.h"

extern uint32   WifiOrder;
volatile uint8  IsWifiSendOK = 'F';
extern volatile uint8  SpecialOn;
extern volatile uint8  RampOn;
void AutoMode(void)    /////自动驾驶模式打开
{
	OLED_CLS();
	NVIC_DisableIRQ(PIT1_IRQn);
	NVIC_DisableIRQ(PIT0_IRQn);


	BUZZDOWN;
	ParameterInit();
	BUZZUP;
	CommonDelay(100);
	BUZZDOWN;
	CommonDelay(100);
	BUZZUP;
	CommonDelay(100);
	BUZZDOWN;



	NVIC_EnableIRQ(PIT0_IRQn);



	CarInfo.time_s = GetSysTime();
	while ((GetSysTime() - CarInfo.time_s) <= 1.5);  /////2S延时启动


	CarInfo.time_s = 0;   /////计时清零
	//ImgVSY_port->ISFR = 1 << ImgVSY_pin;/////清除场中断标志位
	//NVIC_EnableIRQ(PORTA_IRQn);
	CarInfo.Motor_ON = 'T';
	IsWifiSendOK = 'F';
	while (1)
	{
		if (SystemSettings.Wifi_Init == 'T' && IsWifiSendOK == 'T')
		{
			ESP8266_SendData();
			if (WifiOrder == 2)
			{
				CarInfo.IsStop = 'T';
			}
			IsWifiSendOK = 'F';
		}
		OLED_PrintStr(0, 0, "Cross:", 10, 1);
		OLED_PrintVar(70, 0, RealRouteBox[RealCrossType], 3, 1);
		OLED_PrintStr(0, 1, "Cirque:", 10, 1);
		OLED_PrintVar(70, 1, RealRouteBox[RealCirqueType], 3, 1);
		OLED_PrintStr(0, 2, "Obstacle:", 10, 1);
		OLED_PrintVar(70, 2, RealRouteBox[RealObstacleType], 3, 1);
		OLED_PrintStr(0, 3, "ExCur:", 10, 1);
		OLED_PrintVar(70, 3, RunningData.ExpectCurvature, 3, 2);
		if (RoadType == Obstacle || ImageStatus.ObstacleDelay == 'T')
			OLED_PrintStr(80, 4, "Obstacl", 7, 0);
		else if (ImageStatus.IsCinqueOutIn == 'T')
			OLED_PrintStr(80, 4, "CirOut", 7, 0);
		else if (ImageStatus.CirqueOut == 'T')
			OLED_PrintStr(80, 4, "CirIn", 7, 0);
		else if (RoadType == Cirque)
			OLED_PrintStr(80, 4, "Cirque", 7, 0);
		else if (RoadType == Straight)
			OLED_PrintStr(80, 4, "Straig", 7, 0);
		else if (RoadType == Cross)
			OLED_PrintStr(80, 4, "Cross", 7, 0);
		else if (RoadType == Normal)
			OLED_PrintStr(80, 4, "Normal", 7, 0);
		if (ImageStatus.ObstacleDelay == 'T')
		{
			OLED_PrintVar(0, 4, ImageStatus.ObstacleOverDistance, 3, 1);
		}
		else
			OLED_PrintStr(0, 4, "     ", 7, 0);

		if (SpecialOn == 'T')
		{
			OLED_PrintStr(0, 5, "Special", 8, 1);
			OLED_PrintVar(80, 5, SpecialData.Angle, 5, 1);

		}
		else
		{
			OLED_PrintStr(0, 5, "       ", 8, 1);
			OLED_PrintStr(80, 5, "       ", 5, 1);

		}
		if (RampOn == 'T')
			OLED_PrintStr(0, 6, "Ramp", 8, 1);
		else
			OLED_PrintStr(0, 6, "       ", 8, 1);

		if (ImageStatus.RampWayFlag == 'T')
			OLED_PrintStr(60, 7, "RampOn", 8, 1);
		else
			OLED_PrintStr(60, 7, "       ", 8, 1);
		OLED_PrintVar(0, 7, CarInfo.OffsetAll, 5, 2);

		if (CarInfo.StartPass == 'T')
		{
			OLED_PrintStr(70, 7, "Start", 6, 1);
		}
		else
			OLED_PrintStr(70, 7, "     ", 6, 1);
	}
}


void Nextmenu(void)
{
	OLED_CLS();
	if (Cursor == 0)
	{
		BalanceData_Set();
	}
	else if (Cursor == 1)
	{
		YawData_Set();
	}
	else if (Cursor == 2)
	{
		ImageData_Set();
	}
	else if (Cursor == 3)
	{
		ImageSensorView();
	}
	else if (Cursor == 4)
	{
		Set_SystemCheck();
		OLED_CLS();
	}
	else if (Cursor == 5)
	{
		SpecialDealChange();
		OLED_CLS();
	}
	else if (Cursor == 6)
	{
		Data_WR();
		OLED_CLS();
	}
	else if (Cursor == 7)
	{
		Motor_view();
		OLED_CLS();
	}
	else if (Cursor == 8)
	{
		Gyro_offsetview();
		OLED_CLS();
	}
	else
	{
		NowPage--;
	}
}


void OS_menu(void)
{
	NowPage = 2;     //当前页为2号页
	uint8 y = 0;
	Cursor = 0;    //当前光标为0
	Menustatus = 0;
	OLED_CLS();     //清屏

	while (1)
	{
		Key_Event();
		if (Cursor == Main_menuH)Cursor = 0;
		if (SystemSettings.Wifi_Init == 'T')
			RxMain();

		if (WifiOrder == 1) { IsStart = 'T'; WifiOrder = 0; }
		else if (WifiOrder == 65536) { OLED_CLS(); My_FlashWrite(0); OLED_CLS(); CommonDelay(100); WifiOrder = 0; }
		if (Cursor == Main_menuH) Cursor = Main_menuH - 1;
		switch (NowPage)
		{
		case 1:NowPage = 2;
			break;
		case 2:

			for (y = 0; y < 8; y++)
			{
				if (Cursor < 8)
				{
					if (y == Cursor)
					{
						OLED_PrintStr(0, y, Main_menu[y], Main_menuL, 0);     //光标选中反码输出
					}
					else
						OLED_PrintStr(0, y, Main_menu[y], Main_menuL, 1);
				}
				else
				{
					if (y == 7)
					{
						OLED_PrintStr(0, y, Main_menu[Cursor - 7 + y], Main_menuL, 0);     //光标选中反码输出
					}
					else
						OLED_PrintStr(0, y, Main_menu[Cursor - 7 + y], Main_menuL, 1);
				}
			}
			OLED_PrintBat(90, 0, CarInfo.Battery);
			break;
		case 3:Nextmenu();
			Cursor = 0;
			break;
		}
	}
}
