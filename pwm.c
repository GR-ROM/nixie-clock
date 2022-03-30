#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 8000000UL
#define FREQUENCY_MIN 30000 /* Hz */
#define TIMER_PRESCALER 1 /* 1:1 */
#define FREQUENCY_TO_PR_CONVERT(F) (uint8_t)(((_XTAL_FREQ)/ \
(4*(F))/(TIMER_PRESCALER))-1)

void setDuty(int duty) {
    CCP1CONbits.DC1B=duty & 0x3;
    CCPR1L=(duty >> 2) & 0xFF; 
}

void init_pwm(unsigned char period) {
    TMR2IF=0;
    T2CONbits.T2CKPS=0;         // set prescaler to 1:1
    T2CONbits.T2OUTPS=0;        // set postscaler to 1:1
    TMR2=0x00;                  
    PR2=period;                 // set period as req-d
    TMR2IE=0;
    
    CCP1CONbits.DC1B=1;
    CCPR1H=0x00;
    CCPR1L=0x00;        
            
    CCPTMRS0bits.C1TSEL=0b00;
    CCP1CONbits.CCP1M=0b1100;   // PWM mode
    CCP1CONbits.P1M=0b00;          // modulated P1A   
    T2CONbits.TMR2ON=1;
}

