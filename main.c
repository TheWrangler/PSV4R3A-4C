#include "STC8.H"
#include "pll.h"
#include "hw.h"
#include "adc.h"
#include "rtx.h"
#include "cmd.h"
#include "utily.h"

//WARNNING:STC MCU int protype is 16 bit width,not 32 bit width！！！！！！

unsigned char msg[25];

unsigned char _sprintf_pllwait(unsigned char* buf)
{
	buf[0] = 'w';
	buf[1] = 'a';
	buf[2] = 'i';
	buf[3] = 't';
	buf[4] = ' ';
	buf[5] = 'f';
	buf[6] = 'o';
	buf[7] = 'r';
	buf[8] = ' ';
	buf[9] = 'F';
	buf[10] = 'S';
	buf[11] = ' ';
	buf[12] = 'l';
	buf[13] = 'o';
	buf[14] = 'c';
	buf[15] = 'k';
	buf[16] = 'e';
	buf[17] = 'd';
	buf[18] = '.';
	buf[19] = '.';
	buf[20] = '.';
	buf[21] = '\n';

	return 22;
}

unsigned char _sprintf_char(unsigned char* buf,unsigned char var)
{
	unsigned char temp = var / 100;
	buf[0] = temp + 0x30;

	var -= temp * 100;

	temp = var / 10;
	buf[1] = temp + 0x30;

	var -= temp * 10;

	buf[2] = var + 0x30;

	buf[3] = '\n';

	return 4;
}

unsigned char _sprintf_word(unsigned char* buf,unsigned int var)
{
	unsigned char temp = (var & 0xff00) >> 8;
	unsigned char len = _sprintf_char(buf,temp);
	len -= 1;

	temp = (var & 0x00ff);
	len += _sprintf_char(buf+len,temp);

	return len;
}

int main()
{
	unsigned char cmd,len;
	unsigned int var;

	//config sysclk
	HW_SysclkInit();

	//config pins
 	HW_IOInit();

	//config perhaps
	HW_T2Init();
	HW_IntcInit();
	RTX_Init();
	ADC_Init();

	delay_ms(1000);

	len = _sprintf_pllwait(msg);

	//config dds
	PLL_Reset();
	PLL_Config();
	RTX_SendBytes(msg,len);
	delay_ms(500);

	while(PLL_IsLocked() != 1)
	{
		PLL_Reset();
		PLL_Config();
		RTX_SendBytes(msg,len);
		delay_ms(500);
	}
	
	while(1)
	{
		if(Cmd_IsNew())
		{
			Cmd_get(&cmd);
			switch(cmd)
			{
				case 0x00://发射功率：0-正功率值，1-负功率值
					var = ADC_Get(ADC_CH_P1_0);
					len = _sprintf_word(msg,var);
					RTX_SendBytes(msg,len);
					break;
				case 0x01://本振锁定指示
					break;
				case 0x02://以上所有参数
					break;
				case 0x03:
					break;
				default:
					break;
			}
		}
	}
}





