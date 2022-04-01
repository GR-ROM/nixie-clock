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
#define MODE_SET_HOUR 1
#define MODE_SET_MINUTES 2

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

static void checkPower(uint8_t adc) {
    if (adc > 127) engageFullPower();
    if (adc < 100) engagePowerDown();
}

void __interrupt() isr() {
    if (TMR1IF) {
        TMR1H=0x7F; // 0xFFFF - 0x8000 (32768) = 0x7FFF
        /* T = 1s */
        seconds++;
        DP = ~DP;
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
        TMR1IF=0;
    }
    if (TMR0IF) {
        /* T = 1 / (8 000 000 / 4 / 32 / 256) = 4ms */
        driveNixie(&anode, &display[0]);
        if (adcInput == ADC_INPUT_HV) start_adc(ADC_INPUT_HV);
        //if (adcInput == ADC_INPUT_VDD) start_adc(ADC_INPUT_VDD);
        TMR0IF=0;
    }
    if (ADIF) {
        load = ADRESH;
        int value = target_load - load;
        if (adcInput == ADC_INPUT_HV) {
            pwm += value;
            if (pwm > MAX_PWM) pwm = MAX_PWM;
            if (pwm < MIN_PWM) pwm = MIN_PWM;
            setDuty(pwm);
        }
        //if (adcInput == ADC_INPUT_VDD) checkPower(load);
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
    engagePowerDown();
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
    /* Set nixie voltage to 180V DC */
    target_load = DIM;
    adcInput = ADC_INPUT_HV;
    
    /* Hard coded time */
    seconds= 9;
    minutes = 06;
    hours = 14;
    
    unsigned char setHours;
    unsigned char setMinutes;
    while (1) {
        switch (mode) {
            case MODE_CLOCK:
                int2bcd(minutes, &back[2], 2);
                int2bcd(hours, &back[0], 2);
            break;
            case MODE_SET_HOUR:
                int2bcd(minutes, &back[2], 2);
                int2bcd(setHours, &back[0], 2);
            break;
            case MODE_SET_MINUTES:
                int2bcd(setMinutes, &back[2], 2);
                int2bcd(hours, &back[0], 2);
            break;
        }
        flush();
        /* User input handling */
        checkButtons();
        if (getButtonsPressed() & MODE_BTN) {
            if (mode++ == 3) mode = 0;         
            if (mode == 0) {
                clrBlinking(DIG1 | DIG2 | DIG3 | DIG4, &back[0]);
                hours = setHours;
                minutes = setMinutes;
            }
            if (mode == 1) { 
                setHours = hours;
                clrBlinking(DIG3 | DIG4, &back[0]);
                setBlinking(DIG1 | DIG2, &back[0]);
            }
            if (mode == 2) {
                setMinutes = minutes;
                clrBlinking(DIG1 | DIG2, &back[0]);
                setBlinking(DIG3 | DIG4, &back[0]);
            }
        }
        if (getButtonsPressed() & SET_BTN) {
            switch (mode) {
                case MODE_CLOCK: 
                    if (brightness++ == 2) brightness = 0; 
                    if (brightness == 0) {
                        target_load = BRIGHT;
                    }
                    if (brightness == 1) {
                        target_load = DIM;
                    }
                    break;
                case MODE_SET_HOUR: if (setHours++ == 24) setHours = 0; break;
                case MODE_SET_MINUTES: if (setMinutes++ == 60) setMinutes = 0; break;
            }
        }
        clearState();
        __delay_ms(1);
    }
}
/**
 End of File
*/