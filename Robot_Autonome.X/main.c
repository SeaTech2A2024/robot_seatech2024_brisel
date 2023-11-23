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
    
    PWMSetSpeedConsigne(0, MOTEUR_DROIT);
    PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
    
    unsigned int *result;
    unsigned int ADCValue0;
    unsigned int ADCValue1;
    unsigned int ADCValue2;

    
    while(1){
        if(ADCIsConversionFinished()){
            ADCClearConversionFinishedFlag();
            
            result = ADCGetResult();
            
            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];
        }
        
    }

    return (EXIT_SUCCESS);
}
