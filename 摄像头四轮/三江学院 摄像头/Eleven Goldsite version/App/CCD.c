#include "common.h"
#include "include.h"

uint8 CCD_BUFF[TSL1401_MAX*3][TSL1401_SIZE];    //����洢����CCDͼ�������  
int16 CCD_Value[TSL1401_MAX][TSL1401_SIZE];     //�洢���ֵ������Ѳ��

struct TSL1401 ccd1;
extern uint8 ramp;//�µ�ֵ

/**************************

   ccd �� �� �� ʼ ��

**************************/
void CCD_init(void)
{
  ccd1.THRESHOLD = 17;   //�ж���������ֵ���̶�17
  ccd1.CCD_Offset = 10;  //�ģ�����ѹֵ
  ccd1.CROSS = 70;       //�ģ�ƽ��ֵ���ж�ʮ��  
}
/************************************************

        Filter �� �� �� �� �� �� ��

           �� λ ֵ ƽ �� �� ��

*************************************************/
void Filter(uint8 *last, uint8 *news, uint16 len)
{
  uint8 i;
  
  for(i = 1; i < len - 1 ; i++)
  {
    if( (last[i]==last[i-1]) ||(last[i]==last[i+1]) ||(last[i+1]==last[i-1]))
      news[i] = (last[i-1] + last[i] + last[i+1]) / 3;
    else if(last[i] > last[i-1])
    {
      if(last[i] > last[i+1])
      {
        if(last[i-1] > last[i+1])
          news[i] = last[i-1];
        else news[i] = last[i+1];
      }
      else news[i] = last[i];
    }
    else
    {
      if(last[i] < last[i+1])
      {
        if(last[i-1] < last[i+1])
          news[i] = last[i-1];
        else news[i] = last[i+1];
      }
      else news[i] = last[i];
    }
  }   
}
/***************************************

             �� �� �� �� ֵ 

****************************************/
void maxvar(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd)
{
  while(len--)
  {
    buf[len] = buf[len] - ccd->CCD_Offset;//��ȥ����ѹֵ
    if(buf[len] > maxval)
    {
        buf[len]= maxval;
    }
  }
}
/***************************************

          �� �� �� �� ֵ �� �� Ӧ

****************************************/
void maxvar_a(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd)
{
  uint8 i=0;
  uint8 max=0;
 //�ڿ�������Ѱ�ҿ������ֵ
 if(ccd->flag_edge[1] == 11 && ccd->width > 10)
  {
    for(i = 0;i < len;i++)
    {
      if(i > ccd->left_Edge[1] && i < ccd->right_Edge[1])
        if(max < buf[i])
          max = buf[i];
    }
  }
  else if(ccd->flag_edge[1] == 1)
  {
    for(i = 0;i < len;i++)
    {
      if(i < ccd->right_Edge[1])
        if(max < buf[i])
          max = buf[i];
    }
  }
  else if(ccd->flag_edge[1] == 10)
  {
     for(i = 0;i < len;i++)
    {
      if(i > ccd->left_Edge[1])
        if(max < buf[i])
          max = buf[i];     
    }
  }
  else
  {
    for(i = 0;i < len;i++)
    {
      if(max < buf[i])
        max = buf[i];
    }
  }
  //�ж����ֵ�Ƿ�������Ƿ����127
  if(max > maxval) //����������Ӧ��127֮��
  {
    int ratio;
    int temp;
    if(max > ccd->CCD_Offset)
      ratio = 12700 / (max - ccd->CCD_Offset);
    else
      ratio = 0;
    
    for(i = 0;i < len;i++)
    {
      if(buf[i] > ccd->CCD_Offset)
        temp = (buf[i] - ccd->CCD_Offset);
      else
        temp = 0;
      buf[i] = (uint8)(temp * ratio / 100);
      if(buf[i] > maxval)
        buf[i] = maxval;
    }
  }
  else  //���û�������������ȥ����ѹֵ
  {
    while(len--)
    {
      buf[len] = buf[len] - ccd->CCD_Offset;//��ȥ����ѹֵ
      if(buf[len] > maxval)
      {
          buf[len]= maxval;
      }
    }
  }
  
}
/***********************************************

     *  @brief   �� �� �� �� �� �� ֵ

 ***********************************************/
void abs_diff(int16 *dst,uint8 *src,uint16 len)
{
    int8 tmp;
    uint16 lentmp = len;

    while((--lentmp) > 3)       //��ѭ�� len-3 ��
    {
        tmp = *(src+3)- *src;   //ǰһ���ȥ��һ��Ĳ�
        *dst = tmp;             //�Ѳ�ֵ��ֵ��buff��2��3��
        src++;
        dst++;
    }
    // ���3������Ϊ 0             
    *(dst++) = 0;*(dst++) = 0;*dst = 0;
}
/******************************************

        �� �� �� �� �� �� ѹ ֵ

    �� �� ֵ���� С ֵ���� �� ƽ �� ֵ

********************************************/
void findMax(uint8 *buf,struct TSL1401 *ccd)
{
  uint8 i=0;
  uint8 max,min,aver;
  uint32 sum;
  max = 0;
  min = 100;
  sum = 0;
  for(i = 10;i < 119;i++)
  {
    if(buf[i]>max)
      max = buf[i];
    if(buf[i]<min)
      min = buf[i];
    sum = sum + buf[i];
  }
  aver = (uint8)(sum / 108);
  ccd->aver = aver;
  ccd->max  = max;
  ccd->min  = min;
}
/**********************************************

          �� �� �� �ң��� �� ��

***********************************************/
void searchEdge(int16 *bin,struct TSL1401 *ccd)
{
  uint8 i;
  ccd->cross[0] = 0;
  //10�δ��м�������Ѱ�ұ��ߣ���֤�������ȷ��
  if(ccd->search<10)
  {
    for(i=70;i>=10;i--)
    {
      if(bin[i] > ccd->THRESHOLD)
      {
        ccd->left_Edge[0] = i;
        break;
      }
      else if(i == 10)
      {
        ccd->left_Edge[0] = 0;
      }
    }
    for(i=50;i<=118;i++)
    {
      if(bin[i] < -ccd->THRESHOLD)
      {
        ccd->right_Edge[0] = i;
        break; 
      }
      else if(i == 118)
      {
        ccd->right_Edge[0] = 0;
      }
    }
    ccd->search++;
  }
  else//�������ߣ����û������һ�ߣ�����һ�ߵ�ֵ����
  {
   //����ϴ�������һ����Χ�ڣ�����һ����������Ѱ����ߣ�����Ѱ���ұ�
    if(ccd->mid_Line[1]> 5 && ccd->mid_Line[1] < 122)
    {
      for(i = ccd->mid_Line[1] + 0;i >= 5;i--)
      {
        if(bin[i] > ccd->THRESHOLD 
           && CCD_BUFF[1*TSL1401_MAX+0][i-4]<CCD_BUFF[1*TSL1401_MAX+0][i+9] 
           && CCD_BUFF[1*TSL1401_MAX+0][i-5]<CCD_BUFF[1*TSL1401_MAX+0][i+10])
        {
          ccd->left_Edge[0] = i;
          break;
        }
        else if(i == 5)
        {
          ccd->left_Edge[0] = 0;
        }
      }
      for(i = ccd->mid_Line[1] - 0;i<=122;i++)
      {
         if(bin[i] < -ccd->THRESHOLD 
            && CCD_BUFF[1*TSL1401_MAX+0][i+4]<CCD_BUFF[1*TSL1401_MAX+0][i-9] 
            && CCD_BUFF[1*TSL1401_MAX+0][i+5]<CCD_BUFF[1*TSL1401_MAX+0][i-10])
        {
          ccd->right_Edge[0] = i;
          break; 
        }
        else if(i == 122)
        {
          ccd->right_Edge[0] = 0;
        }
      }
    }
    //��һ������������ߣ������������һ����ΧѰ���ұ��ߣ���һС��ΧѰ�������
    //�Ҳ�������ߣ����Բ������
    else if(ccd->mid_Line[1] <= 5)
    {
      ccd->left_Edge[0] = 0;
      if(ccd->right_Edge[1]==0)
      {
        for(i = 5;i<=(ccd->width/2+ccd->width/5);i++)//55
        {
          if(bin[i] < -ccd->THRESHOLD 
              && CCD_BUFF[1*TSL1401_MAX+0][i+9]<CCD_BUFF[1*TSL1401_MAX+0][i-4] 
              && CCD_BUFF[1*TSL1401_MAX+0][i+10]<CCD_BUFF[1*TSL1401_MAX+0][i-5])
          {
            ccd->right_Edge[0] = i;
            break; 
          }
          else if(i == (ccd->width/2+ccd->width/5))
          {
            ccd->right_Edge[0] = 0;
          }
        }
      }
      else
      {
        for(i = 5;i<=(ccd->right_Edge[1]+ccd->width/2+ccd->width/8);i++)
        {
          if(bin[i] < -ccd->THRESHOLD 
              && CCD_BUFF[1*TSL1401_MAX+0][i+9]<CCD_BUFF[1*TSL1401_MAX+0][i-4] 
              && CCD_BUFF[1*TSL1401_MAX+0][i+10]<CCD_BUFF[1*TSL1401_MAX+0][i-5])
          {
            ccd->right_Edge[0] = i;
            break; 
          }
          else if(i == (ccd->right_Edge[1]+ccd->width/2+ccd->width/8))
          {
            ccd->right_Edge[0] = 0;
          }
        }
      }
    }
    //��һ�����������ұߣ������ұ�һ����Ѱ������ߣ���С��Χ��Ѱ���ұ���
    //�Ҳ������ұߣ������ұ�
    else if(ccd->mid_Line[1] >= 122)
    {
      if(ccd->left_Edge[1]==0)
      {
        for(i = 122;i >=(122-ccd->width/2-ccd->width/5);i--)//71
        {
          if(bin[i] > ccd->THRESHOLD 
             && CCD_BUFF[1*TSL1401_MAX+0][i-9]<CCD_BUFF[1*TSL1401_MAX+0][i+4] 
             && CCD_BUFF[1*TSL1401_MAX+0][i-10]<CCD_BUFF[1*TSL1401_MAX+0][i+5])
          {
            ccd->left_Edge[0] = i;
            break;
          }
          else if(i == (122-ccd->width/2-ccd->width/5))
          {
            ccd->left_Edge[0] = 0;
          }
        }
      }
      else
      {
        for(i = 122;i >= (ccd->left_Edge[1]-ccd->width/2-ccd->width/8);i--)
        {
          if(bin[i] > ccd->THRESHOLD 
             && CCD_BUFF[1*TSL1401_MAX+0][i-9]<CCD_BUFF[1*TSL1401_MAX+0][i+4] 
             && CCD_BUFF[1*TSL1401_MAX+0][i-10]<CCD_BUFF[1*TSL1401_MAX+0][i+5])
          {
            ccd->left_Edge[0] = i;
            break;
          }
          else if(i == (ccd->left_Edge[1]-ccd->width/2-ccd->width/8))
          {
            ccd->left_Edge[0] = 0;
          }
        }
      }
      ccd->right_Edge[0] = 0;
    }
  }
  //���ñ��߱��λ
  if(ccd->left_Edge[0]!=0&&ccd->right_Edge[0]!=0)//���ҵ�
  {
    ccd->flag_edge[0] = 11;
  } 
  else if(ccd->left_Edge[0]==0&&ccd->right_Edge[0]!=0)//ֻ���ұ�
  {
    ccd->flag_edge[0] = 01;
  }
  else if(ccd->left_Edge[0]!=0&&ccd->right_Edge[0]==0)//ֻ�����
  {
    ccd->flag_edge[0] = 10;
  }
  else  
  {
    ccd->flag_edge[0] = 0;
    //���û�б��ߣ�����ƽ��ֵ����һ��ֵ��ʮ�ֱ�־Ϊ1������ʮ�ֱ�־Ϊ0
    ccd->cross[0] = 0;
    if(ccd->aver > ccd->CROSS /*&& ramp==0*/)//���ֵ��Ҫ����//�����µ���������ֹ�µ�����
    {
      ccd->cross[0] = 1;
    }
  }
}
/*****************************************

  �� �� �� �� �� �� �� �أ��� �� �� �� ֵ

******************************************/
void CaculateMid_Line(struct TSL1401 *ccd)                
{
  uint8 i;
  
  if(ccd->flag_edge[0] == 11)//���ҵĵ�
  {
    //������ȴ���һ��Ϊ��ȷ���������
    if( ccd->right_Edge[0]-ccd->left_Edge[0] > 25)
    {
      ccd->mid_Line[0] = (uint8)( (ccd->left_Edge[0]+ccd->right_Edge[0])/2 );
      ccd->width = ccd->right_Edge[0] - ccd->left_Edge[0];//����������ȣ����ڲ���
    }
    else
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }
  else if(ccd->flag_edge[0] == 10)//ֻ�ҵ����
  {
    if((ccd->left_Edge[0] + ccd->width/2) < 122)
      ccd->mid_Line[0] = (uint8)(ccd->left_Edge[0] + ccd->width/2);
    else 
      ccd->mid_Line[0] = 122;
  }
  else if(ccd->flag_edge[0] == 01)//ֻ�ҵ��ұ�
  {
    if( (ccd->right_Edge[0]-ccd->width/2) > 5)
      ccd->mid_Line[0] = (uint8)(ccd->right_Edge[0] - ccd->width/2);
    else
      ccd->mid_Line[0] = 5;
  }
  else if(ccd->cross[0] == 1)//ʮ��
  {
    if(ccd->mid_Line[1]>61&&ccd->mid_Line[1]<65)
    {
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
    else
    {
      ccd->mid_Line[0] = 63;
    }
  }
  else 
  {
    if(ccd->flag_edge[2] == 1 && ccd->flag_edge[1] == 1 && ccd->flag_edge[2] == 1)
    {
      ccd->mid_Line[0] = 5;
    }
    else if(ccd->flag_edge[2] == 10 && ccd->flag_edge[1] == 10 && ccd->flag_edge[2] == 10)
    {
      ccd->mid_Line[0] = 122;
    }
    else
    {
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }

  if(ccd->search == 10 && ccd->cross[0] == 0)
  {
    //����������ߺ��ϴ��������̫��Ϊ��������
    if( ccd->mid_Line[0]-ccd->mid_Line[1] > 50)
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
    if( ccd->mid_Line[0]-ccd->mid_Line[1] < -50)
    {
      ccd->flag_edge[0] = 0;
      ccd->mid_Line[0] = ccd->mid_Line[1];
    }
  }
 
  //�洢ǰ50��λ����Ϣ
  for(i=49;i>0;i--)
  {
    ccd->flag_edge[i] = ccd->flag_edge[i-1];
    ccd->cross[i] = ccd->cross[i-1];
    
    ccd->left_Edge[i] = ccd->left_Edge[i-1];
    ccd->right_Edge[i] = ccd->right_Edge[i-1];
    ccd->mid_Line[i] = ccd->mid_Line[i-1];
  }
}
/*******************************************

  �ϰ����������ϰ�λ�ã���������ǰ����λ��

********************************************/
void Obstacle_search(int16 *bin,struct TSL1401 *ccd)
{
  uint8 i;
  
  uint8 ob_r,ob_l,ob_flag;
  ob_r = ob_l = ob_flag = 0;
  
  if( ccd->width > 25 && ccd->width < 40 && ccd->flag_edge[0] == 11)
  {
    //����������һ����Χ�ڣ������ұ߾�Ѱ���ϰ�
    if(ccd1.left_Edge[0] > 25 && ccd1.right_Edge[0] < 102)
    {
      //�������ߵ���߽��ȴ����ұ��ߵ��ұ߽��ȣ��������Ѱ���ϰ�
      if(ccd1.left_Edge[0] >= (127-ccd1.right_Edge[0]) )
      {
        for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//�����ߵ���߽�����һ����Ѱ���ϰ��ұ�
        {
          if(bin[i] < -ccd->THRESHOLD)
          {
            ob_r = i;
            break;
          }
        }
        
        if(ob_r == 10)//���û�ҵ���˵��������ϰ�
        {
          ob_flag = 0;
        }
        else  //����ҵ���������һ��Ѱ���ϰ����
        {
          
          for(i = ob_r - 1;i >= 3;i--)
          {
            if(bin[i] > ccd->THRESHOLD)
            {
              ob_l = i;
              break;
            }
          }
          if(ob_l == 3)
          {
            ob_flag = 0;
          }
          else
          {
            if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
               && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//�ٴ��жϣ��ϰ�left������left�����Ƿ����
            {
              ob_flag = 10;
            }
            else
            {
              ob_flag = 0;
            }
          }
        }
        
        if(ob_flag == 0)//������û���ҵ��ϰ����������Ҳ�Ѱ��
        {
          ob_r = ob_l = ob_flag = 0;//��־����
          
          for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
          {
            if(bin[i] > ccd->THRESHOLD)
            {
              ob_l = i;
              break;
            }
          }
          
          if(ob_l == 118)
          {
            ob_flag = 0;
          }
          else
          {
            for( i = ob_l + 1 ;i <= 124;i++) 
            {
              if(bin[i] < -ccd->THRESHOLD)
              {
                ob_r = i;
                break;
              }
            }
            
            if(ob_l == 124)
            {
              ob_flag = 0;
            }
            else
            {
              if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//�ٴ��жϣ��ϰ�right������right�����Ƿ����
              {
                ob_flag = 01;
              }
              else
              {
                ob_flag = 0;
              }
            }
          }
        }
        else//����ҵ���������
        {
          ;
        }
        
      }
      //�������ߵ���߽���С���ұ��ߵ��ұ߽��ȣ������ұ�Ѱ���ϰ�
      else 
      {
        for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
        {
          if(bin[i] > ccd->THRESHOLD)
          {
            ob_l = i;
            break;
          }
        }
        
        if(ob_l == 118)
        {
          ob_flag = 0;
        }
        else
        {
          for( i = ob_l + 1 ;i <= 124;i++) 
          {
            if(bin[i] < -ccd->THRESHOLD)
            {
              ob_r = i;
              break;
            }
          }
          
          if(ob_l == 124)
          {
            ob_flag = 0;
          }
          else
          {
            if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//�ٴ��жϣ��ϰ�right������right�����Ƿ����
            {
              ob_flag = 01;
            }
            else
            {
              ob_flag = 0;
            }
          }
          
        }
        
        if(ob_flag == 0)//����Ҳ�û���ҵ��ϰ������������Ѱ��
        {
          ob_r = ob_l = ob_flag = 0;//��־����
         
          for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//�����ߵ���߽�����һ����Ѱ���ϰ��ұ�
          {
            if(bin[i] < -ccd->THRESHOLD)
            {
              ob_r = i;
              break;
            }
          }
          
          if(ob_r == 10)//���û�ҵ���˵��������ϰ�
          {
            ob_flag = 0;
          }
          else  //����ҵ���������һ��Ѱ���ϰ����
          {
            
            for(i = ob_r - 1;i >= 3;i--)
            {
              if(bin[i] > ccd->THRESHOLD)
              {
                ob_l = i;
                break;
              }
            }
            
            if(ob_l == 3)
            {
              ob_flag = 0;
            }
            else
            {
              if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
                 && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//�ٴ��жϣ��ϰ�left������left�����Ƿ����
              {
                ob_flag = 10;
              }
              else
              {
                ob_flag = 0;
              }
            }
            
          }
          
        }
        else//�Ҳ��ҵ���������
        {
          ;
        }

      }
      
    }
    //�����С��30���ϰ�ֻ�����Ҳ�
    else if(ccd1.left_Edge[0] <= 25 && ccd1.right_Edge[0] < 102)
    {
      ob_r = ob_l = ob_flag = 0;//��־����
        
      for( i = ccd->right_Edge[0] + 5 ;i <= 118;i++)
      {
        if(bin[i] > ccd->THRESHOLD)
        {
          ob_l = i;
          break;
        }
      }
      
      if(ob_l == 118)
      {
        ob_flag = 0;
      }
      else
      {
        for( i = ob_l + 1 ;i <= 124;i++) 
        {
          if(bin[i] < -ccd->THRESHOLD)
          {
            ob_r = i;
            break;
          }
        }
        
        if(ob_l == 124)
        {
          ob_flag = 0;
        }
        else
        {
          if( (ob_r - ccd1.right_Edge[0]) > 12 &&  (ob_r - ccd1.right_Edge[0]) < 30
                 && (ob_l - ccd1.right_Edge[0]) > (ob_r - ob_l + 3) )//�ٴ��жϣ��ϰ�right������right�����Ƿ���� 
          {
            ob_flag = 01;
          }
          else
          {
            ob_flag = 0;
          }
        }
        
      }
    }
    //�ұ��ߴ���100���ϰ�ֻ�������
    else if(ccd1.left_Edge[0] > 25 && ccd1.right_Edge[0] >= 102)
    {
      for(i = ccd1.left_Edge[0] - 5;i >= 10;i--)//�����ߵ���߽�����һ����Ѱ���ϰ��ұ�
      {
        if(bin[i] < -ccd->THRESHOLD)
        {
          ob_r = i;
          break;
        }
      }
      
      if(ob_r == 10)//���û�ҵ���˵��������ϰ�
      {
        ob_flag = 0;
      }
      else  //����ҵ���������һ��Ѱ���ϰ����
      {
        
        for(i = ob_r - 1;i >= 3;i--)
        {
          if(bin[i] > ccd->THRESHOLD)
          {
            ob_l = i;
            break;
          }
        }
        if(ob_l == 3)
        {
          ob_flag = 0;
        }
        else
        {
          if( (ccd1.left_Edge[0] - ob_l) > 12 && (ccd1.left_Edge[0] - ob_l) < 30
             && (ccd1.left_Edge[0]-ob_r) > (ob_r - ob_l+3) )//�ٴ��жϣ��ϰ�left������left�����Ƿ����
          {
            ob_flag = 10;
          }
          else
          {
            ob_flag = 0;
          }
        }
      }
    }
    
    if(ob_flag == 10)
    {
      ccd->obstacle = 10;
    }
    else if(ob_flag == 01)
    {
      ccd->obstacle = 01;
    }
    else
    {
      ccd->obstacle = 0;
    }
  }
  else
  {
    ccd1.obstacle = 0;
  }
}
