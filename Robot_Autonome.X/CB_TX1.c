#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_TX1.h"
#define CBTX1_BUFFER_SIZE 128
int cbTx1Head;
int cbTx1Tail;
unsigned char cbTx1Buffer[CBTX1_BUFFER_SIZE];
unsigned char isTransmitting = 0;
void SendMessage(unsigned char* message, int length)
{
unsigned char i=0;
if(CB_TX1_GetRemainingSize()>length)
{
//On peut écrire le message
for(i=0;i<length;i++)
CB_TX1_Add(message[i]);
if(!CB_TX1_IsTranmitting())
SendOne();
}
}
void CB_TX1_Add(unsigned char value)
{
    if(CB_TX1_GetRemainingSize()>length){
        cbTx1Buffer[cbTx1Head]=value;
        cbTxHead+=1
    }
}
unsigned char CB_TX1_Get(void)
{
    unsigned char value =cbTxBuf[Tail];
    Tail++;
    if(cbTxTail)==()){
        Tail=0;
    }
}
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
IFS0bits.U1TXIF = 0; // clear TX interrupt flag
if (cbTx1Tail!=cbTx1Head)
{
SendOne();
}
else
isTransmitting = 0;
}
void SendOne()
{
isTransmitting = 1;
unsigned char value=CB_TX1_Get();
U1TXREG = value; // Transmit one character
}
unsigned char CB_TX1_IsTranmitting(void)
{
//...
}
int CB_TX1_GetDataSize(void)
{
//return size of data stored in circular buffer
int dataSize;
//...
return dataSize;
}
int CB_TX1_GetRemainingSize(void)
{
//return size of remaining size in circular buffer
int remainingSize;
if ((cbTx1Head-cbTx1Tail)>0){
remainigSize=CBTX1_BUFFER_SIZE-(cbTx1Head-cbTx1Tail);}
else 
return remainingSize;
}
