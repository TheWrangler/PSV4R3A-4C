#include "stc8.h"
#include "pll.h"
#include "utily.h"

//∑¢…‰4300MHz
double tx_freq = 4300.0;
unsigned int tx_nint_ratio = 430/*430*/;
unsigned char tx_r_ratio = 4/*10*/;	   
unsigned long tx_nfrac_ratio = 0;
unsigned long tx_reg_var[6] = {0x1f86ff,0xf6800a,0xe00000,0x1ae,0x0};

//Ω” ’4289.231MHz
double rx_freq = 4289.231;
unsigned int rx_nint_ratio = 428/*430*/;
unsigned char rx_r_ratio = 4/*10*/;	   
unsigned long rx_nfrac_ratio = 15487048;
unsigned long rx_reg_var[6] = {0x1f86ff,0xf6800a,0xe00000,0x1ae,0x0};

sbit tx_lock = P0^6;
sbit rx_lock = P2^6;

sbit dout = P2^7;
sbit clk = P0^5;
sbit tx_le = P0^3;
sbit rx_le = P0^1;

void PLL_Reset()
{
	clk = 0;
	dout = 0;
	rx_le = 1;
	tx_le = 1;
}

void PLL_Init()
{
	//tx
	//reference div ratio
	tx_reg_var[1] &= 0xffc000;
	tx_reg_var[1] |= tx_r_ratio;

	//n.int div ratio
	tx_reg_var[3] &= 0xff0000;
	tx_reg_var[3] |= tx_nint_ratio;

	//n.frac div ratio
   	tx_reg_var[4] &= 0x000000;
	tx_reg_var[4] |= tx_nfrac_ratio;


	//rx
	//reference div ratio
	rx_reg_var[1] &= 0xffc000;
	rx_reg_var[1] |= rx_r_ratio;

	//n.int div ratio
	rx_reg_var[3] &= 0xff0000;
	rx_reg_var[3] |= rx_nint_ratio;

	//n.frac div ratio
   	rx_reg_var[4] &= 0x000000;
	rx_reg_var[4] |= rx_nfrac_ratio;
}

void PLL_Adjust(char var)
{
	double temp;

	tx_freq += var;
	temp = tx_freq / 10.0;
	tx_nint_ratio = temp;

	temp =  temp - tx_nint_ratio;
	temp =  temp * 4096.0;
	temp *= 4096.0; 
	tx_nfrac_ratio = temp;


	rx_freq += var;
	temp = rx_freq / 10.0;
	rx_nint_ratio = temp;

	temp =  temp - rx_nint_ratio;
	temp =  temp * 4096.0;
	temp *= 4096.0; 
	rx_nfrac_ratio = temp;

	PLL_Reset();
	PLL_Init();
	PLL_Tx_Config();
	PLL_Rx_Config();
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

signed char PLL_IsTxLocked()
{
	return tx_lock;
}

signed char PLL_IsRxLocked()
{
	return rx_lock;
}