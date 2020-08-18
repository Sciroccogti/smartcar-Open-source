#ifndef PROJECTMATH_H
#define PROJECTMATH_H


void    UnZip(uint8 *dst, uint8 *src, uint32 srclen);
float   ABSF(float data);
int     ABS(int data);
int     ABSDet(int datai, int dataj);
float   Uint32toFloat(uint32 value);
uint32   FloattoUint32(float value);
void    Data_Convert_Float(void);
void    Data_Convert_Uint32(void);
void    Default_Settings(void);

#endif
