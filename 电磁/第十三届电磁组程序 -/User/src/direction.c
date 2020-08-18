/*********************************************************************************************************************
 * @file  DirectionControl.c
 *  				�������
 * @core			S9KEA128
 * @date   2018
 ********************************************************************************************************************/
#include  "main.h"

/**********ȫ�ֱ�������********/ 
float Direction_P = 610;	//�������P  ���Pƫ����Ӧ��� �������ת�估ʱ���ǹ����˾�Ӧ����P
float Direction_D = 2200;	//�������D
float DirectionErr[2];	//����ƫ�g_fDirectionError[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ�
				//��g_fDirectionError[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ�
float DirectionErr_dot[2];//����ƫ��΢�֣�g_fDirectionError_dot[0]Ϊһ��ˮƽ��еĲ�Ⱥ�ƫ��΢�֣�
				//��g_fDirectionError_dot[1]Ϊһ�Դ�ֱ��еĲ�Ⱥ�ƫ��΢�֣�
float DirectionOut=0;	//����������
int16 sensor[5]={0};       //��ȡ�ĵ��ֵ

int16 sensorFilter[5]={0};	//�����˲��ĵ��ֵ��δʹ�ã�
uint8 Flag_Round1 = 0;		//���뻷���ı�־���ڻ�����ΪON��
uint8 Flag_Round2 = 0;
uint8 Flag_Round3 = 0;  //Ŀǰ�ж�������ջ������ҹ�

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
  static float DirectionErrTemp[2][5]={0};  //��ʱ�������
	Read_ADC();		//��ȡ���ֵ	
	if(sensor[0]<10 && sensor[1]<10)  Stop_Flag=1;	//�������ͣ������	
	sensor[0] = (sensor[0] < 10? 10:sensor[0]);	//���ֵ�޷�
	sensor[1] = (sensor[1] < 10? 10:sensor[1]);
	sensor[2] = (sensor[2] < 10? 10:sensor[2]);
	sensor[3] = (sensor[3] < 10? 10:sensor[3]);
        sensor[4] = (sensor[4] < 10? 10:sensor[4]);
	//ˮƽ���ƫ�����		
	DirectionErr[0] = (float)(sensor[0]-sensor[1])/(sensor[0]+sensor[1]);//��-��ˮƽ��еĲ�Ⱥ���Ϊƫ��
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);    //ƫ���޷�	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//��ֱ���ƫ�����
	DirectionErr[1] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);//��-�Ҵ�ֱ��еĲ�Ⱥ���Ϊƫ��
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	//ƫ���޷�
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//ˮƽ���ƫ��΢�ּ���(��ʷ���ѭ������)  
	DirectionErrTemp[0][4] = DirectionErrTemp[0][3];
	DirectionErrTemp[0][3] = DirectionErrTemp[0][2];
	DirectionErrTemp[0][2] = DirectionErrTemp[0][1];
	DirectionErrTemp[0][1] = DirectionErrTemp[0][0];
	DirectionErrTemp[0][0] = DirectionErr[0];
	DirectionErr_dot[0] = 5*(DirectionErrTemp[0][0]-DirectionErrTemp[0][3]);//ˮƽ��е�ƫ��΢��
	DirectionErr_dot[0] = (DirectionErr_dot[0]> 0.7? 0.7:DirectionErr_dot[0]);//ƫ��΢���޷�
	DirectionErr_dot[0] = (DirectionErr_dot[0]<-0.7?-0.7:DirectionErr_dot[0]);
        //��ֱ���ƫ��΢�ּ���(��ʷ������ѭ��)  
	DirectionErrTemp[1][4] = DirectionErrTemp[1][3];
	DirectionErrTemp[1][3] = DirectionErrTemp[1][2];
	DirectionErrTemp[1][2] = DirectionErrTemp[1][1];
	DirectionErrTemp[1][1] = DirectionErrTemp[1][0];
	DirectionErrTemp[1][0] = DirectionErr[1];
	DirectionErr_dot[1] = 5*(DirectionErrTemp[1][0]-DirectionErrTemp[1][3]);//��ֱ��е�ƫ��΢��
	DirectionErr_dot[1] = (DirectionErr_dot[1]> 0.7? 0.7:DirectionErr_dot[1]);//ƫ��΢���޷�
	DirectionErr_dot[1] = (DirectionErr_dot[1]<-0.7?-0.7:DirectionErr_dot[1]);	
	//����Ϊ��������
	if((sensor[0]>900)&&(sensor[1]>900)&&(sensor[2]<200)&&(sensor[3]<200)&&(Flag_Round2<3))
	{
          if(Flag_Round1==0)
             {Flag_Round1=1;}
//          if(Flag_Round2==1)
//             {DirectionErr[1]=DirectionErr[1]*10;
//             DirectionErr_dot[1]=DirectionErr_dot[1]*10;}
	}	
	if(Flag_Round1>50) 
	{
		Flag_Round1=0;
                Flag_Round2=Flag_Round2+1;                
	}
	if(Flag_Round1==0){
          if(DirectionErr[0]<0.2&&DirectionErr[0]>-0.2){
            DirectionOut=DirectionErr[0]*(Direction_P-500)+DirectionErr_dot[0]*(Direction_D-2000);}  //��һ��������ˮƽ���λ�ò�ͬҪ����һ����������
          else{
            DirectionOut=DirectionErr[0]*Direction_P+DirectionErr_dot[0]*Direction_D;}
        }
	else
        {   DirectionOut=DirectionErr[1]*700+DirectionErr_dot[1]*1600;
            Flag_Round1=Flag_Round1+1;
        }  //����һ��������ˮƽ���λ�ò�ͬҪ����һ����������	  
}
/**
 * @file  ����źŲɼ�����ֵ�˲�
 * @note      	
 * @date  2018
 **/
void Read_ADC(void) 
{
     int16 i,j,k,temp;
     int16 ad_value[5][5],ad_valueF[5],ad_sum[5];
//     int16 ad_value_old[5],ad_value_new[5];
     for(i=0;i<5;i++)
     {   ad_value[0][i]=AD_Avg(AD1, ADC_12bit, 5);  // AD1ˮƽ��
         ad_value[1][i]=AD_Avg(AD2, ADC_12bit, 5);  // AD2ˮƽ��
         ad_value[2][i]=AD_Avg(AD3, ADC_12bit, 5);  // AD3��ֱ��
         ad_value[3][i]=AD_Avg(AD4, ADC_12bit, 5);  // AD4��ֱ��
     }  //û�й�һ������
/*=========================ð����������==========================*///�������ֵ����Сֵ
     for(i=0;i<5;i++)
     {
       for(j=0;j<4;j++)
       {
         for(k=0;k<4-j;k++)  //������ֻ֪��Ҫ����4��
         {
           if(ad_value[i][k]>ad_value[i][k+1]) //ǰ��ıȺ���Ĵ�����н���
           {
             temp=ad_value[i][k+1];
             ad_value[i][k+1]=ad_value[i][k];
             ad_value[i][k]= temp;
           }
         }
       }
     }
/*===========================��ֵ�˲�=================================*/
     for(i=0;i<5;i++)    //���м�����ĺ�
     {
        ad_sum[i] =ad_value[i][1]+ad_value[i][2]+ad_value[i][3];  //��ȥ�����Сȡ�м�����
        ad_valueF[i]=ad_sum[i]/3;  //��ƽ��ֵ
     }	
	 for(i=0;i<5;i++)  {//����ֵ�и�λ������,���͹��߾���
	     sensor[i] = (int16)(ad_valueF[i]/10*10);  //
		//�ɼ��ݶ�ƽ����ÿ�βɼ����仯40(û���õ�)
//		ad_value_old[i]=sensorFilter[i];
//		ad_value_new[i]=sensor[i];		
//		if(ValueOfADNew[i]>=ValueOfADOld[i])
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);  //��С�仯��
//		else
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);  //��С�仯��
	 }
}
	 


