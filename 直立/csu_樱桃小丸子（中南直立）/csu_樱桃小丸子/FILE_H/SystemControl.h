#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
  float Xoffset;
  float Yoffset;
  float Zoffset;
  uint8 GyrooffsetOK;
}Gyrotypedef;


typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
}Acctypedef;

typedef struct{
  float P;
  float I;
  float D;
}PIDcontroltypedef;

typedef struct{
  int16 R;
  int16 L;
  int16 M;
  int16 BUFF;
}Carpulsetypedef;


typedef struct{
  float ExpectPath;
  float RealPath;
}Pathtypedef;


typedef struct{
  float MinSpeed;           ///最低速
  float MinR;              ///最小半径
  float MaxR;              ///最大半径
  float MaxAcc;            ///最大向心力
  float MaxSpeed;          ///最大速度
  float ExpectAngleGrow;   ///期望角度增幅限制
  float ExpectAngleReduce; ///期望角度降幅限制
  float BalanceAngle;      ///平衡角度
  float AngleMax;          ///最大角度
  float AngleMin;          ///最小角度
  float BodyRadius;         ///车体半径 
  float LimitRmax;
  float ForceStopR;
  float ForceStopL;
  float StopDelay;


  uint8  Quaternion_init;
  uint8  Motor_ON;
  uint8  IsStop;

  float speed_R;
  float speed_L;
  float speed_M;
  float speed_Mtrue;           //真实速度
  float speed_Rtrue;
  float speed_Ltrue;
  float speed_RtrueEncoder;
  float speed_LtrueEncoder;
  float TureSpeedGrowLimit;
  float TureSpeedReduceLimit;
  float TrueSpeedMAXLimit;
  float TrueSpeedYawRate;
  float SpeedLimitGrowTh;
  float SpeedLimitReduceTh;

  float pitchrate;       //俯仰角速度
  float pitch;           //俯仰角
  float yaw;             //偏航角
  float yawrate;        //偏航角速度
  float rollrate;
  float roll;
  float radius;          //半径
  float curvature;       //曲率

  float time_s;         //时间

  float length;            //距离,精准测距
  int16 lengthcount;
  float lengthcarry;
  float TrueLength;       //运动真实距离
  float Run_Distance;      ////运行距离
  
  float OffsetAll;

  float Battery;

  float NowSiteX;
  float NowSiteY;
  
  
  uint8 StartPass;
  uint8 WheelType;
}CarInfotypedef;

typedef struct{
  uint8 Wifi_ON;           ////WIFI是否打开
  uint8 ResetTest;      ////是否发送图像
  uint8 Wifi_SendWave;     ////是否发送波形
  uint8 Wifi_Init;          ////WIFI初始化完成
  uint8 Run_DistanceLimit; ////运行距离限制
  uint8 SpecialCheck;       
  uint8 StartCheck;         ///起跑检测
  uint8 RampCheck;          ///坡道检测
  uint8 CrossCheck;        ///圆环检测
  uint8 SpeedControl;       ///速度闭环
  uint8 SpeedLimit;
  uint8 CrossMask;
}SystemSettingstypedef;

typedef struct{
  float StartAngle;        ///起跑角度
  float ExpectAngleMax;      ///期望角度最大值
  float ExpectAngleMin;      ///期望角度最小值
  float ExpectBalanceAngle;  ///期望均衡角度
  float ExpectRampAngle;
  float ExpectSpeed;         ///期望速度
  float ExpectCurvature;     ///期望曲率
  float StartDistance;       //起跑距离
  uint8  RoadType;            //路况
  uint8 IsStart;              //是否起跑完成
}RunningDatatypedef;

extern RunningDatatypedef     RunningData;
extern SystemSettingstypedef  SystemSettings;
extern Gyrotypedef            Gyro;    
extern Acctypedef             Acc;
extern PIDcontroltypedef      pitchPIDcontrol;    //俯仰角度控制系数
extern PIDcontroltypedef      yawPIDcontrol[2];   //偏航角控制参数
extern Carpulsetypedef        Carpulse;
extern PIDcontroltypedef      SpeedControl;
extern CarInfotypedef         CarInfo;
extern PIDcontroltypedef      SpeedRadius;

extern float ExpectCurvature[8];  //////偏差对应转向（曲率 * 100）
extern float PicErr[8];


void Get_Car_Angle(void);
void Get_Car_curvature(void);
void Get_Car_Time(void);
void Car_motorControl(void);
void Get_Gyro_Offset(void);
void SystemStart(void);
void ParameterInit(void);

#endif
