#include "STC8.H"
#include "pll.h"
#include "hw.h"
#include "adc.h"
#include "rtx.h"
#include "cmd.h"

//WARNNING:STC MCU int protype is 16 bit width,not 32 bit width£¡£¡£¡£¡£¡£¡

int main()
{
	unsigned char cmd;

	//config sysclk
	//HW_SysclkInit();

	//config pins
 	HW_IOInit();

	//config dds
	PLL_Reset();
	PLL_Config();
	while(PLL_IsLocked() != 1);

	//config perhaps
	//HW_T2Init();
	//HW_IntcInit();
	//RTX_Init();
	//ADC_Init();

	while(1)
	{
		//RTX_Send(0xaa);
	}
	
	while(1)
	{
		if(Cmd_IsNew())
		{
			Cmd_get(&cmd);
			switch(cmd)
			{
				case 0x00:
					break;
				case 0x01:
					break;
				case 0x02:
					break;
				case 0x03:
					break;
				default:
					break;
			}
		}
	}
}





