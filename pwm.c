#include "stc8.h"
#include "pwm.h"

void PWM_Init()
{
	PWMCFG &= ~0x40;

   	P_SW2 |= 0x80;

	PWMCH = 0x5d;
	PWMCL = 0xc0;

	PWMCKS &= 0x1f; 

	PWM3CR |= 0x80;
	PWM3CR |= 0x40;

	PWM3CR &= ~0x18;//P1.3
	PWM3CR |= 0x08;

	PWM3CR &= ~0x07;

	P_SW2 &= ~0x80;
}

void PWM_Ctrl(unsigned char per)
{
	P_SW2 |= 0x80;

	PWM3T1H = 0x2e;
	PWM3T1L = 0xe0;

	PWM3T2H	= 0x5d;
	PWM3T2L = 0xc0;

	P_SW2 &= ~0x80;
}