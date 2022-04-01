#include <xc.h>

void initGPIO() {
    ANSELA=0x00;
    ANSELB=0x00;
    
    /* Enable WPU */
    OPTION_REGbits.nWPUEN = 0;
    WPUB = 0x00;
    WPUBbits.WPUB6 = 1;
    WPUBbits.WPUB7 = 1;
    
    /* Inputs */
    ANSELBbits.ANSB5=1;     // HV sense ADC
    TRISBbits.TRISB5=1;
    ANSELBbits.ANSB4=1;     // VDD sense ADC
    TRISBbits.TRISB4=1;
    
    
    TRISBbits.TRISB6=1;     // MODE button
    TRISBbits.TRISB7=1;     // SET button
    
    TRISCbits.TRISC0=1;
    TRISCbits.TRISC1=1;
    /* Outputs */
    TRISCbits.TRISC2=0;     // Set PWM to output
    TRISAbits.TRISA0=0;     // AN1
    TRISBbits.TRISB2=0;     // AN2
    TRISBbits.TRISB3=0;     // AN3
    TRISBbits.TRISB1=0;     // AN4
    
    TRISAbits.TRISA1=0;     // CA0
    TRISAbits.TRISA2=0;     // CA2    
    TRISAbits.TRISA3=0;     // CA8
    TRISAbits.TRISA4=0;     // CA4
    TRISAbits.TRISA5=0;     // CA6
    TRISBbits.TRISB0=0;     // CA5
    TRISCbits.TRISC7=0;     // CA7
    TRISCbits.TRISC6=0;     // CA3
    TRISCbits.TRISC5=0;     // CA9
    TRISCbits.TRISC4=0;     // CA1
    TRISCbits.TRISC3=0;     // DP
}

void disableGPIO() {
    ANSELA=0x00;
    ANSELB=0x00;
    
    /* Disable WPU */
    OPTION_REGbits.nWPUEN = 1;
    WPUB = 0x00;
    
    /* Disable all outputs */
    TRISB = 0xFF;
    TRISA = 0xFF;
    TRISC = 0xFF;
}