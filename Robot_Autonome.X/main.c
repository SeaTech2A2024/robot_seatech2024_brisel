/* 
 * File:   main.c
 * Author: GEII Robot
 *
 * Created on 28 septembre 2023, 10:09
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "timer.h"
#include "robot.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"

unsigned char stateRobot;


void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_GAUCHE_VITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_VITE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_VITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_DROITE_VITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_VITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_VITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_FREINAGE:
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-25, MOTEUR_GAUCHE);

        case STATE_FREINAGE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    //unsigned char positionObstacle = PAS_D_OBSTACLE;
    int sensorState=0b00000;

    if (robotState.distanceTelemetreDroit < 30)
        sensorState |= 0b00001;
    if (robotState.distanceTelemetreMilieuDroit < 35)
        sensorState |= 0b00010;
    if (robotState.distanceTelemetreCentre < 40)
        sensorState |= 0b00100;
    if (robotState.distanceTelemetreMilieuGauche < 35)
        sensorState |= 0b01000;
    if (robotState.distanceTelemetreGauche < 30)
        sensorState |= 0b10000;

    
    
    switch (sensorState) {
        case 0b00000:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE_VITE;
            break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE_VITE;
            break;
        case 0b00100:
            //nextStateRobot = STATE_FREINAGE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_GAUCHE_VITE;
            break;
        case 0b00110:
            //nextStateRobot = STATE_FREINAGE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00111:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE_VITE;
            break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;

        case 0b01010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01100:
            //nextStateRobot = STATE_FREINAGE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01110:
            //nextStateRobot = STATE_FREINAGE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;

        case 0b10000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_DROITE_VITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;

        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE_VITE;
            break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_DROITE_VITE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;         

    }
    
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
    
        if (nextStateRobot == stateRobot - 4)
        stateRobot = STATE_TOURNE_SUR_PLACE_DROITE;

    if (stateRobot == STATE_TOURNE_DROITE_VITE || stateRobot == STATE_TOURNE_GAUCHE_VITE || stateRobot == STATE_TOURNE_DROITE || stateRobot == STATE_TOURNE_GAUCHE) {
        timer_bloc = 0;
        if (nextStateRobot == STATE_AVANCE)
            timer_bloc = 0;
        if (timer_bloc == 5000)
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    }
    
    if (stateRobot == STATE_FREINAGE_EN_COURS){
        nextStateRobot = STATE_TOURNE_GAUCHE;
    }
}

int main(int argc, char** argv) {

    //Initialization of clock
    InitOscillator();

    //Initialization of GPIOs
    InitIO();

    //Initialization of PWM
    InitPWM();

    // Initialization of Timer1
    InitTimer1();

    // Initialization of ADC1
    InitADC1();

    //Initialization of Timer4
    InitTimer4();
    
    // Initialization of the uart
    InitUART();


    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreMilieuDroit = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreMilieuGauche = 34 / volts - 5;
            volts = ((float) result [0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

        }
        SetNextRobotStateInAutomaticMode();
        
        //SendMessageDirect((unsigned char*) "Bonjour", 7);
        //__delay32(40000000);

    }

    return (EXIT_SUCCESS);
}
