void Cmd_InitTable();

void Cmd_set(unsigned char content);
void Cmd_Del(unsigned char size);
unsigned char Cmd_IsNew();
void Cmd_reply();

void BuildPwrRply();
void BuildLOLock();
void BulidPwr();

void process_a1(unsigned char cmd_wd);
void process_a2(unsigned char cmd_wd);
void process_a3(unsigned char cmd_wd);