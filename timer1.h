/* 
 * File:   timer1.h
 * Author: exp10
 *
 * Created on March 30, 2022, 10:38 AM
 */

#ifndef TIMER1_H
#define	TIMER1_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_timer1(uint16_t initial);    
void startTimer1();
void stopTimer1();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER1_H */

