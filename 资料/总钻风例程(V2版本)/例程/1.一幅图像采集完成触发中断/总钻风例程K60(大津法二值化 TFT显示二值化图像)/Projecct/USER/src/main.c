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

#include "math.h"


uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分  
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分  
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }

    return threshold;
}

uint8 image_threshold;  //图像阈值
uint32 use_time;        
int main(void)
{
    get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
    uart_init (uart2, 512000);                          //初始换串口与电脑通信
    camera_init();
	lcd_init();
	
    for(;;)
	{
		if(mt9v032_finish_flag)
        {
            mt9v032_finish_flag = 0;
			
            pit_time_start(pit1);
            image_threshold = otsuThreshold(image[0],COL,ROW);  //大津法计算阈值
            use_time = pit_time_get(pit1)/bus_clk_mhz;          //计算大津法程序消耗时间，单位微秒。
			//本例程大津法时间绝对很长，想直接用必然不可行，需自行优化。
			//有人说：你咋不直接优化好呢。我说：你想得美，要不要我直接把车做好给你啊。
            pit_close(pit1);

            //displayimage032(image[0]);                          //显示灰度图像
            displayimage032_bin(image[0],image_threshold);      //显示二值化后图像
            
            //发送二值化图像至上位机
//            p = image[0];
//            uart_putchar(uart2,0x00);uart_putchar(uart2,0xff);uart_putchar(uart2,0x01);uart_putchar(uart2,0x01);//发送命令
//            for(i=0; i<COL*ROW; i++)
//            {
//                if(p[i]>image_threshold)    uart_putchar(uart2,0xff);
//                else                        uart_putchar(uart2,0x00);
//            }
            //seekfree_sendimg_032();                             //发送灰度至上位机
        }
	}
}

