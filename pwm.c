#include "stc8.h"
#include "pwm.h"

void PWM_Init()
{
	PWMCFG &= ~0xc0;

   	P_SW2 |= 0x80;

	PWMCH = 0x5d;
	PWMCL = 0xc0;

	PWMCKS &= ~0x1f;//计数为系统时钟 

	PWM3CR |= 0x80;//GPIO使能为PWM输出
	PWM3CR |= 0x40;//初始高电平输出

	PWM3CR &= ~0x18;//P1.3
	PWM3CR |= 0x08;

	PWM3CR &= ~0x07;//关闭通道中断、第1个和第2个翻转点中断

	P_SW2 &= ~0x80;
}

void PWM_Ctrl(double per)
{
	double temp;
	unsigned int itemp;

	temp = 240.0*per;
	itemp = temp;

	PWMCR &= ~0x80;

	P_SW2 |= 0x80; 
	
	PWM3T1H = (itemp >> 8);
	PWM3T1L = itemp;

	PWM3T2H	= 0x5d;
	PWM3T2L = 0xc0;

	P_SW2 &= ~0x80;

	PWMCR |= 0x80;
}