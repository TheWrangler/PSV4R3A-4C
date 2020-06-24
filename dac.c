#include "stc8.h"
#include "dac.h"

sbit scl = P3^5;
sbit sda = P3^6;

sbit a0 = P5^0;
sbit a1 = P5^1;


void InitI2C()
{
	a0 = 0;
	a1 = 0;

	scl = 1;
	sda = 1;
}


void WriteI2C(unsigned char var)
{
	//start bit
	sda = 0;
	scl = 0;
	
	//i2c write address
	SendWriteAddress();	

	//wait ack
	scl = 0;
	while(sda != 0);
	scl = 1;

	//i2c reg address
	SendByte(0x00);

	//wait ack
	scl = 0;
	while(sda != 0)
	scl = 1;

	//i2c reg value
	SendByte(var);

	//wait ack
	scl = 0;
	while(sda != 0);
	scl = 1;

	//stop bit
	scl = 1;
	sda = 1;
}

void SendWriteAddress()
{
	unsigned char addr = 0x50,i;
	for(i=0;i<8;i++)
	{
		scl = 0;
		sda = ((addr & 0x80) >> 7);
		scl = 1;
		addr = (addr << 1);
	}
}

void SendByte(unsigned char val)
{
	unsigned char value = val,i;
	for(i=0;i<8;i++)
	{
		scl = 0;
		sda = ((value & 0x80) >> 7);
		scl = 1;
		value = (value << 1);
	}	
}

