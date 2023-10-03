/* 
 * File:   ADC.h
 * Author: GEII Robot
 *
 * Created on 3 octobre 2023, 17:39
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);

#endif	/* ADC_H */

