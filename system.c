#include <xc.h>

void configure_osc() {
      // SCS INTOSC; SPLLEN disabled; IRCF 8MHz_HF; 
    OSCCON = 0x72;
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; 
    BORCON = 0x00;
}

