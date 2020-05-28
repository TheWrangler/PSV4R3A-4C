#include "stc8.h"
#include "pwm.h"

void PWM_Init()
{
	PWMCFG &= ~0xc0;

   	P_SW2 |= 0x80;

	PWMCH = 0x5d;
	PWMCL = 0xc0;

	PWMCKS &= ~0x1f;//����Ϊϵͳʱ�� 

	PWM3CR |= 0x80;//GPIOʹ��ΪPWM���
	PWM3CR |= 0x40;//��ʼ�ߵ�ƽ���

	PWM3CR &= ~0x18;//P1.3
	PWM3CR |= 0x08;

	PWM3CR &= ~0x07;//�ر�ͨ���жϡ���1���͵�2����ת���ж�

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