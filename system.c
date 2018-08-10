/*
 * File:        system.c
 * Author:      Sam Faull
 * Description: System level functions
 */

#include <xc.h>
#include <stdint.h> 
#include <stdbool.h>       
#include "system.h"         

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{

#if 0
        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;

        /* When clock switch occurs switch to Primary Osc (HS, XT, EC) */
        __builtin_write_OSCCONH(0x02);  /* Set OSCCONH for clock switch */
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */
        while(OSCCONbits.COSC != 0b011);

        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, only if PLL is needed */
        /* while(OSCCONbits.LOCK != 1); */
        
 #endif

#if 0
        
        PLLFBD=41;              // PLL prescaler:  M = 43
        CLKDIVbits.PLLPOST = 0; // PLL postscaler: N2 = 2
        CLKDIVbits.PLLPRE = 0;  // PLL divisor:    N1 = 2
        OSCTUN=0;				// Tune FRC oscillator

        RCONbits.SWDTEN=0;      // Disable Watch Dog Timer

        // Clock switch to incorporate PLL
        __builtin_write_OSCCONH(0x01);				// Initiate Clock Switch to
                                                    // FRC with PLL (NOSC=0b001)
        __builtin_write_OSCCONL(0x01);				// Start clock switching
        while (OSCCONbits.COSC != 0b001);			// Wait for Clock switch to occur
        while(OSCCONbits.LOCK!=1) {};               // Wait for PLL to lock
#endif
        
        //setup internal clock for 80MHz/40MIPS using internal fast osc and PLL
        //7.37Mhz / 3 * 65 / 2 = 79.841666Mhz
        CLKDIVbits.PLLPRE = 1; //N1 = 3
        PLLFBD = 63; //M = 65
        CLKDIVbits.PLLPOST = 0; //N2 = 2
        while (!OSCCONbits.LOCK); //wait until PLL is locked
        
        AD1PCFGL = 0xFFFF; //set all pins digital
}

