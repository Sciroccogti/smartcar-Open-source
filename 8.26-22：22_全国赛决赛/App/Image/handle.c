#include "Handle.h"





//��·�ļ�⣬�����Ƿ��ʼʱ���ұ߽�ĸ�ֵ����ֵ���Ƿ���ԣ���Ҫ��֤�������Ǻ�






#define Display_OFF_ON 1    //������ʾ���أ�1Ϊ�� 0Ϊ��
#define SSSSSSS_OFF_ON 1    //СS��⿪�أ�1Ϊ�� 0Ϊ��

uint8 colour[2] = {255, 0},black=1,white=0; //0 �� 1 �ֱ��Ӧ����ɫ
//ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
////////////////////0       2       4       6       8




uchar Width_Max[61]={5,5,5,5,5,5,  5,5,5,5,5,
                        5,5,5,5,5,  5,5,5,5,5,                         
                         12,12,15,15,17,  17,18,18,20,20,                
                             23,23,25,25,26,  26,29,29,31,31,             
                                33,33,35,35,36,  36,38,38,41,41,          
                                    42,42,43,43,46, 46,48,48,50,50};


//uchar Weight[60] = {1,  1,  1,  1,  1,  2,  3,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������

//uchar Weight[60] = {1,  1,  4,  1,  6,  2,  6,  4,  6,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������

//uchar Weight[60] = {1,  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������







uchar Weight[60] = {  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50����
                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                    1,  1,  1,  1,  1,  1,  1,  1,1,  1,  1};	//��Ȩƽ������




//uchar Weight[60] = {  1,  2,  1,  4,  2,  4,  4,  4,  4,       //50����
//                    5,  6,  9, 10, 11, 12, 13, 13, 12, 11,     //40
//                   11, 10,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,1,  1,  1};	//��Ȩƽ������







//uchar Weight[60] = {  2,  1,  4,  2,  4,  4,  4,  4,       //50
//                    5,  6,  9, 10, 11, 12, 13, 15, 12, 13,     //40
//                   11, 12,  6,  5,  4,  3,  4,  3,  3,  2,      //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,  1,  1,  1,1, 1,   1,  1};	//��Ȩƽ������
//
//uchar Weight[60] = {  1,  1,  2,  3,  3,  4,  4,  4,  5,  6,     //50
//                      9, 10, 11, 12, 13, 15, 12, 13,  11,  10,  //40
//                      9,  8,  7,  6,  5,  4,  3,  3,  2,  1, //30
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                      1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������
 


//uchar Weight[60] = {48, 47, 46, 45, 44,   43, 42, 41, 40, 38,
//                    37, 36, 35, 34, 33,   32, 31, 30, 29, 28,
//				    27, 26, 25, 24, 23,   22, 21, 20, 19, 18,
//                    17, 16, 15, 14, 13,   12, 11, 10, 9,  8,
//					7,  6,   5,  5,  5,    5,  5,  5, 5,  5,
//					5,  5,   5,  5,  5,    5,  5,  5, 5,  5};	//��Ȩƽ������

//uchar Weight[60] = {  1, 1, 2,  3,  4,  4,  4,  5,  6,      //50
//                    9, 10, 11, 12, 13, 15, 12, 13, 11, 12,    //40
//                    6,  5,  4,  3,  4,  3,  3,  2,   1,  1,    //30
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,1,   1,  1,  1,  1,  1};	//��Ȩƽ������

//uchar Weight[60] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5,      //50
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,    //40
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,    //30
//                    
//                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3,//20
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������
//uchar Weight[60] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5,      //50
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    //40
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    //30
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,//20
//                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
//                    1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������

//uchar Weight[60] = {  3,  4,  4,  4,  5,  6,  9, 10, 11,  12,    //50
//                      13, 15, 12, 13, 11, 12, 6,  5,  4,   3, //40
//                       4,  3,  3,  2,  1,  1,  1,  1,  1,  1,  //30
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1};	//��Ȩƽ������

//uchar Weight[60] = {                  5,  6,  9, 10, 11,  12,    //50
//                      13, 15, 12, 13, 11, 12, 6,  5,  4,   3, //40
//                       4,  3,  3,  2,  1,  1,  1,  1,  1,  1,  //30
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,//20
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//                       1,  1,  1,  1                       };	//��Ȩƽ������

//uchar Weight[60] = {1,  1,  1,  1,  1,  1,  2,  2,  3,    3,  4,  4,  5,  5,
//                    6,  6,  4,  4,  2,    2,  1,  1,  1,  1,
//				    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,              };	//��Ȩƽ������

//uchar Weight[60] = {1,  1,  1,  1,  1,  1,  1,  2,  2,  3,    3,  4,  4,  5,  5,
//                    6,  6,  4,  4,  2,    2,  1,  1,  1,  1,
//				    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//                    1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1,    1,  1,  1,  1,  1,
//					1,  1,  1,  1,  1               };	//��Ȩƽ������

uchar Left_Line[62], Right_Line[62], Mid_Line[62];	// ԭʼ���ұ߽�����
uchar Left_Add_Line[62], Right_Add_Line[62];		// ���ұ߽粹������
uchar Left_Add_Flag[62], Right_Add_Flag[62];		// ���ұ߽粹�߱�־λ
uchar Width_Real[62];	// ʵ���������
uchar Width_Add[62];	// �����������
uchar Width_Min;		// ��С�������

uchar S_temp;
int8 Ramp_Error[10];    //�µ�������

uchar Ek_compar;         //�����м�ֵ�����е����е�Mid_Line�Ĳ�ֵ
uchar Foresight,Last_Foresight,Ramp_Flag_Last=0;	// ǰհ�������ٶȿ���
uchar Out_Side = 0;	// ���߿���
uchar Line_Count;	// ��¼�ɹ�ʶ�𵽵���������

uchar Left_Add_Start, Right_Add_Start;	// ���Ҳ�����ʼ������
uchar Left_Add_Stop, Right_Add_Stop;	// ���Ҳ��߽���������
float Left_Ka = 0, Right_Ka = 0;
float Left_Kb = 1, Right_Kb = 79;	// ��С���˷�����

uchar Left_Hazard_Flag, Right_Hazard_Flag, Hazard_Flag;	// �����ϰ����־
uint8 Hazard_Time;
uchar Left_Max, Right_Min;

uchar Starting_Line_Flag = 0;	// �����߱�־λ
uchar Starting_Line_Times = 0;	// �����߱�־λ
uchar Starting_Line_Time = 0;	// �����߱�־λ
char Last_Point = 40;

/************** ��·��ر��� *************/
uchar Annu_OFF_ON=0;
uchar Annulus_Flag=0;
uchar Annulus_Mid=0;
uchar Annulus_Left;
uchar Annulus_Right;
uchar Annulus_Count[4] = {0, 1, 0, 1};	         // 1�һ�·��0��·
uint16 Annulus_Delay[4] = {120, 130, 120, 130};	// ��·��ʱʱ��
uchar Annulus_Times = 0;	// ��·����
uchar Annulus2_Delay = 0;
uint16 Annulus_DelayMs = 0;	// ��⵽��·����ʱһ��ʱ����Զ������·��־λ

uint8  Annulus_Nums=0;
uint8  Annulus_Small=0;
uchar Annulus_Mode = 0;	

uchar Annulus_Num_Count=0;
/************** ��·��ر��� *************/


void Annulus_Control(void)
{

		Annulus_Right = Annulus_Count[Annulus_Times];
		Annulus_Left = !Annulus_Right;
		if (Annulus_DelayMs>Annulus_Delay[Annulus_Times])
                {
                   Annulus_Flag=0;
                   Annulus_DelayMs=0;
                   Annulus_Times++;
                   if (Annulus_Times > 3)
                   {
			Annulus_Times = 0;
                        
                   }
                }
		

}


void Annulus2_Control(void)
{
	Annulus_Right = Annulus_Count[Annulus_Times++];
	Annulus_Left = !Annulus_Right;
	
	Annulus_Mode = 1;	// ��ʼ���뻷·
	Annulus_Flag = 1;	// ��·��־λ��λ
	
    
    led(LED_MAX,LED_ON);// ��LEDָʾ��		ȫ����ʼ���ɹ�
    
	if (Annulus_Times >= 4)
	{
		Annulus_Times = 0;
	}
}

/*
*	��·ͼ�����㷨
*
*	˵������·ר�ã���������Ԫ��
*/
void Annulus2_Handle(uchar *data)
{
	uchar i;	// ������
	uchar res;
	
	Line_Count = 0;	// ����������λ
	
	Left_Add_Start = 0;		// ��λ������ʼ������
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;
	
	/***************************** ��һ�����⴦�� *****************************/
	res = First_Line_Handle(data);
	if (res == 0)
	{
		Out_Side++;	// ����
		return;
	}
	if (Left_Add_Start || Right_Add_Start)
	{
		Width_Min = 60;
	}
	/*************************** ��һ�����⴦����� ***************************/
	
	for (i = 59; i >= 15;)
	{
		i -= 2;
		
		if (!data[i*80 + Mid_Line[i+2]])//ǰ2���е��ڱ���Ϊ�ڵ㣬��������
		{ 
			i += 2;
			break;
		}
		else
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 78, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
			/************************** ���߼�⿪ʼ **************************/
			if (Width_Real[i] >= Width_Min)	// ������ȱ��,���뻷·
			{	
				if (Annulus_Left)	// ��·
				{
					Left_Add_Line[i] = Left_Line[i];
                                        Right_Add_Line[i]=Left_Add_Line[i] + (Width_Max[i]+11);  //�������
                                        Right_Add_Line[i]=range_protect(Right_Add_Line[i],0,79);
//                    Right_Add_Line[i]=Left_Line[i] + (Width_Min-1);
					
				}
				else				// �һ�·
				{
					Right_Add_Line[i] = Right_Line[i];
                                        Left_Add_Line[i]=range_protect((Right_Line[i] - (Width_Max[i]+11)),0,79);
//                    Left_Add_Line[i]=Right_Line[i] - (Width_Min-1);
				}
			}
			/************************** ���߼����� **************************/
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	
			if (Width_Add[i] < Width_Min)
			{
				Width_Min = Width_Add[i];
			}
			if (Width_Add[i] <= 6)
			{
				i += 2;
				break;
			}
		}
	}
	Line_Count = i;

#if   Display_OFF_ON	
	for (i = 61; i >= Line_Count+2;)
	{
		i -= 2;
		/**//************************* ��λ����ʾ�߽� *************************/
		/**/data[i*80 + Left_Add_Line[i] + 2] = 0;	// ��λ����ʾ���ߺ����߽磬����ʱ����
		/**/data[i*80 + Right_Add_Line[i] - 2] = 0;	// ��λ����ʾ���ߺ���ұ߽磬����ʱ����
		/**/data[i*80 + Mid_Line[i]] = 0;			// ��λ����ʾ���ߣ�����ʱ����
//		/**/data[i*80 + Left_Line[i] + 2] = 0;		// ��λ����ʾԭʼ��߽磬����ʱ����
//		/**/data[i*80 + Right_Line[i] - 2] = 0;		// ��λ����ʾԭʼ�ұ߽磬����ʱ����
		/**//************************* ��λ����ʾ�߽� *************************/
	}
#endif	
	/**************************** ��·����״̬�ж� ****************************/
	if (Annulus_Left)	// ��·
	{
		if (Right_Add_Flag[59])		// �������Ҫ����
		{
            Annulus_Num_Count++;
            if(Annulus_Num_Count>0)
            {
                if (Annulus_Mode == 1)	// �ϴ�״̬Ϊ��ʼ���뻷·
                {
                    Annulus_Mode = 2;	// ��ǰ״̬Ϊ���뻷·��
                }
                if (Annulus_Mode == 3)	// �ϴ�״̬Ϊ�ѳɹ����뻷·
                {
                    if (Left_Add_Line[59] > Left_Add_Line[Line_Count+4])	// ��߽�����������ģ���������·����
                    {
                        Annulus_Mode = 4;	// ��ǰ״̬Ϊ����·��
                        Annulus2_Delay = 15;	// ������ʱ1s
                    }
                }
            }
		}
		else if(!Right_Add_Flag[59])					// ����в���Ҫ����
		{
            Annulus_Num_Count=0;
			if (Annulus_Mode == 2)	// �ϴ�״̬Ϊ���뻷·��
			{
				Annulus_Mode = 3;	// ��ǰ״̬Ϊ�ѳɹ����뻷·��׼������·
			}
			if (Annulus_Mode == 4)	// �ϴ�״̬Ϊ����·��
			{
				if (!Annulus2_Delay)	// ���ٱ���1s��·ר���㷨����ֹ����
				{
					Annulus_Mode = 0;	// ����״̬Ϊ�ɹ�����·��׼���´ν��뻷·
					Annulus_Flag = 0;	// �����·״̬��־λ
                    Annulus_Small=0; 
                    
                    led(LED_MAX,LED_OFF);// ��LEDָʾ��		ȫ����ʼ���ɹ�
                    
				}
			}
		}
	}
	else	// �һ�·
	{
		if (Left_Add_Flag[59])		// �������Ҫ����
		{
            Annulus_Num_Count++;
            if(Annulus_Num_Count>0)
            {
                if (Annulus_Mode == 1)	// �ϴ�״̬Ϊ��ʼ���뻷·
                {
                    Annulus_Mode = 2;	// ��ǰ״̬Ϊ���뻷·��
                }
                if (Annulus_Mode == 3)	// �ϴ�״̬Ϊ�ѳɹ����뻷·
                {
                    if (Right_Add_Line[59] < Right_Add_Line[Line_Count+4])	// �ұ߽�����������ģ���������·����
                    {
                        Annulus_Mode = 4;	// ��ǰ״̬Ϊ����·��
                        Annulus2_Delay = 15;	// ������ʱ1s
                    }
                }
            }
		}
		else if(!Left_Add_Flag[59])						// ����в���Ҫ����
		{
            Annulus_Num_Count=0;
			if (Annulus_Mode == 2)	// �ϴ�״̬Ϊ���뻷·��
			{
				Annulus_Mode = 3;	// ��ǰ״̬Ϊ�ѳɹ����뻷·��׼������·
			}
			if (Annulus_Mode == 4)	// �ϴ�״̬Ϊ����·��
			{
				if (!Annulus2_Delay)	// ����״̬Ϊ�ɹ�����·��׼���´ν��뻷·
				{
					Annulus_Mode = 0;	// ����״̬Ϊ�ɹ�����·��׼���´ν��뻷·
					Annulus_Flag = 0;	// �����·״̬��־λ
                    Annulus_Small =0; 
                    
                    led(LED_MAX,LED_OFF);// ��LEDָʾ��		ȫ����ʼ���ɹ�
				}
			}
		}
	}
	/************************** ��·����״̬�жϽ��� **************************/
}





/****************** ���㷨 ******************/

/*
*	ͼ���㷨������ʼ��
*
*	˵������Ӱ���һ�����⴦��
*/
void Image_Para_Init(void)
{	
	Mid_Line[61] = 40;
	Left_Line[61] = 1;
	Right_Line[61] = 79;
	Left_Add_Line[61] = 1;
	Right_Add_Line[61] = 79;
	Width_Real[61] = 78;
	Width_Add[61] = 78;
}




/*
*	ͼ�����㷨
*
*	˵����������ͨͼ�񣬰���ʮ�֡��ϰ�
*/
void Image_Handle(uchar *data)
{
	uchar i;	// ������
    uint8 j;
	uchar res,temps;	// ���ڽ��״̬�ж�
	float Result;	// ���ڽ��״̬�ж�
    uchar Add_Start_Max;
	uchar Weight_Left, Weight_Right;
	uchar Mid_Left, Mid_Right;
	uchar Limit_Left, Limit_Right;
	
	Line_Count = 0;	// ����������λ
	Starting_Line_Times=0;
        Starting_Line_Time=0;
        Starting_Line_Flag=0;
	Left_Hazard_Flag = 0;	// ��λ�����ϰ����־λ
	Right_Hazard_Flag = 0;
	Hazard_Flag=0;
    
       
     
     
	Left_Add_Start = 0;		// ��λ������ʼ������
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;
	
	/***************************** ��һ�����⴦�� *****************************/
	res = First_Line_Handle(data);
	if (res == 0)
	{
		Out_Side = 1;	// ����
		return;
	}
	Out_Side = 0;
	Line_Count = 59;
	/*************************** ��һ�����⴦����� ***************************/
	
	for (i = 59; i >= 19;)	// ������ǰ40��ͼ�񣬸��к������20������
	{
		i -= 2;	// ���д�����С��Ƭ������
		
		if (!data[i*80 + Mid_Line[i+2]])//ǰ2���е��ڱ���Ϊ�ڵ㣬����������������Ҳ�����ǻ�·
		{
                /**************************** �����߼�⿪ʼ ****************************/
                 if (i>30 && Annulus_Check && i<59)
                { 
                        Starting_Line_Times=0;
                        Starting_Line_Time=0;
                        if (Last_Point >35 && Last_Point < 45)
                        {
                                for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                {
                                        
                                        if (Starting_Line_Time==0)
                                        {
                                                if(!data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Times++;                                                        
                                                        Starting_Line_Time=1;
                                                }     
                                        }
                                        else
                                        {
                                               if(data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Time=0;
                                                } 
                                        }
                                }
                                if(Starting_Line_Times>5 && Starting_Line_Times<15)
                                {
                                                Starting_Line_Flag=1;                                                
                                }

                        }
                  
                }
                

       if(Annu_OFF_ON==0)
       {
                                   /**************************** ���̵�Բ����⣬����Ӱ���ʱʹ�� ****************************/
                    if (Width_Real[i+2]>30 && Mid_Line[i+2] > 28 && Mid_Line[i+2] <52 && i<55 && Right_Add_Flag[i+2] && Left_Add_Flag[i+2])
                    {
                        if(((Width_Real[i+4] - Width_Real[i+6]) >=0) && ((Width_Real[i+4] - Width_Real[i+6]) <=6) && ((Left_Line[i+6] - Left_Line[i+4]) >=0) 
                            && ((Left_Line[i+6] - Left_Line[i+4]) <=6)&& ((Right_Line[i+4] - Right_Line[i+6])<=6) && ((Right_Line[i+4] - Right_Line[i+6])>=0) && Right_Add_Start && Left_Add_Start)
                        {
                            if(Annulus_Check && Last_Point<50 && Last_Point>30)
                            { 
                                    Starting_Line_Times=0;
                                    Starting_Line_Time=0;
                                    for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                    {                                     
                                            if (Starting_Line_Time==0)
                                            {
                                                    if(data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=1;
                                                            }                                                       
                                                    }     
                                            }
                                            else if(Starting_Line_Time==1)
                                            {
                                                   if(!data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=2;
                                                            }                                                       
                                                    }                                                                                                    
                                            }
                                            else if(Starting_Line_Time==2)
                                            {
                                                   if(data[80*(i-2)+j])
                                                    {
                                                            Starting_Line_Times++;
                                                            if (Starting_Line_Times>=3)
                                                            {
                                                                Starting_Line_Times=0;
                                                                Starting_Line_Time=3;
                                                            }                                                       
                                                    } 
                                            }
                                    }
                                    if (Starting_Line_Time==3)
                                    {
                                      /********** ����СԲ������ ***********/      
                                       Annulus_Nums=0;
                                       for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                       {
                                            if(!data[80*(i-2)+j])
                                            {
                                                    Annulus_Nums++;
                                            }
                                       }
                                       if (Annulus_Nums>10 && Annulus_Nums<35)
                                       {
                                            Annulus_Small=1;
                                       }
                                       /********** ������ ***********/  
                                          Annulus2_Control();    
                                    }
                                    Starting_Line_Time=0;
                               
                                }
                        }
                  }
        //��·������
       }
       else
       {
                        /**************************** �򵥵�Բ����⣬����Ӱ��Сʱʹ�� ****************************/
                        if (Width_Real[i+2]>30 && i<50 && Right_Add_Flag[i+2] && Left_Add_Flag[i+2] && Right_Add_Start && Left_Add_Start && !Right_Add_Stop && !Left_Add_Stop)
                        {
                                if (Left_Add_Start >= Right_Add_Start)
                                {
                                        Add_Start_Max = Left_Add_Start;
                                }
                                else
                                {
                                        Add_Start_Max = Right_Add_Start;
                                }
                                if((Width_Real[i+2] > Width_Real[Add_Start_Max]) && ((Width_Real[i+2] - Width_Real[i+4]) <=10) && ((Left_Line[i+4] - Left_Line[i+2]) <=5) && ((Right_Line[i+2] - Right_Line[i+4])<=5))
                                {
                                        if(Annulus_Check)
                                        {
                                               
                                                /********** ����СԲ������ ***********/      
                                               Annulus_Nums=0;
                                               for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                               {
                                                    if(!data[80*(i-2)+j])
                                                    {
                                                            Annulus_Nums++;
                                                    }
                                               }
                                               if (Annulus_Nums>10 && Annulus_Nums<35)
                                               {
                                                    Annulus_Small=1;
                                               }
                                                /********** ������ ***********/  
                                                Annulus2_Control();
                                        }
                                }
                       }
       }

			break;
		}
        else	// ʹ��ǰ2���е�������ɨ��߽�
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 79, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
		}


			
		/**************************** ���߼�⿪ʼ ****************************/
		if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// ������ȱ��������ʮ�ֻ�·
		{
			// ʹ�������㷨����ͻ����⣬�ž�ʮ��©��
//			Result = Calculate_Angle(Left_Add_Line[i+4], Left_Add_Line[i+2], Left_Add_Line[i]);	// ͻ�����
//			if (Result <= -3)	// -1��135��
//			{
//				Left_Add_Flag[i] = 1;
//			}
//			Result = Calculate_Angle(Right_Add_Line[i+4], Right_Add_Line[i+2], Right_Add_Line[i]);	// ͻ�����
//			if (Result >= 3)	// 1��135��
//			{
//				Right_Add_Flag[i] = 1;
//			}
//			if (i >= 55)	// ����Ͻ�
//			{
//				if (Left_Add_Line[i] <  Left_Add_Line[i+2] && Right_Add_Line[i] >= Right_Add_Line[i+2])
//				{
//					Left_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
//				}
//				if (Right_Add_Line[i] > Right_Add_Line[i+2] && Left_Add_Line[i] <=  Left_Add_Line[i+2])
//				{
//					Right_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
//				}
//			}
                        
                       ////////////////////////////////////////////////
             if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}
			
			if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
					break;
				}
			}
		}
		/**************************** ���߼����� ****************************/
		
		/**************************** �ϰ���⿪ʼ ****************************/
		else
		{
                /**************************** �����߼�⿪ʼ ****************************/
                if (i>30 && Annulus_Check && i<59)
                { 
                        Starting_Line_Times=0;
                        Starting_Line_Time=0;
                        if (Last_Point >35 && Last_Point < 45)
                        {
                                for(j=Left_Line[i+2];j<Right_Line[i+2];j++)
                                {
                                        
                                        if (Starting_Line_Time==0)
                                        {
                                                if(!data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Times++;                                                        
                                                        Starting_Line_Time=1;
                                                }     
                                        }
                                        else
                                        {
                                               if(data[80*(i-2)+j])
                                                {
                                                        Starting_Line_Time=0;
                                                } 
                                        }
                                }
                                if(Starting_Line_Times>5 && Starting_Line_Times<15)
                                {
                                                Starting_Line_Flag=1;                                                
                                }

                        }
                  
                }
                /**************************** �����߼����� ****************************/
             if (i < 57 && i > 25 && Width_Real[i] < 50 && Annulus_Check && !Starting_Line_Flag && (Width_Real[i+2]-Width_Real[i])>6)
			{
				if (Right_Add_Line[i+2] - Right_Add_Line[i]<4 && (Left_Add_Line[i] > (Left_Add_Line[i+2] + 6)) && !Left_Add_Start)	// ������ǰһ����߽��нϴ�ͻ����û�в���
				{        
                    temps=0;
                    res=0;
                    for (j=Left_Add_Line[i+2];j<Left_Add_Line[i]+1;j++) //j<�����Ż�
                    {
                        if(temps==0)
                        {
                            if(data[80*(i-2) + j] && !data[80*(i-2) + j+1])//�����׵�
                            {
                                temps=j;
                            }   
                        }
                        else
                        {
                            if(!data[80*(i-2) + j])
                            {
                                res++;
                            }
                        }
                    }
                    if(res>5 && res<15)
                    {
                        Hazard_Flag=temps;
                    }
                    if(Hazard_Flag && Left_Add_Line[i] - Hazard_Flag > 4) //�����жϿ��Բ�Ҫ
                    {
                        Left_Hazard_Flag = i;	// �ϰ����־λ��λ
//                        save_var(VAR3, Left_Hazard_Flag);
                    }

				}
				else if (Left_Add_Line[i] - Left_Add_Line[i+2] <4 && Right_Add_Line[i] < Right_Add_Line[i+2] - 6 && !Right_Add_Start)	// ������ǰһ���ұ߽��нϴ�ͻ����û�в���
				{
                    temps=0;
                    res=0;
                    for (j=Right_Add_Line[i+2];j>Right_Add_Line[i]-1;j--) //j<�����Ż�
                    {
                        if(temps==0)
                        {
                            if(data[80*(i-2) + j] && !data[80*(i-2) + j-1])//�����׵�
                            {
                                temps=j;
                            }   
                        }
                        else
                        {
                            if(!data[80*(i-2) + j])
                            {
                                res++;
                            }
                        }
                    }
                    if(res>5 && res<15)
                    {
                        Hazard_Flag=temps;
                    }
                    if(Hazard_Flag && Hazard_Flag - Right_Add_Line[i]> 4) //�����жϿ��Բ�Ҫ
                    {
                        Right_Hazard_Flag = i;	// �ϰ����־λ��λ
//                        save_var(VAR3, Right_Hazard_Flag);
                    }
				}
			}
		}
		/**************************** �ϰ������� ****************************/
		
		/*************************** ��һ�ֲ��߿�ʼ ***************************/
		if (Left_Add_Flag[i])	// �����Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// ��¼���߿�ʼ��
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// ʹ��ǰһ֡ͼ����߽�б�ʲ���
			}
			else
			{
				if (!Left_Add_Start)	// ֮ǰû�в���
				{
					Left_Add_Start = i;	// ��¼��ಹ�߿�ʼ��
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// ʹ�����㷨���ֱ��
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// �������
			}
		}
		else
		{
			if (Left_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// ��¼��ಹ�߽�����
					}
				}
			}
		}
		
		if (Right_Add_Flag[i])	// �Ҳ���Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// ��¼���߿�ʼ��
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// ʹ��ǰһ֡ͼ���ұ߽�б�ʲ���
			}
			else
			{
				if (!Right_Add_Start)	// ֮ǰû�в���
				{
					Right_Add_Start = i;	// ��¼�Ҳಹ�߿�ʼ��
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// ʹ�����㷨���ֱ��
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// �������
			}
		}
		else
		{
			if (Right_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// ��¼�Ҳಹ�߽�����
					}
				}
			}
		}
		/*************************** ��һ�ֲ��߽��� ***************************/
		
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ��������
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// ������С�������
		}

		Line_Count = i;	// ��¼�ɹ�ʶ�𵽵���������
                if(Left_Add_Stop&&Right_Add_Stop)    //���߽�������Ҫ�����ⲻ��
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                        Line_Count = i+2;
                    }
                }
	
//        Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
//		if ((Left_Add_Flag[59] && Left_Add_Start && !Left_Add_Stop) && (Right_Add_Flag[59] && Right_Add_Start && !Right_Add_Stop))
//		{
//			Mid_Line[i] = Mid_Line[i+2];
//		}
//		else
//		{
//			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ��������
//		}
//		if (Width_Add[i] < Width_Min)
//		{
//			Width_Min = Width_Add[i];	// ������С�������
//		}
//
//		Line_Count = i;	// ��¼�ɹ�ʶ�𵽵���������
//                if(Left_Add_Stop&&Right_Add_Stop)    //���߽�������Ҫ�����ⲻ��
//                {
//                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
//                    {
//                        break;
//                        Line_Count = i+2;
//                    }
//                }
	}
	/*************************** �ڶ��ֲ����޸���ʼ ***************************/
	if (!Annulus_Flag)	// ���ǻ�·
	{
		if (Left_Add_Start)		// ��߽���Ҫ����
		{
			Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
		}
		if (Right_Add_Start)	// �ұ߽���Ҫ����
		{
			Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
		}
	}

	/*************************** �ڶ��ֲ����޸����� ***************************/
	
	/****************************** �����޸���ʼ ******************************/
	Mid_Line_Repair(Line_Count, data);
	/****************************** �����޸����� ******************************/
    
    if(Ramp_Continue_Flag)  //�����µ����
    {
        Ramp_Check();
    }    
}

/*
*	����ƽ��
*
*	˵�����������⴦��ֱ��ȡ��ֵ
*/
uchar Point_Average(void)
{
	uchar i, Half_Line;
	int32 Point, Sum = 0;
	static uchar Last_Point = 40;
	
	if (Out_Side || Line_Count >= 55)	// �����������ͷͼ���쳣
	{
		Point  = Last_Point;			// ʹ���ϴ�Ŀ���
	}
	else
	{
		Half_Line = 59-(59-Line_Count)/2;	// ������Ч�е��м���
		if (Half_Line%2 == 0)
		{
			Half_Line += 1;
		}
		if (Left_Max + 12 < Right_Min)
		{
			Point = (Left_Max + Right_Min) / 2;
		}
		else
		{
			if (Line_Count <= 21)
			{
				Line_Count = 21;
			}
			for (i = 61; i >= Line_Count; )
			{
				i -= 2;
				Sum += Mid_Line[i];
			}
			Point = Sum / ((61-Line_Count)/2);	// ��������ƽ��
			if (Left_Add_Line[Half_Line] >= 20)				// ̫ƫ��
			{
				Point += (Left_Add_Line[Half_Line]-20);	// �ʵ�����
			}
			if (Right_Add_Line[Half_Line] <= 60)				// ̫ƫ��
			{
				Point -= (60-Right_Add_Line[Half_Line]);	// �ʵ�����
			}
		}
		
		Point = Point*0.9 + Last_Point*0.1;	// ��ͨ�˲�
		Point = range_protect(Point, 1, 79);		// �޷�����ֹ�������
		
		/*** �ϰ�������������� ***/
//		if (Left_Hazard_Flag)			//������ϰ�������Ҫ���ߣ���ʹ����Ҳ�������Ӱ��
//		{
//			Point = Mid_Line[Left_Hazard_Flag]+6;	//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
//			if (Left_Hazard_Flag < 40)
//			{
//				Point += 3;
//			}
//		}
//		else if (Right_Hazard_Flag)	//�Ҳ����ϰ�������Ҫ���ߣ���ʹ����Ҳ�������Ӱ��
//		{
//			Point = Mid_Line[Right_Hazard_Flag]-6;//ʹ���ϰ�����ֵ���һ���е���ΪĿ���
//			if (Right_Hazard_Flag < 40)
//			{
//				Point -= 5;
//			}
//		}
		Last_Point = Point;	// �����ϴ�Ŀ���
	}
	
	return Point;
}

/*
*	��С���˷�����б��
*
*	˵��������б����Ϊת�������,����Ŵ�100��
*/
int32 Least_Squares(void)
{
	int32 Ave_x, Ave_y, Sum1, Sum2;
	uchar i;
	float f_Slope;
	int32 i_Slope;
	static int32 i_Last_Slope = 0;
	
	for (i = 61; i >= Line_Count; )
	{
		i -= 2;
	}
	i_Last_Slope = i_Slope;
	
	return i_Slope;
}

/*
*	б�ʼ���
*
*	˵����ʹ��б����Ϊת�������������б�ʵĵ���
*/
float Slope_Weight(uchar *Mid)
{
	float Slope;
	
	Slope = 1.0 * (Mid[Line_Count] - 40) / (60-Line_Count);
	
	return Slope;
}

/*
*	�����Ƕȼ���
*
*	˵�������ؽ��Ϊ �ң�0��180�㣬1��135�㣬2��90�㣬>= 1����Ϊֱ��ͻ��
*					 ��0��180�㣬-1��135�㣬-2��90�㣬<= -1����Ϊֱ��ͻ��
*/
char Calculate_Angle(uchar Point_1, uchar Point_2, uchar Point_3)
{
	char K1, K2,Result;
	
	K1 = Point_2 - Point_1;
	K2 = Point_3 - Point_2;
	
	Result = (K2 - K1);
	
	return Result;
}

/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
*/
void Curve_Fitting(float *Ka, float *Kb, uchar *Start, uchar *Line, uchar *Add_Flag, uchar Mode)
{
	*Start += 4;
	if (Add_Flag[*Start] == 2)
	{
		if (*Start <= 51)
		{
			*Start += 2;
		}
		*Ka = 1.0*(Line[*Start+4] - Line[*Start]) / 4;
		if (Mode == 2)
		{
			if (*Ka < 0)
			{
				*Ka = 0;
			}
		}
		if (Mode == 1)
		{
			if (*Ka > 0)
			{
				*Ka = 0;
			}
		}
	}
	else
	{
		*Ka = 1.0*(Line[*Start+4] - Line[*Start]) / 4;
	}
	*Kb = 1.0*Line[*Start] - (*Ka * (*Start));
}

/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
*/
void Curve2_Fitting(float *Ka, float *Kb, uchar Start,uchar End, uchar *Line, uchar Mode)
{
        if (Mode==1)
        {
             *Ka = 1.0*((Line[Start]+21) - Line[End]) / (Start-End);
             *Kb = 1.0*Line[End] - (*Ka * End);   
        }
        else
        {
              *Ka = 1.0*((Line[Start]-21) - Line[End]) / (Start-End);
              *Kb = 1.0*Line[End] - (*Ka * End);   
        }
	
}

/*
*	���㲹������
*
*	˵����ʹ�����㷨������ϳ��Ĳ�������
*/
uchar Calculate_Add(uchar i, float Ka, float Kb)	// ���㲹������
{
	float res;
	uchar Result;
	
	res = i * Ka + Kb;
	Result = range_protect((int32)res, 1, 79);
	
	return Result;
}

/*
*	��ͷ����
*
*	˵������ĳһ�㿪ʼ��ֱ����������������Զ������
*/
uchar Limit_Scan(uchar i, uchar *data, uchar Point)
{
	for ( ; i >= 20; i--)
	{
		if (!data[80*i + Point])	// �������ڵ�
		{
			break;
		}
	}
	
	return i;	// ������Զ������
}

/*
*	��һ�����⴦��
*
*	˵������ʹ�õ�60���е���Ϊ��59��(��һ��)������ʼλ�ã��ɹ����������ұ߽��
 	����59���е㸳ֵ����60�б�����һ֡ͼ��ʹ�á������60���е��ڱ���Ϊ�ڵ㣬��
 	�ֱ�ʹ����������ұ����ķ��������߽磬��������Ƚϴ�Ľ����Ϊ��59�б߽磬
 	����Ȼ���������߽�������쳣��Ϊ���磬���緵��0
*/
uchar First_Line_Handle(uchar *data)
{
	uchar i;	// ������
	uchar Weight_Left, Weight_Right;	// �����������
	uchar Mid_Left, Mid_Right;
	Image_Para_Init();
	i = 59;

    
                        
	if (!data[i*80 + Mid_Line[61]])	// ��61���е��ڵ�59��Ϊ�ڵ�
	{
		Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, 79);	// ����������߽�
		Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, 79);// ���Ҳ������߽�
		if (Weight_Left >= Weight_Right && Weight_Left)	// ��������ȴ�������������Ҳ�Ϊ0
		{
			Traversal_Left_Line(i, data, Left_Line, Right_Line);	// ʹ���������ȡ�����߽�
		}
		else if (Weight_Left < Weight_Right && Weight_Right)
		{
			Traversal_Right_Line(i, data, Left_Line, Right_Line);	// ʹ���ұ�����ȡ�����߽�
		}
		else	// ˵��û�鵽
		{
			return 0;
		}
	}
	else
	{
		Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 79, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);	// ��ǰһ���е�������ɨ��
	}
	
      
	Left_Line[61] = Left_Line[59];
	Right_Line[61] = Right_Line[59];
	Left_Add_Line[61] = Left_Add_Line[59];
	Right_Add_Line[61] = Right_Add_Line[59];
	if (Left_Add_Flag[59] && Right_Add_Flag[59])
	{
		Mid_Line[59] = Mid_Line[61];
	}
	else
	{
		Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
		Mid_Line[61] = Mid_Line[59];	// ���µ�60�������е㣬������һ֡ͼ��ʹ��
	}
	Width_Real[61] = Width_Real[59];
	Width_Add[61] = Width_Add[59];
	Width_Min = Width_Add[59];
	
	return 1;
}

/*
*	����࿪ʼ�����߽磬�����������
*
*	˵����������������Ϊ̽������ʹ�ã�������������ȣ�������߽�����
*/
uchar Traversal_Left(uchar i, uchar *data, uchar *Mid, uchar Left_Min, uchar Right_Max)
{
	uchar j, White_Flag = 0;
	uchar Left_Line = Left_Min, Right_Line = Right_Max;
	
	for (j = Left_Min; j <= Right_Max; j++)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ�����߽�
		{
			if (data[i*80 + j])	// ��⵽�׵�
			{
				Left_Line = j;	// ��¼��ǰjֵΪ������߽�
				White_Flag = 1;	// ��߽����ҵ��������ұ߽�
				
				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if (!data[i*80 + j])//���ڵ�
			{
				Right_Line = j-1;//��¼��ǰjֵΪ�����ұ߽�
				
				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	
	if (!White_Flag)	// δ�ҵ����ұ߽�
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;
		
		return (Right_Line - Left_Line);
	}
}

/*
*	���Ҳ࿪ʼ�����߽磬�����������
*
*	˵����������������Ϊ̽������ʹ�ã�������������ȣ�������߽�����
*/
uchar Traversal_Right(uchar i, uchar *data, uchar *Mid, uchar Left_Min, uchar Right_Max)
{
	uchar j, White_Flag = 0;
	uchar Left_Line = Left_Min, Right_Line = Right_Max;
	
	for (j = Right_Max; j >= Left_Min; j--)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ����ұ߽�
		{
			if (data[i*80 + j])	// ��⵽�׵�
			{
				Right_Line = j;	// ��¼��ǰjֵΪ�����ұ߽�
				White_Flag = 1;	// �ұ߽����ҵ���������߽�
				
				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if (!data[i*80 + j])	//���ڵ�
			{
				Left_Line = j+1;	//��¼��ǰjֵΪ������߽�
				
				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	
	if (!White_Flag)	// δ�ҵ����ұ߽�
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;
			
		return (Right_Line - Left_Line);
	}
}

/*
*	����࿪ʼ�����߽磬���������߽磬����1�ɹ� 0ʧ��
*
*	˵����������ʹ�ú󽫱���߽�����
*/
uchar Traversal_Left_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	uchar j, White_Flag = 0;
	
	Left_Line[i] = 1;
	Right_Line[i] = 79;
	
	for (j = 1; j < 80; j++)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ�����߽�
		{
			if (data[i*80 + j])	// ��⵽�׵�
			{
				Left_Line[i] = j;	// ��¼��ǰjֵΪ������߽�
				White_Flag = 1;		// ��߽����ҵ��������ұ߽�
				
				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if (!data[i*80 + j])	//���ڵ�
			{
				Right_Line[i] = j-1;//��¼��ǰjֵΪ�����ұ߽�
				
				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	
	return White_Flag;	// �����������
}

/*
*	���Ҳ࿪ʼ�����߽磬���������߽磬����1�ɹ� 0ʧ��
*
*	˵����������ʹ�ú󽫱���߽�����
*/
uchar Traversal_Right_Line(uchar i, uchar *data, uchar *Left_Line, uchar *Right_Line)
{
	uchar j, White_Flag = 0;
	
	Left_Line[i] = 1;
	Right_Line[i] = 79;
	
	for (j = 79; j > 0; j--)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ����ұ߽�
		{
			if (data[i*80 + j])	// ��⵽�׵�
			{
				Right_Line[i] = j;	// ��¼��ǰjֵΪ�����ұ߽�
				White_Flag = 1;		// �ұ߽����ҵ���������߽�
				
				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if (!data[i*80 + j])	//���ڵ�
			{
				Left_Line[i] = j+1;//��¼��ǰjֵΪ������߽�
				
				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	
	return White_Flag;	// �����������
}

/*
*	���м������������߽�
*
*	˵����������ʹ�ú󽫱���߽�����
*/
//Traversal_Mid_Line(i, data, Mid_Line[i+2], 1, 79, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
void Traversal_Mid_Line(uchar i, uchar *data, uchar Mid, uchar Left_Min, uchar Right_Max, uchar *Left_Line, uchar *Right_Line, uchar *Left_Add_Line, uchar *Right_Add_Line)
{
	uchar j;
	
	Left_Add_Flag[i] = 1;	// ��ʼ�����߱�־λ
	Right_Add_Flag[i] = 1;
	
////	Left_Min = range_protect(Left_Min, 1, 79);	// �޷�����ֹ����
//	if (Left_Add_Flag[i+2])
//	{
//		Left_Min = range_protect(Left_Add_Line[i+2]-10, 1, 79);
//	}
////	Right_Max = range_protect(Right_Max, 1, 79);
//	if (Right_Add_Flag[i+2])
//	{
//		Right_Max = range_protect(Right_Add_Line[i+2]+10, 1, 79);
//	}
	
	Right_Line[i] = Right_Max;
	Left_Line[i] = Left_Min;	// �����߽��ʼֵ
	
	for (j = Mid; j >= Left_Min+1; j--)	// ��ǰһ���е�Ϊ���������ұ߽�
	{
		if (!data[i*80 + j]&&!data[i*80 + j-1])	// ��⵽�ڵ�
		{
			Left_Add_Flag[i] = 0;	//��߽粻��Ҫ���ߣ������־λ
			Left_Line[i] = j;	//��¼��ǰjֵΪ����ʵ����߽�
			Left_Add_Line[i] = j;	// ��¼ʵ����߽�Ϊ������߽�
			
			break;
		}
	}
	for (j = Mid; j <= Right_Max-1; j++)	// ��ǰһ���е�Ϊ������Ҳ����ұ߽�
	{
		if (!data[i*80 + j]&&!data[i*80 + j+1])	//��⵽�ڵ�
		{
			Right_Add_Flag[i] = 0;		//�ұ߽粻��Ҫ���ߣ������־λ
			Right_Line[i] = j;	//��¼��ǰjֵΪ�����ұ߽�
			Right_Add_Line[i] = j;	// ��¼ʵ���ұ߽�Ϊ������߽�
			
			break;
		}
	}
	if (Left_Add_Flag[i])	// ��߽���Ҫ����
	{
		if (i >= 55)	// ǰ6��
		{
			Left_Add_Line[i] = Left_Line[59];	// ʹ�õ�������
		}
		else                     ///
		{
			Left_Add_Line[i] = Left_Add_Line[i+2];	// ʹ��ǰ2����߽���Ϊ������߽�
		}
	}
	if (Right_Add_Flag[i])	// �ұ߽���Ҫ����
	{
		if (i >= 55)	// ǰ6��
		{
			Right_Add_Line[i] = Right_Line[59];	// ʹ�õ�������
		}
		else         //////
		{
			Right_Add_Line[i] = Right_Add_Line[i+2];	// ʹ��ǰ2���ұ߽���Ϊ�����ұ߽�
		}
	}
	Width_Real[i] = Right_Line[i] - Left_Line[i];			// ����ʵ���������
	Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���㲹���������
	
}

/*
*	�����޸�
*
*	˵������ʼ���ղ�ʹ�ã�ֱ��ʹ������б�ʽ��в���
*/
/*
*	�����޸�
*
*	˵������ʼ���ղ�ʹ�ã�ֱ��ʹ������б�ʽ��в���
*/
void Line_Repair(uchar Start, uchar Stop, uchar *data, uchar *Line, uchar *Line_Add, uchar *Add_Flag, uchar Mode)
{
	float res;
	uchar i;	// ������
	float Ka, Kb;
	
	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// ��߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, 79); 
			Width_Add[i] = Width_Add[i+2];
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// �ұ߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, 79); 
			Width_Add[i] = Width_Add[i+2];
		}
	}
	else
	{
		if (Stop)	// ��ʼ����
		{
			if ((Right_Add_Stop >= 47 && Left_Add_Stop >= 47) || (Right_Add_Stop >= 47 && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= 47 && Right_Add_Start <= Left_Add_Stop))	// ֻ�н�������Ҫ���ߣ�������б�ʣ�ֱ����ֱ���²���
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
//			else if (Right_Add_Start >= 57 && Left_Add_Start >= 57)	// ������ʼ�нϽ����������²���
//			{
//				Ka = 1.0*(Line_Add[Stop] - Line_Add[Line_Count]) / (Stop - Line_Count);
//				Kb = 1.0*Line_Add[Stop] - (Ka * Stop);
//				
//				for (i = Stop; i <= 57; )
//				{
//					i += 2;
//					res = i * Ka + Kb;
//					Line_Add[i] = range_protect((int32)res, 1, 159);
//				}
//			}
			else	// ����ʼ�кͽ����м���б�ʲ���
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);
				
				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, 79);
				}
			}
		}
	}
}


void Mid_Line_Repair(uchar count, uchar *data)
{
	int res;
	uchar i,j;	// ������
    Left_Max = 0;
	Right_Min = 79;
    
	for (i = 59; i >= count;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ���������е�
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// �����������
        if (i>21)
        {
            if (Left_Add_Line[i] > Left_Max)
            {
                Left_Max = Left_Add_Line[i];
            }
            if (Right_Add_Line[i] < Right_Min)
            {
                Right_Min = Right_Add_Line[i];
            }	
        }
        //�ϰ�����
                if(Left_Hazard_Flag)
                {
                      Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1);
                      for (j=59;j>=Left_Hazard_Flag;j--)
                      {
                           Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// �������
                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
                      }
                }
                if(Right_Hazard_Flag)
                {
                      Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2);
                      for (j=59;j>=Right_Hazard_Flag;j--)
                      {
                           Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// �������  
                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
                      }
                
                }

#if   Display_OFF_ON	
	/**//*************************** ��λ����ʾ�߽� ***************************/
	/**/data[i*80 + Left_Add_Line[i] + 4] = 0;	// ��λ����ʾ���ߺ����߽磬����ʱ����
	/**/data[i*80 + Right_Add_Line[i] - 4] = 0;	// ��λ����ʾ���ߺ���ұ߽磬����ʱ����
	/**/data[i*80 + Mid_Line[i]] = 0;			// ��λ����ʾ���ߣ�����ʱ����
	/**/data[i*80 + Left_Line[i] + 2] = 0;		// ��λ����ʾԭʼ��߽磬����ʱ����
	/**/data[i*80 + Right_Line[i] - 2] = 0;		// ��λ����ʾԭʼ�ұ߽磬����ʱ����
	/**//*************************** ��λ����ʾ�߽� ***************************/
#endif
	}

	Mid_Line[61] = Mid_Line[59];
}

///*
//*	�����޸�
//*
//*	˵������ͨ�������ʹ��ƽ��������ʽ���е㵽��߽����
//*/
//void Mid_Line_Repair(uchar count, uchar *data)
//{
//	char res;
//	uchar i,j,Left_Hazard,Right_Hazard;	// ������
//	float Ka, Kb;
//        
//        Left_Hazard = 0;
//        Right_Hazard = 0;
//	
//	Left_Max = Left_Add_Line[59];
//	Right_Min = Right_Add_Line[59];
//	
//	for (i = 61; i >= count+2; )
//	{
//		i -= 2;
//		if (Left_Add_Flag[i] && Right_Add_Flag[i])	// ���Ҷ���Ҫ����
//		{
//			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ���������е�
//			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// �����������
//		}
//		else if (Left_Add_Flag[i])	// �������Ҫ����
//		{
//			if (Left_Add_Line[i] == 1 && !Left_Add_Stop)	// ���������ˣ����߱��ο���ֵ
//			{
//			res = Right_Add_Line[i] - Width_Add[i+2] / 2 + (Right_Add_Line[i+2] - Right_Add_Line[i]) / 2;	// ƽ���������һ��
//				Width_Add[i] = 2 * (Right_Add_Line[i] - res);	// �����ұ߽�����������������
//				if (res <= 0)	// �е��ѵ���ͼ���ޣ����پ߱��ο���ֵ
//				{
//					i += 2;	// ��¼�ɹ�ʶ�𵽵��е�����
//					break;
//				}
//			}
//			else
//			{
//				res = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;		// ���������е�
//				Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// �����������
//			}
//			Mid_Line[i] = res;	// �����е�
//		}
//		else if (Right_Add_Flag[i])	// ���Ҳ���Ҫ����
//		{
//			if (Right_Add_Line[i] == 79 && !Right_Add_Stop)	// ���������ˣ����߱��ο���ֵ
//			{
//				res = Left_Add_Line[i] + Width_Add[i+2] / 2 - (Left_Add_Line[i]-Left_Add_Line[i+2]) / 2;	// ƽ���������һ��
//				Width_Add[i] = 2 * (res - Left_Add_Line[i]);	// �������ߺ���߽������������
//				if (res >= 80)	// �е��ѵ���ͼ���ޣ����پ߱��ο���ֵ
//				{
//					i += 2;	// ��¼�ɹ�ʶ�𵽵��е�����
//					break;
//				}
//			}
//			else
//			{
//				res = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;		// ���������е�
//				Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// �����������
//			}
//			Mid_Line[i] = res;	// �����е�
//		}
//                //�ϰ�����
//                if(Left_Hazard_Flag)
//                {
//                      Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1);
//                      for (j=59;j>=Left_Hazard_Flag;j--)
//                      {
//                           Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// �������
//                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//                      }
//                }
//                if(Right_Hazard_Flag)
//                {
//                      Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2);
//                      for (j=59;j>=Right_Hazard_Flag;j--)
//                      {
//                           Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// �������  
//                           Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//                      }
//                
//                }
//		if (Left_Add_Line[i] > Left_Max)
//		{
//			Left_Max = Left_Add_Line[i];
//		}
//		if (Right_Add_Line[i] < Right_Min)
//		{
//			Right_Min = Right_Add_Line[i];
//		}
//
//#if   Display_OFF_ON		
//	/**//*************************** ��λ����ʾ�߽� ***************************/
//	/**/data[i*80 + Left_Add_Line[i] + 2] = 0;	// ��λ����ʾ���ߺ����߽磬����ʱ����
//	/**/data[i*80 + Right_Add_Line[i] - 2] = 0;	// ��λ����ʾ���ߺ���ұ߽磬����ʱ����
//	/**/data[i*80 + Mid_Line[i]] = 0;			// ��λ����ʾ���ߣ�����ʱ����
////	/**/data[i*80 + Left_Line[i] + 2] = 0;		// ��λ����ʾԭʼ��߽磬����ʱ����
////	/**/data[i*80 + Right_Line[i] - 2] = 0;		// ��λ����ʾԭʼ�ұ߽磬����ʱ����
//	/**//*************************** ��λ����ʾ�߽� ***************************/
//#endif
//	}
//	Mid_Line[61] = Mid_Line[59];
//	Line_Count = i;		// ��¼�ɹ�ʶ�𵽵��е�����
//	
//	if (Left_Add_Stop)
//	{
//		Curve22_Fitting(&Left_Ka, &Left_Kb, Left_Add_Stop, Left_Add_Line);
//	}
//	if (Right_Add_Stop)
//	{
//		Curve22_Fitting(&Right_Ka, &Right_Kb, Right_Add_Stop, Right_Add_Line);
//	}
//}

/****************** ���㷨 ******************/

/*
*	��Ȩƽ��
*
*	˵����Ȩ�����Ҵյģ�Ч������
*/
uchar  Motor_Point_Weight(int8 Num)
{
	uchar i ,Point, Point_Mid;
    uchar Count=0;
    int8   Point_id;
	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;

        if (Line_Count <= 15)
            {
              Line_Count = 15;
            }
//        for (i = 61; i >= Line_Count+2; )		//ʹ�ü�Ȩƽ��
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
//		Point = range_protect(Sum / Weight_Count, 2, 78);
        Count = 60-Line_Count;
        if(Count <= Num)
        {
            
        }
        for (i = 59; i >= Line_Count-Num;i-=2)		//ʹ�ü�Ȩƽ��
		{	
            Point_id = i-Num;
            if (Point_id>59)Point_id=59;
			Sum += Mid_Line[Point_id] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
        
        
//        for (i = 61; i >= Line_Count+2; )		//ʹ�ü�Ȩƽ��
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
         //////////////////////////////// 
#if   SSSSSSS_OFF_ON	
         S_temp=0;
         for (i = 57; i >= Line_Count+2; )		//���СS���
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>(Mid_Line[i+4]+1))
                {
                    tep=1;
                    S_temp++;
                }
                else if ((Mid_Line[i]+1)<Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if ((Mid_Line[i]+1)<Mid_Line[i+2])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>(Mid_Line[i+2]+1))
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//        save_var(VAR2, S_temp);
         ////////////////
        if (S_temp>=3 && !Annulus_Flag)
        {
             for (i = 61; i >= Line_Count+2; )		//����СS���ƫ��
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            }
            Point = (R_Max + L_Min) / 2;
        }
#endif
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		

        Point_Mid = Mid_Line[Line_Count+2];

	
	return Point;
}
/*
*	��Ȩƽ��
*
*	˵����Ȩ�����Ҵյģ�Ч������
*/
uchar Line_Num=0;
uchar Dynamic_Point_Weight(int8 Num)
{
	uchar i ,Point, Point_Mid;
    uchar Count=0;
    int8   Point_id;
	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;
	if (Out_Side || Line_Count >= 57)	//�����������ͷͼ���쳣
	{
//        Line_Num++;
		Point  = Last_Point;
               // Speed=0;
//                if(Annulus_Check && Line_Num>=1)
//                {
//                   Stop_Flag = 1;
//                }
	}
    /********************************������ͣ������*******************************/
    if(Starting_Line_Flag)
    {
//            Line_Num++;
            Point  = Last_Point;
           // Speed=0;
            if(Annulus_Check)
            {
                    Stop_Flag = 1;
            }
    }
    /**********************������ͣ������********************/
	else
	{
        if (Line_Count <= 20)
            {
              Line_Count = 21;
            }
//        for (i = 61; i >= Line_Count+2; )		//ʹ�ü�Ȩƽ��
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
//		Point = range_protect(Sum / Weight_Count, 2, 78);
        Count = 60-Line_Count;
        if(Count <= Num)
        {
            
        }
        for (i = 59; i >= Line_Count-Num;i-=2)		//ʹ�ü�Ȩƽ��
		{	
            Point_id = i-Num;
            if (Point_id>59)Point_id=59;
			Sum += Mid_Line[Point_id] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
        
        
//        for (i = 61; i >= Line_Count+2; )		//ʹ�ü�Ȩƽ��
//		{
//			i -= 2;
//			Sum += Mid_Line[i] * Weight[59-i];
//			Weight_Count += Weight[59-i];
//		}
         //////////////////////////////// 
#if   SSSSSSS_OFF_ON	
         S_temp=0;
         for (i = 57; i >= Line_Count+2; )		//���СS���
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>(Mid_Line[i+4]+1))
                {
                    tep=1;
                    S_temp++;
                }
                else if ((Mid_Line[i]+1)<Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if ((Mid_Line[i]+1)<Mid_Line[i+2])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>(Mid_Line[i+2]+1))
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//        save_var(VAR2, S_temp);
         ////////////////
        if (S_temp>=3 && !Annulus_Flag)
        {
             for (i = 61; i >= Line_Count+2; )		//����СS���ƫ��
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            }
            Point = (R_Max + L_Min) / 2;
        }
#endif
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		
							/***** ʹ����Զ�����ݺ�Ŀ�����Ϊǰհ *****/
//		if (Line_Count >= 27)
//		{
//			Point_Mid = Mid_Line[31];
//		}
//		else
//		{
//			Point_Mid = Mid_Line[Line_Count+2];
//		}
                Point_Mid = Mid_Line[Line_Count+2];
                //Ek_compar=(Mid_Line[Line_Count]+Mid_Line[Line_Count+8])>>1;
                //Ek_compar=Mid_Line[Line_Count+4]-Ek_compar;
	}
//        if (Line_Count <= 30)
//        {
//            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
//			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
//	else
//        {
            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
            
           Last_Foresight=Foresight;
	
	return Point;
}

/*
*	��Ȩƽ��
*
*	˵����Ȩ�����Ҵյģ�Ч������
*/
uchar Point_Weight(void)
{
	uchar i ,Point, Point_Mid;

	int32 Sum = 0, Weight_Count = 0;
	uchar tep=0,L_Min=40,R_Max=40;
	if (Out_Side || Line_Count >= 57)	//�����������ͷͼ���쳣
	{
		Point  = Last_Point;
               // Speed=0;
                if(Annulus_Check)
                {
                   Stop_Flag = 1;
                }
	}
        if(Starting_Line_Flag)
        {
                Point  = Last_Point;
               // Speed=0;
                if(Annulus_Check)
                {
                        Stop_Flag = 1;
                }
        }
	else
	{
        if (Line_Count <= 20)
            {
              Line_Count = 21;
            }
         //////////////////////////////// 
            S_temp=0;
         for (i = 59; i >= Line_Count+2; )		//���S���
		{
			i -= 2;
            if(tep==0)
            {
                if (Mid_Line[i]>Mid_Line[i+4]+2)
                {
                    tep=1;
                    S_temp++;
                }
                else if (Mid_Line[i]+2<=Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==1)
            {
                 if (Mid_Line[i]+2<=Mid_Line[i+4])
                {
                    tep=2;
                    S_temp++;
                }
            }
            if (tep==2)
            {
                 if (Mid_Line[i]>Mid_Line[i+4]+2)
                {
                    tep=1;
                    S_temp++;
                }
            }
            
        }
//            save_var(VAR2, S_temp);
        ///////////////////////////////
		for (i = 61; i >= Line_Count+2; )		//ʹ�ü�Ȩƽ��
		{
			i -= 2;
			Sum += Mid_Line[i] * Weight[59-i];
			Weight_Count += Weight[59-i];
		}
		Point = range_protect(Sum / Weight_Count, 2, 78);
         ////////////////
        if (S_temp>=3)
        {
             for (i = 61; i >= Line_Count+2; )		//����S���ƫ��
            {
                i -= 2;
                if(L_Min> Mid_Line[i])
                {
                    L_Min = Mid_Line[i];
                }
                if (R_Max < Mid_Line[i])
                {
                    R_Max = Mid_Line[i];
                }
            } 
            Point = (L_Min + R_Max) / 2;
        }
        ////////////////////////
		Point = range_protect(Point, 2, 78);
		Last_Point = Point;
		
							/***** ʹ����Զ�����ݺ�Ŀ�����Ϊǰհ *****/
//		if (Line_Count >= 27)
//		{
//			Point_Mid = Mid_Line[31];
//		}
//		else
//		{
//			Point_Mid = Mid_Line[Line_Count+2];
//		}
                Point_Mid = Mid_Line[Line_Count+2];
                //Ek_compar=(Mid_Line[Line_Count]+Mid_Line[Line_Count+8])>>1;
                //Ek_compar=Mid_Line[Line_Count+4]-Ek_compar;
	}
//        if (Line_Count <= 30)
//        {
//            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
//			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
//	else
//        {
            Foresight = 0.8 * Error_Transform(Point_Mid, 40)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
			  + 0.2 * Error_Transform(Point,  40);//+0.1*Ek_compar;
//        }
	
	return Point;
}

char Error_Transform(uchar Data, uchar Set)   //�����ֵ
{
	char Error;
	
	Error = Set - Data;
	if (Error < 0)
	{
		Error = -Error;
	}
	
	return Error;
}
int32 abs_int(int32 i)//���ã� �����ֵ  int32��
{
     int32 B;
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


/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb
*/
void Curve22_Fitting(float *Ka, float *Kb, uchar Start, uchar *Line)
{
	*Ka = 1.0*(Line[Start+4] - Line[Start]) / 4;
	*Kb = 1.0*Line[Start] - (*Ka * Start);
}



/*!
*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst             ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}

/*!
*  @brief      ��ֵ��ͼ��ѹ�����ռ� �� ʱ�� ѹ����
*  @param      dst             ͼ��ѹ��Ŀ�ĵ�ַ
*  @param      src             ͼ��ѹ��Դ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_recontract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  
  while(srclen --)
  {
    tmpsrc=0;
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x80;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x40;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x20;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x10;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x08;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x04;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x02;
    }
    
    if(*dst++ == colour[black])
    {
      tmpsrc = tmpsrc + 0x01;
    }
    
    *src++ = tmpsrc;
  }
}

/*!
*  @brief      ȡ�߽���
*  @param      dst             ͼ��ѹ��Ŀ�ĵ�ַ
*  @param      src             ͼ��ѹ��Դ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_getline(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc,buff_y,buff_x;
  
  while(srclen --)
  {
    tmpsrc=0;
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x80;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x40;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x20;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x10;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x08;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x04;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if(*dst != buff_y || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x02;
      *(dst-1)=colour[black];
    }
    
    buff_x = *(dst + 80);
    buff_y = *dst++;
    *(dst-1)=colour[white];
    if((*dst != buff_y && srclen % 10 !=0 ) || (buff_y != buff_x && srclen > 9))
    {
      tmpsrc = tmpsrc + 0x01;
      *(dst-1)=colour[black];
    }
    
    *src++ = tmpsrc;
  }
}

void image_processing(uchar *data)
{
    if (!Annulus_Flag)
    {
      Image_Handle(data);
    }
    else
    {
        Annulus2_Handle(data);
    }
    
}

/**************************** �µ���⿪ʼ ****************************/
void Ramp_Check(void)
{
    uint8 i,j,Ramp_Count,line_Count;
    uint16 Ramp_Error_Sum=0;
    Ramp_Flag = 0;
    Ramp_Count = 0;
    line_Count = 0;
    i = 0;
    
    for(j=41;j>=21;)
    {
      j -= 2;
      if(Last_Foresight<=3&&!Left_Add_Flag[j]&&!Right_Add_Flag[j])
      {
        if (Left_Line[j] >= Left_Line[j+2]&&Right_Line[j] <= Right_Line[j+2]&&Left_Line[j]>Left_Line[59]&&Right_Line[j]<Right_Line[59])	//��ǰһ�еı߽�ʵ�߱Ƚ�	
        {
            Ramp_Error[i]=Width_Real[j]-Width_Max[j];
            if(Ramp_Error[i]>=20)
              Ramp_Error[i]=0;
            else if(Ramp_Error[i]<=0)
              Ramp_Error[i]=0;
          if(Width_Real[j]>Width_Max[j])
            Ramp_Count++;
        }
      }
      Ramp_Error_Sum+=Ramp_Error[i];
      i++;
    }
    //Mid_Line_K = 1.0*(Mid_Line[59]-Mid_Line[21])/38;
    if(Ramp_Count>=5&&Ramp_Error_Sum>=14&&Ramp_Error[8]>=(Ramp_Error[0]+3))
    {
        Ramp_Flag = 1;
    }
    else
    {
        Ramp_Flag = 0;
    }
}
/**************************** �µ���⿪ʼ ****************************/


