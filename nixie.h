/* 
 * File:   nixie.h
 * Author: exp10
 *
 * Created on March 30, 2022, 12:19 PM
 */

#ifndef NIXIE_H
#define	NIXIE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "gpio.h"
#include <xc.h>


#define DIG1 0x01
#define DIG2 0x02
#define DIG3 0x04
#define DIG4 0x08
    
void driveCathode(unsigned char digit);
void driveNixie(unsigned char* anode, unsigned char* display);
void setBlinking(unsigned char mask, unsigned char* display);
void clrBlinking(unsigned char mask, unsigned char* display);

#ifdef	__cplusplus
}
#endif

#endif	/* NIXIE_H */

