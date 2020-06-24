#include "STC8.H"
#include "pll.h"
#include "hw.h"
#include "adc.h"
#include "rtx.h"
#include "cmd.h"
#include "utily.h"
#include "pwm.h"

//WARNNING:STC MCU int protype is 16 bit width,not 32 bit width
extern unsigned char cmd[5];


int main()
{			  
	//config sysclk
	HW_SysclkInit();

	//config pins
 	HW_IOInit();

	//config perhaps
	HW_T2Init();
	HW_IntcInit();
	RTX_Init();
	ADC_Init();
	PWM_Init();

	Cmd_InitTable();

	delay_ms(200);

	//config dds
	PLL_Init();
	PLL_Reset();
	PLL_Tx_Config();
	PLL_Rx_Config();

	delay_ms(200);

//	PWM_Ctrl(79.5);

	while(1)
	{
		if(Cmd_IsNew())
		{
			switch(cmd[0])
			{
				case 0xa1:
					process_a1(cmd[2]);
					break;
				case 0xa2:
					process_a2(cmd[2]);
					break;
				case 0xa3:
					process_a3(cmd[2]);
					break;
				default:
					break;
			}
		}
	}
}





