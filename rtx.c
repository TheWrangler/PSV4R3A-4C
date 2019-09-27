#include "stc8.h"
#include "rtx.h"

unsigned long rtx_baud = 9600;

void RTX_Init()
{
	//mode 0
	S4CON |= 0x80;
	
	//select T2 as baud gen.
	S4CON &= ~0x40;
	
	//enable rx
	S4CON |= 0x10;
}

void RTX_Recv(unsigned char *buf)
{
	*buf = S4BUF;
	S4CON &= ~0x01;
}

void RTX_Send(unsigned char buf)
{
	S4BUF = buf;
	while((S4CON & 0x02) != 0x02);
	S4CON &= ~0x02;
}