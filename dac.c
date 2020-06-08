#include "stc8.h"

sbit scl = P3^5;
sbit sda = P3^6;

sbit a0 = P5^0;
sbit a1 = P5^1;


void InitI2C()
{
	a0 = 0;
	a1 = 0;

	I2CCFG |= 40;

	I2CCFG &= ~0x03f;
	I2CCFG |= 63;

	I2CSLADR = 0x28;

	I2CMSCR &= ~0x80;

	I2CCFG |= 0x80;
}


void WriteI2C(unsigned char var)
{
	//data+ack
	I2CTXD = 0x00;
	I2CMSCR &= ~0x0f;
	I2CMSCR |= 0x0a;

	while((I2CMSST & 0x80) == 0x80);

	//data+ack
	I2CTXD = var;
	I2CMSCR &= ~0x0f;
	I2CMSCR |= 0x0a;

	while((I2CMSST & 0x80) == 0x80);

	//stop
   	I2CMSCR &= ~0x0f;
	I2CMSCR |= 0x06;
}

