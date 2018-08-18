/*
 * File:        navpanel.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NAVPANEL_H
#define	NAVPANEL_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define ENCFREQ     500                     // Frequency to check encoder pin
#define ENCPRD      ((FCY/256)/ENCFREQ)-1   // Period to check encoder pin

#define DEBOUNCE_COUNT 100

#define ROTARY_L PORTCbits.RC0      // Rotary encoder pin L
#define ROTARY_R PORTCbits.RC1      // Rotary encoder pin R
#define BUTTON_OK PORTCbits.RC2      // Rotary encoder push button 
#define BUTTON_BACK PORTBbits.RB5      // back button

typedef enum {
    kNone = 0,
    kRotateCW = 1,
    kRotateCCW = 2,
    kOK = 3,
    kBack = 4,
} control_t;

void navpanel_init(void);
control_t navpanel_pending_action(void);
void navpanel_process(void);

#endif	/* NAVPANEL_H */
