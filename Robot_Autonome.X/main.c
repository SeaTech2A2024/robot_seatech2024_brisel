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

/*
 * 
 */
int main(int argc, char** argv) {
    
    InitOscillator();
    
    InitIO();
    
    LED_ORANGE = 1;
    LED_BLEUE = 1;
    LED_BLANCHE = 1;
    
    while(1){
        LED_ORANGE = ~LED_ORANGE;
        LED_BLEUE = ~LED_BLEUE;
        LED_BLANCHE = ~LED_BLANCHE;
    }

    return (EXIT_SUCCESS);
}
