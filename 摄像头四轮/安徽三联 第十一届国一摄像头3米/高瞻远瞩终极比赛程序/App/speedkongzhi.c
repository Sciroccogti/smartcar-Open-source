#include "include.h"
#include "common.h"
#include "math.h"
//#define dawan_speed 49
//dawan_speed ------User_parameter[0];
int dawan_speed=48;
int longs=63;
int dwucha,speed_wucha,zhidao_flag,lastspeed_wucha,lastspeed_wucha1,lastspeed_wucha2,lastspeed_wucha3,last_zhankongbi,mubiao_speed,qvlv_quanju,qulv_jinduan,qulv_yuandaun;
extern int kp,quanjuCount,my_piancha,maichongshu,qulv_jinduan_right,qulv_jinduan_left,qulv_yuandaun_right,qulv_yuandaun_left,qvlv_quanju_right,qvlv_quanju_left;
extern char podao_flag,time,star_lineflag,qulv_point,L,R;
char changzhidao_flag,zhidao_ruwan_flag,zhongwan_flag,dawan_flag=0,xiaowan_flag=0;


void zhengzhuan(int zheng)//��ת
{
ftm_pwm_duty(FTM0,FTM_CH5,zheng);  //����ռ�ձ�
ftm_pwm_duty(FTM0,FTM_CH4,0); 
}

void fanzhuan(int fan)//��ת
{
ftm_pwm_duty(FTM0,FTM_CH5,0);  //����ռ�ձ�
ftm_pwm_duty(FTM0,FTM_CH4,fan); 
}



/************************************·���ж�***************************************************/
int speed_kp=65,speed_kd=4,zhankongbi,speed_ki=10;/////////////////�ٶ�PID����


void luduan_panduan()//·���ж�
{
 
    if(L||R)//�ϰ�
   {
     mubiao_speed=dawan_speed-4;
     dawan_flag=0;
   }
 else
    if(podao_flag&&podao_flag<60)//�µ�
{
  dawan_flag=0;
mubiao_speed=dawan_speed;
}
else
  if(zhidao_flag>30&&!podao_flag)//��ֱ��
  { //led(LED0, LED_OFF);//��ʾ�ر�
    mubiao_speed=longs;       
//led(LED0, LED_ON);//��ʾ
  }
else
 if(zhidao_flag>15&&zhidao_flag<30&&!podao_flag)//��ֱ��
 {
   //led(LED1, LED_OFF);//��ʾ�ر�
    mubiao_speed=dawan_speed+5;
   // led(LED0, LED_ON);
 }
else
 if(qvlv_quanju>10&&qvlv_quanju<=25&&abs(my_piancha)<=18)//С��
{
mubiao_speed=dawan_speed;
dawan_flag=0;
}
else
 if(qvlv_quanju>25||abs(my_piancha)>18||qulv_point>4)//����
{
  zhidao_ruwan_flag=0;
  dawan_flag=1;
mubiao_speed=dawan_speed;
//led(LED1, LED_OFF);//��ʾ�ر�
}else
 {
//led(LED0, LED_OFF);//��ʾ�ر�
   dawan_flag=0;
mubiao_speed=50;
}
 if(maichongshu>dawan_speed+4&&qulv_yuandaun>8&&qulv_jinduan<5)//����ֱ�������
 { 
   dawan_flag=0;
   zhidao_ruwan_flag=1;
   mubiao_speed=dawan_speed-3;
 }
}




/***************************************�ٶȿ���**************************************************/
void speedcontrol(int speed_now)//�ٶȿ���                                                  
{
 int menkan,dut,piancha_speed;
 if(star_lineflag&&maichongshu>3)//������
 {
   dut=4000;
   menkan=0;
kp=8;
 }else
  if(zhidao_ruwan_flag)//ֱ������
     {
      dut=3200;
      menkan=0;
     }
 else
   if(podao_flag)//�µ�
   {
     dut=2000;
   menkan=3;
   }
 else
 if(dawan_flag)//����
   {
     //led(LED1, LED_OFF);//��ʾ
     dut=1000;
   menkan=10;
   }
   else
   {
   menkan=6;
   dut=1000;
   }
  //lastspeed_wucha=speed_wucha;
 piancha_speed=maichongshu-speed_now;
  speed_wucha=speed_now-maichongshu;// bu
  dwucha=speed_wucha-lastspeed_wucha2;
  zhankongbi=last_zhankongbi+speed_kp*speed_wucha-speed_ki*lastspeed_wucha+speed_kd*dwucha;//pid����
    if(zhankongbi>7000)
     zhankongbi=7000;
    if(zhankongbi<0)
     zhankongbi=0;
if(piancha_speed>menkan)
{
fanzhuan(dut);
}
else
zhengzhuan(zhankongbi);
  last_zhankongbi=zhankongbi;
  lastspeed_wucha3=lastspeed_wucha2;
  lastspeed_wucha2=lastspeed_wucha1;
  lastspeed_wucha1=lastspeed_wucha;
  lastspeed_wucha=speed_wucha;
}