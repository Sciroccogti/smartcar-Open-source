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
  float MinSpeed;           ///�����
  float MinR;              ///��С�뾶
  float MaxR;              ///���뾶
  float MaxAcc;            ///���������
  float MaxSpeed;          ///����ٶ�
  float ExpectAngleGrow;   ///�����Ƕ���������
  float ExpectAngleReduce; ///�����ǶȽ�������
  float BalanceAngle;      ///ƽ��Ƕ�
  float AngleMax;          ///���Ƕ�
  float AngleMin;          ///��С�Ƕ�
  float BodyRadius;         ///����뾶 
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
  float speed_Mtrue;           //��ʵ�ٶ�
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

  float pitchrate;       //�������ٶ�
  float pitch;           //������
  float yaw;             //ƫ����
  float yawrate;        //ƫ�����ٶ�
  float rollrate;
  float roll;
  float radius;          //�뾶
  float curvature;       //����

  float time_s;         //ʱ��

  float length;            //����,��׼���
  int16 lengthcount;
  float lengthcarry;
  float TrueLength;       //�˶���ʵ����
  float Run_Distance;      ////���о���
  
  float OffsetAll;

  float Battery;

  float NowSiteX;
  float NowSiteY;
  
  
  uint8 StartPass;
  uint8 WheelType;
}CarInfotypedef;

typedef struct{
  uint8 Wifi_ON;           ////WIFI�Ƿ��
  uint8 ResetTest;      ////�Ƿ���ͼ��
  uint8 Wifi_SendWave;     ////�Ƿ��Ͳ���
  uint8 Wifi_Init;          ////WIFI��ʼ�����
  uint8 Run_DistanceLimit; ////���о�������
  uint8 SpecialCheck;       
  uint8 StartCheck;         ///���ܼ��
  uint8 RampCheck;          ///�µ����
  uint8 CrossCheck;        ///Բ�����
  uint8 SpeedControl;       ///�ٶȱջ�
  uint8 SpeedLimit;
  uint8 CrossMask;
}SystemSettingstypedef;

typedef struct{
  float StartAngle;        ///���ܽǶ�
  float ExpectAngleMax;      ///�����Ƕ����ֵ
  float ExpectAngleMin;      ///�����Ƕ���Сֵ
  float ExpectBalanceAngle;  ///��������Ƕ�
  float ExpectRampAngle;
  float ExpectSpeed;         ///�����ٶ�
  float ExpectCurvature;     ///��������
  float StartDistance;       //���ܾ���
  uint8  RoadType;            //·��
  uint8 IsStart;              //�Ƿ��������
}RunningDatatypedef;

extern RunningDatatypedef     RunningData;
extern SystemSettingstypedef  SystemSettings;
extern Gyrotypedef            Gyro;    
extern Acctypedef             Acc;
extern PIDcontroltypedef      pitchPIDcontrol;    //�����Ƕȿ���ϵ��
extern PIDcontroltypedef      yawPIDcontrol[2];   //ƫ���ǿ��Ʋ���
extern Carpulsetypedef        Carpulse;
extern PIDcontroltypedef      SpeedControl;
extern CarInfotypedef         CarInfo;
extern PIDcontroltypedef      SpeedRadius;

extern float ExpectCurvature[8];  //////ƫ���Ӧת������ * 100��
extern float PicErr[8];


void Get_Car_Angle(void);
void Get_Car_curvature(void);
void Get_Car_Time(void);
void Car_motorControl(void);
void Get_Gyro_Offset(void);
void SystemStart(void);
void ParameterInit(void);

#endif
