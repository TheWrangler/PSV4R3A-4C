#include "rtx.h"

unsigned char cmd;
signed char bValid = 0;
unsigned char rply[10];
unsigned char rply_len = 0;

void Cmd_set(unsigned char content)
{
	cmd = content;
	bValid = 1;
}

signed char Cmd_IsNew()
{
	return bValid;
}

void Cmd_get(unsigned char* pCmd)
{
	if(!bValid)
		return;
	
	*pCmd = cmd;
	bValid = 0;
	return;
}

void Cmd_clear()
{
	bValid = 0;
}

void Cmd_reply()
{
	unsigned int i;
	for(i=0;i<rply_len;i++)
		RTX_Send(rply[i]);
}