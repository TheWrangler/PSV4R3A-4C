#include "stc8.h"
#include "adc.h"
#include "utily.h"

void ADC_Init()
{
	//open adc power
	ADC_CONTR |= 0x80;
	
	//set format to LSB.
	ADCCFG |= 0x20;
	
	//set speed
	ADCCFG &= ~0x0f;
}

unsigned int ADC_Get(ADC_CH ch)
{
	unsigned int var;
	
	//select ch
	ADC_CONTR &= 0xf0;
	ADC_CONTR |= ch;
	
	//start convert
	ADC_CONTR |= 0x40;
	
	//wait for convert finished
	while(ADC_CONTR & 0x40 == 0x40);
	
	//read result
	var = ADC_RES;
	var = var << 8;
	var += ADC_RESL;
	
	return var;
}

unsigned char GetLO1Voltage()
{
	unsigned int var;
	ADC_Get(ADC_CH_P1_0);
	delay_ms(100);
	var = ADC_Get(ADC_CH_P1_0);
	var = (var >> 4);
	return var;
}

unsigned char GetLO3Voltage()
{
	unsigned int var;
	ADC_Get(ADC_CH_P1_0);
	delay_ms(100);
	var = ADC_Get(ADC_CH_P1_0);
	var = (var >> 4);
	return var;
}

