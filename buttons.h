/* 
 * File:   buttons.h
 * Author: exp10
 *
 * Created on 31 марта 2022 г., 10:37
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MODE_BTN 0x01
#define SET_BTN 0x02
    
#define MODE_PORT PORTBbits.RB7
#define SET_PORT PORTBbits.RB6
    
#define BTN_THRESHOLD 10
#define BTN_LONG_THRESHOLD 1000
    
unsigned char getButtonsPressed();
void init_buttons();
void checkButtons();
void clearState();


#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

