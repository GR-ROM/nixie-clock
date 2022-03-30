
#include <pic16f1936.h>
#include "adc.h"

void start_adc(unsigned char ch) {
    ADCON0bits.CHS=ch;
    ADCON0bits.GO_nDONE=1;
    ADIF=0;
    ADIE=1;
}

void init_adc() {
    FVRCONbits.ADFVR=2; // Vref=2.048v
    FVRCONbits.TSEN=0;
    FVRCONbits.FVREN=1;
    
    ADCON1bits.ADCS=1;
    ADCON1bits.ADFM=0; 
    ADCON1bits.ADPREF=0b11; // Vref=VFR 
    ADCON1bits.ADCS=0b110;
    ADCON0bits.ADON=1;
    ADIE=0;
    ADIF=0;  
}
