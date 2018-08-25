/*
 * File:        timer.h
 * Author:      Sam Faull
 * Description: 
 */

#ifndef TIMER_H
#define	TIMER_H

#include <stdbool.h>

#define TIMER_FREQ     1000                     // Frequency to check encoder pin
#define TIMER_PRD      ((FCY/256)/TIMER_FREQ)-1   // Period between ticks (1ms)

typedef unsigned long timer_t;

void timer_init(void);
void timer_tick(void);
void timer_start(timer_t *startTime);
bool timer_expired(timer_t startTime, timer_t expiryTime);

#endif	/* TIMER_H */

