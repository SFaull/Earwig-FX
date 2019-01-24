/*
 * File:        watchdog.h
 * Author:      Sam Faull
 * Description: 
 */

#ifndef WATCHDOG_H
#define	WATCHDOG_H

#include <xc.h> // include processor files - each processor file is guarded.  

void watchdog_init(void);
void watchdog_kick(void);
void watchdog_test(void);

#endif	/* WATCHDOG_H */

