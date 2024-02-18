#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "main.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char checksum = 0xFE;

    checksum ^= (unsigned char) msgFunction;
    checksum ^= (unsigned char) (msgFunction >> 8);

    checksum ^= (unsigned char) msgPayloadLength;
    checksum ^= (unsigned char) (msgPayloadLength >> 8);

    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }

    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char trame[msgPayloadLength + 6];
    trame[0] = 0xFE;
    trame[1] = (unsigned char) (msgFunction >> 8);
    trame[2] = (unsigned char) (msgFunction);
    trame[3] = (unsigned char) (msgPayloadLength >> 8);
    trame[4] = (unsigned char) (msgPayloadLength);

    for (int i = 0; i < msgPayloadLength + 6; i++) {
        trame[i + 5] = msgPayload[i];
    }

    trame[msgPayloadLength + 5] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

    SendMessage(trame, (msgPayloadLength + 6));
}

enum StateReception rcvState = Waiting;
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entree un octet et servant a reconstituer les trames
    switch (rcvState) {
        case Waiting:
            if (c == 0xFE) {
                msgDecodedPayloadIndex = 0;
                rcvState = FunctionMSB;
            }
            break;

        case FunctionMSB:
            msgDecodedFunction = c << 8;
            rcvState = FunctionLSB;
            break;

        case FunctionLSB:
            msgDecodedFunction |= c;
            rcvState = PayloadLengthMSB;
            break;

        case PayloadLengthMSB:
            msgDecodedPayloadLength = c << 8;
            rcvState = PayloadLengthLSB;
            break;

        case PayloadLengthLSB:
            msgDecodedPayloadLength |= c;
            rcvState = Payload;
            break;

        case Payload:
            msgDecodedPayload[msgDecodedPayloadIndex] = c;
            rcvState = Payload;

            if ((msgDecodedPayloadLength - 1) == msgDecodedPayloadIndex) {
                rcvState = CheckSum;
            }
            msgDecodedPayloadIndex++;
            break;

        case CheckSum:
            if (UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload) == c) {
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            }
            rcvState = Waiting;
            break;

        default:
            rcvState = Waiting;
            break;
    }
}

void UartProcessDecodedMessage(int msgFunction, int payloadLength, unsigned char* msgPayload) {
    //Fonction appelee apres le decodage pour executer l'action correspondant au message recu
    switch (msgFunction) {
        case SET_ROBOT_CONTROL:
            SetRobotState(msgPayload[0]);
            break;
            
        case SET_ROBOT_AUTO_CONTROL:
            SetRobotAutoControlState(msgPayload[0]);
            break;
            
        default:
            break;
    }
}

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
char autoControlActivated = 1;

void SetRobotState(unsigned char payload)
{
    switch(payload)
    {
        case STATE_ATTENTE:
            stateRobot = STATE_ATTENTE;
            break;
        
        case STATE_ATTENTE_EN_COURS:
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;
         
        case STATE_AVANCE:
            stateRobot = STATE_AVANCE;
            break;
        
        case STATE_AVANCE_EN_COURS:
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE:
            stateRobot = STATE_TOURNE_GAUCHE;
            break;
        
        case STATE_TOURNE_GAUCHE_EN_COURS:
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
         
        case STATE_TOURNE_DROITE:
            stateRobot = STATE_TOURNE_DROITE;
            break;
        
        case STATE_TOURNE_DROITE_EN_COURS:
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
         
        case STATE_TOURNE_SUR_PLACE_DROITE:
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_ARRET:
            stateRobot = STATE_ARRET;
            break;
        
        case STATE_ARRET_EN_COURS:
            stateRobot = STATE_ARRET_EN_COURS;
            break;
         
        case STATE_RECULE:
            stateRobot = STATE_RECULE;
            break;
        
        case STATE_RECULE_EN_COURS:
            stateRobot = STATE_RECULE_EN_COURS;
            break;
            
        case STATE_TOURNE_DROITE_VITE:
            stateRobot = STATE_TOURNE_DROITE_VITE;
            break;
            
        case STATE_TOURNE_DROITE_VITE_EN_COURS:
            stateRobot = STATE_TOURNE_DROITE_VITE_EN_COURS;
            break;
            
        case STATE_TOURNE_GAUCHE_VITE:
            stateRobot = STATE_TOURNE_GAUCHE_VITE;
            break;
            
        case STATE_TOURNE_GAUCHE_VITE_EN_COURS:
            stateRobot = STATE_TOURNE_GAUCHE_VITE_EN_COURS;
            break;
            
        case STATE_FREINAGE:
            stateRobot = STATE_FREINAGE;
            break;
            
        case STATE_FREINAGE_EN_COURS:
            stateRobot = STATE_FREINAGE_EN_COURS;
            break;
    }
}

void SetRobotAutoControlState(unsigned char payload)
{
    if(payload == 1)
    {
        autoControlActivated = 1;
    }
    else if(payload == 0)
    {
        autoControlActivated = 0;
        stateRobot = STATE_ATTENTE;
    }
}