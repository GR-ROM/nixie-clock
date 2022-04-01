#include <xc.h>

void configureOscillatorFullPower() {
    // SCS INTOSC; SPLLEN disabled; IRCF 8MHz_HF; 
    OSCCON = 0x72;
    
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; 
    BORCON = 0x00;
}

void configureOscillatorPowerDown() {
    OSCCONbits.IRCF = 0b0101;
    OSCCONbits.SPLLEN = 0b0;
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; 
    BORCON = 0x00;
}
