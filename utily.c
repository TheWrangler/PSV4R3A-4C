 #include "utily.h"


/**************************************
������������ʱ����
��ڲ�����uint16 x ����ֵΪ1ʱ����ʱ1ms
����ֵ����
***************************************/
void delay_ms(unsigned int x) 
{  
	unsigned int j,i;   
	for(j=0;j<x;j++)   
	{    
		for(i=0;i<1100;i++);   
	}  
}

unsigned char CRC(unsigned char* buf,unsigned char size)
{
	unsigned char crc = 0,i=0;
	for(i=0;i<size;i++)
		crc += buf[i];
	return crc;
}
