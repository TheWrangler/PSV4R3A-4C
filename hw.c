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
	//接收lock
	P2M0 &= ~0x40;
	P2M1 &= ~0x40;
	//DATA
	P2M0 |= 0x80;
	P2M1 &= ~0x80;
	//接收LE
	P0M0 |= 0x02;
	P0M1 &= ~0x02;
	//发射LE
	P0M0 |= 0x08;
	P0M1 &= ~0x08;
	//CLK
	P0M0 |= 0x20;
	P0M1 &= ~0x20;
	//发射LOCK
	P0M0 &= 0x40;
	P0M1 &= ~0x40;

	//发射检波
	P1M0 &= 0x02;
	P1M1 &= ~0x02;

	//发射供电控制
	P1M0 |= 0x01;
	P1M1 &= ~0x01;
	//发射功率控制电压
	P1M0 |= 0x08;
	P1M1 &= ~0x08;
	
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
	
	//enable P1.3 output for T2 overflow
	INTCLKO |= 0x04;
	
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