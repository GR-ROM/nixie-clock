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

    void driveCathode(unsigned char digit);
    void driveNixie(unsigned char* anode, unsigned char* display) ;


#ifdef	__cplusplus
}
#endif

#endif	/* NIXIE_H */

