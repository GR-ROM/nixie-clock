#include <xc.h>

void init_timer1(uint16_t initial) {
    T1CONbits.T1OSCEN=1;
    T1CONbits.TMR1CS=0b10;
    
    T1CONbits.T1CKPS=0b00;
    T1CONbits.nT1SYNC=1;
    T1CONbits.TMR1ON=1;
    TMR1=initial;
    TMR1IF=0;
    TMR1IE=1;
}

void startTimer1() {
    T1CONbits.T1OSCEN=1;
}

void stopTimer1() {
    T1CONbits.T1OSCEN=0;
}
