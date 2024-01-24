#ifndef TIMER_H
#define TIMER_H

void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
void SetFreqTimer1(float freq);


extern unsigned long timestamp;
extern unsigned long timer_bloc;
//extern unsigned long timer_freinage;

#endif /* TIMER_H */
