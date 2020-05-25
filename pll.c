#include "stc8.h"
#include "pll.h"
#include "utily.h"

//∑¢…‰4300MHz
unsigned int tx_nint_ratio = 430/*430*/;
unsigned int tx_r_ratio = 4/*10*/;	   
unsigned long tx_nfrac_ratio = 0;
unsigned long tx_reg_var[6] = {0x1f867f,0xf6800a,0xfc0001,0x1ae,0x0};
//Ω” ’4289.231MHz
unsigned int rx_nint_ratio = 428/*430*/;
unsigned int rx_r_ratio = 4/*10*/;	   
unsigned long rx_nfrac_ratio = 967940;
unsigned long rx_reg_var[6] = {0x1f867f,0xf6800a,0xfc0001,0x1ae,0x0};

sbit ldo = P0^3;

sbit dout = P2^7;
sbit clk = P0^5;
sbit tx_le = P0^3;
sbit rx_le = P0^0;

void PLL_Reset()
{
	clk = 0;
	dout = 0;
	rx_le = 1;
	tx_le = 1;

	//tx
	//reference div ratio
	tx_reg_var[1] &= 0xffc000;
	tx_reg_var[1] |= tx_r_ratio;

	//n.int div ratio
	tx_reg_var[3] &= 0xff0000;
	tx_reg_var[3] |= tx_nint_ratio;

	//n.frac div ratio
   	tx_reg_var[4] &= 0xf00000;
	tx_reg_var[4] |= tx_nfrac_ratio;


	//rx
	//reference div ratio
	rx_reg_var[1] &= 0xffc000;
	rx_reg_var[1] |= rx_r_ratio;

	//n.int div ratio
	rx_reg_var[3] &= 0xff0000;
	rx_reg_var[3] |= rx_nint_ratio;

	//n.frac div ratio
   	rx_reg_var[4] &= 0xf00000;
	rx_reg_var[4] |= rx_nfrac_ratio;
}

void PLL_WriteTxReg(unsigned char addr,unsigned long var)
{
	unsigned char i;
	unsigned long content = addr,temp;
	content = content << 24;
	content |= var;
	
	tx_le = 0;
	
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
	tx_le = 1;
}

void PLL_WriteRxReg(unsigned char addr,unsigned long var)
{
	unsigned char i;
	unsigned long content = addr,temp;
	content = content << 24;
	content |= var;
	
	rx_le = 0;
	
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
	rx_le = 1;
}

void PLL_Tx_Config()
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		PLL_WriteTxReg(i, tx_reg_var[i]);

		tx_le = 1;
		delay_ms(1);
		tx_le = 0;
		delay_ms(1);
		tx_le = 1;
	}
}

void PLL_Rx_Config()
{
	unsigned char i;
	for(i=0;i<5;i++)
	{
		PLL_WriteRxReg(i, rx_reg_var[i]);

		rx_le = 1;
		delay_ms(1);
		rx_le = 0;
		delay_ms(1);
		rx_le = 1;
	}
}

signed char PLL_IsLocked()
{
	return ldo;
}