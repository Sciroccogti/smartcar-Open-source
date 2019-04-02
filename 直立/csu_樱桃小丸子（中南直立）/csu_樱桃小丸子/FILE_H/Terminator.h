#define ISRow                60
#define ISCutColumn          10
#define ImageSensorDataSize  (ISRow * 8 * ISCurColumn)
#define ImageSensorMid       39
#define MaxJumpPoint         10
#define ImageScanInterval    4
#define ISColumn             80


typedef struct{
  int   point;
  uint8 type;
}JumpPointtypedef;

typedef struct{
  float   ObstacleDelay;         ////障碍延迟  单位米
  float   RObstacleCenter;       ////右障碍中心
  float   LObstacleCenter;      /////左障碍中心
  float   CirqueLimitRoute;     /////圆环修正距离限制
  float   BodyLimit;             ////车体半径限制
  float   CirqueInSpecial;
  float   CirqueOutSpecial;
  float   LargeCirqueSpecial;
  
  float   TowPointAdjustTh;
  float   TowPointAdjust;
  float   TowPointAdjustMax;
  float   TowPointAdjustMin;
  
  float   RampRKMIN;
  float   RampLKMax;
  
  float   CameraInterAngle;     /////相机轴线夹角
  float   CameraAxasLength;     /////相机轴线长度
  float   CameraZeroAngle;      /////相机零度角时车辆俯仰角
  float   CameraFocusLength;    /////焦距
  float   CameraCalD;           ////相机计算轴长
  float   CameraCalAngle;       /////相机当前俯仰角

  float   TowPointSite;             ///拖曳点
  float   ObstacleGain;      ///目标距离内路径优化
  float   StartCenter;             ////起跑时一段距离的偏向
  float   DirCenter;
  uint8    threshold;              /////阈值
}ImageInfotypedef;

typedef struct{
  /*左右边边界标志    T为正常跳变边    W为无边   P为障碍类多跳边的内边*/
  uint8 IsRightFind;   
  uint8 IsLeftFind;
  
   
  int Wide;              ////边界宽度
  int LeftBorder;        ////左边界
  int RightBorder;       ////右边界
  int Center;            ////中线
  int RightTemp;         ////右边临时值
  int LeftTemp;          /////左边临时值
  int CenterTemp;        ////中线临时值
}ImageDealDatatypedef;

typedef struct{
/*以下关于全局图像正常参数*/   
  uint8    OFFLine;          /////图像顶边
  float   OFFRoute;         /////图像顶边转换厘米
  float   PitchAngle;       ////当前图像俯仰角
  uint8    WhiteLine;        ////双边丢边数

  
  
  uint8   IsDanger; 
  float   ExpectCur;        /////图像期望曲率


/*以下关于障碍*/
  uint8   ObstacleType;
  float   ObstacleDistance;  /////检测到障碍的开始距离
  float   ObstacleCenter;    /////障碍延迟对中
  uint8    ObstacleDelay;     /////障碍延迟标志
  float   ObstacleOverDistance;
 
/*以下关于圆环*/   
  uint8    CirquePass;       /////通过圆环标志
  uint8    CirqueOut;        /////出圆环标志
  uint8    IsCinqueOutIn;
  int    CirquePassLine;
  uint8   CirqueType;
  
/*以下关于起跑线*/  
  uint8     IsStartPass;     ////起跑线通过标志
  float    StartDistance;   ////起跑线距离
  
  
  uint8    CenterGrow;
  uint8    CenterReduce;
  uint8    IsLittleS;
  
  uint8    RampWayFlag;
  float    RBorderK;
  float    LBorderK;
  float   RampStartDistance;
}ImageStatustypedef;

typedef enum
{
  Ramp,           /////坡道
  Cross,          ////十字
  Obstacle,       ////障碍
  Straight,       ////直道
  Cirque,         ////圆环
  Normal,         ////正常
  Start,          ////起跑线
  LargeCirque,    ////大圆环
}RoadType_e;


#define LimitL(L)     (L = ((L < 1) ? 1 : L))
#define LimitH(H)     (H =  ((H > 78) ? 78 : H))

extern ImageInfotypedef      ImageInfo;
extern ImageDealDatatypedef  ImageDeal[ISRow];
extern uint8                 PicForNow[60][80];
extern float                TrueXSite[60];
extern float                TrueYSite[60];
extern uint8                 ImageSensorData[600];
extern ImageStatustypedef    ImageStatus;
extern RoadType_e            RoadType;


void Image_MainDeal(void);
