/*********************************************************************************************************************
 * 
 * @file       		direction.c
 *  				�������
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "includefile.h"


/**********ȫ�ֱ�������********/ 
float g_dirControl_P = 700;		//�������P
float g_dirControl_D = 2200;	//�������D
float g_fDirectionError[2];		//����ƫ�g_fDirectionError[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ�
								//		  ��g_fDirectionError[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ�
float g_fDirectionError_dot[2];//����ƫ��΢�֣�g_fDirectionError_dot[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ��΢�֣�
								//			 ��g_fDirectionError_dot[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ��΢�֣�
float g_fDirectionControlOut;	//����������
int16 g_ValueOfAD[4]={0};		//��ȡ�ĵ��ֵ
int16 g_ValueOfADFilter[4]={0};	//�����˲��ĵ��ֵ��δʹ�ã�
uint8 Flag_Round = OFF;			//���뻷���ı�־���ڻ�����ΪON��



/**
 * @file		�������
 *				һ������£�����ˮƽ��еĲ�Ⱥ���Ϊƫ��
 *				�ڻ���ʱ�У�������ֱ��еĲ�Ⱥ���Ϊƫ��
 *
 *									���ֵ��Ӧ����
 *
 *				g_ValueOfAD[0]									g_ValueOfAD[1]
 *				(ˮƽ����)									��ˮƽ�ҵ�У�
 *						g_ValueOfAD[2]					g_ValueOfAD[3]
 *						����ֱ���У�					����ֱ�ҵ�У�
 * @date		2018
 */
void DirectionControl(void)
{
	static float g_fDirectionErrorTemp[2][5];
	
	Read_ADC();		//��ȡ���ֵ
	
	if(g_ValueOfAD[0]<50 && g_ValueOfAD[1]<50)	Flag_Stop = 1;	//�������ͣ������
	
	g_ValueOfAD[0] = (g_ValueOfAD[0] < 10? 10:g_ValueOfAD[0]);	//�ĸ����ֵ�޷�
	g_ValueOfAD[1] = (g_ValueOfAD[1] < 10? 10:g_ValueOfAD[1]);
	g_ValueOfAD[2] = (g_ValueOfAD[2] < 10? 10:g_ValueOfAD[2]);
	g_ValueOfAD[3] = (g_ValueOfAD[3] < 10? 10:g_ValueOfAD[3]);
			
	g_fDirectionError[0] = (float)(g_ValueOfAD[0] - g_ValueOfAD[1])/(g_ValueOfAD[0] + g_ValueOfAD[1]);//ˮƽ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[0] = (g_fDirectionError[0]>= 1? 1:g_fDirectionError[0]);	//ƫ���޷�
	g_fDirectionError[0] = (g_fDirectionError[0]<=-1?-1:g_fDirectionError[0]);
	
	g_fDirectionError[1] = (float)(g_ValueOfAD[2] - g_ValueOfAD[3])/(g_ValueOfAD[2] + g_ValueOfAD[3]);//��ֱ��еĲ�Ⱥ���Ϊƫ��
	g_fDirectionError[1] = (g_fDirectionError[1]>= 1? 1:g_fDirectionError[1]);	//ƫ���޷�
	g_fDirectionError[1] = (g_fDirectionError[1]<=-1?-1:g_fDirectionError[1]);	
	  
	g_fDirectionErrorTemp[0][4] = g_fDirectionErrorTemp[0][3];
	g_fDirectionErrorTemp[0][3] = g_fDirectionErrorTemp[0][2];
	g_fDirectionErrorTemp[0][2] = g_fDirectionErrorTemp[0][1];
	g_fDirectionErrorTemp[0][1] = g_fDirectionErrorTemp[0][0];
	g_fDirectionErrorTemp[0][0] = g_fDirectionError[0];
	g_fDirectionError_dot[0] = 5*(g_fDirectionErrorTemp[0][0]-g_fDirectionErrorTemp[0][3]);//ˮƽ��е�ƫ��΢��
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]> 0.7? 0.7:g_fDirectionError_dot[0]);//ƫ��΢���޷�
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]<-0.7?-0.7:g_fDirectionError_dot[0]);

	g_fDirectionErrorTemp[1][4] = g_fDirectionErrorTemp[1][3];
	g_fDirectionErrorTemp[1][3] = g_fDirectionErrorTemp[1][2];
	g_fDirectionErrorTemp[1][2] = g_fDirectionErrorTemp[1][1];
	g_fDirectionErrorTemp[1][1] = g_fDirectionErrorTemp[1][0];
	g_fDirectionErrorTemp[1][0] = g_fDirectionError[1];
	g_fDirectionError_dot[1] = 5*(g_fDirectionErrorTemp[1][0]-g_fDirectionErrorTemp[1][3]);//��ֱ��е�ƫ��΢��
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]> 0.7? 0.7:g_fDirectionError_dot[1]);//ƫ��΢���޷�
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]<-0.7?-0.7:g_fDirectionError_dot[1]);
	
	//����Ϊ��������
	if((g_ValueOfAD[0]>2300)&&(g_ValueOfAD[1]>2300)&&(g_ValueOfAD[2]<800)&&(g_ValueOfAD[3]<800))
	{
		Flag_Round = ON;
		gpio_set(BUZZ,1);
	}
	
	if((g_ValueOfAD[2]>200)&&(g_ValueOfAD[3]>200)&&(Flag_Round == ON))
	{
		Flag_Round = OFF;
		gpio_set(BUZZ,0);
	}
	//flash�в���
//	g_dirControl_P = float01;	//700
//	g_dirControl_D = float02;	//2200
	
	//�����㷨��λ��ʽPD��
	g_fDirectionControlOut = g_fDirectionError[0]*g_dirControl_P + g_fDirectionError_dot[0]*g_dirControl_D;
	if(Flag_Round == ON)
	g_fDirectionControlOut = g_fDirectionError[1]*700 + g_fDirectionError_dot[1]*1600;	
	  
}


/**
 * @file		����źŲɼ��ʹ���
 * @note      	
 * @date		2018
 */
void Read_ADC(void)
{
     int16  i,j,k,temp;
     int16  ad_valu[4][5],ad_valu1[4],ad_sum[4];
	 int16 ValueOfADOld[4],ValueOfADNew[4];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=ad_ave(AD1, ADC_12bit, 5);  			// AD1ˮƽ��
         ad_valu[1][i]=ad_ave(AD2, ADC_12bit, 5);     		// AD2ˮƽ��
         ad_valu[2][i]=ad_ave(AD3, ADC_12bit, 5);  			// AD3��ֱ��
         ad_valu[3][i]=ad_ave(AD4, ADC_12bit, 5);     		// AD4��ֱ��		 
     }
/*=========================ð����������==========================*///�������ֵ����Сֵ
     for(i=0;i<4;i++)
     {
        for(j=0;j<4;j++)
        {
           for(k=0;k<4-j;k++)
           {
              if(ad_valu[i][k] > ad_valu[i][k+1])        //ǰ��ıȺ���Ĵ�  ����н���
              {
                 temp = ad_valu[i][k+1];
                 ad_valu[i][k+1] = ad_valu[i][k];
                 ad_valu[i][k] = temp;
              }
           }
        }
     }
/*===========================��ֵ�˲�=================================*/
     for(i=0;i<4;i++)    //���м�����ĺ�
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
     }

	
	 for(i=0;i<4;i++)            //����ֵ�и�λ������
	 {
	 	g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);

		//�ɼ��ݶ�ƽ����ÿ�βɼ����仯40
		ValueOfADOld[i] = g_ValueOfADFilter[i];
		ValueOfADNew[i] = g_ValueOfAD[i];
		
		if(ValueOfADNew[i]>=ValueOfADOld[i])
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);
		else
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);
	 }
	 
}


