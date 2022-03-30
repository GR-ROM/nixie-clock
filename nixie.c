#include "gpio.h"
#include <xc.h>

/* Number to cathode mapping */
void driveCathode(unsigned char digit) {
    CA0=0;
    CA1=0;
    CA2=0;
    CA3=0;
    CA4=0;
    CA5=0;
    CA6=0;
    CA7=0;
    CA8=0;
    CA9=0;

    switch (digit) of: {
        case 0: CA0=1; break;
        case 1: CA1=1; break;
        case 2: CA2=1; break;
        case 3: CA3=1; break;
        case 4: CA4=1; break;
        case 5: CA5=1; break;
        case 6: CA6=1; break;
        case 7: CA7=1; break;
        case 8: CA8=1; break;
        case 9: CA9=1; break;
        default: break;
    }
}

/*
 * A routine responsive for dynamic indication, should 
 * be called at least 100 times per second
 * anode - points to the variable with a number of the current active anode
 * display - four bytes display buffer
 */
void driveNixie(unsigned char* anode, unsigned char* display) {
    /* Dynamic indication */
    AN1 = 0;
    AN2 = 0;
    AN3 = 0;
    AN4 = 0;
    switch (*anode) {
        case 0: AN1 = 1; break;
        case 1: AN2 = 1; break;
        case 2: AN3 = 1; break;
        case 3: AN4 = 1; break;
        default: break;
    }
    driveCathode(display[(*anode)++]);
    if (*anode > 3) *anode = 0;
}