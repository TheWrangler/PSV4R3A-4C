#include "rtx.h"
#include "stc8.h"
#include "adc.h"
#include "cmd.h"
#include "pll.h"
#include "utily.h"
#include "pwm.h"

unsigned char tx_pwr = 0;
unsigned char tx_enable = 0;

sbit tx_en = P1^0;

unsigned char rply[9];
unsigned char rply_len = 0;
unsigned char cmd[5];
unsigned char cmd_backup[5];
unsigned char cmd_len = 0;

idata unsigned int table[64];

void Cmd_InitTable()
{
	table[0] = 795;



	table[63] = 490;
}

void Cmd_set(unsigned char content)
{
	if(cmd_len == 5)
		cmd_len = 0;

	cmd[cmd_len] = content;
	cmd_len++;
}

void Cmd_Del(unsigned char size)
{
	unsigned char remain,i;
	if(cmd_len < size)
		size = cmd_len;

	remain = cmd_len - size;
	for(i=0;i<remain;i++)
		cmd_backup[i] = cmd[size+i];
	for(i=0;i<remain;i++)
		cmd[i] = cmd_backup[i];
	cmd_len =  remain;
}

unsigned char Cmd_IsNew()
{
	if(cmd_len < 3)
		return 0;

	if(cmd[0] == 0xa1)
	{
		if((cmd[2] != 0x00) && (cmd[2] != 0x0a))
		{
			Cmd_Del(3);
			return 0;
		}

		if(cmd_len < 4)
			return 0;

		if(cmd[3] != CRC(cmd,3))
		{
			Cmd_Del(3);
			return 0;
		}
		else return 1;
	}
	else if(cmd[0] == 0xa2)
	{
		if((cmd[2] != 0x01) && (cmd[2] != 0x02))
		{
			Cmd_Del(3);
			return 0;
		}

		if(cmd_len < 5)
			return 0;

		if(cmd[4] != CRC(cmd,4))
		{
			Cmd_Del(3);
			return 0;
		}
		else return 1; 
	}
	else if(cmd[0] == 0xa3)
	{
		if((cmd[2] != 0x01) && (cmd[2] != 0x81))
		{
			Cmd_Del(3);
			return 0;
		}

		if(cmd_len < 5)
			return 0;

		if(cmd[4] != CRC(cmd,4))
		{
			Cmd_Del(3);
			return 0;
		}
		else return 1; 
	}
	else
	{
		Cmd_Del(3);
		return 0;
	}
}


void Cmd_reply()
{
	unsigned int i;
	for(i=0;i<rply_len;i++)
		RTX_Send(rply[i]);

	rply_len = 0;
}

void Cmd_ack(unsigned char size)
{
   	unsigned int i;
	for(i=0;i<size;i++)
		RTX_Send(cmd[i]);
}

void BuildPwrRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x05;
	rply[2]	= 0x00;
	rply[3]	= 0x00;
	rply[4]	= GetLO1Voltage();
	rply[5]	= GetLO1Voltage();
	rply[6]	= GetLO3Voltage();
	rply[7]	= CRC(rply,7);
	rply_len = 8;
}

void BuildLOLockRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x01;

	rply[3]	= PLL_IsRxLocked();
	rply[3] = (rply[2] << 1);

	rply[3]	|= PLL_IsTxLocked();
	rply[3] = (rply[2] << 1);
	rply[3]	|= PLL_IsTxLocked();

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildTxPwrRply()
{
   	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x02;

	rply[3]	= tx_pwr/2;

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildTxLO1PwrRply()
{
   	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x03;

	rply[3]	= GetLO1Voltage();

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildTxLO2PwrRply()
{
   	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x04;

	rply[3]	= GetLO1Voltage();

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildRxLO3Rply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x05;

	rply[3]	= GetLO3Voltage();

	rply[4]	= CRC(rply,4);
	rply_len = 5;	
}

void BuildAllStateRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x06;
	rply[2]	= 0x06;

	rply[3]	= PLL_IsRxLocked();
	rply[3] = (rply[2] << 1);

	rply[3]	|= PLL_IsTxLocked();
	rply[3] = (rply[2] << 1);
	rply[3]	|= PLL_IsTxLocked();

	rply[4]	= tx_pwr/2;

	rply[5]	= GetLO1Voltage();
	rply[6]	= GetLO1Voltage();
	rply[7]	= GetLO3Voltage();

	rply[8]	= CRC(rply,8);
	rply_len = 9;	
}

void BuildTxEnableRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x07;

	rply[3]	= tx_enable;

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildTxPowerATTRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x08;

	rply[3]	= tx_pwr;

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildHWVerisonRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x09;

	rply[3]	= 1;

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void BuildSWVersionRply()
{
	rply[0]	= 0xa1;
	rply[1]	= 0x02;
	rply[2]	= 0x0a;

	rply[3]	= 1;

	rply[4]	= CRC(rply,4);
	rply_len = 5;
}

void process_a1(unsigned char cmd_wd)
{
	switch(cmd_wd)
	{
	   	case 0x00:
	   	  	BuildPwrRply();
		  	break;
		case 0x01:
		  	BuildLOLockRply();
		  	break;
		case 0x02:
			BuildTxPwrRply();	
			break;
		case 0x03:
			BuildTxLO1PwrRply();
			break;
		case 0x04:
			BuildTxLO2PwrRply();
			break;
		case 0x05:
			BuildRxLO3Rply();
			break;
		case 0x06:
			BuildAllStateRply();
			break;
		case 0x07:
			BuildTxEnableRply();
			break;
		case 0x08:
			BuildTxPowerATTRply();
			break;
		case 0x09:
			BuildHWVerisonRply();
			break;
		case 0x0a:
			BuildSWVersionRply();
			break;
		default:
			break;
	}

   	Cmd_Del(4);
	if(rply_len != 0)
		Cmd_reply();
}

void process_a2(unsigned char cmd_wd)
{
	double per;
	if(cmd_wd == 0x01)
	{
		 tx_enable = cmd[3];
		 tx_en = tx_enable;
		 Cmd_ack(5);
	}
	else if(cmd_wd == 0x02)
	{
		tx_pwr = cmd[3];
		per = table[cmd[3]];
		per = per / 10.0;
		PWM_Ctrl(per);	
		Cmd_ack(5);
	}
	Cmd_Del(5);
}

void process_a3(unsigned char cmd_wd)
{
	char var;

	if(cmd_wd == 0x01)
	{
		var = cmd[3] & 0x7f;
		PLL_Adjust(var);
		Cmd_ack(5);
	}
	else if(cmd_wd == 0x81)
	{
		var = cmd[3] & 0x7f;
		var = -var;
		PLL_Adjust(var);
		Cmd_ack(5);
	}

	Cmd_Del(5);
}