#include <pic16f1936.h>

#include "buttons.h"

unsigned int modeButtonCounter;
unsigned int setButtonCounter;

unsigned char pressedButtons;
unsigned char downButtons;
unsigned char longDownButtons;
unsigned char lastModeState;
unsigned char lastSetState;

void init_buttons() {
    modeButtonCounter = 0;
    setButtonCounter = 0;
    pressedButtons = 0;
    downButtons = 0;
    longDownButtons = 0;
}

void checkButtons() {
    if (lastModeState != MODE_PORT) modeButtonCounter = 0;
    if (MODE_PORT == 0 && !(downButtons & MODE_BTN)) {
        if (modeButtonCounter++ == BTN_THRESHOLD) downButtons |= MODE_BTN;
        if (modeButtonCounter == BTN_LONG_THRESHOLD) longDownButtons |= MODE_BTN;
    }
    if (MODE_PORT == 1 && (downButtons & MODE_BTN)) {
        if (modeButtonCounter++ == BTN_THRESHOLD) {
            pressedButtons |= MODE_BTN;
            downButtons &= ~MODE_BTN;
            longDownButtons &= ~MODE_BTN;
        }
    }
    lastModeState = MODE_PORT;
    
    if (lastSetState != SET_PORT) setButtonCounter = 0;
    if (SET_PORT == 0 && !(downButtons & SET_BTN)) {
        if (setButtonCounter++ == BTN_THRESHOLD) downButtons |= SET_BTN;
        if (setButtonCounter == BTN_LONG_THRESHOLD) longDownButtons |= SET_BTN;
    }
    if (SET_PORT == 1 && (downButtons & SET_BTN)) {
        if (setButtonCounter++ == BTN_THRESHOLD) {
            pressedButtons |= SET_BTN;
            downButtons &= ~SET_BTN;
            longDownButtons &= ~SET_BTN;
        }
    }
    
    lastSetState = SET_PORT;
}

unsigned char getButtonsPressed() {
    return pressedButtons;
}

void clearState() {
    pressedButtons = 0;
}
