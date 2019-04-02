#define FifoSize      20
#define KeyPORT       PORTA_BASE_PTR
#define KeyGPIO       PTA_BASE_PTR
#define GPIO_get(n)   ((KeyGPIO->PDIR >> n) & 0x01)
#define BUZZPORT      PORTD_BASE_PTR
#define BUZZGPIO      PTD_BASE_PTR
#define BUZZNum       10
#define BUZZUP        (BUZZGPIO->PDOR |= (1 << BUZZNum))
#define BUZZDOWN      (BUZZGPIO->PDOR &= ~(1 << BUZZNum))

typedef enum
{
    Key_U,  //上
    Key_D,  //下

    Key_Y,  //确认
    Key_N,  //取消

    Key_MAX,
} Key_e;


typedef struct
{
    Key_e key;        //按键编号
    uint8 status;
} Key_msg_t;


void key_init(void);
uint8 key_check(Key_e key);
uint8 key_get(Key_e key);
void key_scan(void);
uint8 Get_KeyFifo(Key_msg_t *keymsg);
void BUZZSystem(void);