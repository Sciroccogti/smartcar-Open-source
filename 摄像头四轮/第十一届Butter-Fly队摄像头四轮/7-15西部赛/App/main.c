#include "common.h"
#include "include.h"
  
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);	//ͼ���ַ
uint8 img[CAMERA_W*CAMERA_H]; //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��
uint8 screen_mid = 40;		  //��Ļ�е�

uint8  threshold_value = 43,var_speed = 5;
uint16 ADC_light,			//����ͷ��ֵ����
	   ADC_voltage; 		//��ص�ѹ���
int32 S_D5_Duty = 4455;  	//(3330��4480��5630)

/*** ���Բ��� ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

/*********** main����***********/
  void  main(void)
{	
	uchar i;          	//��������
	uchar Send_Count; 	//������Ҫ���͵����ݸ���
	uchar Point = 40;	//Ŀ��������
	uint8 Mid_Count;	//����ʶ�𵽵��е�����
  
  	System_Init();		//��ʼ������ģ��
	SCCB_WriteByte (OV7725_CNST, 45);				//�ı�ͼ����ֵ
							 				//������55
  	while(1)
  	{   
		/************************ ͼ��ɼ�����ʾ  ***********************/
		camera_get_img();                       //����ͷ��ȡͼ��   
		img_extract(img, imgbuff,CAMERA_SIZE);	//��ѹ
			
		
		/************************* ͼ���㷨���� ************************/
		Mid_Count = Image_GetLine(img);	//ͼ��������õ��е�����
		
		
		/************************ ����Ŀ��Ƕ� *************************/
		Point = Point_Weight();				//ʹ�ü�Ȩƽ����ȡĿ��������	
//		Point = Wave_Filtering(Point, 3);
		save_var(VAR3, Point);				//Ŀ������귢����λ��
		save_var(VAR4, Foresight);			//��������ǰհ������λ��
		
		
		/************************ ����ٶȿ��� *************************/
		Start_Control();	//�����߼����ͣ��//		//�ٶȿ��Ʒ��ڶ�ʱ���ж��У�10msִ��һ��
/*		Send_Count = DataScope_Data_Generate(10); 	//����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
		for( i = 0 ; i < Send_Count; i++)  			//ѭ������,ֱ���������   
		{  
			uart_putchar(UART4, DataScope_OutPut_Buffer[i]); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ      
		}*/
		
		
		/************************* ������� ****************************/			 
		S_D5_Duty = PlacePID_Control(&S_D5_PID, 40, Point);	//ʹ��λ��ʽPID����	
//		S_D5_Duty = range_protect(S_D5_Duty, 3330, 5630);	//���ռ�ձ��޷�����
		S_D5_Duty = range_protect(S_D5_Duty, 3320, 5700);
		ftm_pwm_duty(S_D5_FTM, S_D5_CH, S_D5_Duty);			//�޸Ķ��ռ�ձ�5404 5540
				
		
		/********************** HMI���������շ�   **********************/ 
		if (!System_OK)
		{
			HMI_Data_Handle();
		}
	   
		
		/************************ �������ݷ���   ***********************/  
		if(Image_Send_Flag && Send_OK) //ѡ���Ƿ���ͼ����Ϣ����λ��
		{
			Send_OK = 0;
			
			img_recontract(img, imgbuff,CAMERA_SIZE); //ͼ��ѹ��
			data_send();							   //��������λ��
		}
	}
}











