#include <xc.h>

void init_timer1(uint16_t initial) {
    /* Crystal oscillator on T1OSI/T1OSO */
    T1CONbits.TMR1CS = 0b10;
    /* Enable oscillator */
    T1CONbits.T1OSCEN = 1;
    /* Prescaler 1:1 */
    T1CONbits.T1CKPS = 0b00;
    /* Do not synchronize external clock input */
    T1CONbits.nT1SYNC = 1;
    /* Do not start the timer */
    T1CONbits.TMR1ON = 0;
    TMR1 = initial;
    TMR1IF = 0;
    TMR1IE = 1;
}

void startTimer1() {
    /* Start timer */
    T1CONbits.TMR1ON = 1;
}

void stopTimer1() {
    /* Stop timer */
    T1CONbits.TMR1ON = 0;
}
