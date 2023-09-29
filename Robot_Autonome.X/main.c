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


int main(int argc, char** argv) {
    
    //Initialization of clock
    InitOscillator();
    
    //Initialization of GPIOs
    InitIO();
    
    InitPWM();
    PWMSetSpeed(0, MOTEUR_GAUCHE);
    
    while(1){
        
    }

    return (EXIT_SUCCESS);
}
