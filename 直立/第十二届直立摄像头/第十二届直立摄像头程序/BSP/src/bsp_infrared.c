/*!
 *		��������ѧԺֱ������ͷ
 *
 * @file       bsp_infrared.c
 * @brief      �������
 * @version    v5.2
 * @author     ��������ѧԺ
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
 *  @brief      ��ȡ�ߵ�ƽ����ʱ��
 *  @since      ZROBOT
 */
uint16 GetHighTime(void)
{
    uint16 hightime;
    pit_time_start(PIT2);
    while(gpio_get(IR_PIN)!=0)  //�����ȡ�ĵ�ƽ��Ϊ0  ��Ϊ�ߵ�ƽ
    {
        
    }
    hightime = pit_time_get_us(PIT2);
    
    return hightime;    
}

/*!
 *  @brief      ��ȡ�͵�ƽ����ʱ��
 *  @since      ZROBOT
 */
uint16 GetLowTime(void)
{
    uint16 lowtime;
    pit_time_start(PIT2);
    while(gpio_get(IR_PIN)==0);  //�����ȡ�ĵ�ƽΪ0  ��Ϊ�͵�ƽ
    lowtime = pit_time_get_us(PIT2);
    
    return lowtime;    
}


void IR_Decoding(void)
{
    uint8 i,j;
    uint16 time;
    uint8 byt;
      
   time = GetLowTime();
   if((time<3500) || (time>5000))//4ms�͵�ƽ
   {
         return ; 
       
   }
   
   time = GetHighTime();
   if((time<3500) || (time>5000))//4ms�ߵ�ƽ
   {
        return;
   }
   gpio_set (BUZZ, 1);//��ʾң����Ч
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
        
        CodeMapping();  //������������ֶ�Ӧ��ֵ
     
}

void CodeMapping(void)
{
             if(KeyData == 171)//  ��Ӧ��Դ��
            {
            	Flag_Stop =! Flag_Stop; 
				car_ten_status=normal_ten;
            }                 
            if(KeyData == 3)//  ��Ӧ����
            {
				Flag_Buzz =! Flag_Buzz;
            }
            if(KeyData == 115)//  ��Ӧ�ٶȿ��ƣ�1����
            {   
				Flag_Speed =! Flag_Speed;
            } 
            if(KeyData == 179)//  ��Ӧ������ƣ�2����
            {   
				Flag_Direction =! Flag_Direction;
            } 			
            if(KeyData ==70)//��Ӧ�˵���
            {
                debug_flag =~debug_flag;
            }          
            if(KeyData == 101)//��Ӧ�ϼ�
            {
               irkey_value = BT_UP_PRESS;
            }
            if(KeyData == 208)//��ӦOK��
            {
                irkey_value = BT_OK_PRESS;
            }
            if(KeyData == 149)//��Ӧ���
            {
                 irkey_value = BT_LE_PRESS;
            }
            if(KeyData == 229)//��Ӧ�¼�
            {
                 irkey_value = BT_DN_PRESS;
            }
            if(KeyData == 21)//��Ӧ�Ҽ�
            {
                 irkey_value = BT_RI_PRESS;
            }   
}

//��ú��ⰴ��
uint8 GetIRKey(void)
{
  return irkey_value; 
}

//���ú��ⰴ��
void SetIRKey(unsigned char key)
{
    irkey_value = key;
}
