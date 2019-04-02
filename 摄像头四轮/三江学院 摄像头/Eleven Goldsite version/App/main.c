#include "common.h"
#include "include.h"

#define BIN_MAX 0x80   //���Ƶ����ֵ

Size_t imgsize = {TSL1401_SIZE, 1}; //ͼ���С
Size_t size    = {TSL1401_SIZE,30}; //��ʾ�����С

Site_t site1   = { 0, 0};           //��ʾͼ�����Ͻ�λ��
Site_t sitemax = {30,45};           //��ʾmaxֵ
Site_t sitemin = {60,45};           //��ʾminֵ
Site_t siteaver= {90,45};           //��ʾƽ��ֵ
Site_t sitecnt = {90,30};           //��ʾ�ڵ����
Site_t sitel   = { 0,30};           //�����λ��
Site_t sitem   = {30,30};           //����λ��
Site_t siter   = {60,30};           //�ұ���λ��
Site_t siteW   = { 0,45};           //�������
Site_t dis   = { 0,90};             //�������
      
void PIT0_IRQHandler(void);        //�жϺ���
void System_init(void);            //ϵͳ��ʼ��
void changliang_init(void);        //������ʼ��
void Rampway(void);                //�µ����
void Stop(void);                   //ͣ�����

extern struct TSL1401 ccd1;        //ccd
extern struct motor pidl;          //���pid
extern struct motor pidr;
uint32 dial_ccd,ramp_straight,speed_dial;//��¼���뿪����ֵ
uint16 distance[3];                  //���������ADCת�����

extern uint8 CCD_BUFF[TSL1401_MAX*3][TSL1401_SIZE]; //����洢����CCDͼ�������  
extern int16 CCD_Value[TSL1401_MAX][TSL1401_SIZE];  //�洢���ֵ������Ѳ��

uint16 star,STAR_NUMBER;             //��ʼ����������߱��
uint8 stop_flag;                     //ͣ��־
int16 a,b,c,d,g;        //�ٶ��趨������ɲ��ϵ����0�����߸���

/***************************************
  �޸� PIT0 �Ķ�ʱʱ�伴���޸��ع�ʱ��
****************************************/
void  main(void)
{
    System_init();        //ϵͳ��ʼ��
    changliang_init();    //������ʼ��
    
    led(LED_MAX,LED_ON);
    //����Ҫ�޸Ĳ���ʱ��ѡ�񰴼���������
    //while(key_check(KEY_A) == 1);//������������
    //��Ҫ�޸Ĳ���ʱ��ѡ������޸Ĳ����˵����޸ĺ�Ĳ���ֻ�ڱ�����������Ч�������ϵ����Ч
    KEY_inter();                   //�����޸Ĳ˵����޸Ĳ���������
    led(LED_MAX,LED_OFF);
    
    uint8 time = 10;      //�ع�ʱ��
    tsl1401_set_addrs(TSL1401_MAX,(uint8 *)&CCD_BUFF[0]);//��ʼ�� ����CCD
    tsl1401_init(time);                                  //���� �ж�ʱ��Ϊ time
    set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);    //���� �жϺ��� �� ʹ���ж�
    
    //��ȡ��������ֵ
    ramp_straight  = gpio_get(PTB0);//�µ��Ƿ��ֱ
    dial_ccd       = gpio_get(PTB1);//СҺ���۲�CCD
    speed_dial     = gpio_get(PTB2) * 10 + gpio_get(PTB3);//�ٶȿ���
    
    if(speed_dial == 1)        //���ٴ�ɲ��
    { 
      a=85;b=90;c=95;d=115; g = 20;
    }
    else if(speed_dial == 10) //���ٲ���ɲ��
    {
      a=85;b=90;c=95;d=110; g = 30;
    }
    else if(speed_dial == 11) //����
    {
      a=85;b=90;c=95;d=100; g = 20;
    }
    else if(speed_dial == 0)  //����
    {
      a=85;b=90;c=90;d=90;  g = 20;
    }
    
    uart_init (UART4, 115200); //���ڳ�ʼ��
    
    DELAY_MS(2000);            //��ʱ����
    enable_irq(PIT0_IRQn); 
    
    while(1)
    {
      tsl1401_get_img();       //�ɼ� ����CCD ͼ�� �������ڲ������������
      
      if(star < (STAR_NUMBER+50) )
      {
        star++; 
      }
      else 
      {
        star = STAR_NUMBER + 50;//С�ڵ���ʱ+50 �����+1
      }
      
      if(star < STAR_NUMBER )
      {
      }
      else 
      { 
        Stop();                 //С�ڵ���ʱ�����������
      }
    }
}
/*********************************
        PIT0�жϷ�����
 ********************************/  
extern uint8 ramp;
//uint8 ccnt;
void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);  
  
  /*ccnt++;
  if(ccnt == 100)
  {
    //FENG = ~FENG;
    ccnt = 0;
  }*/
  
  tsl1401_time_isr();

  maxvar_a((uint8 *)&CCD_BUFF[0],TSL1401_SIZE,BIN_MAX,&ccd1);                         //�������ֵ
  Filter((uint8 *)&CCD_BUFF[0],(uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],TSL1401_SIZE);     //�����˲�
  findMax((uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],&ccd1);                                 //�������Сֵ��ƽ��ֵ  
  abs_diff((int16 *)&CCD_Value[0],(uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],TSL1401_SIZE);  //���β��  
  
  searchEdge((int16 *)&CCD_Value[0],&ccd1);                                           //Ѱ�ұ���
  CaculateMid_Line(&ccd1);                                                            //��������
  
  Rampway();                                                                          //�µ���⣬����ʹ�ã����������µ�
  //****************   �����ϰ��ļ��   *************************//
  if(ramp <= 50) //�������ƽ·ʱ��������ϰ��������µ�ʱ���������ϰ������Բ��ü���ϰ�
  {
    Obstacle_search((int16 *)&CCD_Value[0],&ccd1); //�ϰ����
  }
  else
  {
    ccd1.obstacle = 0;    //�ϰ���־����
  }
  //************  ������ʾ��״̬   **************//
  if(ccd1.obstacle != 0 || ccd1.cross[0] == 1 || ramp > 50)
  {
    FENG = 0;
  }
  else
  {
    FENG = 1;
  }
  //***************   ���������ƻ���   *****************//
  if(ccd1.obstacle == 0 || ramp > 50) //������ϰ������ߴ����µ��У������������
  {
    direction_Control();
  }
  else if(ccd1.obstacle == 01) //����ϰ����ұߣ��Ӵ�ƫ�ƫ��ɵ���
  {
      S3010AngleOB(&ccd1,-12);
  }
  else if(ccd1.obstacle == 10) //����ϰ�����ߣ��Ӵ�ƫ�ƫ��ɵ���
  {
      S3010AngleOB(&ccd1,12);
  }
  //*****************  ���������ƻ���   *****************//
  //���ͣ����־λΪ0 ����δ���յ㣬�����ٶȿ���
  if(stop_flag == 0) 
  {
    if(dial_ccd == 0)
    {    
      speed_Contorl();
    }
    else if(dial_ccd == 1)
    {
      LCD_Img_gray_Z(site1,size,(uint8 *)&CCD_BUFF[0],imgsize);         //��ʾͼ��
      LCD_wave_display(site1,size,(uint8 *)&CCD_BUFF[0],BIN_MAX,BLUE);  //��ʾ�˲���Ĳ���
      //LCD_num_BC(sitemax,ccd1.max,3,RED,BLACK);                       //���ֵ
      //LCD_num_BC(sitemin,ccd1.min,3,RED,BLACK);                       //��Сֵ
      LCD_num_BC(siteaver,ccd1.aver,3,RED,BLACK);                       //ƽ��ֵ
      //LCD_num_BC(sitecnt,ccd1.obstacle,2,RED,BLACK);                  //�ϰ�
      LCD_num_BC(sitel,ccd1.left_Edge[0],3,RED,BLACK);                  //�����
      LCD_num_BC(sitem,ccd1.mid_Line[0],3,RED,BLACK);                   //����
      LCD_num_BC(siter,ccd1.right_Edge[0],3,RED,BLACK);                 //�ұ���
      LCD_num_BC(siteW,ccd1.width,2,RED,BLACK);                         //�������
      LCD_num_BC(dis,distance[0],5,RED,BLACK);                          //����������
      
      M1(0);
      M2(0);
      M3(0);
      M4(0);
    }
    else
    { 
      speed_Contorl();
    }
  }
  //���ͣ����־λ��λ���������յ㣬��ȡͣ�����ԣ�
  //������PWM���Ϊ0����СΪ-9999�����˴�ʵ�ʴ���ӦΪ��PWMͨ����������ȡ����ֵ  
  else  
  {
    //FENG = 0;
    pidl.setPoint = 0; //�ٶ�Ԥ����Ϊ0����ֹͣ
    pidr.setPoint = 0;
    Getpluse();        //��ȡ������ֵ
    motorPID111();     //���PID����,����ת
    
    if(pidl.PWM >= 0)
    {
      M1(0);
      M2(0);
    }
    else
    {
      M2( ABS(pidl.PWM) );
      M1(0);
    }
    
    if(pidr.PWM >=0 )
    {
      M3(0);
      M4(0);
    }
    else
    {
      M4( ABS(pidr.PWM) );
      M3(0);
    }
  }
}
/******************************
    ��ʼ��ϵͳ
******************************/
void System_init(void)
{  
  gpio_init(PTA26,GPO,1);     //�������˿�
  //��������߶˿ڳ�ʼ��
  gpio_init(PTA28,GPI,0);
  gpio_init(PTA29,GPI,0);   
  port_init_NoALT(PTA28,PULLUP);
  port_init_NoALT(PTA29,PULLUP);
  //����Լ������ʼ��
  ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
  
  ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,725);      //��ʼ�� ��� PWM
  //�������룬�Ѿ��ⲿ����
  ftm_quad_init(FTM1);                                 //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
  ftm_quad_init(FTM2);                                 //FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
  
  adc_init(infrared); //���������ų�ʼ��
  
  LCD_init();  
  key_init(KEY_MAX);
  led_init(LED_MAX);
  
  //���뿪�س�ʼ��
  gpio_init(PTB0,GPI,0);
  gpio_init(PTB1,GPI,0);
  gpio_init(PTB2,GPI,0);
  gpio_init(PTB3,GPI,0);   
  port_init_NoALT(PTB0,PULLUP);
  port_init_NoALT(PTB1,PULLUP);
  port_init_NoALT(PTB2,PULLUP);
  port_init_NoALT(PTB3,PULLUP);
  
  gpio_init(PTE8,GPI,0);
  gpio_init(PTE9,GPI,0);
  gpio_init(PTE10,GPI,0);
  gpio_init(PTE12,GPI,0);   
  port_init_NoALT(PTE8,PULLUP);
  port_init_NoALT(PTE9,PULLUP);
  port_init_NoALT(PTE10,PULLUP);
  port_init_NoALT(PTE12,PULLUP);
}
/***************************************
  
***************************************/
void changliang_init(void)
{
  STAR_NUMBER = 1600;  //�����߼�⵹��ʱ
  CCD_init();
  PID_init();
}
/**************************************
        �� �� �� ��
***************************************/
uint8 ramp;
void Rampway(void)
{
  distance[0] = adc_once(infrared,ADC_10bit);             //��ȡ������
  
  for(uint8 i=2;i>0;i--)   distance[i] = distance[i-1];  //����3�κ���������
  
  if(ramp == 0)
  {
    if(distance[0]>540 && distance[1]>540 && distance[2]>540 && ccd1.width>50) //��������С����ֵ
    {
      ramp = 100;    //�ٶȴ�����ʱ�����ֵ�����ʵ�����
      
      if(star> (STAR_NUMBER-100) ) star = STAR_NUMBER-100;//�µ�ʱ����������ߣ���ֹ��⣬���ֵҪ����������ֵ���
    }
    else
    {
      ramp = 0;
    }
  }
  else
  {
    ramp--;
  }
}
/*******************************************
               ͣ ֹ �� �� ��
        �ж������ȽϿ��̣����ʵ�����
  ����ѡ���ֻ�����ͣ������ע�͵��Ľ�ע����
********************************************/
void Stop(void)
{
  //char str[100];       //���ڽ�������
  
  //��������߳��򣬼���������������ֹ����
  if( /*(gpio_get(PTA28)==1 && gpio_get(PTA29)==1)
       ||*/ (gpio_get(PTE8)==1 && gpio_get(PTE9)==0 && gpio_get(PTE10)==0 && gpio_get(PTE12)==1)
         
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==1 && gpio_get(PTE10)==0 && gpio_get(PTE12)==1)
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==1 && gpio_get(PTE10)==0 && gpio_get(PTE12)==0)
         
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==0 && gpio_get(PTE10)==1 && gpio_get(PTE12)==1)
       || (gpio_get(PTE8)==0 && gpio_get(PTE9)==0 && gpio_get(PTE10)==1 && gpio_get(PTE12)==1)
         
       || (gpio_get(PTE8)==0 && gpio_get(PTE9)==1 && gpio_get(PTE10)==1 && gpio_get(PTE12)==0)
     /*|| (uart_querystr (UART4,str,sizeof(str)-1) != 0)*/)
  {
    if( ccd1.flag_edge[1]==11 
        && (ccd1.width>40&&ccd1.width<60) 
        && (ccd1.mid_Line[0]>45&&ccd1.mid_Line[0]<81) 
        && (distance[0]<520)
       )
    {
      while(1)               //��⵽ͣ�ߣ�������ѭ������������ѭ��ת�Ƶ��˴�
      {
        stop_flag = 1;        //ͣ����־��λ
        tsl1401_get_img1();   //�ɼ� ����CCD ͼ�� �������ڲ�������������ߣ���ע��˺������������вɼ�������������
      }
    }
  }
}
