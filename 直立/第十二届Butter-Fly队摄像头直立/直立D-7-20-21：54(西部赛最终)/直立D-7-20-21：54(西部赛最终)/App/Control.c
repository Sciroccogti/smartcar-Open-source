#include "Control.h"

uchar Run_Flag = 0;
uchar Stop_Flag = 0;
uchar Run_Stop = 1;
char Left_Crazy = 0;	// 电机疯转
char Right_Crazy = 0;	// 电机疯转
int32 RC_Get = 1500;	// 遥控器的通道值
int32 MOTOR_Duty_Left  = 0;
int32 MOTOR_Duty_Right = 0;
int32 MOTOR_Speed_Left = 0;
int32 MOTOR_Speed_Right = 0; 
int32 MOTOR_Speed_Left_Last = 0;
int32 MOTOR_Speed_Right_Last = 0;
int32 MOTOR_Left_Acc = 0;
int32 MOTOR_Right_Acc = 0;

uint32 Annulus_Error = 0;	// 环路误判控制


/******* 电机速度测量 ********/
void Speed_Measure(void)
{
	int32 Pulses;
	static int32 Speed_Last = 0;
	static int32 Crazy_Count = 0;
	
	
	/******* 右电机速度相关控制 ********/
	Pulses = ftm_quad_get(FTM1);	// 获取FTM 正交解码 的脉冲数(负数表示反方向)
	ftm_quad_clean(FTM1);   		// 正交解码寄存器清零
	MOTOR_Speed_Right = -Pulses;	// 得到右轮转速

	MOTOR_Right_Acc = MOTOR_Speed_Right - MOTOR_Speed_Right_Last;	// 计算加速度
	if (MOTOR_Right_Acc > 100)
	{
		Right_Crazy = 1;	// 疯转
	}
	if (MOTOR_Speed_Right > Speed_Set + 200)
	{
		Right_Crazy = 2;	// 疯转
	}
	if (MOTOR_Speed_Right < -350)
	{
		Right_Crazy = -1;	// 倒转
	}
	
	if (Right_Crazy)
	{
		if (MOTOR_Right_Acc <= 100)
		{
			if (MOTOR_Speed_Right < Speed_Set + 200 && MOTOR_Speed_Right > 0)
			{
				Right_Crazy = 0;
			}
		}
	}
	
	if (!Right_Crazy)
	{
		MOTOR_Speed_Right = MOTOR_Speed_Right*0.9 + MOTOR_Speed_Right_Last*0.1;
		MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// 更新右轮速度
	}
	else
	{
		MOTOR_Speed_Right = MOTOR_Speed_Right*0.5 + MOTOR_Speed_Right_Last*0.5;
		MOTOR_Speed_Right_Last = MOTOR_Speed_Right;	// 更新右轮速度
	}
	/******* 右电机速度相关控制结束 ********/
	
	/******* 左电机速度相关控制 ********/
	Pulses = ftm_quad_get(FTM2);	// 获取FTM 正交解码 的脉冲数(负数表示反方向)
	ftm_quad_clean(FTM2);			// 正交解码寄存器清零
	MOTOR_Speed_Left = Pulses;		// 得到左轮转速
	
	MOTOR_Left_Acc = MOTOR_Speed_Left - MOTOR_Speed_Left_Last;	// 计算加速度
	if (MOTOR_Left_Acc > 100)
	{
		Left_Crazy = 1;
	}
	if (MOTOR_Speed_Left > Speed_Set + 200)
	{
		Left_Crazy = 2;
	}
	if (MOTOR_Speed_Left < -350)
	{
		Left_Crazy = -1;
	}
	
	if (Left_Crazy)
	{
		if (MOTOR_Left_Acc <= 100)
		{
			if (MOTOR_Speed_Left < Speed_Set + 200 && MOTOR_Speed_Left > 0)
			{
				Left_Crazy = 0;
			}
		}
	}
	
	if (!Left_Crazy)
	{
		MOTOR_Speed_Left = 0.9*MOTOR_Speed_Left + 0.1*MOTOR_Speed_Left_Last;	// 低通滤波
		MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// 更新左轮速度
	}
	else
	{
		MOTOR_Speed_Left = 0.5*MOTOR_Speed_Left + 0.5*MOTOR_Speed_Left_Last;	// 低通滤波
		MOTOR_Speed_Left_Last = MOTOR_Speed_Left;	// 更新左轮速度
	}

	
	
	/******* 左电机速度相关控制结束 ********/
	
	
	if ((Left_Crazy && Right_Crazy) || (Left_Crazy && MOTOR_Speed_Right < 20) || (Right_Crazy && MOTOR_Speed_Left < 20))
	{
		Crazy_Count++;
		if (Crazy_Count >= 40)
		{
			Crazy_Count = 0;
			Run_Flag = 0;
		}
	}
	else
	{
		Right_Crazy = 0;
	}
	
	/******* 电机疯转特殊处理 ********/
	if ((Left_Crazy > 0) && (Right_Crazy > 0))
	{
		Speed_Now = Speed_Set;			// 两边都疯转，使用上次速度作为当前实际速度
	}
	else if (Left_Crazy)
	{
		if (MOTOR_Speed_Right > Speed_Set)
		{
			Speed_Now = Speed_Set;
		}
		else
		{
			Speed_Now = MOTOR_Speed_Right;	// 左电机疯转，使用上次速度作为当前实际速度
		}
	}
	else if (Right_Crazy)
	{
		if (MOTOR_Speed_Left > Speed_Set)
		{
			Speed_Now = Speed_Set;
		}
		else
		{
			Speed_Now = MOTOR_Speed_Left;	// 右电机疯转，使用上次速度作为当前实际速度
		}
	}
	else
	{
		Speed_Now = (MOTOR_Speed_Left + MOTOR_Speed_Right) / 2;	// 左右取平均计算车子实际速度
	}
	
	Speed_Now = Speed_Now *0.9 + Speed_Last * 0.1;
	Speed_Last = Speed_Now;
	
/*	if (Annulus_Mode == 1 || Annulus_Mode == 2)
	{
		Annulus_Error += Speed_Now;
		if (Annulus_Error >= 10000)
		{
			Annulus_Error = 0;
			
			led(LED_MAX,LED_OFF);// 关闭LED指示灯
			Annulus_Mode = 0;
			Annulus_Flag = 0;
			if (Annulus_Times)
			{
				Annulus_Times--;
			}
			else
			{
				Annulus_Times = 9;
			}
		}
	}
	else
	{
		Annulus_Error = 0;
	}*/
}

void Speed_Control(void)
{
		switch(Mode_Set)
		{
		case 0:		Speed_Set = 120;
				 	break;
		case 1:		Speed_Set = 130;
				 	break;
		case 2:		Speed_Set = 140;	// 默认初始速度
				 	break;
		case 3:		Speed_Set = 150;
				 	break;
		case 4:		Speed_Set = 155;
				 	break;
		case 5:		Speed_Set = 160;
				 	break;
		case 6:		Speed_Set = 165;
					break;
		default:	Speed_Set = 0;
					break;
		}
}

void Speed_Set_6(void)
{
	static uchar Speed_Count = 0;
	
	if (Left_Add_Start || Right_Add_Start)
	{
		Speed_Set = 150;
		Speed_Count = 0;
	}
	else
	{
		if (Speed_Set == 150)
		{
			Speed_Count++;
			if (Speed_Count > 20)
			{
				Speed_Set = 160;
			}
		}
	}
}

void Speed_Set_7(void)
{
	if (Foresight >= 18)
	{
		Speed_Set = 160;
	}
	else if (Foresight >= 16)
	{
		Speed_Set = 170;
	}
	else if (Foresight >= 14)
	{
		Speed_Set = 180;
	}
	else if (Foresight >= 12)
	{
		Speed_Set = 190;
	}
	else if (Foresight >= 10)
	{
		Speed_Set = 200;
	}
	else if (Foresight >= 8)
	{
		Speed_Set = 210;
	}
	else if (Foresight >= 7)
	{
		Speed_Set = 220;
	}
	else if (Foresight >= 6)
	{
		Speed_Set = 230;
	}
	else if (Foresight >= 5)
	{
		Speed_Set = 240;
	}
	else
	{
		Speed_Set = 260;
	}
}

void MOTOR_Control(int32 LDuty, int32 RDuty)
{
	if (LDuty >= 0)
	{
		LDuty = range_protect(LDuty, 0, MOTOR_MAX);	// 限幅保护
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,0);	  	// 占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,LDuty);	// 占空比最大990！！！
	}
	else
	{
		LDuty = range_protect(-LDuty, 0, MOTOR_MAX);// 限幅保护
		ftm_pwm_duty(MOTOR_FTM, MOTOR1_PWM,0);		// 占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,LDuty);	// 占空比最大990！！！
	}
	
	if (RDuty >= 0)
	{
		RDuty = range_protect(RDuty, 0, MOTOR_MAX);	// 限幅保护
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,0);	  	// 占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,RDuty);	// 占空比最大990！！！	
	}
	else
	{
		RDuty = range_protect(-RDuty, 0, MOTOR_MAX);// 限幅保护
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,0);		// 占空比最大990！！！
		ftm_pwm_duty(MOTOR_FTM, MOTOR4_PWM,RDuty);	// 占空比最大990！！！
	}
}

void Start_Control(void)
{
	static uchar Start_OK = 0;
	static uchar Ready_Stop = 0;
	static uint16 No_Start_Line_Count = 0;
	
	if (Run_Flag)
	{									/** 起跑阶段检测 **/
		if (!Starting_Line_Flag && !Start_OK)	//没有检测到起跑线且未成功出发
		{
			No_Start_Line_Count++;			//相当于延时约5s
			if (No_Start_Line_Count >= 300)	//连续200次没有检测到起跑线，避免起跑时的光线异常干扰
			{
				No_Start_Line_Count = 0;
				Start_OK = 1;	//出发成功
			}
		}
										/** 结束阶段检测 **/
		if (Start_OK && Starting_Line_Flag)	//成功起跑后检测到终点线
		{
			Ready_Stop = 1;	//准备停车
		}
		
		if (Ready_Stop)
		{
			if (!Starting_Line_Flag)	/** 检测到停车线后延时一段时间停车 **/
			{
				No_Start_Line_Count++;
				if (No_Start_Line_Count >= 15)	//连续30次没有检测到起跑线，约延时1s
				{
					No_Start_Line_Count = 0;
					if (Run_Stop)	// 由屏幕设置是否停车，默认停车
					{
						Stop_Flag = 1;
						Run_Flag = 0;			//停车
					}
					Start_OK = 0;			//清除出发成功标志位
					Ready_Stop = 0;			//清除准备停车标志位
					led(LED_MAX,LED_ON);	//关闭LED指示灯
				}
			}
		}
	}
}

/******** 限幅保护 *********/
int32 range_protect(int32 duty, int32 min, int32 max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}
