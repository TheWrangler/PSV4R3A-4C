void Cmd_InitTable();

void Cmd_set(unsigned char content);
void Cmd_Del(unsigned char size);
unsigned char Cmd_IsNew();
void Cmd_reply();
void Cmd_ack(unsigned char size);

void BuildPwrRply();
void BuildLOLockRply();
void BuildTxPwrRply();
void BuildTxLO1PwrRply();
void BuildTxLO2PwrRply();
void BuildRxLO3Rply();
void BuildAllStateRply();
void BuildTxEnableRply();
void BuildTxPowerATTRply();
void BuildHWVerisonRply();
void BuildSWVersionRply();

void process_a1(unsigned char cmd_wd);
void process_a2(unsigned char cmd_wd);
void process_a3(unsigned char cmd_wd);