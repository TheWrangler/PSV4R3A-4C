#include "stc8.h"
#include "pll.h"
#include "utily.h"

unsigned int nint_ratio = 172/*430*/;
unsigned int r_ratio = 4/*10*/;	   
unsigned long nfrac_ratio = 0;
unsigned long reg_var[6] = {0x1f867f,0xf6800a,0xfc0001,0x1ae,0x0};

unsigned int tx_vol[50] = {0x0060,0x0066,0x006d,0x0072,0x007a,0x0083,0x0096,0x009e,0x00ae,0x00ba,
							0x00c3,0x00ce,0x00d4,0x00ed,0x00f8,0x00ff,0x0105,0x0122,0x00142,0x016b,
							0x019f,0x01d5,0x01e8,0x0225,0x024d,0x028a,0x02c6,0x030a,0x0352,0x0394,
							0x03dc,0x0435,0x0495,0x04eb,0x0545,0x05b0,0x061e,0x0674,0x06e8,0x0763,
							0x07d0,0x0865,0x08f0,0x0974,0x09f9,0x0a7f,0x0af7,0x0b70,0x0bbb,0x0c50};

sbit ldo = P0^3;
sbit dout = P0^4;
sbit clk = P0^5;
sbit le = P0^6;

sbit txen = P0^7;

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

void PLL_TxEn(unsigned char en)
{
	if(en == 1)
		txen = 1;
	else txen = 0;	
}

unsigned char PLL_GetATT(unsigned int pwr)
{
	unsigned char temp;
	signed char var;

	for(temp=0;temp<50;temp++)
	{
	 	if(tx_vol[temp] > pwr)
			break;
	}

	var = temp;
	var -= 46;

	if(var < 0)
	{
	 	temp = -var;
		temp |= 0x80;
	}
	else temp = var;

	return temp;	 
}