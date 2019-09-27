 #include "utily.h"


/**************************************
功能描述：延时函数
入口参数：uint16 x ，该值为1时，延时1ms
返回值：无
***************************************/
void delay_ms(unsigned int x) 
{  
	unsigned int j,i;   
	for(j=0;j<x;j++)   
	{    
		for(i=0;i<1100;i++);   
	}  
}
