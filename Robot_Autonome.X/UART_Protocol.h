/* 
 * File:   UART_Protocol.h
 * Author: andre
 *
 * Created on February 16, 2024, 9:50 AM
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_PROTOCOL_H */

