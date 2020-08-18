#ifndef _picture_deal_
#define _picture_deal_

#include "common.h"
#include "include.h"

#define PP(i,j)    img[(i)][(j)]	//ͼ����һ�㣬���꣨i��j��
#define LL(i)      left_line[(i)]	//����ߵ�
#define RL(i)      right_line[(i)]	//�ұ��ߵ�
#define CL(i)      center_line[(i)]	//�����ߵ�
#define SL(i)      start_line[(i)]	//��ʼ���ߵ�

#define BLACK_P    (0)	//����ڵ�
#define WHITE_P    (1)	//����׵�

#define LEFT       (0)
#define RIGH       (1)

typedef enum
{
  normal=0,				//��ʼ��״̬
  bef_in_circle_left,	//���뻷��֮ǰ
  mid_in_circle_left,	//���뻷��֮��
  aft_in_circle_left,	//���뻷��֮��
  bef_out_circle_left,	//������֮ǰ
  mid_out_circle_left,	//������֮��
  aft_out_circle_left,	//������֮��  
  bef_in_circle_righ,	//�ҽ��뻷��֮ǰ
  mid_in_circle_righ,	//�ҽ��뻷��֮��
  aft_in_circle_righ,	//�ҽ��뻷��֮��
  bef_out_circle_righ,	//����뻷��֮ǰ
  mid_out_circle_righ,	//����뻷��֮��
  aft_out_circle_righ, 	//����뻷��֮��   
}position;		//���ڻ���λ�ӱ�־�ṹ��

typedef enum
{
  normal_ten=0,	//��ʼ��״̬
  bef_ten,		//ʮ����ǰ
  mid_ten,		//ʮ������
  aft_ten,		//ʮ�����
  
}ten_status;	//����ʮ����λ�ӱ�־�ṹ��

typedef enum
{
  running=0,	//����
  stop=1,		//ͣ��
}status;	//����ͣ״̬�ṹ�壨����ʶ�������ͣ����

/**************picture_deal_global_var******/
extern position car_position;
extern status   car_status;
extern ten_status car_ten_status;
extern int16 left_line[120];
extern int16 right_line[120];
extern int16 center_line[120];
extern int16 start_line[120];
extern int16 compensation_line[10];
extern int16 near_lost_count_left;
extern int16 near_lost_count_righ;
extern int16 lost_to_have_flag;
extern int16 turn_flag;
extern int16 lost_count;
/**************function*********************/
extern float get_picture_diff(int16 sline,int16 eline);

void param_reset(void);
status check_end_line(int16 sline,int16 eline);
int16 search_start(int16 sline,int16 eline);
int16 gb_recursion(int16 start,int16 end,int16 line);
void find_line(int16 line,int16 start);
void judge_init_start(int16 line);
float get_average_diff(int16 i,int16 j,int16* arry);
int16 partition(int16 arr[], int16 low, int16 high);
void quick_sort(int16 arr[], int16 start, int16 end);
int16 judge_next_line(int16 sline,int16 eline,int16 start);
int16 search_black_line(int16 line,int16 start);
int16 search_white_line(int16 line,int16 start);
int16 judge_column(int16 line);
void judge_position(void);
#endif /*_picture_deal_*/