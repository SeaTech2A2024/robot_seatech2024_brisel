/* 
 * File:   PWM.h
 * Author: GEII Robot
 *
 * Created on 29 septembre 2023, 14:38
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_GAUCHE 0
#define MOTEUR_DROIT 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, uint8_t moteur);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);

#endif	/* PWM_H */

