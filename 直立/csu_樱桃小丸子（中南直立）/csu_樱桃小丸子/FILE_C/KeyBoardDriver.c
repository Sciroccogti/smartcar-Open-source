#include "GeneralDeclare.h"
#include "KeyBoardDriver.h"
#include "TimerEventDriver.h"



uint8 KeyBox[Key_MAX] = {14, 17, 15, 16};
Key_msg_t  key_msg[FifoSize];             //按键消息FIFO
volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //接收FIFO的指针
volatile uint8      key_msg_flag = 0;           //按键消息FIFO状态


volatile uint8 IsStart = 'F';
void BUZZSystem(void)
{
  /*if(((KeyGPIO->PDIR >> 14) & 0x01) == 0 ||
     ((KeyGPIO->PDIR >> 15) & 0x01) == 0 ||
     ((KeyGPIO->PDIR >> 16) & 0x01) == 0 ||
     ((KeyGPIO->PDIR >> 17) & 0x01) == 0)
  {
   BUZZUP;
  }
  else
    BUZZDOWN;
  */
  if(0 == ((PTA_BASE_PTR->PDIR >> 19) & 1))
  {
    while(0 == ((PTA_BASE_PTR->PDIR >> 19) & 1));
      if(IsStart == 'F')
        IsStart = 'T';
  }
}

void Start_ISR_Handler(void)
{
  if(PORTA_BASE_PTR->ISFR &  (1 << 19))
  {
    PORTA_BASE_PTR->ISFR |= 1 << 19;
    if(0 == ((PTA_BASE_PTR->PDIR >> 19) & 1))
    {
      while(0 == ((PTA_BASE_PTR->PDIR >> 19) & 1));
      if(IsStart == 'F')
        IsStart = 'T';
    }
  }
}

void key_init(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; 
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; 
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    for(uint8 i = 14; i < 18; i++)
    {
       KeyPORT->PCR[i] = PORT_PCR_MUX(1);
       KeyGPIO->PDDR &= ~(1 << i);
    }
    PORTC_BASE_PTR->PCR[4] = PORT_PCR_MUX(1);
    PTC_BASE_PTR->PDDR |= (1 << 4);
    PORTC_BASE_PTR->PCR[5] = PORT_PCR_MUX(1);
    PTC_BASE_PTR->PDDR |= (1 << 5);
    PORTC_BASE_PTR->PCR[6] = PORT_PCR_MUX(1);
    PTC_BASE_PTR->PDDR |= (1 << 6);

    PORTA_BASE_PTR->PCR[19] = PORT_PCR_MUX(1);
    PTA_BASE_PTR->PDDR &= ~(1 << 19); 
    
    BUZZPORT->PCR[BUZZNum] = PORT_PCR_MUX(1);
    BUZZGPIO->PDDR |= (1 << BUZZNum);
    return;
}

uint8 key_get(Key_e key)
{
    if(GPIO_get(KeyBox[key]) == 0)
    {
        return 0;
    }
    return 1;
}

uint8 key_check(Key_e key)
{
    if(key_get(key) == 0)
    {
        CommonDelay(5);
        if( key_get(key) == 0)
        {
            return 0;
        }
    }
    return 1;
}

void SendToFifo(Key_msg_t keymsg)
{
    uint8 temp = 0;
    if(key_msg_flag == 1)
    {
        return ;
    }
    
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= FifoSize)
    {
        key_msg_rear = 0;                       //重头开始
    }

    temp = key_msg_rear;
    if(temp == key_msg_front)                   //追到屁股了，满了
    {
        key_msg_flag = 1;
    }
    else
    {
        key_msg_flag = 2;
    }
}

uint8 Get_KeyFifo(Key_msg_t *keymsg)
{
    uint8 temp;

    if(key_msg_flag == 0)               //按键消息FIFO为空，直接返回0
    {
        return 0;
    }

    keymsg->key = key_msg[key_msg_front].key;       //从FIFO队首中获取按键值
    keymsg->status = key_msg[key_msg_front].status; //从FIFO队首中获取按键类型

    key_msg_front++;                                //FIFO队首指针加1，指向下一个消息

    if(key_msg_front >= FifoSize)          //FIFO指针队首溢出则从0开始计数
    {
        key_msg_front = 0;                          //重头开始计数（循环利用数组）
    }

    temp = key_msg_rear;
    if(key_msg_front == temp)                        //比较队首和队尾是否一样，一样则表示FIFO已空了
    {
        key_msg_flag = 0;
    }
    else
    {
        key_msg_flag = 2;
    }

    return 1;
}

void key_scan(void)
{
  Key_e num;
  uint8 status = 0;
  Key_msg_t keymsg;
  static uint8 keytime[Key_MAX];
  BUZZSystem();
  for(num = (Key_e)0; num < Key_MAX; num++)   //逐个扫描
  {
    status = key_get(num);
    if(status == 0)        //状态为按下
    {
      keytime[num]++;
      if(keytime[num] < 2)     //消抖时间
      {
        continue;      //未到消抖时间继续扫描
      }
      else if(keytime[num] == 2)   //达到响应时间
      {
         keymsg.key = num;
         keymsg.status = 0;
         SendToFifo(keymsg);
      }
      else if(keytime[num] < 40)
      {
        continue;
      }
      else if(keytime[num] >= 40)
      {
         keymsg.key = num;
         keymsg.status = 1;        
         SendToFifo(keymsg);
         keytime[num] = 0;//计时清零
      }
    }
    else
    {
      if(keytime[num] >= 2)    //是否按下过该按键
      {
        keymsg.key = num;
        keymsg.status = 2;        
        SendToFifo(keymsg);
        keytime[num] = 0;//计时清零
      }
    }
  }
}