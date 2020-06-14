#include "STC8.H"
#include "pll.h"
#include "hw.h"
#include "adc.h"
#include "rtx.h"
#include "cmd.h"
#include "utily.h"

//WARNNING:STC MCU int protype is 16 bit width,not 32 bit width！！！！！！

unsigned char msg[5];
unsigned char len;

void GetPowerVoltage()
{
	unsigned int var;
	var = ADC_Get(ADC_CH_P1_1);
	msg[0] = var >> 8;
	msg[1] = var;
	len = 2;
	RTX_SendBytes(msg,len);	
}

void GetPLLLockSta()
{
	if(PLL_IsLocked() == 1)
		msg[0] =  0;
	else  msg[0] =  1;
	len = 1;
	RTX_SendBytes(msg,len);
}

int main()
{
	unsigned char cmd;
				  
	//config sysclk
	HW_SysclkInit();

	//config pins
 	HW_IOInit();

	//config perhaps
	HW_T2Init();
	HW_IntcInit();
	RTX_Init();
	ADC_Init();

	delay_ms(500);

	//config dds
	PLL_Reset();
	PLL_Config();

	while(1)
	{
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
				case 0x03:
					PLL_TxEn(1);
					break;
				case 0x04:
					PLL_TxEn(0);
					break;
				default:
					break;
			}
		}
	}
}





