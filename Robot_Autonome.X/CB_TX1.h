#ifndef CB_TX1_H
#define CB_TX1_H

void CB_TX1_Add(unsigned char value);
void SendOne();
void SendMessage(unsigned char* message, int length);
unsigned char CB_TX1_Get(void);
unsigned char CB_TX1_IsTranmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_GetRemainingSize(void);
int cbTx1Head;
int cbTx1Tail;
unsigned char cbTx1Buffer[];
unsigned char isTransmitting = 0;



#endif /* CB_TX1_H */