/*
 * File:        watchdog.h
 * Author:      Sam Faull
 * Description: WAtchdog
 */
#include "watchdog.h"
#include "system.h"

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

void watchdog_test(void)
{
    const int interval = 100;
    int time_in_ms = 0;
    
    printf("Starting WDT:\n");
    while(time_in_ms < 5000)
    {
        __delay_ms(interval);
        time_in_ms += interval;
        printf("%ims\n", time_in_ms);
    }
    printf("WDT failed\n");
}