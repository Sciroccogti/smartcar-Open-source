/* *****************************************       
      ͼ������
      ���ܣ��ҵ����ߺ���
            �ҳ�����
            ����ʮ����
            �����Ч��
            ����ֱ���� 
 **************  3��13��   ******************/  
#include "common.h"
#include "include.h" 
#include "init.h"
#include "tx_process.h" 




extern float   DP_1 ;        // 6      �����
extern float   DD_1 ;        // 4  
extern float   DS_1 ;        //1.2
extern float   DP_2 ;        //7       С���
extern float   DD_2 ;        // 5
extern float   DS_2 ;        //1.2
extern float   DP_3 ;        //2        ֱ��
extern float   DD_3 ;        //1
extern float   DS_3 ;        // 0.5

void   DX_deal(uint8 qishi_hang,int16 center_point);

extern float  P_1; 
extern float  P_2; 
extern float  P_3; 
extern float  D_1;
extern float  D_2; 
extern float  D_3;

extern float  S_1;
extern float  S_2;
extern float  S_3;

int16   blackright[num],blackleft[num],center[num],center_2[num];
uint8   valid_row,valid_row_;
int32   angle_pwm;
int32   last_angle;
uint8   valid_row_11;
int8    GX_flag;
int8    bad_pic=0;

extern int8 num_00,num_11;
extern uint8 heixian_flag_0;
extern int16  val_z, val_y;  

int16   bxiankuan[52]= {   32,32,32,31,31,31,30,30,30,29,29,
                           28,28,27,27,27,26,26,25,25,25,
                           24,24,23,23,22,22,21,21,20,20,
                           19,19,19,18,18,17,17,16,16,16,
                           15,15,14,14,13,13,13,13,12,12 ,11};
/*
int16   Fxiankuan[52]= {   16,16,16,16,16,16,15,15,15,15,15,
                           14,14,14,14,14,13,13,13,13,13,
                           12,12,12,12,11,11,11,11,10,10,
                           10,10,10, 9, 9, 9, 9, 8, 8, 8,
                            8, 8, 7, 7, 7, 7, 7, 7, 6, 6,6};
*/
int16   Fxiankuan[52]= {   15,14,14,14,14,14,13,13,13,13,13,
                           13,12,12,12,12,12,11,11,11,11,
                           11,10,10,10,10, 9, 9, 9, 9, 9,
                            9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 
                            6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4};

uint8  sample_row[52]={   57,55,54,53,52,51,50,49,48,47,
                          46,45,44,43,42,41,40,39,38,37,
                          36,35,34,33,32,31,30,29,28,27,
                          26,25,24,23,22,21,20,19,18,17,
                          16,14,13,12,11,10, 9, 8, 7, 6, 5, 4};

float  quanzhi[52]=    {  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1,
                          1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.5, 1.5, 1.5, 1.5, 1.5,
                          1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
                          1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8,
                          2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0 };
float  quanzhi_1[52]=    {  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1,
                          1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.5, 1.5, 1.5, 1.5, 1.5,
                          1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
                          1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8,
                          2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0 };
float abs_flt(float i)//���ã� �����ֵ  ������
{
     float A;
     A=i;
     if(A<0) return (-A);
     else    return A;
} 

int16 abs_int(int16 i)//���ã� �����ֵ  int��
{
     int16 B;
      B=i;
     if(B<0) return (-B);
     else    return B;
} 
/*************��������********************        
          �˲�������ֵ�˲�
          
          ���Ż� 
*************3��13��  ��֤ͨ�� ***********/  
uint8  MiddleFilter(uint8 a,uint8 b,uint8 c)
{  
  uint8 temp;
  if(a>b) 
  {
    temp=b;
    b=a;
    a=temp;
  }
  if(b>c)
  {
    temp=c;
    c=b;
    b=temp;
  }
  if(a>b)
  {
    temp=b;
    b=a;
    a=temp;
  }
  return b;
}


int16 max(int16 a,int16 b,int16 c)
{
     uint16 temp;
     temp=a;
     if(b>temp) 
     {
       temp=b;
     }
     if(c>temp)
     {
       temp=c;
     }
      if(temp==a&&temp!=b&&temp!=c)
      return 49;
      if(temp==b&&temp!=a&&temp!=c)
      return 50;
      if(temp==c&&temp!=a&&temp!=b)
      return 51;
      if(temp==a&&temp==b&&temp!=c)
      return 50;
      if(temp==a&&temp!=b&&temp==c)
      return 50;
      if(temp!=a&&temp==b&&temp==c)
      return 51;
      if(temp==c&&temp==b&&temp==a)
      return 51;
}

/*************��������********************        
          Ԥ������ֱ�ǵķ���
            
          ���Ż�  
*************3��13��  ��֤ͨ�� ***********/  


/*Ԥ�к������ж�ֱ�ǵ����ҷ��򣬱������У�˼·���£�
����Ѱ�ߣ��������������ж�����Ϊ��ɫ���ұ�û���ߣ���ԶС��79.��Ϊ��ֱ��
����֮����Ϊ���˶���*/

int8  y_zj_flag=0,z_zj_flag=0;

void yupan_zy()
{
   int8 i;
   for(i=0;i<=50;i++)
   {
      if(blackleft[i]>3&&blackright[i]>=75&&
         blackleft[i+1]>3&&blackright[i+1]>=75)
      {
          if(i!=0)       //�ų����˶���
          {
               y_zj_flag=1;              
               break;
          }
          else
            break;
       }
       else if(blackleft[i]<=3&&blackright[i]<75&&
                blackleft[i+1]<=3&&blackright[i+1]<75)
       {
           if(i!=0)       //�ų����˶���
           {
                z_zj_flag=1;               
                break;
           }
           else
             break;
       }
    }
}
/*************��������********************        
          �Ľ��ĸ��ٱ��ؼ�ⷨ
          
          ���Ż�  
*************3��13��  ��֤ͨ�� ***********/  
int8  SX_flag=0,DX_flag=0,DS_flag=0,SD_flag=0; 
uint8  danxian_count=0,suangxian_count=0;
int16  blackright_0[6]={79,79,79,79,79,79},blackleft_0[6]={0,0,0,0,0,0};
int16  blackright_1[6]={79,79,79,79,79,79},blackleft_1[6]={0,0,0,0,0,0};
int8   z_diuxian=0,y_diuxian=0;
uint8  white_count;
int8   qianduan_flag=0,zhongduan_flag=0,houduan_flag=0; 
int8   last_pic;
int8   qianduan_count,zhongduan_count,houduan_count;
uint8  zhongdian;
int8   DX_flag_1,DX_flag_2,DX_flag_3,DX_flag_4,DX_flag_5,DX_flag_6,DX_FLAG,SX_FLAG,err_flag,D_FLAG,SD_FLAG;
int8   DX_count=0,SX_count=0,CZ_DX, DS_FLAG,CS;

void ShiBie_DX()
{
    int8 i,j,sample_row_1,k; 
    zhongdian=0;
    danxian_count=0;
    suangxian_count=0;
    white_count=0;
    DX_flag=0;
    SX_flag=0; 
    DS_flag=0;
    SD_flag=0; 
    qianduan_flag=0;
    zhongduan_flag=0;
    houduan_flag=0; 
    qianduan_count=0;
    zhongduan_count=0;
    houduan_count=0;
    
    CS=0;
    D_FLAG=0;
    CZ_DX=0;
    DS_FLAG=0;
    SD_FLAG=0;
    DX_FLAG=0;
    DX_flag_1=0;
    DX_flag_2=0;
    DX_flag_3=0;
    DX_flag_4=0;
    DX_flag_5=0;
    DX_flag_6=0;
    DX_FLAG=0;
    SX_FLAG=0;
    err_flag=0;
    DX_count=0;
    SX_count=0;
    for(j=0;j<6;j++)
    {
          if(j==0)   sample_row_1=sample_row[0];
          if(j==1)   sample_row_1=sample_row[10];       //ǰ��
          if(j==2)   sample_row_1=sample_row[15];
          if(j==3)   sample_row_1=sample_row[20];       //�ж�
          if(j==4)   sample_row_1=sample_row[30];
          if(j==5)   sample_row_1=sample_row[40];       //���
       //   sample_row_1=sample_row[j];
        //for(i=79-2;i>0;i--)
          for(i=0;i<79-2;i++)
          {
            if(img[sample_row_1][i]==1&&img[sample_row_1][i+1]==0)
            {
          //  for(k=i;k>0;k--)
              for(k=i;k<79-2;k++)
              {
                  if(img[sample_row_1][k]==0&&img[sample_row_1][k+1]==1)
                  {
                      if(abs_int(k+1-(i+1))<=9&&abs_int(k+1-(i+1))>1)
                      {       
                             blackleft_0[j]=i+1  ;
                             blackright_0[j]=k+1 ;
                             break;
                      }   
                  }
               }
             }
           }  //һ������
        ///////////////˫��ʶ��/////////////////////  
          for(i=0;i<79-2;i++)
          {
            if(img[sample_row_1][i]==0&&img[sample_row_1][i+1]==1)
            {
              for(k=i;k<79-2;k++)
              {
                  if(img[sample_row_1][k]==1&&img[sample_row_1][k+1]==0)
                  {
                      if(abs_int(k+1-(i+1))>18)
                      {       
                             blackleft_1[j]=i+1  ;
                             blackright_1[j]=k+1 ;
                             break;
                      }   
                  }
               }
             }
           }  //һ������  
          
       ///////////////˫��ʶ�����/////////////////////           
          if(abs_int(blackright_0[j]-blackleft_0[j])<10)  //����  ��Ҫ��һ���ж�
          {
                 DX_flag=1;
                 DX_count=DX_count+1;  
          }
          else if(abs_int(blackright_0[j]-blackleft_0[j])>8&&abs_int(blackright_0[j]-blackleft_0[j])<2*bxiankuan[j]+10)
          {
                 SX_flag=1;
                 SX_count=SX_count+1; 
          }
          else      //�����ǵ����ֲ���˫��
          {
                 err_flag=1;
          }
    }
    if(DX_flag)
    {
        if(abs_int(blackleft_0[0]-blackright_0[0])<=9)  
        {
             DX_deal(0,(blackright_0[0]+blackleft_0[0])/2);
             if(valid_row_11>=4&&abs_int(blackright_0[0]-blackleft_0[0])<10)
             {
                  DX_flag_1=1;  
             }
                 //  else if(valid_row_11>2)                    //����ĩ�ˣ��ǳ�����˫��
        }
        if(DX_flag_1==0&&abs_int(blackright_0[1]-blackleft_0[1])<8)  
        {
              DX_deal(10,(blackright_0[1]+blackleft_0[1])/2);
              if(valid_row_11>5&&abs_int(blackright_0[1]-blackleft_0[1])<9)
              {
                   DX_flag_2=1;
              }
     
        }
        if(DX_flag_1==0&&DX_flag_2==0&&abs_int(blackright_0[2]-blackleft_0[2])<8)  
        {
                   
              DX_deal(15,(blackright_0[2]+blackleft_0[2])/2);
              if(valid_row_11>5&&abs_int(blackright_0[2]-blackleft_0[2])<9)
              {
                  DX_flag_3=1;
              }
                   
        }
        if(DX_flag_1==0&&DX_flag_2==0&&DX_flag_3==0&&abs_int(blackright_0[3]-blackleft_0[3])<8)  
        {
              DX_deal(20,(blackright_0[3]+blackleft_0[3])/2);
              if(valid_row_11>5&&abs_int(blackright_0[3]-blackleft_0[3])<8)
              {
                  DX_flag_4=1;
              }        
        }
        if(DX_flag_1==0&&DX_flag_2==0&&DX_flag_3==0&&DX_flag_4==0&&abs_int(blackright_0[4]-blackleft_0[4])<8)  
        {
              DX_deal(30,(blackright_0[4]+blackleft_0[4])/2);
              if(valid_row_11>5&&abs_int(blackright_0[4]-blackleft_0[4])<8)
              {
                    DX_flag_5=1;
              }        
        }
        if(DX_flag_1==0&&DX_flag_2==0&&DX_flag_3==0&&DX_flag_4==0&&DX_flag_5==0&&abs_int(blackright_0[5]-blackleft_0[5])<8)  
        {
               DX_deal(40,(blackright_0[5]+blackleft_0[5])/2);
               if(valid_row_11>2&&abs_int(blackright_0[5]-blackleft_0[5])<6)
               {
                    DX_flag_6=1;
               }       
        }
    }
    if(DX_flag_1||DX_flag_2||DX_flag_3||DX_flag_4||DX_flag_5||DX_flag_6)    //���ڵ��� ��ȫ������˫��˫�����������
    {
         CZ_DX=1;
         if(DX_flag_1)                                                    //ȫ����  ���ߵ�˫�л��� ,�������һ����
         {
              D_FLAG=1;
              if(valid_row_11<30&&img[sample_row[valid_row_11+1]][center[valid_row_11]]==1)
              {
                   DS_FLAG=1;
              } 
         }
         else                                                             //ֻ��һ����� ��˫���л��� 
         {
               SD_FLAG=1;  
               if(DX_flag_2)         CS=10;
               else if(DX_flag_3)   CS=15;
               else if(DX_flag_4)   CS=20;
               else if(DX_flag_5)   CS=30;
               else if(DX_flag_6)   CS=40;                   
          }
    }
    else                                                                     //ȫΪ˫��
    {
          SX_FLAG=1;      
    }  
}


////////////////////���򲻿ɶ�///////////////////////////////
int8  Z_Z_Flag=0,Y_Y_Flag=0 ,M=0, L=0, Z_COUNT=0, Y_COUNT=0,last_flag_Z=0,last_flag_Y=0;  //�ϰ�ʶ��
int8  Z_hang=0 ,Y_hang=0;
void  SX_deal()     //��ȡ����
{
    uint8 i,j,sample_row_1,find_right,find_left;
    int8 center_row_1,center_row_2;
     Z_Z_Flag=0;
     Y_Y_Flag=0;
    for(i=0;i<52;i++)       //ֻɨ�����30�е�ͼ������
    {
        find_right=0;
        find_left=0;
        if(i==0)
        {
          center_row_1=40;
          center_row_2=40;
        }
        sample_row_1=sample_row[i];
        if(center_row_1>r)
           center_row_1=r;
        if(center_row_1<z)  
           center_row_1=z;
        if(center_row_2>r)
           center_row_2=r;
        if(center_row_2<z)  
           center_row_2=z; 
        for(j=center_row_1;j>1;j--)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0&&img[sample_row_1][j-2]==0)
          {
                 blackleft[i]=j;    //�ҵ���ߺڵ�
                 find_left=1;
                 center_row_1=blackleft[i]+8;
                 break;
          }
        }
        for(j=center_row_2;j<78;j++)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0&&img[sample_row_1][j+2]==0)
          {
                  blackright[i]=j;    //�ҵ��ұߺڵ�
                  find_right=1;
                  center_row_2=blackright[i]-8;
                  break;
          }      
        }  
/*************��������******************** 
          �ϰ�ʶ����
          
          ���Ż�
*************6��7��  ��֤ͨ����1�� ***********/ 
        if(find_right&&find_left&&(Z_Z_Flag==0&&Y_Y_Flag==0))
        { 
             if(!img[sample_row_1][blackleft[i]+Fxiankuan[i]]&&img[sample_row_1][blackright[i]-Fxiankuan[i]])    //��Ϊ��0���ұ�Ϊ��1��
             {
                L=L+1;                 //Ĭ��ȫ��0
                if(last_flag_Z==1||L==1)  //ȫ����1    //��һ��      �ڶ���      ������  ���Ĵ�û��
                {                           //        1           2           3         0      
                   Z_COUNT=Z_COUNT+1;                 
                } 
                last_flag_Z=1;
             }
             else
             {
                  last_flag_Z=0;         //����
                  Z_COUNT=0;             //����
                  L=0;                   //����
             }
 
             if(img[sample_row_1][blackleft[i]+Fxiankuan[i]]&& !img[sample_row_1][blackright[i]-Fxiankuan[i]])    //��Ϊ��0���ұ�Ϊ��1��
             {
                 M=M+1;                 //Ĭ��ȫ��0
                 if(last_flag_Y==1||M==1)  //ȫ����1    //��һ��      �ڶ���      ������  ���Ĵ�û��
                 {  
                    Y_COUNT=Y_COUNT+1;  
                 }
                 last_flag_Y=1;
             }
             else
             {
                  last_flag_Y=0;         //����
                  Y_COUNT=0;             //����
                  M=0;                   //����
             }
             if(Z_COUNT>2)        //
             {
                Z_Z_Flag=1;
                Z_hang=i;
                
             }
            
             if(Y_COUNT>2)        //
             {
                Y_Y_Flag=1;
                Y_hang=i;
  
             }
        }
/*************��������******************** 
          �ϰ�ʶ����
         
          ���Ż�
*************6��7��  ��֤ͨ����0�� ***********/ 
    }      
}

////////////////////���򲻿ɶ�///////////////////////////////
/*
      ���� ����10*1������ �Ľ��ĸ��ٱ��ؼ�ⷨ   ����������

      ���룺  qishi_hang         ��ʼ��
              center_point       ��ʼ�е�
      ��ֹ�У�52

      5��29��                    ���ü���
                                                              */
uint8  jiezhi_hang_2;
int16  start_1,end_1,sample_row_1;
void   DX_deal(uint8 qishi_hang,int16 center_point)
{
        uint8 i,j,k;
        int8  flag_count=0;
       // uint8 center_point=(_blackright+_blackleft)/2;
         start_1=center_point-10;
         end_1=center_point+10;
        if(start_1<0)   start_1=0;
        if(end_1>79)     end_1=79;
        valid_row_11=0;
        for(j=qishi_hang;j<50;j++)
        {
          flag_count=0;
          sample_row_1=sample_row[j];
          for(i=start_1;i<end_1;i++)
          {
            if(img[sample_row_1][i]==1&&img[sample_row_1][i+1]==0)
            {
              for(k=i;k<end_1;k++)
              {
                  if(img[sample_row_1][k]==0&&img[sample_row_1][k+1]==1)
                  {
                      if(abs_int(k-i)<=9&&abs_int(k-i)>1)
                      {
         
                             blackleft[j]=i  ;
                             blackright[j]=k ;
                             center[j]=(blackright[j]+blackleft[j])/2;
                             
                              valid_row_=j;
                              valid_row_11=j-qishi_hang;
                             
                             flag_count=1;
                             start_1=center[j]-10;
                             end_1=  center[j]+10;
                             if(start_1<0)   start_1=0;
                             if(end_1>79)     end_1=79;
                             break;
                       }  
                   }
               }
             }  
           }  //һ������
           if(flag_count==0)  break;
        }
}
/////////////////////////    ˫�߽�����       /////////////////////////////
    
                       //    ֻ���Ż�         //
                       //              //
/////////////////////////     6��20��         /////////////////////////////
int16  valid_row_sd,valid_row_0;
void SD_deal()    //ÿ���ȱ��ظ���  �ٴӱ������м��ң����߿����жϵ��ߺ�˫�ߣ����е����ҳ���
{
   int8    i,j,sample_row_1;
   int8    center_12,center_21; 
   center_12=(blackright[CS]+blackleft[CS])/2;
   center_21=(blackleft_1[0]+ blackright_1[0])/2;
   center[0]=center_21;
   for(i=CS;i>0;i--)
   {
     center[i]=(center_21-center_12)*(CS-i)/CS+center_12;
   }
   for(i=CS;i<52;i++)
   {
       blackleft[i]=0;
       blackright[i]=79;
   }
   for(i=CS;i<52;i++)       //ֻɨ�����30�е�ͼ������
   {
        sample_row_1=sample_row[i];
        for(j=0;j<78;j++)       //ֻɨ�����30�е�ͼ������
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0)
                {
                      blackright[i]=j+1;
                      break;
                }
        }
        for(j=79;j>1;j--)       //ֻɨ�����30�е�ͼ������
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0)
                {
                      blackleft[i]=j-1;
                      break;
                }
        }
        if(abs_int(blackright[i]-blackleft[i])>15)
        {
            valid_row_0=i;
            break;
        }
        else//
        {
           valid_row_0=i;
        }
        center[i]=(blackright[i]+blackleft[i])/2;
    }
}

int16 zhongzhi_hang,flag_wan,center_111;
void DS_deal()
{
   //��֪�����߽�����valid_row_11,center[valid_row_11]
   int16 center_row_1,sample_row_1,valid_row_12;
   int8  find_left=0,find_right=0,i,j,flag_=0;
   zhongzhi_hang=0;
   flag_wan=0;
   center_111=0;
   //
   for(i=valid_row_11;i<valid_row_11+8;i++)     //ȥ��
   {
       sample_row_1=sample_row[i];
       for(j=0;j<79;j++)
       {
         img[sample_row_1][j]=1;
       }
       blackleft[i]=0;
       blackright[i]=79;
   }
   center_row_1=center[valid_row_11-1];
   for(i=valid_row_11+8;i<52;i++)
   {
       sample_row_1=sample_row[i];
       for(j=center_row_1;j>1;j--)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0&&img[sample_row_1][j-2]==0)
          {
                 blackleft[i]=j;    //�ҵ���ߺڵ�
                 find_left=1;
              //   center_row_1=blackleft[i]+8;
                 break;
          }
        }
        for(j=center_row_1;j<78;j++)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0&&img[sample_row_1][j+2]==0)
          {
                  blackright[i]=j;    //�ҵ��ұߺڵ�
                  find_right=1;
                 // center_row_2=blackright[i]-8;
                  break;
          }      
        }
  //  if(flag_==0&&find_right&&find_left&&(blackright[i]-blackleft[i])<2*bxiankuan[i])
      if(flag_==0&&(blackright[i]-blackleft[i])<2*bxiankuan[i])
       {
           flag_=1;
           center_row_1=( blackright[i]+ blackleft[i])/2;
           valid_row_12=i;
           center[i]=center_row_1;
       } 
       if(flag_)
       {
           center_row_1=( blackright[i]+ blackleft[i])/2;
           center[i]=center_row_1;
           if(!find_right&&find_left)
              center[i]=blackleft[i]+bxiankuan[i]+0;
           if(find_right&&!find_left)
              center[i]=blackright[i]-bxiankuan[i]-0;
       }
   }
   for(i=valid_row_12;i>=valid_row_11;i--)   //valid_row_11,center[valid_row_11];valid_row_12,center[valid_row_12]
   {
       center[i]=(center[valid_row_11]-center[valid_row_12])*(valid_row_12-i)/(valid_row_12-valid_row_11)+center[valid_row_12];
   }
     
}
/********************����************************
                   
                     
          
****************���������ߴ���******************/
void Centerline()
{
    uint8 i;
    bad_pic=0;
    for(i=0;i<52;i++)
    {
       center[i]=0;   
       center_2[i]=0;  
       blackleft[i]=0;
       blackright[i]=79;
       if(i<6)
       {
         blackright_0[i]=79;
         blackleft_0[i]=0;
       }
    }  
    ShiBie_DX();
    if(SX_FLAG)
    {
        SX_deal();
    }
    else if(SD_FLAG)
    {
        SD_deal();
    }
    else if(DS_FLAG)   //ȫ��������ڳ��򣬲��ֿ�����  
    {
         DS_deal();  
    }
    else
    {
        bad_pic=1;//ERROE    ����     ��֡������
    }
    

}

/*************��������********************        
               ������� 
               ���ߴ���			   
	        
	    ֻ����������ߵ����
	        �ж�����
	      ����ֱ�������
	 
*************6��9��   ********************/  
int8  Y_HX_flag=0,Z_HX_flag=0,JD_Hang=0;
void HX_ZJ()              
{
    int8  YP_HX_flag=0,flag_1=0,count_y=0,count_z=0,HX_flag=0;
    uint8 i,k,m,n;
    Y_HX_flag=0;
    Z_HX_flag=0;
    JD_Hang=0;
    for(i=0;i<40;i++)
    {
       sample_row_1=sample_row[i];
       if(YP_HX_flag==0&&abs_int(blackright[i]-blackleft[i])<15&&blackleft[i]!=0&&blackright[i]!=79)
       {
	  YP_HX_flag=1;       //Ԥ��
	  JD_Hang=i;		
       }
       if(YP_HX_flag&&JD_Hang-3>0)		
       {
           for(k=JD_Hang;k>JD_Hang-5;k--)             //����ϵ������ѡ���ж� ������б�ĽǶ�
	   {
	      if(abs_int(blackright[k]-blackleft[k])<(bxiankuan[k]*2-5)&&blackleft[k]!=0&&blackright[k]!=79)
	       {
	          flag_1++;
	       }
	    }
            if(YP_HX_flag&&flag_1>=3)
	    {
	         HX_flag=1;	        
	    }
	    if(HX_flag)
	    {
		for(m=blackleft[JD_Hang];m>0;m--)
		{
		    if(img[sample_row[JD_Hang]][m]==1)
	            {
			count_z++;
		    }
		}
                for(m=blackright[JD_Hang];m<79;m++)
	        {
		    if(img[sample_row[JD_Hang]][m]==1)
		    {
			count_y++;
		    }
	        }
		if(count_y>count_z)
		{
		    Y_HX_flag=1;
		}
	        else if(count_y<count_z)
	        {
		    Z_HX_flag=1;
		}
	        else 
		{
			   //Ԥ��
	        }
           }
          YP_HX_flag=0;       //Ԥ�к��߱�־λ����
      }	
      if(YP_HX_flag&&JD_Hang-5<0)		
      {

        for(n=15;n<50;n++)   //18
        {
              if(center[n+1]-center[n]<4)
              {
                  center[n-15]=center[n];
              }
              else  break;
        }  
      }
   }
   if(Y_HX_flag)
   {
     for(i=0;i<40;i++)
     { 
         center[i]=blackleft[i]+bxiankuan[i]+4;
         if(center[i]>79)  center[i]=79;
     }  
     for(i=0;i<40;i++)
     {
         if(abs_int(center[i+1]-center[i])>4)
         {
            center[i+1]=2*center[i]-center[i-1];
         }
     }
   }
}

/*************��������********************        
      ������ʮ����Ͳ���бʮ�ֲ���
          ���Ż�бʮ�ִ���
          ���Ż�ʮ���϶����²���
*************3��13��  ��֤ͨ�� ***********/  
int8 shizhi_flag=0; 
uint8  qishi_flag=0,jiezhi_flag=0;  
uint8 qishi=0,jiezhi=0;
int8  zx=0,yx=0,zs=0,ys=0; 

uint8 h=0;

uint8 xieshizi_zuoflag=0,xieshizi_youflag=0,youxiaozuoxieshizi=0,youxiaoyouxieshizi=0,xieyx=0,xieys=0,xiezs=0,xiezx=0;

uint8  zuoqishi;
void shizi_handle()
{   
    zx=0;yx=0;zs=0;ys=0; 
    qishi_flag=0;
    jiezhi_flag=0; 
    qishi=0;
    jiezhi=0;
    uint8  sample_row_1,sample_row_2,sample_row_3;
    
    uint8  jinru_flag=0,z_flag=0, x_flag=0,zx_flag=0,yx_flag=0;
    int16  aa,bb,cc,dd;
    int8   i;
    int8   zs_flag=0, ys_flag=0;
    
    shizhi_flag=0;  
    for(i=0;i<50;i++)                                    //1
    {
       sample_row_1=sample_row[i];
       sample_row_2=sample_row[i+1];
       sample_row_3=sample_row[i+2];
       if(qishi_flag==0&& (blackleft[i]==z)&&(blackright[i]==r)&&
                   (blackleft[i+1]==z)&&(blackright[i+1]==r)&&
                   (blackleft[i+2]==z)&&(blackright[i+2]==r)&&
                   img[sample_row_1][z]==white&&img[sample_row_1][r]==white&&
                   img[sample_row_2][z]==white&&img[sample_row_2][r]==white)
       {
         qishi=i;                    //����ʮ�ֿ�ʼ��
         qishi_flag=1;
       }
       if(  jiezhi_flag==0&&qishi_flag!=0&&
              blackleft[i]==z &&blackright[i]==r&&
            blackleft[i+1]==z &&blackright[i+1]==r&&
           img[sample_row_1][z+5]==white&&img[sample_row_1][r-5]==white&&
           img[sample_row_1][z+5]==white&&img[sample_row_2][r-5]==white&&
          (img[sample_row_3][z+5]==black||img[sample_row_3][r-5]==black||
            blackright[i+2]<r||blackleft[i+2]>z))
         {                                                                         //��ʵǰ�������͹��ˣ�����������Ϊ�˱��ղżӵ�
            jiezhi=i+1;                  //����ʮ�ֽ�����
            jiezhi_flag=1;
            break;//������ʡʱ��
         }   
     }
     if(jiezhi-qishi>3)  shizhi_flag=1;          // 2
     if( shizhi_flag)                            //ֻ��ʮ�ֲ��ܽ���
     {
        for(i=0;i<=qishi+2;i++)                     //���¶˹յ�
        {         
            aa=blackleft[i+2]-blackleft[i];
            bb=blackleft[i+2]-blackleft[i+4];
            cc=blackright[i+2]-blackright[i];
            dd=blackright[i+2]-blackright[i+4];
            if(aa>=0&&bb>0&&abs_int(bb)>3&&blackleft[i+2]>z)
                zx=i+2;                       //����
            if(cc<=0&&dd<0&&abs_int(dd)>3&&blackright[i+2]<r)
                yx=i+2;                         //����     
         }
        //4��29 ��������
         if(shizhi_flag)
         {
             uint8 b=0;
              h=0;
             uint8 a=0;
             uint8 m=0;
             int8 count_9=0;
             for(m=15;m<65;m++)
             {
                   for(uint8 k=qishi;k<52;k++)
                   {
                       uint8 sample_row_4=sample_row[k];
                       uint8 sample_row_5=sample_row[k+1];
                       if(img[sample_row_4][m]==1&&img[sample_row_5][m]==1)
                       {
                            a=k+1;
                       }
                       else break;
                   }
                   if(a>b)
                   {
                              b=a;
                              h=m;
                   }
             }
             for(uint8 k=qishi;k<52;k++)
             {
                uint8 sample_row_4=sample_row[k];
                for(uint8 l=h;l<79;l++)  
                {
                   if(img[sample_row_4][l]==1&&img[sample_row_4][l+1]==0)
                   {
                     blackright[k]=l;
                     break;
                   }
                }
                for(uint8 l=h;l>0 ;l--)
                {
                   if(img[sample_row_4][l]==1&&img[sample_row_4][l-1]==0)
                   {
                     blackleft[k]=l;
                     break;
                   }
                }
                if(blackleft[k]>0&&blackright[k]<79)
                {
                  count_9=count_9+1;
                  if(count_9>=3)
                  {
                    h=(blackleft[k]+ blackright[k])/2;  
                  }  
                }
             }
         }
        //
         for(i=jiezhi-2;i<52;i++)                     //���¶˹յ�
         {  
            if(i<=49)
            { 
              aa=blackleft[i]-blackleft[i-2];
              bb=blackleft[i]-blackleft[i+2];
              cc=blackright[i]-blackright[i-2];
              dd=blackright[i]-blackright[i+2];  
              if(aa>0&&bb<=0&&abs_int(aa)>3&&abs_int(bb)<=2&&blackleft[i]>z)
              { zs=i;zs_flag=1;}                       //����       
              if(cc<0&&dd>=0&&abs_int(cc)>3&&abs_int(dd)<=2&&blackright[i]<r)
              { ys=i;ys_flag=1;}                      //����
            }
            if(zs_flag==0&&i>49&&blackleft[49]!=0&&blackleft[50]!=0&&blackleft[51]!=0) //�������
            {
                  zs=max(blackleft[49],blackleft[50],blackleft[51]);          
            }
            if(ys_flag==0&&i>49&&blackright[49]!=r&&blackright[50]!=r&&blackright[51]!=r)
            {
                  ys=max(blackright[49],blackright[50],blackright[51]);              
            }
         }
         if(zx==0&&yx==0&&ys!=0&&zs!=0&&zs>jiezhi&&ys>jiezhi)
                jinru_flag=1;                 //����ʮ��
         if(zx!=0&&yx!=0&&ys!=0&&zs!=0&&zs>jiezhi&&ys>jiezhi&&zx<qishi&&yx<qishi)
                 z_flag=1;                    //��ʮ��
         if(zx!=0&&yx!=0&&(ys!=0||zs!=0))     //бʮ��  
                 x_flag=1;  
         if(zx!=0&&zs!=0&&yx==0)
             zx_flag=1;
         if(ys!=0&&yx!=0&&zx==0)
             yx_flag=1;
     }
     if(zx_flag)
     {
         for(i=zs;i>zx-1;i--)
         {
           blackleft[i-1]=((blackleft[zs]-blackleft[zx-1])*(zs-i))/(zx-1-zs)+blackleft[zs];//ע��˳������յ����������� 
           center[i-1]= blackleft[i-1]+bxiankuan[i-1];       
         }
     }
     if(yx_flag)
     {
         for(i=ys+1;i>yx-1;i--)
         {
           blackright[i-1]=(blackright[yx-1]-blackright[ys+1])*(ys+1-i)/(ys+1-yx+1)+blackright[ys+1];//ע��˳������յ����������� 
           center[i-1]= blackright[i-1]-bxiankuan[i-1];
         } 
     }
     if(z_flag||x_flag)                       //��ʮ�ֲ���   
     {   
         for(i=zs;i>zx-1;i--)
         {
           blackleft[i-1]=((blackleft[zs]-blackleft[zx-1])*(zs-i))/(zx-1-zs)+blackleft[zs];//ע��˳������յ����������� 
         }
         for(i=ys+1;i>yx-1;i--)
         {
           blackright[i-1]=(blackright[yx-1]-blackright[ys+1])*(ys+1-i)/(ys+1-yx+1)+blackright[ys+1];//ע��˳������յ����������� 
         } 
     }
     if(jinru_flag)                           //����ʮ�ֲ���      
     {
         for(i=zs+1;i>=0;i--)
         {
          // blackleft[i-1]=-blackleft[zs+1]*(zs+1-i)/(zs+1)+blackleft[zs+1];
             blackleft[i-1]=blackleft[zs+1];
         }
         for(i=ys+1;i>=0;i--)
         {
          // blackright[i-1]=(r-blackright[ys+1])*(ys+1-i)/(ys+1)+blackright[ys+1];
             blackright[i-1]=blackright[ys+1];
         } 
     } 
}   //ʮ�ֲ��߽���





/*************ֱ�ǳ���**************************        
       ʶ�����
       ���ò��߷���
       ����
      
*************  ��֤ͨ����1�� ***************/  

int8  y_90_flag=0,Cont=0,Trg=0,ZJ_Process_flag=0,ReadData=0, flag_0=0,flag_0_0;
int8  count_flag_0=0,flag_1=0,flag_2=0,  y_flag=0 , z_flag=0;
int8  D1,y,jiezhi_hang_1,jiezhi_hang_0,z_1;
int8  zj_flag=0,zj_flag_1=0,flag_z=0,y_z,z_z,count_flag=0;
extern int8 flag_00;
void _90_ZJ()
{

//�����ֵ��   1482
//�����  :    1700
//�Ҵ�����     1265
   int8 i,k;
   uint8  C0,jiezhi_flag_1=0,jiezhi_flag_2=0,sample_row_1,sample_row_2;
   
   
   zj_flag=0 ;
   jiezhi_hang_1=0;
   jiezhi_hang_0=0;
   D1=0;
   y_flag=0; 
   z_flag=0;
   zj_flag_1=0;
   flag_1=0;     
   y=0;
   z_1=0;
                                   //û��    ��һ�μ��   �ڶ��μ��    �����μ��     û��   �ڶ���û��
   ReadData=0x01^heixian_flag_0;   // 0       1            1             1              0      0
   Trg=ReadData&(ReadData^Cont);   // 0       1            0             0              0      0 
   Cont=ReadData;                  // 0       1            1             1              0      0
   if(Trg==1)
   {
        count_flag= count_flag+1;
        ZJ_Process_flag=0x01^ZJ_Process_flag;         //��ת��־λ
        if(count_flag%2==0)
        {
          y_z=0;
          z_z=0;
          flag_z=0;
          z_zj_flag=0;
          y_zj_flag=0;
          flag_00=0;
        }
   }
   if(flag_z==0&&ZJ_Process_flag&&Cont==0)
   {
      yupan_zy();
      if(y_zj_flag)  
      {
         y_z=1;
         flag_z=1;
      }
      if(z_zj_flag)
      {
         z_z=1;
         flag_z=1;
      }
   }

   if(y_z&&ZJ_Process_flag&&Cont==0)
   {
     for(i=0;i<50;i++)
     {     
       if(y_zj_flag&&(blackleft[i]!=0&&blackright[i]==79&&
          blackleft[i+1]!=0&&blackright[i+1]==79&&
            blackleft[i+2]!=0&&blackright[i+2]==79)||(blackleft[i]==0&&blackright[i]==79))  //��ֱ��
        {
            y=i;  
            y_flag=1;                    //�ж�����  ����Ԥ�к���
            if(y<num_00)                 //�޸�ϵ������  �ı���λ��36
            {
                     
                for(k=y;k<50;k++)
                { 
                   sample_row_1=sample_row[k];
                   sample_row_2=sample_row[k+1];
                   if( jiezhi_flag_1==0&&img[sample_row_1][79]==1&&img[sample_row_2][79]==0)
		   {
			jiezhi_hang_1=k;
		        jiezhi_flag_1=1;
                        zj_flag=1;
                        break;
	           }
                }
           }
         }
       }
       if(zj_flag)
       {
            D1=jiezhi_hang_1;
            C0=(blackright[0]+blackleft[0])/2;
            if(y-3<0)  y=3;
            for(i=D1;i>0;i--)
            {
              center[i]=79-(79-C0)*(D1-i)/D1;
              if(center[i]>79) center[i]=79;
            }
        }
    }
   
   if(z_z&&ZJ_Process_flag&&Cont==0)
   {
     for(i=0;i<50;i++)
     {     
       if(z_zj_flag&&(blackleft[i]==0&&blackright[i]!=79&&
          blackleft[i+1]==0&&blackright[i+1]!=79&&
            blackleft[i+2]==0&&blackright[i+2]!=79)||(blackleft[i]==0&&blackright[i]==79))  //��ֱ��
        {
            z_1=i;  
            z_flag=1;                    //�ж�����  ����Ԥ�к���
            if(z_1<num_11)              //�޸�ϵ������  �ı���λ��  40
            {
                     
                for(k=z_1;k<=50;k++)
                { 
                   sample_row_1=sample_row[k];
                   sample_row_2=sample_row[k+1];
                   if( jiezhi_flag_2==0&&img[sample_row_1][0]==1&&img[sample_row_2][0]==0)
		   {
			jiezhi_hang_0=k;
		        jiezhi_flag_2=1;
                        zj_flag_1=1;
                        break;
	           }
                }
           }
         }
       }
       if(zj_flag_1)
       {
            D1=jiezhi_hang_0;
            C0=(blackright[0]+blackleft[0])/2;
            if(z_1-3<0)  z_1=3;
            for(i=D1;i>0;i--)
            {
              center[i]=0+(C0-0)*(D1-i)/D1;
              if(center[i]<0) center[i]=0;
            }
        }
    }

}


int8 heixian=0, heixian_flag=0,q_flag=0,q_flag_1=0;
void  HX_D()
{
   int8 q_hang,i,j,count=0;
   heixian=0;
   heixian_flag=0;
   q_flag=0;
   q_flag_1=0;
   for(i=0;i<40;i++)
   {
     if(q_flag==0&&abs_int(blackright[i]-blackleft[i])<15&&blackright[i]!=79&&blackleft[i]!=0&&blackright[i+2]==79&&(abs_int(blackright[i]-blackright[i-3])<4))
     {
         q_hang=i-1;
         q_flag=1;  
     }
     if(q_flag_1==0&&abs_int(blackright[i]-blackleft[i])<15&&blackright[i]!=79&&blackleft[i]!=0&&blackleft[i+2]==0&&(abs_int(blackleft[i]-blackleft[i-3])<4))
     {
         q_hang=i-1;
         q_flag_1=1;  
     }
     if(i<6)
     {
        count=0;
        for(j=0;j<79;j++)
        {
          if(img[sample_row[i]][j]==0)
          {
             count++;
          }
        }
        if(count>75)
        {
            heixian++;
        } 
        if(heixian>1)
        {
            if(blackright[20]!=79&&blackleft[20]!=0&&blackright[21]!=79&&blackleft[21]!=0&&
               blackright[22]!=79&&blackleft[22]!=0&&blackright[23]!=79&&blackleft[23]!=0)
            {
              for(j=20;j>=0;j--)
              {
                  center[j]=center[20];
              }
            }
        }
     }
   }
   if(q_flag)
   {
      
      for(i=q_hang;i<q_hang+10;i++)
      {
         blackright[i]= 2*blackright[i-3]-blackright[i-6];
      }
      for(i=0;i<q_hang+10;i++)
      {  
         center[i]=blackright[i]-bxiankuan[i];
         if(center[i]<0)  
         {
           for(j=i;j<=51;j++)
               center[j]=0;
         }
      }
   }
   if(q_flag_1)
   {
     
      for(i=q_hang;i<q_hang+10;i++)
      {
         blackleft[i]= 2*blackleft[i-2]-blackleft[i-4];
      }
      for(i=0;i<q_hang+10;i++)
      {  
         center[i]=blackleft[i]+bxiankuan[i];
         if(center[i]>79)  
         {
           for(j=i;j<=51;j++)
                center[j]=79;
         }
      }
   }
}

void _lvbo()
{
   uint8 i;
   for(i=0;i<52-2;i++)
   {
     blackleft[i]=MiddleFilter (blackleft[i],blackleft[i+1],blackleft[i+2]);
     blackright[i]=MiddleFilter( blackright[i], blackright[i+1], blackright[i+2]);
   } 
}


/*************��������**************************        
       ��Ҫ��90�Ⱦ�ͷһ����ȫ���߽��в��ߣ�
           ��Զ����в���
       ���Խ��ζ���
       ���Ż�����ʶ��
*************3��13��  ��֤ͨ�� ***************/  
//uint8  z_yi=0,z_zi=0,y_yi=0,y_zi=0;
uint8  youxiaozuobian=0,youxiaoyoubian=0;
int8   jinduan_Y=0,jinduan_Z=0;
uint8  wandao_flag,inwandao_flag,s_zhidao_flag, zong,valid_row;
void  nihe_center()                    //�������
{
    uint8  i,j,m;
    uint8   sample_row_2, sample_row_3;
    uint8   youbian_Y,zoubian_Z;
       
    jinduan_Y=0;
    jinduan_Z=0;

    youxiaozuobian=0;
    youxiaoyoubian=0;
    uint8  z_yi=0,z_zi=0,y_yi=0,y_zi=0;

    s_zhidao_flag=0;
    inwandao_flag=0;
    wandao_flag=0;
    valid_row=0;
    
    for(i=0;i<52-1;i++)
    {
      sample_row_1=sample_row[i];
      if(center[i]==0)
      {
        if(blackleft[i]>0&&blackright[i]<r&&center[i]==0)
        {
          center[i]=(blackleft[i]+blackright[i])/2;
        }
        else if(blackleft[i]!=0&&blackright[i]>r-2&&center[i]==0)  //�ұ߶���
        {
           if(y_yi==0)
               y_yi=i;
           if(y_zi==0)
           {
             for(j=y_yi;j<52;j++)
             {
                sample_row_2=sample_row[j];
                sample_row_3=sample_row[j+1];
                if((blackright[j]!=r&&blackright[j+1]!=r&&blackright[j+2]!=r)||
                (img[sample_row_2][blackright[j]-1]==1&&img[sample_row_3][blackright[j+1]-1]==0))
                {
                  y_zi=j;
                  break;
                }
                else
                {
                  y_zi=j;
                }
             }
           }
           if(youxiaozuobian==0)
           {
               for(m=y_yi;m<y_zi+5;m++)//�ж����������ԣ���ȷ����������
               {
                  if((blackleft[m+1]-blackleft[m])<0||blackleft[m]>=(r-1))
                  {//���������ǵ������ҵģ����������ߵݼ��˻��ܵ��ұ����˾ͽ��룬
                      youxiaozuobian=m;
                      break;
                  }
                   else if((blackleft[m+1]-blackleft[m])>=0||blackleft[m]<(r-1))
                       youxiaozuobian=m;//����ʱһֱ���´�ֵ
               }
           }
           if(y_yi==0||y_yi==1||(y_yi==2))//�����������Ӧ���ǽ��˶��ߣ����ж���
           {
             if(y_yi<3&&jinduan_Y==0)
             {
               for(int8 as=y_zi+2;as<52;as++)
               {
                  if((blackright[as+1]-blackright[as])<=0&&blackright[as]<r)
                  {//���ϵݼ� 
                     youbian_Y=as;
                     if(youbian_Y>6&&(youbian_Y-y_zi)>6)//��֤�Ƿ�ȷʵ�ǽ��˶���  ��ֵӦ�붪�ߴ���Զ
                     {
                        for(int8 as=y_zi;as>=0;as--)//�ܽ���������ȷ��ȷʵ���˶�����
                        {
                          if(center[as]==0)
                          {
                             jinduan_Y=1;//��λ��־  //����ǰһ�е�����������е�����
                             center[as]=(blackleft[as+1]+blackright[as+1])/2-blackleft[as+1]+blackleft[as]+0;
                             if(center[as]>=r) center[as]=r;
                          }
                        }
                     }
                      if(jinduan_Y==1)  break;//���������������
                  }
                  else break;//��Ϊ���뼱�䣬�������
                }
              }
               if(jinduan_Y==0&&y_yi<=3&&y_zi>=8&&(youxiaozuobian>=y_yi))//������ǽ��˶��ߣ����ǽ��뼱��  ��
               {//��������ͷ�ľ�ͷ��120�����ϣ�90�ȵ����׽���˴�
                 inwandao_flag=1;
                 for(int as=0;as<y_zi;as++)
                 {
                    valid_row=y_zi; 
                    if(blackleft[as]!=0)
                    {
                       center[as]=blackleft[as]+bxiankuan[as]+4;//�ı�ϵ��1�Ĵ�С���ɸ��ƽ��뼱���ת������
                       if(center[as]>=r)
                       { 
                          center[as]=r;
                          valid_row=as;     
                          break;
                       }
                    }
                 }
               } 
           }
            else  if(center[i]==0&&blackleft[i]!=0)//Զ��  �������ʱ 
            {
              center[i]=center[i-2]-blackleft[i-2]+blackleft[i]+1;//�ı�ϵ��4�Ĵ�С���ɸ��ƽ��뼱���ת������
              if(center[i]>=r)  center[i]=r;
            }
        }
       
        else if(blackleft[i]<2&&blackright[i]!=r&&center[i]==0)  //��߶���
        {
           if(z_yi==0)
              z_yi=i;
           if(z_zi==0)
           {
             for(j=z_yi;j<52-1;j++)
             {
                sample_row_2=sample_row[j];
                sample_row_3=sample_row[j+1];
                if((blackleft[j]!=0&&blackleft[j+1]!=0&&blackleft[j+2]!=0)||
                img[sample_row_2][blackleft[j]+1]==1&&img[sample_row_3][blackleft[j+1]+1]==0)
                {
                  z_zi=j;
                  break;
                }
                else
                {
                   z_zi=j;
                }
             }
           }
           if(youxiaoyoubian==0)
           {
               for(m=z_yi;m<z_zi+5;m++)//�ж����������ԣ���ȷ����������
               {
                  if((blackright[m+1]-blackright[m])>0||blackright[m]<=0)
                  {//���������ǵ������ҵģ����������ߵݼ��˻��ܵ��ұ����˾ͽ��룬
                      youxiaoyoubian=m;
                      break;
                  }
                   else if((blackright[m+1]-blackright[m])<=0||blackright[m]>0)
                       youxiaoyoubian=m;//����ʱһֱ���´�ֵ
               }
           }
           if(z_yi==0||z_yi==1||(z_yi==2))//�����������Ӧ���ǽ��˶��ߣ����ж���
           {
             if(z_yi<3&&jinduan_Z==0)
             {
               for(uint8 as=z_zi+2;as<52;as++)
               {
                  if((blackleft[as+1]-blackleft[as])>=0&&blackleft[as]>0)
                  {//���ϵݼ� 
                     zoubian_Z=as;
                     if(zoubian_Z>6&&(zoubian_Z-z_zi)>6)//��֤�Ƿ�ȷʵ�ǽ��˶���  ��ֵӦ�붪�ߴ���Զ
                     {
                        for(int as=z_zi;as>=0;as--)//�ܽ���������ȷ��ȷʵ���˶�����
                        {
                          if(center[as]==0)
                          {
                             jinduan_Z=1;//��λ��־  //����ǰһ�е�����������е�����
                             center[as]=-(blackright[as+1]-(blackleft[as+1]+blackright[as+1])/2)+blackright[as];
                             if(center[as]<0)  center[as]=0;
                          }
                        }
                     }
                      if(jinduan_Z==1)  break;//���������������
                  }
                  else break;//��Ϊ���뼱�䣬�������
                }
               }
               if(jinduan_Z==0&&z_yi<3&&z_zi>8&&youxiaoyoubian>=z_yi)//������ǽ��˶��ߣ����ǽ��뼱��  ��
               {//��������ͷ�ľ�ͷ��120�����ϣ�90�ȵ����׽���˴�
                 inwandao_flag=1;
                 for(int as=0;as<z_zi;as++)
                 {
                    valid_row=z_zi; 
                    if(blackright[as]!=r)
                    {
                       center[as]=blackright[as]-bxiankuan[as]-4;//�ı�ϵ��1�Ĵ�С���ɸ��ƽ��뼱���ת������
                       if(center[as]<0)
                       {
                          valid_row=as; 
                          center[as]=0;
                          break;
                       }  
                    }
                 }
               } 
            }
            else  if(center[i]==0&&blackright[i]!=r)//Զ��  �������ʱ 
            { 
              center[i]=-(blackright[i-2]-center[i-2])+blackright[i]-1;//�ı�ϵ��4�Ĵ�С���ɸ��ƽ��뼱���ת������
              if(center[i]<0)  center[i]=0;
            }
        }
       else if(center[i]==0)//���������ֱ��ȡ��ֵ
       {
         center[i]=(blackright[i]+blackleft[i])/2;  
       }
     }
   }
}


////////////////������Ч��////////////////
int8 youwan_flag=0,zuowan_flag=0;
void  youxiao_hang_1()
{
    int8 i;
    int16 right=0,left=0;
    youwan_flag=0;zuowan_flag=0;
    for(i=0;i<52-1;i++)
    {
      if(abs_int(center[i+1]-center[i])>5||center[i]==0||center[i]==79)   
      {
           valid_row_=i;
           break;
      }
      else
      {
           valid_row_=i;     
      }
      if(SD_FLAG)
      {
        valid_row_=valid_row_0;
      }
      
    }
    for(i=0;i<valid_row_;i++)
    {
       if(center[i+2]-center[i]>0)
          right++;
       if(center[i+2]-center[i]<0)
          left++;
       if(right>(int16)((right+left)*0.75))
       {
           youwan_flag=1;
       }
       if(left>(int16)((right+left)*0.75))
       {
           zuowan_flag=1;  
       }
    }
    if(zuowan_flag)
    {
      //
    }
    if(youwan_flag)
    {
      //
    }
}
/*************��������********************        
          ��Ч�еļ��㻹������
          
          ���Ż� 
*************3��15��  ��֤ͨ�� ***********/  

int16 error_3,bad_flag;
void youxiao_hang()
{
  uint8 i;
  error_3=0;
  s_zhidao_flag=0;
  bad_flag=0;
  uint8 you_flag=0,zuo_flag=0,j;
  if(center[5]-center[0]>1)
  {
        you_flag=1;
  }
  else  if(center[5]-center[0]<-1)
  {
         zuo_flag=1;
  }
  else  
  {
        for(i=0;i<52-1;i++)
        {
                        //if(abs_int(center[i+1]-center[i])>4||center[i+1]==0||center[i+1]==79)
           if(abs_int(center[i+1]-center[i])>5&&abs_int(center[i+2]-center[i])>5)   
           {
                if(center[i+1]<=2||center[i+1]>=78)  
                {
                    valid_row_=i-1;
                    break;
                 }
                 valid_row_=i;
                 break;
            }
            else valid_row_=i;   
         }
             
   }
  
   if(you_flag)
   {
      for(j=0;j<52-4;j++)
      {            
          if(center[j+2]-center[j]>=0&&center[j]!=0&&center[j]!=79)   //
          {       
                valid_row_=j+1;
          }
           else    break;
      }  
    }
    else if(zuo_flag)   
    {
       for(j=0;j<52-2;j++)
       {
           if(center[j+2]-center[j]<=0&&center[j]!=0&&center[j]!=79)   //
           {       
                valid_row_=j+1; 
           }
           else    break ;
       } 
    } 
    else
    {
           //Ԥ��
    }
       
       
  if(q_flag_1||q_flag)
  {
    valid_row_=50;
  }   
  if(ZJ_Process_flag)
  {
      for(j=0;j<52-2;j++)
      {            
          if(abs_int(center[j+1]-center[j])<=8&&center[j+1]!=0&center[j+1]!=79)   
          {       
                valid_row_=j+1;
          }
          else    break;
      }  
       
  }
       
  //********************************
  
  error_3=abs_int(center[valid_row_-1]-center[1]);
  if(valid_row_>=45)
  {
     s_zhidao_flag=1;
  }
  if(inwandao_flag)
  {
     valid_row_=valid_row;
  }
  if(valid_row_<8||valid_row_>52)
  {
     valid_row_=10;
     bad_flag=1;
  }
  
  

  
/*********�ϰ��ж� 6��8��*************/
  if(Z_hang>valid_row_)
  {
       Z_Z_Flag=0;
  }
  if(Y_hang>valid_row_)
  {
       Y_Y_Flag=0;
  }
/*********�ϰ��ж� 6��8��*************/ 
}

/*********�ϰ����� 6��8��************* 
         
*********�ϰ����� 6��8��*************/ 
void  Z_A_deal()
{ 
     int8 i,j,h,m;
     if(Z_Z_Flag)    //���ϰ�   ���߿϶�����
     {
        for(i=blackright[Z_hang]-5;i>0;i--)
        {
          if(img[sample_row[Z_hang]][i]==1&&img[sample_row[Z_hang]][i-1]==0)
          {
            if(blackright[Z_hang]-i<2*bxiankuan[Z_hang]-3)
            {
              blackleft[Z_hang]=i;
              break;
            }
          }
        }
        for(h=0;h<valid_row_;h++)
        {
          center[h]=(blackleft[Z_hang]+blackright[Z_hang])/2+3;  //2
        }
     }
     if(Y_Y_Flag)    //���ϰ�   ���߿϶�����
     {
        for(j=blackleft[Y_hang]+5;j<78;j++)
        {
          if(img[sample_row[Y_hang]][j]==1&&img[sample_row[Y_hang]][j+1]==0)
          {
            if(j-blackleft[Y_hang]<2*bxiankuan[Y_hang]-3)
            {
              blackright[Y_hang]=j;
              break;
            }
          }
        }
        for(m=0;m<valid_row_;m++)
        {
          center[m]=(blackleft[Y_hang]+blackright[Y_hang])/2-3;//2
        }    
     }
}


/////////////////////////////////////////////////////////////////////
void pinghua()
{
  uint8 i;
  for(i=0;i<52-2;i++)
     center[i]=MiddleFilter(center[i],center[i+1],center[i+2]);
}

  float  sum=0.0;
  float CenterEq=0.0;
  float  error=0;
  float error1=0;
  float  error_4=0;
  volatile float error_s=0;
  
  int8  last_hx_flag=0;
void angle_pd()
{
   sum=0.0;
   int16 xianzhong=0;
     error_s=0;
   uint8  i,center_value=40;
   for(i=0;i<valid_row_;i++)//�˲�����м�Ȩƽ����
   {
        sum+=(center[i]-center_value)*quanzhi[i];
        xianzhong+=quanzhi[i];                  //����
   }
   error=((center[valid_row_-1]+center[valid_row_-2])/2.0-(center[1]+center[2])/2.0)/(valid_row_*1.0-2.0);    //��Զ�������ƫ��
  
   error_4=(center[valid_row_-1]+center[valid_row_-2])/2.0-(center[1]+center[2])/2.0;//�����ж�
 
   CenterEq=sum/xianzhong;                      //�õ�ƽ��kֵ  
   error_s=(CenterEq-error1)*10.0;
   error1=CenterEq; 
   if(inwandao_flag)
   {
       angle_pwm=(uint32)(DJ_cen -CenterEq*P_1-error*D_1- error_s*S_1);
   }
   else if (s_zhidao_flag)
   {
       angle_pwm=(uint32)(DJ_cen-CenterEq*P_2-error*D_2- error_s*S_2);
   }
   else if(bad_flag&&!DX_FLAG&&!DS_FLAG)
   {
       angle_pwm=last_angle;
   }   
   else 
   {
       angle_pwm=(uint32)(DJ_cen-CenterEq*P_3-error*D_3- error_s*S_3);
   } 
   if(heixian_flag)
   {
        angle_pwm=DJ_cen;
   }
 //  if((last_hx_flag==4||last_hx_flag==2)&&SX_FLAG)  //
 //  {
 //     angle_pwm=last_angle;
 //      DELAY_MS(10);
 //  }
   last_angle=angle_pwm;                // 
}

//////////// ����PD/////////////


int16 error_00;
int16 error_01;
int32 last_angle_1;
void angle_pd_1()
{
   int8 i;
   int16 num_0,center_ex;
   error_00=0;
   if(valid_row_>8)
   {
       for(i=0;i<8;i++)
       {
            num_0=num_0+center[i];
       }
       center_ex=num_0/8;
       error_00=abs_int(center_ex-39);
       
       sum=0.0;
       int16 xianzhong=0;
       error_s=0;
       int16  i,center_value=40;
       for(i=0;i<=valid_row_;i++)//�˲�����м�Ȩƽ����
       {
            sum+=(center[i]-center_value)*quanzhi_1[i];
            xianzhong+=quanzhi_1[i];                  //����
       }
       error=((center[valid_row_-1]+center[valid_row_])/2.0-(center[1]+center[0])/2.0)/(valid_row_*1.0-2.0);    //��Զ�������ƫ��
       error_01=(center[valid_row_-1]+center[valid_row_])/2-(center[1]+center[0])/2;
        
       error_4=(center[valid_row_-1]+center[valid_row_-2])/2.0-(center[1]+center[2])/2.0;//�����ж�

       CenterEq=sum/xianzhong;                      //�õ�ƽ��kֵ  
       error_s=(CenterEq-error1)*10.0;
       error1=CenterEq; 
       if(error_00<10)
       {
            angle_pwm=(uint32)(DJ_cen-CenterEq*DP_1-error*DD_1*10.0- error_s*DS_1);
       }
       else if(error_00<25)
       {
            angle_pwm=(uint32)(DJ_cen-CenterEq*DP_2-error*DD_2*10.0- error_s*DS_2);
       }
       else 
       {
           if(zuowan_flag)
           {
               angle_pwm=DJ_zuo;
           }
           if(youwan_flag)
           {
               angle_pwm=DJ_you;
           }
       }
       if(abs_int(error_01)<10&&error_00<10)
       {
          angle_pwm=(uint32)(DJ_cen-CenterEq*DP_3-error*DD_3- error_s*DS_3);
       }
   }
   else
   {
       angle_pwm=last_angle_1;
   }
   last_angle_1=angle_pwm;
}
/*************�»��ߺ���********************        
          ������
          ע�ⲻ��ȫ����ʾ ��ֻ����ʾ�ֱ��ʴ�С��ͼ������
          ���Ż� 
          shuzu:Ҫ��ʾ������
         point_num:Ҫ��ʾ�������
         rgb565����ɫ
******************************************************/

void LCD_points (int16 *shuzu,uint32 point_num, uint16 rgb565)                
{
     uint8 i;
     Site_t site[52];
     for(i=0;i<point_num;i++)
     {
        // site[i].x=sample_row[i];
        // site[i].y=shuzu[i];
           site[i].y=sample_row[i];
           site[i].x=shuzu[i];
     }
    while(point_num--)
    {
        LCD_point(site[point_num],rgb565) ;             
    }

}
void LCD_point_s ()          //����          
{
     uint8 i,point_num=60;
     Site_t site[60];
     for(i=0;i<60;i++)
     {
           site[i].y=i;
           site[i].x=79;
     }
    while(point_num--)
    {
        LCD_point(site[point_num],BLUE) ;             
    }

}
void LCD_point_s_s ()       //����         
{
     uint8 i,point_num=80;
     Site_t  site[80];
     for(i=0;i<80;i++)
     {
           site[i].y=59;
           site[i].x=i;
     }
    while(point_num--)
    {
        LCD_point(site[point_num],BLUE);             
    }

}
void DISPLAY()
{
  
    uint32 point_num=52;
                              //д�ڴ�
    LCD_point_s ();
    LCD_point_s_s ();
    LCD_points (center, point_num,WHITE);
    LCD_points (blackleft, point_num,BLUE);
    LCD_points (blackright, point_num,BLUE);
   

}
extern int8 qipao_kaishi;
void DISPLAY_1()
{
    uint32 point_num=52;
    LCD_points (center, point_num,RED);        //����
    LCD_points (blackleft, point_num,RED);     //����
    LCD_points (blackright, point_num,RED);    //����
   
      Site_t site = {0,110};   //x = 10 ,y = 20
      LCD_str(site,"z", BLUE,RED);
      Site_t site_1 = {25,110};   //x = 10 ,y = 20
      LCD_num_BC(site_1,val_z,5, BLUE,RED);
      
      Site_t site_2 = {0,90};  
      LCD_str(site_2,"y:",  BLUE,RED);
      
      Site_t site_2_1 = {25,90};   //x = 10 ,y = 20
      LCD_num_BC(site_2_1,val_y,5,  BLUE,RED);
      
      Site_t site_3 = {0,70};  
      LCD_str(site_3,"H",  BLUE,RED);
      
      Site_t site_3_1 = {25,70};   //x = 10 ,y = 20
      LCD_num_BC(site_3_1,valid_row_,2, BLUE,RED);
 
      Site_t site_4 = {50,90};  
      LCD_str(site_4,"SD:",  BLUE,RED);
      
      Site_t site_4_1 = {75,90};  
      LCD_num_BC(site_4_1,SD_FLAG,2,  BLUE,RED);

      
      Site_t site_5 = {50,70};  
      LCD_str(site_5,"2:", BLUE,RED);
      
      Site_t site_5_1 = {75,70};
      LCD_num_BC(site_5_1,q_flag,2,BLUE,RED);
 
      Site_t site_6 = {50,110};  
      LCD_str(site_6,"1:",BLUE,RED);
      
      Site_t site_6_1 = {75,110};
      LCD_num_BC(site_6_1,q_flag_1,2,BLUE,RED);

}

void store_data()
{
     if(SX_FLAG)
     {
        last_hx_flag=1;
     }
     else if(DX_FLAG)
     {
        last_hx_flag=2;
     }
     else if(SD_FLAG)
     {
        last_hx_flag=3;
     }
     else if(DS_FLAG)
     {
        last_hx_flag=4;
     }
     else
     {
        last_hx_flag=5;
     }
       
}
/**********************************************            
                  ������



 

************************************************/

void  PROCESS()
{
    DISPLAY_1();                            //����
    Centerline();
    if(ZJ_Process_flag)        SX_FLAG=1;
    if(SX_FLAG) 
    {
        if(!ZJ_Process_flag)   shizi_handle();    
        _lvbo();
        nihe_center();
        _90_ZJ();         
        pinghua(); 
        HX_D(); 
        youxiao_hang();
        if(!ZJ_Process_flag)    Z_A_deal();
        angle_pd(); 
       // SCCB_WriteByte(OV7725_CNST,0X43);  //�����Ƕ�̬�ı���ֵ�����ⲻ��Ҫ�ķ��գ����ﲻͬ��
    }
    else                           
    {
       _lvbo(); 
       pinghua(); 
       youxiao_hang_1();
       angle_pd_1();
     //  SCCB_WriteByte(OV7725_CNST,0X43);
    }     
     DISPLAY();
     store_data();    
     GX_flag=last_pic;
}