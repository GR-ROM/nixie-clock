/* 
 * File:   timer1.h
 * Author: exp10
 *
 * Created on 30 марта 2022 г., 7:11
 */

#ifndef TIMER0_H
#define	TIMER0_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_timer0();
void inline stop_timer0();
void inline start_timer0();
void disableTimer0();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER0_H */

