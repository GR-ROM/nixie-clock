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

    void init_pwm();
    void setDuty(int duty);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

