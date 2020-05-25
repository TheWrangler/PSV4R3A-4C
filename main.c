#include "STC8.H"
#include "pll.h"
#include "hw.h"
#include "adc.h"
#include "rtx.h"
#include "cmd.h"
#include "utily.h"

//WARNNING:STC MCU int protype is 16 bit width,not 32 bit width！！！！！！

//unsigned char msg[25];
unsigned char len;

unsigned char _sprintf_splashscreen(unsigned char* buf)
{
	buf[0] = 'i';
	buf[1] = 'n';
	buf[2] = 'i';
	buf[3] = 't';
	buf[4] = ' ';
	buf[5] = 'p';
	buf[6] = 'r';
	buf[7] = 'o';
	buf[8] = 'c';
	buf[9] = 'e';
	buf[10] = 's';
	buf[11] = 's';
	buf[12] = '.';
	buf[13] = '.';
	buf[14] = '.';
	buf[15] = '\n';

	return 16;
}

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
	buf[16] = '.';
	buf[17] = '.';
	buf[18] = '.';
	buf[19] = '\n';

	return 20;
}

unsigned char _sprintf_plllocked(unsigned char* buf)
{
	buf[0] = 'F';
	buf[1] = 'S';
	buf[2] = ' ';
	buf[3] = 'l';
	buf[4] = 'o';
	buf[5] = 'c';
	buf[6] = 'k';
	buf[7] = 'e';
	buf[8] = 'd';
	buf[9] = '.';
	buf[10] = '.';
	buf[11] = '.';
	buf[12] = '\n';

	return 13;
}

unsigned char _sprintf_pllunlocked(unsigned char* buf)
{
	buf[0] = 'F';
	buf[1] = 'S';
	buf[2] = ' ';
	buf[3] = 'u';
	buf[4] = 'n';
	buf[5] = 'l';
	buf[6] = 'o';
	buf[7] = 'c';
	buf[8] = 'k';
	buf[9] = '.';
	buf[10] = '.';
	buf[11] = '.';
	buf[12] = '\n';

	return 13;
}


unsigned char _sprintf_float(unsigned char* buf,float var)
{
	unsigned char n1,n2;

	n1 = var; 
	buf[0] = n1 + 0x30;
	buf[1] = '.';

	n2 = var * 10 - n1 * 10;
	buf[2] = n2 + 0x30;

	n2 = var * 100 - n1 * 100 - n2 * 10;
   	buf[3] = n2 + 0x30;

	buf[4] = '\n';
	return 5;
}

void GetPowerVoltage()
{
	unsigned int var;
	float fv;
	ADC_Get(ADC_CH_P1_0);
	delay_ms(100);
	var = ADC_Get(ADC_CH_P1_0);
	fv = var;
	fv = fv / 4096.0;
	fv = fv * 3.0;
	//len = _sprintf_float(msg,fv);
	//RTX_SendBytes(msg,len);	
}

void GetPLLLockSta()
{
//	if(PLL_IsLocked() != 1)
//		len = _sprintf_pllunlocked(msg);
//	else len = _sprintf_plllocked(msg);
//	RTX_SendBytes(msg,len);
}

int main()
{
	unsigned char cmd,flag = 0,ledf = 0;
				  
	//config sysclk
	HW_SysclkInit();

	//config pins
 	HW_IOInit();

	//config perhaps
	HW_T2Init();
	HW_IntcInit();
	RTX_Init();
	ADC_Init();

	//len = _sprintf_splashscreen(msg);
	//RTX_SendBytes(msg,len);

	delay_ms(500);

	//config dds
	PLL_Reset();
	PLL_Tx_Config();
	PLL_Rx_Config();

	/*
	len = _sprintf_pllwait(msg);
	RTX_SendBytes(msg,len);
	delay_ms(1000);
	if(PLL_IsLocked() == 1)
	{
		 len = _sprintf_plllocked(msg);
		 RTX_SendBytes(msg,len);
	}
	*/
	/*
	while(1)
	{
		if(PLL_IsLocked() != 1)
		{
			PLL_Reset();
			PLL_Config();

			len = _sprintf_pllwait(msg);
			RTX_SendBytes(msg,len);
			delay_ms(1000);

			flag = 1;
		}
		else if(flag == 1)
		{
			len = _sprintf_plllocked(msg);
			RTX_SendBytes(msg,len);
			flag = 0;
		}

		if(Cmd_IsNew())
		{
			Cmd_get(&cmd);
			switch(cmd)
			{
				case 0x00://发射功率：0-正功率值，1-负功率值
					GetPowerVoltage();
					break;
				case 0x01://本振锁定指示
					GetPLLLockSta();
					break;
				case 0x02://以上所有参数
					GetPowerVoltage();
					GetPLLLockSta();
					break;
				default:
					break;
			}
		}
	}
	*/
}





