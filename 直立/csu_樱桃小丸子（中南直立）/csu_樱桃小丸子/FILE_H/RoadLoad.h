#define  RealCirqueType   0
#define  RealObstacleType 1
#define  RealCrossType    2 
#define  RealStraightType 3

typedef struct{
  uint8 Type;
  float Start;
  float End;
  float Angle;
}SpecialDatatypedef;

typedef struct{
  uint8      Mask;
  RoadType_e Road;
  uint8      RoadNum;
  uint8      IsRamp;
  float     StartDistance;
  float     EndDistance;
  float     Angle;
}Specialtypedef;

extern uint8        RouteMask[3];
extern uint8        RealRouteBox[3];
extern uint8        RouteStatus[3];
extern float        RealRouteDistance[3][10];
extern SpecialDatatypedef   SpecialData;
extern Specialtypedef       SpecialRoute[10];
extern uint8                CirqueDirBox[6];
extern uint8                CrossBox[6];

uint8 RouteMaskClear(void);
void RoadTypeTest(void);
void RouteTypeDeal(RoadType_e NowRoute); ///路径消息处理
void SpecialDeal(void);
