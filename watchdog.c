/*
 * File:        watchdog.h
 * Author:      Sam Faull
 * Description: WAtchdog
 */
#include "watchdog.h"

void watchdog_init(void)
{
    // enable the watchdog, but not in debug mode
    #ifndef __DEBUG
        RCONbits.SWDTEN = 1;
    #endif
}

void watchdog_kick(void)
{
    #ifndef __DEBUG
        asm ("clrwdt"); // clear the watchdog counter
    #endif
}