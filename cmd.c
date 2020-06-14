#include "rtx.h"

unsigned char cmd[2] = {0x00,0x00};
signed char bValid = 0;

void Cmd_set(unsigned char content)
{
	cmd[0] = cmd[1];
	cmd[1] = content;
	if(cmd[0] != 0xa1)
	{
		bValid = 0;
		return;
	}

	if(cmd[1] > 0x04)
	{
		bValid = 0;
		return;
	}
	
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
	
	*pCmd = cmd[1];
	bValid = 0;
	return;
}

void Cmd_clear()
{
	bValid = 0;
}