void PLL_Reset();
void PLL_WriteReg(unsigned char addr,unsigned long var);
void PLL_Config();
signed char PLL_IsLocked();
void PLL_TxEn(unsigned char en);
unsigned char PLL_GetATT(unsigned int pwr);