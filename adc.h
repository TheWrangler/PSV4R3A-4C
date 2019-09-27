typedef enum tagADC_CH
{
	ADC_CH_P1_0 = 0,
	ADC_CH_P1_1,
	ADC_CH_P1_2,
	ADC_CH_P1_3,
	ADC_CH_P1_4,
	ADC_CH_P1_5,
	ADC_CH_P1_6,
	ADC_CH_P1_7,
	ADC_CH_P0_0,
	ADC_CH_P0_1,
	ADC_CH_P0_2,
	ADC_CH_P0_3,
	ADC_CH_P0_4,
	ADC_CH_P0_5,
	ADC_CH_P0_6,
	ADC_CH_TEST
}ADC_CH;

void ADC_Init();
unsigned short ADC_Get(ADC_CH ch);