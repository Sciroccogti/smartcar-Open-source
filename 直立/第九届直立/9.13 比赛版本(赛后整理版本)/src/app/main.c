#include "include.h"


/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵��������ȫ�ֱ����Ķ����Լ���ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
struct CarAngle Angle = {0,0,0,0,0,0,0,0,0,0};
struct CarSpeed Speed = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
struct CarDirection Direction = {0,0,0,0,0,0};
struct CarStartLine StartLine = {0,0,0,0};
struct CarRen Ren = {0,0,0,0,0,0,0,0,{0,0}};
struct CarBlock Block = {0,0,0,0};
long int Time_1ms=0;



/*************************************************************************
*  �������ƣ�main
*  ����˵����������
*  ����˵������
*  �������أ���r
*  �޸�ʱ�䣺2012-2-14    һֱ�ڲ���
*************************************************************************/
void main()
{
    DisableInterrupts;
    delayms(100);
    System_Init();
    delayms(100);
    EnableInterrupts;
    
    while(1)
    {
        while(1)
        {
            //����ʱ��,��������
            #if 0
            static int Test_num=0;
            if(Test_num==1)
            {
                Test_IO = 1;         
                Test_num = 0;
                //��������ʱ��ĺ�����������
            }
            else
            {
                Test_IO = 0;
                Test_num = 1;
                //��������ʱ��ĺ�����������
            }
            #endif
            
            
            //ͼ��ɼ��ʹ���ͷ������
            ov7725_get_img();
            Process_Image();
            Direction.output_enable = 1;
            

            //send infomation
            #if 0
            if(StartLine.flag==0)
            {   
                SCISend_to_Own(UART3);
            }
            #endif
        }
    }
}
