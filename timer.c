/*
 * File:        timer.c
 * Author:      Sam Faull
 * Description: 
 */

#include <xc.h>
#include <stdbool.h>
#include "timer.h"
#include "system.h"

static void initTimer1(void);

static timer_t runTime;   // total number of ms since boot

void timer_init(void)
{
    runTime = 0;
    initTimer1();
}

static void initTimer1(void)   // Timer to periodically check encoder pin
{
    T1CONbits.TON = 0;      // Disable Timer
    T1CONbits.TCS = 0;      // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;    // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR1 = 0x00;            // Clear timer register
    PR1 = TIMER_PRD;           // Load the period value
    IPC0bits.T1IP = 0x01;   // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0;      // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Start Timer
}

void timer_tick(void)
{
    runTime++;
}

/* pass this function a pointer to an unsigned long to store the start time for the timer */
void timer_start(timer_t *startTime)
{
  *startTime = runTime;  // store the current time
}

/* call this function and pass it the variable which stores the timer start time and the desired expiry time 
   returns true if timer has expired */
bool timer_expired(timer_t startTime, timer_t expiryTime)
{
  if ( (runTime - startTime) >= expiryTime )
    return true;
  else
    return false;
}