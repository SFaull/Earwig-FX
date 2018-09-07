/*
 * File:        heartbeat.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef HEARTBEAT_H
#define	HEARTBEAT_H

#include <xc.h>
#include "system.h"

#define HEARTBEAT_LED      LATAbits.LATA10     // LED

void heartbeat_init(void);
void heartbeat_process(void);

#endif	/* HEARTBEAT_H */