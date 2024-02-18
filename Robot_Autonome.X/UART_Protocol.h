/* 
 * File:   UART_Protocol.h
 * Author: andre
 *
 * Created on February 16, 2024, 9:50 AM
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#define SET_ROBOT_CONTROL 0x0051
#define SET_ROBOT_AUTO_CONTROL 0x0052

extern char autoControlActivated;

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);
void SetRobotState(unsigned char payload);
void SetRobotAutoControlState(unsigned char payload);

enum StateReception
{
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
};

#endif	/* UART_PROTOCOL_H */

