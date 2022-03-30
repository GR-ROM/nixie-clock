/* 
 * File:   gpio.h
 * Author: exp10
 *
 * Created on March 30, 2022, 12:15 PM
 */

#ifndef GPIO_H
#define	GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#define AN1 PORTAbits.RA0
#define AN2 PORTBbits.RB2
#define AN3 PORTBbits.RB3
#define AN4 PORTBbits.RB1
    
#define CA0 PORTAbits.RA1
#define CA1 PORTCbits.RC4
#define CA2 PORTAbits.RA2
#define CA3 PORTCbits.RC6
#define CA4 PORTAbits.RA4
#define CA5 PORTBbits.RB0
#define CA6 PORTAbits.RA5
#define CA7 PORTCbits.RC7
#define CA8 PORTAbits.RA3
#define CA9 PORTCbits.RC5

void init_gpio();


#ifdef	__cplusplus
}
#endif

#endif	/* GPIO_H */

