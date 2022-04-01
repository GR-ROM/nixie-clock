#include "timer0.h"
#include <xc.h>

void inline start_timer0() {
    OPTION_REGbits.T0CS=0;
}

void inline stop_timer0() {
    OPTION_REGbits.T0CS=1;
}

void init_timer0() {
    TMR0=0;
    OPTION_REGbits.PS=0b010;  // 1:16
    OPTION_REGbits.PSA=0;
      
    TMR0IF=0;
    TMR0IE=1;
}

void disableTimer0() {
    OPTION_REGbits.PS=0b000;  // off
    TMR0IF=0;
    TMR0IE=0;
}