void RTX_Init();
unsigned char RTX_IsRecvInt();
void RTX_Recv(unsigned char *buf);
void RTX_Send(unsigned char buf);
void RTX_SendBytes(unsigned char* buf,unsigned char len);