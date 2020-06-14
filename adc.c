#include "stc8.h"
#include "adc.h"

void ADC_Init()
{
	//open adc power
	ADC_CONTR |= 0x80;
	
	//set format to LSB.
	ADCCFG |= 0x20;
	
	//set speed
	ADCCFG &= ~0x0f;

	ADC_Get(ADC_CH_P1_1);
}

unsigned int ADC_Get(ADC_CH ch)
{
	unsigned int var;

	ADC_CONTR &= ~0x20;
	
	//select ch
	ADC_CONTR &= 0xf0;
	ADC_CONTR |= ch;
	
	//start convert
	ADC_CONTR |= 0x40;
	
	//wait for convert finished
	while((ADC_CONTR & 0x20) != 0x20);
	
	//read result
	var = ADC_RES;
	var = var << 8;
	var += ADC_RESL;
	
	return var;
}