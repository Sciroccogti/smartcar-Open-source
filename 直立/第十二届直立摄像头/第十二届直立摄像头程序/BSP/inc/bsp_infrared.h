#ifndef _bsp_infrared_
#define _bsp_infrared_

#include "common.h"
#include "include.h"
/**********************************************/

#define  IR_PIN    PTC4

//°´¼üID¶¨Òå
#define BT_UP_PRESS  (1)
#define BT_DN_PRESS  (2)
#define BT_LE_PRESS  (3)
#define BT_RI_PRESS  (4)
#define BT_OK_PRESS  (5)

extern uint8 UserCode,UserData,KeyCode,KeyData;
extern uint8 irkey_value;
extern uint8 debug_flag;
extern uint8 Flag_ir;

uint16 GetHighTime(void);
uint16 GetLowTime(void);
extern void infrared_init(void);
extern void IR_Decoding(void);

void CodeMapping(void);
extern uint8 GetIRKey(void);
extern void SetIRKey(unsigned char key);


#endif  /*_bsp_infrared_*/