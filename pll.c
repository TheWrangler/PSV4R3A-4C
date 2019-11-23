#include "stc8.h"
#include "pll.h"
#include "utily.h"

unsigned int nint_ratio = 172/*430*/;
unsigned int r_ratio = 4/*10*/;	   
unsigned long nfrac_ratio = 0;
unsigned long reg_var[6] = {0x1f867f,0xf6800a,0xfc0001,0x1ae,0x0};

sbit ldo = P0^3;
sbit dout = P0^4;
sbit clk = P0^5;
sbit le = P0^6;

void PLL_Reset()
{
	clk = 0;
	dout = 0;
	le = 1;

	//reference div ratio
	reg_var[1] &= 0xffc000;
	reg_var[1] |= r_ratio;

	//n.int div ratio
	reg_var[3] &= 0xff0000;
	reg_var[3] |= nint_ratio;

	//n.frac div ratio
   	reg_var[4] &= 0xf00000;
	reg_var[4] |= nfrac_ratio;
}

void PLL_WriteReg(unsigned char addr,unsigned long var)
{
	unsigned char i;
	unsigned long content = addr,temp;
	content = content << 24;
	content |= var;
	
	le = 0;
	
	for(i=0;i<27;i++)
	{
		temp = (0x04000000 >> i);
		
		clk = 0;
		
		if((content & temp) == temp)
			dout = 1;
		else dout = 0;
		
		clk = 1;
	}	
	
	clk = 0;
	dout = 0;
	le = 1;
}

void PLL_Config()
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		PLL_WriteReg(i, reg_var[i]);

		le = 1;
		delay_ms(1);
		le = 0;
		delay_ms(1);
		le = 1;
	}
}

signed char PLL_IsLocked()
{
	return ldo;
}