#include <xc.h>
#include <stdint.h>
#include "system.h"
#include "nixie.h"
#include "gpio.h"
#include "pwm.h"
#include "timer0.h"
#include "timer1.h"
#include "adc.h"
#include "buttons.h"

#define MODE_CLOCK 0
#define MODE_SECONDS 1
#define MODE_SET_HOUR 2
#define MODE_SET_MINUTES 3

#define DIM 130
#define BRIGHT 150

#define ADC_INPUT_HV 13
#define ADC_INPUT_VDD 11

uint8_t hours;
uint8_t minutes;
uint8_t seconds;

extern int blinkCounter;
unsigned int target_load;
unsigned int load;
unsigned int pwm;
unsigned char volatile anode;
unsigned char volatile back[4];
unsigned char volatile display[4];
unsigned char mode;
unsigned char brightness;
unsigned char backup;
unsigned char adcInput;
unsigned int powerDownTimer;
uint24_t clock = 0;

static void engageFullPower();
static void engagePowerDown();

void int2bcd(int num, uint8_t* bcd, int maxDig) {
    uint8_t d;
    uint8_t c=0;

    for (int i=num;i!=0;){
        i/=10;
        c++;
    }
    
    for (int i = 0; i != maxDig; i++){
        if (i>c) {
            d = 0; 
        } else {
            d = num % 10;
            num /= 10;   
        }
        bcd[maxDig-i-1] &= 0xF0;
        bcd[maxDig-i-1] |= d;
    }
}

static void flush() {
    display[0]=back[0];
    display[1]=back[1];
    display[2]=back[2];
    display[3]=back[3];
}

static void checkPower() {
    if (VDD == 0) {
        //if (powerDownTimer-- == 0) {
            engagePowerDown();
            backup = 1;
            powerDownTimer = 100;
       //}
    }
        if (VDD == 1) {
        //if (powerDownTimer-- == 0) {
            engageFullPower();
            backup = 0;
            powerDownTimer = 100;
       //}
    }
}

void __interrupt() isr() {
    /* T = 1s */
    if (TMR1IF) {
        TMR1=0x7FFF; // 0xFFFF - 0x8000 (32768) = 0x7FFF
        clock++;
        TMR1IF=0;
        DP = ~DP;
    }
    /* T = 1 / (8 000 000 / 4 / 16 / 256) = 2ms */
    if (TMR0IF) {
        driveNixie(&anode, &display[0]);
        start_adc(ADC_INPUT_HV);
        TMR0IF=0;
    }
    if (ADIF) {
        load = ADRESH;
        if (load < target_load) pwm += 2;
        if (load > target_load) pwm -= 2;
        setDuty(pwm);
        load = 0;
        ADIF = 0;
    }
}

static void engageFullPower() {
    configureOscillatorFullPower();
    initGPIO();
    /* Dynamic indication */
    init_timer0();
    start_timer0();
    /* HV converter control */
    init_adc();    
    initPWM(TIMER2_PERIOD);
    start_adc(ADC_INPUT_HV);
    setDuty(MIN_PWM);
}

static void engagePowerDown() {
    configureOscillatorPowerDown();
    disableTimer0();
    disablePWM();
    disableGPIO();
}

void main(void) {
    engageFullPower();
    engageFullPower();
    init_buttons();
    /* Clock timer */
    init_timer1(0x7FFF);
    startTimer1();
    /* Allow interrupts */
    GIE=1;
    PEIE=1;
    
    brightness = 0;
    mode = MODE_CLOCK;
    anode = 0;
    load = 0;
    pwm = 10;
    blinkCounter = 0;
    backup = 0;
    powerDownTimer = 100;
    /* Set nixie voltage to 180V DC */
    target_load = DIM;
    adcInput = ADC_INPUT_HV;
    
    /* Hard coded time */
    seconds = 0;
    minutes = 0;
    hours = 0;
    
    unsigned char setHours;
    unsigned char setMinutes;
    while (1) {
        switch (mode) {
            case MODE_CLOCK:
                minutes = (clock % 3600) / 60;
                hours = clock / 3600;
                int2bcd(minutes, &back[2], 2);
                int2bcd(hours, &back[0], 2);
            break;
            case MODE_SECONDS:
                seconds = clock % 60;
                minutes = (clock % 3600) / 60;
                int2bcd(seconds, &back[2], 2);
                int2bcd(minutes, &back[0], 2);
            break;
            case MODE_SET_HOUR:
                int2bcd(setMinutes, &back[2], 2);
                int2bcd(setHours, &back[0], 2);
            break;
            case MODE_SET_MINUTES:
                int2bcd(setMinutes, &back[2], 2);
                int2bcd(setHours, &back[0], 2);
            break;
        }
        flush();
        /* User input handling */
        checkButtons();
        if (getButtonsPressed() & MODE_BTN) {
            if (++mode == 4) mode = 0;         
            if (mode == 0) { 
                if (setHours != hours && setMinutes != minutes) {
                    GIE = 0;
                    clock = (((long)setHours) * 3600) + ((long)setMinutes) * 60;
                    GIE = 1;
                }
                clrBlinking(DIG1 | DIG2 | DIG3 | DIG4, &back[0]);
            }
            if (mode == 2) { 
                setHours = hours;
                setMinutes = minutes;
                clrBlinking(DIG3 | DIG4, &back[0]);
                setBlinking(DIG1 | DIG2, &back[0]);
            }
            if (mode == 3) {
                clrBlinking(DIG1 | DIG2, &back[0]);
                setBlinking(DIG3 | DIG4, &back[0]);
            }
        }
        if (getButtonsPressed() & SET_BTN) {
            switch (mode) {
                case MODE_CLOCK: 
                    if (++brightness == 2) brightness = 0; 
                    if (brightness == 0) {
                        target_load = BRIGHT;
                    } else {
                        target_load = DIM;
                    }
                    break;
                case MODE_SET_HOUR: if (++setHours == 24) setHours = 0; break;
                case MODE_SET_MINUTES: if (++setMinutes == 60) setMinutes = 0; break;
            }
        }
        clearState();
        checkPower();
        __delay_ms(1);
    }
}
/**
 End of File
*/