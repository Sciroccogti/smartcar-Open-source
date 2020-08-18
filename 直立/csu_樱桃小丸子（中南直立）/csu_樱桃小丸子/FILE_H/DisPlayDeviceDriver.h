#define OLED_port         PORTE_BASE_PTR
#define OLED_GPIO_port    PTE_BASE_PTR
#define OLED_SDA_pin      27
#define OLED_SCL_pin      26
#define OLED_DC_pin       25
#define OLED_RST_pin      24

#define OLED_SDA_H        (OLED_GPIO_port->PDOR |= (1 << OLED_SDA_pin))
#define OLED_SDA_L        (OLED_GPIO_port->PDOR &=~ (1 << OLED_SDA_pin))
#define OLED_SCL_H        (OLED_GPIO_port->PDOR |= (1 << OLED_SCL_pin))
#define OLED_SCL_L        (OLED_GPIO_port->PDOR &=~ (1 << OLED_SCL_pin))
#define OLED_DC_H         (OLED_GPIO_port->PDOR |= (1 << OLED_DC_pin))
#define OLED_DC_L         (OLED_GPIO_port->PDOR &=~ (1 << OLED_DC_pin))

#define OLED_W            128
#define OLED_H            64

void OLED_init(void);
void OLED_CLS(void);
void OLED_PrintChar(uint8 x, uint8 y, uint8 data);
void OLED_PrintBat(uint8 x, uint8 y, float data);
void OLED_PrintImg(uint8 *pucTable, uint16 usRowNum, uint16 usColumnNum);
void OLED_PrintString(uint8 x, uint8 y, uint8 *str, uint8 type);
void OLED_PrintInt(uint8 x, uint8 y, int data, uint8 num, uint8 type);
void OLED_PrintFloat(uint8 x, uint8 y, float data, uint8 num);
void OLED_PrintVar(uint8 x, uint8 y, double data, uint8 Length, uint8 num);
void OLED_PrintStr(uint8 x, uint8 y, uint8 *str, uint8 Length, uint8 type);
