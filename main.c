#include <xc.h>
#include <stdint.h>
#include "system.h"
#include "nixie.h"
#include "gpio.h"
#include "pwm.h"
#include "timer0.h"
#include "timer1.h"
#include "adc.h"

uint8_t hours;
uint8_t minutes;
uint8_t seconds;

extern int blinkCounter;
unsigned int target_load;
unsigned int load;
unsigned int pwm;
unsigned char volatile anode;
unsigned char volatile display[4];

void int2bcd(int num, uint8_t* bcd, int maxDig) {
    uint8_t d;
    uint8_t c=0;

    for (int i=num;i!=0;){
        i/=10;
        c++;
    }
    
    for (int i=0;i!=maxDig;i++){
        if (i>c) {
            d = 0; 
        } else {
            d = num % 10;
            num /= 10;   
        }
        bcd[maxDig-i-1]=d;
    }
}

void __interrupt() isr() {
    if (TMR0IF) {
        /* T = 1 / (8 000 000 / 4 / 32 / 256) = 4ms */
        driveNixie(&anode, &display[0]);
        start_adc(13);
        TMR0IF=0;
    }
    if (TMR1IF) {
        /* T = 1s */
        seconds++;
        if (seconds==60) {
            seconds=0;
            minutes++;
            if (minutes==60) {
                minutes=0;
                hours++;
                if (hours==24) {
                    hours=0;
                }
            }
        }
        TMR1=0xFFFF-32768;
        TMR1IF=0;
    }
    if (ADIF) {
        ADCON0bits.CHS=13;
        load = ADRESH;
        int delta = target_load - load; 
        
        if (delta + pwm > 0 && delta + pwm < 400) pwm+=delta;
        setDuty(pwm);  
        load = 0;
        ADIF = 0;
    }
}

void main(void) {
    configure_osc();
    init_gpio();
    /* Dynamic indication */
    init_timer0();
    start_timer0();
    /* HV converter control */
    init_adc();    
    init_pwm(0x7F);
    start_adc(13);
    setDuty(10);
    /* Clock timer */
    init_timer1(0xFFFF-32768);
    startTimer1();
    /* Allow interrupts */
    GIE=1;
    PEIE=1;
    
    anode = 0;
    load = 0;
    pwm = 10;
    blinkCounter = 0;
    /* Set nixie voltage to 180V DC */
    target_load = 155;
    
    /* Hard coded time */
    seconds= 9;
    minutes = 54;
    hours = 16;
    while (1) {
        int2bcd(minutes, &display[2], 2);
        int2bcd(hours, &display[0], 2);
        setBlinking(DIG1 | DIG2, &display[0]);
        __delay_ms(200);
    }
}
/**
 End of File
*/