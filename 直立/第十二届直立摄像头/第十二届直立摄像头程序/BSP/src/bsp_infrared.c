/*!
 *		湖北工程学院直立摄像头
 *
 * @file       bsp_infrared.c
 * @brief      红外解码
 * @version    v5.2
 * @author     湖北工程学院
 * @date       2017
 */
#include"bsp_infrared.h"

uint8 UserCode,UserData,KeyCode,KeyData;
uint8 ircode[4];
uint8 irkey_value=0;
uint8 Flag_ir = 0;

void infrared_init(void)
{
    port_init(IR_PIN, IRQ_FALLING |PF | ALT1 | PULLUP );
}

/*!
 *  @brief      获取高电平持续时间
 *  @since      ZROBOT
 */
uint16 GetHighTime(void)
{
    uint16 hightime;
    pit_time_start(PIT2);
    while(gpio_get(IR_PIN)!=0)  //如果获取的电平不为0  即为高电平
    {
        
    }
    hightime = pit_time_get_us(PIT2);
    
    return hightime;    
}

/*!
 *  @brief      获取低电平持续时间
 *  @since      ZROBOT
 */
uint16 GetLowTime(void)
{
    uint16 lowtime;
    pit_time_start(PIT2);
    while(gpio_get(IR_PIN)==0);  //如果获取的电平为0  即为低电平
    lowtime = pit_time_get_us(PIT2);
    
    return lowtime;    
}


void IR_Decoding(void)
{
    uint8 i,j;
    uint16 time;
    uint8 byt;
      
   time = GetLowTime();
   if((time<3500) || (time>5000))//4ms低电平
   {
         return ; 
       
   }
   
   time = GetHighTime();
   if((time<3500) || (time>5000))//4ms高电平
   {
        return;
   }
   gpio_set (BUZZ, 1);//提示遥控有效
   Flag_ir = 20;
   disable_irq(PORTC_IRQn);
   
   for(i=0; i<3; i++)
   {
        for(j=0; j<8; j++)
        {
            time = GetLowTime(); 
            time = GetHighTime();
            if((time>850)&&(time<1100))//940us
            {
                byt >>= 1;
            }
            else if((time>1200)&&(time<2300))//1.9ms
            {
                byt >>= 1; 
                byt |= 0x80;
            }
            else
            {
                return;
             
            }
        } 
        ircode[i] = byt;
        
   }
        UserCode = ircode[0];
        UserData = ircode[1]>>4;
		KeyCode = ircode[1]&0x0f;
        KeyData = ircode[2];
        
        CodeMapping();  //解码出来的数字对应键值
     
}

void CodeMapping(void)
{
             if(KeyData == 171)//  对应电源键
            {
            	Flag_Stop =! Flag_Stop; 
				car_ten_status=normal_ten;
            }                 
            if(KeyData == 3)//  对应静音
            {
				Flag_Buzz =! Flag_Buzz;
            }
            if(KeyData == 115)//  对应速度控制（1键）
            {   
				Flag_Speed =! Flag_Speed;
            } 
            if(KeyData == 179)//  对应方向控制（2键）
            {   
				Flag_Direction =! Flag_Direction;
            } 			
            if(KeyData ==70)//对应菜单键
            {
                debug_flag =~debug_flag;
            }          
            if(KeyData == 101)//对应上键
            {
               irkey_value = BT_UP_PRESS;
            }
            if(KeyData == 208)//对应OK键
            {
                irkey_value = BT_OK_PRESS;
            }
            if(KeyData == 149)//对应左键
            {
                 irkey_value = BT_LE_PRESS;
            }
            if(KeyData == 229)//对应下键
            {
                 irkey_value = BT_DN_PRESS;
            }
            if(KeyData == 21)//对应右键
            {
                 irkey_value = BT_RI_PRESS;
            }   
}

//获得红外按键
uint8 GetIRKey(void)
{
  return irkey_value; 
}

//设置红外按键
void SetIRKey(unsigned char key)
{
    irkey_value = key;
}
