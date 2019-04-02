#include "GeneralDeclare.h"
#include "TimerEventDriver.h"
#include "ProjectMath.h"
#include "Attitude_Calculation.h"
#include "Terminator.h"
#include "SystemControl.h"
#include "SoftIIC.h"
#include "PulseCollectDriver.h"
#include "RoadLoad.h"
#include "KeyBoardDriver.h"

float   PCconvert = 0.1750881; 
#define ADconvert           (float)0.000212960 

#define RampSpeedLimit      80
#define Yaw_Control_I_Max   1000        
#define Yaw_Control_P_Max   1000       

Gyrotypedef            Gyro;             
Acctypedef             Acc;             
EulerAngleTypedef      Car_Attitude;     
EulerAngleTypedef      Car_AttitudeRate;  
Carpulsetypedef        Carpulse;        
CarInfotypedef         CarInfo; 

PIDcontroltypedef      yawPIDcontrol[2];
PIDcontroltypedef      pitchPIDcontrol;
PIDcontroltypedef      SpeedControl;
PIDcontroltypedef      SpeedRadius;
RunningDatatypedef     RunningData;
SystemSettingstypedef  SystemSettings;

float                   NormalBat = 7.2;
static float           SetAngleTemp = 0; 
extern volatile uint8  SpecialOn;  
extern volatile uint8  RampOn;  
extern volatile uint32 SysTicks; 
void ParameterInit(void) 
{
	SysTicks = 0;
	CarInfo.time_s = 0;
	CarInfo.yaw = 0;
	CarInfo.Motor_ON = 'F';
	CarInfo.Quaternion_init = 'F';
	CarInfo.ExpectAngleGrow = 40;
	CarInfo.ExpectAngleReduce = 30;
	CarInfo.MaxSpeed = 600;
	CarInfo.MinSpeed = 300;
	CarInfo.TrueSpeedMAXLimit = 600;
	CarInfo.TureSpeedGrowLimit = 300;
	CarInfo.TureSpeedReduceLimit = 300;
	CarInfo.length = 0;
	CarInfo.lengthcarry = 0;
	CarInfo.lengthcount = 0;
	CarInfo.TrueLength = 0;
	CarInfo.speed_Mtrue = 0;
	CarInfo.speed_Ltrue = 0;
	CarInfo.speed_LtrueEncoder = 0;
	CarInfo.speed_Rtrue = 0;
	CarInfo.speed_RtrueEncoder = 0;
	CarInfo.BodyRadius = 8.9;
	CarInfo.AngleMax = 28;
	CarInfo.AngleMin = -20;
	RunningData.IsStart = 0;
	SetAngleTemp = CarInfo.AngleMax;
	CarInfo.IsStop = 'F';

	ImageInfo.CameraFocusLength = 0.196;
	ImageInfo.CameraAxasLength = 23.5;
	ImageInfo.CirqueLimitRoute = 80;
	CarInfo.LimitRmax = 50;
	CarInfo.MaxR = 49;
	SystemSettings.SpeedControl = 'T';


	ImageStatus.ObstacleDelay = 'F'; 
	ImageStatus.CirquePass = 'F';      
	ImageStatus.CirqueOut = 'F';     
	ImageStatus.IsCinqueOutIn = 'F';  
	ImageStatus.ExpectCur = 0;         
	SystemSettings.SpecialCheck = 'F'; 
	ImageStatus.IsStartPass = 'F';    
	ImageStatus.RampWayFlag = 'F';    
	RoadType = Normal;               
	CarInfo.StartPass = 'F';
	RampOn = 'F';
	SpecialOn = 'F';

	if (SpecialRoute[0].Mask == 'T')
		SystemSettings.SpecialCheck = 'T';
 

	if (SystemSettings.CrossCheck == 'T')
	{
		PTC_BASE_PTR->PDOR &= ~(1 << 4);
	}
	else
		PTC_BASE_PTR->PDOR |= ~(1 << 4);

	if (SystemSettings.SpeedControl == 'T')
	{
		PTC_BASE_PTR->PDOR &= ~(1 << 5);
	}
	else
		PTC_BASE_PTR->PDOR |= (1 << 5);

	if (SystemSettings.RampCheck == 'T')
	{
		PTC_BASE_PTR->PDOR &= ~(1 << 6);
	}
	else
		PTC_BASE_PTR->PDOR |= (1 << 6);
        
        if(CarInfo.WheelType == 0)
          PCconvert = 0.1750881;
        else if(CarInfo.WheelType == 1)
          PCconvert = 0.18330;
        else
          PCconvert = 0.1750881;
}

float AccZAngle = 0; 
extern uint32 SendFloat[25];

void Get_Gyro_Offset(void)
{
	static uint16  n = 0;
	static float XF = 0, YF = 0, ZF = 0;

	IIC_Read_Sensor(&Gyro, &Acc);
	XF += Gyro.Xdata;
	YF += Gyro.Ydata;
	ZF += Gyro.Zdata;
	n++;

	if (n == 1000)
	{
		Gyro.Xoffset = XF * 0.001f;
		Gyro.Yoffset = YF * 0.001f;
		Gyro.Zoffset = ZF * 0.001f;
		Gyro.GyrooffsetOK = 1;
		XF = 0;
		YF = 0;
		ZF = 0;
		n = 0;
	}
	CommonDelay(2);
	return;
}



void Get_Car_Angle(void) 
{
	static float AccZ = 0;
	static float LastAngle = 0;
	static float AccZAdjust = 0;
	IIC_Read_Sensor(&Gyro, &Acc); 
#if (UsingSensor == 0)
	Gyro.Xdata = (Gyro.Xdata - Gyro.Xoffset) * 0.03060163;
	Gyro.Ydata = (Gyro.Ydata - Gyro.Yoffset) * 0.03060163;
	Gyro.Zdata = (Gyro.Zdata - Gyro.Zoffset) * 0.03060163;

	Acc.Xdata *= 0.0009765625;
	Acc.Ydata *= 0.0009765625;
	Acc.Zdata *= 0.0009765625;

#elif (UsingSensor == 1)
	Gyro.Xdata = (Gyro.Xdata - Gyro.Xoffset) * 0.03060163;
	Gyro.Ydata = (Gyro.Ydata - Gyro.Yoffset) * 0.03060163;
	Gyro.Zdata = (Gyro.Zdata - Gyro.Zoffset) * 0.03060163;

	Acc.Xdata *= 0.00048828;
	Acc.Ydata *= 0.00048828;
	Acc.Zdata *= 0.00048828;
	Acc.Zdata = (Acc.Zdata + 0.95) / (0.95 + 1.05) * 2 - 1;
	Acc.Ydata = (Acc.Ydata + 1.04) / (0.96 + 1.04) * 2 - 1;
	Acc.Xdata = (Acc.Xdata + 0.97) / (0.97 + 1.03) * 2 - 1;

#endif  


	if (CarInfo.Quaternion_init == 'F')
	{
		Quaternion_init();             
		CarInfo.Quaternion_init = 'T';
	}
	Attitude_UpdateGyro();
	Attitude_UpdateAcc();

	CarInfo.pitchrate = -EulerAngleRate.Roll / PI * 180;
	CarInfo.yawrate = EulerAngleRate.Yaw / PI * 180;    
	CarInfo.pitch = -EulerAngle.Roll / PI * 180;    
	CarInfo.yaw = EulerAngle.Yaw / PI * 180;
	CarInfo.rollrate = EulerAngleRate.Pitch / PI * 180;
	AccZ = -Acc.Zdata;
	if (AccZ > 1)
		AccZ = 1;
	if (AccZ < -1)
		AccZ = -1;            
	AccZAngle = asinf(AccZ) * 180 / PI;
	LastAngle = CarInfo.pitch;
	AccZAdjust = (AccZAngle - LastAngle);
	CarInfo.pitch += (-Gyro.Ydata + AccZAdjust) * 0.002;
	return;
}



void Get_Car_curvature(void)
{
	float devspeed = 0;
	float sumspeed = 0;

	devspeed = CarInfo.speed_Ltrue - CarInfo.speed_Rtrue;
	sumspeed = CarInfo.speed_Rtrue + CarInfo.speed_Ltrue;


	if (sumspeed == 0)CarInfo.curvature = 0;
	else CarInfo.curvature = devspeed / (sumspeed * CarInfo.BodyRadius);
}


void Get_TrueSpeed(void)
{
	float SpeedLw = CarInfo.speed_L - CarInfo.BodyRadius * CarInfo.yawrate * PI / 180;
	float SpeedRw = CarInfo.speed_R + CarInfo.BodyRadius * CarInfo.yawrate * PI / 180;
	float SpeedMw = CarInfo.speed_M;

	float errLw = ABSF(SpeedLw - CarInfo.speed_Mtrue);
	float errRw = ABSF(SpeedRw - CarInfo.speed_Mtrue);
	float errLR = ABSF(SpeedMw - CarInfo.speed_Mtrue);

	float speed = 0;
	if (CarInfo.yawrate > 30)
	{
		speed = SpeedLw;   
	}
	else if (CarInfo.yawrate < -30)
	{
		speed = SpeedRw;   
	}
	else
	{
		speed = SpeedMw;     
	}
	if ((speed - CarInfo.speed_Mtrue) > CarInfo.TureSpeedGrowLimit / 500)
		CarInfo.speed_Mtrue += CarInfo.TureSpeedGrowLimit / 500;
	else if ((speed - CarInfo.speed_Mtrue) < -CarInfo.TureSpeedReduceLimit / 500)
		CarInfo.speed_Mtrue -= CarInfo.TureSpeedReduceLimit / 500;
	else
		CarInfo.speed_Mtrue = speed;

	if (CarInfo.speed_Mtrue > CarInfo.TrueSpeedMAXLimit)
		CarInfo.speed_Mtrue = CarInfo.TrueSpeedMAXLimit;

	CarInfo.speed_Ltrue = CarInfo.speed_Mtrue + CarInfo.BodyRadius * CarInfo.yawrate * PI / 180;
	CarInfo.speed_Rtrue = CarInfo.speed_Mtrue - CarInfo.BodyRadius * CarInfo.yawrate * PI / 180;


	if ((CarInfo.speed_L - CarInfo.speed_LtrueEncoder) > CarInfo.TureSpeedGrowLimit / 500)
		CarInfo.speed_LtrueEncoder += CarInfo.TureSpeedGrowLimit / 500;
	else if ((CarInfo.speed_L - CarInfo.speed_LtrueEncoder) < -CarInfo.TureSpeedReduceLimit / 500)
		CarInfo.speed_LtrueEncoder -= CarInfo.TureSpeedGrowLimit / 500;
	else
		CarInfo.speed_LtrueEncoder = CarInfo.speed_L;

	if ((CarInfo.speed_R - CarInfo.speed_RtrueEncoder) > CarInfo.TureSpeedGrowLimit / 500)
		CarInfo.speed_RtrueEncoder += CarInfo.TureSpeedGrowLimit / 500;
	else if ((CarInfo.speed_R - CarInfo.speed_RtrueEncoder) < -CarInfo.TureSpeedReduceLimit / 500)
		CarInfo.speed_RtrueEncoder -= CarInfo.TureSpeedGrowLimit / 500;
	else
		CarInfo.speed_RtrueEncoder = CarInfo.speed_R;

	CarInfo.TrueSpeedYawRate = (CarInfo.speed_LtrueEncoder - CarInfo.speed_RtrueEncoder) / (2 * CarInfo.BodyRadius) * 180 / PI;

	return;
}



PulseCounttypedef pulseBUFF;
void Get_Car_Speed(void) 
{
	GetPulse(&pulseBUFF);
	CarInfo.lengthcount += pulseBUFF.Lcount + pulseBUFF.Rcount;
	CarInfo.speed_R = (float)pulseBUFF.Rcount * PCconvert * 50;
	CarInfo.speed_L = (float)pulseBUFF.Lcount * PCconvert * 50;


	CarInfo.speed_M = (CarInfo.speed_R + CarInfo.speed_L) / 2;
	Get_TrueSpeed();
	CarInfo.NowSiteX += sinf(CarInfo.yaw * PI / 180) * CarInfo.speed_Mtrue * 0.002;
	CarInfo.NowSiteY += cosf(CarInfo.yaw * PI / 180) * CarInfo.speed_Mtrue * 0.002;
}



void Get_Car_Length(void)
{
	if (CarInfo.lengthcount >= 1200)
	{
		CarInfo.lengthcarry += 1.2 * PCconvert * 0.5;
		CarInfo.lengthcount -= 1200;
	}
	CarInfo.length = CarInfo.lengthcarry + (float)CarInfo.lengthcount * PCconvert  * 0.0005;
	CarInfo.TrueLength += CarInfo.speed_Mtrue * 0.002 / 100;
}



void Get_Car_BAT(void)
{
	static float LastBAT = 0;
	uint16 buff = BAT_AD();
	if (LastBAT == 0)LastBAT = buff;
	LastBAT += (buff - LastBAT) * 0.1;
	CarInfo.Battery = (float)LastBAT * ADconvert;
}



Pathtypedef    PathHitory[40];
static int    PathHitoryCount = 0;
static uint32 PathCount = 0;
void PathMemory(float ExpCurvature, float RealCurvature)
{
	PathHitory[PathHitoryCount].ExpectPath = ExpCurvature;
	PathHitory[PathHitoryCount].RealPath = RealCurvature;
	PathHitoryCount++;
	PathCount++;
	if (PathHitoryCount == 40)
		PathHitoryCount = 0;
}

float PathDeal(void)
{
	if (PathCount < 40)return 0;
	float PathOffset = 0;
	int i = 0;
	for (i = 0; i < 40; i++)
	{
		PathOffset += PathHitory[i].ExpectPath - PathHitory[i].RealPath;
	}
	return PathOffset;
}



static int Set_CurrentAngle(void)
{
	float SetAngle = 0;
	float ErrAngle = 0;
	int   Anglepwm = 0;
	float SpeedErr = 0;
	float SpeedAngle = 0;


	if (RunningData.IsStart == 0)
	{
		SetAngle = RunningData.StartAngle;
	}
	else
	{
		if (SystemSettings.SpeedControl == 'T')
		{
			if (RoadType == Straight)
			{
				SpeedErr = CarInfo.speed_Mtrue - RunningData.ExpectSpeed;
				SpeedAngle = RunningData.ExpectBalanceAngle - SpeedErr * SpeedControl.P;
				if (SpeedAngle > RunningData.ExpectBalanceAngle)
					SetAngle = RunningData.ExpectBalanceAngle;
				else if (SpeedAngle < RunningData.ExpectAngleMin)
					SetAngle = RunningData.ExpectAngleMin;
				else                
					SetAngle = SpeedAngle;  
			}
			else if (RouteStatus[RealCrossType] == 'I')
			{
				SetAngle = RunningData.ExpectAngleMax;
			}
			else
			{
				SetAngle = RunningData.ExpectBalanceAngle;
			}
		}
		else
		{
			SetAngle = RunningData.ExpectBalanceAngle;
		}

		if (SpecialOn == 'T') 
		{
			if (CarInfo.length < SpecialData.End && CarInfo.length > SpecialData.Start)
			{
				SetAngle = SpecialData.Angle;
			}
			else if (CarInfo.length >= SpecialData.End)
			{
				SpecialOn = 'F';
				RampOn = 'F';
			}
		}
		if (RoadType == Ramp)
		{
			SetAngle = RunningData.ExpectRampAngle;
		}
	}

	if ((-SetAngleTemp + SetAngle) > CarInfo.ExpectAngleGrow * 0.002)
	{
		SetAngleTemp += CarInfo.ExpectAngleGrow * 0.002;
	}
	else if ((-SetAngleTemp + SetAngle) < (-CarInfo.ExpectAngleReduce * 0.002))
	{
		SetAngleTemp -= CarInfo.ExpectAngleReduce * 0.002;
	}
	else
	{
		SetAngleTemp = SetAngle;
	}

	if (ImageStatus.IsStartPass == 'T')
		SetAngleTemp = RunningData.ExpectAngleMax;

	if (SetAngleTemp >= CarInfo.AngleMax)
		SetAngleTemp = CarInfo.AngleMax;
	else if (SetAngleTemp <= CarInfo.AngleMin)
		SetAngleTemp = CarInfo.AngleMin;

	SendFloat[15] = FloattoUint32(SetAngleTemp);
	ErrAngle = CarInfo.pitch - (-1.5f + SetAngleTemp);

	Anglepwm = (int)(pitchPIDcontrol.P * ErrAngle + pitchPIDcontrol.D * CarInfo.pitchrate);

	return (Anglepwm);
}


static int Set_Curvature(float Excurvature)
{
	static float   Yaw_Control_I = 0;
	static float   ExYawRate = 0;
	static int     OutPwm = 0;
	float SpeedMinRadius = 0;
	float PathMinRadius = 0;
	float MinRadius = 0;
	float MaxCurvature = 0;
	float SpeedGainCur = 1;

	if (CarInfo.speed_Mtrue > CarInfo.MinSpeed)
	{
		SpeedMinRadius = CarInfo.speed_Mtrue * CarInfo.speed_Mtrue * 0.01 / CarInfo.MaxAcc;
		if (SpeedMinRadius < CarInfo.MinR)
			MinRadius = CarInfo.MinR;
	}
	else
		MinRadius = CarInfo.MinR;


	float th = 60;
	static float inte = 0;
	float b = 50;
	float rate = 1000;

	if (CarInfo.yawrate > 0)
	{ 
		inte += 0.002 * (CarInfo.yawrate - th);
		if (Excurvature < 0.005)
			inte -= 0.002 * rate;
		if (inte > 150) inte = 150;
		if (inte < 0) inte = 0;
	}
	else
	{
		inte -= 0.002 * (-CarInfo.yawrate - th);
		if (Excurvature > -0.005)
			inte += 0.002 * rate;
		if (inte < -150) inte = -150;
		if (inte > 0) inte = 0;
	}
	float inteABS = ABSF(inte);
	if (inteABS > b)
	{
		PathMinRadius = CarInfo.MinR + (inteABS - b) * 0.002 * 100;
	}
	else
	{
		PathMinRadius = CarInfo.MinR;
	}

	if (PathMinRadius > CarInfo.MaxR)
		PathMinRadius = CarInfo.MaxR;

	if (PathMinRadius > SpeedMinRadius)
		MinRadius = PathMinRadius;
	else
		MinRadius = SpeedMinRadius;

	if (MinRadius >= CarInfo.LimitRmax)
	{
		MinRadius = CarInfo.LimitRmax;
		if (CarInfo.yawrate > th)
		{
			inte = 150;
		}
		else if (CarInfo.yawrate < -th)
		{
			inte = -150;
		}
	}


	if (CarInfo.speed_Mtrue > CarInfo.MinSpeed)
	{
		SpeedGainCur = ((CarInfo.speed_Mtrue - CarInfo.MinSpeed) * SpeedRadius.P + 1);
		if (SpeedGainCur > 1.8)  SpeedGainCur = 1.8;
	}
	Excurvature *= SpeedGainCur;

	if (RouteStatus[RealCrossType] == 'I' && CrossBox[RealRouteBox[RealCrossType]] == 1)
		MinRadius = CarInfo.LimitRmax;
        else if(RouteStatus[RealCrossType] == 'I')
                MinRadius = (CarInfo.LimitRmax + CarInfo.MinR) / 2;
	else if (ImageStatus.ObstacleDelay == 'T')
		MinRadius = CarInfo.MinR;
	if (RoadType == Ramp)
		MinRadius = 100;
	MaxCurvature = 1 / MinRadius; 



	if (Excurvature > MaxCurvature)Excurvature = MaxCurvature;
	else if (Excurvature < -MaxCurvature)Excurvature = -MaxCurvature; 

	ExYawRate = Excurvature * CarInfo.speed_Mtrue * 180 / PI;

	float Yaw_Control_P = ExYawRate - CarInfo.yawrate;

	Yaw_Control_I += Yaw_Control_P * 0.002;

	if (Yaw_Control_I > Yaw_Control_I_Max)Yaw_Control_I = Yaw_Control_I_Max;
	else if (Yaw_Control_I < -Yaw_Control_I_Max)Yaw_Control_I = -Yaw_Control_I_Max;

	if (Yaw_Control_P > Yaw_Control_P_Max)Yaw_Control_P = Yaw_Control_P_Max;
	else if (Yaw_Control_P < -Yaw_Control_P_Max)Yaw_Control_P = -Yaw_Control_P_Max;

	float SetP, SetD;
	if (RunningData.IsStart == 0 || ImageStatus.IsStartPass == 'T')
	{
		SetP = yawPIDcontrol[0].P;
		SetD = yawPIDcontrol[0].D;
	}
	else if (RunningData.IsStart == 1)
	{
		SetP = yawPIDcontrol[1].P;
		SetD = yawPIDcontrol[1].D;
	}

	if (Yaw_Control_P * CarInfo.yawrate < 0)
		SetD = 0;


	OutPwm = (int)(SetP * Yaw_Control_P + SetD * CarInfo.yawrate);
	return (OutPwm);
}



float PicErr[8] = { 0 };
float ExpectCurvature[8] = { 0 };
static float CurvatureControl(float Curvature)
{
	int Xb = 0;
	float DownRadio = 0;
	float OUTCurvature = 0;
	for (Xb = 0; Xb <= 8; Xb++)
	{
		if (Xb == 8)break;
		if (ABSF(Curvature) <= PicErr[Xb])
			break;
	}
	if (Xb == 0)
	{
		DownRadio = ABSF(Curvature) / PicErr[0];
		if (Curvature > 0)
		{
			OUTCurvature = ExpectCurvature[0] * DownRadio;
		}
		else if (Curvature < 0)
		{
			OUTCurvature = -ExpectCurvature[0] * DownRadio;
		}
		else
			OUTCurvature = 0;
	}
	else if (Xb == 8)
	{
		if (Curvature > 0)
			OUTCurvature = ExpectCurvature[7];
		else if (Curvature < 0)
			OUTCurvature = -ExpectCurvature[7];
		else
			OUTCurvature = 0;
	}
	else
	{
		DownRadio = (ABSF(Curvature) - PicErr[Xb - 1]) / (PicErr[Xb] - PicErr[Xb - 1]);
		if (Curvature > 0)
		{
			OUTCurvature = (ExpectCurvature[Xb - 1] + (ExpectCurvature[Xb] - ExpectCurvature[Xb - 1]) * DownRadio);
		}
		else if (Curvature < 0)
		{
			OUTCurvature = -(ExpectCurvature[Xb - 1] + (ExpectCurvature[Xb] - ExpectCurvature[Xb - 1]) * DownRadio);
		}
		else
			OUTCurvature = 0;
	}
	return OUTCurvature;
}



static float PictureCalculation(float PicData)
{
	static float LastData = 0;
	float ErrData = PicData - LastData;
	if (ErrData >= 0.3)
		ErrData = 0.3;
	else if (ErrData <= -0.3)
		ErrData = -0.3;
	LastData = PicData;
	return (SpeedRadius.D * ErrData + PicData);
}


void Motor_Disable(void)
{
	PTD_BASE_PTR->PDOR &= ~(1 << 2);
	PTD_BASE_PTR->PDOR &= ~(1 << 3);
}



void Motor_Enable(void)
{
	PTD_BASE_PTR->PDOR |= (1 << 2);
	PTD_BASE_PTR->PDOR |= (1 << 3);
}

float P = 1.5, I = 0.025;
int TestPID(float speed)
{
  float ErrSpeed = (speed - CarInfo.speed_M);
  static int LastPWM = 0;
  static float LastErr = 0;
  LastPWM += (int)(P * (ErrSpeed - LastErr) + I * ErrSpeed);
  LastErr = ErrSpeed;
  return LastPWM;
}

float Errtest = 1.5;
void Car_motorControl(void)
{
	static uint8 IsStopDo = 'F';
	static int   loopcount = 0;
	int Lpwm = 0, Rpwm = 0, Anglepwm = 0, Directionpwm = 0;
	uint8 SpeedRLimit_Grow = 'F';
	uint8 SpeedRLimit_Reduce = 'F';
	uint8 SpeedLLimit_Grow = 'F';
	uint8 SpeedLLimit_Reduce = 'F';

	RunningData.ExpectCurvature = CurvatureControl(PictureCalculation(ImageStatus.ExpectCur));

	if (RunningData.IsStart == 0 && CarInfo.TrueLength >= RunningData.StartDistance)
	{
		RunningData.IsStart = 1;
	}
       if (CarInfo.Motor_ON == 'T')
	Anglepwm = TestPID(150);

	Directionpwm = Set_Curvature(0.01 * Errtest);


	Lpwm = (int)((float)(Anglepwm + Directionpwm) * NormalBat / CarInfo.Battery);
	Rpwm = (int)((float)(Anglepwm - Directionpwm) * NormalBat / CarInfo.Battery);
	/*if (CarInfo.IsStop == 'T')
	{
		Lpwm = -(int)CarInfo.ForceStopL + Directionpwm;
		Rpwm = -(int)CarInfo.ForceStopR - Directionpwm;
	}*/

	if (Lpwm > 999)Lpwm = 999;
	else if (Lpwm < -999)Lpwm = -999;
	if (Rpwm > 999)Rpwm = 999;
	else if (Rpwm < -999)Rpwm = -999;

	/*if (SystemSettings.SpeedLimit == 'T' && CarInfo.IsStop == 'F')
	{
		if (RoadType == Ramp)
		{
			if (CarInfo.speed_L >(CarInfo.speed_Ltrue + CarInfo.SpeedLimitGrowTh + RampSpeedLimit))
				SpeedLLimit_Grow = 'T';
			else if (CarInfo.speed_L < (CarInfo.speed_Ltrue + CarInfo.SpeedLimitReduceTh - RampSpeedLimit))
				SpeedLLimit_Reduce = 'T';
			if (CarInfo.speed_R > (CarInfo.speed_Rtrue + CarInfo.SpeedLimitGrowTh + RampSpeedLimit))
				SpeedRLimit_Grow = 'T';
			else if (CarInfo.speed_R < (CarInfo.speed_Rtrue + CarInfo.SpeedLimitReduceTh - RampSpeedLimit))
				SpeedRLimit_Reduce = 'T';
		}
		else
		{
			if (CarInfo.speed_L > (CarInfo.speed_Ltrue + CarInfo.SpeedLimitGrowTh))
				SpeedLLimit_Grow = 'T';
			else if (CarInfo.speed_L < (CarInfo.speed_Ltrue + CarInfo.SpeedLimitReduceTh))
				SpeedLLimit_Reduce = 'T';
			if (CarInfo.speed_R > (CarInfo.speed_Rtrue + CarInfo.SpeedLimitGrowTh))
				SpeedRLimit_Grow = 'T';
			else if (CarInfo.speed_R < (CarInfo.speed_Rtrue + CarInfo.SpeedLimitReduceTh))
				SpeedRLimit_Reduce = 'T';
		}
	}

	if ((SpeedLLimit_Grow = 'T' && SpeedRLimit_Reduce == 'T') || (SpeedRLimit_Reduce = 'T' && SpeedLLimit_Grow == 'T') || ImageStatus.IsStartPass == 'T')
	{
		SpeedLLimit_Grow = 'F';
		SpeedLLimit_Reduce = 'F';
		SpeedRLimit_Reduce = 'F';
		SpeedRLimit_Grow = 'F';
	}*/

	if (CarInfo.Motor_ON == 'T')
	{
		if (CarInfo.IsStop == 'F')
		{
			if (SystemSettings.SpeedLimit != 'T')
				PWM_Set(Rpwm, Lpwm);
			else
			{
				if (SpeedLLimit_Grow == 'T')
					Lpwm = -999;
				else if (SpeedLLimit_Reduce == 'T')
					Lpwm = 999;
				if (SpeedRLimit_Grow == 'T')
					Rpwm = -999;
				else if (SpeedRLimit_Reduce == 'T')
					Rpwm = 999;
				PWM_Set(Rpwm, Lpwm);
			}
		}
		else if (CarInfo.IsStop == 'T' && IsStopDo == 'F')
		{
			loopcount++;
			if (loopcount > 300)
			{
				Motor_Disable();
				IsStopDo = 'T';
				CarInfo.Motor_ON = 'F';
				loopcount = 0;
			}
			PWM_Set(Rpwm, Lpwm);
		}
	}
}

void SystemStart(void)
{

	static uint8 IsCallStop = 'F';
	if (CarInfo.Motor_ON == 'T')
		CarInfo.time_s = GetSysTime();

	Get_Car_Angle(); 
	Get_Car_Speed(); 
	Get_Car_curvature();
	Get_Car_BAT(); 
	Get_Car_Length(); 



	if (SystemSettings.Run_DistanceLimit == 'T'
		&& CarInfo.length > CarInfo.Run_Distance
		&& IsCallStop == 'F')
	{
		IsCallStop = 'T';
		CarInfo.IsStop = 'T';
	}      
	if (SystemSettings.Wifi_Init == 'T')
	{
		SendFloat[0] = FloattoUint32(CarInfo.NowSiteX);
		SendFloat[1] = FloattoUint32(CarInfo.NowSiteY);
		SendFloat[2] = FloattoUint32(CarInfo.Battery);
		SendFloat[3] = FloattoUint32(CarInfo.time_s);
		SendFloat[4] = FloattoUint32(CarInfo.length);
		SendFloat[5] = FloattoUint32(CarInfo.pitch);
		SendFloat[6] = FloattoUint32(CarInfo.rollrate);
		SendFloat[7] = FloattoUint32(CarInfo.yawrate);
		SendFloat[8] = FloattoUint32(CarInfo.speed_Mtrue);
		SendFloat[9] = FloattoUint32(CarInfo.speed_Ltrue);
		SendFloat[10] = FloattoUint32(CarInfo.speed_L);
		SendFloat[11] = FloattoUint32(CarInfo.speed_Rtrue);
		SendFloat[12] = FloattoUint32(CarInfo.speed_R);
		SendFloat[13] = FloattoUint32(1 / CarInfo.curvature);
		SendFloat[14] = FloattoUint32(CarInfo.speed_Mtrue / CarInfo.yawrate * 100);
		SendFloat[16] = FloattoUint32(ImageStatus.ExpectCur);

		if (RoadType == Normal)
			SendFloat[17] = 0;
		else if (RoadType == Obstacle)
			SendFloat[17] = 1;
		else if (RoadType == Cirque)
			SendFloat[17] = 2;
		else if (RoadType == Straight)
			SendFloat[17] = 3;
		else if (RouteStatus[RealCrossType] == 'I')
			SendFloat[17] = 4;

		SendFloat[18] = 10;
		SendFloat[19] = 20;
		SendFloat[20] = 30;
		SendFloat[21] = 40;
		SendFloat[22] = 50;
		SendFloat[23] = 60;
		SendFloat[24] = 70;
	}

	if (RoadType == Ramp)
		BUZZUP;
	else
		BUZZDOWN;
}
