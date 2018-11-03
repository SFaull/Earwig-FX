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

#define DEBOUNCE_COUNT 5

#define ROTARY_L PORTCbits.RC0      // Rotary encoder pin L
#define ROTARY_R PORTCbits.RC1      // Rotary encoder pin R
#define BUTTON_OK PORTCbits.RC2      // Rotary encoder push button 
#define BUTTON_BACK PORTBbits.RB5      // back button

typedef enum {
    kNone = 0,
    kRotateCW = 1,
    kRotateCCW = 2,
    kOK = 3,
    kOKLong = 4,
    kBack = 5,
    kBackLong = 6
} control_t;

void navpanel_init(void);
int navpanel_getEncoderSteps(void);
void navpanel_setControl(control_t action);
control_t navpanel_getControl(void);
void navpanel_process(void);

#endif	/* NAVPANEL_H */

