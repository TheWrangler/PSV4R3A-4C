void PLL_Reset();
void PLL_WriteRxReg(unsigned char addr,unsigned long var);
void PLL_WriteTxReg(unsigned char addr,unsigned long var);
void PLL_Tx_Config();
void PLL_Rx_Config();
signed char PLL_IsLocked();