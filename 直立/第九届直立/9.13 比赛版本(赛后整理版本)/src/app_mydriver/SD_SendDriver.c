#include"include.h"
/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��OwnSD_Driver.c
 * ����         ������ģ���ʼ������
 * ʵ��ƽ̨     ������ӡ�󿪷���
 * ��汾       ������Ұ���
 * Ƕ��ϵͳ     ��
 * ����         ��xuxu
**********************************************************************************/ 


FIL fdst;  //�ļ�
FATFS fs;  //�ļ�ϵͳ
UINT bw;
unsigned char Send_Img[81];
unsigned int add=0;
static unsigned char Sd_writeOnce=0;
unsigned char SD_enable=0;
static unsigned int flash_write_count=0;
#define write_count_max 2240  //2240/75=29.8s 75Ϊ1s֡�� 


/************************************************
*  �������ƣ�SDcard_Init
*  ����˵����SD����ʼ����ʹ��K60�Դ���SDģ�飩
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SDcard_Init()
{
    FIL fdst;  //�ļ�
    FATFS fs;  //�ļ�ϵͳ
    int res;
    char *str = "��л��ѡ�� Ұ��Kinetis������ ��^_^ ";
    f_mount(0, &fs);	//SD��ϵͳ��ʼ��
    res = f_open(&fdst, "\\FireDemo.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //���ļ������û�оʹ���������д��
    if( res == FR_DISK_ERR)
    {
        printf( "\nû����SD������\n" );
    }
    else if ( res == FR_OK )
    {
        printf( "\n�ļ��򿪳ɹ� \n" );
    }
    else
    {
        printf("\n����ֵ�쳣");
    }
    f_puts(str, &fdst);
    f_close(&fdst);//�ر��ļ� 
}



   
   
/*************************************************************************
*  �������ƣ�SD_Writedata
*  ����˵����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
void SD_Writedata()
{
    //�浥��ͼ��
    static int SECTOR_NUM=0;
    static unsigned int H,L,write_data = 0;
    static unsigned char data8; 
    static unsigned int data32;
    static short int wH,wL;
    
    //������һ��ͼ��
    /*if(Sd_writeOnce==0)//write flash
    {
        LCD_PutString(0,60,"SD start write!!!",Red,Green);
        //ͣ��
        Sd_writeOnce = 1;
        //����
        f_mount(0, &fs);	//SD��ϵͳ��ʼ��
        f_open(&fdst, "\\data.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //���ļ������û�оʹ���������д��
    }
    if(Sd_writeOnce == 1)
    {
        Sd_writeOnce = 2;
        for(int j=0;j<240;j++)
        {
            for(int i=0,ii=0;i<40;i++)
            {
                Send_Img[ii++] = ((((unsigned int)(Image_fire[j][i]))&0xf0)>>4) + 'a';
                Send_Img[ii++] = (((unsigned int)(Image_fire[j][i]))&0x0f) + 'a';
            }
            Send_Img[80]='\n';
            f_write(&fdst,Send_Img,81,&bw);
        }
        LCD_PutString(0,60,"SD write finish!!!",Red,Green);
        f_close(&fdst);//�ر��ļ� 
    }
     return;  */
        
        
    //��ʼ��
    if(Sd_writeOnce==0)//write flash
    {
        #define Send_hang 150
        //Image_fire[Send_hang]
        SECTOR_NUM = flash_write_count/32;
        add = (flash_write_count%32)*64;
        flash_write_buf(186 + SECTOR_NUM,add,40,Image_fire[Send_hang]);
        
        //Time_1ms
        //centre[Send_hang]
        H = (unsigned int)(Time_1ms);
        L = (unsigned int)(centre[Send_hang]);
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 40,write_data);
        
        //black_L[Send_hang]
        //black_R[Send_hang]
        H = (unsigned int)(black_L[Send_hang]);
        L = (unsigned int)(black_R[Send_hang]);
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 44,write_data);
        
        //halfwidth[Send_hang]
        //Angle.m_angle
        H = (unsigned int)(halfwidth[Send_hang]);
        L = (unsigned int)((int)(Angle.m_angle*10));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 48,write_data);
        
        //Angle.G_angle
        //Speed.Car
        H = (unsigned int)((int)(Angle.G_angle*10));
        L = (unsigned int)((int)(Speed.Car));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 52,write_data);
        
        //Direction.Gyro
        //Angle.PWM
        H = (unsigned int)((int)(Direction.Gyro));
        L = (unsigned int)((int)(Angle.PWM));//all_PWM_L
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 56,write_data);
        
        //Speed.PWM
        //Speed.PWM_Integral
        H = (unsigned int)((int)(Speed.PWM));//all_PWM_R
        L = (unsigned int)((int)(Speed.PWM_Integral));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 60,write_data);
        
        flash_write_count++;
        if(flash_write_count >= write_count_max)
           Sd_writeOnce = 1;
    }
    else if(Sd_writeOnce==1 && StartLine.flag==1 && Time_1ms>StartLine.time_record + StartLine.Delaytime + 1)//read flash and write SD
    {
        //����
        _LED4 = 0;
        LCD_PutString(0,80,"SD start write!!!",Red,Green);
        //����
        f_mount(0, &fs);	//SD��ϵͳ��ʼ��
        f_open(&fdst, "\\data.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //���ļ������û�оʹ���������д��
        for(int SECTOR_count = 186;SECTOR_count <= 255;SECTOR_count++)
        {
            for(int in_count = 0;in_count < 32;in_count++)
            {
                int read_add = in_count * 64;
                for(int i=0,ii=0;i<40;i++)
                {
                    data8 = flash_read(SECTOR_count, read_add + i,uint8);
                    Send_Img[ii++] = ((((unsigned int)(data8))&0xf0)>>4) + 'a';
                    Send_Img[ii++] = (((unsigned int)(data8))&0x0f) + 'a';
                }
                f_write(&fdst,Send_Img,80,&bw);
                for(int i=40;i<64;i+=4)
                {
                    data32 = flash_read(SECTOR_count,read_add + i,uint32);	//��ȡ4�ֽ�
                    wH = (short int)(data32>>16);
                    wL = (short int)(data32&0x0000ffff);
                    f_printf(&fdst, " %d", wH);
                    if(i==60)
                        f_printf(&fdst, " %d\n", wL);
                    else
                        f_printf(&fdst, " %d", wL);
                }
            } 
        }
        f_close(&fdst);//�ر��ļ� 
        Sd_writeOnce = 2;
        LCD_PutString(0,80,"SD write finish!!!",Red,Green);
    }
    else if(Sd_writeOnce ==2)
    {
        _LED4 = (Time_1ms/500)%2;
        return;
    }
}