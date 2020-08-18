/******************************************************/
/*Author  :    中南大学黄竞辉                           */
/*Date    ：      2016/12/12                             */
/*Describe：      OLED 0.96'驱动                      */
/*Version ：      1.0                                     */
/******************************************************/

#include "GeneralDeclare.h"
#include "DisPlayDeviceDriver.h"
#define   BATFull      8.0f
#define   BATNone      7.0f

//  ASCLL字符表
const uint8 F6x8[][6] = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
		{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
		{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
		{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
		{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
		{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
		{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
		{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
		{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
		{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
		{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
		{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
		{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
		{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
		{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
		{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
		{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
		{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
		{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
		{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
		{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
		{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
		{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
		{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
		{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
		{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
		{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
		{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
		{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
		{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
		{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
		{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
		{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
		{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
		{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
		{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
		{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
		{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
		{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
		{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
		{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
		{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
		{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
		{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
		{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
		{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
		{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
		{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
		{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
		{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
		{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
		{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
		{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
		{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
		{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
		{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
		{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
		{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
		{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
		{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
		{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
		{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
		{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
		{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
		{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
		{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
		{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
		{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
		{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
		{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
		{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
		{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
		{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
		{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
		{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
		{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
		{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
		{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
		{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
		{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
		{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
		{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
		{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
		{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
		{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
		{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
		{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
		{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
		{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
		{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
		{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
		{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

const uint8 BatPic[2][36] = {
{0xF8,0x08,0xFF,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0xFF},
{0x1F,0x10,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x80,0x80,0x80,0xFF}
};/*"D:\Documents\电池.BMP",0*/

static void OLED_Delay(uint16 ms) {
	uint16 a;
	while (ms) {
		a = 13350;
		while (a--)
			;
		ms--;
	}

	return;
}

static void OLED_WriteData(uint8 data, uint8 type) {
	uint8 i = 8;
	if (!type) {
		data = ~data;
	}
	OLED_DC_H;
	asm("nop");
	OLED_SCL_L;
	asm("nop");
	while (i--) {
		if (data & 0x80) {
			OLED_SDA_H;
		} else {
			OLED_SDA_L;
		}
		OLED_SCL_H;
                asm("nop");
		OLED_SCL_L;
		data <<= 1;
	}
}

static void OLED_WriteCmd(uint8 cmd) 
{
	uint8 i = 8;
	OLED_DC_L;
	OLED_SCL_L;
	while (i--) 
        {
		if (cmd & 0x80) 
                {
			OLED_SDA_H;
		} 
                else 
                {
			OLED_SDA_L;
		}
		OLED_SCL_H;
                asm("nop");
		OLED_SCL_L;
		cmd <<= 1;
	}
}

static void OLED_Set_Pos(uint8 x, uint8 y) 
{
	OLED_WriteCmd((0xb0 + y));
	OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WriteCmd((x & 0x0f) | 0x01);
}

static void OLED_Fill(uint8 data) 
{
	uint8 y, x;

	for (y = 0; y < 8; y++) 
        {
		OLED_WriteCmd(0xb0 + y);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10);
		for (x = 0; x < OLED_W; x++)
			OLED_WriteData(data, 1);
	}
}

void OLED_CLS(void) 
{
	OLED_Fill(0);
	return;
}

void OLED_init(void) 
{
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	OLED_port->ISFR = (1 << OLED_SDA_pin) | (1 << OLED_SCL_pin)
			| (1 << OLED_DC_pin) | (1 << OLED_RST_pin);
	OLED_port->PCR[OLED_SDA_pin] = PORT_PCR_MUX(1);
	OLED_port->PCR[OLED_SCL_pin] = PORT_PCR_MUX(1);
	OLED_port->PCR[OLED_DC_pin] = PORT_PCR_MUX(1);
	OLED_port->PCR[OLED_RST_pin] = PORT_PCR_MUX(1);
	OLED_GPIO_port->PDDR |= (1 << OLED_SDA_pin) | (1 << OLED_SCL_pin)
			| (1 << OLED_DC_pin) | (1 << OLED_RST_pin);
	OLED_SCL_H;
	OLED_GPIO_port->PDOR &= ~(1 << OLED_RST_pin);    //RST拉低
	OLED_Delay(200);
	OLED_GPIO_port->PDOR |= (1 << OLED_RST_pin);

	OLED_WriteCmd(0xae);    //--turn off oled panel
	OLED_WriteCmd(0x00);    //---set low column address
	OLED_WriteCmd(0x10);    //---set high column address
	OLED_WriteCmd(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCmd(0x81);    //--set contrast control register
	OLED_WriteCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WriteCmd(0xa1); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteCmd(0xc8); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteCmd(0xa6); //--set normal display
	OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3f); //--1/64 duty
	OLED_WriteCmd(0xd3); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCmd(0x00); //-not offset
	OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCmd(0xd9); //--set pre-charge period
	OLED_WriteCmd(0xf1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteCmd(0xda); //--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); //--set vcomh
	OLED_WriteCmd(0x40); //Set VCOM Deselect Level
	OLED_WriteCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCmd(0x02); //
	OLED_WriteCmd(0x8d); //--set Charge Pump enable/disable
	OLED_WriteCmd(0x14); //--set(0x10) disable
	OLED_WriteCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
	OLED_WriteCmd(0xa7); // Disable Inverse Display On (0xa6/a7)
	OLED_WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
	OLED_Set_Pos(0, 0);
}

/////电池电压显示
void OLED_PrintBat(uint8 x, uint8 y, float data)
{
  uint8 i = 0, j = 0;
  if(data >= BATFull) j =  2;
  else if(data <= BATNone) j =  35;
  else
    j = (uint8)((BATFull - data) * 33) + 2;
  
  OLED_Set_Pos(x, y);
  for(i = 0; i < 36; i++)
  {
    if(i >= j)
       OLED_WriteData((BatPic[0][i] | 0xff), 1);
    else
       OLED_WriteData(BatPic[0][i], 1);
  }
  OLED_Set_Pos(x, y + 1);
  for(i = 0; i < 36; i++)
  {
    if(i >= j)
       OLED_WriteData((BatPic[1][i] | 0xff), 1);
    else
       OLED_WriteData(BatPic[1][i], 1);
  }
  

}

void OLED_PrintChar(uint8 x, uint8 y, uint8 data)  //显示单个字符
{
	uint8 c = 0, i = 0;

	c = data - 32;
	if (x > 122) {
		x = 0;
		y++;
	}
	OLED_Set_Pos(x, y);
	for (i = 0; i < 6; i++) {
		OLED_WriteData(F6x8[c][i], 1);
	}
}

void OLED_PrintImg(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum) 
{
	uint8 ucData;
	uint16 i, j, k, m, n;
	uint16 usRowTmp;

	m = usRowNum >> 3;   //计算图片行数以8位为一组完整的组数
	n = usRowNum % 8;    //计算分完组后剩下的行数

	for (i = 0; i < m; i++) //完整组行扫描
			{
		OLED_Set_Pos(0, (uint8) i);
		usRowTmp = i << 3;    //计算当前所在行的下标
		for (j = 0; j < usColumnNum; j++) //列扫描
				{
			ucData = 0;
			for (k = 0; k < 8; k++) //在i组中对这8行扫描
					{
				ucData = ucData >> 1;
				if ((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0x00) {
					ucData = ucData | 0x80;
				}

			}
			OLED_WriteData(ucData, 0);
		}
	}

	OLED_Set_Pos(0, (uint8) i); //设置剩下的行显示的起始坐标
	usRowTmp = i << 3;       //计算当前所在行的下标
	for (j = 0; j < usColumnNum; j++) //列扫描
			{
		ucData = 0;
		for (k = 0; k < n; k++) //对剩下的行扫描
				{
			ucData = ucData >> 1;
			if ((pucTable + (usRowTmp + k) * usColumnNum)[j] == 0x00) {
				ucData = ucData | 0x80;
			}

		}
		ucData = ucData >> (8 - n);
		OLED_WriteData(ucData, 0);
	}

	return;
}

void OLED_PrintString(uint8 x, uint8 y, uint8 *str, uint8 type)
{
  uint8 c=0,i=0,j=0,num = 0;
  while(str[j]!='\0')
  {    
    c = str[j]-32;
    if(x>122){x=0;y++;}
    OLED_Set_Pos(x,y);    
    for(i=0;i<6;i++)
    {
       if(!type)
       OLED_WriteData(F6x8[c][i],0);
       else
       OLED_WriteData(F6x8[c][i],1);
    }
    x+=6;
    j++;
    num++;
  }
}

void OLED_PrintStr(uint8 x, uint8 y, uint8 *str, uint8 Length, uint8 type)
{
  uint8 c = 0, i = 0, j = 0, num = 0;
  while((str[j] != '\0') || (num < Length))
  {
    if(str[j] != '\0')
     c = str[j] - 32;
    else
     c = ' ' - 32;
    if(x > 122)
    {
      x = 0; 
      y++;
    }
    OLED_Set_Pos(x,y);    
    for(i = 0; i < 6; i++)
    {
       if(!type)
       OLED_WriteData(F6x8[c][i],0);
       else
       OLED_WriteData(F6x8[c][i],1);
    }
    x += 6;
    if(str[j] != '\0')
    j++;
    num++;
  }
}


void OLED_PrintVar(uint8 x, uint8 y, double data, uint8 Length, uint8 num)
{
  uint8 StrTemp[20] = {0};
  uint8 Site = 0;
  int bit = 0;
  int32 datatemp = 0;
  
  
  while(Site < num)
  {
    data *= 10;
    Site++;
  }
  datatemp = (int32)data;
  
  Site = 0;
  if(datatemp < 0)
  {
    StrTemp[Site] = '-';
    datatemp = -datatemp;
    Site++;   
  }
  else if(datatemp == 0)
  {
    StrTemp[Site] = '0';
    Site++;
  }
  
  bit = 0;
  int32 d = datatemp;
  while(d > 0)
  {
    d /= 10;
    bit++;                   /////取得显示的数字个数
  }
  
  uint8 dotsite = 0; ////小数点位置
  if(bit <= num)
  {
    StrTemp[Site] = '0';
    Site++;
    dotsite = 0;
    bit = num;
  }
  else
  {
    dotsite = bit - num;
   // bit;
  }
  
  StrTemp[Site + bit + 1] = '\0';
  while(bit >= 0)
  {
    if(bit == dotsite)
    {
      StrTemp[Site + bit] = '.';
    }
    else
    {
      StrTemp[Site + bit] = datatemp % 10 + '0';
      datatemp /= 10;
    }
    bit--;
  }
  OLED_PrintStr(x, y, StrTemp, Length, 1);
}

void OLED_PrintInt(uint8 x, uint8 y, int data, uint8 num, uint8 type)  //num显示数位 type是否有符号1为有符号0为无符号
{
 	uint8 l =0, m =0, i = 0, j = 0, k = 0;
        if(type)
        {
          if(data < 0)
	  {
		OLED_PrintChar(x,y,'-');
		data = - data;	
          }
	  else
	  {
		OLED_PrintChar(x,y,'+');
	  } 
        }
        else
 	  OLED_PrintChar(x, y, '.');
        
	l  = data/10000;        
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	OLED_PrintChar(x+6,y,k+48);
				break;
		case 2:  	OLED_PrintChar(x+6,y,j+48);
				OLED_PrintChar(x+12,y,k+48);
				break;
		case 3:	        OLED_PrintChar(x+6,y,i+48);
				OLED_PrintChar(x+12,y,j+48);
				OLED_PrintChar(x+18,y,k+48);
				break;
		case 4: 	OLED_PrintChar(x+6,y,m+48);
				OLED_PrintChar(x+12,y,i+48);
				OLED_PrintChar(x+18,y,j+48);
				OLED_PrintChar(x+24,y,k+48);
				break;
                case 5:         OLED_PrintChar(x+6,y,l+48);
                                OLED_PrintChar(x+12,y,m+48);
				OLED_PrintChar(x+18,y,i+48);
				OLED_PrintChar(x+24,y,j+48);
				OLED_PrintChar(x+30,y,k+48);
				break;
	}
}

void OLED_PrintFloat(uint8 x, uint8 y, float data, uint8 num)
 {
 	uint8 l,m,i,j,k;  //万千百十个
 	uint8 databiti = 6; //整数位数
 	int tempdataui = 0;
  	int tempdataii = (int)data; //整数部分
 	long int tempdatalp = (long int)((data - (int)data)*10000); //取小数位后4位
 	
 	//整数部分显示
 	if(data < 0.0000001)  OLED_PrintChar(x, y,'-'); 
 	else OLED_PrintChar(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //去掉整数部分负号
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //五位
 	{
 		OLED_PrintChar(x+6,y,l+48);
 		OLED_PrintChar(x+12,y,m+48);
		OLED_PrintChar(x+18,y,i+48);
		OLED_PrintChar(x+24,y,j+48);
		OLED_PrintChar(x+30,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		OLED_PrintChar(x+6,y,m+48);
 		OLED_PrintChar(x+12,y,i+48);
		OLED_PrintChar(x+18,y,j+48);
		OLED_PrintChar(x+24,y,k+48);
 	}
  	else if(i != 0) //三位
  	{
  		databiti = 4;
  	 	OLED_PrintChar(x+6,y,i+48);
 		OLED_PrintChar(x+12,y,j+48);
		OLED_PrintChar(x+18,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    		databiti = 3;	
  		OLED_PrintChar(x+6,y,j+48);
 		OLED_PrintChar(x+12,y,k+48);
  	}
	else 	
	{
		databiti = 2;
		OLED_PrintChar(x+6,y,k+48);
	}	
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//去掉小数部分负号
	switch(num)
	{
		case 0: break;
		case 1:  OLED_PrintInt(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000),num,0);break;
		case 2:  OLED_PrintInt(x + databiti * 6, y, (unsigned int)(tempdatalp / 100),num,0);break;
		case 3:  OLED_PrintInt(x + databiti * 6, y, (unsigned int)(tempdatalp / 10),num,0);break;
		case 4:  OLED_PrintInt(x + databiti * 6, y, (unsigned int)(tempdatalp),num,0);break;					
	}
 }