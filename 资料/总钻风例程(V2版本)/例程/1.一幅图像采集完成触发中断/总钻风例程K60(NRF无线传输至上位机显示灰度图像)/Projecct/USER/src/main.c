/********************************************
逐飞科技 总钻风-摄像头  历程
Designed by Fly Sir
软件版本:V1.1
最后更新:2016年5月3日
相关信息参考下列地址：
淘宝店：https://seekfree.taobao.com/
------------------------------------
软件版本： IAR 7.2 or MDK 5.17
目标核心： MK60DN512VLL10
============================================
MT9V032接线定义：
------------------------------------ 
	模块管脚            单片机管脚
    SDA(51的RX)         C17
    SCL(51的TX)         C16
	场中断              C6
    行中断				未使用，因此不接
    像素中断            C18            
    数据口              C8-C15 

    串口  
    波特率 115200 
    数据位 8 
    校验位 无
    停止位 1位
    流控   无
	串口连接注意事项：切勿使用蓝牙等无线串口连接
    RX                  D3
    TX                  D2

    NRF接线请查看SEEKFREE_NRF24L01.h文件最上方备注
    
============================================

分辨率是                188*120
摄像头参数设置可以到    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h

总钻风-摄像头测试步骤：
1.下载程序到开发板
2.插上串口线或者USB转TTL
3.接好MT9V032模块接线
4.通电在TFT液晶上即可观看    
*********************************************/  
#include "headfile.h"


void nrf_send_array(uint8 *dat, uint16 length)
{
    uint16 i;
    uint16 send_num;
    
    length += 4;//加上4个字节的帧头
    //计算需本次要发送的数据个数，每次发送的数据不能超过31个。
    if(31 < length)   
    {
        send_num = 31;
        length -= 31;
    }
    else
    {
        send_num = length;
        length = 0;
    }
    NRF_TX_Buff[0] = send_num;
    NRF_TX_Buff[1] = 0X00;
    NRF_TX_Buff[2] = 0XFF;
    NRF_TX_Buff[3] = 0X01;
    NRF_TX_Buff[4] = 0X01;
    for(i=5; i<32; i++)
    {
        NRF_TX_Buff[i] = *dat;
        dat++;
    }
    NRF_Send_Packet(NRF_TX_Buff);
    systick_delay_ms(2);

    while(length)
    {
        //计算需本次要发送的数据个数，每次发送的数据不能超过31个。
        if(31 < length)   
        {
            send_num = 31;
            length -= 31;
        }
        else
        {
            send_num = length;
            length = 0;
        }
        
        NRF_TX_Buff[0] = send_num;
        for(i=1; i<32; i++)
        {
            NRF_TX_Buff[i] = *dat;
            dat++;
        }
        
        NRF_Send_Packet(NRF_TX_Buff);
        systick_delay_ms(2);
    }
}


int main(void)
{
    get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
    uart_init (uart2, 115200);                          //初始换串口与电脑通信
    camera_init();
	NRF_Dev_Init();
    for(;;)
	{

        if(mt9v032_finish_flag)
        {
            nrf_send_array(image[0],COL*ROW);
            mt9v032_finish_flag = 0;
        }
	}
}

