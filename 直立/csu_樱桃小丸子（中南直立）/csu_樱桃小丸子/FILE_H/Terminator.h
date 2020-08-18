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
  float   ObstacleDelay;         ////�ϰ��ӳ�  ��λ��
  float   RObstacleCenter;       ////���ϰ�����
  float   LObstacleCenter;      /////���ϰ�����
  float   CirqueLimitRoute;     /////Բ��������������
  float   BodyLimit;             ////����뾶����
  float   CirqueInSpecial;
  float   CirqueOutSpecial;
  float   LargeCirqueSpecial;
  
  float   TowPointAdjustTh;
  float   TowPointAdjust;
  float   TowPointAdjustMax;
  float   TowPointAdjustMin;
  
  float   RampRKMIN;
  float   RampLKMax;
  
  float   CameraInterAngle;     /////������߼н�
  float   CameraAxasLength;     /////������߳���
  float   CameraZeroAngle;      /////�����Ƚ�ʱ����������
  float   CameraFocusLength;    /////����
  float   CameraCalD;           ////��������᳤
  float   CameraCalAngle;       /////�����ǰ������

  float   TowPointSite;             ///��ҷ��
  float   ObstacleGain;      ///Ŀ�������·���Ż�
  float   StartCenter;             ////����ʱһ�ξ����ƫ��
  float   DirCenter;
  uint8    threshold;              /////��ֵ
}ImageInfotypedef;

typedef struct{
  /*���ұ߽߱��־    TΪ���������    WΪ�ޱ�   PΪ�ϰ�������ߵ��ڱ�*/
  uint8 IsRightFind;   
  uint8 IsLeftFind;
  
   
  int Wide;              ////�߽���
  int LeftBorder;        ////��߽�
  int RightBorder;       ////�ұ߽�
  int Center;            ////����
  int RightTemp;         ////�ұ���ʱֵ
  int LeftTemp;          /////�����ʱֵ
  int CenterTemp;        ////������ʱֵ
}ImageDealDatatypedef;

typedef struct{
/*���¹���ȫ��ͼ����������*/   
  uint8    OFFLine;          /////ͼ�񶥱�
  float   OFFRoute;         /////ͼ�񶥱�ת������
  float   PitchAngle;       ////��ǰͼ������
  uint8    WhiteLine;        ////˫�߶�����

  
  
  uint8   IsDanger; 
  float   ExpectCur;        /////ͼ����������


/*���¹����ϰ�*/
  uint8   ObstacleType;
  float   ObstacleDistance;  /////��⵽�ϰ��Ŀ�ʼ����
  float   ObstacleCenter;    /////�ϰ��ӳٶ���
  uint8    ObstacleDelay;     /////�ϰ��ӳٱ�־
  float   ObstacleOverDistance;
 
/*���¹���Բ��*/   
  uint8    CirquePass;       /////ͨ��Բ����־
  uint8    CirqueOut;        /////��Բ����־
  uint8    IsCinqueOutIn;
  int    CirquePassLine;
  uint8   CirqueType;
  
/*���¹���������*/  
  uint8     IsStartPass;     ////������ͨ����־
  float    StartDistance;   ////�����߾���
  
  
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
  Ramp,           /////�µ�
  Cross,          ////ʮ��
  Obstacle,       ////�ϰ�
  Straight,       ////ֱ��
  Cirque,         ////Բ��
  Normal,         ////����
  Start,          ////������
  LargeCirque,    ////��Բ��
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
