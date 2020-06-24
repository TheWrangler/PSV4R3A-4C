#include "stc8.h"
#include "hw.h"
#include "rtx.h"
#include "cmd.h"
#include "utily.h"

unsigned long sysclk = 24000000; 
extern unsigned long rtx_baud;
unsigned char temp;

void HW_IOInit()
{
	//接收lock P2.6
	P2M0 &= ~0x40;
	P2M1 &= ~0x40;
	//接收LE P0.1
	P0M0 |= 0x02;
	P0M1 &= ~0x02;
	//接收CLK P0.0
	P0M0 |= 0x01;
	P0M1 &= ~0x01;	
	//接收DATA P2.7
	P2M0 |= 0x80;
	P2M1 &= ~0x80;

	//发射DATA P4.3
	P4M0 |= 0x08;
	P4M1 &= ~0x08;
	//发射LE P0.6
	P0M0 |= 0x40;
	P0M1 &= ~0x40;
	//发射CLK P0.5
	P0M0 |= 0x20;
	P0M1 &= ~0x20;
	//发射LOCK P0.2
	P0M0 &= ~0x04;
	P0M1 &= ~0x04;

	//发射检波P1.1,(2.7v)
	P1M0 &= ~0x02;
	P1M1 &= ~0x02;

	//发射开关控制P1.0
//	P1M0 |= 0x01;
//	P1M1 &= ~0x01;
	P0M0 |= 0x80;
	P0M1 &= ~0x80;

	//ds3502
	//SCL P3.5
	P3M0 |= 0x20;
	P3M1 &= ~0x20;
	//SDA  P3.6	,open-drain
	P3M0 |= 0x40;
	P3M1 |= 0x40;

	//A0 P5.0
	P5M0 |= 0x01;
	P5M1 &= ~0x01;
	//A1 P5.1
	P5M0 |= 0x02;
	P5M1 &= ~0x02;
	
	//set P5.2/P5.3 as uart,RX=P5.2,TX=P5.3
	P_SW2 |= 0x04;
	P5M0 &= ~0x04;//set P5.2 as inout port
	P5M1 &= ~0x04;
	P5M0 |= 0x08;//set P5.3 as output,TODO: if push up with 3K~5K resistor,comment this.
	P5M1 &= ~0x08;

	//set P5.4 as output for sysclk
	P5M0 |= 0x10;
	P5M1 &= ~0x10;
}

void HW_SysclkInit()
{
	//EAXFR must be set if access to XDATA Regs 
	P_SW2 |= 0x80;
	
	//divide factor 1.
	CLKDIV = 0x01;

	//check if internal OSC locked.
	while((IRC24MCR & 0x01) != 0x01)
	{
		;
	}

	P_SW2 &= ~0x80;
}

void HW_T2Init()
{
	unsigned int div = 65536 - sysclk / (4 * rtx_baud);
	
	//set counter
	T2L = div & 0xff;
	T2H = (div & 0xff00) >> 8;
	
	//comment this because P1.3 used for PWM Output
	//enable P1.3 output for T2 overflow
	//INTCLKO |= 0x04;
	
	//counter with sysclk,,
	AUXR |= 0x04;
	
	//work as timer
	AUXR &= ~0x08;
	
	//start timer
	AUXR |= 0x10;
}

void HW_IntcInit()
{
	//enable uart4 int.
	IE2 |= 0x10;

	//enable global int.
	IE |= 0x80;
}

void HW_Uart4ISR(void) interrupt 18
{
	if(RTX_IsRecvInt()==1)
	{
	   	RTX_Recv(&temp);
		Cmd_set(temp);
	}
}