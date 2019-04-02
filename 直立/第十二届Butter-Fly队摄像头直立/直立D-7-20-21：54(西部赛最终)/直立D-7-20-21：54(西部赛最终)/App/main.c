#include "common.h"
#include "include.h"

/*** ���Բ��� ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

/* 
	0��120 �ٶ�Լ 2.19
	1��130 �ٶ�Լ 2.25
	2��140 �ٶ�Լ 2.30
	3��150 �ٶ�Լ 2.45
	4��155 �ٶ�Լ 2.6
	5��160 �ٶ�Լ
	6��165 �ٶ�Լ
*/
char Mode_Set = 4;	// ģʽѡ�� 	very important !!!!!!

/*********** main����***********/
void main(void)
{  
  	System_Init();		//��ʼ������ģ��
	
  	while(1)
  	{
		/************************ ͼ��ɼ�����ʾ  ***********************/
		while (!Image_Flag);
		Image_Flag = 0;
		img_extract(Img, ImageBuff,CAMERA_SIZE);	// ��ѹ
		camera_get_img();                       // ����ͷ��ȡͼ�� 
		
			
		/************************* ͼ���㷨���� ************************/
		if (!Annulus_Flag)
		{
			Image_Handle(Img);			// ��ͨͼ����
		}
		if (Annulus_Flag)
		{
			Annulus_Handle(Img);		// ��·ר��ͼ����
		}

		Difference = Point_Average();	// ���߷�
		Radius = PlacePID_Control(&Turn_PID, Turn[Fres], Difference, 0);	// ��̬PID����ת��
		
		Start_Control();	// �����߼��
		
		
		/************************ �������ݷ���   ***********************/  
		if(Image_Send_Flag && Send_OK) //ѡ���Ƿ���ͼ����Ϣ����λ��
		{
			Send_OK = 0;
			img_recontract(Img, imgbuff,CAMERA_SIZE);		// ͼ��ѹ��
			seekfree_sendimg_032(imgbuff, CAMERA_SIZE/2);	// ���Ͷ�ֵ��ͼ���������λ��
//			vcan_sendimg(imgbuff, CAMERA_SIZE);				// ���Ͷ�ֵ��ͼ����ɽ����λ��
//			data_send();							   		// �������ߵ�����
		}
	}
}











