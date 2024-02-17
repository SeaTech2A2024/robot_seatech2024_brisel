#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char checksum = 0xFE;
    
    checksum ^= (unsigned char)msgFunction;
    checksum ^= (unsigned char)(msgFunction>>8);
    
    checksum ^= (unsigned char)msgPayloadLength;
    checksum ^= (unsigned char)(msgPayloadLength >> 8);

    for (int i = 0; i < msgPayloadLength; i++)
    {
        checksum ^= msgPayload[i];
    }

    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char trame[msgPayloadLength+6];
    trame[0] = 0xFE;
    trame[1] = (unsigned char)(msgFunction>>8);
    trame[2] = (unsigned char)(msgFunction);
    trame[3] = (unsigned char)(msgPayloadLength>>8);
    trame[4] = (unsigned char)(msgPayloadLength);
    
    for(int i=0; i<msgPayloadLength+6; i++)
    {
        trame[i+5] = msgPayload[i];
    }
    
    trame[msgPayloadLength+5] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    
    SendMessage(trame,(msgPayloadLength+6));
}
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

//void UartDecodeMessage(unsigned char c) {
//    //Fonction prenant en entree un octet et servant a reconstituer les trames
//    ...
//}
//
//void UartProcessDecodedMessage(int function,
//        int payloadLength, unsigned char* payload) {
//    //Fonction appelee apres le decodage pour executer l?action
//    //correspondant au message recu
//    ...
//}



//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/