#include "GeneralDeclare.h"
#include "TimerEventDriver.h"
#include "CommunicateDriver.h"
#include "SystemControl.h"
#include "Wifi.h"
#include "ProjectMath.h"
#include "DisPlayDeviceDriver.h"


#define KEY_IPD          0
#define KEY_DISCONN      1
#define KEY_CONN         2
#define KEY_CLOSED       3
#define KEY_OK           4
#define KEY_ERROR        5
#define KEY_BYTES        6
#define KEY_BUSY         7
#define KEY_SEND_OK      8
#define KEY_SEND_FAIL    9
#define KEY_BUFFER_FULL  10
#define KEY_QUEUE_FULL   11
#define KEY_WAIT_TIMEOUT 12
#define KEY_NO_CHANGE    13
#define KEY_LINK_N_VALID 14

extern volatile   uint32 SysTicks;
static     size_t RxBufferMark = 0;
volatile   size_t DMALoopCount = 0;    //在多个不同线程中访问同一个变量，必须用 volatile 修饰符
uint8 RecvBuff[BUFFSIZE + BUFFSIZE];

static size_t tempAvailable = 0;
static size_t UART_Available(void)
{
	size_t p = DMA_TCD1_CITER_ELINKNO;
	size_t n = DMALoopCount;
	tempAvailable = 2048 - p + n * 2048 - RxBufferMark;
	return tempAvailable;
}

// 从可读缓冲区接收n个字节
#define UART_Read(bytes)    {RxBufferMark += bytes;}
// 返回可读缓冲区内给定序号[i]对应的字节数据
#define UART_GetChar(i)     (*(RecvBuff + (RxBufferMark + i) % 2048))
// 返回给定数据范围[a,b)的"连续"可用数据空间的指针
const uint8 *UART_GetArray(size_t a, size_t b) //避免被修改，使用const
{
	if ((RxBufferMark + b) / 2048 > (RxBufferMark + a) / 2048)
	{
		memcpy(RecvBuff + 2048, RecvBuff, (RxBufferMark + b) % 2048);
	}
	return (RecvBuff + (RxBufferMark + a) % 2048);
}

static uint8_t RxKeyN[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


bool ESP8266_Init(void)
{
	Communicate_uart_init();
	CommonDelay(20);
	printf("+++");
	CommonDelay(20);
	Communicate_dma_init(RecvBuff);
	printf("\r\n");
	CommonDelay(100);
	UART_Read(UART_Available());
	uint32  ticks = SysTicks + 3000;
	do
	{
		UART_Read(UART_Available());
		printf("ATE0\r\n");//开关回显
		RxKeyN[KEY_OK] = 0;
		CommonDelay(100);
		RxMain();
		if (SysTicks > ticks)
		{
			UART_Read(UART_Available());//读取所有可用字节，清缓冲区。
			return false;
		}
		OLED_PrintString(0, 2, "Exit after", 1);
		OLED_PrintFloat(68, 2, (float)(3 - SysTicks / 1000), 1);
		OLED_PrintString(80, 2, "s", 1);

	} while (!RxKeyN[KEY_OK]);
	WriteATCommond("AT+RFPOWER=82\r\n");//功率(0~82)
	WriteATCommond("AT+CIPMUX=1\r\n");//多连接
	WriteATCommond("AT+CIPSERVER=1,50\r\n");//建立 server,段口号50
	WriteATCommond("AT+CIPSTO=180\r\n");//设置超时时间
	WriteATCommond("AT+CWMODE=3\r\n");
	SystemSettings.Wifi_Init = 'T';
	return true;
}


uint32 timeoutTimes = 0;
uint32 errorTimes = 0;
bool WriteATCommond(const char *str)
{
	uint32  ticks = SysTicks + 300;
	RxKeyN[KEY_OK] = 0;
	RxKeyN[KEY_ERROR] = 0;
	printf(str);
	while (!RxKeyN[KEY_OK] && !RxKeyN[KEY_ERROR])
	{
		if (SysTicks > ticks)
		{
			UART_Read(UART_Available());//读取所有可用字节，清缓冲区。
			timeoutTimes++;
			return false;
		}
		RxMain();
	}
	if (RxKeyN[KEY_OK])
		return true;
	if (RxKeyN[KEY_ERROR])
	{
		errorTimes++;
		return false;
	}
	return true;
}



static void RxParseLine(const uint8 *pLineStart, size_t len)
{
	// 判断该行内容
	const uint8 * pLineEnd = pLineStart + len;
	if (len == 2 && !memcmp(pLineStart, "OK", 2))
	{
		RxKeyN[KEY_OK]++;
	}
	else if (len == 5 && !memcmp(pLineStart, "ERROR", 5))
	{
		RxKeyN[KEY_ERROR]++;
	}
	else if (len >= 9 && !memcmp(pLineStart, "busy", 4))
	{
		RxKeyN[KEY_BUSY]++;
	}
	else if (len >= 12 && !memcmp(pLineStart, "Recv ", 5) && !memcmp(pLineEnd - 6, " bytes", 6))
	{
		RxKeyN[KEY_BYTES]++;
	}
	else if (len >= 11 && !memcmp(pLineEnd - 8, ",SEND OK", 8))
	{
		RxKeyN[KEY_SEND_OK]++;
	}
	else if (len >= 13 && !memcmp(pLineEnd - 10, ",SEND FAIL", 10))
	{
		RxKeyN[KEY_SEND_FAIL] ++;
	}
	else if (len == 9 && !memcmp(pLineStart, "no change", 9))
	{
		RxKeyN[KEY_NO_CHANGE] ++;
	}
	else if (len == 10 && !memcmp(pLineStart, "QUEUE FULL", 10))
	{
		RxKeyN[KEY_BUFFER_FULL] ++;
	}
	else if (len == 11 && !memcmp(pLineStart, "BUFFER FULL", 11))
	{
		RxKeyN[KEY_BUFFER_FULL] ++;
	}
	else if (len == 17 && !memcmp(pLineStart, "link is not valid", 17))
	{
		RxKeyN[KEY_LINK_N_VALID] ++;
	}
	else//接收到其他数据
	{

	}
}


///////IPD信息处理，上位机信息

uint32   WifiOrder = 0;
static void onDataRecvd(const uint8 *p, int len)
{
	static uint8    OrderCode[50] = { 0 };
	static uint32   DataCode[50] = { 0 };
	int PreLen = 0;
	uint8  OrderSite = 0;
	while ((PreLen + 5) <= len)
	{
		OrderCode[OrderSite] = *(p + PreLen);
		DataCode[OrderSite] = ((*(p + PreLen + 1) << 24) | (*(p + PreLen + 2) << 16) | (*(p + PreLen + 3) << 8) | (*(p + PreLen + 4)));
		PreLen += 5;
		OrderSite++;
	}
	uint8   i = 0;
	while (OrderSite--)
	{
		switch (OrderCode[i])
		{
		case 100:WifiOrder = DataCode[i]; break;
		case 110:pitchPIDcontrol.P = Uint32toFloat(DataCode[i]); break;
		case 111:pitchPIDcontrol.D = Uint32toFloat(DataCode[i]); break;

		case 120:yawPIDcontrol[0].P = Uint32toFloat(DataCode[i]); break;
		case 121:yawPIDcontrol[1].P = Uint32toFloat(DataCode[i]); break;
		case 122:yawPIDcontrol[0].I = Uint32toFloat(DataCode[i]); break;
		case 123:yawPIDcontrol[1].I = Uint32toFloat(DataCode[i]); break;
		case 124:yawPIDcontrol[0].D = Uint32toFloat(DataCode[i]); break;
		case 125:yawPIDcontrol[1].D = Uint32toFloat(DataCode[i]); break;

		default:break;
		}
		i++;
	}
	return;
}

static bool isIPD = false; // 是否正在接收+IPD数据

void RxMain(void)
{
	static bool isReadIPD = false;
	static int  IPDDataNum = 0;
	static uint8 IPDData[512];
	uint8 charnow = 0;
	static bool maybeIPD = true;    // 该行数据可能为+IPD
	static int32 parsedBytes = 0;   // 缓冲区中已分析过的数据字节数(数据值为已分析字节数+1)
	while (UART_Available() > parsedBytes)
	{
		if (isIPD)
		{
			if (!isReadIPD)
			{
				// 未读取IPD数据长度
				while (UART_Available() > parsedBytes)
				{
					if (UART_GetChar(parsedBytes) == ':')
					{
						// 已接受完成+IPD指令， 等待接收+IPD数据
						isReadIPD = true;
						UART_Read(parsedBytes + 1);
						parsedBytes = 0;
						break;
					}
					parsedBytes++;
				}
				continue;
			}
			else
			{
				// 接受到+IPD数据

				charnow = *UART_GetArray(0, 1);
				IPDData[IPDDataNum] = charnow;
				UART_Read(1);
				if (charnow == '#')
				{
					// 所有+IPD数据已接收
					IPDData[IPDDataNum] = '\0';
					isIPD = false;
					maybeIPD = true;
					isReadIPD = false;
					onDataRecvd(IPDData, IPDDataNum);
					IPDDataNum = 0;
				}
				else
				{
					IPDDataNum++;
					continue;
				}
			}
		}
		// 过滤无用字符， parsedBytes=0避免有效内容被过滤
		if (!parsedBytes) //已经分析字节数为0，表示已经分析完上一行
		{
			while (UART_Available())
			{
				switch (UART_GetChar(0))
				{
				case '>':
				case '\r':
				case '\n':
				case ' ':
					UART_Read(1);
					continue;
				}
				break;
			}
		}

		// 查找缓冲区是否包含\n或+IPD
		while (UART_Available() > parsedBytes)
		{
			uint8 ch = UART_GetChar(parsedBytes);
			if (maybeIPD)
			{
				if (parsedBytes >= 4)
				{
					isIPD = true; // 确认为+IPD
					parsedBytes++;
					break;
				}
				else if (ch != "+IPD,"[parsedBytes]) //当前分析的字符串如果是+IPD的其中一个字符，则认为可能是+IPD，
				{
					maybeIPD = false;//所有字符都不是+IPD其中一个，确认不是+IPD
				}
			}
			parsedBytes++;
			// 判断是否为\n
			if (ch == '\n')
			{
				// 获取连续的缓冲区内容
				// 分析缓冲区内容
				RxParseLine(UART_GetArray(0, parsedBytes - 2), parsedBytes - 2);  //parsedBytes - 2 找到\n之后
				// 读至\n后
				UART_Read(parsedBytes);
				parsedBytes = 0;
				maybeIPD = true;
				break;
			}
		}
	}
	return;
}


extern volatile uint8 IsSendPicOK;
extern uint8 SendPicBuff[600];
uint32 SendFloat[25] = { 0 };
void SendData(void)
{
	printf("#####");
	SendBuff((uint8 *)SendFloat, sizeof(SendFloat));
	printf("$$$$$");
	printf("@@@@@");
	printf("\r\n");
	IsSendPicOK = 'F';
}


uint32 TxdCnt = 0;

bool ESP8266_SendData(void)
{
	RxKeyN[KEY_SEND_OK] = 0;
	RxKeyN[KEY_BUFFER_FULL] = 0;

		if (WriteATCommond("AT+CIPSENDBUF=0,117\r\n"))
		{
			RxKeyN[KEY_BYTES] = 0;
			SendData();
			while (!RxKeyN[KEY_BYTES])
			{
				RxMain();
			}
			TxdCnt++;
			return true;
		}
		else
		{
			CommonDelay(1);
			return false;
		}
}
