/* 
 * File:   pwm.h
 * Author: exp10
 *
 * Created on 28 марта 2022 г., 23:02
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define TIMER2_PERIOD 0x7F
#define MAX_PWM TIMER2_PERIOD
#define MIN_PWM 1

    void initPWM();
    void setDuty(unsigned int duty);
    void disablePWM();

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

