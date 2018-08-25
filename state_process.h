/*
 * File:        state_process.h
 * Author:      Sam Faull
 * Description: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATE_PROCESS_H
#define	STATE_PROCESS_H

typedef enum {
    kStartup = 0,
    kHome = 1,
    kMainMenu = 2,
    kParamEdit = 3,
    kChainEdit = 4        
} state_t;

void state_process(void);

#endif	/* STATE_PROCESS_H */

