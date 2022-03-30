// PIC16F1936 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include "pwm.h"
#include "timer0.h"
#include "timer1.h"
#include "adc.h"

#define AN1 PORTAbits.RA0
#define AN2 PORTBbits.RB2
#define AN3 PORTBbits.RB3
#define AN4 PORTBbits.RB1
#define CA1 PORTCbits.RC4

uint8_t hours;
uint8_t minutes;
uint8_t seconds;

unsigned int target_load;
unsigned int load;
int pwm;
uint8_t anode;
uint8_t display[4];

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

void drive_cathode(uint8_t digit) {
    PORTAbits.RA1=0;
    PORTCbits.RC4=0;
    PORTAbits.RA2=0;
    PORTCbits.RC6=0;
    PORTAbits.RA4=0;
    PORTBbits.RB0=0;
    PORTAbits.RA5=0;
    PORTCbits.RC7=0;
    PORTAbits.RA3=0;
    PORTCbits.RC5=0;

    switch (digit) of: {
        case 0: PORTAbits.RA1=1; break;
        case 1: PORTCbits.RC4=1; break;
        case 2: PORTAbits.RA2=1; break;
        case 3: PORTCbits.RC6=1; break;
        case 4: PORTAbits.RA4=1; break;
        case 5: PORTBbits.RB0=1; break;
        case 6: PORTAbits.RA5=1; break;
        case 7: PORTCbits.RC7=1; break;
        case 8: PORTAbits.RA3=1; break;
        case 9: PORTCbits.RC5=1; break;
        default: break;
    }
}

void __interrupt() isr() {
    if (TMR0IF) {
        AN1 = 0;
        AN2 = 0;
        AN3 = 0;
        AN4 = 0;
        switch (anode) {
            case 0: AN1 = 1; break;
            case 1: AN2 = 1; break;
            case 2: AN3 = 1; break;
            case 3: AN4 = 1; break;
            default: break;
        }
        drive_cathode(display[anode++]);
        if (anode > 3) anode = 0;
        TMR0IF=0;
        start_adc(13);
    }
        if (TMR1IF) {
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
        load+=ADRESH;        
        if (load>target_load && pwm > 0) pwm--;
        if (load<target_load && pwm < 400) pwm++;
        setDuty(pwm);  
        load = 0;
        ADIF = 0;
    }
}

void configure_osc() {
      // SCS INTOSC; SPLLEN disabled; IRCF 8MHz_HF; 
    OSCCON = 0x72;
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; 
    BORCON = 0x00;
}

void init_gpio() {
    ANSELBbits.ANSB5=1;     // HV sense ADC
    TRISBbits.TRISB5=1;
    
    TRISCbits.TRISC0=1;
    TRISCbits.TRISC1=1;
    
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
}

void main(void) {
    configure_osc();
    ANSELA=0x00;
    ANSELB=0x00;
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
    /* Set HV to 200V DC */
    target_load = 155;
    
    /* Hard coded time */
    seconds= 9;
    minutes = 34;
    hours = 10;
    while (1) {
        int2bcd(minutes, &display[2], 2);
        int2bcd(hours, &display[0], 2);
    }
}
/**
 End of File
*/