#ifndef _bsp_macro_
#define _bsp_macro_

#include "common.h"
#include "include.h"
/**********************************************/

/********************小车参数结构体******************/
struct PARAM
{
  int16 int01;
  int16 int02;
  int16 int03;
  int16 int04;
  int16 int05;
  int16 int06;
  int16 int07;
  int16 int08;
  int16 int09;
  int16 int10;
  int16 int11;
  int16 int12;
  
  int16 float01;
  int16 float02;
  int16 float03;
  int16 float04;
  int16 float05;
  int16 float06;
  int16 float07;
  int16 float08;
  int16 float09;
  int16 float10;
  int16 float11;
  int16 float12;
 
  int16 param_flag;          
};


/**********************摄像头有关**********************/

/*******************图像有关****************************/
#define FAR_LINE  (14)

#define ALL        (1)
#define TRACK      (0)
#define NEAR_FIVE  (2)

#define Black      (0)
#define White      (1)


#define GET      (0x01u)
#define LOST     (0x02u)



#endif  /*_bsp_macro_*/