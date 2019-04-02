#ifndef _CCD_H
#define _CCD_H

struct TSL1401
{
  uint8 left_Edge[50];
  uint8 right_Edge[50];
  uint8 mid_Line[50];
 /*
  ����11���߶��ҵ�������01ֻ�ҵ��ұߣ�����10ֻ�ҵ����,����0��û��
  ���û�б��ߣ�������һ�α��ֵ������ҵ����ߣ��޸ı��ֵ
  �ٸ��ݱ��ֵȷ��Ѱ�ұ��߷�Χ��
 */
  uint8 flag_edge[50];
  uint8 cross[50];
  uint8 obstacle;     
  uint8 max;
  uint8 min;
  uint8 aver;
  uint8 THRESHOLD;
  uint8 width;
  uint8 CCD_Offset;
  uint8 search;
  uint8 CROSS;
};

void CCD_init(void);

void Filter(uint8 *last, uint8 *news, uint16 len); //�˲�

void maxvar(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd);  //�������ֵ

void maxvar_a(uint8 *buf,uint16 len,uint8  maxval,struct TSL1401 *ccd);  //�������ֵ

void abs_diff(int16 *dst,uint8 *src,uint16 len);   //����

void findMax(uint8 *buf,struct TSL1401 *ccd);     //Ѱ�������Сֵ

void searchEdge(int16 *bin,struct TSL1401 *ccd);  //Ѱ�ұ���

void Obstacle_search(int16 *bin,struct TSL1401 *ccd);

void CaculateMid_Line(struct TSL1401 *ccd);       //��������


#endif