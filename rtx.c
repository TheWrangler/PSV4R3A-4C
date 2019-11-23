#include "stc8.h"
#include "rtx.h"

unsigned long rtx_baud = 9600;

void RTX_Init()
{
	//mode 0
	S4CON &= ~0x80;
	
	//select T2 as baud gen.
	S4CON &= ~0x40;
	
	//enable rx
	S4CON |= 0x10;
}

unsigned char RTX_IsRecvInt()
{
	if((S4CON & 0x01) == 0x01)
		return 1;
	else return 0;
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

void RTX_SendBytes(unsigned char* buf,unsigned char len)
{
	unsigned char idx;
	for(idx=0;idx<len;idx++);
		RTX_Send(buf[idx]);
}